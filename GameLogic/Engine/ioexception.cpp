#include "ioexception.hh"

namespace Common
{

IoException::IoException(const std::string& msg): GameException(msg) {}

}
