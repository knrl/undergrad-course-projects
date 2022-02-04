#ifndef MNGWINDOW_H
#define MNGWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSqlQueryModel>
#include "databasemanagement.h"

namespace Ui {
class mngWindow;
}

class mngWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mngWindow(QWidget *parent = nullptr);
    explicit mngWindow(databaseManagement *dbConnection, QWidget *parent = nullptr);
    void changeInterface();
    void resetAllThings();
    void closeEvent(QCloseEvent *event);
    ~mngWindow();

private slots:
    void on_matchRadioButton_clicked();
    void on_rangeRadioButton_clicked();
    void on_resetButton_2_clicked();
    void on_addFilterButton_clicked();
    void on_resetButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::mngWindow *ui;
    databaseManagement *dbConnection = nullptr;
    QSqlQueryModel mModel;
    int row_table = 0, row_filter = 0;
    QString column_names = "";
    QList<QString> cond_relation_query_list;
    QList<QString> cond_query_list;
};

#endif // MNGWINDOW_H
