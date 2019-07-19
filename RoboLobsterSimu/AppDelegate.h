//
//  AppDelegate.h
//  RoboLobsterSimu
//
//  Created by HongQuan Do on 6/14/17.
//  Copyright © 2017 HongQuan Do. All rights reserved.
//
#import <Cocoa/Cocoa.h>
#import <CoreData/CoreData.h>
#import "Waveforms.h"
#import "traceSelector.h"
#import "CommandStateViewController.h"
//#import "SynapseController.h"
#import "ParameterSetter.h"

@class OscilloscopeController;
@class AnalysisWindowController;
@class GraphViewController;
@class DSPViewController;
@class Waveforms;
@class TraceSelector;
@class PreferenceController;
@class CommandStateViewController;
    //#define numTraces = 8

#pragma mark --- Constants

@interface AppDelegate : NSObject <NSApplicationDelegate>

extern long elapsed;

#pragma mark --- Property Declarations
@property (readonly, strong) NSPersistentContainer *persistentContainer;


#pragma mark --- Window controller
@property (nonatomic, strong) IBOutlet AnalysisWindowController  *analysisWindowController;
@property (nonatomic, strong) IBOutlet TraceSelector             *traceSelector;
@property (nonatomic, strong) PreferenceController               *preferenceController;
@property (nonatomic, strong) CommandStateViewController         *commandController;
//@property (nonatomic, strong) SynapseController                  *synapseController; //create an instance of synapse controller here
@property (nonatomic, strong) ParameterSetter                    *parameterSetter; //create an instance of ParameterSetter here


#pragma mark --- Document and associated view stuff
@property (nonatomic, strong) NSDocumentController                  *documentController;
//@property (nonatomic, strong) SignalProcessing                      *signalProcessing;
@property (nonatomic, strong) Waveforms                             *traceWaveforms;

@property (nonatomic, strong) NSNumber                              *currentWorkingTime;
@property (nonatomic, assign) int                                    numTraces;
@property (nonatomic, assign) int                                    selectedTrace;
@property (nonatomic, assign) NSString                               *selectedTraceName;
@property (nonatomic, assign) int                                    outputFileType;
@property (nonatomic, assign) int                                    plotType;


@property (readwrite,retain) NSMutableArray                         *plottingArray;    //This is an array of traces
//@property (readwrite,retain) NSMutableArray                         *ipbuf;           //This is a trace array
@property (nonatomic, assign) BOOL                                   drawChart;
@property (nonatomic, assign) BOOL                                   drawUnclustered;
@property (nonatomic, assign, getter=isCalibrationType) BOOL         calibrationType;

#pragma mark --- menuItemsf
@property (nonatomic, strong) IBOutlet NSMenuItem                   *Cluster;
@property (nonatomic, strong) IBOutlet NSMenuItem                   *StaticCalibration;
@property (readwrite, assign) NSNumber*                                    SampleSize; //sample size of traces

@property (readwrite) double sweepDuration;
@property (readwrite) double sweepOffset;

@property (readwrite,retain) NSMutableArray                         *traceGainArray;    //This  an array of traces gain

@property (readwrite,retain) NSMutableArray                         *traceOffsetArray;    //This is an array of traces offset

@property (readwrite) int                         switchColor;    //This is an array of traces offset
@property (readwrite) int                         invertSign;    //This is an array of traces offset

@property (readwrite) int IDofCellChosen; //id of cell

@property (readwrite) double                         oneitertosec;    //This is an array of traces offset

- (IBAction)saveAction:(id)sender;

#pragma mark --- Load Nib files
#pragma mark --- Show Windows
- (IBAction) showAnalysisWindow: (id) sender;
- (IBAction) showParameterSetterWindow: (id) sender;
- (IBAction) openParameterSetterWindow:         (id) sender; //this is to open the Parameter Setter window

#pragma mark --- Signal Processing Menu Selections
- (IBAction) drawOscilloscope:         (id) sender; //This is a yellow on black graph of the analog traceWaveforms
-(void) displaySampledWaveforms : (NSMutableArray*)cellName : (double)offset : (double)duration;

//- (IBAction) drawChart:                (id) sender; //This is a blackon white graph of the analog traceWaveforms

#pragma mark --- User Interface
- (BOOL)validateMenuItem:(NSMenuItem *)menuItem;
@end

