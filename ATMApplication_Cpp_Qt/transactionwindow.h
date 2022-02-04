#ifndef TRANSACTIONWINDOW_H
#define TRANSACTIONWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "databasemanagement.h"

namespace Ui {
class transactionWindow;
}

class transactionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit transactionWindow(QWidget *parent = nullptr);
    explicit transactionWindow(int accID, QWidget *parent = nullptr);
    void updateInterface();
    void changeInterface();
    ~transactionWindow();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::transactionWindow *ui;
    databaseManagement *dbConnection = nullptr;
    QSqlQueryModel mModel;
    int userID;
    int accID;

    typedef struct
    {
        int accID;
        int balanceID;
        int balance;
    } userInfoStruct;

    QMap<int,userInfoStruct> userInfoMap;

    bool checkWhetherAccountIsExist(int userID, int accID, userInfoStruct *a);
};

#endif // TRANSACTIONWINDOW_H
