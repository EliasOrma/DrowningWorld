#include "illegalmoveexception.hh"

namespace Common
{

IllegalMoveException::IllegalMoveException(const std::string& msg): GameException(msg) {}

}
