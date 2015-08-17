#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QOpenGLWidget>
#include <QPixmap>
//#include <QImage>
#include <QVector>
#include <QMouseEvent>

class RenderArea : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit RenderArea(const QVector<int> *samples, const QVector<int> *markers, const int *PtrMarkerPos, QWidget *parent = 0);
    ~RenderArea();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void setSampleMaxValue(int v) {maxSampleValue=v;}
    void setSelectedInterval(int ind) {selectedInterval=ind;}
    void updatePlot();
signals:
    void markerPositionChanged(int newPosition);
public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
private:
    QPixmap *pixmap;
//    QImage *image;
    QRect *Area;
    QBrush *brBackground;
    QPen *pnAxis, *pnCurve, *pnCurveSelected, *pnMarker, *pnMarks;
    QPoint *pointLeftAxisEnd, *pointRightAxisEnd, *pointUpperMarkerEnd, *pointLowerMarkerEnd;
    unsigned int maxSampleValue;
    const int *markerPos;
    // -----------------------------------------
    const QVector<int> *vectSamples, *vectMarks;
    // -----------------------------------------
    double xScaleSamples, yScaleSamples;
    int selectedInterval;
    // Drawing functions:
    void drawBackground(QPainter &painter);
    void drawMarker(QPainter &painter);
    void drawAxis(QPainter &painter);
    void drawSamples(QPainter &painter);
    void drawMarks(QPainter &painter);
};

#endif // RENDERAREA_H
