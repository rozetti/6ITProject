#ifndef RZQSCROLLAREA_H
#define RZQSCROLLAREA_H

#include <QtWidgets>

class RZQScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    RZQScrollArea(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);

    void allowVerticalScrolling(bool b);
    void allowHorizontalScrolling(bool b);
    void clampToBottom(bool b);

private:
    void updateScrollingPolicy();

signals:
    void userInteracted();

private:
    bool m_allowVerticalScrolling;
    bool m_allowHorizontalScrolling;
    bool m_clampToBottom;

private slots:
    void moveScrollBarToBottom(int min, int max);
};

#endif // RZQSCROLLAREA_H
