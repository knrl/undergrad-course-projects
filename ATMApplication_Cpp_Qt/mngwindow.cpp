#include "mngwindow.h"
#include "ui_mngwindow.h"
#include <QMessageBox>

mngWindow::mngWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mngWindow)
{
    ui->setupUi(this);

    // init state of windowForm
    ui->matchRadioButton->setChecked(1);
    ui->label_4->setText("Eşitliğinde");
    ui->lineEdit_2->hide();
    ui->label_5->hide();
}

mngWindow::mngWindow(databaseManagement *dbConnection, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mngWindow)
{
    ui->setupUi(this);
    this->dbConnection = dbConnection;

    // init state of window
    ui->radioButton->setChecked(1);
    ui->noFilterRadioButton->setChecked(1);
    ui->label_4->setText("Eşitliğinde");
    ui->lineEdit_2->hide();
    ui->label_5->hide();

    if (dbConnection == nullptr)
    {
        // create database connection
        this->dbConnection = new databaseManagement("SQL SERVER","DESKTOP-FKCTEH2","ATMDB","DESKTOP-FKCTEH2\\kaane","",true);

        QString error;
        if (!dbConnection->openDatabase(&error))
        {
            // give connection error
        }
    }
}

void mngWindow::changeInterface()
{
    ui->comboBox_2->clear();
    mModel.setQuery("SELECT * FROM Istemci");
    QString columnName;
    for (int i=0;i<mModel.columnCount();i++)
    {
        columnName = mModel.headerData(i, Qt::Horizontal).toString();
        ui->comboBox_2->addItem(columnName);
    }
}

void mngWindow::on_matchRadioButton_clicked()
{
    ui->label_4->setText("eşitliğinde");
    ui->label_5->hide();
    ui->lineEdit_2->hide();
}

void mngWindow::on_rangeRadioButton_clicked()
{
    ui->label_4->setText("ve");
    ui->label_5->show();
    ui->lineEdit_2->show();
}

void mngWindow::on_addFilterButton_clicked()
{
    bool check = 1;
    for (int i=0;i<row_filter;i++)
    {
        if (QString::compare(ui->listWidget_2->item(i)->text(), ui->comboBox_2->currentText(), Qt::CaseInsensitive) == 0)
        {
            check = 0;
        }
    }

    if (check)
    {
        ui->listWidget_2->insertItem(row_filter,ui->comboBox_2->currentText());
        row_filter++;

        QString cond_query = "";
        if (ui->matchRadioButton->isChecked())
        {
            cond_query = ui->comboBox_2->currentText() +"="+ ui->lineEdit->text();
            cond_query_list.push_back(cond_query);

            if (ui->radioButton->isChecked())
            {
                cond_relation_query_list.push_back("AND");
            }
            else
            {
                cond_relation_query_list.push_back("OR");
            }
        }
        else if (ui->rangeRadioButton->isChecked())
        {
            cond_query = "(" +ui->comboBox_2->currentText() +" BETWEEN "+ ui->lineEdit->text() +" AND "+ ui->lineEdit_2->text()+ ")";
            cond_query_list.push_back(cond_query);

            if (ui->radioButton->isChecked())
            {
                cond_relation_query_list.push_back("AND");
            }
            else
            {
                cond_relation_query_list.push_back("OR");
            }
        }

        if (QString::compare(column_names, "", Qt::CaseInsensitive) == 0)
        {
            column_names = ui->comboBox_2->currentText();
        }
        else
        {
            column_names += "," + ui->comboBox_2->currentText();
        }
    }
}

void mngWindow::on_resetButton_clicked()
{
    resetAllThings();
}

void mngWindow::resetAllThings()
{
    row_filter = 0;

    while(ui->listWidget_2->count() > 0)
    {
        ui->listWidget_2->takeItem(0);
    }

    for (int i=0; i<cond_query_list.count(); i++)
    {
        cond_query_list.takeAt(0);
    }

    for (int i=0; i<cond_relation_query_list.count(); i++)
    {
        cond_relation_query_list.takeAt(0);
    }
    column_names = "";
}

void mngWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->comboBox_2->clear();
    QString query_text = "SELECT * FROM " + ui->comboBox->currentText();
    mModel.setQuery(query_text);
    QString columnName;
    for (int i=0;i<mModel.columnCount();i++)
    {
        columnName = mModel.headerData(i, Qt::Horizontal).toString();
        ui->comboBox_2->addItem(columnName);
    }
    ui->label_7->setText(query_text);
    resetAllThings();
}

// show
void mngWindow::on_resetButton_2_clicked()
{
    QString last_cond_query = "";
    if (cond_query_list.size() > 0)
    {
        last_cond_query = cond_query_list[0];
        for (int i=1;i<cond_query_list.count();i++)
        {
            last_cond_query += " " +cond_relation_query_list[i]+ " " +cond_query_list[i];
        }
    }

    QString query_text = "SELECT";

    if (column_names.length() > 1)
    {
        query_text += " " +column_names;
    }
    else
    {
        query_text +=" *";
    }

    query_text += " FROM " + ui->comboBox->currentText();

    if (last_cond_query.length() > 1)
            query_text += " WHERE " +last_cond_query;

    mModel.setQuery(query_text);
    ui->tableView->setModel(&mModel);
    ui->label_7->setText(query_text);
}

void mngWindow::on_comboBox_2_currentIndexChanged(int index)
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
}

void mngWindow::closeEvent(QCloseEvent *event)
{
    this->~mngWindow();
}

mngWindow::~mngWindow()
{
    delete dbConnection;
    delete ui;
}
