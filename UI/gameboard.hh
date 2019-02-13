#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include "igameboard.hh"

namespace Student {

class GameBoard : public Common::IGameBoard
{
public:
    GameBoard() = default;
    virtual ~GameBoard() = default;


    /**
     * @brief checkTileOccupation Checks the current amount of pawns on the tile
     * @param tileCoord The location of the tile in coordinates.
     * @return The number of the pawns in the tile or -1 if the tile does not exist.
     * @post Exception quarantee: strong
     */
    virtual int checkTileOccupation(Common::CubeCoordinate tileCoord) const;

    /**
     * @brief isWaterTile checks if the tile is a water tile.
     * @param tileCoord The location of the tile in coordinates.
     * @return true, if the tile is a water tile, else (or if the tile does not exist) false.
     * @post Exception quarantee: nothrow
     */
    virtual bool isWaterTile(Common::CubeCoordinate tileCoord) const;


    /**
     * @brief getHex returns the hex gameboard tile
     * @param hexCoord The location of the hex in coordinates.
     * @return Shared pointer to the hex or nullptr, if the hex not exists.
     * @post Exception quarantee: nothrow
     */
    virtual std::shared_ptr<Common::Hex> getHex(Common::CubeCoordinate hexCoord) const;

    /**
     * @brief addPawn adds a new pawn to the game
     * @param playerId. Id of player, whose pawn is added
     * @param pawnId. Id of the pawn
     * @post Pawn is added to the game. Exception quarantee: basic
     */
    virtual void addPawn(int playerId, int pawnId);

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
    virtual void addPawn(int playerId, int pawnId, Common::CubeCoordinate coord);

    /**
     * @brief movePawn sets a new location for the pawn.
     * @param pawnId The identifier of the pawn.
     * @param pawnCoord The target location of the pawn in coordinates.
     * @pre Pawn exists
     * @post Pawn is moved to the target location. Exception quarantee: basic
     */
    virtual void movePawn(int pawnId, Common::CubeCoordinate pawnCoord);

    /**
     * @brief removePawn removes a pawn.
     * @param pawnId The identifier of the pawn.
     * @pre Pawn exists
     * @post pawn matching the id is removed. Exception quarantee: basic
     */
    virtual void removePawn(int pawnId);

    /**
     * @brief addActor adds a new actor to the game board
     * @param actor
     * @param actorCoord
     * @pre coordinates must contain a hex
     * @post actor has been added to the hex in target coordinates
     */
    virtual void addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord);

    /**
     * @brief moveActor sets a new location for the actor.
     * @param actorId The identifier of the actor.
     * @param actorCoord The target location of the actor in coordinates.
     * @pre Actor exists
     * @post actor actorId is moved to a new location: Exception quarantee: basic
     */
    virtual void moveActor(int actorId, Common::CubeCoordinate actorCoord);

    /**
     * @brief removeActor removes an actor.
     * @param actorId The identifier of the actor.
     * @pre Actor exists
     * @post Actor actorId is removed. Exception quarantee: basic
     */
    virtual void removeActor(int actorId);

    /**
     * @brief addHex adds a new hex tile to the board
     * @param newHex Pointer of a new hex to add
     * @pre newHex is valid
     * @post newHex is added to the board. Any existing hex at the same
     * coordinates is replaced. Exception quarantee: basic
     */
    virtual void addHex(std::shared_ptr<Common::Hex> newHex);

    /**
     * @brief addTransport adds a new transport to the game board
     * @param transport transport to be added
     * @param coord
     * @pre coordinates must contain a hex
     * @post Transport has been added to the hex in target coordinates
     */
    virtual void addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord);

    /**
     * @brief moveTransport sets a new location for the transport.
     * @param id The identifier of the transport.
     * @param coord The target location of the transport in coordinates.
     * @post transport is moved to a new location: Exception quarantee: basic
     */
    virtual void moveTransport(int id, Common::CubeCoordinate coord);

    /**
     * @brief removeTransport removes an transport.
     * @param id The identifier of the transport.
     * @post transport removed from the gameboard. Exception quarantee: basic
     */
    virtual void removeTransport(int id);

    /**
    * @brief getHexMap returns the map of hexes
    * @return map of hexes
    * @post Exception quarantee: nothrow
    */
    std::map<Common::CubeCoordinate,std::shared_ptr<Common::Hex>> getHexMap();

    /**
     * @brief checkPawnPropertyPlace gets player's owned pawns index in hexes
     * pawn-vector. if pawn or hex doesn't exist, returns -1
     * @param coord The coordinates of a hex
     * @param playerId id of the player
     * @return index player's pawn in a hexes pawnvector or -1
     * @post Expecion quarantee: nothrow
     */
    int checkPawnPropertyPlace(Common::CubeCoordinate coord,int playerId);

    /**
     * @brief checkActorExistanse checks if there is any this type of actor
     * in any hex
     * @return bool of an actor existance in a _hexMap
     * @post Expecion quarantee: nothrow
     */
    bool checkActorExistanse(std::string actorType);

    /**
     * @brief noPawnsLeft checks if there is pawns left in a gameboard
     * @return bool of is there pawns left
     */
    bool noPawnsLeft();


private:
    std::map<Common::CubeCoordinate,std::shared_ptr<Common::Hex>> hexMap_;
    std::map<int,std::shared_ptr<Common::Pawn>> pawnMap_;
    std::map<int,std::shared_ptr<Common::Actor>> actorMap_;
    std::map<int,std::shared_ptr<Common::Transport>> transportMap_;
};
}
#endif // GAMEBOARD_HH
