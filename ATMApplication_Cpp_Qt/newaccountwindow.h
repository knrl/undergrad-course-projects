#ifndef NEWACCOUNTWINDOW_H
#define NEWACCOUNTWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "databasemanagement.h"

namespace Ui {
class newAccountWindow;
}

class newAccountWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit newAccountWindow(QWidget *parent = nullptr);
    explicit newAccountWindow(int accID, QWidget *parent = nullptr);
    void changeInterface();
    ~newAccountWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_pushButton_clicked();

private:
    Ui::newAccountWindow *ui;
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

#endif // NEWACCOUNTWINDOW_H
