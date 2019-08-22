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
@property (strong) IBOutlet NSPopUpButton *neuronSelector;
@property (strong) IBOutlet NSPopUpButton *neuronSideSelector;
@property (strong) IBOutlet NSPopUpButton *neuronSegSelector;
@property (strong) IBOutlet NSPopUpButton *synapseSideSelector;
@property (strong) IBOutlet NSPopUpButton *synapseSegSelector;
@property (strong) IBOutlet NSTextField *xRPTextBox;
@property (strong) IBOutlet NSTextField *GammaTextBox;
@property (strong) IBOutlet NSTextField *gStrengthTextBox;
@property (strong) IBOutlet NSTextField *CellName;
@property (strong) IBOutlet NSTextField *alphaTextBox;
@property (strong) IBOutlet NSTextField *sigmaTextBox;
@property (strong) IBOutlet NSTextField *sigmaETextBox;
@property (strong) IBOutlet NSTextField *sigmaITextBox;
@property (strong) IBOutlet NSTextField *betaETextBox;
@property (strong) IBOutlet NSTextField *betaITextBox;
@property (strong) IBOutlet NSTextField *IdcTextBox;

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
//- (IBAction)saveSynapseParams:(id)sender;

- (int)synapseMapping: (int)synapseID;

//Adding prototypes to remove warnings
void editSynapseParam(unsigned long int *synapseName, int* side, int* seg,double*xrp, double* gamma, double* gStrength, int size);
//void saveParamsToFile(int flagWriteToFile);

void CreateParamFile (void);
void setNeuronParams(int id, unsigned long side, unsigned long seg, double a, double s, double sE, double sI, double bE, double bI, double Idc);
void SaveAllParams(void);
void setSynapseParams(unsigned long int id, unsigned long side, unsigned long seg, double xrp, double gamma, double gStrength);
void setNeuronParams(int id, unsigned long side, unsigned long seg, double a, double s, double sE, double sI, double bE, double bI, double Idc);
double ReturnCurrentSynapseParams(long id, long side, long seg, int data);
double ReturnCurrentNeuronParams(long id, long side, long seg, int data);
@end

NS_ASSUME_NONNULL_END
