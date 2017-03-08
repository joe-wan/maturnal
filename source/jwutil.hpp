#ifndef JWUTIL_HPP
#define JWUTIL_HPP

// #include "maturnal.h"
#include <string>
#include <map>

namespace jwutil {
   std::map<std::string, std::string> argvToMap(int argc, char *argv[]);
}

#endif
