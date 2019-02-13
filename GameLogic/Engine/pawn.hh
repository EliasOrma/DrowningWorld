#ifndef PAWN_HH
#define PAWN_HH

#include "cubecoordinate.hh"

#include <memory>

/**
 * @file
 * @brief Pawn. Implements the pawn functionality
 */

namespace Common {
/**
 * @brief Represents a player-owned game piece on the board
 */
class Pawn : public std::enable_shared_from_this<Pawn> {

public:

    /**
     * @brief Constructor.
     */
    Pawn();

    /**
      * @brief Constructor to create Pawn when IDs and Coordinate is already known.
      * @details Only internal values are set. No external effects.
      * @param pawnId Pawn's ID
      * @param playerID Pawn's owner's ID
      * @param pawnCoord Pawn's starting CubeCoordinate
      * @pre No preconditions.
      */
    Pawn(int pawnId, int playerId, Common::CubeCoordinate pawnCoord);

    /**
     * @brief setRegularCoordinates sets location for the pawn as x,y-coordinates
     * @param x X-coordinate.
     * @param y Y-coordinate.
     */
    void setRegularCoordinates(int x, int y);

    /**
     * @brief setCoordinates sets location for the pawn in Cube coordinates.
     * @param pawnCoord The location of the pawn in coordinates.
     */
    void setCoordinates(Common::CubeCoordinate pawnCoord);

    /**
     * @brief getCoordinates gets the coordinates of the Pawn in CubeCoordinates
     * @return The Pawn's coordinates in Common::CubeCoordinate format
     */
    Common::CubeCoordinate getCoordinates();

    /**
     * @brief setId adds a new pawn.
     * @param pawnId The identifier of the pawn.
     * @param playerId The identifier of the player.
     */
    void setId(int pawnId, int playerId);

    /**
     * @brief getId returns id of the pawn
     * @return  id of the pawn
     */
    int getId();

    /**
     * @brief getId returns id of the player the pawn belongs to
     * @return  id of the player who owns this pawn
     */
    int getPlayerId();

private:

    //! Cube coordinate.
    Common::CubeCoordinate coord_;

    //! The identifier of the pawn.
    int pawnId_;
    //! The identifier of the player.
    int playerId_;

};

}

#endif
