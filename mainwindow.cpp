#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startPoint.rx() = 0;
    startPoint.ry() = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    Uml_diagram* garabageTruck;
    for(int i = 0; i < listOfDiagrams.length();i++) {
        garabageTruck = listOfDiagrams[i];
        delete garabageTruck;
        garabageTruck = nullptr;
    }
}

void MainWindow::on_actionOpen_triggered()
{
     startPoint.rx() += 200;
    QString qfileName;
    qfileName = QFileDialog::getOpenFileName(this,tr("Open File"),
                            QCoreApplication::applicationDirPath(),
                            tr("Headers (*.h)"));
    QFile* file = new QFile(qfileName);

    this->setWindowTitle(qfileName);

    Uml_diagram* diagram = new Uml_diagram(this,file,startPoint.y(),startPoint.x());
    listOfDiagrams.push_back(diagram);

    //listOfDiagrams.push_back( diagram );
    file->close();
    delete file;

}

void MainWindow::paintEvent(QPaintEvent *)
{

    QPainter* painter = new QPainter(this);
    QBrush brush = painter->brush();
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawRect(0,0,this->geometry().right(),this->geometry().bottom());


    for(int i = 0; i < listOfDiagrams.length(); i++) {
        listOfDiagrams[i]->paintDiagram(painter);
    }
     delete painter;
}
