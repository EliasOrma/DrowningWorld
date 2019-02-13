#include "seamunster.hh"

namespace Common {

Seamunster::Seamunster(int typeId): Actor(typeId)
{

}

void Seamunster::doAction()
{
    hex_->clearTransports();
    hex_->clearPawnsFromTerrain();
}

std::string Seamunster::getActorType() const
{
    return "seamunster";
}


}
