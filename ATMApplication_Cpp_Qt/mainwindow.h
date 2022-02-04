#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mngloginwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void hideWindow();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_mngButton_clicked();
    void on_userButton_clicked();

private:
    Ui::MainWindow *ui;
    mngLoginWindow *mnglogwindow;
};
#endif // MAINWINDOW_H
