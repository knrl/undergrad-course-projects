#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideWindow()
{
    this->hide();
}

void MainWindow::on_mngButton_clicked()
{
    mnglogwindow = new mngLoginWindow(this);
    mnglogwindow->changeInterface(0);
    mnglogwindow->show();
}

void MainWindow::on_userButton_clicked()
{
    mnglogwindow = new mngLoginWindow(this);
    mnglogwindow->changeInterface(1);
    mnglogwindow->show();
}

