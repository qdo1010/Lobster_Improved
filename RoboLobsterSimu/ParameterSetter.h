#ifndef   PARAMETERSETTER_H
#define   PARAMETERSETTER_H
//  ParameterSetter.h
//  RoboLobsterSimu
//
//  Created by Quan Do on 1/31/19.
//  Copyright © 2019 HongQuan Do. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"
//@class AppDelegate;
#import "traceSelector.h"


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

@interface ParameterSetter : NSWindowController <NSTextFieldDelegate>

// Moved from Parameter Setter
@property (readwrite)  int neuronChosen;      //Duration of file in Sec
@property (weak) IBOutlet NSPopUpButton *neuronID;    //Neuron ID is formerly displayTraceID

 @property (readwrite) int *cellID;
 @property (readwrite) unsigned long *side;
 @property (readwrite) unsigned long *seg;
 @property (readwrite)  double *alpha;
 @property (readwrite)  double *sigma;
 @property (readwrite)  double *sigmaE;
 @property (readwrite)  double *sigmaI;
 @property (readwrite)  double *betaE;
 @property (readwrite)  double *betaI;
 @property (readwrite)  double *Idc;
- (IBAction)chooseNeuron:(id)sender;   //Formerly ChooseTraceID in OscilloscopeController
@property (readwrite) int settingUpParams;
@property (weak) IBOutlet NSButton *setParamsButton;
@property (readwrite)  int firstTimeChangeParams;       //is this first change

 
@property (strong) IBOutlet NSPopUpButton *synapseSelector;
@property (strong) IBOutlet NSPopUpButton *SideSelector;
@property (strong) IBOutlet NSPopUpButton *SegSelector;
@property (strong) IBOutlet NSTextField *xRPTextBox;
@property (strong) IBOutlet NSTextField *GammaTextBox;
@property (strong) IBOutlet NSTextField *gStrengthTextBox;

@property (strong) IBOutlet NSPopUpButton *synapseTypeSelector;

@property (readwrite) unsigned long *synapseID;
@property (readwrite)  double *xrp;
@property (readwrite)  double *gamma;
@property (readwrite)  double *gStrength;

@property (weak) IBOutlet NSTextField *cellNameTextBox; //cell Name
@property (weak) IBOutlet NSTextField *alphaTextBox;
@property (weak) IBOutlet NSTextField *sigmaTextBox;
@property (weak) IBOutlet NSTextField *sigmaETextBox;
@property (weak) IBOutlet NSTextField *sigmaITextBox;
@property (weak) IBOutlet NSTextField *betaETextBox;
@property (weak) IBOutlet NSTextField *betaITextBox;
@property (weak) IBOutlet NSTextField *IdcTextBox;

- (IBAction)ConfigureDefaultSynapseParams:(id)sender;

- (IBAction)updateSynapseParams:(id)sender;

//action to set param of synapses
- (IBAction)setSynapseParams:(id)sender;
- (IBAction)saveSynapseParams:(id)sender;
- (IBAction)resetParamsToDefault:(id)sender;
- (int)synapseMapping: (int)synapseID;

//Function prototypes to fix warnings
void editParam(int *neuronName, unsigned long *side, unsigned long *seg, double *a, double *s, double *sE, double *sI, double *bE, double *bI, double *Idc, unsigned long int size);
void editSynapseParam(unsigned long int *synapseName, unsigned long *side, unsigned long *seg,double*xrp, double *gamma, double *gStrength, int size);
void SaveAllParams(void);

void CreateParamFile (void);

@end

NS_ASSUME_NONNULL_END
#endif
