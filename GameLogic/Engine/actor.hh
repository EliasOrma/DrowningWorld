#ifndef ACTOR_HH
#define ACTOR_HH

#include "hex.hh"
#include <string>
#include <memory>


/**
 * @file
 * @brief Implements abstract base class Actor
 */

namespace Common
{

/**
 * @brief Offers an abstract base class, which is used as the base class to
 * different actors. Actors roam around the
 * gameboard and cause actions whenever their location hex tiles are flipped.
 */
class Actor : public std::enable_shared_from_this<Actor>
{
public:
    /**
     * @brief Default constructor, exists solely for documentation.
     */
    Actor() = default;

    /**
     * @brief Constructor, sets the id of the actor.
     * @param id the id of the actor
     */
    Actor( int id );

    /**
     * @brief Destructor of Actor.
     * @post Exception quarantee: nothrow
     */
    virtual ~Actor();

    /**
     * @brief move moves the actor from the current hex tile to another
     * @param to indicates the target hex
     * @pre move must be legal
     * @post Actor moved to the hex tile to
     * @post Actor's location updated.
     * @post Exception quarantee: strong
     */
    virtual void move( std::shared_ptr<Common::Hex> to );

    /**
     * @brief doAction provides the action performed by the actor when the tile it occupies is turned
     * @post action performed according to the type of the actor
     * @post exception quarantee: basic
     */
    virtual void doAction() = 0;

    /**
     * @brief getActorType returns the type of the actor as a string
     * @return type identifier of actor
     */
    virtual std::string getActorType() const;

    /**
     * @brief getId returns the id of the actor
     * @return id of the actor
     */
    virtual int getId() const;

    /**
     * @brief addHex adds the actor to the hex
     * @param hex the hex tile the actor is added to
     * @post Actor added to hex
     * @post Actor's location set to hex
     * @post exception quarantee: nothrow
     */
    virtual void addHex( std::shared_ptr<Common::Hex> hex );

    /**
     * @brief getHex gets a pointer to the Hex the actor is on.
     * @return shared_ptr to the Hex actor is on, nullptr if actor is not on
     * any hex.
     */
    virtual std::shared_ptr<Common::Hex> getHex();

protected:
    std::shared_ptr<Common::Hex> hex_;

private:
    int id_;
};

}
#endif // ACTOR_HH
