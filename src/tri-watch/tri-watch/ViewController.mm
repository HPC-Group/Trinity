//
//  ViewController.m
//  tri-watch
//
//  Created by Andrey Krekhov on 05.12.15.
//  Copyright © 2015 Andrey Krekhov. All rights reserved.
//

#import "ViewController.h"
#import <WatchConnectivity/WatchConnectivity.h>

#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/MoccaNetworkService.h"
#include "mocca/net/TCPNetworkService.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"
#include "ProcessingPrx.h"
#include "common/Commands.h"

using namespace mocca::net;


@interface ViewController ()

@end

@implementation ViewController

std::unique_ptr<trinity::ProcessingPrx> processingNode;
static int reconnectInSec = 5;

- (void) initLog {
    using mocca::LogManager;
    LogManager::initialize(LogManager::LogLevel::Debug, true);
    auto console = new mocca::ConsoleLog();
    LogMgr.addLog(console);
}

- (void)initNetworkCommunication {
    NSLog(@"init network");
    [self initLog];
    
    NetworkServiceLocator::provideService(std::make_shared<MoccaNetworkService>(
                                                                                std::unique_ptr<IPhysicalNetworkService>(new TCPNetworkService())));
    
    Endpoint processingEndpoint(MoccaNetworkService::protocolStatic(),
                                TCPNetworkService::transportStatic(), "134.91.11.158:5678");
    

    processingNode =
    std::unique_ptr<trinity::ProcessingPrx>(new trinity::ProcessingPrx(processingEndpoint));
    

    
}

- (void)sendX {
    NSLog(@"sending... (app)");
    
    processingNode->connect();
    processingNode->spawnRenderer(trinity::vcl::DUMMY_RENDERER);
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    if ([WCSession isSupported]) {
        WCSession *session = [WCSession defaultSession];
        session.delegate = self;
        [session activateSession];
    }
    
    [self initNetworkCommunication];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)connectButton:(id)sender {
    [self sendX];
}

- (void)session:(nonnull WCSession *)session didReceiveMessage:(nonnull NSDictionary *)message replyHandler:(nonnull void (^)(NSDictionary * __nonnull))replyHandler {
    
    NSLog(@"msg from watch");
    
    [self sendX];
    
    /*
    //Use this to update the UI instantaneously (otherwise, takes a little while)
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [self.counterData addObject:counterValue];
        [self.mainTableView reloadData];
    });
     */
}

@end
