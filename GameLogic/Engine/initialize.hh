#ifndef INITIALIZE_HH
#define INITIALIZE_HH

#include "igameboard.hh"
#include "igamerunner.hh"
#include "igamestate.hh"
#include "iplayer.hh"
#include "actorfactory.hh"
#include "transportfactory.hh"

#include <memory>

/**
 * @file
 * @brief Declaration of intialization functions
 */

namespace Common {

/**
 * @brief This module contains the functions related to the initialization of the game.
 * This is used to integrate the code from the studend side to the GameLogic.
 * Module services include 1) a creator for game engine object, responsible for running the game,
 * and 2) adder services that enable adding custom actors and transporter classes in to the game.
 */
namespace Initialization {

/**
 * @brief getGameRunner Creates an instance of the class that implements IGameRunner interface.
 * @param boardPtr Shared pointer to the game board.
 * @param statePtr Shared pointer to the game state.
 * @param playerVector Vector that contains players.
 * @exception IOException Could not open file Assets/actors.json or Assets/pieces.json for reading.
 * @exception FormatException Format of file Assets/actors.json or Assets/pieces.json is invalid.
 * @return Created instance of IGameRunner.
 * @note HOX! Custom actors and transports MUST be added before calling getGameRunner!
 * @post GameBoard added
 */
std::shared_ptr<IGameRunner> getGameRunner(std::shared_ptr<IGameBoard> boardPtr,
                                           std::shared_ptr<IGameState> statePtr,
                                           std::vector<std::shared_ptr<IPlayer>> playerVector);

/**
 * @brief addNewActorType registers a new actor type to game
 * @param typeName Name of the new actor type
 * @param buildFunction Function that will be used for automatically
 * instantiatiating objects from the given custom actor class and creating
 * shared pointers to them.
 * @post The game can now use actors of the registered type
 * @note This function needs to be used only if we want to create new
 *   types of actors as an additional feature
 */
void addNewActorType(std::string typeName, Logic::ActorBuildFunction buildFunction);

/**
 * @brief addNewTransportType registers a new transport type to game
 * @param typeName Name of the new transport type
 * @param buildFunction Function that will be used for automatically
 * instantiatiating objects from the given custom transporter class and
 * creating shared pointers to them.
 * @post The game can now use transports of the registered type
 * @note This function needs to be used, only if we want to create new
 *   types of transports as an additional feature
 */
void addNewTransportType(std::string typeName, Logic::TransportBuildFunction buildFunction);


}
}

#endif
