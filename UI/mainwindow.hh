#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QObject>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPushButton>
#include <map>
#include <string>
#include <vector>
#include <QTimer>
#include <random>

#include "cubecoordinate.hh"
#include "hex.hh"
#include "igamerunner.hh"
#include "gameboard.hh"
#include "gamestate.hh"
#include "player.hh"

#include "actor.hh"
#include "myhexitem.hh"
#include "wheelitem.hh"
#include "illegalmoveexception.hh"
#include "dialogask.hh"
#include "dialogwin.hh"


/**
 *@brief when using mainwindow you HAVE to use method setobject
 * before using the whole mainwindow class
 */

namespace Student {
class Myhexitem;
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();

    virtual ~MainWindow();
    /**
     * @brief setObjects sets "must have"-private attributes for the object
     * @param board Student::GameBoard
     * @param state Student::GameState
     * @param p_vec Student::Player
     * @param runner Common::IGameRunner
     */
    void setObjects(std::shared_ptr<Student::GameBoard> board,
                    std::shared_ptr<Student::GameState> state,
                    std::vector<std::shared_ptr<Student::Player>> pvec,
                    std::shared_ptr<Common::IGameRunner> runner);

public slots:
    /**
     * @brief setSelected configs of click of a hex depending on gamephase
     * @param coordinates : coordinates of hex clicked
     */
    void setSelected(Common::CubeCoordinate coordinates);

    /**
     * @brief spinWheel uses gamerunner to get wheel values and calls
     * wheeltimes() function.
     */
    void spinWheel();

    /**
     * @brief animalWheelSpin calls wheelobject to spin itself
     */
    void animalWheelSpin();

    /**
     * @brief moveWheelSpin calls wheelobject to spin itself
     */
    void moveWheelSpin();

    /**
     * @brief giveUpMovement changes gamephase if user is moving pawns in
     * moving phase or moving actors in spinning phase
     */
    void giveUpMovement();

private:
    Ui::MainWindow *ui;

    /**
     * @brief gamesettings
     * Inits the objects game needs
     */
    void gamesettings();

    /**
     * @brief drawBoard Draws gameboard on the graphicsscene
     * @param _hexMap map(Coordinate : Hex object)
     */
    void drawBoard(std::map<Common::CubeCoordinate,
                   std::shared_ptr<Common::Hex>> _hexMap);


    /**
     * @brief setMovement sets coordinates for movement when hex is clicked
     * @param coordinates : coordinates of clicked hex
     */
    void setMovement(Common::CubeCoordinate coordinates);

    /**
     * @brief setSinking sets coordinates for sinking when hex is clicked
     * @param coordinates : coordinates of clicked hex
     */
    void setSinking(Common::CubeCoordinate coordinates);

    /**
     * @brief setSpinning sets coordinates for Vehicleactor movement when hex
     * is clicked
     * @param coordinates : coordinates of clicked hex
     */
    void setSpinningVehicle(Common::CubeCoordinate coordinates);

    /**
     * @brief setSpinning sets coordinates for Actormovement when hex is
     * clicked. Doesn't set the coordinate if there is no right actor
     * in the clicked hex. Doesn't set the end coordinate if there is
     * dolphin at the end hex.
     * @param coordinates : coordinates of clicked hex
     */
    void setSpinningActor(Common::CubeCoordinate coordinates);

    /**
     * @brief movementPawn checks if pawn can move from coordinate to
     * another when user is trying to move a pawn
     * @param pawn : pawn that is owned by the player who is in turn
     */
    void movementPawn(std::shared_ptr<Common::Pawn> pawn);

    /**
     * @brief movementVehicle checks if pawn can move from coordinate to
     * another when user is trying to move a vehicle which has players owned
     * pawn and changes gamephase
     * @param pawn : pawn that is owned by the player who is in turn
     */
    void movementVehicle(std::shared_ptr<Common::Pawn> pawn);

    /**
     * @brief movementOutOfVehicle moves pawn from boat to land or
     * to another vehicle. Changes gamephase when completed
     * @param pawn : pawn that is owned by the player who is in turn
     */
    void movementOutOfVehicle(std::shared_ptr<Common::Pawn> pawn);

    /**
     * @brief playgame calls different game functions depending on gamephase
     */
    void playgame();

    /**
     * @brief movement moves pawn or vehicle if activecoord1 and 2 are set
     * and the movement is legal, sets errormessages and changes gamephase
     * when actions are done
     */
    void movement();

    /**
     * @brief sinking flips tile if activecoord1 is set and flip is legal,
     * sets errormessages and changes gamephase when actions are done
     */
    void sinking();

    /**
     * @brief spinning checks if actor to move is available and moves actor
     * if movement is avaliable and move is legal, sets errormessages and
     * changes gamephase when actions are done.
     */
    void spinning();

    /**
     * @brief setmsg sets chosen messages to mainwindow
     * @param endstr message that is related to inform player
     * @param errormsg message that is related to inform about errors
     */
    void setmsg(std::string endstr, std::string errormsg);

    /**
     * @brief wheelTimes sets the times for wheels to spins, call functions
     * for spinning them.
     */
    void wheelTimes();

    /**
     * @brief configWheelWait count the amount of millisecond to spin the wheel
     * @return amount of ms to spin the wheel
     */
    int configWheelWait(bool animal);

    /**
     * @brief wait waits for amount of mseconds that has been given as a param
     * before continuing function where it is called
     * @param msecs am
     */
    void wait(int msecs);

    /**
     * @brief gameEndCheck checks if game should be ended and ends game
     * if needed.
     */
    void gameEndCheck();

    /**
     * @brief changePhase changes gamephase changes game phase to next phase
     */
    void changePhase();

    QGraphicsScene *scene;
    QGraphicsScene *wheelScene;
    std::shared_ptr<Student::WheelItem> _wheelAnimal;
    std::shared_ptr<Student::WheelItem> _wheelMoves;
    QTimer *_timerAnimal;
    QTimer *_timerMove;

    std::map<Common::CubeCoordinate,
        std::shared_ptr<Student::Myhexitem>> _hexItemMap;

    std::shared_ptr<Student::GameBoard> _board;
    std::shared_ptr<Student::GameState> _state;
    std::vector<std::shared_ptr<Student::Player>> _pvec;
    std::shared_ptr<Common::IGameRunner> _runner;

    Common::CubeCoordinate activeCoord1;
    Common::CubeCoordinate activeCoord2;
    bool activeCoord1set;
    bool activeCoord2set;

    bool freezeclick;
    bool wheelspinned;
    bool _inActorMovement;

    std::pair<std::string,std::string> _spinvalues;
};

#endif // MAINWINDOW_HH
