#ifndef TRANSPORTFACTORY_HH
#define TRANSPORTFACTORY_HH

#include "transport.hh"

#include <functional>
#include <string>
#include <vector>
#include <map>

/**
 * @file
 * @brief Singleton class that creates transports. Transprots must be registered using
 * registerTransport function
 */

namespace Logic {

using TransportPointer = std::shared_ptr<Common::Transport>;
using TransportBuildFunction = std::function<TransportPointer (int)>;

/**
 * @brief Singleton factory for creating Transports.
 *
 */
class TransportFactory {

public:

    /**
     * @return A reference to the factory.
     */
    static TransportFactory& getInstance();

    /**
     * @brief Adds a buildable transport
     * @param type transport type identifier
     * @param buildFunction function that performs the building
     */
    void addTransport(std::string type, TransportBuildFunction buildFunction);

    /**
     * @brief getAvailableTransports
     * @return vector containing the type identifiers of available transports
     */
    std::vector<std::string> getAvailableTransports() const;

    /**
     * @brief createTransport
     * @param type
     * @return the created transport. Ownership is transferred to caller
     */
    TransportPointer createTransport(std::string type);

private:

    TransportFactory();

    std::map<std::string, TransportBuildFunction> transportDefinitions_;
    int idCounter_;
};

}

#endif // TRANSPORTFACTORY_HH
