//
//  traceSelector.h
//  RoboLobsterSimu
//
//  Created by Joseph Ayers on 6/15/17.
//  Copyright © 2017 HongQuan Do. All rights reserved.
//
//#import "traceSelector.h"
#import <Cocoa/Cocoa.h>
#import "Waveforms.h"
#import "AppDelegate.h"




@interface TraceSelector : NSWindowController{
}
- (IBAction)addCellTrace:(id)sender;  //send commnd to include the chosen traces
@property (nonatomic, strong) IBOutlet id                   delegate;
@property (nonatomic)         IBOutlet NSTableView          *cellTable;
@property (nonatomic, strong) IBOutlet NSWindow             *traceSelectorWindow;
@property (nonatomic)         IBOutlet NSTableView			*traceTable;
@property (nonatomic, strong) IBOutlet NSPopUpButton		*numberOfTracesSelector;
@property (nonatomic, strong) IBOutlet NSPopUpButton		*traceSelector;
@property (nonatomic, strong) IBOutlet NSPopUpButton		*cellSelector;
@property (strong)            IBOutlet NSTableColumn        *cellColView;
@property (strong)            IBOutlet NSTableColumn        *traceColView;
@property (nonatomic, strong) IBOutlet NSArrayController    *traceNameController;
@property (nonatomic, strong) IBOutlet NSArrayController    *traceNumberController;

@property (nonatomic, strong) NSDictionary                *tracecellDict;

@property (nonatomic, strong) NSMutableArray                *traceArray;
@property (nonatomic, strong) NSMutableArray                *traceNumberArray;
@property (nonatomic, strong) NSMutableArray                *traceArraytobeSent;

    //@property (nonatomic, strong) IBOutlet NSScroller     *verticalController;
    //@property (nonatomic, strong) IBOutlet NSScroller     *horizontalController;
- (IBAction)chooseTraceNums:(id)sender;
- (IBAction)selectTraces:(id)sender;
- (IBAction)selectCells:(id)sender;

- (void) awakeFromNib;
- (id)init;

@end
