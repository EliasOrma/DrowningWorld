#ifndef ILLEGALMOVEEXCEPTION_HH
#define ILLEGALMOVEEXCEPTION_HH

#include "gameexception.hh"

/**
 * @file
 * @brief Defines an exception class for errors occuring in flip tile and
 * pawn/actor/transport movement.
 */

namespace Common {

/**
 * @brief Exception class for flipping tile and moving pawn/actor/transport
 * errors.
 */
class IllegalMoveException: public GameException {

  public:

   /**
    * @brief Constructor.
    * @param msg Message declaring the reason of the exception.
    */
    explicit IllegalMoveException(const std::string& msg = "");

   /**
    * @brief Destructor.
    */
    virtual ~IllegalMoveException() = default;

};

}

#endif
