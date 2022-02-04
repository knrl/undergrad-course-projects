#include "mngloginwindow.h"
#include "ui_mngloginwindow.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QtSql>

#include <iostream>

mngLoginWindow::mngLoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mngLoginWindow)
{
    ui->setupUi(this);
}

mngLoginWindow::~mngLoginWindow()
{
    delete ui;
}

bool mngLoginWindow::databaseConnection(QString uid, QString password)
{
    // create database connection
    this->dbConnection = new databaseManagement("SQL SERVER","DESKTOP-FKCTEH2","ATMDB",uid,password,true);

    QString error;
    if (!dbConnection->openDatabase(&error))
    {
        // give connection error
        return 1;
    }
    return 0;
}

bool mngLoginWindow::checkUserNamePassword(QString uName, QString cardID)
{
    // I assume that the database connected at the background (server) which normally stored at the server side
    if (databaseConnection("DESKTOP-FKCTEH2\\kaane",""))
    {
        QMessageBox msgBox;
        msgBox.setText("Database connection error.");
        msgBox.exec();
    }

    // check person full user name and kartID
    QSqlQueryModel mModel;
    mModel.setQuery("SELECT TamIsim,kartID FROM Istemci");
    int rowCount = mModel.rowCount();
    QString fullUserName;
    int kartID;
    for (int i=0;i<rowCount;i++)
    {
        // index(row,column)
        fullUserName = mModel.data(mModel.index(i, 0)).toString();
        kartID       = mModel.data(mModel.index(i,1)).toInt();
        if (QString::compare(fullUserName, uName, Qt::CaseInsensitive) == 0
                && kartID == cardID.toInt())
        {
            return 0;
        }
    }
    return 0;
}

void mngLoginWindow::changeInterface(const int argID)
{
    switch(argID)
    {
        case 0:
            this->argID = argID;
            ui->label->setText("Yönetici ID");
            ui->label_2->setText("Şifresi");
            break;
        case 1:
            this->argID = argID;
            ui->label->setText("Hesap ID");
            ui->label_2->setText("Kart Şifresi");
            break;
        default:
            break;
    }
}

void mngLoginWindow::on_pushButton_clicked()
{
    int id_line           = (ui->lineEdit->text()).toInt();
    std::string pass_line = (ui->lineEdit_2->text()).toStdString();

    switch(this->argID)
    {
        case 0:
            // symbolic authentication for database administrator login
            // uid,password = "DESKTOP-FKCTEH2\\kaane","",
            if(databaseConnection(ui->lineEdit->text(),ui->lineEdit_2->text()))
            {
                QMessageBox msgBox;
                msgBox.setText("User ID or Password is not correct.");
                msgBox.exec();
            }

            hide();
            mngwindow = new mngWindow(this->dbConnection, this);
            mngwindow->changeInterface();
            mngwindow->show();

            break;
        case 1:
            // symbolic authentication for user login
            // check user login
            if(checkUserNamePassword(ui->lineEdit->text(),ui->lineEdit_2->text()))
            {
                // if login info is not correct
            }

            // close database connection
            delete dbConnection;

            hide();
            userwindow = new userWindow(id_line, this);
            userwindow->updateInterface();
            userwindow->show();

            break;
    }
}

