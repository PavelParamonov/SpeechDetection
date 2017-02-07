#include "renderarea.h"
#include <QPainter>
#include <QtMath>
#include <QVariant>
#include <algorithm>

#include <QThread>
#include "workerwavformwrecalculation.h"

RenderArea::RenderArea(const QVector<int> *samples, const QVector<int> *markers, const QVector<QString> *labels, const int *PtrMarkerPos, QWidget *parent) : vectSamples(samples), vectMarks(markers), vectLabels(labels), markerPos(PtrMarkerPos), selectedInterval(0), QWidget(parent)
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
    arraysReadyCnt = 0;
    state = ACTIVEDRAWING;
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
    if((*markerPos >= leftVisibleBorder) && (*markerPos <= rightVisibleBorder)) {
        painter.setPen(*pnMarker);
        pointUpperMarkerEnd->setX(qFloor(static_cast<double>(*markerPos - leftVisibleBorder)/xScaleSamples));
        pointLowerMarkerEnd->setX(qFloor(static_cast<double>(*markerPos - leftVisibleBorder)/xScaleSamples));
        pointLowerMarkerEnd->setY(this->height());
        painter.drawLine(*pointUpperMarkerEnd, *pointLowerMarkerEnd);
    }
}

void RenderArea::drawMarks(QPainter &painter)
{
    painter.setPen(*pnMarks);
    for(int i=0; i<vectMarks->length()-1; i++) {
        if((vectMarks->data()[i] >= leftVisibleBorder) && (vectMarks->data()[i] <= rightVisibleBorder)) {
            QPoint upperPoint(qFloor(static_cast<double>(vectMarks->data()[i]- leftVisibleBorder)/xScaleSamples), 0);
            QPoint lowerPoint(qFloor(static_cast<double>(vectMarks->data()[i]- leftVisibleBorder)/xScaleSamples),this->height());
            painter.drawLine(upperPoint, lowerPoint);
        }
        bool xleftVisible = ((vectMarks->data()[i] >= leftVisibleBorder) && (vectMarks->data()[i] <= rightVisibleBorder));
        bool xrightVisible = ((vectMarks->data()[i+1] >= leftVisibleBorder) && (vectMarks->data()[i+1] <= rightVisibleBorder));
        if((vectMarks->data()[i]<=rightVisibleBorder) && (vectMarks->data()[i+1]>=leftVisibleBorder)){
            // if interval between left and right marks is visible then we draw the label:
            int fontSize = 12;
            painter.setFont(QFont(QString("Times New Roman"), fontSize));
            int xleft = xleftVisible? qFloor(static_cast<double>(vectMarks->data()[i]- leftVisibleBorder)/xScaleSamples) : 0;
            int xright = xrightVisible? qFloor(static_cast<double>(vectMarks->data()[i+1]- leftVisibleBorder)/xScaleSamples) : this->width();
            QPoint textCoord(xleft + (xright-xleft)/2 - vectLabels->data()[i].length()/2*(fontSize/2), this->height()/2 + 3*this->height()/8);
            painter.drawText(textCoord, vectLabels->data()[i]);
        }
    }
    int lastMarkInd = vectMarks->length()-1;
    if((vectMarks->data()[lastMarkInd] >= leftVisibleBorder) && (vectMarks->data()[lastMarkInd] <= rightVisibleBorder)) {
        QPoint upperPoint(qFloor(static_cast<double>(vectMarks->data()[lastMarkInd]- leftVisibleBorder)/xScaleSamples), 0);
        QPoint lowerPoint(qFloor(static_cast<double>(vectMarks->data()[lastMarkInd]- leftVisibleBorder)/xScaleSamples),this->height());
        painter.drawLine(upperPoint, lowerPoint);
    }
}

void RenderArea::drawSamples(QPainter &painter)
{
    /*  Let's implement drawing in three parts:
        - draw samples BEFORE selected interval with pen pnCurve;
        - draw samples INSIDE selected interval with pen pnCurveSelected;
        - draw sample AFTER selected interval with pen pnCurve;
    */
    if(vectSamples->length() > 0) {
        double hd_cast = static_cast<double>(this->height())/2;

        int samplesPerPixel = qCeil((rightVisibleBorder - leftVisibleBorder+1) / static_cast<double>(this->width()));
        // ------------------------------------------------
        // Choose array with the most suitable window size (samples per pixel value):
        const QVector< QPair <int, int> > *vectExtrema = 0;
        int samplesPerPixelValues[] = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
        int vectExtremaIndex = 0;
        for(int i=0; i<9; i++)
            if(qAbs(samplesPerPixel-samplesPerPixelValues[i]) < qAbs(samplesPerPixel-samplesPerPixelValues[vectExtremaIndex]))
                vectExtremaIndex = i;
        switch(vectExtremaIndex) {
            case 0: vectExtrema = &vectExtrema64; break;
            case 1: vectExtrema = &vectExtrema128; break;
            case 2: vectExtrema = &vectExtrema256; break;
            case 3: vectExtrema = &vectExtrema512; break;
            case 4: vectExtrema = &vectExtrema1024; break;
            case 5: vectExtrema = &vectExtrema2048; break;
            case 6: vectExtrema = &vectExtrema4096; break;
            case 7: vectExtrema = &vectExtrema8192; break;
            case 8: vectExtrema = &vectExtrema16384; break;
        }
        // ------------------------------------------------
        int startingIndex = qAbs(static_cast<double>(leftVisibleBorder) / samplesPerPixelValues[vectExtremaIndex]);
        int endingIndex = qAbs(static_cast<double>(rightVisibleBorder) / samplesPerPixelValues[vectExtremaIndex]);
        QVector<QPoint> pointsToDraw((endingIndex - startingIndex + 1)*2);    // in every pixel we draw maximal and minimal values
        for(int i=startingIndex; i<=endingIndex; i++){
            // First extremum (either max or min) for the i-th window:
            int pixel_x = qFloor((vectExtrema->data()[i].first - leftVisibleBorder)/xScaleSamples);
            pointsToDraw.data()[(i-startingIndex)*2].setX(pixel_x);
            int pixel_y = -yScaleSamples*vectSamples->data()[vectExtrema->data()[i].first]+hd_cast;
            pointsToDraw.data()[(i-startingIndex)*2].setY(pixel_y);
            // Second extremum for the i-th window:
            pixel_x = qFloor((vectExtrema->data()[i].second - leftVisibleBorder)/xScaleSamples);
            pointsToDraw.data()[(i-startingIndex)*2+1].setX(pixel_x);
            pixel_y = -yScaleSamples*vectSamples->data()[vectExtrema->data()[i].second]+hd_cast;
            pointsToDraw.data()[(i-startingIndex)*2+1].setY(pixel_y);
        }
        // Draw curve before selected interval:
        painter.setPen(*pnCurve);
        int selectedIntervalLeftIndex = qAbs(static_cast<double>(vectMarks->at(selectedInterval)) / samplesPerPixelValues[vectExtremaIndex]);
        int selectedIntervalRightIndex = qAbs(static_cast<double>(vectMarks->at(selectedInterval+1)) / samplesPerPixelValues[vectExtremaIndex]);
        int pointsCntBefore = (selectedIntervalLeftIndex < endingIndex? selectedIntervalLeftIndex : endingIndex) - startingIndex;
        pointsCntBefore = pointsCntBefore > 0? pointsCntBefore*2 : 0;   // We draw TWO points for every pixel (max and min values)
        painter.drawPolyline(pointsToDraw.data(), pointsCntBefore);
        // Draw curve inside selected interval:
        painter.setPen(*pnCurveSelected);
        // We assume that both borders of selected interval belong to this interval (that's why "+1"):
        int pointsCntInside = (selectedIntervalRightIndex < endingIndex? selectedIntervalRightIndex : endingIndex) -
                                     (selectedIntervalLeftIndex   > startingIndex?  selectedIntervalLeftIndex   : startingIndex) + 1;
        pointsCntInside = pointsCntInside > 0? pointsCntInside*2 : 0;
        painter.drawPolyline(pointsToDraw.data() + pointsCntBefore, pointsCntInside);
        // Draw curve after selected interval:
        painter.setPen(*pnCurve);
        int pointsCntAfter = endingIndex - (selectedIntervalRightIndex > startingIndex? selectedIntervalRightIndex : startingIndex);
        pointsCntAfter = pointsCntAfter > 0? pointsCntAfter*2 : 0;
        painter.drawPolyline(pointsToDraw.data() + pointsCntBefore + pointsCntInside, pointsCntAfter);
    }
}

void RenderArea::drawMessage(QPainter &painter, QString &message)
{
    int fontSize = 14;
    painter.setFont(QFont(QString("Times New Roman"), fontSize));
    painter.drawText(QPoint(this->width()/2 - message.length()/2*(fontSize/2), this->height()/2 + (fontSize/2)), message);
}

void RenderArea::resizeEvent(QResizeEvent *event)
{
    Area->setRect(0,0,this->width(), this->height());
    redrawPixmap();
}

void RenderArea::redrawPixmap()
{
    delete pixmap;
    pixmap = new QPixmap(Area->width(), Area->height());
    QPainter painter(pixmap);
    switch(state) {
    case ACTIVEDRAWING:
        yScaleSamples = static_cast<double>(this->height())/static_cast<double>(maxSampleValue);
        //    xScaleSamples = static_cast<double>(vectSamples->length())/static_cast<double>(this->width());
        xScaleSamples = static_cast<double>(rightVisibleBorder - leftVisibleBorder+1)/static_cast<double>(this->width());
        drawBackground(painter);
        drawBackground(painter);
        drawAxis(painter);
        drawSamples(painter);
        drawMarks(painter);
        drawMarker(painter);
        break;
    case INACTIVE:
        drawMessage(painter, QString(inactiveMessage));
        break;
    default:
        drawMessage(painter, QString("render area is in unknown state"));
    }
}

void RenderArea::updatePlot()
{
    redrawPixmap();
    this->update();
}

void RenderArea::prepareExtremaArray(QVector<QPair<int, int> > *vectExtrema, int samplesPerPixel)
{
    WorkerWavformPrecalculation *worker = new WorkerWavformPrecalculation(vectSamples, vectExtrema, samplesPerPixel, &mutex);
    QThread *new_thread = new QThread;
    worker->moveToThread(new_thread);
    connect(new_thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), new_thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(new_thread, SIGNAL(finished()), new_thread, SLOT(deleteLater()));
    connect(worker, SIGNAL(finished()), this, SLOT(gatherPrecalculationResults()));
    new_thread->start();
}

void RenderArea::preparePrecalculatedArrays()
{
    arraysReadyCnt = 0;
    vectExtrema64.clear();
    prepareExtremaArray(&vectExtrema64, 64);
    vectExtrema128.clear();
    prepareExtremaArray(&vectExtrema128, 128);
    vectExtrema256.clear();
    prepareExtremaArray(&vectExtrema256, 256);
    vectExtrema512.clear();
    prepareExtremaArray(&vectExtrema512, 512);
    vectExtrema1024.clear();
    prepareExtremaArray(&vectExtrema1024, 1024);
    vectExtrema2048.clear();
    prepareExtremaArray(&vectExtrema2048, 2048);
    vectExtrema4096.clear();
    prepareExtremaArray(&vectExtrema4096, 4096);
    vectExtrema8192.clear();
    prepareExtremaArray(&vectExtrema8192, 8192);
    vectExtrema16384.clear();
    prepareExtremaArray(&vectExtrema16384, 16384);
}

void RenderArea::gatherPrecalculationResults()
{
    arraysReadyCnt++;
    emit stepsOfPrecalculation(arraysReadyCnt);
    if(arraysReadyCnt == 9)
        emit precalculatedArraysReady();
}

 void RenderArea::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     painter.drawPixmap(0, 0, *pixmap);
 }

 void RenderArea::mouseReleaseEvent(QMouseEvent * event)
 {
    emit markerPositionChanged(qFloor(event->x() * xScaleSamples) + leftVisibleBorder);
 }
