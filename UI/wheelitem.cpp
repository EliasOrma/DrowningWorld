#include "wheelitem.hh"

static int BASICROTATION = 45;

Student::WheelItem::WheelItem(QGraphicsScene *scene,
                              double scale) : _scene(scene)
{
    drawWheel();
    this->setScale(scale);
    this->setTransformOriginPoint(QPointF(75.5,75.5));
    this->setRotation(BASICROTATION);
    rotated = this->rotation();
}
void Student::WheelItem::spin()
{
    rotated++;
    this->setRotation(rotated);
}

void Student::WheelItem::resetRotation()
{
    this->setRotation(BASICROTATION);
    rotated = BASICROTATION;
}

void Student::WheelItem::drawWheel()
{
    _scene->addItem(this);
}
