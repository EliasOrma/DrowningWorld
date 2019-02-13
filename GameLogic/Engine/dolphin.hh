#ifndef DOLPHIN_HH
#define DOLPHIN_HH

#include "transport.hh"

namespace Common {

/**
 * @brief Dolphin moves between hexes according to game rules and can
 * transport a single pawn at a time.
 */
class Dolphin : public Transport
{
public:
    /**
     * @brief Constructor, sets the id of the dolphin.
     * @param typeId the id of the dolphin
     */
    Dolphin(int id);

    /**
     * @copydoc Common::Transport::getTransportType()
     */
    virtual std::string getTransportType();

    /**
     * @copydoc Common::Transport::move()
     */
    virtual void move( std::shared_ptr<Common::Hex> to);


    /**
     * @copydoc Common::Transport::canMove()
     */
    virtual bool canMove( int playerId ) const;

};
}
#endif // DOLPHIN_HH
