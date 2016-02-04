
#include "ISerialObjectFactory.h"

#include "StringifiedObject.h"

using namespace trinity::common;

std::unique_ptr<ISerialObject> ISerialObjectFactory::create() {
    
    // later, we can read the serialization type class from a config file
    return std::unique_ptr<ISerialObject>(new StringifiedObject);
}
