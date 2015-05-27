#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPixmap>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(unsigned int initMarkerPos, QWidget *parent = 0);
    ~RenderArea();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event);
private:
    QPixmap pixmap;
    QRect *Area;
    QBrush *brBackground;
    QPen *pnAxis, *pnCurve, *pnMarker;
    QPoint *pointLeftAxisEnd, *pointRightAxisEnd, *pointUpperMarkerEnd, *pointLowerMarkerEnd;
    unsigned int markerPos;
};

#endif // RENDERAREA_H
