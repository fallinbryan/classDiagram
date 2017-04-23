#pragma once

#include <QMainWindow>
#include "uml_diagram.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
    QVector<Uml_diagram*> listOfDiagrams;
    QVector<QPoint> listOfDiagramOffsets;
    QPoint startPoint;

protected:
    void paintEvent(QPaintEvent*);
};


