#include "newaccountwindow.h"
#include "ui_newaccountwindow.h"

newAccountWindow::newAccountWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::newAccountWindow)
{
    ui->setupUi(this);
}

newAccountWindow::newAccountWindow(int accID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::newAccountWindow)
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

void newAccountWindow::changeInterface()
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
        ui->comboBox->addItem(QString::number(mModel.data(mModel.index(i, 0)).toInt()) +
                              " - " + mModel.data(mModel.index(i, 1)).toString() +
                              " Bakiye: " + mModel.data(mModel.index(i, 2)).toString());
    }
}

void newAccountWindow::on_pushButton_clicked()
{
   mModel.setQuery("SELECT hsp.hesapID,hsp.hesapTuru,ist.tamIsim,isl.islemTipi,prs.miktar,rp.raporTarih "
                   "FROM Rapor AS rp "
                   "INNER JOIN Hesap AS hsp "
                   "ON rp.hesapID=hsp.hesapID "
                   "INNER JOIN Istemci AS ist "
                   "ON hsp.istemciID=ist.istemciID "
                   "INNER JOIN ParaIslemleri AS prs "
                   "ON ist.istemciID=prs.istemciID "
                   "INNER JOIN islemTipi AS isl "
                   "ON rp.islemTipiID=isl.islemTipiID;");

    ui->tableView->setModel(&mModel);
}

void newAccountWindow::on_comboBox_currentIndexChanged(int index)
{

}

newAccountWindow::~newAccountWindow()
{
    delete dbConnection;
    delete ui;
}
