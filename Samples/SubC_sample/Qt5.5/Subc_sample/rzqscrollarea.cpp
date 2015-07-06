#include "rzqscrollarea.h"

RZQScrollArea::RZQScrollArea(QWidget *parent) :
    QScrollArea(parent)
{

}

void RZQScrollArea::mousePressEvent(QMouseEvent *event)
{
    emit userInteracted();
}

