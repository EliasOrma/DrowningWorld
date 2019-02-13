#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH

#include "cubecoordinate.hh"
#include "igameboard.hh"
#include "igamerunner.hh"
#include "igamestate.hh"
#include "iplayer.hh"
#include "wheellayoutparser.hh"

#include <memory>
#include <string>
#include <vector>
#include <map>

/**
 * @file
 * @brief Implements interface IGameRunner.
 */

/**
 * @brief Contains pre-existing classes that may not be referred by code written by students.
 */
namespace Logic {

/**
 * @brief Implements the interface IGamerunner.
 */
class GameEngine : public Common::IGameRunner
{

  public:

    /**
     * @brief Constructor.
     * @param boardPtr Shared pointer to the game board.
     * @param statePtr Shared pointer to the game state.
     * @param playerVector Vector that contains players.
     */
    GameEngine(std::shared_ptr<Common::IGameBoard> boardPtr,
               std::shared_ptr<Common::IGameState> statePtr,
               std::vector<std::shared_ptr<Common::IPlayer>> players);

    /**
     * @copydoc Common::IGameRunner::movePawn()
     */
    virtual int movePawn(Common::CubeCoordinate origin,
                         Common::CubeCoordinate target,
                         int pawnId);

    /**
     * @copydoc Common::IGameRunner::checkPawnMovement()
     */
    virtual int checkPawnMovement(Common::CubeCoordinate origin,
                                  Common::CubeCoordinate target,
                                  int pawnId);
    /**
     * @copydoc Common::IGameRunner::moveActor()
     */
    virtual void moveActor(Common::CubeCoordinate origin,
                           Common::CubeCoordinate target,
                           int actorId,
                           std::string moves);

    /**
     * @copydoc Common::IGameRunner::checkActorMovement()
     */
    virtual bool checkActorMovement(Common::CubeCoordinate origin,
                                    Common::CubeCoordinate target,
                                    int actorId,
                                    std::string moves);
    /**
     * @copydoc Common::IGameRunner::moveTransport(
     * Common::CubeCoordinate origin, Common::CubeCoordinate target,
     * int transportId)
     */
    virtual int moveTransport(Common::CubeCoordinate origin,
                              Common::CubeCoordinate target,
                              int transportId);

    /**
     * @copydoc Common::IGameRunner::moveTransportWithSpinner(
     * Common::CubeCoordinate origin, Common::CubeCoordinate target,
     * int transportId, std::string moves)
     */
    virtual int moveTransportWithSpinner(Common::CubeCoordinate origin,
                               Common::CubeCoordinate target,
                               int transportId,
                               std::string moves);

    /**
     * @copydoc Common::IGameRunner::checkTransportMovement()
     */
    virtual int checkTransportMovement(Common::CubeCoordinate origin,
                                    Common::CubeCoordinate target,
                                    int transportId,
                                    std::string moves);
    /**
     * @copydoc Common::IGameRunner::flipTile()
     */
    virtual std::string flipTile(Common::CubeCoordinate tileCoord);

    /**
     * @copydoc Common::IGameRunner::spinWheel()
     */
    virtual std::pair<std::string,std::string> spinWheel();

    /**
     * @copydoc  Common::IGameRunner::getSpinnerLayout()
     */
    virtual Common::SpinnerLayout getSpinnerLayout() const override;

    /**
     * @copydoc Common::IGameRunner::getCurrentPlayer()
     */
    virtual std::shared_ptr<Common::IPlayer> getCurrentPlayer();

    /**
     * @copydoc Common::IGameRunner::currentPlayer()
     */
    virtual int currentPlayer() const;

    /**
     * @copydoc Common::IGameRunner::currentGamePhase()
     */
    virtual Common::GamePhase currentGamePhase() const;

    /**
     * @copydoc Common::IGameRunner::playerAmount()
     */
    virtual int playerAmount() const;

  private:

    bool breadthFirst(Common::CubeCoordinate FromCoord, Common::CubeCoordinate ToCoord, unsigned int actionsLeft);

    unsigned int cubeCoordinateDistance(Common::CubeCoordinate source, Common::CubeCoordinate target) const;

    std::vector<Common::CubeCoordinate> addHexToBoard(Common::CubeCoordinate coord,
                                                      std::string pieceType);
    void initializeBoard();
    void initializeBoats();

    std::vector<std::shared_ptr<Common::IPlayer>> playerVector_;
    std::shared_ptr<Common::IGameBoard> board_;
    std::shared_ptr<Common::IGameState> gameState_;

    //! Actortypes.

    WheelLayoutParser layoutParser_;

    //! Piecetypes.
    std::vector<std::pair<std::string,int>> islandPieces_;

    // Radius of the island, needed to spawn boats
    int islandRadius_;
};

}

#endif
