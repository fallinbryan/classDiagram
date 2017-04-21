#pragma once

#include "uml_diagram.h"

#include <QTextStream>



void Uml_diagram::parseFile(QFile *file)
{
    //todo store this comment as a pop-up tool tip when hovering over a name
    QString comment;
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(line.startsWith("//") || line.startsWith("/*") || line.endsWith("*/"))  {
            comment.append(line);
        }
        if(line.startsWith("class") || line.startsWith("struct")) {
            if(line.startsWith("class")) {
                memberVisibilityState &= privateMember;
                processingState &=  processingClass;
            }
            if(line.startsWith("struct")) {
                memberVisibilityState &= publicMember;
                processingState &= processingStruct;
            }
            QStringList splitLine = line.split(' ',QString::SkipEmptyParts);
            this->title = splitLine[1];
            if(splitLine[2] == ":") {
                //parent = splitLine[3]  <---todo  figure that one out
            }

        }
        if( (processingState & processingClass) == processingClass ||
                (processingState & processingStruct) == processingStruct) {
            QStringList splitLine = line.split(' ',QString::SkipEmptyParts);
            if(splitLine[0] == "private") {
                memberVisibilityState = 0;
                memberVisibilityState &= privateMember;
            }else if (splitLine[0] == "protected") {
                memberVisibilityState = 0;
                memberVisibilityState &= protectedMember;
            }else if (splitLine[0] == "public") {
                memberVisibilityState = 0;
                memberVisibilityState &= publicMember;
            }else if (isAType(splitLine[0])) {
                switch (memberVisibilityState) {
                case (privateMember):
                    //todo determine if line is a attribute or a function
                    privateAttributes.push_back(splitLine[0] + " " + splitLine[1]);
                    break;
                case publicMember:
                    publicAttributes.push_back(splitLine[0] + " " + splitLine[1]);
                    break;
                case protectedMember:
                    protectedAttributes.push_back(splitLine[0] + " " + splitLine[1]);
                    break;
                default:
                    break;
                }

            }
        }
    }
}


Uml_diagram::Uml_diagram(QWidget* widget, QFile* file)
{
    parent = widget;
    topLeft = parent->geometry().topLeft();
    topLeft.rx() += 20;
    topLeft.ry() += 20;
    bgColor = QColor(30,252,178); // teal
    titleColor = QColor(Qt::black);
    privateTextColor = QColor(135,8,8); // maroonish
    protectedTextColor = QColor(137,131,4); // ochre
    publicTextColor = QColor(7,86,1); // dark green

    processingState = 0;
    memberVisibilityState = 0;
    parseFile(file);

}

Uml_diagram::~Uml_diagram()
{

}

void Uml_diagram::paintDiagram(QPainter* painter)
{
    QBrush brush = painter->brush();
    brush.setColor(bgColor);
    painter->drawRoundedRect(topLeft.x(),topLeft.y(),100,100,6,6,Qt::AbsoluteSize);


}

void Uml_diagram::moveDiagram(QPoint point)
{

}
