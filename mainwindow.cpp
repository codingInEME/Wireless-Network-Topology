
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "source.cpp"
#include "dialog.h"

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




int routerSize, gridSize;
udg_generation grid;
topologyControl tc;
graph<router>net(false);
graph<router>xtc_net(false);

std::string net_graph_data;
std::string xtc_graph_data;

void MainWindow::on_generateButton_clicked()
{
    routerSize = ui->routerInput->text().toInt();
    gridSize = ui->gridInput->text().toInt();
    ui->routerOutput->setText(QString::number(routerSize));
    ui->gridOutput->setText(QString::number(gridSize));

    net = grid.generate(routerSize, gridSize);

    //ui->output_text->setText(QString::fromStdString(output));
    ui->udg_check->setEnabled(true);
    ui->udg_check->setCheckable(true);
    ui->udg_check->setCheckState(Qt::Checked);
    ui->udg_check->setEnabled(false);
    ui->apply_button->setEnabled(true);
    ui->show_net_button->setEnabled(true);
    ui->graph_pdf_button->setEnabled(true);

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
}

