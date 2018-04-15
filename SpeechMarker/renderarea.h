/******************************************************************************
 * SpeechMarker:  Qt-based GUI application for marking wav files
 * Copyright (C) 2015-2018 Pavel Paramonov <pa.pawka@gmail.com>

 * SpeechMarker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * SpeechMarker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with SpeechMarker; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QMouseEvent>
#include <QPair>
#include <QString>
#include <QMutex>

enum RenderAreaState {ACTIVEDRAWING, INACTIVE};

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(const QVector<int> *samples, const QVector<int> *markers, const QVector<QString> *labels, const int *PtrMarkerPos, QWidget *parent = 0);
    ~RenderArea();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void setSampleMaxValue(int v) {maxSampleValue=v;}
    void setSelectedInterval(int ind) {selectedInterval=ind;}
    void setVisibleBorders(int left, int right) {leftVisibleBorder = left; rightVisibleBorder = right;}
    int getRightVisibleBorder() {return rightVisibleBorder;}
    void updatePlot();
    void preparePrecalculatedArrays();  // Prepare pre-calsulated extrema arrays for drawing
    void setState(RenderAreaState newState, QString newMessage=QString("")) {state = newState; inactiveMessage = newMessage;}
signals:
    void markerPositionChanged(int newPosition);
    void stepsOfPrecalculation(int stepVal);
    void precalculatedArraysReady();
public slots:
    void gatherPrecalculationResults();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
private:
    QPixmap *pixmap;
    QRect *Area;
    QBrush *brBackground;
    QPen *pnAxis, *pnCurve, *pnCurveSelected, *pnMarker, *pnMarks;
    QPoint *pointLeftAxisEnd, *pointRightAxisEnd, *pointUpperMarkerEnd, *pointLowerMarkerEnd;
    unsigned int maxSampleValue;
    const int *markerPos;
    int arraysReadyCnt;
    QMutex mutex;
    // -----------------------------------------
    const QVector<int> *vectSamples, *vectMarks;
    const QVector<QString> *vectLabels;
    // -----------------------------------------
    QVector< QPair<int, int> > vectExtrema64, vectExtrema128, vectExtrema256, vectExtrema512, vectExtrema1024, vectExtrema2048, vectExtrema4096, vectExtrema8192, vectExtrema16384;
    // -----------------------------------------
    double xScaleSamples, yScaleSamples;
    int selectedInterval;
    int leftVisibleBorder, rightVisibleBorder;
    RenderAreaState state;
    QString inactiveMessage;
    // Drawing functions:
    void drawBackground(QPainter &painter);
    void drawMarker(QPainter &painter);
    void drawAxis(QPainter &painter);
    void drawSamples(QPainter &painter);
    void drawMarks(QPainter &painter);
    void prepareExtremaArray(QVector< QPair<int, int> > *vectExtrema, int samplesPerPixel);
    void drawMessage(QPainter &painter, const QString &message);
    void redrawPixmap();
};

#endif // RENDERAREA_H
