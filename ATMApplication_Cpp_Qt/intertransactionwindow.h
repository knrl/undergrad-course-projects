#ifndef INTERTRANSACTIONWINDOW_H
#define INTERTRANSACTIONWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "databasemanagement.h"

namespace Ui {
class interTransactionWindow;
}

class interTransactionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit interTransactionWindow(QWidget *parent = nullptr);
    explicit interTransactionWindow(int accID, QWidget *parent = nullptr);
    void updateInterface();
    void changeInterface();
    ~interTransactionWindow();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::interTransactionWindow *ui;
    databaseManagement *dbConnection = nullptr;
    QSqlQueryModel mModel;
    int accID;
    int userID;

    typedef struct
    {
        int accID;
        int balanceID;
        int balance;
    } userInfoStruct;

    QMap<int,userInfoStruct> userInfoMap;
};

#endif // INTERTRANSACTIONWINDOW_H
