#ifndef IGAMEBOARD_HH
#define IGAMEBOARD_HH

#include "cubecoordinate.hh"
#include "hex.hh"

#include <memory>

/**
 * @file
 * @brief Defines the interface for game board.
 */

namespace Common {

/**
 * @brief Interface for game board.
 */
class IGameBoard : public std::enable_shared_from_this<Common::IGameBoard> {

  public:

    /**
     * @brief Default constructor, exists solely for documentation.
     */
    IGameBoard() = default;

    /**
     * @brief Virtual destructor. Does nothing, since this is an interface class.
     */
    virtual ~IGameBoard() = default;


    /**
     * @brief checkTileOccupation Checks the current amount of pawns on the tile
     * @param tileCoord The location of the tile in coordinates.
     * @return The number of the pawns in the tile or -1 if the tile does not exist.
     * @post Exception quarantee: strong
     */
    virtual int checkTileOccupation(Common::CubeCoordinate tileCoord) const = 0;

    /**
     * @brief isWaterTile checks if the tile is a water tile.
     * @param tileCoord The location of the tile in coordinates.
     * @return true, if the tile is a water tile, else (or if the tile does not exist) false.
     * @post Exception quarantee: nothrow
     */
    virtual bool isWaterTile(Common::CubeCoordinate tileCoord) const = 0;


    /**
     * @brief getHex returns the hex gameboard tile
     * @param hexCoord The location of the hex in coordinates.
     * @return Shared pointer to the hex or nullptr, if the hex not exists.
     * @post Exception quarantee: nothrow
     */
    virtual std::shared_ptr<Common::Hex> getHex(Common::CubeCoordinate hexCoord) const = 0;

    /**
     * @brief addPawn adds a new pawn to the game
     * @param playerId. Id of player, whose pawn is added
     * @param pawnId. Id of the pawn
     * @post Pawn is added to the game. Exception quarantee: basic
     */
    virtual void addPawn(int playerId, int pawnId) = 0;

    /**
     * @brief addPawn adds a new pawn to the game
     * @details Creates a new Pawn with specified IDs and the created Pawn is then added to a
     * Hex-object that has been specified with a Common::CubeCoordinate.
     * @param playerId. Id of player, whose pawn is added
     * @param pawnId. Id of the pawn
     * @param coord. CubeCoordinate, where pawn is added
     * @pre CubeCoordinate must contain a Hex-tile. No Pawn with pawnId exists.
     * @post Pawn is added to the game. Target Hex-object contains a new Pawn.
     * @post Pawn's location set to coord
     * @post Exception quarantee: basic
     */
    virtual void addPawn(int playerId, int pawnId, Common::CubeCoordinate coord) = 0;

    /**
     * @brief movePawn sets a new location for the pawn.
     * @details Pawn is added to a Hex that is in the specified coordinates and removed from the old.
     * If there is no Hex-object in specified coordinates, the pawn remains in the old Hex.
     * @param pawnId The identifier of the pawn.
     * @param pawnCoord The target location of the pawn in coordinates.
     * @pre Pawn exists
     * @post Pawn is moved to the target location or stays in place if location doesn't exist.
     * @post Pawn's location is updated.
     * Exception quarantee: basic
     */
    virtual void movePawn(int pawnId, Common::CubeCoordinate pawnCoord) = 0;

    /**
     * @brief removePawn removes a pawn.
     * @details Removed pawn should be removed from a Hex-object if it is contained in one.
     * @param pawnId The identifier of the pawn.
     * @pre Pawn exists
     * @post pawn matching the id is removed. Exception quarantee: basic
     */
    virtual void removePawn(int pawnId) = 0;

    /**
     * @brief addActor adds a actor to the game board
     * @param actor
     * @param actorCoord
     * @pre CubeCoordinate must contain a Hex-tile. Actor is not null-pointer.
     * @post Actor has been added to the hex in target coordinates
     * @post Actor's location set to the hex in target coordinates
     */
    virtual void addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord) = 0;

    /**
     * @brief moveActor sets a new location for the actor.
     * @details Actor is added to a Hex that is in the specified coordinates and removed from the old.
     * If there is no Hex-object in specified coordinates, the actor remains in the old Hex.
     * @param actorId The identifier of the actor.
     * @param actorCoord The target location of the actor in coordinates.
     * @pre Actor exists
     * @post Actor actorId is moved to a new location or stays in place if location doesn't exist.
     * @post Actor's location updated.
     * @post Exception quarantee: basic
     */
    virtual void moveActor(int actorId, Common::CubeCoordinate actorCoord) = 0;

    /**
     * @brief removeActor removes an actor.
     * @details The actor should be removed from a Hex-object.
     * @param actorId The identifier of the actor.
     * @pre Actor exists
     * @post Actor actorId is removed. Exception quarantee: basic
     */
    virtual void removeActor(int actorId) = 0;

    /**
     * @brief addHex adds a new hex tile to the board
     * @param newHex Pointer of a new hex to add
     * @pre newHex is valid
     * @post newHex is added to the board. Any existing hex at the same
     * coordinates is replaced. Exception quarantee: basic
     */
    virtual void addHex(std::shared_ptr<Common::Hex> newHex) = 0;

    /**
     * @brief addTransport adds a new transport to the game board
     * @param transport transport to be added
     * @param coord
     * @pre CubeCoordinate must contain a Hex-tile. Transport isn't null-pointer.
     * @post Transport has been added to the hex in target coordinates.
     * @post Transport's location set to the hex in target coordinates.
     * Exception guarantee: basic
     */
    virtual void addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord) = 0;

    /**
     * @brief moveTransport sets a new location for the transport.
     * @details Transport is added to a Hex that is in the specified coordinates and removed from the old.
     * If there is no Hex-object in specified coordinates, the transport remains in the old Hex.
     * @param id The identifier of the transport.
     * @param coord The target location of the transport in coordinates.
     * @post transport is moved to a new location or stays in place if location doesn't exist.
     * @post All pawns in transport are moved to the new location with the transport.
     * @post Transport's and included pawns' locations will be updated.
     * Exception quarantee: basic
     */
    virtual void moveTransport(int id, Common::CubeCoordinate coord) = 0;

    /**
     * @brief removeTransport removes an transport.
     * @param id The identifier of the transport.
     * @post transport removed from the gameboard and Hex-object. Exception quarantee: basic
     */
    virtual void removeTransport(int id) = 0;
};

}

#endif
