//
//  InterfaceController.m
//  tri-watch WatchKit Extension
//
//  Created by Andrey Krekhov on 05.12.15.
//  Copyright © 2015 Andrey Krekhov. All rights reserved.
//

#import "InterfaceController.h"


@interface InterfaceController()

@end


@implementation InterfaceController




- (IBAction)connect {
    NSLog(@"connect please");
    
    NSString *connectString = @"connect please";
    NSDictionary *applicationData = [[NSDictionary alloc] initWithObjects:@[connectString] forKeys:@[@"counterValue"]];
    
    [[WCSession defaultSession] sendMessage:applicationData
                               replyHandler:^(NSDictionary *reply) {
                                   //handle reply from iPhone app here
                               }
                               errorHandler:^(NSError *error) {
                                   //catch any errors here
                               }
     ];

    

    
}
- (IBAction)handshake {
    NSLog(@"handshake");
}

- (void)awakeWithContext:(id)context {
    [super awakeWithContext:context];
    
    // Configure interface objects here.
}

- (void)willActivate {
    // This method is called when watch view controller is about to be visible to user
    [super willActivate];
    
    if ([WCSession isSupported]) {
        NSLog(@"WC is supported");
        WCSession *session = [WCSession defaultSession];
        session.delegate = self;
        [session activateSession];
    }
}

- (void)didDeactivate {
    // This method is called when watch view controller is no longer visible
    [super didDeactivate];
}

@end



