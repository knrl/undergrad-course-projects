#include "intertransactionwindow.h"
#include "ui_intertransactionwindow.h"
#include <QMessageBox>
#include <QDateTime>

interTransactionWindow::interTransactionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::interTransactionWindow)
{
    ui->setupUi(this);
}

interTransactionWindow::interTransactionWindow(int accID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::interTransactionWindow)
{
    ui->setupUi(this);
    this->accID = accID;

    if (dbConnection == nullptr)
    {
        // create database connection
        this->dbConnection = new databaseManagement("SQL SERVER","DESKTOP-FKCTEH2","ATMDB","DESKTOP-FKCTEH2\\kaane","",true);

        QString error;
        if (!dbConnection->openDatabase(&error))
        {
            // give connection error
        }

        // find user id according to account id
        mModel.setQuery("SELECT hsp.istemciID "
                        "FROM Hesap AS hsp "
                        "WHERE hsp.hesapID="+ QString::number(this->accID) + ";");

        userID = mModel.data(mModel.index(0, 0)).toInt();
    }
}

void interTransactionWindow::changeInterface()
{
    // find user's accounts from user id
    mModel.setQuery("SELECT hsp.hesapID,hsp.hesapTuru,bs.bakiye,bs.bakiyeID "
                    "FROM Istemci AS ist "
                    "INNER JOIN Hesap AS hsp "
                    "ON hsp.istemciID=ist.istemciID "
                    "INNER JOIN BakiyeSorgulama as bs "
                    "ON bs.bakiyeID=hsp.bakiyeID "
                    "WHERE ist.istemciID="+ QString::number(userID) + ";");

    // add all posible account info
    for (int i=0;i<mModel.rowCount();i++)
    {
        userInfoStruct a;
        a.accID     = mModel.data(mModel.index(i, 0)).toInt();
        a.balance   = mModel.data(mModel.index(i, 2)).toInt();
        a.balanceID = mModel.data(mModel.index(i, 3)).toInt();
        userInfoMap.insert(i,a);
        ui->comboBox->addItem(QString::number(mModel.data(mModel.index(i, 0)).toInt()) + " - " + mModel.data(mModel.index(i, 1)).toString());
        ui->comboBox_2->addItem(QString::number(mModel.data(mModel.index(i, 0)).toInt()) + " - " + mModel.data(mModel.index(i, 1)).toString());
    }

    ui->balanceLabel->setText(QString::number(userInfoMap.value(0).balance) + " TL");
    ui->balanceLabel_2->setText(QString::number(userInfoMap.value(0).balance) + " TL");
}

void interTransactionWindow::updateInterface()
{
    // find user's accounts from user id
    mModel.setQuery("SELECT hsp.hesapID,hsp.hesapTuru,bs.bakiye,bs.bakiyeID "
                    "FROM Istemci AS ist "
                    "INNER JOIN Hesap AS hsp "
                    "ON hsp.istemciID=ist.istemciID "
                    "INNER JOIN BakiyeSorgulama as bs "
                    "ON bs.bakiyeID=hsp.bakiyeID "
                    "WHERE ist.istemciID="+ QString::number(userID) + ";");

    // add all posible account info
    for (int i=0;i<mModel.rowCount();i++)
    {
        userInfoStruct a;
        a.accID     = mModel.data(mModel.index(i, 0)).toInt();
        a.balance   = mModel.data(mModel.index(i, 2)).toInt();
        a.balanceID = mModel.data(mModel.index(i, 3)).toInt();
        userInfoMap[i] = a;
    }

    ui->balanceLabel->setText(QString::number(userInfoMap.value(ui->comboBox->currentIndex()).balance) + " TL");
    ui->balanceLabel_2->setText(QString::number(userInfoMap.value(ui->comboBox_2->currentIndex()).balance) + " TL");
}

void interTransactionWindow::on_pushButton_clicked()
{
    bool check = 0;

    if (ui->comboBox->currentIndex() == ui->comboBox_2->currentIndex())
    {
        QMessageBox msgBox;
        msgBox.setText("You must choose different account from first account");
        msgBox.exec();
    }
    else
    {
        // read user input
        int process_money = (ui->lineEdit->text()).toInt();
        int acc_1_balance = userInfoMap.value(ui->comboBox->currentIndex()).balance;
        int acc_2_balance = userInfoMap.value(ui->comboBox_2->currentIndex()).balance;

        if ((acc_1_balance - process_money) >= 0)
        {
            check = 1;
            // update accounts' balances

            acc_1_balance -= process_money;
            acc_2_balance += process_money;
            mModel.setQuery("UPDATE BakiyeSorgulama "
                            "SET bakiye=" + QString::number(acc_1_balance) + " "
                            "WHERE bakiyeID="+ QString::number(userInfoMap.value(ui->comboBox->currentIndex()).balanceID) + ";");

            mModel.setQuery("UPDATE BakiyeSorgulama "
                            "SET bakiye=" + QString::number(acc_2_balance) + " "
                            "WHERE bakiyeID="+ QString::number(userInfoMap.value(ui->comboBox_2->currentIndex()).balanceID) + ";");
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("You don't have enough money.");
            msgBox.exec();
        }

        if (check)
        {
            // database logs for first account
            int temp_balance  = -1 * acc_1_balance;
            QString islemTipi = "gönderildi";

            // insert database logs
            mModel.setQuery("INSERT INTO ParaIslemleri "
                            "VALUES("+ QString::number(userID) +","+ QString::number(temp_balance) + ");");

            mModel.setQuery("SELECT paraIslemID "
                            "FROM ParaIslemleri "
                            "WHERE istemciID=" + QString::number(userID) + " "
                            "AND miktar=" + QString::number(temp_balance) + ";");

            QString paraIslemID = mModel.data(mModel.index(0, 0)).toString();

            mModel.setQuery("INSERT INTO IslemTipi "
                            "VALUES(" + paraIslemID + "," +
                            QString::number(userInfoMap.value(ui->comboBox->currentIndex()).balanceID) + "," +
                            islemTipi + ");");

            mModel.setQuery("SELECT islemTipiID "
                            "FROM IslemTipi "
                            "WHERE paraIslemID=" + paraIslemID + " "
                            "AND bakiyeID=" + QString::number(userInfoMap.value(ui->comboBox->currentIndex()).balanceID) + ";");

             QString islemTipiID = mModel.data(mModel.index(0, 0)).toString();

             QDateTime date = QDateTime::currentDateTime();
             QString tarih = date.toString("yyyy-MM-dd");

             mModel.setQuery("INSERT INTO Rapor "
                             "VALUES(" + QString::number(userID) + "," +
                             QString::number(accID) + "," +
                             islemTipiID + "," +
                             QString::number(acc_1_balance) +  "," +
                             tarih + ");");

             // database logs for second account
             temp_balance  = acc_2_balance;
             islemTipi     = "alındı";

             mModel.setQuery("UPDATE BakiyeSorgulama "
                             "SET bakiye=" + QString::number(temp_balance) + " "
                             "WHERE bakiyeID=" + QString::number(userInfoMap.value(ui->comboBox_2->currentIndex()).balanceID) + ";");

             // insert database logs
             mModel.setQuery("INSERT INTO ParaIslemleri "
                             "VALUES("+ QString::number(userID) +","+ QString::number(temp_balance) + ");");

             mModel.setQuery("SELECT paraIslemID "
                             "FROM ParaIslemleri "
                             "WHERE istemciID=" + QString::number(userID) + " "
                             "AND miktar=" + QString::number(temp_balance) + ";");

             paraIslemID = mModel.data(mModel.index(0, 0)).toString();

             mModel.setQuery("INSERT INTO IslemTipi "
                             "VALUES(" + paraIslemID + "," +
                             QString::number(userInfoMap.value(ui->comboBox_2->currentIndex()).balanceID) + "," +
                             islemTipi + ");");

             mModel.setQuery("SELECT islemTipiID "
                             "FROM IslemTipi "
                             "WHERE paraIslemID=" + paraIslemID + " "
                             "AND bakiyeID=" + QString::number(userInfoMap.value(ui->comboBox_2->currentIndex()).balanceID) + ";");

              islemTipiID = mModel.data(mModel.index(0, 0)).toString();

              mModel.setQuery("INSERT INTO Rapor "
                              "VALUES(" + QString::number(userID) + "," +
                              QString::number(accID) + "," +
                              islemTipiID + "," +
                              QString::number(acc_2_balance) +  "," +
                              tarih + ");");
        }

        updateInterface();
    }
}

void interTransactionWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->balanceLabel->setText(QString::number(userInfoMap.value(index).balance) + " TL");
}

void interTransactionWindow::on_comboBox_2_currentIndexChanged(int index)
{
    ui->balanceLabel_2->setText(QString::number(userInfoMap.value(index).balance) + " TL");
}

interTransactionWindow::~interTransactionWindow()
{
    delete ui;
}
