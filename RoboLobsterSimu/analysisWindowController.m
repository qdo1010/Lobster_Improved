//
//  analysisWindowController.m
//  Roboplasm
//
//  Created by Joseph Ayers on 1/5/14.
//  Copyright 2014 Northeastern University. All rights reserved.
//

#import "analysisWindowController.h"
#import "AppDelegate.h"
#import "Waveforms.h"
#import "OscilloscopeController.h"
#import "OscilloscopeView.h"


@implementation AnalysisWindowController

@synthesize oscilloscopeController = _oscilloscopeController;
@synthesize oscilloscopeView       = _oscilloscopeView;
@synthesize myCurrentView          = _myCurrentView;
@synthesize myCurrentViewController=_myCurrentViewController;
@synthesize progress               = _progress;
@synthesize clusterTime            = _clusterTime;
@synthesize analysisWindow         = _analysisWindow;//This is the Graphics Window
                                                     //@synthesize scopeItem              = _scopeItem;//This is the oscilloscopeTabItem
@synthesize scopeDrawer            = _scopeDrawer;
@synthesize scopeInited            = _scopeInited;
@synthesize scopeScaled            = _scopeScaled;

/*  ••••••••••  Plot types
scattergram                        = 1,// This is a classic x/y scattergram, correlation coefficient and regression line optional
binnedScattergram                  = 2,// This is a scattergram with the x axis binned and plotted as the meain and sd/se
shapeSequence                      = 3,//This a series of shapes plotted individual in columns and rows
displacementShapes                 = 4,//This is a series of shapes superimposed.
flexion                            = 5,//This is a graph of the locus of flexion raw data plotted vs time
flexionWave                        = 6,//This is a graph of the locus of flexion waves plotted vs time
curvature                          = 7;//This is a graph of the curvature of flexion waves plotted vs locus
histogram                          = 8//This is a linear historgram
circHistogram                      = 9 this is a circular histogram
barGraph                           = 10//This is a horizontal bar graph
colGraph                           = 11//This is a vertical bar graph
•••••••••• */



enum	// popup tag choices
{
kScopeView                         = 0,
kGraphView                         = 1,
kDSPView                           = 2
};


NSString *const kScopeTitle        = @"OscilloscopeController";
NSString *const kGraphTitle        = @"GraphController";
NSString *const kDSPTitle          = @"DSPViewController";


    // -------------------------------------------------------------------------------
    //	changeViewController:whichViewTag
    //
    //	Change the current NSViewController to a new one based on a tag obtained from
    //	the NSPopupButton control.
    // -------------------------------------------------------------------------------

- (id)init
{
	AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
	AnalysisWindowController *awc;
	awc = [super initWithWindowNibName:@"AnalysisWindowController"];
	if (!awc)
		{
        NSLog(@"Warning! Could not load AnalysisWindowController file.\n");
		}
     else
		 {
		 NSLog(@"AnalysisWindowController Nib Loaded");
		[awc logAnalysisWindowController];
		 }
    return awc;
}

- (void)windowDidLoad
{
    NSLog(@"analysis view loaded");
    [super windowDidLoad];
    [self showWindow:nil];
    NSTimer *t = [NSTimer scheduledTimerWithTimeInterval: 0.1
                                                 target: self
                                                selector:@selector(updateRect)
                                userInfo: nil repeats:YES];
   // AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];

  //  [[[appDelegate analysisWindowController] oscilloscopeController] setDoInitAndScale:YES];
  //  NSRect iRect = [[self oscilloscopeView] bounds];
  //  [_oscilloscopeController setProxOffset:((iRect.size.height / 3) * 2)];
    // [[self oscilloscopeView] setNeedsDisplay:YES];
    
    [self viewScope:self];
    [[self scopeDrawer] open];
    
}

- (void) updateRect{
   // NSLog(@"updating");
    [[self oscilloscopeView] setNeedsDisplay:YES];
}
   // [self viewScope:self];


- (id)initWithDefaultWindowNib
{
/*	AnalysisWindowController *awc;
	awc = [super initWithWindowNibName:@"AnalysisWindowController"];
	if (!awc)
		{
		NSLog(@"Warning! Could not load AnalysisWindowController file.\n");
		}
	else
		{
		NSLog(@"AnalysisWindowController Nib Loaded");
		[awc logAnalysisWindowController];
		}
	return awc;
 */
	if (!self){
	self = [super initWithWindowNibName:@"AnalysisWindowController"];
	if (self)
		{
		[self logAnalysisWindowController];
		}
	else
		{
		NSLog(@"Warning! Could not load AnalysisWindowController nib.\n");
		}
	}
	return self;

}

- (void)logAnalysisWindowController{
        //Log Contents of analysisWindowController  with the current file data
    AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
    NSLog(@"\n Analysis Window Controller  %@ \n  analysisWindow: %@ \n oscilloscopeController:  %@\n oscilloscopeView: %@ \n  myCurrentView: %@ \n",
          self,                             //This is the Analysis Window controller
          [[appDelegate analysisWindowController] analysisWindow],             //This is the Graphics Window
          [[appDelegate analysisWindowController] oscilloscopeController],
          [[appDelegate analysisWindowController] oscilloscopeView],
          [[appDelegate analysisWindowController] myCurrentView]);
}

-(void) awakeFromNib
{
AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
    [appDelegate setAnalysisWindowController:self];
	[self  setScopeInited:NO];
	[self  logAnalysisWindowController];
        //    [[[appDelegate analysisWindowController] myTabView] setDelegate:(id)self];
    [[self scopeDrawer] open];
}

- (IBAction)  viewScope:(id)sender
{
    NSLog(@"Executing View Scope");
	AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
	[appDelegate showAnalysisWindow:self];
    //[appDelegate setAnalysisWindowController:[[AnalysisWindowController alloc]init]];
   // [self logAnalysisWindowController];
    
    [[[appDelegate analysisWindowController] oscilloscopeController] setDoInitAndScale:YES];
	[self  setMyCurrentView: (NSView*) [self oscilloscopeView]];
	//if(![[appDelegate analysisWindowController] scopeInited])
	//	[_oscilloscopeController initScopeSweep];
//    [self showWindow: _oscilloscopeView];

	NSRect iRect = [[self oscilloscopeView] bounds];
	[_oscilloscopeController setProxOffset:((iRect.size.height / 2) * 2)];
	[[self oscilloscopeView] setNeedsDisplay:YES];
    [[self scopeDrawer] open];
        //	return self;
    [[self scopeDrawer] open];
}

- (IBAction) viewChart:(id)sender
{
	AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];

	[appDelegate showAnalysisWindow:self];
	[self logAnalysisWindowController];
	[[[appDelegate analysisWindowController] oscilloscopeController] setDoInitAndScale:YES];
	[[self scopeDrawer] open];
	[self  setMyCurrentView: (NSView*) [self oscilloscopeView]];
	if(![[appDelegate analysisWindowController] scopeInited])
		[_oscilloscopeController initScopeSweep];

	NSRect  iRect = [[self oscilloscopeView] bounds];

	[_oscilloscopeController setProxOffset:((iRect.size.height /2) * 2)];
	[[[appDelegate analysisWindowController] oscilloscopeView] setNeedsDisplay:YES];
        //	return self;
}



- (IBAction) showProgress: (id) sender{
AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
    [[NSApplication sharedApplication] beginSheet:_progress
       modalForWindow:_analysisWindow
        modalDelegate: nil
       didEndSelector: NULL
          contextInfo: NULL];
    [_progress makeKeyAndOrderFront:[appDelegate analysisWindowController]];
}

- (IBAction) endProgress: (id) sender{
    [[NSApplication sharedApplication] endSheet:_progress];      //Return to normal event handling
    [_progress orderOut:(id)sender];  //HideTheSheet
}

- (void) redrawProgressBar: (id)sender {
AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
    [[appDelegate analysisWindowController] showProgress:[appDelegate analysisWindowController]]; //[progress makeKeyAndOrderFront:[appDelegate analysisWindowController]];
                              //[progress displayIfNeeded];

    float progressStep = 0.0;
        //progressStep                       = [[appDelegate signalProcessing] progressStep];
    [_clusterTime setIndeterminate:NO];
    [_clusterTime setDoubleValue:progressStep];
    [_clusterTime displayIfNeeded];
    NSLog(@"progressStep %f", progressStep);
    if (progressStep >= 99.0) {
        [[appDelegate analysisWindowController] endProgress:[appDelegate analysisWindowController]]; //[progress orderOut:nil];
    }
}

@end
