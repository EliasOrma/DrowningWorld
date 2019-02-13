#include "initialize.hh"
#include "gameengine.hh"
#include "hex.hh"

#include "boat.hh"
#include "dolphin.hh"
#include "shark.hh"
#include "kraken.hh"
#include "seamunster.hh"
#include "vortex.hh"

namespace Common {
namespace Initialization {

std::shared_ptr<IGameRunner> getGameRunner(std::shared_ptr<IGameBoard> boardPtr,
                                           std::shared_ptr<IGameState> statePtr,
                                           std::vector<std::shared_ptr<IPlayer>> playerVector)
{

    auto& actorFactory = Logic::ActorFactory::getInstance();
    actorFactory.addActor("shark",
                          [=] (int id) -> std::shared_ptr<Actor>
    {
        return std::make_shared<Shark>(id);
    });
    actorFactory.addActor("kraken",
                          [=] (int id) -> std::shared_ptr<Actor>
    {
        return std::make_shared<Kraken>(id);
    });
    actorFactory.addActor("seamunster",
                          [=] (int id) -> std::shared_ptr<Actor>
    {
        return std::make_shared<Seamunster>(id);
    });
    actorFactory.addActor("vortex",
                          [=] (int id) -> std::shared_ptr<Actor>
    {
        return std::make_shared<Vortex>(id);
    });

    auto& transportFactory = Logic::TransportFactory::getInstance();
    transportFactory.addTransport("boat",
                                  [=] (int id) -> std::shared_ptr<Transport>
    {
        return std::make_shared<Boat>(id);
    });
    transportFactory.addTransport("dolphin",
                                  [=] (int id) -> std::shared_ptr<Transport>
    {
        return std::make_shared<Dolphin>(id);
    });

    std::shared_ptr <Logic::GameEngine> runner =
            std::make_shared<Logic::GameEngine>(boardPtr, statePtr, playerVector);
    return runner;

}
void addNewActorType(std::string typeName, Logic::ActorBuildFunction buildFunction)
{
    Logic::ActorFactory::getInstance().addActor(typeName, buildFunction);
}

void addNewTransportType(std::string typeName, Logic::TransportBuildFunction buildFunction)
{
    Logic::TransportFactory::getInstance().addTransport(typeName, buildFunction);
}


}
}
