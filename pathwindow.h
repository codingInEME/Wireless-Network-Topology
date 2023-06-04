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
    PathWindow(QWidget *parent = nullptr, graph<router> *net = nullptr , graph<router> *xtc = nullptr);
    ~PathWindow();

private slots:
    void on_FindPath_button_clicked();

private:
    Ui::PathWindow *ui;
    graph<router> *net;
    graph<router> *xtc;
};

#endif // PATHWINDOW_H
