#ifndef MYHEXITEM_HH
#define MYHEXITEM_HH

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>

#include "hex.hh"
#include "mainwindow.hh"
#include "actor.hh"
#include "transport.hh"
#include "pawn.hh"

class MainWindow;

namespace Student {

class Myhexitem :  public QGraphicsPolygonItem
{

public:
    Myhexitem(std::shared_ptr<Common::Hex> hexPtr,QGraphicsScene *scene,
              MainWindow *mw);

    /**
     * @brief updateHex draws hexes contents again, they might havechanged
     */
    void updateHex();


private:
    /**
     * @brief inithex draws hexagon and its contents in spesific coordinates
     */
    void inithex();

    /**
     * @brief drawpawns draws pawns inside of hex in spesific coordinates
     */
    void drawpawns();

    /**
     * @brief drawvehicle draws vehicle inside of hex in spesific coordinates
     */
    void drawvehicle();

    /**
     * @brief drawactor draws actor inside of hex in spesific coordinates
     */
    void drawactor();

    /**
     * @brief getColor
     * @param type type of
     * @return QColor of ordered type of hextile
     */
    QColor getColor(std::string type);

    /**
     * @brief getPlrColor
     * @param player is players ID
     * @return QColor of ordered color of spesific player
     */
    QColor getPlrColor(int player);

    /**
     * @brief getActorFilePath
     * @param actor is actor type
     * @return path to resources
     */
    QString getActorFilePath(std::string actor);

    std::shared_ptr<Common::Hex> _hex;

    MainWindow *_mw;
    QGraphicsScene *_scene;

    std::vector<std::shared_ptr<Common::Pawn>> _pawnVec;
    std::vector<std::shared_ptr<QGraphicsEllipseItem>> _pawnGraphicsVec;
    std::shared_ptr<QGraphicsPixmapItem> _veh;
    std::shared_ptr<QGraphicsPixmapItem> _actor;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

};
}

#endif // MYHEXITEM_HH
