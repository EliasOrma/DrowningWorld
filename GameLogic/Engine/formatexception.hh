#ifndef FORMATEXCEPTION_HH
#define FORMATEXCEPTION_HH

#include "gameexception.hh"

/**
 * @file
 * @brief Defines an exception class for errors occuring in data format.
 */

namespace Common {

/**
 * @brief Exception class for erroneous data format.
 */
class FormatException: public GameException {

  public:

   /**
    * @brief Constructor.
    * @param msg Message declaring the reason of the exception.
    */
    explicit FormatException(const std::string& msg = "");

   /**
    * @brief Destructor.
    */
    virtual ~FormatException() = default;

};

}

#endif
