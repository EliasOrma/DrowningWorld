#include "gameexception.hh"

namespace Common
{

GameException::GameException(const std::string& msg): msg_(msg) {}

std::string GameException::msg() const
{

    return msg_;

}

}
