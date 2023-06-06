#include "experimentswindow.h"
#include "ui_experimentswindow.h"

ExperimentsWindow::ExperimentsWindow(QWidget *parent, QString output) :
    QDialog(parent),
    ui(new Ui::ExperimentsWindow)
{
    ui->setupUi(this);
    ui->experiments_Output->setText(output);
}

ExperimentsWindow::~ExperimentsWindow()
{
    delete ui;
}
