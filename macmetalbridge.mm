#ifdef __APPLE__

#include "macmetalbridge.h"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

void* ForceMetalLayerForNSView(void* nsViewPtr)
{
    NSView* nsView = (__bridge NSView*)nsViewPtr;
    if (!nsView) {
        NSLog(@"[ForceMetalLayerForNSView] NSView is null!");
        return nullptr;
    }

    [nsView setWantsLayer:YES];
    if (![nsView.layer isKindOfClass:[CAMetalLayer class]]) {
        CAMetalLayer *metalLayer = [CAMetalLayer layer];
        [nsView setLayer:metalLayer];
    }
    return (__bridge void*)nsView.layer;
}

#endif 

