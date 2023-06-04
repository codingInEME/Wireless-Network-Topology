#ifndef PATHWINDOW_H
#define PATHWINDOW_H

#include <QDialog>
//#include "source.cpp"
#include"router.h"
#include"graph.h"

namespace Ui {
class PathWindow;
}

class PathWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PathWindow(QWidget *parent = nullptr);
    PathWindow(QWidget *parent = nullptr, graph<router> *net = nullptr , graph<router> *xtc = nullptr, int lower = 0, int upper = 0);
    ~PathWindow();

private slots:
    void on_FindPath_button_clicked();

    void on_generate_path_graph_button_clicked();

    void on_show_button_clicked();

private:
    Ui::PathWindow *ui;
    graph<router> *net;
    graph<router> *xtc;
    int lower;
    int upper;
};

#endif // PATHWINDOW_H
