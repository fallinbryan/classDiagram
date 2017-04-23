

#include "uml_diagram.h"

#include <QTextStream>

QVector<QString> varTypes = { "int",
                              "double",
                              "float",
                              "bool",
                              "unsigned",
                              "void"
                            };

bool isAType(QString string) {
    for(int i = 0; i < varTypes.length(); i++) {
        if(string == varTypes[i]) return true;
    }
    return false;
}


void Uml_diagram::parseFile(QFile *file)
{
    //todo store this comment as a pop-up tool tip when hovering over a name
    QString comment;
    QString includes;

    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(file);
    while(!in.atEnd()) {

        QString line = in.readLine();

        if(line.startsWith("#")) {
            includes.append(line);
            continue;
        }

        if(line.startsWith("//") || line.startsWith("/*") || line.endsWith("*/"))  {
            comment.append(line);
            continue;
        }

        if(line.startsWith("{")) continue;

        if(line.startsWith("class") || line.startsWith("struct")) {

            if(line.startsWith("class")) {
                memberVisibilityState |= privateMember;
                processingState |=  processingClass;
            }

            if(line.startsWith("struct")) {
                memberVisibilityState |= publicMember;
                processingState |= processingStruct;
            }

            QStringList splitLine = line.split(' ',QString::SkipEmptyParts);
            this->title = splitLine[1];
            if(splitLine.length() > 2 ) {
                if(splitLine[2] == ":") {
                    //parent = splitLine[3]  <---todo  figure that one out
                }
            }

        }

        if( (processingState & processingClass) == processingClass ||
                (processingState & processingStruct) == processingStruct) {
            if(line.length() > 1) {
                QStringList splitLine = line.split(' ',QString::SkipEmptyParts);

                if(splitLine[0].startsWith("private")) {
                    memberVisibilityState = 0;
                    memberVisibilityState |= privateMember;
                }else if (splitLine[0].startsWith("protected")) {
                    memberVisibilityState = 0;
                    memberVisibilityState |= protectedMember;
                }else if (splitLine[0].startsWith("public")) {
                    memberVisibilityState = 0;
                    memberVisibilityState |= publicMember;
                }else if (isAType(splitLine[0])) {
                    //todo determine if line is a attribute or a function
                    bool isAttribute = true;
                    QString labelString;
                    for(int k = 0; k < splitLine.length();k++) {
                        if(isAType(splitLine[k]))
                            labelString.append(splitLine[k] + " ");
                        if(splitLine[k].contains('(') )
                            isAttribute = false;
                    }
                    labelString.append(splitLine.last());

                    switch (memberVisibilityState) {

                    case (privateMember):

                        if(isAttribute)
                            privateAttributes.push_back(labelString);
                        else
                            privateFunctions.push_back(labelString);
                        break;
                    case publicMember:
                        if(isAttribute)
                            publicAttributes.push_back(labelString);
                        else
                            publicFunctions.push_back(labelString);
                        break;
                    case protectedMember:
                        if(isAttribute)
                            publicAttributes.push_back(labelString);
                        else
                            publicFunctions.push_back(labelString);
                        break;
                    default:
                        break;
                    }

                }
            }
        }
    }
}


Uml_diagram::Uml_diagram(QWidget* widget, QFile* file, int top, int left)
{
    parent = widget;
    topLeft = parent->geometry().topLeft();
    topLeft.rx() = left;
    topLeft.ry() = top;
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

    int spacer = 30;
    int width = 200;
    int textXAlign = 10;
    int textYAlign = 20;
    QRect elementRect;
    elementRect.setTop(topLeft.y() + spacer);
    elementRect.setBottom(elementRect.top()+spacer);
    elementRect.setLeft(topLeft.x());
    elementRect.setRight(width);


    QBrush brush = painter->brush();
    QPen pen = painter->pen();



    brush.setColor(bgColor);
    brush.setStyle(Qt::SolidPattern);

    pen.setColor(titleColor);

    painter->setBrush(brush);
    painter->drawRoundedRect(topLeft.x(),topLeft.y(),width,spacer,6,6,Qt::AbsoluteSize);

    painter->setPen(pen);
    painter->drawText(topLeft.x() + textXAlign, topLeft.y() + textYAlign, title);

    brush.setStyle(Qt::NoBrush);
    painter->setBrush(brush);
    pen.setColor(privateTextColor);
    painter->setPen(pen);
    for(int i = 0; i < privateAttributes.length(); i++) {
        painter->drawRect(elementRect);


        painter->drawText(elementRect.left()+textXAlign,
                          elementRect.top()+textYAlign,
                          privateAttributes[i]);

        elementRect.setTop(elementRect.bottom());
        elementRect.setBottom(elementRect.top()+spacer);
    }

    for(int i = 0; i < privateFunctions.length(); i++) {
        painter->drawRect(elementRect);


        painter->drawText(elementRect.left()+textXAlign,
                          elementRect.top()+textYAlign,
                          privateFunctions[i]);

        elementRect.setTop(elementRect.bottom());
        elementRect.setBottom(elementRect.top()+spacer);
    }
    pen.setColor(protectedTextColor);
    painter->setPen(pen);
    for(int i = 0; i < protectedAttributes.length(); i++) {
        painter->drawRect(elementRect);


        painter->drawText(elementRect.left()+textXAlign,
                          elementRect.top()+textYAlign,
                          protectedAttributes[i]);

        elementRect.setTop(elementRect.bottom());
        elementRect.setBottom(elementRect.top()+spacer);
    }

    for(int i = 0; i < protectedFunctions.length(); i++) {
        painter->drawRect(elementRect);


        painter->drawText(elementRect.left()+textXAlign,
                          elementRect.top()+textYAlign,
                          protectedFunctions[i]);

        elementRect.setTop(elementRect.bottom());
        elementRect.setBottom(elementRect.top()+spacer);
    }
    pen.setColor(publicTextColor);
    painter->setPen(pen);
    for(int i = 0; i < publicAttributes.length(); i++) {
        painter->drawRect(elementRect);


        painter->drawText(elementRect.left()+textXAlign,
                          elementRect.top()+textYAlign,
                          publicAttributes[i]);

        elementRect.setTop(elementRect.bottom());
        elementRect.setBottom(elementRect.top()+spacer);
    }

    for(int i = 0; i < publicFunctions.length(); i++) {
        painter->drawRect(elementRect);


        painter->drawText(elementRect.left()+textXAlign,
                          elementRect.top()+textYAlign,
                          publicFunctions[i]);

        elementRect.setTop(elementRect.bottom());
        elementRect.setBottom(elementRect.top()+spacer);
    }



}

void Uml_diagram::moveDiagram(QPoint point)
{

}
