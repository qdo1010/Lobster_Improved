//
//  SynapseController.h
//  RoboLobsterSimu
//
//  Created by Quan Do on 1/31/19.
//  Copyright © 2019 HongQuan Do. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN
/*struct Synapse{
 NSString* name;
 int side;
 int seg;
 double xrp;
 double gamma;
 double gStrength;
 };*/

// create a dictionary
//what would this dictionary look like?
//idk?

@interface ParameterSetter : NSWindowController
@property (strong) IBOutlet NSPopUpButton *synapseSelector;
@property (strong) IBOutlet NSPopUpButton *SideSelector;
@property (strong) IBOutlet NSPopUpButton *SegSelector;
@property (strong) IBOutlet NSTextField *xRPTextBox;
@property (strong) IBOutlet NSTextField *GammaTextBox;
@property (strong) IBOutlet NSTextField *gStrengthTextBox;

@property (strong) IBOutlet NSPopUpButton *synapseTypeSelector;

@property (readwrite) unsigned long int *synapseID;
@property (readwrite) int *side;
@property (readwrite) int *seg;
@property (readwrite)  double *xrp;
@property (readwrite)  double *gamma;
@property (readwrite)  double *gStrength;


- (IBAction)ConfigureDefaultSynapseParams:(id)sender;

- (IBAction)updateSynapseParams:(id)sender;

//action to set param of synapses
- (IBAction)setSynapseParams:(id)sender;
- (IBAction)saveSynapseParams:(id)sender;

- (int)synapseMapping: (int)synapseID;

//Adding prototypes to remove warnings
void editSynapseParam(unsigned long int *synapseName, int* side, int* seg,double*xrp, double* gamma, double* gStrength, int size);
//void saveParamsToFile(int flagWriteToFile);


@end

NS_ASSUME_NONNULL_END
