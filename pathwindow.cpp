#include "pathwindow.h"
#include "ui_pathwindow.h"

#include "path.h"

PathWindow::PathWindow(QWidget *parent, graph<router> *net, graph<router> *xtc) :
    QDialog(parent),
    ui(new Ui::PathWindow),
    net(net),
    xtc(xtc)
{
    ui->setupUi(this);
}

PathWindow::~PathWindow()
{
    delete ui;
}

void PathWindow::on_FindPath_button_clicked()
{

    path a;
    std::string source = ui->source_name->text().toStdString();
    std::string destination = ui->destination_name->text().toStdString();
    std::string path_out;
    std::vector<node<router>> path_vec;
    if(ui->with_button->isChecked()){

        path_vec = a.find_path(*xtc,source,destination);
        std::cout << "Abdullah Gay" << std::endl;
        path_out = path::display_path(path_vec);
    }
    else{
        path_vec = a.find_path(*net,source,destination);
        path_out = path::display_path(path_vec);
    }

    ui->output_text->setText(QString::fromStdString(path_out));
}

