#ifndef KRAKEN_HH
#define KRAKEN_HH


#include "actor.hh"

#include <string>

namespace Common {

/**
 * @brief Actor that destroys boats. Pawns onboard a boat encountered by Kraken
 * are left to float on the water hex.
 */
class Kraken: public Actor
{
public:
    /**
     * @brief Constructor, sets the id of the kraken.
     * @param typeId the id of the kraken
     */
    Kraken(int typeId);

    /**
     * @copydoc Common::Actor::doAction()
     */
    virtual void doAction();

    /**
     * @copydoc Common::Actor::getActorType()
     */
    virtual std::string getActorType() const;

};
}
#endif // KRAKEN_HH
