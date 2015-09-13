#ifndef SPLITTOOLS
#define SPLITTOOLS

#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> &splitString(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> splitString(const std::string &s, char delim) {
    std::vector<std::string> elems;
    splitString(s, delim, elems);
    return elems;
}

#endif // SPLITTOOLS

