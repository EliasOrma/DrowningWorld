#ifndef WHEELITEM_HH
#define WHEELITEM_HH

#include <QGraphicsItem>
#include <QGraphicsScene>

namespace Student {


class WheelItem : public QGraphicsPixmapItem
{
public:
    WheelItem(QGraphicsScene *scene,double scale);

    /**
     * @brief spin rotates wheel image
     */
    void spin();

    /**
     * @brief resetRotation sets wheel rotation back to 45 degrees
     */
    void resetRotation();

private:
    /**
     * @brief drawWheel draws wheel
     */
    void drawWheel();

    qreal rotated;
    QGraphicsScene *_scene;
};
}

#endif // WHEELITEM_HH
