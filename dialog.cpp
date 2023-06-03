#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(QWidget *parent, QString message) :
    QDialog(parent),
    ui(new Ui::Dialog)
    //output(out)
{
    ui->setupUi(this);
    ui->out_text->setText(message);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_Dialog_accepted()
{
    //->out_text->setText(QString::fromStdString(MainWindow::output));
}

