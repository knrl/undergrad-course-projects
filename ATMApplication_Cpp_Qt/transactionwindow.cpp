#include "transactionwindow.h"
#include "ui_transactionwindow.h"
#include <QMessageBox>
#include <QDateTime>

transactionWindow::transactionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::transactionWindow)
{
    ui->setupUi(this);
}

transactionWindow::transactionWindow(int accID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::transactionWindow)
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

void transactionWindow::changeInterface()
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
    }

    ui->balanceLabel->setText(QString::number(userInfoMap.value(0).balance) + " TL");
}

void transactionWindow::updateInterface()
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
}

bool transactionWindow::checkWhetherAccountIsExist(int userID, int accID, userInfoStruct *a)
{
    // find unknown account's balanceID and balance with userID and accID
    mModel.setQuery("SELECT bs.bakiyeID,bs.bakiye "
                    "FROM Istemci AS ist "
                    "INNER JOIN Hesap AS hsp "
                    "ON hsp.istemciID=ist.istemciID "
                    "INNER JOIN BakiyeSorgulama as bs "
                    "ON bs.bakiyeID=hsp.bakiyeID "
                    "WHERE ist.istemciID=" + QString::number(userID) + " "
                    "AND hsp.hesapID=" + QString::number(accID) + ";");

    if (mModel.rowCount() == 1)
    {
        a->balance   = mModel.data(mModel.index(0, 1)).toInt();;
        a->balanceID = mModel.data(mModel.index(0, 0)).toInt();
        return 1;
    }
    return 0;
}

void transactionWindow::on_pushButton_clicked()
{
    bool check = 0;
    // user id
    int line_1_id = ui->lineEdit->text().toInt();
    // account id
    int line_2_id = ui->lineEdit_2->text().toInt();

    int acc_1_balance;
    userInfoStruct acc_2;
    if (checkWhetherAccountIsExist(line_1_id, line_2_id, &acc_2))
    {
        // amount of money
        int process_money = ui->lineEdit_3->text().toInt();
        acc_1_balance = userInfoMap.value(ui->comboBox->currentIndex()).balance;

        if (userInfoMap.value(ui->comboBox->currentIndex()).balance >= process_money)
        {
            check = 1;
            // update accounts' balances
            acc_1_balance -= process_money;
            acc_2.balance += process_money;
            mModel.setQuery("UPDATE BakiyeSorgulama "
                            "SET bakiye=" + QString::number(acc_1_balance) + " "
                            "WHERE bakiyeID="+ QString::number(userInfoMap.value(ui->comboBox->currentIndex()).balanceID) + ";");

            mModel.setQuery("UPDATE BakiyeSorgulama "
                            "SET bakiye=" + QString::number(acc_2.balance) + " "
                            "WHERE bakiyeID="+ QString::number(acc_2.balanceID) + ";");
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("You don't have enough money.");
            msgBox.exec();
        }
    }

    if (check)
    {
        // database logs for first account
        int temp_balance  = -1 * acc_1_balance;
        QString islemTipi = "gönderildi";

        // insert database logs
        mModel.setQuery("INSERT INTO ParaIslemleri "
                        "VALUES("+ QString::number(userID) +","+ QString::number(temp_balance) + ")");

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
         temp_balance  = acc_2.balance;
         islemTipi     = "alındı";

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
                         QString::number(acc_2.balanceID) + "," +
                         islemTipi + ");");

         mModel.setQuery("SELECT islemTipiID "
                         "FROM IslemTipi "
                         "WHERE paraIslemID=" + paraIslemID + " "
                         "AND bakiyeID=" + QString::number(acc_2.balanceID) + ";");

          islemTipiID = mModel.data(mModel.index(0, 0)).toString();

          mModel.setQuery("INSERT INTO Rapor "
                          "VALUES(" + QString::number(userID) + "," +
                          QString::number(accID) + "," +
                          islemTipiID + "," +
                          QString::number(acc_2.balance) +  "," +
                          tarih + ");");
    }

    updateInterface();
}

void transactionWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->balanceLabel->setText(QString::number(userInfoMap.value(index).balance) + " TL");
}

transactionWindow::~transactionWindow()
{
    delete dbConnection;
    delete ui;
}
