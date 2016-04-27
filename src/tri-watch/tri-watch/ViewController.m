//
//  ViewController.m
//  tri-watch
//
//  Created by Andrey Krekhov on 05.12.15.
//  Copyright © 2015 Andrey Krekhov. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)initNetworkCommunication {
    NSLog(@"init network");
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)@"134.91.11.158", 5678, &readStream, &writeStream);
    inputStream = (NSInputStream *)CFBridgingRelease(readStream);
    outputStream = (NSOutputStream *)CFBridgingRelease(writeStream);
    [inputStream setDelegate:self];
    [outputStream setDelegate:self];
    [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [inputStream open];
    [outputStream open];
    NSLog(@"Status of outputStream: %i", [outputStream streamStatus]);
    
    
    
    
}


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    if ([WCSession isSupported]) {
        WCSession *session = [WCSession defaultSession];
        session.delegate = self;
        [session activateSession];
    }
    
    //[self initNetworkCommunication];

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)button:(id)sender {
    [self sendX];
}

- (void)sendX {
    [self initNetworkCommunication];
    NSData *data = [[NSData alloc] initWithData:[@"CON_0_1" dataUsingEncoding:NSASCIIStringEncoding]];
    int32_t size = 7;
    char buffer[20];
    char* sizePos = &buffer;
    char* dataPos = &buffer + 4;
    
    
    buffer[4] = 'C';
    buffer[5] = 'O';
    buffer[6] = 'N';
    buffer[7] = '_';
    buffer[8] = '0';
    buffer[9] = '_';
    buffer[10] = '1';
    
    
    *sizePos = size;
    
    [outputStream write:&buffer  maxLength:20];
    NSLog(@"data sent");

}

- (void)session:(nonnull WCSession *)session didReceiveMessage:(nonnull NSDictionary *)message replyHandler:(nonnull void (^)(NSDictionary * __nonnull))replyHandler {
    
    
    
    NSString *counterValue = [message objectForKey:@"counterValue"];
    
     NSLog(@"connect (app)");
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
