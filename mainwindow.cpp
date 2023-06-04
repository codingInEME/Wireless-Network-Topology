
#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "dialog.h"
#include "pathwindow.h"
#include "experimentswindow.h"
#include "source.cpp"
#include <thread>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->routerInput->setValidator( new QIntValidator(1, 10000, this) );
    ui->gridInput->setValidator( new QIntValidator(1, 10000, this) );
    ui->rangeInput->setValidator( new QIntValidator(1, 10000, this) );
    ui->errorLabel->setStyleSheet("QLabel {color : red;}");
    ui->errorLabel->hide();
    srand(std::time(0));
}

MainWindow::~MainWindow()
{
    delete ui;
}




int routerSize =0, gridSize =0, range =0;
udg_generation grid;
topologyControl tc;
graph<router>net(false);
graph<router>xtc_net(false);

std::string net_graph_data;
std::string xtc_graph_data;

void MainWindow::on_generateButton_clicked()
{
    if(ui->routerInput->text().toInt() != routerSize || ui->gridInput->text().toInt() != gridSize || ui->rangeInput->text().toInt() != range){
        ui->errorLabel->setText("");
        ui->errorLabel->hide();

        routerSize = ui->routerInput->text().toInt();
        gridSize = ui->gridInput->text().toInt();
        range = ui->rangeInput->text().toInt();

        ui->routerOutput->setEnabled(true);
        ui->routerOutput->setText(QString::number(routerSize));
        ui->routerOutput->setEnabled(false);

        ui->gridOutput->setEnabled(true);
        ui->gridOutput->setText(QString::number(gridSize));
        ui->gridOutput->setEnabled(false);

        ui->rangeOutput->setEnabled(true);
        ui->rangeOutput->setText(QString::number(range));
        ui->rangeOutput->setEnabled(false);

        //std::thread thread_obj(generate, routerSize, gridSize,range);
        net = grid.generate(routerSize, gridSize,range);

    //ui->output_text->setText(QString::fromStdString(output));

        ui->udg_check->setEnabled(true);
        ui->udg_check->setCheckable(true);
        ui->udg_check->setCheckState(Qt::Checked);
        ui->udg_check->setEnabled(false);
        ui->apply_button->setEnabled(true);
        ui->show_net_button->setEnabled(true);
        ui->graph_pdf_button->setEnabled(true);

        ui->top_check->setEnabled(true);
        ui->top_check->setCheckState(Qt::Unchecked);
        ui->top_check->setEnabled(false);

        ui->pushButton->setEnabled(false);

        ui->graph_pdf_check->setEnabled(true);
        ui->graph_pdf_check->setCheckState(Qt::Unchecked);
        ui->graph_pdf_check->setEnabled(false);

        ui->xtc_pdf_button->setEnabled(false);

        ui->XTC_pdf_check->setEnabled(true);
        ui->XTC_pdf_check->setCheckState(Qt::Unchecked);
        ui->XTC_pdf_check->setEnabled(false);

        ui->open_graph_button->setEnabled(false);

        ui->open_xtc_button->setEnabled(false);

        ui->open_path_window->setEnabled(false);
    }else if(ui->routerInput->text().toInt() == 0 || ui->gridInput->text().toInt() == 0 || ui->rangeInput->text().toInt() == 0){
        ui->errorLabel->setText("Invalid Input!");
        ui->errorLabel->show();
    }
}


void MainWindow::on_pushButton_clicked()
{
    xtc_graph_data = xtc_net.display();
    Dialog* xtc_display = new Dialog(this,QString::fromStdString(xtc_graph_data));
    xtc_display->setModal(true);
    xtc_display->exec();
}


void MainWindow::on_show_net_button_clicked()
{
    net_graph_data = net.display();
    Dialog* net_display = new Dialog(this,QString::fromStdString(net_graph_data));
    net_display->setModal(true);
    net_display->exec();

}


void MainWindow::on_apply_button_clicked()
{
    auto start1 = std::chrono::high_resolution_clock::now();
    xtc_net = tc.XTC_protocol(net);
    auto stop1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
    std::cout << "\n" << duration1.count() << std::endl;
    ui->top_check->setCheckable(true);
    ui->top_check->setEnabled(true);
    ui->top_check->setCheckState(Qt::Checked);
    ui->top_check->setEnabled(false);

    ui->pushButton->setEnabled(true);
    ui->xtc_pdf_button->setEnabled(true);

    ui->open_path_window->setEnabled(true);
}



void MainWindow::on_graph_pdf_button_clicked()
{
    auto start3 = std::chrono::high_resolution_clock::now();
    generateFile(net, "graph");
    generateImage("graph");
    auto stop3 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
    std::cout << "\n"<< duration3.count() << std::endl;
    ui->graph_pdf_check->setEnabled(true);
    ui->graph_pdf_check->setCheckState(Qt::Checked);
    ui->graph_pdf_check->setEnabled(false);

    ui->open_graph_button->setEnabled(true);
}


void MainWindow::on_xtc_pdf_button_clicked()
{
    auto start2 = std::chrono::high_resolution_clock::now();
    generateFile(xtc_net, "graph_xtc");
    generateImage("graph_xtc");
    auto stop2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);
    std::cout << "\n" << duration2.count()<< std::endl;
    ui->XTC_pdf_check->setEnabled(true);
    ui->XTC_pdf_check->setCheckState(Qt::Checked);
    ui->XTC_pdf_check->setEnabled(false);

    ui->open_xtc_button->setEnabled(true);

}


void MainWindow::on_open_graph_button_clicked()
{
    openImage("graph.pdf");
}



void MainWindow::on_open_xtc_button_clicked()
{
    openImage("graph_xtc.pdf");
}


void MainWindow::on_open_path_window_clicked()
{
    PathWindow *pathwindow = new PathWindow(this,&net,&xtc_net);
    pathwindow->setModal(true);
    pathwindow->exec();
}


void MainWindow::on_experiments_button_clicked()
{
    std::string output = run_experiments(false);
    std::cout << "Abdullah Gay" << std::endl;
    ExperimentsWindow *experiments_window = new ExperimentsWindow(this, QString::fromStdString(output));
    experiments_window->setModal(true);
    experiments_window->exec();
}

