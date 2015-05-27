#include "renderarea.h"
#include <QPainter>
#include <QtMath>

RenderArea::RenderArea(unsigned int initMarkerPos, QWidget *parent) : QWidget(parent)
{
    Area = new QRect(0,0,this->width(), this->height());
    brBackground = new QBrush(QColor(255,255,255));
    pnAxis = new QPen(QColor(0,0,0));
    pnCurve = new QPen(QColor(0,0,200));
    pnMarker = new QPen(QColor(200,0,0));
    pointLeftAxisEnd = new QPoint(0, qFloor(this->height()/2));
    pointRightAxisEnd = new QPoint(this->width(), qFloor(this->height()/2));
    markerPos = initMarkerPos;
    pointUpperMarkerEnd = new QPoint(qFloor(this->width()/2), 0);
    pointLowerMarkerEnd = new QPoint(qFloor(this->width()/2), this->height());
}

RenderArea::~RenderArea()
{
    delete pointRightAxisEnd;
    delete pointLeftAxisEnd;
    delete pnMarker;
    delete pnCurve;
    delete pnAxis;
    delete brBackground;
    delete Area;
}

QSize RenderArea::sizeHint() const
{
    return QSize(400,200);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100,100);
}
void RenderArea::resizeEvent(QResizeEvent *event)
{
    Area->setRect(0,0,this->width(), this->height());
    pointLeftAxisEnd->setY(qFloor(this->height()/2));
    pointRightAxisEnd->setX(this->width());
    pointRightAxisEnd->setY(qFloor(this->height()/2));
}

 void RenderArea::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     painter.setPen(*pnAxis);
     painter.fillRect(*Area, *brBackground);
     painter.drawLine(*pointLeftAxisEnd, *pointRightAxisEnd);
     painter.setPen(*pnMarker);
     painter.drawLine(*pointUpperMarkerEnd, *pointLowerMarkerEnd);
 }
