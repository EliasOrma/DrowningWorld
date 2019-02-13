#ifndef TRANSPORT_HH
#define TRANSPORT_HH
#include "hex.hh"
#include "pawn.hh"

#include <memory>
#include <vector>

/**
 * @file  The Transport class
 * @brief Implements abstract base class Transport
 */

namespace Common {

/**
 * @brief Offers an abstract base class, which is used as the base class to
 * different transports. Transporter objects can transport pawns
 * around the game board as they move between hexes according to the game
 * rules.
 */
class Transport : public std::enable_shared_from_this<Transport>
{
public:
    /**
     * @brief default constructor
     */
    Transport() = default;

    /**
     * @brief Constructor of Transport
     * @param id Unique identifier for the transport.
     */
    Transport( int id );

    /**
     * @brief desctructor
     * @post Exception quarantee: nothrow
     */
    virtual ~Transport();

    /**
     * @brief getTransportType returns the type name of the transport
     * @return The name of the transports type.
     */
    virtual std::string getTransportType() = 0;

    /**
     * @brief Adds pawn to transport
     * @param pawn
     * @post If there is space, pawn is added to transport
     * @note note: Pawn is not removed from the hex
     * @post If transport is full, pawn is not added
     */
    void addPawn( std::shared_ptr<Common::Pawn> pawn );

    /**
     * @brief removePawn removes a pawn from transport
     * @param pawn Pawn to be removed.
     * @post If pawn is in transport, it will be removed from it
     */
    void removePawn( std::shared_ptr<Common::Pawn> pawn );

    /**
     * @brief Moves the transport from the current hex to another hex
     * @param to target hex
     * @pre move must be legal
     * @post Transport moved to the hex to
     * @post All pawns in transport moved to the hex to
     * @post Transport's and pawns' locations updated.
     * @post Exception quarantee: strong
     */
    virtual void move( std::shared_ptr<Common::Hex> to ) = 0;

    /**
     * @brief getCapacity returns the amount of pawns the transport is able to carry
     * @return the amount of free places
     */
    virtual int getCapacity() const;

    /**
     * @brief getMaxCapacity return the maximum capacity of the transport
     * @return maximum capacity of the transport
     */
    virtual int getMaxCapacity() const;

    /**
     * @brief canMove checks if the player playerId is allowed to move the transport
     * @param playerId
     * @return true is playerId can move the transport, false if not
     */
    virtual bool canMove( int playerId ) const = 0;

    /**
     * @brief addHex adds the transport to the hex
     * @param the hex hex where the transport will be added to
     * @post Transport added to hex
     * @post Transport's location updated.
     * @post exception quarantee: nothrow
     */
    virtual void addHex( std::shared_ptr<Common::Hex> hex );

    /**
     * @brief getHex gets a pointer to the Hex the transport is on.
     * @return shared_ptr to the Hex actor is on, nullptr if actor is not on
     * any hex.
     */
    virtual std::shared_ptr<Common::Hex> getHex();

    /**
     * @brief getPawnsInTransport Returns a vector of pawn-pointers in the
     * Transport.
     * @return Vector of pointers to pawns that are in the Transport
     */
    virtual std::vector<std::shared_ptr<Common::Pawn>> getPawnsInTransport();

    /**
     * @brief isPawnInTransport checks if pawn is in transport
     * @param pawn the pawn we want to check for
     * @return true if pawn is in transport, otherwise false
     */
    bool isPawnInTransport(std::shared_ptr<Common::Pawn> pawn);

    /**
     * @brief getId returns id of the transport
     * @return id of the transport
     */
    int getId();
    /**
     * @brief removePawns removes pawns from transport
     * @post Pawns are no longer in the transport
     */
    void removePawns();

protected:
    using PawnVector = std::vector<std::shared_ptr<Common::Pawn>>;
    int capacity_;
    PawnVector pawns_;
    std::shared_ptr<Common::Hex> hex_;

private:
    int id_;

};

}
#endif // TRANSPORT_HH
