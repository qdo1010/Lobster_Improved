//
// AnalysisWindowController.h
//  Roboplasm
//
//  Created by Joseph Ayers on 2/9/08.
//  Copyright 2008 Northeastern University. All rights reserved.
//

#import <Cocoa/Cocoa.h>
    //#import "GraphViewController.h"
    //#import "DSPViewController.h"
#import "OscilloscopeController.h"
    //#import "signalProcessing.h"
#import "OscilloscopeView.h"

@class GraphicsView;

@interface AnalysisWindowController : NSWindowController {

}
//@property(nonatomic, strong)  IBOutlet id   		delegate;
@property(strong)  IBOutlet NSWindow             	*analysisWindow;                  // the visualization window
                                                                                      //@property(strong)  IBOutlet NSTabViewItem        	*scopeItem;                  // the scope item
@property(strong)  IBOutlet OscilloscopeView     	*oscilloscopeView;
@property(strong)  IBOutlet OscilloscopeController 	*oscilloscopeController;               // the oscilloscope view controller

@property (strong) IBOutlet NSDrawer             	*scopeDrawer;
@property(strong)  NSView                        	*myCurrentView;				// the view to be updated
@property(strong)  NSViewController                 *myCurrentViewController;				// the view to be updated
																				//@property(nonatomic, assign)  NSInteger                     *scopeIndex;                  // the scope index
																				//@property(nonatomic, assign)  NSInteger                     *graphIndex;                  // the graph index
@property (nonatomic, strong) IBOutlet NSWindow             *progress;
@property (nonatomic, strong) IBOutlet NSProgressIndicator  *clusterTime;
@property (nonatomic, assign)          BOOL scopeInited;
@property (nonatomic, assign)          BOOL scopeScaled;


- (void)awakeFromNib;

    //- (IBAction) updateScopeDrawerOpenState;
    //- (IBAction) updateFormattingDrawerOpenState;

- (id)initWithDefaultWindowNib;
- (void)logAnalysisWindowController;
- (void)windowDidLoad;
- (IBAction) viewScope:(id)sender;
- (IBAction) viewChart:(id)sender;
- (IBAction) showProgress: (id) sender;
- (void) redrawProgressBar: (id)sender;
- (IBAction) endProgress: (id) sender;




@end
