#include "actor.hh"
#include <string>
#include <memory>

namespace Common {

Actor::Actor(int id ):
    id_( id ),
    hex_(nullptr){}

Actor::~Actor(){}

void Actor::move(std::shared_ptr<Hex> to)
{
    addHex(to);
}

void Actor::doAction(){}

std::string Actor::getActorType() const
{
    return "DefaultActorType";
}

int Actor::getId() const
{
    return id_;
}

void Actor::addHex( std::shared_ptr<Common::Hex> hex )
{
    hex->addActor(shared_from_this());
    if (hex_ != nullptr) {
        hex_->removeActor(shared_from_this());
    }
    hex_ = hex;
}

std::shared_ptr<Hex> Actor::getHex()
{
    return hex_;
}

}
