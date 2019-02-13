#ifndef IGAMERUNNER_HH
#define IGAMERUNNER_HH

#include "cubecoordinate.hh"
#include "igamestate.hh"
#include "iplayer.hh"
#include "pawn.hh"

#include <map>
#include <string>

/**
 * @file
 * @brief Interface class that is used to control the flow of the game.
 */

/**
 * @brief Contains pre-existing classes that may be referred by code written by students.
 */
namespace Common {

using SpinnerLayout = std::map<std::string, std::map<std::string,unsigned>>;
/**
 * @brief Offers an interface, which is used to control the game logic.
 */
class IGameRunner {

public:
    /**
     * @brief Default constructor, exists solely for documentation.
     */
    IGameRunner() = default;

    /**
     * @brief Virtual destructor. Does nothing, since this is an interface class.
     */
    virtual ~IGameRunner() = default;

    /**
     * @brief movePawn moves pawn to given target, if legal.
     * @param origin The coordinates of the hex to move from
     * @param target The coordinates of the hex to move to
     * @param pawnId The id of the pawn to move
     * @return 0-3 (number of moves left)
     * @throw Common::IllegalMoveException Illegal move was attempted.
     * @post The current player's actions left are decreased by the amount \n
     * of distance the transport was moved.
     * @post Exception quarantee: strong
     */
    virtual int movePawn(CubeCoordinate origin,
                         CubeCoordinate target,
                         int pawnId) = 0;

    /**
     * @brief moveActor Moves actor to given target, if legal.
     * @param origin The coordinates of the hex to move from
     * @param target The coordinates of the hex to move to
     * @param actorId The id of the actor to move
     * @param moves The distance in moves the actor can move to ( as returned
     * by IGameRunner::SpinWheel() )
     * @throw Common::IllegalMoveException Illegal move was attempted.
     * @post currentPlayers's moves are restored(end of turn).
     * @post Exception quarantee: strong
     */
    virtual void moveActor(CubeCoordinate origin,
                           CubeCoordinate target,
                           int actorId,
                           std::string moves) = 0;

    /**
     * @brief moveTransport Moves transport to given target, if legal
     * @param origin The coordinates of the hex to move from
     * @param target The coordinates of the hex to move to
     * @param transportId The id of the transport to move
     * @return 0-3 (amount of moves left),
     *  if movement is legal
     * @note HOX: This version of the move function
     * should be used when moving the transport
     * during player's movement phase.
     * @throw Common::IllegalMoveException Illegal move was attempted
     * @post Exception quarantee: strong
     * @post The current player's actions left are decreased by the amount \n
     * of distance the transport was moved.
     */
    virtual int moveTransport(CubeCoordinate origin,
                              CubeCoordinate target,
                              int transportId) = 0;

    /**
     * @brief moveTransportWithSpinner Moves transport to given target, if legal
     * @param origin The coordinates of the hex to move from
     * @param target The coordinates of the hex to move to
     * @param transportId The id of the transport to move
     * @param moves The distance in moves the transport can move to ( as returned
     * by IGameRunner::SpinWheel() )
     * @return 0-3 (amount of moves left) if movement is legal
     * @note HOX: some transports might be movable in the SPINNING-phase along
     * with actors. They will then be moved in the same way as actors would.
     * @throw Common::IllegalMoveException Illegal move was attempted
     * @post If the transport "dives" (moves=="D"), the pawns are not moved \n
     * with the transport, and are removed from the transport
     * @post If the transports is moved "normally" the pawns are moved  \n
     * with the transport
     * @post If no moves are left, currentPlayers's moves are restored(end of turn).
     * @post Exception quarantee: strong
     */
    virtual int moveTransportWithSpinner(CubeCoordinate origin,
                               CubeCoordinate target,
                               int transportId,
                               std::string moves) = 0;

    /**
     * @brief checkPawnMovement tells if move is possible and the number of
     * moves left.
     * @details Pawn move is illegal, if one of the following holds:\n
     * (1) Source-, target-hex or the pawn doesn't exist.\n
     * (2) Pawn is not on the source-hex\n
     * (3) Target hex at target is fully occupied (max 3 pawns)\n
     * (4) Distance > moves left for Player\n
     * (5) Distance != 1 if moving in water\n
     * (6) No possible route to the target hex found\n
     * @param origin The origin of the proposed move.
     * @param target The destination of the proposed move.
     * @param pawnId The identifier of the pawn.
     * @return 0-3 (number of moves left) or -1 (movement is impossible)
     * @post Exception quarantee: nothrow
     */
    virtual int checkPawnMovement(Common::CubeCoordinate origin,
                                  Common::CubeCoordinate target,
                                  int pawnId) = 0;

    /**
     * @brief checkActorMovement tells if the move is possible.
     * @details Actor move is illegal, if one of the following holds:\n
     * (1) Source-, target-hex or actor doesn't exist\n
     * (2) Actor is not on source-hex\n
     * (3) Target-hex is not a water tile\n
     * (4) Target-hex is too far away (specified by parameter moves)\n
     * @param origin The origin of the proposed move.
     * @param target The destination of the proposed move.
     * @param actorId The identifier of the actor.
     * @return true, if the move is possible, false otherwise.
     * @post Exception quarantee: nothrow
     */
    virtual bool checkActorMovement(Common::CubeCoordinate origin,
                                    Common::CubeCoordinate target,
                                    int actorId,
                                    std::string moves) = 0;
    /**
     * @brief checkTransportMovement tells if the move is legal.
     * @details Transport move is illegal, if one of folowing holds:\n
     * (1) Source-, target-hex or actor doesn't exist\n
     * (2) Transport is not on source-hex\n
     * (3) Target-hex is not a water tile\n
     * (4) moves is diving ("D") and transport is not empty \n
     * (5) Target-hex is too far away (specified by parameter moves)\n
     * (6) Current player is not allowed to move the transport
     *  (Transport::canMove())\n
     * @param origin The origin of the proposed move.
     * @param target The destination of the proposed move.
     * @param transportId The identifier of the transport.
     * @return gamephase is SPINNING: 0 if legal, -1 if illegal\n
     * gamephase is MOVING: 0-3 (amount of moves left) if legal, -1 if illegal.
     * @post Exception quarantee: nothrow
     */
    virtual int checkTransportMovement(Common::CubeCoordinate origin,
                                    Common::CubeCoordinate target,
                                    int transportId,
                                    std::string moves) = 0;
    /**
     * @brief flipTile sinks the tile if possible and tells the actor on the bottom of the tile.
     * @param tileCoord Coordinate of the selected tile.
     * @return The actor on the bottom of the tile or empty string (fail).
     * @exception IllegalMoveException, if any error occured.
     * @post Gamestate changed to sinking
     * @post Tile changed to a sea tile
     * @post Exception quarantee: strong
     */
    virtual std::string flipTile(CubeCoordinate tileCoord) = 0;

    /**
     * @brief spinWheel decide and report which "animal" moves and how much it
     * moves.
     * @details spinWheel decides and reports which actor/transport moves after
     * the game phase is set to 3 (SPINNING). The types of actors and transports
     * returnable by spinwheel are configurable with Assets/layout.json .
     * By-default the possible types are: "dolphin", "kraken", "seamunster",
     * "shark", here also referred to as animals.
     * @note HOX: Unlike kraken, seamunster and shark (Common::Actor),
     * dolphin is of class Common::Transport
     * @return a pair <type of the actor/vehicle, number of movements>
     * @post game phase set to three. Exception quarantee: strong
     */
    virtual std::pair<std::string,std::string> spinWheel() = 0;

    /**
     * @brief getSpinnerLayout
     * @return the layout of the spinner
     */
    virtual SpinnerLayout getSpinnerLayout() const = 0 ;

    /**
     * @brief getCurrentPlayer get pointer to the current player in turn.
     * @return Common::IPlayer-pointer to the current player in turn, or
     * nullptr if no player has the id set as current player
     * (IGameState::currentPlayer())
     * @post Exception guarantee: nothrow
     */
    virtual std::shared_ptr<Common::IPlayer> getCurrentPlayer() = 0;

    /**
     * @brief currentPlayer tells the player in turn.
     * @return The player id in the turn.
     * @post Exception quarantee: nothrow
     */
    virtual int currentPlayer() const = 0;

    /**
     * @brief playerAmount tells the number of players in the game
     * @return The number of players in the game
     * @post Exception guarantee: nothrow
     */
    virtual int playerAmount() const = 0;

    /**
     * @brief currentGamePhase tells the game phase of the game.
     * @return The game phase of the game.
     * @post Exception quarantee: nothrow
     */
    virtual Common::GamePhase currentGamePhase() const = 0;



};

}

#endif
