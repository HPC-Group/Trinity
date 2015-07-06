
#include "logging/logmanager.h"
#include "IOLocal.h"

#include <IO/IOManager.h>
#include <IO/TuvokIOError.h>

#include <core/FileFinder.h>

using namespace std;
using namespace Core::Math;

namespace Tuvok {

class DSEntryFreed : public std::runtime_error {
public:
    DSEntryFreed() : std::runtime_error("Dataset reached a retain count of 0 and is being freed!") {}
};


IOLocal::IOLocal(std::string serviceName) {
}

IOLocal::~IOLocal() {
}

shared_ptr<LinearIndexDataset> IOLocal::getDataset(uint16_t datasetHandle){
    std::lock_guard<std::recursive_mutex> _lock(_dsMutex);

    auto datasetIt = _handleToDS.find(datasetHandle);
    if(datasetIt == _handleToDS.end())
        return nullptr;
        //throw DatasetNotFoundException(datasetHandle);

    shared_ptr<Dataset> ds = datasetIt->second.getDataset();

    auto retPtr = std::dynamic_pointer_cast<LinearIndexDataset>(ds);

    if(!retPtr)
    return nullptr;
        //throw DatasetCastException("LinearIndexDataset");

    return retPtr;
}

vector<string> IOLocal::getFileList() {
    std::vector<std::string> files;
    Core::FileFinder::getInstance().readFiles("DataSets/",files);
    Core::FileFinder::getInstance().readFiles("",files);
    Core::FileFinder::getInstance().readFiles("datasets/",files);
    std::cout << "FOUND FILES : "<< files.size();
    for(std::string s : files){
        std::cout << s << std::endl;
    }
    return files;
}

uint16_t IOLocal::openFile(string filename) {


    std::lock_guard<std::recursive_mutex> _lock(_dsMutex);

    auto datasetIt = _filenameToDs.find(filename);
    if(datasetIt != _filenameToDs.end()) {
        datasetIt->second.incrRetainCount();
        return datasetIt->second.getHandle();
    }
    else {
        const IOManager& mgr = IOManager::Instance();

        try {
            Dataset* ds = mgr.CreateDataset(filename, mgr.GetMaxBrickSize(), false);
            if(NULL == ds) {
                return 0;
                //throw DatasetCreationException("Unknown error occured.");
            }
            LINFOC("LocalIO","Load successful, initializing renderer!");


            shared_ptr<Dataset> ds_p = shared_ptr<Dataset>(ds);

            m_dataset = std::dynamic_pointer_cast<LinearIndexDataset>(ds_p);

            return 1;
        }
		catch (const io::DSOpenFailed& e) {
            string exceptionString = "IOManager could not load file(" + filename + "), giving up...";
            string reason = e.what();
            if(!reason.empty())
                exceptionString += string("(Detail: ") + reason + string(")");

            //throw DatasetCreationException(exceptionString);

            return 0;
        }
    }
}
void IOLocal::closeDataset() {
    std::lock_guard<std::recursive_mutex> _lock(_dsMutex);

    auto datasetIt = _handleToDS.find(0);
    if(datasetIt == _handleToDS.end())
        return;

    DSEntry& entry = datasetIt->second;

    try {
        entry.decrRetainCount();
    }
    catch (DSEntryFreed&) {
        _filenameToDs.erase(entry.getFilename());
        _handleToDS.erase(entry.getHandle());
    }
}

Vec3ui IOLocal::GetMaxBrickSize() {
    return m_dataset->GetMaxBrickSize();
}
Vec3ui IOLocal::GetMaxUsedBrickSizes( ){
    return m_dataset->GetMaxUsedBrickSizes();
}
MinMaxBlock IOLocal::MaxMinForKey(  const BrickKey& key){
    return m_dataset->MaxMinForKey(key);
}
unsigned IOLocal::GetLODLevelCount( ){
    return m_dataset->GetLODLevelCount();
}
uint64_t IOLocal::GetNumberOfTimesteps( ){
    return m_dataset->GetNumberOfTimesteps();
}
Vec3ui64 IOLocal::GetDomainSize(  const size_t lod, const size_t ts){
    return m_dataset->GetDomainSize(lod, ts);
}
Vec3d IOLocal::GetScale( ){
    return m_dataset->GetScale();
}
Vec3ui IOLocal::GetBrickOverlapSize( ){
    return m_dataset->GetBrickOverlapSize();
}
size_t IOLocal::GetLargestSingleBrickLOD(  size_t ts){
    return m_dataset->GetLargestSingleBrickLOD(ts);
}
Vec3ui IOLocal::GetBrickVoxelCounts(  const BrickKey& key){
    return m_dataset->GetBrickVoxelCounts(key);
}
Vec3f IOLocal::GetBrickExtents(  const BrickKey& key){
    return m_dataset->GetBrickExtents(key);
}
Vec3ui IOLocal::GetBrickLayout(  size_t LoD, size_t timestep){
    return m_dataset->GetBrickLayout(LoD, timestep);
}
unsigned IOLocal::GetBitWidth( ){
    return m_dataset->GetBitWidth();
}
uint64_t IOLocal::GetComponentCount( ){
	return m_dataset->GetComponentCount();
}
uint64_t IOLocal::GetTotalBrickCount( ){
	return m_dataset->GetTotalBrickCount();
}
bool IOLocal::GetIsSigned( ){
    return m_dataset->GetIsSigned();
}
bool IOLocal::GetIsFloat( ){
    return m_dataset->GetIsFloat();
}
Vec4ui IOLocal::IndexTo4D(  const BrickKey& key){
    return m_dataset->IndexTo4D(key);
}
BrickKey IOLocal::IndexFrom4D(  const Vec4ui& four, size_t timestep){
    return m_dataset->IndexFrom4D(four, timestep);
}

bool IOLocal::GetBrick(  const BrickKey& key, std::vector<uint8_t>& vec){
    return m_dataset->GetBrick(key, vec);
}
bool IOLocal::GetBrick(  const BrickKey& key, std::vector<uint16_t>& vec){
    return m_dataset->GetBrick(key, vec);
}
bool IOLocal::GetBrick(  const BrickKey& key, std::vector<uint32_t>& vec){
    return m_dataset->GetBrick(key, vec);
}
/*bool IOService::GetBrick(  const BrickKey& key, std::vector<uint64_t>& vec){
    return getDataset(handle.customHandle)->GetBrick(key, vec);
}*/
bool IOLocal::GetBrick(  const BrickKey& key, std::vector<int8_t>& vec){
    return m_dataset->GetBrick(key, vec);
}
bool IOLocal::GetBrick(  const BrickKey& key, std::vector<int16_t>& vec){
    return m_dataset->GetBrick(key, vec);
}
bool IOLocal::GetBrick(  const BrickKey& key, std::vector<int32_t>& vec){
    return m_dataset->GetBrick(key, vec);
}
/*bool IOService::GetBrick(  const BrickKey& key, std::vector<int64_t>& vec){
    return getDataset(handle.customHandle)->GetBrick(key, vec);
}*/
bool IOLocal::GetBrick(  const BrickKey& key, std::vector<float>& vec){
    return m_dataset->GetBrick(key, vec);
}
bool IOLocal::GetBrick(  const BrickKey& key, std::vector<double>& vec){
    return m_dataset->GetBrick(key, vec);
}
Vec2f IOLocal::GetRange( ){
	std::pair<float, float> f = m_dataset->GetRange();
	return Vec2f(f.first, f.second);
}
}
