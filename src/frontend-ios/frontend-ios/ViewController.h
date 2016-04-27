//
//  ViewController.h
//  frontend-ios
//
//  Created by andrey on 25.03.16.
//  Copyright © 2016 andrey. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "ProcessingNodeProxy.h"
#include "common/IONodeProxy.h"
#include "RendererProxy.h"

@interface ViewController : UIViewController {
    std::unique_ptr<trinity::ProcessingNodeProxy> m_processingNode;
    std::unique_ptr<trinity::IONodeProxy> m_ioNode;
    std::unique_ptr<trinity::RendererProxy> m_renderer;
    __weak IBOutlet UIImageView *m_image;
    UIImage *m_image1;
}


@end

