#pragma once
#include "ICommand.h"

namespace trinity {
namespace commands {

class ListFilesCmd : public ICommand {
public:
    ListFilesCmd(ISerialObject&);
    ListFilesCmd(int sid, int rid);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(const ISerialObject& serial) override;
};

class InitIOSessionCmd : public ICommand {

public:
    InitIOSessionCmd(ISerialObject&);
    InitIOSessionCmd(int sid, int rid, const std::string& protocol, int fileId);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(const ISerialObject& serial) override;

    const std::string& getProtocol() const;
    int getFileId() const;

private:
    std::string m_protocol;
    int m_fileId;
};

class ReplyInitIOSessionCmd : public ICommand {

public:
    ReplyInitIOSessionCmd(ISerialObject&);
    ReplyInitIOSessionCmd(int sid, int rid);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(const ISerialObject& serial) override;

    void setControlPort(const int port);
    int getControlPort() const;
    void setNewSid(int sid);

private:
    int m_controlPort;
};

class GetLODLevelCountCmd : public ICommand {
public:
    GetLODLevelCountCmd(ISerialObject&);
    GetLODLevelCountCmd(int sid, int rid);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(const ISerialObject& serial) override;
};

class ReplyGetLODLevelCountCmd : public ICommand {

public:
    ReplyGetLODLevelCountCmd(ISerialObject&);
    ReplyGetLODLevelCountCmd(int sid, int rid);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(const ISerialObject& serial) override;

    int getLODLevelCount() const;
    void setLODLevelCount(int lod);

private:
    int m_lodCount;
};

class IOData : public ISerializable {
public:
    IOData(const std::string& name, int fileId, const VclType& dataType);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(const ISerialObject& serial) override;

    std::string getName() const;
    int getFileId() const;
    VclType getDataType() const;

private:
    std::string m_name;
    int m_fileId;
    VclType m_dataType;
};
}
}