//
//  AppDelegate.m
//  RoboLobsterSimu
//
//  Created by HongQuan Do on 6/14/17.
//  Copyright © 2017 HongQuan Do. All rights reserved.
//

#import "AppDelegate.h"
#import "OscilloscopeController.h"
#import "analysisWindowController.h"
#import "Waveforms.h"


@interface AppDelegate ()

- (IBAction)saveAction:(id)sender;

@end

@implementation AppDelegate

#pragma mark Properties
@synthesize numTraces;
@synthesize selectedTrace;
@synthesize selectedTraceName;
@synthesize currentWorkingTime;
@synthesize plotType;
@synthesize drawUnclustered;
@synthesize drawChart;
@synthesize Cluster;
@synthesize StaticCalibration;
@synthesize calibrationType;
@synthesize traceSelector;
@synthesize plottingArray;
@synthesize traceWaveforms;
@synthesize persistentContainer = _persistentContainer;
@synthesize SampleSize;
//@synthesize durationArray;
//@synthesize offsetArray;

@synthesize sweepOffset;
@synthesize sweepDuration;

@synthesize traceOffsetArray;
@synthesize traceGainArray;
@synthesize switchColor;
@synthesize invertSign;
@synthesize oneitertosec;
@synthesize IDofCellChosen;

@synthesize commandController;
@synthesize synapseController;
#pragma mark - App Lifetime


- (void) awakeFromNib {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self
           selector:@selector(appDidLaunch:)
               name:NSApplicationDidFinishLaunchingNotification
             object:nil];
    
    traceSelector = [[TraceSelector alloc] init];
    [self setTraceSelector:traceSelector];
    [appDelegate showTraceSelectorWindow:self];

   
    NSLog(@"AppDelegate Nib Loaded");
}


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    NSLog(@"AppDelegate Did Finish Loading");
    
    int IterNum = 10000;
    //Tell C code this is how much I want to iterate,basically allocate this much memory to start calculating for this many iterations
    indicateNumberOfIteration(IterNum);
    
    traceWaveforms = [[Waveforms alloc] init];
    sweepDuration = 500;
    sweepOffset = 0;
    traceGainArray = [[NSMutableArray alloc] init];
    traceOffsetArray = [[NSMutableArray alloc] init];
    invertSign = 0;
    switchColor = 1;
    
    [self performSelectorInBackground:@selector(createWaveForm) withObject:nil];
  
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (void)appDidLaunch:(NSNotification*)notification
{
    NSLog(@"Application Just Launched");
   // NSLog(@"%@",[traceWaveforms ipbuf]);
}

#pragma mark - Menu Items
-(void) createWaveForm{
    xmain(); //run pc2dsp and write to temp files, automatically run in background async
   // NSLog(@"total elapse time for 1 iter = %f s", elapsedTime);
}

//-(void) readWaveForm{
  //  [traceWaveforms runCalculation];
//}
-(void) sendWaveform{
//    int size = [SampleSize intValue];
 //   NSMutableArray* arr = [NSMutableArray arrayWithObjects:@"cellElevatorL1",@"cellDepressorL1",@"cellSwingL1",@"cellStanceL1",@"cellElevatorR2", nil];
    //[traceWaveforms readMultipleArrays:arr :size];
}
-(void) displaySampledWaveforms : (NSMutableArray*)cellName : (double)offset : (double)duration{
 //  NSLog(@"sending!!!!!!!!");
    
    [traceWaveforms readMultipleArrays:cellName :offset :duration];
   // printf("1 loop iter elapsed = %ld ms\n", elapsed);

}

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem
{
    BOOL enabled = YES;//Should be no
    SEL action = [menuItem action];
    if (action == @selector(drawOscilloscope:))
    {
        enabled = YES;
    }
    else if (action == @selector(drawChart:))
    {
        enabled = YES;
    }
    return enabled;
}

//••••••••••••
#pragma mark --- NIB Loaders ---
//••••••••••••



//••••••••••••
#pragma mark ---Window Controllers ---
//••••••••••••


- (AnalysisWindowController *)analysisWindowController
{
    if (_analysisWindowController == nil)
    {
        AnalysisWindowController *newAnalsysisWindowController = [[AnalysisWindowController alloc] initWithDefaultWindowNib];
        [self setAnalysisWindowController:newAnalsysisWindowController];
    }
    return _analysisWindowController;
}


- (IBAction) showAnalysisWindow: (id) sender
{
    [[self analysisWindowController].window orderFront:self];
    
} // showAnalysisWindow


- (IBAction) showTraceSelectorWindow: (id) sender
{
    [[self traceSelector].window orderFront:self];
    
} // showAnalysisWindow


- (IBAction) showCommandWindow: (id) sender
{
    [[self commandController].window orderFront:self];
    
} // showCommandWindow

- (IBAction) showSynapseControllerWindow:(id)sender
{
    [[self synapseController].window orderFront:self];
    
} // showSynapseWindow

#pragma mark --- Getters and Setters ---


#pragma mark ••••• Window Controllers •••••

#pragma mark --- View Menu Commands

#pragma mark --- AcquisitionMenu

#pragma mark --- Start Analyses


- (IBAction) openCommandWindow:(id) sender //This is to open command controller{
{
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    NSLog(@"Executing command Window");
    CommandStateViewController* newCommandWindow = [[CommandStateViewController alloc] init];
    [self setCommandController:newCommandWindow];
    
    [appDelegate showCommandWindow:self];

}

- (IBAction) openSynapseControllerWindow: (id) sender //this is to open the synapseController window
{
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    NSLog(@"Executing synapse controller Window");
    SynapseController* newSynapseWindow = [[SynapseController alloc] init];
    [self setSynapseController:newSynapseWindow];
    [appDelegate showSynapseControllerWindow:self];
}



- (IBAction) drawOscilloscope:(id) sender //This is a virtual oscilloscope{
{
       AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    NSLog(@"Executing drawOscilloscope");

       AnalysisWindowController *newAnalysisWindowController = [[AnalysisWindowController alloc] init];
      [self setAnalysisWindowController:newAnalysisWindowController];

    [appDelegate showAnalysisWindow:self];
     [[appDelegate analysisWindowController]  viewScope:self];
     [appDelegate setDrawChart: NO];
    
}



#pragma mark - Core Data stack


- (NSPersistentContainer *)persistentContainer {
    // The persistent container for the application. This implementation creates and returns a container, having loaded the store for the application to it.
    @synchronized (self) {
        if (_persistentContainer == nil) {
            _persistentContainer = [[NSPersistentContainer alloc] initWithName:@"RoboLobsterSimu"];
            [_persistentContainer loadPersistentStoresWithCompletionHandler:^(NSPersistentStoreDescription *storeDescription, NSError *error) {
                if (error != nil) {
                    // Replace this implementation with code to handle the error appropriately.
                    // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
                    
                    /*
                     Typical reasons for an error here include:
                     * The parent directory does not exist, cannot be created, or disallows writing.
                     * The persistent store is not accessible, due to permissions or data protection when the device is locked.
                     * The device is out of space.
                     * The store could not be migrated to the current model version.
                     Check the error message to determine what the actual problem was.
                    */
                    NSLog(@"Unresolved error %@, %@", error, error.userInfo);
                    abort();
                }
            }];
        }
    }
    
    return _persistentContainer;
}

#pragma mark - Core Data Saving and Undo support

- (IBAction)saveAction:(id)sender {
    // Performs the save action for the application, which is to send the save: message to the application's managed object context. Any encountered errors are presented to the user.
    NSManagedObjectContext *context = self.persistentContainer.viewContext;

    if (![context commitEditing]) {
        NSLog(@"%@:%@ unable to commit editing before saving", [self class], NSStringFromSelector(_cmd));
    }
    
    NSError *error = nil;
    if (context.hasChanges && ![context save:&error]) {
        // Customize this code block to include application-specific recovery steps.              
        [[NSApplication sharedApplication] presentError:error];
    }
}

- (NSUndoManager *)windowWillReturnUndoManager:(NSWindow *)window {
    // Returns the NSUndoManager for the application. In this case, the manager returned is that of the managed object context for the application.
    return self.persistentContainer.viewContext.undoManager;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    // Save changes in the application's managed object context before the application terminates.
    NSManagedObjectContext *context = self.persistentContainer.viewContext;

    if (![context commitEditing]) {
        NSLog(@"%@:%@ unable to commit editing to terminate", [self class], NSStringFromSelector(_cmd));
        return NSTerminateCancel;
    }
    
    if (!context.hasChanges) {
        return NSTerminateNow;
    }
    
    NSError *error = nil;
    if (![context save:&error]) {

        // Customize this code block to include application-specific recovery steps.
        BOOL result = [sender presentError:error];
        if (result) {
            return NSTerminateCancel;
        }

        NSString *question = NSLocalizedString(@"Could not save changes while quitting. Quit anyway?", @"Quit without saves error question message");
        NSString *info = NSLocalizedString(@"Quitting now will lose any changes you have made since the last successful save", @"Quit without saves error question info");
        NSString *quitButton = NSLocalizedString(@"Quit anyway", @"Quit anyway button title");
        NSString *cancelButton = NSLocalizedString(@"Cancel", @"Cancel button title");
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:question];
        [alert setInformativeText:info];
        [alert addButtonWithTitle:quitButton];
        [alert addButtonWithTitle:cancelButton];

        NSInteger answer = [alert runModal];
        
        if (answer == NSAlertSecondButtonReturn) {
            return NSTerminateCancel;
        }
    }

    return NSTerminateNow;
}

@end
