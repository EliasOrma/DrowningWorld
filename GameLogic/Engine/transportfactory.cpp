#include "transportfactory.hh"

namespace Logic {

using std::map;
using std::string;
using std::vector;

TransportFactory::TransportFactory():
    transportDefinitions_(),
    idCounter_(0)
{

}

TransportFactory& TransportFactory::getInstance()
{

    static TransportFactory instance;
    return instance;

}

void TransportFactory::addTransport(string type, TransportBuildFunction buildFunction)
{
    transportDefinitions_[type] = buildFunction;
}

std::vector<std::string> TransportFactory::getAvailableTransports() const
{
    auto types = vector<string>();
    for (auto it = transportDefinitions_.begin();
         it != transportDefinitions_.end();
         ++it) {
        types.push_back(it->first);
    }
    return types;

}

TransportPointer TransportFactory::createTransport(string type)
{
    ++idCounter_;
    return transportDefinitions_[type](idCounter_);
}

}
