#pragma once

#include "common/IONodeProxy.h"

#include <QStandardItemModel>

class IODataModel : public QStandardItemModel
{
    Q_OBJECT
public:
    IODataModel(const trinity::IONodeProxy& ioNode, QObject *parent);

private:
    void createRecursive(const std::string& dataID, QStandardItem* parent);

private:
    const trinity::IONodeProxy& m_ioNode;
};

Q_DECLARE_METATYPE(trinity::IOData*);