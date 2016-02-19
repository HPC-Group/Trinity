#pragma once
#include <istream>
#include <ostream>

#include "Vcl.h"


namespace trinity {
namespace commands {

class ISerializable;
class ISerialObject
{
public:
    virtual ~ISerialObject() {}

    virtual void append(const std::string& key, float value) = 0;
    virtual void append(const std::string& key, int value) = 0;
    virtual void append(const std::string& key, const std::string& value) = 0;
    virtual void append(const std::string& key, const ISerializable& obj) = 0;
    
    virtual float getFloat(const std::string& key) = 0;
    virtual int getInt(const std::string& key) = 0;
    virtual std::string getString(const std::string& key) = 0;
    virtual void getSerializable(const std::string& key, ISerializable& obj) = 0;

    // dmc: do we really need to be able to write to / read from streams? until now, a serial object
    // is always created from data read from a connection or used to create data that's sent through
    // a connection, right? so until now, it would be ok to write to / read from ByteArray. we might
    // think of extending mocca to write to / read from o/istreams. but that would have to happen on
    // the IStream level, not on the IMessage level, which is the level we're concerned with here
    // (we wouldn't want to send a command through a stream-connection because we want to send /
    // receive complete commands, not partial ones)
    virtual void writeTo(std::ostream& stream) = 0;
    virtual void readFrom(std::istream& stream) = 0;
    
    virtual void setType(VclType type) { m_type = type; }
    virtual VclType getType() {return m_type; }
    
protected:
    VclType m_type;
};
}
}