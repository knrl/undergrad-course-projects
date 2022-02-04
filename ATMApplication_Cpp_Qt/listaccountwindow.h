#ifndef LISTACCOUNTWINDOW_H
#define LISTACCOUNTWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "databasemanagement.h"

namespace Ui {
class listAccountWindow;
}

class listAccountWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit listAccountWindow(QWidget *parent = nullptr);
    explicit listAccountWindow(int accID, QWidget *parent = nullptr);
    void changeInterface();
    ~listAccountWindow();

private:
    Ui::listAccountWindow *ui;
    databaseManagement *dbConnection = nullptr;
    QSqlQueryModel mModel;
    int accID;
    int userID;
};

#endif // LISTACCOUNTWINDOW_H
