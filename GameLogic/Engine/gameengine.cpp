#include "actorfactory.hh"
#include "gameengine.hh"
#include "hex.hh"
#include "actor.hh"
#include "boat.hh"
#include "illegalmoveexception.hh"
#include "piecefactory.hh"
#include "transportfactory.hh"

#include <algorithm>
#include <iostream>
#include <ctime>

namespace Logic {

//! Rule for max pawns per tile
int const MAX_PAWNS_PER_HEX = 3;
int const MAX_ACTIONS_PER_TURN = 3;

GameEngine::GameEngine(std::shared_ptr<Common::IGameBoard> boardPtr,
                       std::shared_ptr<Common::IGameState> statePtr,
                       std::vector<std::shared_ptr<Common::IPlayer> > players):
    playerVector_(players),
    board_(boardPtr),
    gameState_(statePtr),
    islandRadius_(0)
{
    // Initialize random-number seed
    std::srand(std::time(0));

    PieceFactory::getInstance().readJSON();

    initializeBoard();
    try {
        initializeBoats();
    } catch (Common::GameException& e) {
        std::cout<< e.msg() <<std::endl;
    }

    layoutParser_.readJSON("Assets/layout.json");
}

int GameEngine::movePawn(Common::CubeCoordinate origin,
                         Common::CubeCoordinate target,
                         int pawnId)
{
    std::shared_ptr<Common::IPlayer> player = getCurrentPlayer();

    // Current player not found
    if (player == nullptr){
        throw Common::IllegalMoveException("Illegal transport move:"
                                           " no current player");
    }

    int movesLeft = checkPawnMovement(origin, target, pawnId);
    if (movesLeft < 0)
    {
        throw Common::IllegalMoveException("Illegal pawn move");
    } else {
        board_->movePawn(pawnId, target);
        player->setActionsLeft(movesLeft);
    }

    return movesLeft;
}


int GameEngine::checkPawnMovement(Common::CubeCoordinate origin,
                                  Common::CubeCoordinate target,
                                  int pawnId)
{

    // Move is illegal (return -1), if:
    //    (1) Source-, target-hex or pawn doesn't exist
    //    (2) Pawn is not on source-hex
    //    (3) targetHex is occupied (full, max pawns per tile is 3)
    //    (4) distance > moves left
    //    (5) distance != 1 if moving in water
    //    (6) No possible route to target found

    std::shared_ptr<Common::Hex> sourceHex = board_->getHex(origin);
    std::shared_ptr<Common::Hex> targetHex = board_->getHex(target);

    // (1)

    if (sourceHex == nullptr || targetHex == nullptr) {
        return -1;
    }

    // (2)
    std::shared_ptr<Common::Pawn> pawn = sourceHex->givePawn(pawnId);
    if (pawn == nullptr) {
        return -1;
    }

    // (3)
    if (targetHex->getPawnAmount() >= MAX_PAWNS_PER_HEX) {
        return -1;
    }

    unsigned int distance = cubeCoordinateDistance(origin, target);

    int playerId = pawn->getPlayerId();
    if (playerId == gameState_->currentPlayer()) {

        // Find ptr to player
        auto playerIt = playerVector_.begin();
        while (playerIt != playerVector_.end()) {
            if ((*playerIt)->getPlayerId() == gameState_->currentPlayer()) {
                break;
            }
            ++playerIt;
        }

        // (4)
        if ((*playerIt)->getActionsLeft() >= distance) {
            if (board_->isWaterTile(origin) > 0) {
                // (5)
                if ((distance == 1) && ((*playerIt)->getActionsLeft() >= 3)) {
                    return 0;
                }
            } else {
                // (6)
                if (breadthFirst(origin,target,
                                (*playerIt)->getActionsLeft())) {
                    unsigned int hadActions = (*playerIt)->getActionsLeft();
                    return hadActions - distance;
                }
            }
        }
    }

    return -1;

}

void GameEngine::moveActor(Common::CubeCoordinate origin,
                           Common::CubeCoordinate target,
                           int actorId,
                           std::string moves)
{
    bool validMove = checkActorMovement(origin, target, actorId,
                                        moves);

    if (!validMove) {
        throw Common::IllegalMoveException("Illegal actor move");
    } else
    {
        board_->moveActor(actorId, target);
        getCurrentPlayer()->setActionsLeft(MAX_ACTIONS_PER_TURN);
    }


}

bool GameEngine::checkActorMovement(Common::CubeCoordinate origin,
                                    Common::CubeCoordinate target,
                                    int actorId,
                                    std::string moves)
{
    // Move is illegal (return false), if:
    //    (1) Source-, target-hex or actor doesn't exist
    //    (2) Actor is not on source-hex
    //    (3) Target-hex is not a water tile
    //    (4) Target-hex is too far away

    // (1)
    std::shared_ptr<Common::Hex> sourceHex = board_->getHex(origin);
    std::shared_ptr<Common::Hex> targetHex = board_->getHex(target);
    if ( (sourceHex == nullptr) || (targetHex == nullptr) ) {
        return false;
    }

    // (2)
    std::shared_ptr<Common::Actor> actor = sourceHex->giveActor(actorId);
    if (actor == nullptr) {
        return false;
    }

    // (3)
    if (!targetHex->isWaterTile()) {
        return false;
    }

    // (4)
    bool distancePass = false;
    unsigned int numMoves = 0;
    if (moves == "D") {
        // Actor can dive any distance
        distancePass = true;
    } else {

        try {
            numMoves = std::stoi(moves);
        } catch(...) {
            // Given moves-argument couldn't be translated to int
            distancePass = false;
            numMoves = 0;
        }

        if (cubeCoordinateDistance(origin, target) <= numMoves) {
            distancePass = true;
        }
    }
    if (!distancePass){
        return false;
    }

    return true;
}

int GameEngine::moveTransport(Common::CubeCoordinate origin,
                              Common::CubeCoordinate target,
                              int transportId)
{
    // Find current player
    std::shared_ptr<Common::IPlayer> player = getCurrentPlayer();

    // Current player not found
    if (player == nullptr){
        throw Common::IllegalMoveException("Illegal transport move:"
                                           " no current player");
    }

    std::string movesStr = std::to_string(player->getActionsLeft());
    int movesLeft = checkTransportMovement(origin, target, transportId,
                                           movesStr);

    if (movesLeft < 0) {
        throw Common::IllegalMoveException("Illegal transport move");
    } else
    {
        player->setActionsLeft(movesLeft);
        board_->moveTransport(transportId, target);
    }
    return movesLeft;
}

int GameEngine::moveTransportWithSpinner(Common::CubeCoordinate origin,
                                         Common::CubeCoordinate target,
                                         int transportId,
                                         std::string moves)
{
    int movesLeft = checkTransportMovement(origin, target, transportId,
                                        moves);

    if (movesLeft < 0) {
        throw Common::IllegalMoveException("Illegal transport move");
    } else
    {
        if (moves == "D") {
            board_->getHex(origin)->giveTransport(transportId)->removePawns();
            movesLeft=0;
        }
        board_->moveTransport(transportId, target);
    }
    if (movesLeft == 0 ){
        getCurrentPlayer()->setActionsLeft(MAX_ACTIONS_PER_TURN);
    }
    return movesLeft;

}

int GameEngine::checkTransportMovement(Common::CubeCoordinate origin,
                                       Common::CubeCoordinate target,
                                       int transportId,
                                       std::string moves)
{
    // Move is illegal (return -1), if:
    //    (1) Source-, target-hex or actor doesn't exist
    //    (2) Transport is not on source-hex
    //    (3) Target-hex is not a water tile
    //    (4) Move is diving and transport is not empty
    //    (5) Target-hex is too far away
    //    (6) Current player is not allowed to move this transport

    // (1)
    std::shared_ptr<Common::Hex> sourceHex = board_->getHex(origin);
    std::shared_ptr<Common::Hex> targetHex = board_->getHex(target);
    if (sourceHex == nullptr || targetHex == nullptr) {
        return -1;
    }

    // (2)
    std::shared_ptr<Common::Transport> transport =
            sourceHex->giveTransport(transportId);
    if (transport == nullptr) {
        return -1;
    }

    // (3)
    if (!targetHex->isWaterTile()) {
        return -1;
    }

    bool distancePass = false;
    unsigned int numMoves = 0;
    unsigned int distance = 0;

    //Checking if transport is empty
    bool isTransportEmpty = transport->getMaxCapacity() == transport->getCapacity();
    // (4)
    if (moves == "D") {
        distancePass = true;
        numMoves = 3;
    // (5)
    } else {

        try {
            numMoves = std::stoi(moves);
        } catch(std::exception &e) {
            // Given moves-argument couldn't be translated to int
            distancePass = false;
            numMoves = 0;
        }

        distance = cubeCoordinateDistance(origin, target);
        if (distance <= numMoves) {
            //Check if player can move transport or transport is empty (6)
            if ( transport->canMove( gameState_->currentPlayer() ) ||
                 isTransportEmpty){
                distancePass = true;
            }
        }
    }
    if (!distancePass){
        return -1;
    }

    // If we got here, the move is legal.

    // If current gamestate is SPINNING, movement is done in one go and no moves
    // left needed.
    if (currentGamePhase() == Common::GamePhase::SPINNING){
        return 0;
    }

    // Otherwise, return amount of moves left.
    int movesLeft = numMoves-distance;
    return movesLeft;
}

std::string GameEngine::flipTile(Common::CubeCoordinate tileCoord)
{

    gameState_->changeGamePhase(Common::GamePhase::SINKING);

    // Haetaan ko. saaripala ja tarkistetaan sen olemassaolo.
    std::shared_ptr<Common::Hex> currentHex = board_->getHex(tileCoord);
    if (currentHex == nullptr) {
        throw Common::IllegalMoveException("The tile does not exist.");
    }
    std::string pieceType = currentHex->getPieceType();

    // Vesi- ja maaliruutuja ei voi olla mahdollista kääntää.
    if (pieceType == "Water") {
        throw Common::IllegalMoveException("Can not flip the water tile.");
    } else if (pieceType == "Coral") {
        throw Common::IllegalMoveException("Can not flip the coral tile.");
    }

    // Check if islandPieces still has pieces tracked
    if (islandPieces_.size() == 0) {
        throw Common::IllegalMoveException("No flippable tiles left");
    }
    // Noudatetaan poistojärjestystä: ranta, metsä, vuoristo.

    auto& currentLayer = islandPieces_.back();
    if( pieceType != currentLayer.first ) {
        throw Common::IllegalMoveException("All tiles of type " +
                                           currentLayer.first +
                                           " have not yet been flipped.");
    }

    // Laskurin päivitys.
    --currentLayer.second;
    if (currentLayer.second == 0) {
      islandPieces_.pop_back();

    }

    // Toimijan arvontaa.
    auto actors = Logic::ActorFactory::getInstance().getAvailableActors();
    auto transports = Logic::TransportFactory::getInstance().getAvailableTransports();


    std::vector<std::string> creatables;
    creatables.reserve(actors.size() + transports.size());
    creatables.insert(creatables.end(), actors.begin(), actors.end());
    creatables.insert(creatables.end(), transports.begin(), transports.end());
    std::random_shuffle(creatables.begin(), creatables.end());
    auto selected = creatables.back();

    auto matchString = [selected](auto a)->bool{return a == selected;};
    if(std::find_if(transports.begin(), transports.end(), matchString) != transports.end()){
        board_->addTransport(Logic::TransportFactory::getInstance().createTransport(selected), tileCoord);
    } else if (std::find_if(actors.begin(), actors.end(), matchString) != actors.end()) {
        board_->addActor(ActorFactory::ActorFactory::getInstance().createActor(selected), tileCoord);
    }
    // muutetaan ruutu vesiruuduksi.
    currentHex->setPieceType("Water");

    return selected;

}

std::pair<std::string,std::string> GameEngine::spinWheel()
{

    gameState_->changeGamePhase(Common::GamePhase::SPINNING);

    // Mikä eläin (arvonta)...
    layoutParser_.getSections();
    std::vector<std::string> sections = layoutParser_.getSections();;
    std::random_shuffle(sections.begin(),sections.end());
    std::string toMove = sections.back();

    // ...ja paljon liikkuu (1,2,3,D -> arvonta).

    auto moves = layoutParser_.getChancesForSection(toMove);
    std::random_shuffle(moves.begin(),moves.end());
    std::string moveAmount = moves.back().first;

    return std::pair<std::string,std::string> (toMove, moveAmount);

}

Common::SpinnerLayout GameEngine::getSpinnerLayout() const
{
    using Common::SpinnerLayout;
    auto sections = layoutParser_.getSections();
    SpinnerLayout layout;
    for (const auto& section: sections){
        auto chaces = layoutParser_.getChancesForSection(section);
        for (const auto& chance: chaces) {
            layout[section].insert(chance);
        }
    }
    return layout;
}

std::shared_ptr<Common::IPlayer> GameEngine::getCurrentPlayer()
{
    int id = currentPlayer();
    auto end = playerVector_.end();
    for (auto iter = playerVector_.begin(); iter != end; ++iter) {
        if ((*iter)->getPlayerId() == id) {
            return *iter;
        }
    }
    return nullptr;
}

bool GameEngine::breadthFirst(Common::CubeCoordinate FromCoord, Common::CubeCoordinate ToCoord, unsigned int actionsLeft)
{

    unsigned int currentIndex = 0;
    std::vector<std::pair<Common::CubeCoordinate,unsigned int>> checkVector;
    std::pair<Common::CubeCoordinate,unsigned int> firstTile = std::make_pair(FromCoord,currentIndex);
    checkVector.push_back(firstTile);

    std::vector<Common::CubeCoordinate> workVector;
    workVector.push_back(FromCoord);

    while(not workVector.empty()){

        Common::CubeCoordinate currentCoord = workVector.at(0);
        workVector.erase(workVector.begin());
        std::shared_ptr<Common::Hex> currentHex = board_->getHex(currentCoord);

        if(currentHex->getPawnAmount() < MAX_PAWNS_PER_HEX
                || currentCoord == FromCoord){
            if(!(currentHex)->isWaterTile()){
                unsigned int newIndex = 0;
                while(newIndex < checkVector.size()){
                    if(checkVector.at(newIndex).first == currentCoord){
                        break;
                    }
                    newIndex++;
                }
                currentIndex = newIndex;

                // Use only existing neighbours
                std::vector<Common::CubeCoordinate> neighbourVector = {};
                auto neighbourCoords = currentHex->getNeighbourVector();
                auto neighboursEnd = neighbourCoords.end();
                for (auto iter = neighbourCoords.begin(); iter != neighboursEnd;
                     ++iter)
                {
                    if (board_->getHex(*iter) != nullptr) {
                        neighbourVector.push_back(*iter);
                    }
                }

                for(auto neighIt = neighbourVector.begin(); neighIt != neighbourVector.end(); neighIt++){

                    if((*neighIt) == ToCoord){
                        std::pair<Common::CubeCoordinate,int> finalTile = std::make_pair((*neighIt),currentIndex);
                        checkVector.push_back(finalTile);

                        unsigned int nextTile = checkVector.size();
                        std::vector<Common::CubeCoordinate> finalRoadVector;
                        while(nextTile != 0){

                            finalRoadVector.insert(finalRoadVector.begin(),checkVector.at(nextTile - 1).first);
                            nextTile = checkVector.at(nextTile  - 1).second;
                        }
                        //finalRoadVector.push_back(FromCoord);
                        if(finalRoadVector.size() <= actionsLeft){
                            return true;
                        }
                        else{
                            return false;
                        }
                    }

                    auto checkerIt = checkVector.begin();
                    bool tileNotVisited = true;
                    while(checkerIt != checkVector.end()){
                        if((*checkerIt).first == (*neighIt)){
                            tileNotVisited = false;
                            break;
                        }
                        checkerIt++;
                    }
                    if(tileNotVisited){
                        std::pair<Common::CubeCoordinate,int> tileToCheck = std::make_pair((*neighIt),currentIndex);
                        checkVector.push_back(tileToCheck);
                        workVector.push_back((*neighIt));
                    }
                }

            }
        }
        currentIndex++;
    }

    return false;

}

std::vector<Common::CubeCoordinate> GameEngine::addHexToBoard(
                            Common::CubeCoordinate coord, std::string pieceType)
{
    /* Method creates a new hex and adds it to board.
     * Also tracks the amount of hexes added to the board in _islandPieces
     * Returns the neighbour-vector of the created hex to be used in algorithms
    */

    // Keep track of piece-types and amounts

    // Find the field in islandPieces tracking the amount of this pieceType
    auto matchType = [pieceType](auto a)->bool{return a.first == pieceType;};
    auto islandPiecesField = std::find_if(islandPieces_.begin(),
                                          islandPieces_.end(), matchType);

    std::shared_ptr<Common::Hex> prevHex = board_->getHex(coord);
    if (prevHex != nullptr) {
        // There's already a hex in this position, for whatever reason.
        // It is going to be replaced.

        std::string prevType = prevHex->getPieceType();
        if (islandPiecesField != islandPieces_.end())
        {
            islandPiecesField->second -= 1;
            if (islandPiecesField->second < 0) islandPiecesField->second = 0;
        }

    } else if (islandPiecesField != islandPieces_.end()) {
        islandPiecesField->second += 1;
    } else if (pieceType != "Water" && pieceType != "Coral") {
        // Water and Coral can't be sunk, so don't push them here.
        // New pieceType, push front for sinking-order
        islandPieces_.push_back({pieceType, 1});

    } else {
        // Do nothing to track the amount of Water and Coral
    }

    // Add the hex
    std::shared_ptr<Common::Hex> newHex = std::make_shared<Common::Hex>();
    newHex->setCoordinates(coord);
    newHex->setPieceType(pieceType);

    // Add all already existing neighbour-pointers for this hex and add this
    // hex as their neighbour
    auto neighbourVector = newHex->getNeighbourVector();
    for (auto it = neighbourVector.begin(); it != neighbourVector.end(); ++it) {
        std::shared_ptr<Common::Hex> neighbourHex = board_->getHex(*it);
        if (neighbourHex != nullptr) {
            newHex->addNeighbour(neighbourHex);
            neighbourHex->addNeighbour(newHex);
        }
    }

    board_->addHex(newHex);

    return neighbourVector;
}

void GameEngine::initializeBoard()
{
    /* Method initializes the game board -hexes
     * Pieces to fill the board with are defined in Assets/pieces.json
    */

    // Size (radius) of the goal areas on the edge of the board
    int goalSize = 2;

    // Get pieces from piecefactory
    Logic::PieceFactory pieceFactory = Logic::PieceFactory::getInstance();
    pieceFactory.readJSON();
    typedef std::vector<std::pair<std::string,int>> pieceVector;
    pieceVector pieces =
            pieceFactory.getGamePieces();

    // Generate layers of the island starting from center
    int currentLayer = 0;
    pieceVector::iterator iter = pieces.begin();
    while (iter != pieces.end())
    {
        // Do as many layers as specified for this piece-type
        for (int i = 0; i < iter->second; ++i)
        {
            Common::CubeCoordinate coord;

            // Count this layer to islandRadius
            if ((iter->first != "Water") && (iter->first != "Coral")) {
                ++islandRadius_;
            }

            // Center-piece
            if (i == 0 && iter == pieces.begin())
            {
                coord = Common::CubeCoordinate(0,0,0);
                addHexToBoard(coord, iter->first);
            }

            // Start from bottom-left corner of the ring and step through
            // neighbours until a closed ring
            coord = Common::CubeCoordinate(-currentLayer, 0, currentLayer);

            for (int j = 0; j < 6; ++j)
            {
                // Looping sides of the ring
                for(int k = 0; k < currentLayer; ++k)
                {
                    std::string type = iter->first;

                    // Place goal-hexes only in corners
                    if (type == "Coral")
                    {
                        if (abs(coord.x) >= goalSize && abs(coord.y) >= goalSize
                            && abs(coord.z) >= goalSize)
                        {
                            // Put water between goal-hexes
                            type = "Water";
                        }
                    }

                    std::vector<Common::CubeCoordinate> neighbours =
                            addHexToBoard(coord, type);
                    // Next tile
                    coord = neighbours.at(j);
                }
            }
            ++currentLayer;
        }
        ++iter;
    }
}

void GameEngine::initializeBoats()
{
    /* Initializes boats at the corners of the board.
     * Spawns boats first in opposing corners of the island. When all corners
     * are full, spawns more boats along each side of the island (equally if
     * possible).
     * Stops spawning boats if the coastline is full.
     * Should be called after initializeBoard()
     * Expects transportfactory to already know how to build boats.
     */
    auto& factory = Logic::TransportFactory::getInstance();
    int players = playerAmount();

    // Throw if transportfactory doesn't know boats.
    auto available = factory.getAvailableTransports();
    if (std::find(available.begin(),
                  available.end(), "boat") == available.end()) {
        throw Common::GameException("Transport factory doesn't know boats,"
                                    " are transports initialized?");
    }

    int offset = 0;
    for (int i = 0; i < players; ++i)
    {

        // Stop adding boats if the coastline is full
        if (offset >= islandRadius_) {
            break;
        }

        Common::CubeCoordinate coordToAdd;

        int islandSides = 6;
        switch (i % islandSides) {
        case 0: {
            coordToAdd = Common::CubeCoordinate(
                        islandRadius_, -islandRadius_+offset, -offset);
            break;
        }
        case 1: {
            coordToAdd = Common::CubeCoordinate(
                        -islandRadius_, islandRadius_-offset, offset);
            break;
        }
        case 2: {
            coordToAdd = Common::CubeCoordinate(
                        offset, -islandRadius_, islandRadius_-offset);
            break;
        }
        case 3: {
            coordToAdd = Common::CubeCoordinate(
                        -offset, islandRadius_, -islandRadius_+offset);
            break;
        }
        case 4: {
            coordToAdd = Common::CubeCoordinate(
                        islandRadius_-offset, offset, -islandRadius_);
            break;
        }
        case 5: {
            coordToAdd = Common::CubeCoordinate(
                        -islandRadius_+offset, -offset, islandRadius_);
            // Update offset before next lap
            ++offset;
            break;
        }
        default: {
            break;
        }
        }

        // Add the boat, if a water hex exists at coordToAdd.
        // No boats are added, if the board doesn't have a singe water-hex
        std::shared_ptr<Common::Hex> hexToAdd = board_->getHex(coordToAdd);
        if (hexToAdd != nullptr) {
            if (hexToAdd->getPieceType() == "Water") {
                std::shared_ptr<Common::Transport> newBoat =
                                factory.createTransport("boat");
                board_->addTransport(newBoat, coordToAdd);
            }
        }

    }
}

unsigned int GameEngine::cubeCoordinateDistance(Common::CubeCoordinate source, Common::CubeCoordinate target) const
{

    return ((abs(source.x - target.x) + abs(source.y - target.y)
             + abs(source.z - target.z)) / 2);

}

int GameEngine::currentPlayer() const
{

    return gameState_->currentPlayer();

}

Common::GamePhase GameEngine::currentGamePhase() const
{

    return gameState_->currentGamePhase();

}

int GameEngine::playerAmount() const
{
    return playerVector_.size();
}



}
