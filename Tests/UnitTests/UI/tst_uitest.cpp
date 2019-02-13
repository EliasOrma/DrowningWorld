#include <QString>
#include <QtTest>

#include "igameboard.hh"
#include "gameboard.hh"
#include "mainwindow.hh"
#include "myhexitem.hh"
#include "actorfactory.hh"

#include "boat.hh"
#include "dolphin.hh"
#include "kraken.hh"
#include "shark.hh"
#include "vortex.hh"

// Rantautuminen veneestä vie 3 vuoroa ja veneestä toiseen hyppääminen
//_board->addTransport(std::make_shared<Common::Boat>(3),Common::CubeCoordinate(-9,0,9));
//_board->addActor(std::make_shared<Common::Kraken>(1),Common::CubeCoordinate(-8,0,8));
//_board->addActor(std::make_shared<Common::Shark>(2),Common::CubeCoordinate(-9,1,8));

// shark eats swimmer
// _board->addActor(std::make_shared<Common::Shark>(1),Common::CubeCoordinate(-9,0,9));
// _board->movePawn(0,Common::CubeCoordinate(-9,0,9));

// shark doesnt eat pawns that are in boat
// _board->addActor(std::make_shared<Common::Shark>(1),Common::CubeCoordinate(-8,0,8));
// _board->addTransport(std::make_shared<Common::Boat>(3),Common::CubeCoordinate(-9,0,9));
// _board->movePawn(0,Common::CubeCoordinate(-9,0,9));
// _board->moveActor(1,Common::CubeCoordinate(-9,0,9))

// kraken destroys boat but doesnt kill pawns
// _board->addActor(std::make_shared<Common::Shark>(1),Common::CubeCoordinate(-9,0,9));
// _board->addTransport(std::make_shared<Common::Boat>(3),Common::CubeCoordinate(-9,0,9));

// seamunster eats boat and pawns

// vortex deletes everything around it
// _board->addTransport(std::make_shared<Common::Boat>(3),Common::CubeCoordinate(-9,1,8));
// _board->addActor(std::make_shared<Common::Shark>(1),Common::CubeCoordinate(-8,0,8));
// _board->addActor(std::make_shared<Common::Vortex>(2),Common::CubeCoordinate(-9,0,9));

// Check win
//_board->removePawn(1);
//_board->removePawn(2);
// _board->removePawn(3);
//  _board->removePawn(4);
//   _board->removePawn(5);
//   _board->movePawn(0,Common::CubeCoordinate(-1,1,0));

class UITest : public QObject
{
    Q_OBJECT

public:
    UITest();

private Q_SLOTS:
    void testCase1();
    void testShark();

private:
    std::shared_ptr<Common::IGameBoard> boardTester;

};

UITest::UITest()
{
}

void UITest::testCase1()
{
    QVERIFY2(true, "Failure");
}

void UITest::testShark()
{
    boardTester = std::make_shared<Student::GameBoard>();

}



QTEST_APPLESS_MAIN(UITest)

#include "tst_uitest.moc"
