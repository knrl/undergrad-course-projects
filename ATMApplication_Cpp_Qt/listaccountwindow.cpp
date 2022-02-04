#include "listaccountwindow.h"
#include "ui_listaccountwindow.h"

listAccountWindow::listAccountWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::listAccountWindow)
{
    ui->setupUi(this);
}

listAccountWindow::listAccountWindow(int accID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::listAccountWindow)
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

void listAccountWindow::changeInterface()
{
    mModel.setQuery("SELECT hsp.hesapID,hsp.hesapTuru,hsp.hesapAcilisTarihi,bs.bakiye "
                    "FROM Hesap AS hsp "
                    "INNER JOIN BakiyeSorgulama AS bs "
                    "ON hsp.hesapID=bs.hesapID "
                    "WHERE hsp.istemciID=" + QString::number(this->userID) + ";");

    ui->tableView->setModel(&mModel);
}

listAccountWindow::~listAccountWindow()
{
    delete dbConnection;
    delete ui;
}
