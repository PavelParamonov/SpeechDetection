#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{

}

RenderArea::~RenderArea()
{

}

QSize RenderArea::sizeHint() const
{
    return QSize(400,200);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100,100);
}
