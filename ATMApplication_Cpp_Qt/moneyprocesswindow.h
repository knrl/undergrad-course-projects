#ifndef MONEYPROCESSWINDOW_H
#define MONEYPROCESSWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "databasemanagement.h"

namespace Ui {
class moneyProcessWindow;
}

class moneyProcessWindow : public QMainWindow
{
    Q_OBJECT

public:
    int processID;
    explicit moneyProcessWindow(QWidget *parent = nullptr);
    explicit moneyProcessWindow(int accID, QWidget *parent = nullptr);
    void changeInterface(const int processID);
    void updateInterface();
    void closeEvent(QCloseEvent *event);
    ~moneyProcessWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::moneyProcessWindow *ui;
    databaseManagement *dbConnection = nullptr;
    QSqlQueryModel mModel;
    int accID;
    int balance;
    int userID;
};

#endif // MONEYPROCESSWINDOW_H
