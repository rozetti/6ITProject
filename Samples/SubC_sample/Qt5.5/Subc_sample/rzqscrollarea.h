#ifndef RZQSCROLLAREA_H
#define RZQSCROLLAREA_H

#include <QtWidgets>

class RZQScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    RZQScrollArea(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);

signals:
    void userInteracted();
};

#endif // RZQSCROLLAREA_H
