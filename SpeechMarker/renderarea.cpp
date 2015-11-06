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
    for(int i=0; i<vectMarks->length(); i++) {
        if((vectMarks->data()[i] >= leftVisibleBorder) && (vectMarks->data()[i] <= rightVisibleBorder)) {
            QPoint upperPoint(qFloor(static_cast<double>(vectMarks->data()[i]- leftVisibleBorder)/xScaleSamples), 0);
            QPoint lowerPoint(qFloor(static_cast<double>(vectMarks->data()[i]- leftVisibleBorder)/xScaleSamples),this->height());
            painter.drawLine(upperPoint, lowerPoint);
        }
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
// ---------------------
// New drawing algorithm (based on extrema):
// ---------------------
        /*
        int samplesPerPixel = qFloor(xScaleSamples);
        QVector<QPoint> pointsToDraw(this->width()*4);
        int pointsToDrawIndex = 0;
        for(int i=0; i<this->width(); i++) {
            // Left-most sample:
            pointsToDraw[pointsToDrawIndex].setX(i);
            pointsToDraw[pointsToDrawIndex].setY(-yScaleSamples*vectSamples->data()[i*samplesPerPixel+leftVisibleBorder]+hd_cast);
            pointsToDrawIndex++;
            // Highest and lowest samples:
            int highestSample = vectSamples->data()[i*samplesPerPixel + leftVisibleBorder];
            int lowestSample = vectSamples->data()[i*samplesPerPixel + leftVisibleBorder];
            for(int j = i*samplesPerPixel + leftVisibleBorder + 1;
                j < i*samplesPerPixel + leftVisibleBorder + samplesPerPixel;
                j++)
            {
                if(vectSamples->data()[j] > highestSample)
                    highestSample = vectSamples->data()[j];
                if(vectSamples->data()[j] < lowestSample)
                    lowestSample = vectSamples->data()[j];
            }
            // Add highest and lowest samples :
            pointsToDraw[pointsToDrawIndex].setX(i);
            pointsToDraw[pointsToDrawIndex].setY(-yScaleSamples*highestSample + hd_cast);
            pointsToDrawIndex++;
            pointsToDraw[pointsToDrawIndex].setX(i);
            pointsToDraw[pointsToDrawIndex].setY(-yScaleSamples*lowestSample + hd_cast);
            pointsToDrawIndex++;
            // Right-most sample:
            pointsToDraw[pointsToDrawIndex].setX(i);
            pointsToDraw[pointsToDrawIndex].setY(-yScaleSamples*vectSamples->data()[(i+1)*samplesPerPixel - 1 +leftVisibleBorder]+hd_cast);
            pointsToDrawIndex++;
        }*/
        // Remove unnecessary points:
        //        pointsToDraw.remove(pointsToDrawIndex, pointsToDraw.length() - pointsToDrawIndex);
        // Draw polyline:
        //painter.drawPolyline(pointsToDraw.data(), pointsToDraw.length());
// ---------------------
// Old drawing algorithm:
// ---------------------
        /*QVector<QPoint> pointsToDraw(rightVisibleBorder - leftVisibleBorder+1);
        for(int i=0; i < pointsToDraw.length(); i++) {
            pointsToDraw[i].setX(qFloor(i/xScaleSamples));
            pointsToDraw[i].setY(-yScaleSamples*vectSamples->data()[i+leftVisibleBorder]+hd_cast);
        }

        // Before selected interval:
        painter.setPen(*pnCurve);
        int pointsCntBefore = (vectMarks->at(selectedInterval) < rightVisibleBorder? vectMarks->at(selectedInterval) : rightVisibleBorder) - leftVisibleBorder;
        pointsCntBefore = pointsCntBefore > 0? pointsCntBefore : 0;
        painter.drawPolyline(pointsToDraw.data(), pointsCntBefore);
        // Inside selected interval:
        painter.setPen(*pnCurveSelected);
        int pointsCntInside = (vectMarks->at(selectedInterval+1) < rightVisibleBorder? vectMarks->at(selectedInterval+1) : rightVisibleBorder) -
                              (vectMarks->at(selectedInterval)   > leftVisibleBorder?  vectMarks->at(selectedInterval)   : leftVisibleBorder);
        pointsCntInside = pointsCntInside > 0? pointsCntInside : 0;
        painter.drawPolyline(pointsToDraw.data() + pointsCntBefore, pointsCntInside);
        // After selected interval:
        painter.setPen(*pnCurve);
        int pointsCntAfter = rightVisibleBorder - (vectMarks->at(selectedInterval+1) > leftVisibleBorder? vectMarks->at(selectedInterval+1) : leftVisibleBorder) + 1;
        pointsCntAfter = pointsCntAfter > 0? pointsCntAfter : 0;
        painter.drawPolyline(pointsToDraw.data() + pointsCntBefore + pointsCntInside, pointsCntAfter);*/
// ---------------------
// Brand-new algorithm (extrema-based drawing):
// ---------------------
        int samplesPerPixel = qCeil((rightVisibleBorder - leftVisibleBorder+1) / static_cast<double>(this->width()));
        // ------------------------------------------------
        // Choose array with the most suitable window size:
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
        int startingIndex = 0;
        int endingIndex = vectExtrema->length()-1;
        while((vectExtrema->data()[startingIndex].first < leftVisibleBorder) &&
              (vectExtrema->data()[startingIndex].second < leftVisibleBorder))
            startingIndex++;
        while((vectExtrema->data()[endingIndex].first > rightVisibleBorder) &&
              (vectExtrema->data()[endingIndex].second > rightVisibleBorder))
            endingIndex--;
        QVector<QPoint> pointsToDraw((endingIndex - startingIndex)*2);    // in every pixel we draw maximal and minimal values
        for(int i=startingIndex; i<endingIndex; i++){
            // Minimal value for i-th window:
            int pixel_x = qFloor((vectExtrema->data()[i].first - leftVisibleBorder)/xScaleSamples);
            pointsToDraw.data()[(i-startingIndex)*2].setX(pixel_x);
            int pixel_y = -yScaleSamples*vectSamples->data()[vectExtrema->data()[i].first]+hd_cast;
            pointsToDraw.data()[(i-startingIndex)*2].setY(pixel_y);
            // Maximal value for i-th window:
            pixel_x = qFloor((vectExtrema->data()[i].second - leftVisibleBorder)/xScaleSamples);
            pointsToDraw.data()[(i-startingIndex)*2+1].setX(pixel_x);
            pixel_y = -yScaleSamples*vectSamples->data()[vectExtrema->data()[i].second]+hd_cast;
            pointsToDraw.data()[(i-startingIndex)*2+1].setY(pixel_y);
        }
        painter.drawPolyline(pointsToDraw.data(), pointsToDraw.length());
    }
}

void RenderArea::resizeEvent(QResizeEvent *event)
{
    Area->setRect(0,0,this->width(), this->height());
    yScaleSamples = static_cast<double>(this->height())/static_cast<double>(maxSampleValue);
//    xScaleSamples = static_cast<double>(vectSamples->length())/static_cast<double>(this->width());
    xScaleSamples = static_cast<double>(rightVisibleBorder - leftVisibleBorder+1)/static_cast<double>(this->width());
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
    yScaleSamples = static_cast<double>(this->height())/static_cast<double>(maxSampleValue);
//    xScaleSamples = static_cast<double>(vectSamples->length())/static_cast<double>(this->width());
    xScaleSamples = static_cast<double>(rightVisibleBorder - leftVisibleBorder+1)/static_cast<double>(this->width());
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

void RenderArea::prepareExtremaArray(QVector<QPair<int, int> > *vectExtrema, int samplesPerPixel)
{
    int vectLen128 = qCeil(vectSamples->length() / static_cast<double>(samplesPerPixel));
    vectExtrema->resize(vectLen128);
    for(int i=0; i < vectExtrema->length(); i++) {
        vectExtrema->data()[i].first = i*samplesPerPixel;     // Index of a minimal value for the i-th window
        vectExtrema->data()[i].second = i*samplesPerPixel;    // Index of a maximal value for the i-th window
        for(int j=i*samplesPerPixel + 1; (j < (i+1)*samplesPerPixel) && (j < vectSamples->length()); j++) {
            // Find index of a minimal number in the i-th window:
            if(vectSamples->data()[j] < vectSamples->data()[vectExtrema->data()[i].first])
                vectExtrema->data()[i].first = j;
            // Find index of a maximal number in the i-th window:
            if(vectSamples->data()[j] > vectSamples->data()[vectExtrema->data()[i].second])
                vectExtrema->data()[i].second = j;
        }
    }
}

void RenderArea::preparePrecalculatedArrays()
{
    prepareExtremaArray(&vectExtrema64, 64);
    prepareExtremaArray(&vectExtrema128, 128);
    prepareExtremaArray(&vectExtrema256, 256);
    prepareExtremaArray(&vectExtrema512, 512);
    prepareExtremaArray(&vectExtrema1024, 1024);
    prepareExtremaArray(&vectExtrema2048, 2048);
    prepareExtremaArray(&vectExtrema4096, 4096);
    prepareExtremaArray(&vectExtrema8192, 8192);
    prepareExtremaArray(&vectExtrema16384, 16384);
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
