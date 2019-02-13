#include "shark.hh"

namespace Common {

Shark::Shark(int typeId):Actor(typeId){

}

void Shark::doAction()
{
    hex_->clearPawnsFromTerrain();
}

std::string Shark::getActorType() const
{
    return "shark";
}

}
