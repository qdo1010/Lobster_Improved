
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


#define mmSeg    4              //  Number of Segments in a Spinal Cord model (110)
#define mmSeg2   2*mmSeg        //  Number of Segments in a Spinal Cord model (110)
#define mmSide    2
#define pitchStates 5
//Trying this for setting up dictionary

/*
typedef struct{
    double alphaSlow, alphaMed, alphaFast, alphaStop;
} alphaVal;
typedef struct{
    double sigmaSlow, sigmaMed, sigmaFast, sigmaStop;
} sigmaVal;

typedef struct{
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, sigmaE, sigmaI, betaE, betaI, Idc;
    double alpha;
    double sigma;
    int spike;
} burstingNeuron;

typedef struct{
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, sigmaE, sigmaI, betaE, betaI, Idc;
    double alpha;
    double sigma;
    int spike;
} spikingNeuron;

typedef struct{
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, sigmaE, sigmaI, betaE, betaI, Idc;
    double alpha;
    double sigma;
    int spike;
    double yr, xr, x2, y2;
    double alphaInit;
} pacemakerNeuron;

typedef struct{
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, sigmaE, sigmaI, betaE, betaI, Idc;
    double alpha;
    double sigma;
    int spike;
} customNeuron;

typedef struct{
    double xRp;            //Reversal Potential
    double gamma;          //Time Constantf
    double gStrength;
} synapse;

typedef struct{
    double xRp;            //Reversal Potential
    double gamma;          //Time Constantf
    double gStrength;
} modSynapse;

typedef struct{
    burstingNeuron burstingNeuron;
    spikingNeuron spikingNeuron;
    pacemakerNeuron pacemakerNeuron;
    customNeuron customNeuron;
    synapse synapse;
    modSynapse modSynapse;
} paramStruct;
*/
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
@property (strong) IBOutlet NSPopUpButton *neuronTypeSelector;

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
- (id)initWithDefaultWindowNib;
void editParam(int *neuronName, unsigned long *side, unsigned long *seg, double *a, double *s, double *sE, double *sI, double *bE, double *bI, double *Idc, unsigned long int size);
void editSynapseParam(unsigned long int *synapseName, unsigned long *side, unsigned long *seg,double*xrp, double *gamma, double *gStrength, int size);
void SaveAllParams(void);
void setNeuronParams(int id, unsigned long side, unsigned long seg, double a, double s, double sE, double sI, double bE, double bI, double Idc);
double ReturnCurrentParams(long id, long side, long seg);
void CreateParamFile (void);

@end
