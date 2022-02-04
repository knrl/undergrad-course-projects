#include "moneyprocesswindow.h"
#include "ui_moneyprocesswindow.h"
#include "databasemanagement.h"
#include <QMessageBox>
#include <QDateTime>

moneyProcessWindow::moneyProcessWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::moneyProcessWindow)
{
    ui->setupUi(this);
}

moneyProcessWindow::moneyProcessWindow(int accID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::moneyProcessWindow)
{
    this->accID = accID;
    ui->setupUi(this);

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

void moneyProcessWindow::changeInterface(const int processID)
{
    switch(processID)
    {
        // deposit money
        case 0:
            this->processID = processID;
            ui->pushButton->setText("Yatır");
            break;

        // withdraw money
        case 1:
            this->processID = processID;
            ui->pushButton->setText("Çek");
            break;
        default:
            break;
    }
    updateInterface();
}

void moneyProcessWindow::updateInterface()
{
    // check balance by account id from database
    mModel.setQuery("SELECT bs.bakiye,bs.bakiyeID "
                    "FROM Hesap AS hsp "
                    "INNER JOIN BakiyeSorgulama AS bs "
                    "ON hsp.bakiyeID=bs.bakiyeID "
                    "WHERE hsp.hesapID="+ QString::number(this->accID) + ";");

    this->balance = mModel.data(mModel.index(0, 0)).toInt();
    ui->balanceLabel->setText(QString::number(this->balance) + " TL");
}

void moneyProcessWindow::on_pushButton_clicked()
{
    bool check = 0;
    // check balance by account id from database
    mModel.setQuery("SELECT bs.bakiye,bs.bakiyeID "
                    "FROM Hesap AS hsp "
                    "INNER JOIN BakiyeSorgulama AS bs "
                    "ON hsp.bakiyeID=bs.bakiyeID "
                    "WHERE hsp.hesapID="+ QString::number(this->accID) + ";");

    int bakiyeID = 0;
    // index(row,column)
    this->balance   = mModel.data(mModel.index(0, 0)).toInt();
    bakiyeID = mModel.data(mModel.index(0, 1)).toInt();

    // read user input
    int process_money = (ui->lineEdit->text()).toInt();

    switch(this->processID)
    {
        // deposit money
        case 0:
            this->balance += process_money;
            check = 1;
            break;
        case 1:
            if ((this->balance - process_money) >= 0)
            {
                this->balance -= process_money;
                check = 1;
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("You don't have enough money.");
                msgBox.exec();
            }
            break;
        default:
            break;
    }

    if (check)
    {
        mModel.setQuery("UPDATE BakiyeSorgulama "
                        "SET bakiye=" + QString::number(this->balance) + " "
                        "WHERE bakiyeID=" + QString::number(bakiyeID) + ";");

        int temp_balance  = this->balance;
        QString islemTipi = "yatırma";
        if (this->processID == 1)
        {
            temp_balance *= -1;
            islemTipi = "çekme";
        }

        // insert database logs
        mModel.setQuery("INSERT INTO ParaIslemleri "
                        "VALUES("+ QString::number(userID) +","+ QString::number(temp_balance) + ");");

        mModel.setQuery("SELECT paraIslemID "
                        "FROM ParaIslemleri "
                        "WHERE istemciID=" + QString::number(userID) + " "
                        "AND miktar=" + QString::number(temp_balance) + ";");

        QString paraIslemID = mModel.data(mModel.index(0, 0)).toString();

        mModel.setQuery("INSERT INTO IslemTipi "
                        "VALUES("+ paraIslemID +","+ QString::number(bakiyeID) +",'"+ islemTipi +"');");

        mModel.setQuery("SELECT islemTipiID "
                        "FROM IslemTipi "
                        "WHERE paraIslemID=" + paraIslemID + " "
                        "AND bakiyeID=" + QString::number(bakiyeID) + ";");

         QString islemTipiID = mModel.data(mModel.index(0, 0)).toString();


         QDateTime date = QDateTime::currentDateTime();
         QString tarih = date.toString("yyyy-MM-dd");

         mModel.setQuery("INSERT INTO Rapor "
                         "VALUES(" + QString::number(userID) + "," +
                         QString::number(accID) + "," +
                         islemTipiID + "," +
                         QString::number(this->balance) +  ",'" +
                         tarih + "');");
    }

    updateInterface();
}

void moneyProcessWindow::closeEvent(QCloseEvent *event)
{
    this->~moneyProcessWindow();
}


moneyProcessWindow::~moneyProcessWindow()
{
    delete dbConnection;
    delete ui;
}
