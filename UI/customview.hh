#ifndef CUSTOMVIEW_HH
#define CUSTOMVIEW_HH

#include <QGraphicsView>
#include <QtWidgets>
#include <QWidget>

namespace Student {
class CustomView;
}

class CustomView : public QGraphicsView
{
public:
    explicit CustomView(QWidget *parent);

protected:
    /**
     * @brief wheelEvent zooms in or out when user rolls wheel
     * @param event QWheelEvent.
     */
    virtual void wheelEvent(QWheelEvent *event);
};
#endif // CUSTOMVIEW_HH
