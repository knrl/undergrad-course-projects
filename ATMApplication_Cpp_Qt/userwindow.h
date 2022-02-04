#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "moneyprocesswindow.h"
#include "transactionwindow.h"
#include "intertransactionwindow.h"
#include "newaccountwindow.h"
#include "listaccountwindow.h"
#include "databasemanagement.h"

namespace Ui {
class userWindow;
}

class userWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit userWindow(QWidget *parent = nullptr);
    explicit userWindow(int accID, QWidget *parent = nullptr);
    void updateInterface();
    ~userWindow();

private slots:
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_anotherAccountButton_clicked();
    void on_newAccountButton_clicked();
    void on_listAccountButton_clicked();
    void on_interAccountButton_clicked();

private:
    Ui::userWindow *ui;
    moneyProcessWindow *moneyprocesswindow;
    transactionWindow *transactionwindow;
    interTransactionWindow *intertransactionwindow;
    newAccountWindow *newaccountwindow;
    listAccountWindow *listaccountwindow;
    QSqlQueryModel mModel;
    databaseManagement *dbConnection = nullptr;
    int accID;
    int balance;
};

#endif // USERWINDOW_H
