#include "userlogin.h"
#include "ui_userlogin.h"

userLogin::userLogin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::userLogin)
{
    ui->setupUi(this);
}

userLogin::~userLogin()
{
    delete ui;
}
