#include "rzqscrollarea.h"

RZQScrollArea::RZQScrollArea(QWidget *parent) :
    QScrollArea(parent),
    m_allowHorizontalScrolling(true),
    m_allowVerticalScrolling(true),
    m_clampToBottom(false)
{
    updateScrollingPolicy();
}

void RZQScrollArea::mousePressEvent(QMouseEvent *event)
{
    emit userInteracted();
}

void RZQScrollArea::allowVerticalScrolling(bool b)
{
    m_allowVerticalScrolling = b;
    updateScrollingPolicy();
}

void RZQScrollArea::allowHorizontalScrolling(bool b)
{
    m_allowHorizontalScrolling = b;
    updateScrollingPolicy();
}

void RZQScrollArea::clampToBottom(bool b)
{
    m_clampToBottom = b;
    updateScrollingPolicy();
}

void RZQScrollArea::moveScrollBarToBottom(int min, int max)
{
    Q_UNUSED(min);
    verticalScrollBar()->setValue(max);
}

void RZQScrollArea::updateScrollingPolicy()
{
    auto scroller = QScroller::scroller(viewport());
    auto props = scroller->scrollerProperties();

    if (m_allowHorizontalScrolling)
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        props.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootWhenScrollable);
    }
    else
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        props.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    }

    if (m_allowVerticalScrolling)
    {
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        props.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootWhenScrollable);
    }
    else
    {
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        props.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    }

    scroller->setScrollerProperties(props);

    // hack crz: this does not work properly on WinRT phone
//    auto type = QSysInfo::productType();
//    if ("winphone" != type)
//    {
        QScroller::grabGesture(viewport(), QScroller::LeftMouseButtonGesture);
//    }

    QScrollBar* vs = verticalScrollBar();
    if (m_clampToBottom)
    {
        QObject::connect(vs, SIGNAL(rangeChanged(int,int)), this, SLOT(moveScrollBarToBottom(int, int)));
    }
    else
    {
        QObject::disconnect(vs, SIGNAL(rangeChanged(int,int)), this, SLOT(moveScrollBarToBottom(int, int)));
    }
}

