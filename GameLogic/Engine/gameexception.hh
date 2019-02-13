#ifndef GAMEEXCEPTION_HH
#define GAMEEXCEPTION_HH

#include <exception>
#include <string>

/**
 * @file
 * @brief Defines exception class for errors occuring in the game.
 */

namespace Common {

/**
 * @brief Base class for exceptions used in the game.
 */
class GameException: public std::exception {

  public:

    /**
     * @brief Constructor.
     * @param msg Message declaring the reason of the exception.
     */
    explicit GameException(const std::string& msg = "");

    /**
     * @brief Destructor.
     */
    virtual ~GameException() = default;

    /**
     * @brief msg Gives message declaring the reason of the exception.
     * @return Message given in the constructor or an empty string.
     */
    virtual std::string msg() const;

  private:

    //! Message declaring the reason of the exception.
    const std::string msg_;

};

}

#endif
