#pragma once
#include <QWidget>
#include <QString>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QPainter>
#include <QFile>




enum  parseFlags {
    processingClass =  1,
    processingStruct = 2,
    processingFunc =   4,
    processingEnum =   8
};

enum protectionFlags {
    privateMember = 1,
    protectedMember = 2,
    publicMember = 4
};


class Uml_diagram
{
private:
    unsigned short processingState;
    unsigned short memberVisibilityState;

    QWidget* parent;
    QPoint topLeft;
    QColor bgColor;
    QColor titleColor;
    QColor privateTextColor;
    QColor publicTextColor;
    QColor protectedTextColor;

    QString title;
    QVector<QString> privateAttributes;
    QVector<QString> protectedAttributes;
    QVector<QString> publicAttributes;
    QVector<QString> privateFunctions;
    QVector<QString> protectedFunctions;
    QVector<QString> publicFunctions;

    void parseFile(QFile* file);

public:
    Uml_diagram(QWidget*, QFile* file, int top, int left);
    ~Uml_diagram();
    void paintDiagram(QPainter* painter);
    void moveDiagram(QPoint point);
};

