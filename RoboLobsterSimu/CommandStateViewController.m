//
//  CommandStateViewController.m
//  Roboplasm
//
//  Created by Brian King on 4/5/13.
//  Copyright (c) 2013 Northeastern Univeristy. All rights reserved.
//

#import "CommandStateViewController.h"
//#import "TableDefinition.h"
#import "AppDelegate.h"
@implementation CommandStateViewController
@synthesize pitchBox;
@synthesize rollBox;
@synthesize abdBox;
@synthesize htBox;
//@synthesize ltChYBox;
//@synthesize rtChYBox;
//@synthesize rAntYBox;
@synthesize ltAntYBox;
@synthesize spdBox;
@synthesize yawBox;
@synthesize chPBox;
@synthesize uroBox;

/*- (BOOL)canEditTableType:(enum TableType)tableType
{
    return tableType == TableTypeCommandState;
}*/
- (void) awakeFromNib
{
    [super awakeFromNib];
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    
  //  [appDelegate setTraceSelector:self];
  //  traceTable = [[NSTableView alloc] init];
  // cellColView = [[NSTableColumn alloc] init];
  //  [self loadView];
    
  //    [self setItemsEnabled:YES];
}


- (void)setCurrentParameters:(NSMutableDictionary *)currentParameters
{
    [self willChangeValueForKey:@"currentParameters"];
    _currentParameters = currentParameters;
    [self didChangeValueForKey:@"currentParameters"];
    
    [self updateBoxes:self];
   // self.spriteView.currentParameters = currentParameters;
   // [self.spriteView setNeedsDisplay:YES];
}

/*- (void)setCurrentTable:(Table *)currentTable
{
    [self willChangeValueForKey:@"currentTable"];
    _currentTable = currentTable;
    [self didChangeValueForKey:@"currentTable"];
    
    if ([self.currentTable.items count] > 0)
        self.currentParameters = [self.currentTable.items objectAtIndex:0];
    else
        self.currentParameters = [self defaultValues];
}

- (NSMutableDictionary *)defaultValues
{
    return [[TableDefinition definitionForType:TableTypeCommandState].defaultValues mutableCopy];
}

- (void)timeDidChangeToTimeKey:(NSString *)timeKey
{
    NSUInteger tableIndex = [self.currentTable indexOfTimeKey:timeKey];
    if (tableIndex != NSNotFound)
        self.currentParameters = [self.currentTable.items objectAtIndex:tableIndex];
    else
        self.currentParameters = [self defaultValues];
}

- (NSView *)videoOverlayView
{
    return nil;
}

*/

- (void)updateTable
{
    [self.delegate commandStateVC:self didUpdateParameters:self.currentParameters];
}

- (IBAction) updateBoxes: (id)sender{
    
	[abdBox setIntValue:  [[self.currentParameters valueForKey:@"Abd"]  intValue]];
	[yawBox setIntValue:  [[self.currentParameters valueForKey:@"yaw"]  intValue]];
	[chPBox setIntValue:  [[self.currentParameters valueForKey:@"chP"]  intValue]];
	[htBox setIntValue:  [[self.currentParameters valueForKey:@"ht"]  intValue]];
	[ltAntYBox setIntValue:  [[self.currentParameters valueForKey:@"lAntYaw"]  intValue]];
	[pitchBox setIntValue:  [[self.currentParameters valueForKey:@"ptch"]  intValue]];
//	[ltChYBox setIntValue:  [[self.currentParameters valueForKey:@"lChY"]  intValue]];
//	[rAntYBox setIntValue:  [[self.currentParameters valueForKey:@"rAntYaw"]  intValue]];
//	[rtChYBox setIntValue:  [[self.currentParameters valueForKey:@"rChY"]  intValue]];
	[rollBox setIntValue:  [[self.currentParameters valueForKey:@"roll"]  intValue]];
	[spdBox setIntValue:  [[self.currentParameters valueForKey:@"spd"]  intValue]];
	[uroBox setIntValue:  [[self.currentParameters valueForKey:@"uro"]  intValue]];
}



- (IBAction)setAbPitch:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: Ext] forKey:@"Abd"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:Ele]  forKey:@"Abd"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:Norm]  forKey:@"Abd"];}break;
		case 3:{[self.currentParameters setValue: [NSNumber numberWithInt:Dep]  forKey:@"Abd"];}break;
		case 4:{[self.currentParameters setValue: [NSNumber numberWithInt:Flex]  forKey:@"Abd"];}break;
	}
	[abdBox setIntValue:  [[self.currentParameters valueForKey:@"Abd"]  intValue]];
	[self updateTable];
}

- (IBAction)setYaw:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: hardLeft] forKey:@"yaw"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:easyLeft]  forKey:@"yaw"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:mystraight]  forKey:@"yaw"];}break;
		case 3:{[self.currentParameters setValue: [NSNumber numberWithInt:easyRight]  forKey:@"yaw"];}break;
		case 4:{[self.currentParameters setValue: [NSNumber numberWithInt:hardRight]  forKey:@"yaw"];}break;
	}
	[yawBox setIntValue:  [[self.currentParameters valueForKey:@"yaw"]  intValue]];
	[self updateTable];
}

- (IBAction)setChPitch:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: hHigh] forKey:@"chP"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:hNor]  forKey:@"chP"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:hLow]  forKey:@"chP"];}break;
	}
	[chPBox setIntValue:  [[self.currentParameters valueForKey:@"chP"]  intValue]];
	[self updateTable];
}

- (IBAction)setHeight:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: hHigh] forKey:@"ht"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:hNor]  forKey:@"ht"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:hLow]  forKey:@"ht"];}break;
	}
	[htBox setIntValue:  [[self.currentParameters valueForKey:@"ht"]  intValue]];
	[self updateTable];
}

- (IBAction)setLAntYaw:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: ltAtPro] forKey:@"lAntYaw"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:ltAtNor]  forKey:@"lAntYaw"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:ltAtLat]  forKey:@"lAntYaw"];}break;
		case 3:{[self.currentParameters setValue: [NSNumber numberWithInt:ltAtRet]  forKey:@"lAntYaw"];}break;
	}
	[ltAntYBox setIntValue:  [[self.currentParameters valueForKey:@"lAntYaw"]  intValue]];
	[self updateTable];
}

//- (IBAction)setLClawYaw:(id)sender
//{
//    
//	NSInteger selector = [sender selectedRow];
//	switch (selector){
//		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: rosUp] forKey:@"lChY"];}break;
//		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:pLevel]  forKey:@"lChY"];}break;
//		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:rosDn]  forKey:@"lChY"];}break;
//	}
//	[ltChYBox setIntValue:  [[self.currentParameters valueForKey:@"lChY"]  intValue]];
//	[self updateTable];
//	
//}

- (IBAction)setPitch:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: rosUp] forKey:@"ptch"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:pLevel]  forKey:@"ptch"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:rosDn]  forKey:@"ptch"];}break;
    }
	[pitchBox setIntValue:  [[self.currentParameters valueForKey:@"ptch"]  intValue]];
	[self updateTable];
}

//- (IBAction)setRAntYaw:(id)sender
//{
//    
//	NSInteger selector = [sender selectedRow];
//	switch (selector){
//		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: ltAtPro] forKey:@"rAntYaw"];}break;
//		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:ltAtNor]  forKey:@"rAntYaw"];}break;
//		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:ltAtLat]  forKey:@"rAntYaw"];}break;
//		case 3:{[self.currentParameters setValue: [NSNumber numberWithInt:ltAtRet]  forKey:@"rAntYaw"];}break;
//	}
//	[rAntYBox setIntValue:  [[self.currentParameters valueForKey:@"rAntYaw"]  intValue]];
//	[self updateTable];
//}
//
//- (IBAction)setRClawYaw:(id)sender
//{
//    
//	NSInteger selector = [sender selectedRow];
//	switch (selector){
//		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: rosUp] forKey:@"rChY"];}break;
//		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:pLevel]  forKey:@"rChY"];}break;
//		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:rosDn]  forKey:@"rChY"];}break;
//	}
//	[rtChYBox setIntValue:  [[self.currentParameters valueForKey:@"rChY"]  intValue]];
//	[self updateTable];
//	
//}

- (IBAction)setRoll:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: ltUp] forKey:@"roll"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:rLevel]  forKey:@"roll"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:rtUp]  forKey:@"roll"];}break;
	}
	[rollBox setIntValue:  [[self.currentParameters valueForKey:@"roll"]  intValue]];
	[self updateTable];
}

- (IBAction)setSpeed:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: high] forKey:@"spd"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:medium]  forKey:@"spd"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:slow]  forKey:@"spd"];}break;
		case 3:{[self.currentParameters setValue: [NSNumber numberWithInt:mystop]  forKey:@"spd"];}break;
	}
	[spdBox setIntValue:  [[self.currentParameters valueForKey:@"spd"]  intValue]];
	[self updateTable];
}

- (IBAction)setUropods:(id)sender
{
    
	NSInteger selector = [sender selectedRow];
	switch (selector){
		case 0:{[self.currentParameters setValue: [NSNumber numberWithInt: uroFlare] forKey:@"uro"];}break;
		case 1:{[self.currentParameters setValue: [NSNumber numberWithInt:uroNorm]  forKey:@"uro"];}break;
		case 2:{[self.currentParameters setValue: [NSNumber numberWithInt:uroAdd]  forKey:@"uro"];}break;
	}
	[uroBox setIntValue:  [[self.currentParameters valueForKey:@"uro"]  intValue]];
    [self updateTable];
}


- (IBAction)setlFor:(id)sender{
    bool sta = [sender state];
	NSNumber * onOrOff =[NSNumber numberWithBool:sta] ;
	[self.currentParameters setValue: onOrOff forKey:@"lFor"];
    [self updateTable];
}

- (IBAction)setrFor:(id)sender{
    
	NSNumber * onOrOff =[NSNumber numberWithBool:(bool)[sender state]] ;
	[self.currentParameters setValue: onOrOff forKey:@"rFor"];
	[self updateTable];
}

- (IBAction)setlBack:(id)sender{
    
	NSNumber * onOrOff =[NSNumber numberWithBool:(bool)[sender state]] ;
	[self.currentParameters setValue: onOrOff forKey:@"lBak"];
	[self updateTable];
}
- (IBAction)setrBack:(id)sender{
    
	NSNumber * onOrOff =[NSNumber numberWithBool:(bool)[sender state]] ;
	[self.currentParameters setValue: onOrOff forKey:@"rBak"];
	[self updateTable];
}
- (IBAction)setlLead:(id)sender{
    
	NSNumber * onOrOff =[NSNumber numberWithBool:(bool)[sender state]] ;
	[self.currentParameters setValue: onOrOff forKey:@"lLed"];
	[self updateTable];
}

- (IBAction)setrLead:(id)sender{
    
	NSNumber * onOrOff =[NSNumber numberWithBool:(bool)[sender state]] ;
	[self.currentParameters setValue: onOrOff forKey:@"rLed"];
	[self updateTable];
}

- (IBAction)setlTrail:(id)sender{
    
	NSNumber * onOrOff =[NSNumber numberWithBool:(bool)[sender state]] ;
	[self.currentParameters setValue: onOrOff forKey:@"lTra"];
	[self updateTable];
}
- (IBAction)setrTrail:(id)sender{
    
	NSNumber * onOrOff =[NSNumber numberWithBool:(bool)[sender state]] ;
	[self.currentParameters setValue: onOrOff forKey:@"rTra"];
	[self updateTable];
}



@end
