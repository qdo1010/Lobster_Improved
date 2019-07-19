#ifndef   OSCILLOSCOPEVIEW_H
#define   OSCILLOSCOPEVIEW_H
//  OScopeView.h
//  Roboplasm
//
//  Created by Joseph Ayers on 6/20/13.
//  Copyright (c) 2013 Northeastern Univeristy. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"
#import "analysisWindowController.h"
    //#import "signalProcessing.h"
#import "Waveforms.h"
//#import "DSPSources.h"
    //#import "MovieController.h"
#import "OscilloscopeController.h"
    //#import "GraphicsView.h"

@interface OscilloscopeView : NSView {
}
@property (nonatomic, assign) NSRect wRect;      // This is the framing Rect of the Graphics View.
@property (nonatomic, assign) CGRect gRect;      // This is the framing Rect of the Graph.

@property (nonatomic, strong) NSColorList *graphColors;
- (void)drawRect:(NSRect)rect;


@end
#endif
