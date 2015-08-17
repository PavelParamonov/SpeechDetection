#include "renderarea.h"
#include <QPainter>
#include <QtMath>
#include <QVariant>

RenderArea::RenderArea(const QVector<int> *samples, const QVector<int> *markers, const int *PtrMarkerPos, QWidget *parent) : vectSamples(samples), vectMarks(markers), markerPos(PtrMarkerPos), selectedInterval(0), QOpenGLWidget(parent)
{
    Area = new QRect(0,0,this->width(), this->height());
    pixmap = new QPixmap(Area->width(), Area->height());
//    image = new QImage(Area->width(), Area->height(), QImage::Format_RGB16);
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
    painter.setPen(*pnMarks);
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

//     for(int i=0; (i<this->width()-1) && (vectSamples->length()>0); i++){
//         QPoint leftEnd(i, -yScaleSamples*vectSamples->data()[qFloor(xScaleSamples*i)]+this->height()/2);
//         QPoint rightEnd(i+1, -yScaleSamples*vectSamples->data()[qFloor(xScaleSamples*(i+1))]+this->height()/2);
//         painter.drawLine(leftEnd, rightEnd);
//     }
//    QPoint leftEnd(0, -yScaleSamples*vectSamples->data()[0]+this->height()/2);
//    for(int k = 0; k<this->width(); k++) {
//        QVector<char> listOfValues(maxSampleValue*2, 0);
//        for(int j=0; j<qFloor(xScaleSamples); j++) {
//            if(listOfValues[maxSampleValue + vectSamples->data()[k*qFloor(xScaleSamples)+j]] == 0) {
//                QPoint rightEnd(k, -yScaleSamples*vectSamples->data()[k*qFloor(xScaleSamples)+j]+this->height()/2);
//                painter.drawLine(leftEnd, rightEnd);
//                leftEnd = rightEnd;
//                listOfValues[maxSampleValue + vectSamples->data()[k*qFloor(xScaleSamples)+j]] = 1;
//            }
//        }
//    }
//    for(int i=0; i<vectSamples->length()-1; i++){
//        if(i == (qFloor(xScaleSamples*k))) {
//            QPoint leftEnd(qFloor(i/xScaleSamples), -yScaleSamples*vectSamples->data()[i]+this->height()/2);
//            QPoint rightEnd(qFloor((i+1)/xScaleSamples), -yScaleSamples*vectSamples->data()[i+1]+this->height()/2);
//            painter.drawLine(leftEnd, rightEnd);
//            leftEnd = rightEnd;
//            k++;
//        }
//    }
    /*  Let's implement drawing in three parts:
        - draw samples BEFORE selected interval with pen pnCurve;
        - draw samples INSIDE selected interval with pen pnCurveSelected;
        - draw sample AFTER selected interval with pen pnCurve;
    */
    if(vectSamples->length() > 0) {
        // Before selected interval:
        painter.setPen(*pnCurve);
        for(int i=0; (i<vectMarks->at(selectedInterval)-1); i++){
            QPoint leftEnd(qFloor(i/xScaleSamples), -yScaleSamples*vectSamples->data()[i]+this->height()/2);
            QPoint rightEnd(qFloor((i+1)/xScaleSamples), -yScaleSamples*vectSamples->data()[i+1]+this->height()/2);
            painter.drawLine(leftEnd, rightEnd);
        }
        // Inside selected interval:
        painter.setPen(*pnCurveSelected);
        for(int i=vectMarks->at(selectedInterval); i<vectMarks->at(selectedInterval+1)-1; i++){
            QPoint leftEnd(qFloor(i/xScaleSamples), -yScaleSamples*vectSamples->data()[i]+this->height()/2);
            QPoint rightEnd(qFloor((i+1)/xScaleSamples), -yScaleSamples*vectSamples->data()[i+1]+this->height()/2);
            painter.drawLine(leftEnd, rightEnd);
        }
        // After selected interval:
        painter.setPen(*pnCurve);
        for(int i=vectMarks->at(selectedInterval+1); i<vectSamples->length()-1; i++){
            QPoint leftEnd(qFloor(i/xScaleSamples), -yScaleSamples*vectSamples->data()[i]+this->height()/2);
            QPoint rightEnd(qFloor((i+1)/xScaleSamples), -yScaleSamples*vectSamples->data()[i+1]+this->height()/2);
            painter.drawLine(leftEnd, rightEnd);
        }
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
//    delete image;
    pixmap = new QPixmap(Area->width(), Area->height());
//    image = new QImage(Area->width(), Area->height(), QImage::Format_RGB16);
    QPainter painter(pixmap);
//    QPainter painter(image);
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
//    delete image;
//    image = new QImage(Area->width(), Area->height(), QImage::Format_RGB16);
//    QPainter painter(image);
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
//     painter.drawImage(0,0, *image);
 }

 void RenderArea::mouseReleaseEvent(QMouseEvent * event)
 {
    emit markerPositionChanged(qFloor(event->x() * xScaleSamples));
 }
