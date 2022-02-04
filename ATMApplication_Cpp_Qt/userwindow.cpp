#include "userwindow.h"
#include "ui_userwindow.h"

userWindow::userWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::userWindow)
{
    ui->setupUi(this);
}

userWindow::userWindow(int accID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::userWindow)
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

    }
}

void userWindow::updateInterface()
{
    // check balance by account id from database
    mModel.setQuery("SELECT bs.bakiye,bs.istemciID "
                    "FROM Hesap AS hsp "
                    "INNER JOIN BakiyeSorgulama AS bs "
                    "ON hsp.bakiyeID=bs.bakiyeID "
                    "WHERE hsp.hesapID=" + QString::number(this->accID) + " ;");

    this->balance = mModel.data(mModel.index(0, 0)).toInt();
    int istemciID = mModel.data(mModel.index(0, 1)).toInt();
    ui->balanceLabel->setText(QString::number(this->balance) + " TL");

    mModel.setQuery("SELECT tamIsim,soyad FROM Istemci "
                    "WHERE istemciID=" + QString::number(istemciID) + " ;");
    QString fullUserName = mModel.data(mModel.index(0, 0)).toString();
    QString lastUserName = mModel.data(mModel.index(0, 1)).toString();
    ui->accountOwnerLabel->setText(fullUserName + " " + lastUserName);
}

// deposit money
void userWindow::on_pushButton_6_clicked()
{
    this->updateInterface();
    moneyprocesswindow = new moneyProcessWindow(this->accID, this);
    moneyprocesswindow->changeInterface(0);
    moneyprocesswindow->show();
}

// withdraw money
void userWindow::on_pushButton_7_clicked()
{
    this->updateInterface();
    moneyprocesswindow = new moneyProcessWindow(this->accID, this);
    moneyprocesswindow->changeInterface(1);
    moneyprocesswindow->show();
}

// transaction to another account
void userWindow::on_anotherAccountButton_clicked()
{
    this->updateInterface();
    transactionwindow = new transactionWindow(this->accID, this);
    transactionwindow->changeInterface();
    transactionwindow->show();
}

// create a new account
void userWindow::on_newAccountButton_clicked()
{
    this->updateInterface();
    newaccountwindow = new newAccountWindow(this->accID, this);
    newaccountwindow->changeInterface();
    newaccountwindow->show();
}

// list user's all accounts
void userWindow::on_listAccountButton_clicked()
{
    this->updateInterface();
    listaccountwindow = new listAccountWindow(this->accID, this);
    listaccountwindow->changeInterface();
    listaccountwindow->show();
}

void userWindow::on_interAccountButton_clicked()
{
    this->updateInterface();
    intertransactionwindow = new interTransactionWindow(this->accID, this);
    intertransactionwindow->changeInterface();
    intertransactionwindow->show();
}

userWindow::~userWindow()
{
    delete dbConnection;
    delete ui;
}
