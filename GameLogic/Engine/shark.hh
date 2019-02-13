#ifndef SHARK_HH
#define SHARK_HH

#include "actor.hh"

#include <string>

namespace Common {

/**
 * @brief Water tile actor that eats pawns at the hex, if they are not being
 * carried by a transporter.
 */
class Shark : public Actor
{
public:
    /**
     * @brief Constructor, sets the id of the shark.
     * @param typeId the id of the shark
     */
    Shark(int typeId);
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
#endif // SHARK_HH
