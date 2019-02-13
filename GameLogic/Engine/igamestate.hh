#ifndef IGAMESTATE_HH
#define IGAMESTATE_HH

#include <memory>

/**
 * @file
 * @brief Defines the interface which keeps track of the game state.
 */

namespace Common {

/**
 * @brief Type that denotes the phase of the game. The phases of the game are: Movement, Sinking and Spinning.
 */
enum GamePhase { MOVEMENT = 1, SINKING = 2, SPINNING = 3 };

/**
 * @brief Defines the interface for statistics class.
 */
class IGameState {

  public:

    /**
     * @brief Constructor, exists solely for documentation.
     */
    IGameState() = default;

    /**
     * @brief Virtual destructor. Does nothing, since this is an interface class.
     */
    virtual ~IGameState() = default;

    /**
     * @brief currentGamePhase tells the phase of the game.
     * @return The phase of the game.
     * @post Exception quarantee: nothrow
     */
    virtual GamePhase currentGamePhase() const = 0;

    /**
     * @brief currentPlayer tells the player in turn.
     * @return The identifier of the player in turn.
     * @post Excetion quarantee: nothrow
     */
    virtual int currentPlayer() const = 0;

    /**
     * @brief changeGamePhase sets the phase of the game.
     * @param newPhase The next phase of the game.
     * @post Game phase changed. Exception quarantee: basic
     */
    virtual void changeGamePhase(Common::GamePhase nextPhase) = 0;

    /**
     * @brief changePlayerTurn sets the player in turn.
     * @param nextPlayer The identifier of the next player in turn.
     * @post Turn changed to player nextPlayer. Exception quarantee: basic
     */
    virtual void changePlayerTurn(int nextPlayer) = 0;

};

}

#endif
