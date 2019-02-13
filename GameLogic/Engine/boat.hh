#ifndef BOAT_HH
#define BOAT_HH

#include "transport.hh"

namespace Common
{

/**
 * @brief Boat moves between hexes and can transport up to three pawns per
 * player at a time.
 */
class Boat : public Transport
{
public:
    /**
     * @brief Constructor, sets the id of the boat.
     * @param id the id of the boat
     */
    Boat(int id);

    /**
     * @copydoc Common::Transport::getTransportType()
     */
    virtual std::string getTransportType();

    /**
     * @brief move moves the boat from the current hex tile to another
     * @param to indicates the target tile
     * @pre move must be legal
     * @post Transport moved to the hex tile to
     * @post Exception quarantee: strong
     */
    virtual void move( std::shared_ptr<Common::Hex> to);

    /**
     * @copydoc Common::Transport::canMove()
     */
    virtual bool canMove( int playerId ) const;

};
}
#endif // BOAT_HH
