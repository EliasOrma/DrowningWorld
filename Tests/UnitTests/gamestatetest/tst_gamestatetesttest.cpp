#include "gamestate.hh"
#include <QString>
#include <QtTest>
#include <memory>

class GamestatetestTest : public QObject
{
    Q_OBJECT

public:
    GamestatetestTest();

private Q_SLOTS:
    void testCase1();
    void testCurrentPhase();
    void testCurrentPlayer();
    void testPhaseChange();
    void testChangePlayerTurn();

};

GamestatetestTest::GamestatetestTest()
{
}

void GamestatetestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

void GamestatetestTest::testCurrentPhase()
{
    std::shared_ptr<Common::IGameState> state = std::make_shared<Student::GameState>();
    Common::GamePhase phase = Common::GamePhase::MOVEMENT;
    QVERIFY(state->currentGamePhase() == phase);
}

void GamestatetestTest::testCurrentPlayer()
{
    std::shared_ptr<Common::IGameState> state = std::make_shared<Student::GameState>();
    int player = 1;
    QVERIFY(state->currentPlayer() == player);
}

void GamestatetestTest::testPhaseChange()
{
    std::shared_ptr<Common::IGameState> state = std::make_shared<Student::GameState>();
    state->changeGamePhase(Common::GamePhase::SPINNING);
    QVERIFY(state->currentGamePhase() == Common::GamePhase::SPINNING);
}

void GamestatetestTest::testChangePlayerTurn()
{
    std::shared_ptr<Common::IGameState> state = std::make_shared<Student::GameState>();
    state->changePlayerTurn(2);
    QVERIFY(state->currentPlayer() == 2);
}


QTEST_APPLESS_MAIN(GamestatetestTest)

#include "tst_gamestatetesttest.moc"
