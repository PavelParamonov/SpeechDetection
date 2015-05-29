#include "renderarea.h"
#include <QPainter>
#include <QtMath>

RenderArea::RenderArea(unsigned int initMarkerPos, QWidget *parent) : QWidget(parent)
{
    Area = new QRect(0,0,this->width(), this->height());
    pixmap = new QPixmap(Area->width(), Area->height());
    brBackground = new QBrush(QColor(255,255,255));
    pnAxis = new QPen(QColor(0,0,0));
    pnCurve = new QPen(QColor(0,0,200));
    pnMarker = new QPen(QColor(200,0,0));
    pointLeftAxisEnd = new QPoint(0, qFloor(this->height()/2));
    pointRightAxisEnd = new QPoint(this->width(), qFloor(this->height()/2));
    markerPos = initMarkerPos;
    pointUpperMarkerEnd = new QPoint(0,0);
    pointLowerMarkerEnd = new QPoint(0,0);
    maxSampleValue = 1;
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

void RenderArea::drawBackground(QPainter &painter)
{
    painter.fillRect(*Area, *brBackground);
}

void RenderArea::drawAxis(QPainter &painter)
{
    painter.setPen(*pnAxis);
    painter.drawLine(*pointLeftAxisEnd, *pointRightAxisEnd);
}

void RenderArea::drawMarker(QPainter &painter)
{
    painter.setPen(*pnMarker);
    painter.drawLine(*pointUpperMarkerEnd, *pointLowerMarkerEnd);
}

void RenderArea::drawSamples(QPainter &painter)
{
    painter.setPen(*pnCurve);
//     for(int i=0; (i<this->width()-1) && (vectSamples.length()>0); i++){
//         QPoint leftEnd(i, -yScaleSamples*vectSamples.data()[qFloor(xScaleSamples*i)]+this->height()/2);
//         QPoint rightEnd(i+1, -yScaleSamples*vectSamples.data()[qFloor(xScaleSamples*(i+1))]+this->height()/2);
//         painter.drawLine(leftEnd, rightEnd);
//     }
    for(int i=0; i<vectSamples.length()-1; i++){
        QPoint leftEnd(qFloor(i/xScaleSamples), -yScaleSamples*vectSamples.data()[i]+this->height()/2);
        QPoint rightEnd(qFloor((i+1)/xScaleSamples), -yScaleSamples*vectSamples.data()[i+1]+this->height()/2);
        painter.drawLine(leftEnd, rightEnd);
    }
}

void RenderArea::setNewSamples(const QVector<int> &newVectSamples, unsigned int maxValue)
{
    vectSamples.clear();
    vectSamples = newVectSamples;
    maxSampleValue = maxValue;
    yScaleSamples = static_cast<double>(this->height())/maxSampleValue;
    xScaleSamples = vectSamples.length()/this->width();
    QPainter painter(pixmap);
    drawBackground(painter);
    drawAxis(painter);
    drawMarker(painter);
    drawSamples(painter);
    update();
}

void RenderArea::setMarkerPosition(unsigned int newPosition)
{
    markerPos = newPosition;
    pointUpperMarkerEnd->setX(qFloor(markerPos/xScaleSamples));
    pointUpperMarkerEnd->setY(0);
    pointLowerMarkerEnd->setX(qFloor(markerPos/xScaleSamples));
    pointLowerMarkerEnd->setY(this->height());
    QPainter painter(pixmap);
    drawBackground(painter);
    drawAxis(painter);
    drawMarker(painter);
    drawSamples(painter);
    update();
}

void RenderArea::resizeEvent(QResizeEvent *event)
{
    Area->setRect(0,0,this->width(), this->height());
    pointLeftAxisEnd->setY(qFloor(this->height()/2));
    pointRightAxisEnd->setX(this->width());
    pointRightAxisEnd->setY(qFloor(this->height()/2));
    yScaleSamples = static_cast<double>(this->height())/maxSampleValue;
    xScaleSamples = vectSamples.length()/this->width();
    pointUpperMarkerEnd->setX(qFloor(markerPos/xScaleSamples));
    pointUpperMarkerEnd->setY(0);
    pointLowerMarkerEnd->setX(qFloor(markerPos/xScaleSamples));
    pointLowerMarkerEnd->setY(this->height());

    delete pixmap;
    pixmap = new QPixmap(Area->width(), Area->height());
    QPainter painter(pixmap);
    drawBackground(painter);
    drawAxis(painter);
    drawMarker(painter);
    drawSamples(painter);
}

 void RenderArea::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     painter.drawPixmap(0, 0, *pixmap);
 }
