#include "renderarea.h"
#include <QPainter>
#include <QtMath>
#include <QVariant>

RenderArea::RenderArea(const QVector<int> *samples, const QVector<int> *markers, const int *PtrMarkerPos, QWidget *parent) : vectSamples(samples), vectMarks(markers), markerPos(PtrMarkerPos), selectedInterval(0), QWidget(parent)
{
    Area = new QRect(0,0,this->width(), this->height());
    pixmap = new QPixmap(Area->width(), Area->height());
    brBackground = new QBrush(QColor(255,255,255));
    pnAxis = new QPen(QColor(0,0,0));
    pnCurve = new QPen(QColor(0,0,200));
    pnCurveSelected = new QPen(QColor(128,0,128));
    pnMarker = new QPen(QColor(200,0,0));
    pnMarks = new QPen(QColor(0,200,0));
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
    pointLeftAxisEnd->setY(qFloor(this->height()/2));
    pointRightAxisEnd->setX(this->width());
    pointRightAxisEnd->setY(qFloor(this->height()/2));
    painter.drawLine(*pointLeftAxisEnd, *pointRightAxisEnd);
}

void RenderArea::drawMarker(QPainter &painter)
{
    painter.setPen(*pnMarker);
    yScaleSamples = static_cast<double>(this->height())/static_cast<double>(maxSampleValue);
    xScaleSamples = static_cast<double>(vectSamples->length())/static_cast<double>(this->width());
    pointUpperMarkerEnd->setX(qFloor(static_cast<double>(*markerPos)/xScaleSamples));
    pointLowerMarkerEnd->setX(qFloor(static_cast<double>(*markerPos)/xScaleSamples));
    pointLowerMarkerEnd->setY(this->height());
    painter.drawLine(*pointUpperMarkerEnd, *pointLowerMarkerEnd);
}

void RenderArea::drawMarks(QPainter &painter)
{
    painter.setPen(*pnMarks);
    yScaleSamples = static_cast<double>(this->height())/static_cast<double>(maxSampleValue);
    xScaleSamples = static_cast<double>(vectSamples->length())/static_cast<double>(this->width());
    for(int i=0; i<vectMarks->length(); i++) {
        QPoint upperPoint(qFloor(static_cast<double>(vectMarks->data()[i])/xScaleSamples), 0);
        QPoint lowerPoint(qFloor(static_cast<double>(vectMarks->data()[i])/xScaleSamples),this->height());
        painter.drawLine(upperPoint, lowerPoint);
    }
}

void RenderArea::drawSamples(QPainter &painter)
{

    yScaleSamples = static_cast<double>(this->height())/static_cast<double>(maxSampleValue);
    xScaleSamples = static_cast<double>(vectSamples->length())/static_cast<double>(this->width());

    /*  Let's implement drawing in three parts:
        - draw samples BEFORE selected interval with pen pnCurve;
        - draw samples INSIDE selected interval with pen pnCurveSelected;
        - draw sample AFTER selected interval with pen pnCurve;
    */
    if(vectSamples->length() > 0) {
        double hd_cast = static_cast<double>(this->height())/2;
        QVector<QPoint> pointsToDraw(vectSamples->length());
        for(int i=0; i < pointsToDraw.length(); i++) {
            pointsToDraw[i].setX(qFloor(i/xScaleSamples));
            pointsToDraw[i].setY(-yScaleSamples*vectSamples->data()[i]+hd_cast);
        }
        // Before selected interval:
        painter.setPen(*pnCurve);
        int pointsCnt = vectMarks->at(selectedInterval);
        painter.drawPolyline(pointsToDraw.data(), pointsCnt);
        // Inside selected interval:
        painter.setPen(*pnCurveSelected);
        pointsCnt = vectMarks->at(selectedInterval+1)-vectMarks->at(selectedInterval);
        painter.drawPolyline(pointsToDraw.data() + vectMarks->at(selectedInterval), pointsCnt);
        // After selected interval:
        painter.setPen(*pnCurve);
        pointsCnt = vectSamples->length() - vectMarks->at(selectedInterval+1);
        painter.drawPolyline(pointsToDraw.data() + vectMarks->at(selectedInterval+1), pointsCnt);
    }
}

void RenderArea::resizeEvent(QResizeEvent *event)
{
    Area->setRect(0,0,this->width(), this->height());

    delete pixmap;
    pixmap = new QPixmap(Area->width(), Area->height());
    QPainter painter(pixmap);
    drawBackground(painter);
    drawBackground(painter);
    drawAxis(painter);
    drawSamples(painter);
    drawMarks(painter);
    drawMarker(painter);
}

void RenderArea::updatePlot()
{
    delete pixmap;
    pixmap = new QPixmap(Area->width(), Area->height());
    QPainter painter(pixmap);
    drawBackground(painter);
    drawAxis(painter);    
    drawSamples(painter);
    drawMarks(painter);
    drawMarker(painter);
    this->update();
}

 void RenderArea::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     painter.drawPixmap(0, 0, *pixmap);
 }

 void RenderArea::mouseReleaseEvent(QMouseEvent * event)
 {
    emit markerPositionChanged(qFloor(event->x() * xScaleSamples));
 }
