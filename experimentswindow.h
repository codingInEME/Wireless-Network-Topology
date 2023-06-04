#ifndef EXPERIMENTSWINDOW_H
#define EXPERIMENTSWINDOW_H

#include <QDialog>

namespace Ui {
class ExperimentsWindow;
}

class ExperimentsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ExperimentsWindow(QWidget *parent = nullptr);
    ExperimentsWindow(QWidget *parent = nullptr, QString output = "default");
    ~ExperimentsWindow();

private:
    Ui::ExperimentsWindow *ui;
};

#endif // EXPERIMENTSWINDOW_H
