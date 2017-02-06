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
    explicit RenderArea(const QVector<int> *samples, const QVector<int> *markers, const int *PtrMarkerPos, QWidget *parent = 0);
    ~RenderArea();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void setSampleMaxValue(int v) {maxSampleValue=v;}
    void setSelectedInterval(int ind) {selectedInterval=ind;}
    void setVisibleBorders(int left, int right) {leftVisibleBorder = left; rightVisibleBorder = right;}
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
    void drawMessage(QPainter &painter, QString &message);
    void redrawPixmap();
};

#endif // RENDERAREA_H
