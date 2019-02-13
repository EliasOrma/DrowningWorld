#include "kraken.hh"

namespace Common {

Kraken::Kraken(int typeId):Actor(typeId)
{

}

void Kraken::doAction()
{
    hex_->clearTransports();
}

std::string Kraken::getActorType() const
{
    return "kraken";
}

}
