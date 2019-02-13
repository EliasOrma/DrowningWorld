#include "actorfactory.hh"

namespace Logic {

using std::map;
using std::string;
using std::vector;

ActorFactory::ActorFactory():
    actorDefinitions(),
    idCounter(0)
{

}

ActorFactory& ActorFactory::getInstance()
{

    static ActorFactory instance;
    return instance;

}

void ActorFactory::addActor(string type, ActorBuildFunction buildFunction)
{
    actorDefinitions[type] = buildFunction;
}

std::vector<std::string> ActorFactory::getAvailableActors() const
{
    auto types = vector<string>();
    for (auto it = actorDefinitions.begin();
         it != actorDefinitions.end();
         ++it) {
        types.push_back(it->first);
    }
    return types;

}

ActorPointer ActorFactory::createActor(string type)
{
    ++idCounter;
    return actorDefinitions[type](idCounter);
}

}
