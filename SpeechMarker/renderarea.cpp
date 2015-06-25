#include "renderarea.h"
#include <QPainter>
#include <QtMath>

RenderArea::RenderArea(const QVector<int> *samples, const QVector<int> *markers, const unsigned int *PtrMarkerPos, QWidget *parent) : vectSamples(samples), vectMarks(markers), markerPos(PtrMarkerPos), QWidget(parent)
{
    Area = new QRect(0,0,this->width(), this->height());
    pixmap = new QPixmap(Area->width(), Area->height());
    brBackground = new QBrush(QColor(255,255,255));
    pnAxis = new QPen(QColor(0,0,0));
    pnCurve = new QPen(QColor(0,0,200));
    pnMarker = new QPen(QColor(200,0,0));
    pointLeftAxisEnd = new QPoint(0, qFloor(this->height()/2));
    pointRightAxisEnd = new QPoint(this->width(), qFloor(this->height()/2));
    pointUpperMarkerEnd = new QPoint(0,0);
    pointLowerMarkerEnd = new QPoint(0,this->height());
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
    pointUpperMarkerEnd->setX(qFloor(static_cast<double>(*markerPos)/xScaleSamples));
    pointLowerMarkerEnd->setX(qFloor(static_cast<double>(*markerPos)/xScaleSamples));
    pointLowerMarkerEnd->setY(this->height());
    painter.drawLine(*pointUpperMarkerEnd, *pointLowerMarkerEnd);
}

void RenderArea::drawMarks(QPainter &painter)
{

}

void RenderArea::drawSamples(QPainter &painter)
{
    painter.setPen(*pnCurve);
//     for(int i=0; (i<this->width()-1) && (vectSamples.length()>0); i++){
//         QPoint leftEnd(i, -yScaleSamples*vectSamples.data()[qFloor(xScaleSamples*i)]+this->height()/2);
//         QPoint rightEnd(i+1, -yScaleSamples*vectSamples.data()[qFloor(xScaleSamples*(i+1))]+this->height()/2);
//         painter.drawLine(leftEnd, rightEnd);
//     }
    yScaleSamples = static_cast<double>(this->height())/maxSampleValue;
    xScaleSamples = static_cast<double>(vectSamples->length())/static_cast<double>(this->width());

    for(int i=0; i<vectSamples->length()-1; i++){
        QPoint leftEnd(qFloor(i/xScaleSamples), -yScaleSamples*vectSamples->data()[i]+this->height()/2);
        QPoint rightEnd(qFloor((i+1)/xScaleSamples), -yScaleSamples*vectSamples->data()[i+1]+this->height()/2);
        painter.drawLine(leftEnd, rightEnd);
    }
}

void RenderArea::resizeEvent(QResizeEvent *event)
{
    Area->setRect(0,0,this->width(), this->height());
    pointLeftAxisEnd->setY(qFloor(this->height()/2));
    pointRightAxisEnd->setX(this->width());
    pointRightAxisEnd->setY(qFloor(this->height()/2));
    yScaleSamples = static_cast<double>(this->height())/static_cast<double>(maxSampleValue);
    xScaleSamples = static_cast<double>(vectSamples->length())/static_cast<double>(this->width());
    pointUpperMarkerEnd->setX(qFloor(static_cast<double>(*markerPos)/xScaleSamples));
    pointLowerMarkerEnd->setX(qFloor(static_cast<double>(*markerPos)/xScaleSamples));
    pointLowerMarkerEnd->setY(this->height());

    delete pixmap;
    pixmap = new QPixmap(Area->width(), Area->height());
    QPainter painter(pixmap);
    drawBackground(painter);
    drawAxis(painter);
    drawMarker(painter);
    drawSamples(painter);
}

void RenderArea::updatePlot()
{
    delete pixmap;
    pixmap = new QPixmap(Area->width(), Area->height());
    QPainter painter(pixmap);
    drawBackground(painter);
    drawAxis(painter);
    drawMarker(painter);
    drawSamples(painter);
    this->update();
}

 void RenderArea::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     painter.drawPixmap(0, 0, *pixmap);
 }
