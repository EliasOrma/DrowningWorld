#include <QString>
#include <QtTest>
#include <vector>

#include "gameboard.hh"
#include "hex.hh"
#include "pawn.hh"
#include "transport.hh"
#include "dolphin.hh"
#include "boat.hh"
#include "actor.hh"
#include "kraken.hh"
#include "seamunster.hh"
#include "shark.hh"
#include "vortex.hh"

// Constant for aiding Hex's NeighbourVector accessing.
const int TST_MAX_SIDES = 6;

// Because Hex-tile's type can't effect the functionality of gameboard. (except for "Water" tile's)
const std::string TST_HEXTYPE = "None";

const std::string TST_DEFAULT_ACTOR_TYPE = "Shark";
const std::string TST_DEFAULT_TRANSPORT_TYPE = "Boat";

class GameBoardTest : public QObject
{
    Q_OBJECT

public:
    GameBoardTest();

private Q_SLOTS:
    void init();
    void testConsturctor();

    // Hex additions
    void testAddHex();
    void testAddHexReplace();

    // Tiletype
    void testIsWaterTile();

    // "Piece" additions"
    void testAddActor();
    void testAddPawn();
    void testAddTransport();

    // Tileoccupation
    void testCheckTileOccupationNonExistent();
    void testCheckTileOccupation();

    // Actor movement
    void testMoveActor();
    void testMoveActorNonExistentTile();

    // Actor removal
    void testRemoveActor();
    void testRemoveActorCorrect();

    // Pawn movement
    void testMovePawn();
    void testMovePawnNonExistentTile();

    // Pawn removal
    void testRemovePawn();
    void testRemovePawnFullHex();
    void testRemovePawnCorrectHex();

    // Transport movement
    void testMoveTransport();
    void testMoveTransportNonExistentTile();

    // Transport removal
    void testRemoveTransport();
    void testRemoveTransportCorrect();

private:
    Common::CubeCoordinate center_;
    std::shared_ptr<Common::IGameBoard> board_;

    void generateTileCircle(int range);
    std::vector<Common::CubeCoordinate> addHex(
            Common::CubeCoordinate coord,
            std::string pieceType);
    std::shared_ptr<Common::Pawn> addPawn(
            int pawnid,
            int playerid,
            Common::CubeCoordinate coord);
    std::shared_ptr<Common::Actor> addActor(
            int actorid,
            Common::CubeCoordinate coord,
            std::string type);
    std::shared_ptr<Common::Transport> addTransport(
            int transportid,
            Common::CubeCoordinate coord,
            std::string type);
};

GameBoardTest::GameBoardTest():
    center_(0, 0 ,0),
    board_(nullptr)
{
}

void GameBoardTest::init()
{
    board_.reset();
    board_= std::make_shared<Student::GameBoard>();
}

void GameBoardTest::testConsturctor()
{

    //centerHex->getCoordinates();
    QVERIFY(board_->getHex(center_) == nullptr);

}

void GameBoardTest::testAddHex()
{
    QVERIFY(board_->getHex(center_) == nullptr);
    std::shared_ptr<Common::Hex> newhex(new Common::Hex);
    newhex->setCoordinates(center_);
    board_->addHex(newhex);
    QVERIFY(board_->getHex(center_) == newhex);
}

void GameBoardTest::testAddHexReplace()
{
    std::shared_ptr<Common::Hex> newhex(new Common::Hex);
    newhex->setCoordinates(center_);
    board_->addHex(newhex);
    QVERIFY(board_->getHex(center_) == newhex);

    std::shared_ptr<Common::Hex> replacehex(new Common::Hex);
    replacehex->setCoordinates(center_);
    board_->addHex(replacehex);
    QVERIFY(board_->getHex(center_) == replacehex);
}

void GameBoardTest::testIsWaterTile()
{
    addHex(center_, TST_HEXTYPE);
    QVERIFY(not board_->isWaterTile(center_));

    addHex(center_, "Water");
    QVERIFY(board_->isWaterTile(center_));
}

void GameBoardTest::testAddActor()
{
    addHex(center_, TST_HEXTYPE);
    QVERIFY(board_->getHex(center_)->getActors().size() == 0);
    addActor(1, center_, TST_DEFAULT_ACTOR_TYPE);
    QVERIFY(board_->getHex(center_)->getActors().size() == 1);
}

void GameBoardTest::testAddPawn()
{
    addHex(center_, TST_HEXTYPE);
    QVERIFY(board_->getHex(center_)->getPawns().size() == 0);
    addPawn(1, 1, center_);
    QVERIFY(board_->getHex(center_)->getPawns().size() == 1);
}

void GameBoardTest::testAddTransport()
{
    addHex(center_, TST_HEXTYPE);
    QVERIFY(board_->getHex(center_)->getTransports().size() == 0);
    addTransport(1, center_, TST_DEFAULT_TRANSPORT_TYPE);
    QVERIFY(board_->getHex(center_)->getTransports().size() == 1);
}

void GameBoardTest::testCheckTileOccupationNonExistent()
{
    QCOMPARE(board_->checkTileOccupation(center_), -1);
}

void GameBoardTest::testCheckTileOccupation()
{
    addHex(center_, TST_HEXTYPE);
    QVERIFY(board_->checkTileOccupation(center_) == 0);
    for( int pawncount = 1; pawncount < 4; ++pawncount)
    {
        addPawn(pawncount, 1, center_);
        QVERIFY(board_->checkTileOccupation(center_) == pawncount);
    }
}

void GameBoardTest::testMoveActor()
{
    generateTileCircle(1);

    for(int i = 0; i < TST_MAX_SIDES; ++i)
    {
        addActor(i, center_, TST_DEFAULT_ACTOR_TYPE);
        QVERIFY(board_->getHex(center_)->giveActor(i));

        Common::CubeCoordinate target =
                board_->getHex(center_)->getNeighbourVector().at(i);
        QVERIFY(!board_->getHex(target)->giveActor(i));
        board_->moveActor(i, target);
        QVERIFY(!board_->getHex(center_)->giveActor(i));
        QVERIFY(board_->getHex(target)->giveActor(i));
    }
}

void GameBoardTest::testMoveActorNonExistentTile()
{
    generateTileCircle(1);
    int actorID = 0;
    Common::CubeCoordinate coord(-1, 0, 1);
    std::vector<Common::CubeCoordinate> neighbours;

    for(int dir = 0; dir < TST_MAX_SIDES; ++dir)
    {
        actorID = dir;
        addActor(actorID, coord, TST_DEFAULT_ACTOR_TYPE);
        neighbours = board_->getHex(coord)->getNeighbourVector();

        for(int i = 3; i < 6; ++i)
        {
            QVERIFY(board_->getHex(coord)->giveActor(actorID));
            Common::CubeCoordinate nonexistent =
                    neighbours.at((dir + i) % TST_MAX_SIDES);
            board_->moveActor(actorID, nonexistent);
            QVERIFY(board_->getHex(coord)->giveActor(actorID));
        }
        coord = neighbours.at(dir);
    }

}

void GameBoardTest::testRemoveActor()
{
    int actorID = 1;
    addHex(center_, TST_HEXTYPE);
    addActor(actorID, center_, TST_DEFAULT_ACTOR_TYPE);
    QVERIFY(board_->getHex(center_)->giveActor(actorID));
    board_->removeActor(actorID);
    QVERIFY(!board_->getHex(center_)->giveActor(actorID));
}

void GameBoardTest::testRemoveActorCorrect()
{
    generateTileCircle(2);
    std::vector<Common::CubeCoordinate> neighbours =
            board_->getHex(center_)->getNeighbourVector();

    int actorID = 1;
    addActor(actorID, center_, TST_DEFAULT_ACTOR_TYPE);
    QVERIFY(board_->getHex(center_)->giveActor(actorID));

    for(int i = 0; i < TST_MAX_SIDES; ++i)
    {
        addActor(i+2, neighbours.at(i), TST_DEFAULT_ACTOR_TYPE);
        QVERIFY(board_->getHex(neighbours.at(i))->giveActor(i+2));
    }

    board_->removeActor(actorID);
    QVERIFY(!board_->getHex(center_)->giveActor(actorID));

    for(int i = 0; i < TST_MAX_SIDES; ++i)
    {
        QVERIFY(board_->getHex(neighbours.at(i))->giveActor(i+2));
    }

}

void GameBoardTest::testMovePawn()
{
    generateTileCircle(2);

    for(int i = 0; i < TST_MAX_SIDES; ++i)
    {
        addPawn(i, 1, center_);
        QVERIFY(board_->checkTileOccupation(center_) == 1);

        Common::CubeCoordinate target =
                board_->getHex(center_)->getNeighbourVector().at(i);
        QVERIFY(board_->checkTileOccupation(target) == 0);
        board_->movePawn(i, target);
        QVERIFY(board_->checkTileOccupation(center_) == 0);
        QVERIFY(board_->checkTileOccupation(target) == 1);
    }

}

void GameBoardTest::testMovePawnNonExistentTile()
{
    generateTileCircle(1);
    int pawnID = 1;
    int playerID = 1;
    Common::CubeCoordinate coord(-1, 0, 1);
    std::vector<Common::CubeCoordinate> neighbours;

    for(int dir = 0; dir < TST_MAX_SIDES; ++dir)
    {
        pawnID = dir;
        addPawn(pawnID, playerID, coord);
        neighbours = board_->getHex(coord)->getNeighbourVector();

        for(int i = 3; i < 6; ++i)
        {
            QVERIFY(board_->getHex(coord)->givePawn(pawnID));
            Common::CubeCoordinate nonexistent =
                    neighbours.at((dir + i) % TST_MAX_SIDES);
            board_->movePawn(pawnID, nonexistent);
            QVERIFY(board_->getHex(coord)->givePawn(pawnID));
        }

        coord = neighbours.at(dir);
    }

}

void GameBoardTest::testRemovePawn()
{
    addHex(center_, TST_HEXTYPE);
    addPawn(1, 1, center_);

    QVERIFY(board_->checkTileOccupation(center_) == 1);
    board_->removePawn(1);
    QVERIFY(board_->checkTileOccupation(center_) == 0);
}

void GameBoardTest::testRemovePawnFullHex()
{
    addHex(center_, TST_HEXTYPE);

    for(int i = 0; i < 3; ++i)
    {
        addPawn(i, 1, center_);
        QVERIFY(board_->getHex(center_)->givePawn(i) != nullptr);
    }
    board_->removePawn(1);
    QVERIFY(board_->getHex(center_)->givePawn(1) == nullptr);
    QVERIFY(board_->getHex(center_)->givePawn(0) != nullptr);
    QVERIFY(board_->getHex(center_)->givePawn(2) != nullptr);
}

void GameBoardTest::testRemovePawnCorrectHex()
{
    generateTileCircle(2);

    std::vector<Common::CubeCoordinate> neighbours =
            board_->getHex(center_)->getNeighbourVector();
    for(int i = 0; i < TST_MAX_SIDES; ++i)
    {
        QVERIFY(board_->getHex(neighbours.at(i))->givePawn(i) == nullptr);
        addPawn(i, 1, neighbours.at(i));
        QVERIFY(board_->getHex(neighbours.at(i))->givePawn(i) != nullptr);
    }

    for(int i = TST_MAX_SIDES - 1; i >= 0; --i)
    {
        board_->removePawn(i);
        QVERIFY(board_->getHex(neighbours.at(i))->givePawn(i) == nullptr);
        for(int j = i-1; i >= 0; --i)
        {
            QVERIFY(board_->getHex(neighbours.at(j))->givePawn(j) != nullptr);
        }
    }
}

void GameBoardTest::testMoveTransport()
{
    generateTileCircle(1);

    for(int i = 0; i < TST_MAX_SIDES; ++i)
    {
        addTransport(i, center_, TST_DEFAULT_TRANSPORT_TYPE);
        QVERIFY(board_->getHex(center_)->giveTransport(i));

        Common::CubeCoordinate target =
                board_->getHex(center_)->getNeighbourVector().at(i);
        QVERIFY(!board_->getHex(target)->giveTransport(i));
        board_->moveTransport(i, target);
        QVERIFY(!board_->getHex(center_)->giveTransport(i));
        QVERIFY(board_->getHex(target)->giveTransport(i));
    }
}

void GameBoardTest::testMoveTransportNonExistentTile()
{
    generateTileCircle(1);
    int transportID = 0;
    Common::CubeCoordinate coord(-1, 0, 1);
    std::vector<Common::CubeCoordinate> neighbours;

    for(int dir = 0; dir < TST_MAX_SIDES; ++dir)
    {
        transportID = dir;
        addTransport(transportID, coord, TST_DEFAULT_TRANSPORT_TYPE);
        neighbours = board_->getHex(coord)->getNeighbourVector();

        for(int i = 3; i < 6; ++i)
        {
            QVERIFY(board_->getHex(coord)->giveTransport(transportID));
            Common::CubeCoordinate nonexistent =
                    neighbours.at((dir + i) % TST_MAX_SIDES);
            board_->moveTransport(transportID, nonexistent);
            QVERIFY(board_->getHex(coord)->giveTransport(transportID));
        }
        coord = neighbours.at(dir);
    }
}

void GameBoardTest::testRemoveTransport()
{
    int transportID = 1;
    addHex(center_, TST_HEXTYPE);
    addTransport(transportID, center_, TST_DEFAULT_TRANSPORT_TYPE);
    QVERIFY(board_->getHex(center_)->giveTransport(transportID));
    board_->removeTransport(transportID);
    QVERIFY(!board_->getHex(center_)->giveTransport(transportID));
}

void GameBoardTest::testRemoveTransportCorrect()
{
    generateTileCircle(2);
    std::vector<Common::CubeCoordinate> neighbours =
            board_->getHex(center_)->getNeighbourVector();

    int transportID = 1;
    addTransport(transportID, center_, TST_DEFAULT_TRANSPORT_TYPE);
    QVERIFY(board_->getHex(center_)->giveTransport(transportID));

    for(int i = 0; i < TST_MAX_SIDES; ++i)
    {
        addTransport(i+2, neighbours.at(i), TST_DEFAULT_TRANSPORT_TYPE);
        QVERIFY(board_->getHex(neighbours.at(i))->giveTransport(i+2));
    }

    board_->removeTransport(transportID);
    QVERIFY(!board_->getHex(center_)->giveTransport(transportID));

    for(int i = 0; i < TST_MAX_SIDES; ++i)
    {
        QVERIFY(board_->getHex(neighbours.at(i))->giveTransport(i+2));
    }

}

void GameBoardTest::generateTileCircle(int range)
{
    Common::CubeCoordinate coord;
    addHex(Common::CubeCoordinate(0,0,0), TST_HEXTYPE);
    for(int currentrange = 0; currentrange <= range; ++currentrange)
    {
        coord = Common::CubeCoordinate(-currentrange, 0, currentrange);
        for(int side = 0; side < TST_MAX_SIDES; ++side)
        {
            for(int tile = 0; tile < currentrange; ++tile)
            {
                std::vector<Common::CubeCoordinate> neighbours =
                        addHex(coord, TST_HEXTYPE);

                coord = neighbours.at(side);
            }
        }
    }
}

std::vector<Common::CubeCoordinate> GameBoardTest::addHex(
        Common::CubeCoordinate coord,
        std::string pieceType)
{
    std::shared_ptr<Common::Hex> newHex = std::make_shared<Common::Hex>();
    newHex->setCoordinates(coord);
    newHex->setPieceType(pieceType);
    board_->addHex(newHex);

    return newHex->getNeighbourVector();
}

std::shared_ptr<Common::Pawn> GameBoardTest::addPawn(
        int pawnid,
        int playerid,
        Common::CubeCoordinate coord)
{
    std::shared_ptr<Common::Pawn> newpawn =
            std::make_shared<Common::Pawn>(pawnid, playerid, coord);
    board_->addPawn(playerid, pawnid, coord);

    return newpawn;
}

std::shared_ptr<Common::Actor> GameBoardTest::addActor(
        int actorid,
        Common::CubeCoordinate coord,
        std::string type)
{
    std::shared_ptr<Common::Actor> newactor;
    if( type == "Kraken")
    {
       newactor = std::make_shared<Common::Kraken>(actorid);
    }
    else if (type == "Seamunster")
    {
        newactor = std::make_shared<Common::Seamunster>(actorid);
    }
    else if(type == "Shark")
    {
        newactor = std::make_shared<Common::Shark>(actorid);
    }
    else if(type == "Vortex")
    {
        newactor = std::make_shared<Common::Vortex>(actorid);
    }

    board_->addActor(newactor, coord);
    return newactor;
}

std::shared_ptr<Common::Transport> GameBoardTest::addTransport(
        int transportid,
        Common::CubeCoordinate coord,
        std::string type)
{
    std::shared_ptr<Common::Transport> newtransport;
    if( type == "Dolphin")
    {
        newtransport = std::make_shared<Common::Dolphin>(transportid);
    }
    else if ( type == "Boat")
    {
        newtransport = std::make_shared<Common::Boat>(transportid);
    }

    board_->addTransport(newtransport, coord);
    return newtransport;
}


QTEST_APPLESS_MAIN(GameBoardTest)

#include "tst_gameboardtest.moc"
