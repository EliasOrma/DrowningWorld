#ifndef IOEXCEPTION_HH
#define IOEXCEPTION_HH

#include "gameexception.hh"

/**
 * @file
 * @brief Defines an exception class for errors that happen when handling files.
 */

namespace Common {

/**
 * @brief Exception class for file handling errors.
 */
class IoException: public GameException {

  public:

    /**
     * @brief Constructor.
     * @param msg Message containing the reason for the error.
     */
    explicit IoException(const std::string& msg = "");

    /**
     * @brief Destructor.
     */
    virtual ~IoException() = default;

};

}

#endif
