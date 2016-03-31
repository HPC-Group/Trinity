#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "common/IONodeProxy.h"
#include "frontend-base/ProcessingNodeProxy.h"

class ConnectionSingleton{
public:
    static ConnectionSingleton& getInstance(){
         static ConnectionSingleton    instance;
         return instance;
    }

    trinity::IONodeProxy& ioNode();
    trinity::ProcessingNodeProxy& processingNode();
    trinity::RendererProxy& renderer();

    bool isReady() {return m_initDone && m_renderer;}

    void connectIO(const std::string& hostname, const std::string& port);
    void connectProcessing(const std::string& hostname, const std::string& port);

    void initRenderer(const std::string& iohostname,
                      const std::string& ioport,
                      const uint32_t width,
                      const uint32_t height,
                      const std::string fileId = "FractalData@3");

private:
    ConnectionSingleton();
    ~ConnectionSingleton();

    std::unique_ptr<trinity::ProcessingNodeProxy>   _processingNode;
    std::unique_ptr<trinity::IONodeProxy>           _ioNode;
    std::unique_ptr<trinity::RendererProxy>         m_renderer;
    bool                                            m_initDone;

};
