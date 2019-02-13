#ifndef SEAMUNSTER_HH
#define SEAMUNSTER_HH

#include "actor.hh"

#include <string>

namespace Common {

/**
 * @brief Water tile actor that eats boats and pawns. R.I.P.
 */
class Seamunster : public Actor
{
public:
    /**
     * @brief Constructor, sets the id of the sea munster.
     * @param typeId the id of the sea munstes
     */
    Seamunster(int typeId);

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
#endif // SEAMUNSTER_HH
