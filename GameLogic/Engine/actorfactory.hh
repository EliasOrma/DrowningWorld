#ifndef ACTORFACTORY_HH
#define ACTORFACTORY_HH

#include "actor.hh"

#include <functional>
#include <string>
#include <vector>
#include <map>

/**
 * @file
 * @brief Singleton class that creates actors. Actors must be registered using
 * registerActor function
 */

namespace Logic {

using ActorPointer = std::shared_ptr<Common::Actor>;
using ActorBuildFunction = std::function<ActorPointer (int)>;
/**
 * @brief Singleton class for creating actors.
 *
 * The factory is requested to read a JSON file, after which it will requested
 * to return a data structure, which containts the read actors.
 */
class ActorFactory {

public:

    /**
     * @return A reference to the factory.
     */
    static ActorFactory& getInstance();

    /**
     * @brief Adds a build
     * @param type Actor type identifier
     * @param buildFunction function that performs the building
     */
    void addActor(std::string type, ActorBuildFunction buildFunction);

    /**
     * @brief getAvailableActors
     * @return vector contailing the type identifiers of available actors
     */
    std::vector<std::string> getAvailableActors() const;

    /**
     * @brief createActor
     * @param type
     * @return the created actor. Ownership is transferred to caller
     */
    ActorPointer createActor(std::string type);

private:

    ActorFactory();

    std::map<std::string, ActorBuildFunction> actorDefinitions;
    int idCounter;
};

}

#endif
