#ifndef MNGLOGINWINDOW_H
#define MNGLOGINWINDOW_H

#include <QMainWindow>
#include "mngwindow.h"
#include "userwindow.h"
#include "databasemanagement.h"

namespace Ui {
class mngLoginWindow;
}

class mngLoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    int argID;
    void changeInterface(const int argID);
    bool databaseConnection(QString uid, QString password);
    bool checkUserNamePassword(QString uName, QString cardID);
    explicit mngLoginWindow(QWidget *parent = nullptr);
    explicit mngLoginWindow(databaseManagement *dbConnection, QWidget *parent = nullptr);
    ~mngLoginWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::mngLoginWindow *ui;
    mngWindow *mngwindow;
    userWindow *userwindow;
    databaseManagement *dbConnection = nullptr;
};

#endif // MNGLOGINWINDOW_H
