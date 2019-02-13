#include "vortex.hh"

namespace Common {

Vortex::Vortex(int typeId):Actor(typeId)
{

}

void Vortex::move(std::shared_ptr<Hex> to)
{
    (void)to;//Vortex doesn't move
}

void Vortex::doAction()
{
    hex_->clearAllFromNeightbours();
    hex_->clear();
}

std::string Vortex::getActorType() const
{
    return "vortex";
}

}
