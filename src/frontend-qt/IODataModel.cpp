#include "IODataModel.h"

IODataModel::IODataModel(const trinity::IONodeProxy& ioNode, QObject* parent)
    : m_ioNode(ioNode) {
    QStandardItem* rootItem = invisibleRootItem();
    auto rootNames = ioNode.getRoots();
    for (const auto& rootName : rootNames) {
        QStandardItem* item = new QStandardItem(QString::fromStdString(rootName));
        item->setEditable(false);
        rootItem->appendRow(item);
        createRecursive(rootName, item);
    }
}

void IODataModel::createRecursive(const std::string& dataID, QStandardItem* item) {
    auto ioItems = m_ioNode.listFiles(dataID);
    for (const auto& ioItem : ioItems) {
        QStandardItem* newItem = new QStandardItem(QString::fromStdString(ioItem.getName()));
        newItem->setEditable(false);
        QVariant data = QVariant::fromValue(new trinity::IOData(ioItem));
        newItem->setData(data);
        item->appendRow(newItem);
        createRecursive(ioItem.getFileId(), newItem);
    }
}