#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString qfileName;
    qfileName = QFileDialog::getOpenFileName(this,tr("Open File"),
                            QCoreApplication::applicationDirPath(),
                            tr("Headers (*.h)"));
    QFile* file = new QFile(qfileName);
    Uml_diagram diagram(this,file);
    QPainter* painter(ui->centralWidget);
    diagram.paintDiagram(painter);
    //listOfDiagrams.push_back( diagram );
    file->close();
    delete file;
    delete painter;
}
