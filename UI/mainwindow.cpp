#include "mainwindow.hh"
#include "ui_mainwindow.h"


#include <utility>
#include <vector>
#include <cmath>
#include <string>
#include <map>
#include <QString>
#include <QMouseEvent>
#include <cstdlib>

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    wheelScene = new QGraphicsScene(this);
    ui->Forwheel->setScene(wheelScene);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setObjects(std::shared_ptr<Student::GameBoard> board,
                            std::shared_ptr<Student::GameState> state,
                            std::vector<std::shared_ptr<Student::Player>> pvec,
                            std::shared_ptr<Common::IGameRunner> runner)
{
    _board = board;
    _state = state;
    _pvec = pvec;
    _runner = runner;
    gamesettings();
}

void MainWindow::gameEndCheck()
{
    if(!_board->noPawnsLeft())
    {
        return;
    }
    // Ends up here if game should be ended
    int winnerPlayer = 0;
    bool tie = false;
    int savedPawns = 0;

    std::vector<std::shared_ptr<Student::Player>>::iterator iter;
    std::vector<std::shared_ptr<Student::Player>>::iterator iterend
            = _pvec.end();

    for(iter = _pvec.begin();iter != iterend; ++iter)
    {
        int newSavedPawns = iter->get()->getSavedPawns();
        int playerId = iter->get()->getPlayerId();
        if(newSavedPawns > savedPawns)
        {
            savedPawns = newSavedPawns;
            winnerPlayer = playerId;
            tie = false;
        }
        else
        {
            if(newSavedPawns == savedPawns)
            {
                tie = true;
            }
        }
    }

    DialogWIn dlog;
    std::string msg;
    if(tie)
    {
        msg = "Match is a tie";
    }
    else
    {
        msg = "Player" + std::to_string(winnerPlayer) + " has won!";
        dlog.setWinner(winnerPlayer,savedPawns);
    }
    dlog.setEndmsg(msg);
    dlog.exec();
    destroy();
}

void MainWindow::gamesettings()
{
    // White backboard for zoom
    int xAndY = -1000;
    int wAndH = 3000;
    scene->addRect(xAndY,xAndY,wAndH,wAndH,QPen(Qt::white),QBrush(Qt::white));

    _wheelAnimal = std::make_shared<Student::WheelItem>(wheelScene,0.8);
    _wheelMoves = std::make_shared<Student::WheelItem>(wheelScene,0.3);
    _wheelAnimal->setPixmap(QPixmap(R"(:/pics/kiekkoAnimal.png)"));
    _wheelMoves->setPixmap(QPixmap(R"(:/pics/kiekkoMoves.png)"));

    // Initing arrow image
    QGraphicsPixmapItem *pointerArrow =
            new QGraphicsPixmapItem(QPixmap(R"(:/pics/pointer.png)"));
    double scaleForArrow = 0.1;
    int arrowInitRotation = 180;
    pointerArrow->setScale(scaleForArrow);
    pointerArrow->setRotation(arrowInitRotation);

    double width = (_wheelAnimal->boundingRect().width()/2) +
            ((pointerArrow->boundingRect().width()*scaleForArrow)/2);
    double height = pointerArrow->boundingRect().height()*scaleForArrow;
    // Setting arrow in scene
    QPointF positionForArrow (width,height);
    pointerArrow->setPos(positionForArrow);
    wheelScene->addItem(pointerArrow);

    freezeclick = false;
    _inActorMovement = false;
    activeCoord1set = false;
    activeCoord2set = false;

    // Making timers for spinning the wheels
    _timerAnimal = new QTimer();
    _timerMove = new QTimer();
    connect(_timerAnimal,SIGNAL(timeout()),this,SLOT(animalWheelSpin()));
    connect(_timerMove,SIGNAL(timeout()),this,SLOT(moveWheelSpin()));

    int pawnNum = 0;
    std::vector<std::shared_ptr<Student::Player>>::iterator iter;
    std::vector<std::shared_ptr<Student::Player>>::iterator iterEnd
            = _pvec.end();

    for(iter = _pvec.begin(); iter != iterEnd; ++iter)
    {
        int PawnsLimit = 0;
        for(int pawn = iter->get()->getpawnsleft();
            pawn != PawnsLimit; --pawn)
        {
            _board->addPawn(iter->get()->getPlayerId(),pawnNum);
            ++pawnNum;
        }
    }
    int playersMin = 2;
    int playersMax = 4;
    int playerVectorSize = _pvec.size();
    if(playerVectorSize > playersMin)
    {
        ui->playerslot3->setText("Player 3 Color: Cyan");

        if(playerVectorSize == playersMax)
        {
            ui->playerslot4->setText("Player 4 Color: Green");
        }
    }
    drawBoard(_board->getHexMap());
}

void MainWindow::drawBoard(std::map<Common::CubeCoordinate,
                           std::shared_ptr<Common::Hex>> hexMap)
{
    if(!hexMap.empty())
    {
        for (auto i:hexMap)
        {
            std::shared_ptr<Student::Myhexitem> hexagon =
                    std::make_shared<Student::Myhexitem>(i.second, scene, this);
            _hexItemMap[i.second->getCoordinates()] = hexagon;
        }
    }
}

void MainWindow::spinWheel()
{
    if(_state->currentGamePhase()==Common::GamePhase::SPINNING)
    {
        _spinvalues = _runner->spinWheel();
        wheelspinned = true;
        wheelTimes();
    }
}

void MainWindow::wheelTimes()
{
    int firstwait = configWheelWait(true);
    int secondwait = configWheelWait(false);
    _wheelAnimal->resetRotation();
    _wheelMoves->resetRotation();

    // spin's animal wheel
    _timerAnimal->start(1);
    wait(firstwait);
    _timerAnimal->stop();

    // Spin's movement wheel
    _timerMove->start(1);
    wait(secondwait);
    _timerMove->stop();

    setmsg("Select " + _spinvalues.first,"");
    playgame();
}

int MainWindow::configWheelWait(bool animal)
{
    // Degrees of animals in wheel
    // dolphin, 1 = 0-90; seamuster, D = 90-180; shark,
    // 3 = 180-270; kraken, 2 = 270-360;
    std::pair<int,int> rotList [4] = {{0,90},{90,180},{180,270},{270,360}};

    // Spins atleast 2 rounds in every spin event
    int totalwait = 2 * 360;
    int baseRot = 45;
    int rotListIndex = 0;

    // this if else gets the needed int-degree pair from the rotList
    if(animal)
    {
        std::string actor = _spinvalues.first;
        if(actor == "kraken")
        {
            rotListIndex = 3;
        }
        else if(actor == "shark")
        {
            rotListIndex = 2;
        }
        else if(actor == "seamunster")
        {
            rotListIndex = 1;
        }
        else
        {
            rotListIndex = 0;
        }
    }
    else
    {
        std::string move = _spinvalues.second;
        if(move == "2")
        {
            rotListIndex = 3;
        }
        else if(move == "3")
        {
            rotListIndex = 2;
        }
        else if(move == "D")
        {
            rotListIndex = 1;
        }
        else
        {
            rotListIndex = 0;
        }
    }

    std::pair<int,int> usedRotLimit = rotList[rotListIndex];

    // When rotation is in between of degree pirs, while loop stops
    while(!((baseRot > usedRotLimit.first) && (baseRot < usedRotLimit.second)))
    {
        int radAdd = 90;
        totalwait += radAdd;
        baseRot += radAdd;
    }
    return totalwait;
}

void MainWindow::wait(int msecs)
{
    freezeclick = true;
    QTime dieTime = QTime::currentTime().addMSecs(msecs);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    freezeclick = false;
}

void MainWindow::animalWheelSpin()
{
    _wheelAnimal->spin();
}

void MainWindow::moveWheelSpin()
{
    _wheelMoves->spin();
}

void MainWindow::setmsg(std::string endstr, std::string errormsg)
{
    std::string actionstr = "Player" + std::to_string(_state->currentPlayer())
            + " " + endstr;

    ui->Errormessage->setText(QString::fromStdString(errormsg));
    ui->Action->setText(QString::fromStdString(actionstr));
}

void MainWindow::giveUpMovement()
{
    if(_state->currentGamePhase() == Common::GamePhase::MOVEMENT)
    {
        activeCoord1set = false;
        activeCoord2set = false;
        changePhase();
        return;
    }
    if((_state->currentGamePhase() == Common::GamePhase::SPINNING)
            && _inActorMovement)
    {
        activeCoord1set = false;
        activeCoord2set = false;
        changePhase();
    }
}

void MainWindow::changePhase()
{
    Common::GamePhase phase = _state->currentGamePhase();
    if(phase == Common::GamePhase::MOVEMENT)
    {
        // Init player actions
        _pvec.at(_state->currentPlayer()-1)->resetActions();

        _state->changeGamePhase(Common::GamePhase::SINKING);
        setmsg("select tile to flip","");
    }
    else if( phase == Common::GamePhase::SINKING)
    {
        _state->changeGamePhase(Common::GamePhase::SPINNING);
        setmsg("spin wheels","");
    }
    else
    {
        int playerVectorSize = _pvec.size();
        if(_state->currentPlayer() == playerVectorSize)
        {
            _state->changePlayerTurn(1);
        }
        else
        {
            _state->changePlayerTurn(_state->currentPlayer() + 1);
        }

        _state->changeGamePhase(Common::GamePhase::MOVEMENT);
        setmsg("select pawn","");
    }
    gameEndCheck();
}


void MainWindow::setMovement(Common::CubeCoordinate coordinates)
{
    // Sets movements starting coordinate of the movement if player has
    // pawns in the selected coordinate

    // Gets index of the players owned pawn
    int index = _board->checkPawnPropertyPlace(coordinates,_state->
                                               currentPlayer());
    if((!activeCoord1set) && (index != -1))
    {
        activeCoord1 = coordinates;
        activeCoord1set = true;
        setmsg("select the hex to move to","");
    }
    // Sets the ending coordineate of the movement if the selected
    // coordinate is not starting coordinhate
    if(activeCoord1set && (!(coordinates == activeCoord1)))
    {
        activeCoord2 = coordinates;
        activeCoord2set = true;
    }
}

void MainWindow::setSinking(Common::CubeCoordinate coordinates)
{
    Dialogask w;
    // Sets sinking coordinate if dialogask emits signal accepted()
    // (If player presses "Yes")
    if(w.exec() == QDialog::Accepted)
    {
        activeCoord1 = coordinates;
        activeCoord1set = true;
    }
}

void MainWindow::setSpinningVehicle(Common::CubeCoordinate coordinates)
{
    std::string actorType = _spinvalues.first;

    int transportVecSize = _board->getHex(coordinates)->getTransports().size();
    // if the starting coordinate is not set and there is
    // actor in selected coordinate
    if((!activeCoord1set) && (transportVecSize != 0))
    {
        activeCoord1 = coordinates;
        activeCoord1set = true;
        setmsg("select the hex to move to","");
    }
    else if (activeCoord1set && (!(coordinates == activeCoord1)))
    {
        if(transportVecSize == 0)
        {
            activeCoord2 = coordinates;
            activeCoord2set = true;
        }
    }
    else
    {
        return;
    }

}

void MainWindow::setSpinningActor(Common::CubeCoordinate coordinates)
{
    std::string actorType = _spinvalues.first;
    int actorVecSize = _board->getHex(coordinates)->getActors().size();

    // Checks if the end hex has dolphin
    bool isDolphin = false;
    int vehVecSize = _board->getHex(coordinates)->getTransports().size();
    if(vehVecSize != 0)
    {
        if(_board->getHex(coordinates)->getTransports().at(0)->
                getTransportType() == "dolphin")
        {
            isDolphin = true;
        }
    }

    // if the starting coordinate is not set and there is actor in
    // selected coordinate
    if((!activeCoord1set) && (actorVecSize != 0))
    {
        // Sets the starting coordinate if right actor is in the
        // tile
        std::string hexesActorType = _board->getHex(coordinates)->
                getActors().at(0)
                ->getActorType();
        if(hexesActorType == actorType)
        {
            activeCoord1 = coordinates;
            activeCoord1set = true;
            setmsg("select the hex to move to","");
        }
    }
    // sets end coordinate if its not starting coordinate and
    // there is no actors in destination coordinate
    else
    {
        if (activeCoord1set && (!(coordinates == activeCoord1)))
        {
            if((actorVecSize == 0) && (!isDolphin))
            {
                activeCoord2 = coordinates;
                activeCoord2set = true;
            }
        }
    }
}


void MainWindow::setSelected(Common::CubeCoordinate coordinates)
{
    if(freezeclick)
    {
        return;
    }

    else if(_state->currentGamePhase()==Common::GamePhase::MOVEMENT)
    {
        setMovement(coordinates);
    }

    else if(_state->currentGamePhase()==Common::GamePhase::SINKING)
    {
        setSinking(coordinates);
    }

    else
    {
        if(wheelspinned)
        {
            std::string actorType = _spinvalues.first;
            if(actorType == "dolphin")
            {
                // Dolphin is a vehicle
                setSpinningVehicle(coordinates);
            }
            else
            {
                // For moving any other actor that dolphin
                setSpinningActor(coordinates);
            }
        }
    }
    playgame();
}

void MainWindow::playgame()
{
    Common::GamePhase phase =_state->currentGamePhase();
    if(phase == Common::GamePhase::MOVEMENT)
    {
        movement();
    }
    else if( phase == Common::GamePhase::SINKING)
    {
        if(activeCoord1set)
        {
            sinking();
        }
    }
    else
    {
        if(wheelspinned)
        {
            spinning();
        }
    }
}

void MainWindow::movementPawn(std::shared_ptr<Common::Pawn> pawn)
{
    int pawnId = pawn->getId();
    try
    {
        int Actleft = _runner->movePawn(activeCoord1,activeCoord2,pawnId);

        // Checks if there is vehicles at the second hex
        int VehAmountInHex = _board->getHex(activeCoord2)->
                getTransports().size();

        if(VehAmountInHex != 0)
        {
            // Adds pawn in the vehicle
            _board->getHex(activeCoord2)->getTransports().at(0)->addPawn(pawn);
        }
        if(activeCoord2 == Common::CubeCoordinate (0,0,0))
        {
            _board->removePawn(pawn->getId());
            _pvec.at(_state->currentPlayer()-1)->savePawn();
        }

        if(Actleft == 0)
        {
            changePhase();
        }
        else
        {
            setmsg("select pawn","");
        }
        _hexItemMap[activeCoord1]->updateHex();
        _hexItemMap[activeCoord2]->updateHex();
    }
    catch (Common::IllegalMoveException &illegalmovemsg)
    {
        setmsg("select pawn",illegalmovemsg.msg());
    }
}

void MainWindow::movementVehicle(std::shared_ptr<Common::Pawn> pawn)
{

    int playerInTurn = _state->currentPlayer();
    std::string actionsLeft = std::to_string(_pvec.at(playerInTurn-1)->
                                             getActionsLeft());
    int transportId = _board->getHex(activeCoord1)->
            getTransports().at(0)->getId();
    int endHexTransportAmount = _board->getHex(activeCoord2)->
            getTransports().size();
    int endHexActorAmount = _board->getHex(activeCoord2)->
            getActors().size();
    bool isEndHexWater = _board->getHex(activeCoord2)->isWaterTile();

    try
    {
        if((endHexTransportAmount == 0) && isEndHexWater)
        {
            if(endHexActorAmount == 0)
            {
                _runner->moveTransport(activeCoord1,activeCoord2,
                                       transportId);
                _hexItemMap[activeCoord1]->updateHex();
                _hexItemMap[activeCoord2]->updateHex();
            }
            else
            {
                throw(Common::IllegalMoveException(
                            "You cannot move to a hex that has actor in it"));
            }
        }
        else
        {
            // Try to move to coordinate with pawn
            movementOutOfVehicle(pawn);
        }
    }
    catch(Common::IllegalMoveException &illegalmovemsg)
    {
        setmsg("select pawn",illegalmovemsg.msg());
        activeCoord2set = false;
    }
    if(_runner->getCurrentPlayer()->getActionsLeft() == 0)
    {
        changePhase();
    }
}

void MainWindow::movementOutOfVehicle(std::shared_ptr<Common::Pawn> pawn)
{
    int pawnId = pawn->getId();
    try
    {
        _runner->movePawn(activeCoord1,activeCoord2,pawnId);
        _board->getHex(activeCoord1)->getTransports().at(0)
                ->removePawn(pawn);

        int endCoordVehAmount = _board->
                getHex(activeCoord2)->
                getTransports().size();

        // To another vehicle
        if(endCoordVehAmount != 0)
        {
            _board->getHex(activeCoord2)->
                    getTransports().at(0)->addPawn(pawn);
        }
        _hexItemMap[activeCoord1]->updateHex();
        _hexItemMap[activeCoord2]->updateHex();

        changePhase();
    }
    catch(Common::IllegalMoveException &exception)
    {
        setmsg("select pawn",exception.msg());
    }
}

void MainWindow::movement()
{
    int playerInTurn = _state->currentPlayer();

    if(activeCoord1set && activeCoord2set)
    {
        std::vector<std::shared_ptr<Common::Pawn>> pawns =
                _board->getHex(activeCoord1)->getPawns();

        // Gets the index of the players own pawn in the hexes pawn-vector
        int pawnOwned = _board->checkPawnPropertyPlace(activeCoord1,
                                                       playerInTurn);

        // Selects the pawn owned by the player from the hex
        std::shared_ptr<Common::Pawn> pawn = pawns.at(pawnOwned);

        // If pawn is in a vehicle
        if(_board->getHex(activeCoord1)->getTransports().size() != 0)
        {
            // Checks if player can move selected vehicle
            bool canMove = _board->getHex(activeCoord1)->
                    getTransports().at(0)->canMove(playerInTurn);
            if(canMove)
            {
                movementVehicle(pawn);
            }
            // Ends the moving phase if player cannot move the vehicle
            else
            {
                setmsg("select pawn", "Illegal Vehicle move");
            }
        }
        // If pawn is not in a vehicle
        else
        {
            movementPawn(pawn);
        }
        activeCoord1set = false;
        activeCoord2set = false;
    }
}

void MainWindow::sinking()
{
    try
    {
        std::string actorType = _runner->flipTile(activeCoord1);

        if(actorType == "vortex")
        {
            _hexItemMap[activeCoord1]->updateHex();
            setmsg("","OH NO, VORTEX APPEARED");
            int msecsToWait = 2000;
            wait(msecsToWait);

            std::vector<Common::CubeCoordinate> neighbourVec =
                    _board->getHex(activeCoord1)->getNeighbourVector();
            std::vector<Common::CubeCoordinate>::iterator iter;
            std::vector<Common::CubeCoordinate>::iterator iterEnd
                    = neighbourVec.end();

            for(iter = neighbourVec.begin();iter != iterEnd;++iter)
            {
                // Hex coordinates has to be real
                if(_hexItemMap.find(*iter) != _hexItemMap.end())
                {
                    int actSize = _board->getHex(*iter)->getActors().size();
                    int vehSize = _board->getHex(*iter)->
                            getTransports().size();
                    if( vehSize != 0)
                    {
                        _board->removeTransport(_board->getHex(*iter)->
                                                getTransports().at(0)->
                                                getId());
                    }
                    if(actSize != 0)
                    {
                        _board->removeActor(_board->getHex(*iter)->
                                            getActors().at(0)->getId());
                    }
                    _hexItemMap[*iter]->updateHex();
                }
            }
            _board->getHex(activeCoord1)->getActors().at(0)->
                    doAction();
        }
        else
        {
            if ((!(actorType == "boat")) && (!(actorType == "dolphin")))
            {
                _board->getHex(activeCoord1)->getActors().at(0)->
                        doAction();
            }
        }
        _hexItemMap[activeCoord1]->updateHex();
        changePhase();
    }
    catch(Common::IllegalMoveException &illegalmovemsg)
    {
        setmsg("select tile to flip",illegalmovemsg.msg());
    }
    activeCoord1set = false;
}

void MainWindow::spinning()
{

    _inActorMovement = true;
    // Checks if spinned actor exist in gameboard
    if(_board->checkActorExistanse(_spinvalues.first))
    {
        if(activeCoord1set && activeCoord2set)
        {
            try
            {
                // If movable actor if dolphin, because dolphin is classed
                // as a vehicle
                if(_spinvalues.first == "dolphin")
                {
                    int dolphinId = _board->getHex(activeCoord1)->
                            getTransports().at(0)->getId();
                    _runner->moveTransportWithSpinner(activeCoord1,
                                                      activeCoord2,
                                                      dolphinId,
                                                      _spinvalues.second);
                }
                // Any other actors than dolphin
                else
                {
                    int actorId = _board->getHex(activeCoord1)->
                            getActors().at(0)->getId();
                    _runner->moveActor(activeCoord1,activeCoord2,
                                       actorId,_spinvalues.second);
                }

                _hexItemMap[activeCoord1]->updateHex();
                _hexItemMap[activeCoord2]->updateHex();

                changePhase();

                activeCoord1set = false;
                activeCoord2set = false;
                wheelspinned = false;
            }
            catch(Common::IllegalMoveException &illegalmovemsg)
            {
                setmsg("select " + _spinvalues.first,illegalmovemsg.msg());
                activeCoord1set = false;
                activeCoord2set = false;
            }
            _inActorMovement = false;
        }
    }
    else
    {
        setmsg("No Actor Found","");
        _inActorMovement = false;
        int msecsToWait = 2000;
        wait(msecsToWait);
        changePhase();

        activeCoord1set = false;
        activeCoord2set = false;
        wheelspinned = false;
    }
}
