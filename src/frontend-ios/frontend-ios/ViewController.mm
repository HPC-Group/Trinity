//
//  ViewController.m
//  frontend-ios
//
//  Created by andrey on 25.03.16.
//  Copyright © 2016 andrey. All rights reserved.
//

#import "ViewController.h"

#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "commands/Vcl.h"

#include "ProcessingNodeProxy.h"
#include "common/ProxyUtils.h"
#include "common/IONodeProxy.h"

using namespace mocca::net;

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    [self initLog];
    
}


- (IBAction)goPressed:(id)sender {
    [self connect];
}


- (void)connect {
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), "192.168.1.10", "8678");
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), "192.168.1.10", "6678");
    
    m_ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
    LINFO("listing fractal dir with \"flat data\": ");
    auto datalist = m_ioNode->listFiles("FractalData@Flat");
    for(const auto& data : datalist) {
        LINFO(data.toString());
    }
    LINFO("listing fractal dir with \"bricked npot data\": ");
    datalist = m_ioNode->listFiles("FractalData@2b");
    for(const auto& data : datalist) {
        LINFO(data.toString());
    }
    
    m_processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));
    
    // the file id will be available after implementing the listdata command
    std::string fileId = "FractalData@3a";
    trinity::StreamingParams params(1000, 1000);
    m_renderer = m_processingNode->initRenderer(trinity::VclType::SimpleRenderer, fileId, endpointIO, params);
    
    m_renderer->initContext();
    m_renderer->setIsoValue(0.0f);
    // sending commands
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self frameLoop];
    });
    
}

static int displayed = 0;

- (void)frameLoop {
    int arrived = 0;
    float rot = 0.0f;
    m_image.image = m_image1;

    while(true) {

    auto visStream = m_renderer->getVisStream();
    auto frameNullable = visStream->get();
        if (!frameNullable.isNull()) {
            LINFO("frame arrived nr " + std::to_string(arrived++));
        auto frame = frameNullable.release();
            //m_image.image = nil;
        NSData* data = [NSData dataWithBytes:(const void *)frame.data() length:sizeof(unsigned char)*frame.size()];
        m_image1 = [UIImage imageWithData:data];
        dispatch_sync(dispatch_get_main_queue(), ^{
            m_image.image = m_image1;
            m_renderer->setIsoValue(rot);
            LINFO("frame displayed nr " + std::to_string(displayed++));
        });
        
        rot += 0.01f;
        //LINFO("content: " + frame.read(frame.size()));
    }
    }

}

- (void)initLog {
        using mocca::LogManager;
        LogManager::initialize(LogManager::LogLevel::Debug, true);
        auto console = new mocca::ConsoleLog();
        LogMgr.addLog(console);
        ConnectionFactorySelector::addDefaultFactories();
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
