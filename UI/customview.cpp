#include "customview.hh"

CustomView::CustomView(QWidget *parent) : QGraphicsView(parent)
{
}

void CustomView::wheelEvent(QWheelEvent *event)
{
    // Anchors zoom under mouse
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;


    // If wheel is rolled forward
    if(event->delta() > 0)
    {
        // Zooms in
        scale(scaleFactor,scaleFactor);
    }
    // If wheel is rolled backwards
    else
    {
        // Zooms out
        scale(1/scaleFactor,1/scaleFactor);
    }
}
