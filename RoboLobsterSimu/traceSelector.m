//
//  traceSelector.m
//  RoboLobsterSimu
//
//  Created by Joseph Ayers on 6/15/17.
//  Copyright © 2017 HongQuan Do. All rights reserved.
//

#include "traceSelector.h";
#import <Foundation/Foundation.h>

@implementation TraceSelector

@synthesize traceSelectorWindow;
@synthesize traceTable;
@synthesize cellTable;
@synthesize delegate;
@synthesize numberOfTracesSelector;

@synthesize traceArray;
@synthesize traceNumberArray;

@synthesize traceSelector;
@synthesize cellSelector;

@synthesize traceNameController;
@synthesize traceNumberController;

@synthesize traceColView;
@synthesize cellColView;

@synthesize traceArraytobeSent;

const char * traceLabels[] = {
    "cellElevatorL1",
    "cellElevatorL2",
    "cellElevatorL3",
    "cellElevatorL4",
    "cellElevatorR1",
    "cellElevatorR2",
    "cellElevatorR3",
    "cellElevatorR4",
    "cellSwingL1",
    "cellSwingL2",
    "cellSwingL3",
    "cellSwingL4",
    "cellSwingR1",
    "cellSwingR2",
    "cellSwingR3",
    "cellSwingR4",
    "cellDepressorL1",
    "cellDepressorL2",
    "cellDepressorL3",
    "cellDepressorL4",
    "cellDepressorR1",
    "cellDepressorR2",
    "cellDepressorR3",
    "cellDepressorR4",
    "cellStanceL1",
    "cellStanceL2",
    "cellStanceL3",
    "cellStanceL4",
    "cellStanceR1",
    "cellStanceR2",
    "cellStanceR3",
    "cellStanceR4",
    "cellCoordL1",
    "cellCoordL2",
    "cellCoordL3",
    "cellCoordL4",
    "cellCoordR1",
    "cellCoordR2",
    "cellCoordR3",
    "cellCoordR4",
    "cellProtractorL1",
    "cellProtractorL2",
    "cellProtractorL3",
    "cellProtractorL4",
    "cellProtractorR1",
    "cellProtractorR2",
    "cellProtractorR3",
    "cellProtractorR4",
    "cellRetractorL1",
    "cellRetractorL2",
    "cellRetractorL3",
    "cellRetractorL4",
    "cellRetractorR1",
    "cellRetractorR2",
    "cellRetractorR3",
    "cellRetractorR4",
    "cellExtensorL1",
    "cellExtensorL2",
    "cellExtensorL3",
    "cellExtensorL4",
    "cellExtensorR1",
    "cellExtensorR2",
    "cellExtensorR3",
    "cellExtensorR4",
    "cellFlexorL1",
    "cellFlexorL2",
    "cellFlexorL3",
    "cellFlexorL4",
    "cellFlexorR1",
    "cellFlexorR2",
    "cellFlexorR3",
    "cellFlexorR4",
    "cellFL",
    "cellFR",
    "cellBL",
    "cellBR",
    "cellLLL",
    "cellLLR",
    "cellLTL",
    "cellLTR",
    "cellPcnLlow",
    "cellPcnLrosDown",
    "cellPcnLlevel",
    "cellPcnLrosUp",
    "cellPcnLpHigh",
    "cellPcnRlow",
    "cellPcnRrosDown",
    "cellPcnRlevel",
    "cellPcnRrosUp",
    "cellPcnRpHigh",
    "cellModComL",
    "cellModComR",
    "cellHL",
    "cellHR"
};

- (id)init
{
//    AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
    TraceSelector *ts;
    ts = [super initWithWindowNibName:@"TraceSelector"];
    traceNameController = [[NSArrayController alloc]init];
    traceNumberController = [[NSArrayController alloc]init];

    self.traceArray = [[NSMutableArray alloc] init];
    self.traceNumberArray = [[NSMutableArray alloc] init];
    NSLog(@"Hello trace selector");
    if (!ts)
    {
        NSLog(@"Warning! Could not load Trace Selector file.\n");
    }
    else
    {
        [cellSelector removeAllItems];
        NSLog(@"Trace Selector Nib Loaded");
    }
    return ts;
}


- (IBAction)chooseTraceNums:(id)sender
{
//    AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
    NSString *title;
    title = [numberOfTracesSelector titleOfSelectedItem];
       NSLog(@"Number of Traces changed to %@", title);
  //  [appDelegate setNumTraces:[title integerValue]];
    

}

- (IBAction)selectTraces:(id)sender {
//    AppDelegate *appDelegate           = [[NSApplication sharedApplication] delegate];
    NSString *title;
    title = [traceSelector titleOfSelectedItem];
    NSLog(@"Selected Trace is %@", title);
 //[appDelegate setSelectedTrace:[title integerValue]];
   
    [self.traceNumberArray addObject:title];
    [self.traceNumberController rearrangeObjects];

}

- (IBAction)selectCells:(id)sender {
    NSString *title;
    title = [cellSelector titleOfSelectedItem];
    NSLog(@"Selected cell is %@", title);
    [self.traceArray addObject:title];
    [self.traceNameController rearrangeObjects];

}

- (void) awakeFromNib
{
    [super awakeFromNib];
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    [appDelegate setTraceSelector:self];
    traceTable = [[NSTableView alloc] init];
   // cellColView = [[NSTableColumn alloc] init];
    [self loadView];

        //    [self setItemsEnabled:YES];
}

- (void)loadView
{
    traceTable = [[NSTableView alloc] init];
    traceTable.delegate = self;
    traceTable.dataSource = self;
    [traceTable reloadData];
    cellTable = [[NSTableView alloc] init];
    cellTable.delegate = self;
    cellTable.dataSource = self;
    [cellTable reloadData];
    
    }

- (void)windowDidLoad
{
    NSLog(@"trace view loaded");
    [super windowDidLoad];
    [self showWindow:nil];
    [cellSelector removeAllItems];
    [cellSelector addItemWithTitle:@"cellElevatorL1"];
    [cellSelector addItemWithTitle:@"cellElevatorL2"];
    [cellSelector addItemWithTitle:@"cellElevatorL3"];
    [cellSelector addItemWithTitle:@"cellElevatorL4"];
    [cellSelector addItemWithTitle:@"cellElevatorR1"];
    [cellSelector addItemWithTitle:@"cellElevatorR2"];
    [cellSelector addItemWithTitle:@"cellElevatorR3"];
    [cellSelector addItemWithTitle:@"cellElevatorR4"];
    [cellSelector addItemWithTitle:@"cellSwingL1"];
    [cellSelector addItemWithTitle:@"cellSwingL2"];
    [cellSelector addItemWithTitle:@"cellSwingL3"];
    [cellSelector addItemWithTitle:@"cellSwingL4"];
    [cellSelector addItemWithTitle:@"cellSwingR1"];
    [cellSelector addItemWithTitle:@"cellSwingR2"];
    [cellSelector addItemWithTitle:@"cellSwingR3"];
    [cellSelector addItemWithTitle:@"cellSwingR4"];
    [cellSelector addItemWithTitle:@"cellDepressorL1"];
    [cellSelector addItemWithTitle:@"cellDepressorL2"];
    [cellSelector addItemWithTitle:@"cellDepressorL3"];
    [cellSelector addItemWithTitle:@"cellDepressorL4"];
    [cellSelector addItemWithTitle:@"cellDepressorR1"];
    [cellSelector addItemWithTitle:@"cellDepressorR2"];
    [cellSelector addItemWithTitle:@"cellDepressorR3"];
    [cellSelector addItemWithTitle:@"cellDepressorR4"];
    [cellSelector addItemWithTitle:@"cellStanceL1"];
    [cellSelector addItemWithTitle:@"cellStanceL2"];
    [cellSelector addItemWithTitle:@"cellStanceL3"];
    [cellSelector addItemWithTitle:@"cellStanceL4"];
    [cellSelector addItemWithTitle:@"cellStanceR1"];
    [cellSelector addItemWithTitle:@"cellStanceR2"];
    [cellSelector addItemWithTitle:@"cellStanceR3"];
    [cellSelector addItemWithTitle:@"cellStanceR4"];
    [cellSelector addItemWithTitle:@"cellCoordL1"];
    [cellSelector addItemWithTitle:@"cellCoordL2"];
    [cellSelector addItemWithTitle:@"cellCoordL3"];
    [cellSelector addItemWithTitle:@"cellCoordL4"];
    [cellSelector addItemWithTitle:@"cellCoordR1"];
    [cellSelector addItemWithTitle:@"cellCoordR2"];
    [cellSelector addItemWithTitle:@"cellCoordR3"];
    [cellSelector addItemWithTitle:@"cellCoordR4"];
    [cellSelector addItemWithTitle:@"cellProtractorL1"];
    [cellSelector addItemWithTitle:@"cellProtractorL2"];
    [cellSelector addItemWithTitle:@"cellProtractorL3"];
    [cellSelector addItemWithTitle:@"cellProtractorL4"];
    [cellSelector addItemWithTitle:@"cellProtractorR1"];
    [cellSelector addItemWithTitle:@"cellProtractorR2"];
    [cellSelector addItemWithTitle:@"cellProtractorR3"];
    [cellSelector addItemWithTitle:@"cellProtractorR4"];
    [cellSelector addItemWithTitle:@"cellRetractorL1"];
    [cellSelector addItemWithTitle:@"cellRetractorL2"];
    [cellSelector addItemWithTitle:@"cellRetractorL3"];
    [cellSelector addItemWithTitle:@"cellRetractorL4"];
    [cellSelector addItemWithTitle:@"cellRetractorR1"];
    [cellSelector addItemWithTitle:@"cellRetractorR2"];
    [cellSelector addItemWithTitle:@"cellRetractorR3"];
    [cellSelector addItemWithTitle:@"cellRetractorR4"];
    [cellSelector addItemWithTitle:@"cellExtensorL1"];
    [cellSelector addItemWithTitle:@"cellExtensorL2"];
    [cellSelector addItemWithTitle:@"cellExtensorL3"];
    [cellSelector addItemWithTitle:@"cellExtensorL4"];
    [cellSelector addItemWithTitle:@"cellExtensorR1"];
    [cellSelector addItemWithTitle:@"cellExtensorR2"];
    [cellSelector addItemWithTitle:@"cellExtensorR3"];
    [cellSelector addItemWithTitle:@"cellExtensorR4"];
    [cellSelector addItemWithTitle:@"cellFlexorL1"];
    [cellSelector addItemWithTitle:@"cellFlexorL2"];
    [cellSelector addItemWithTitle:@"cellFlexorL3"];
    [cellSelector addItemWithTitle:@"cellFlexorL4"];
    [cellSelector addItemWithTitle:@"cellFlexorR1"];
    [cellSelector addItemWithTitle:@"cellFlexorR2"];
    [cellSelector addItemWithTitle:@"cellFlexorR3"];
    [cellSelector addItemWithTitle:@"cellFlexorR4"];
    [cellSelector addItemWithTitle:@"cellFL"];
    [cellSelector addItemWithTitle:@"cellFR"];
    [cellSelector addItemWithTitle:@"cellBL"];
    [cellSelector addItemWithTitle:@"cellBR"];
    [cellSelector addItemWithTitle:@"cellLLL"];
    [cellSelector addItemWithTitle:@"cellLLR"];
    [cellSelector addItemWithTitle:@"cellLTL"];
    [cellSelector addItemWithTitle:@"cellPcnLlow"];
    [cellSelector addItemWithTitle:@"cellPcnLrosDown"];
    [cellSelector addItemWithTitle:@"cellPcnLlevel"];
    [cellSelector addItemWithTitle:@"cellPcnLrosUp"];
    [cellSelector addItemWithTitle:@"cellPcnLpHigh"];
    [cellSelector addItemWithTitle:@"cellPcnRlow"];
    [cellSelector addItemWithTitle:@"cellPcnRrosDown"];
    [cellSelector addItemWithTitle:@"cellPcnRlevel"];
    [cellSelector addItemWithTitle:@"cellPcnRrosUp"];
    [cellSelector addItemWithTitle:@"cellPcnRpHigh"];
    [cellSelector addItemWithTitle:@"cellModComL"];
    [cellSelector addItemWithTitle:@"cellModComR"];
    [cellSelector addItemWithTitle:@"cellHL"];
    [cellSelector addItemWithTitle:@"cellHR"];
}

- (IBAction)addCellTrace:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    traceArraytobeSent = [[NSMutableArray alloc] init];
    
    NSLog(@"Executing AddCellTrace");
    for (int i = 0; i < [traceArray count]; i ++){
        [traceArraytobeSent addObject:[traceArray objectAtIndex:i]];
        
    }
    [appDelegate drawOscilloscope:appDelegate];

}
@end


////////////////OLD CODE//////////////////////////////


// [traceNumberController bind:@"contentArray" toObject:self withKeyPath:@"traceMajorArray" options:nil];

// [traceNameController bind:@"contentArray" toObject:self withKeyPath:@"traceArray" options:nil];

//bind num
// NSString *akey = @"trace";
// NSString *keypath = [NSString stringWithFormat:@"arrangedObjects.%@",akey];
// [traceColView bind:NSValueBinding toObject:traceNumberController withKeyPath:keypath options:nil];


//bind name
//  NSString *akey2 = @"cell";
//  NSString *keypath2 = [NSString stringWithFormat:@"arrangedObjects.%@",akey2];
//[cellColView bind:NSValueBinding toObject:traceNameController withKeyPath:keypath2 options:nil];

/*-(id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
 
 return [traceArray objectAtIndex:row];
 }
 
 - (NSView *)tableView:(NSTableView *)tableView
 viewForTableColumn:(NSTableColumn *)tableColumn
 row:(NSInteger)row {
 NSLog(@"?");
 // Get an existing cell with the MyView identifier if it exists
 NSTextField *result = [tableView makeViewWithIdentifier:@"Trace Selector" owner:self];
 // There is no existing cell to reuse so create a new one
 if (result == nil) {
 result = [[NSTextField alloc] init];
 // Create the new NSTextField with a frame of the {0,0} with the width of the table.
 // Note that the height of the frame is not really relevant, because the row height will modify the height.
 //        result = [[NSTextField alloc] initWithFrame:...];
 
 // The identifier of the NSTextField instance is set to MyView.
 // This allows the cell to be reused.
 result.identifier = @"Trace Selector";
 
 }
 
 // result is now guaranteed to be valid, either as a reused cell
 // or as a new cell, so set the stringValue of the cell to the
 // nameArray value at row
 result.stringValue = [self.traceArray objectAtIndex:row -1];
 
 // Return the result
 return result;
 }
 
 - (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
 NSLog(@"hello, i am cal");
 return traceArray.count;
 }*/


//   traceMajorArray=[traceNumberArray arrayByAddingObjectsFromArray:traceArray];

// [traceMajorArray addObject:traceNumberArray];
// [traceMajorArray addObject:traceArray];

//  NSLog(@"Add a new property");

// PCSDescriptionProperty *property = [PCSDescriptionProperty insertInManagedObjectContext: self.storage.managedObjectContext];
//  property.name = @"New property";
//  property.info = @"New property info";
