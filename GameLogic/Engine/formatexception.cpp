#include "formatexception.hh"

namespace Common
{

FormatException::FormatException(const std::string& msg): GameException(msg) {}

}
