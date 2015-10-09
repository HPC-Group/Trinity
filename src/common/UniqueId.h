#pragma once

#include <string>

namespace mui {

/* Creates a string that should be unique for all pratical purposes, although it's no a real UUID */
std::string createUniqueId();
}