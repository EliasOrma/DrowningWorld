#include "myhexitem.hh"

#include <string>
#include <cmath>

#include <QGraphicsEllipseItem>

using p = std::pair<int,int>;

static unsigned int SIZEMULTIPLY=35;

// Equals to amount of layers written in Assets/pieces
static unsigned int TOTALRADIUS = 10;

// for drawing
static int w = sqrt(3)*SIZEMULTIPLY;
static int h = 2*SIZEMULTIPLY;
static int realh = 0.75*h;


Student::Myhexitem::Myhexitem(std::shared_ptr<Common::Hex> hexPtr,
                     QGraphicsScene *scene, MainWindow* mw):
                     _hex(hexPtr),_mw(mw),_scene(scene),_veh(nullptr),
                     _actor(nullptr)
{
    inithex();
}

void Student::Myhexitem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event)
    {
        _mw->setSelected(_hex->getCoordinates());
    }
}

QColor Student::Myhexitem::getColor(std::string type)
{
    QColor color;
    if(type == "Water"){color = Qt::blue;}
    else if(type == "Beach"){color = Qt::yellow;}
    else if(type == "Forest"){color = Qt::darkGreen;}
    else if(type == "Mountain"){color = Qt::gray;}
    else if(type == "Peak"){color = Qt::darkGray;}
    else if(type == "Coral"){color = Qt::darkCyan;}
    else {color = Qt::white;}
    return color;
}

QColor Student::Myhexitem::getPlrColor(int player)
{
    QColor color;
    int playerOne = 1;
    int playerTwo = 2;
    int playerThree = 3;

    if(player == playerOne) {color = Qt::red;}
    else if(player == playerTwo) {color = Qt::darkMagenta;}
    else if(player == playerThree) {color =  Qt::cyan;}
    else {color = Qt::green;} // Player 4
    return color;
}

QString Student::Myhexitem::getActorFilePath(std::string actor)
{
    QString path;
    if(actor == "kraken"){path = R"(:/pics/kraken.png)";}
    else if(actor == "shark"){path = R"(:/pics/hai.png)";}
    else if(actor == "seamunster"){path = R"(:/pics/merihirvio.png)";}
    else {path = R"(:/pics/vortex.png)";}
    return path;
}

void Student::Myhexitem::inithex()
{
    Common::CubeCoordinate coord = _hex->getCoordinates();
    int x = coord.x;
    int z = coord.z;
    int offset = (z+TOTALRADIUS)*w*0.5;

    std::string color = _hex->getPieceType();
    QColor brushcolor = getColor(color);
    QBrush brush(brushcolor);
    QPen pen(Qt::black);

    QPolygonF *poly = new QPolygonF;

    p hexcord [7] = {p(w, 0), p(1.5*w, h/4), p(1.5*w, (3*h)/4),
                     p(w, h), p(0.5*w, (3*h)/4), p(0.5*w, h/4), p(w, 0)};

    int nStop = 7;
    for (int n = 0 ;n != nStop; ++n)
    {
        int a = (hexcord[n].first) + (x*w) + offset;
        int b = (hexcord[n].second) + (z*realh);
        *poly << QPointF(a,b);
    }
    this->setPolygon(*poly);
    this->setBrush(brush);
    _scene->addItem(this);

    if (_hex->getPawnAmount() != 0)
    {
        drawpawns();
    }

    // displaying vehicle image
    if(_hex->getPieceType() == "Water")
    {
            // check if actor or vehicle
            if(_hex->getTransports().size() != 0)
            {
                drawvehicle();
            }

            if(_hex->getActors().size() != 0)
            {
                drawactor();
            }
    }
}

void Student::Myhexitem::drawvehicle()
{
    Common::CubeCoordinate coord = _hex->getCoordinates();
    int x = coord.x;
    int z = coord.z;
    int offset = (z+TOTALRADIUS)*w*0.5;

    if(_hex->getTransports().at(0)->getTransportType() == "boat")
    {
        QString pic = R"(:/pics/vene.png)";
        std::shared_ptr<QGraphicsPixmapItem> veh =
                std::make_shared<QGraphicsPixmapItem>(pic);

        veh->setScale(0.8);
        veh->setPos((0.5*w) + (x*w) + offset, (z*realh) + (0.1*realh));
        _veh = veh;
        _scene->addItem(veh.get());
    }
    else
    {
        QString pic = R"(:/pics/delfiini.png)";
        std::shared_ptr<QGraphicsPixmapItem> veh =
                std::make_shared<QGraphicsPixmapItem>(pic);

        veh->setScale(0.8);
        veh->setPos((0.5*w) + (x*w) + offset, (z*realh) + (0.1*realh));
        _veh = veh;
        _scene->addItem(veh.get());
    }
}

void Student::Myhexitem::drawactor()
{
    Common::CubeCoordinate coord = _hex->getCoordinates();
    int x = coord.x;
    int z = coord.z;
    int offset = (z+TOTALRADIUS)*w*0.5;


    std::shared_ptr<QGraphicsPixmapItem> actor =
            std::make_shared<QGraphicsPixmapItem>
            (getActorFilePath(_hex->getActors().at(0)->getActorType()));


    actor->setScale(0.8);
    actor->setPos((0.6*w) + (x*w) + offset, (z*realh) + (0.1*realh));
    _actor = actor;
    _scene->addItem(actor.get());
}

void Student::Myhexitem::drawpawns()
{
    Common::CubeCoordinate coord = _hex->getCoordinates();
    int x = coord.x;
    int z = coord.z;
    int offset = (z+TOTALRADIUS)*w*0.5;

    // for pawn different colors for different players!

    p pawnSlots [3] = {p((0.83*w) + (x*w) + offset, (z*realh) + (1.0*realh)),
                      p((1.03*w) + (x*w) + offset, (z*realh) + (1.0*realh)),
                      p((0.93*w) + (x*w) + offset, (z*realh) + (0.75*realh))};

    _pawnVec = _hex->getPawns();
    int pUsed = 0;

    std::vector<std::shared_ptr<Common::Pawn>>::iterator iStopCnd =
                                                         _pawnVec.end();

    for(auto p = _pawnVec.begin();p != iStopCnd; ++p)
    {
        QColor pcolor = getPlrColor(p->get()->getPlayerId());
        QBrush pawnBrush(pcolor);
        QPen pen(Qt::black);
        QRectF rect(pawnSlots[pUsed].first, pawnSlots[pUsed].second,
                    SIZEMULTIPLY*0.26, SIZEMULTIPLY*0.26);

        std::shared_ptr<QGraphicsEllipseItem> pawnitem =
                std::make_shared<QGraphicsEllipseItem>(rect);
        _pawnGraphicsVec.push_back(pawnitem);

        pawnitem->setBrush(pawnBrush);
        pawnitem->setPen(pen);

        _scene->addItem(pawnitem.get());

        ++pUsed;
    }
}

void Student::Myhexitem::updateHex()
{
    // For piecetype
    std::string color = _hex->getPieceType();
    QColor brushcolor = getColor(color);
    QBrush brush(brushcolor);
    this->setBrush(brush);

    // Actors
    _actor = nullptr;

    int actorVecSize = _hex->getActors().size();
    if(actorVecSize != 0)
    {
        drawactor();
    }

    // Vehicles
    _veh = nullptr;

    int vehicleVecSize = _hex->getTransports().size();
    if(vehicleVecSize != 0)
    {
        drawvehicle();
    }

    // Pawns
    unsigned long int pawnAmount = _hex->getPawnAmount();
    if(pawnAmount != _pawnGraphicsVec.size())
    {
        _pawnGraphicsVec.clear();
        drawpawns();
    }
}
