//
//  SynapseController.m
//  RoboLobsterSimu
//
//  Created by Quan Do on 1/31/19.
//  Copyright © 2019 Quan Do. All rights reserved.
//
#import "AppDelegate.h"
/*
 pFastExc, pFastInh, pSlowExc, pSlowInh,                                                              // SynapseTypes
 */

//TODO: add feedback to view the synapse params value
//synapse struct

#import "ParameterSetter.h"

@interface ParameterSetter ()

@end

@implementation ParameterSetter

//synthesize buttons on window
@synthesize synapseSelector;
@synthesize synapseTypeSelector;
@synthesize neuronSideSelector;
@synthesize neuronSegSelector;
@synthesize synapseSideSelector;
@synthesize synapseSegSelector;
@synthesize xRPTextBox;
@synthesize GammaTextBox;
@synthesize gStrengthTextBox;
@synthesize neuronSelector;
@synthesize CellName;
@synthesize alphaTextBox;
@synthesize sigmaTextBox;
@synthesize sigmaETextBox;
@synthesize sigmaITextBox;
@synthesize betaETextBox;
@synthesize betaITextBox;
@synthesize IdcTextBox;

@synthesize synapseID;
@synthesize gStrength;
@synthesize gamma;
@synthesize seg;
@synthesize side;
@synthesize xrp;

- (NSString *)windowNibName {
    return @"ParameterSetter"; // this name tells AppKit which nib file to use
}

- (id)init
{
    self = [super initWithWindowNibName:@"ParameterSetter"];
    NSLog(@"Hello this is parameter setter");
    return self;
}

- (void)windowDidLoad {
    [super windowDidLoad];
    [synapseSelector removeAllItems];
    [synapseTypeSelector removeAllItems];
    [neuronSideSelector removeAllItems];
    [neuronSegSelector removeAllItems];
    [neuronSelector removeAllItems];
    [synapseSideSelector removeAllItems];
    [synapseSegSelector removeAllItems];
    
    //  pInhSegCoordEle[mmSide][mmSeg],                                                                      // Segmental Inhibition from an Coordinating Neuron to an Elevator
    //  pExcSegContEleCoord[mmSide][mmSeg],                                                                  // Intersegmental Exitation from and elevator to a contralatersl coordinating neuron
    // pExcIntRosEleCoord[mmSide][mmSeg],pExcIntCaudEleCoord[mmSide][mmSeg],                                // Intersegmental Exitation from and elevator to a rostral or caudal ipsilateral coordinating neuron
    [synapseSelector addItemWithTitle:@"pInhSegCoordEle"];
    [synapseSelector addItemWithTitle:@"pExcSegContEleCoord"];
    [synapseSelector addItemWithTitle:@"pExcIntRosEleCoord"];
    [synapseSelector addItemWithTitle:@"pExcIntCaudEleCoord"];
    
    // pInhIntFSwing[mmSide], pInhIntFStance[mmSide], pInhIntBSwing[mmSide],pInhIntBStance[mmSide],         // Presynaptic Inhibition from a command to bifunctional interneuron synapse
    // pInhIntLLSwing[mmSide],pInhIntLLStance[mmSide],pInhIntLTSwing[mmSide],pInhIntLTStance[mmSide],       // Presynaptic Inhibition from a command to bifunctional interneuron synapse
    [synapseSelector addItemWithTitle:@"pInhIntFSwing"];
    [synapseSelector addItemWithTitle:@"pInhIntFStance"];
    
    [synapseSelector addItemWithTitle:@"pInhIntBSwing"];
    [synapseSelector addItemWithTitle:@"pInhIntBStance"];
    
    [synapseSelector addItemWithTitle:@"pInhIntLLSwing"];
    [synapseSelector addItemWithTitle:@"pInhIntLLStance"];
    
    [synapseSelector addItemWithTitle:@"pInhIntLTSwing"];
    [synapseSelector addItemWithTitle:@"pInhIntLTStance"];
    
    // pExcIntRosEleCoord[mmSide][mmSeg],  pExcIntRCaudEleCoord[mmSide][mmSeg],                             //Excitatory Synapses from Elevator to adjacent coordinating neurons
    // pExcSegEleContraLat[mmSide][mmSeg],                                                                    //Excitatory Synapses from Elevator to contralateral coordinating neurons
    // pInhSegEleDep[mmSide][mmSeg],pInhSegEleStance[mmSide][mmSeg],pInhSegStanceSwing[mmSide][mmSeg],      //Inhibitory Synapses from Elevator to Depressor and Swing Interneurons
    [synapseSelector addItemWithTitle:@"pExcIntRosEleCoord"];
    [synapseSelector addItemWithTitle:@"pExcIntRCaudEleCoord"];
    [synapseSelector addItemWithTitle:@"pExcSegEleContraLat"];
    [synapseSelector addItemWithTitle:@"pInhSegEleDep"];
    [synapseSelector addItemWithTitle:@"pInhSegEleStance"];
    [synapseSelector addItemWithTitle:@"pInhSegStanceSwing"];
    
    // pExcSegStanceProt[mmSide][mmSeg],pExcSegStanceRet[mmSide][mmSeg],pExcSegStanceExt[mmSide][mmSeg],pExcSegStanceFlx[mmSide][mmSeg],   //Excitatory Synapses from Stance to Bifuncitonal Motor Neurons
    // pExcSegSwingProt[mmSide][mmSeg], pExcSegSwingRet[mmSide][mmSeg], pExcSegSwingExt[mmSide][mmSeg],pExcSegSwingFlx[mmSide][mmSeg],     //Excitatory Synapses from Swing to Biunctional Neurons
    
    [synapseSelector addItemWithTitle:@"pExcSegStanceProt"];
    [synapseSelector addItemWithTitle:@"pExcSegStanceRet"];
    [synapseSelector addItemWithTitle:@"pExcSegStanceExt"];
    [synapseSelector addItemWithTitle:@"pExcSegStanceFlx"];
    [synapseSelector addItemWithTitle:@"pExcSegSwingProt"];
    [synapseSelector addItemWithTitle:@"pExcSegSwingRet"];
    [synapseSelector addItemWithTitle:@"pExcSegSwingExt"];
    [synapseSelector addItemWithTitle:@"pExcSegSwingFlx"];
    
    //  pExcHLYL[mmSide][mmSeg],pExcHLRL[mmSide][mmSeg],pExcYLFR[mmSide][mmSeg],pExcRLFR[mmSide][mmSeg],
    //  pExcHRYR[mmSide][mmSeg],pExcHRRR[mmSide][mmSeg],pExcYRFL[mmSide][mmSeg],pExcRRFL[mmSide][mmSeg],
    //  pExcRSLeft[mmSide][mmSeg],pExcRSRight[mmSide][mmSeg],
    //  pExcSegPcnDep[mmSide][mmSeg],                                                                        //Synapse between pitch Command and segmental depressors
    //  pExcC[mmSide][mmSeg], pInhF[mmSide][mmSeg], pExcB[mmSide][mmSeg],
    
    [synapseSelector addItemWithTitle:@"pExcHLYL"];
    [synapseSelector addItemWithTitle:@"pExcHLRL"];
    [synapseSelector addItemWithTitle:@"pExcYLFR"];
    [synapseSelector addItemWithTitle:@"pExcRLFR"];
    [synapseSelector addItemWithTitle:@"pExcHRYR"];
    [synapseSelector addItemWithTitle:@"pExcHRRR"];
    [synapseSelector addItemWithTitle:@"pExcYRFL"];
    [synapseSelector addItemWithTitle:@"pExcRRFL"];
    [synapseSelector addItemWithTitle:@"pExcRSLeft"];
    [synapseSelector addItemWithTitle:@"pExcRSRight"];
    [synapseSelector addItemWithTitle:@"pExcSegPcnDep"];
    [synapseSelector addItemWithTitle:@"pExcC"];
    [synapseSelector addItemWithTitle:@"pInhF"];
    [synapseSelector addItemWithTitle:@"pExcB"];
    
    // pExcModComEle[mmSide][mmSeg],pExcModComDep[mmSide][mmSeg], pExcModComSwing[mmSide][mmSeg], pExcModComStance[mmSide][mmSeg],//Excitatory synapses from modulatory Commands to CPG Neurons
    // pExcForRet[mmSide][mmSeg], pExcBackProt[mmSide][mmSeg], pExcLLFlx[mmSide][mmSeg], pExcLTExt[mmSide][mmSeg],     //Excitatory Synapses from Directional Commands to propulsive synergies
    // pExcForModCom[mmSide], pExcBackModCom[mmSide], pExcLLModCom[mmSide], pExcLTModCom[mmSide];     //Excitatory Synapses from Directional Commands to ModCom
    [synapseSelector addItemWithTitle:@"pExcModComEle"];
    [synapseSelector addItemWithTitle:@"pExcModComDep"];
    [synapseSelector addItemWithTitle:@"pExcModComSwing"];
    [synapseSelector addItemWithTitle:@"pExcModComStance"];
    [synapseSelector addItemWithTitle:@"pExcForRet"];
    [synapseSelector addItemWithTitle:@"pExcBackProt"];
    [synapseSelector addItemWithTitle:@"pExcLLFlx"];
    [synapseSelector addItemWithTitle:@"pExcLTExt"];
    [synapseSelector addItemWithTitle:@"pExcForModCom"];
    [synapseSelector addItemWithTitle:@"pExcBackModCom"];
    [synapseSelector addItemWithTitle:@"pExcLLModCom"];
    [synapseSelector addItemWithTitle:@"pExcLTModCom"];
    
    [neuronSideSelector addItemWithTitle:@"0"];
    [neuronSideSelector addItemWithTitle:@"1"];
    
    [neuronSegSelector addItemWithTitle:@"0"];
    [neuronSegSelector addItemWithTitle:@"1"];
    [neuronSegSelector addItemWithTitle:@"2"];
    [neuronSegSelector addItemWithTitle:@"3"];
    
    [neuronSideSelector addItemWithTitle:@"0"];
    [neuronSideSelector addItemWithTitle:@"1"];
    
    [neuronSegSelector addItemWithTitle:@"0"];
    [neuronSegSelector addItemWithTitle:@"1"];
    [neuronSegSelector addItemWithTitle:@"2"];
    [neuronSegSelector addItemWithTitle:@"3"];
    
    [synapseSideSelector addItemWithTitle:@"0"];
    [synapseSideSelector addItemWithTitle:@"1"];
    
    [synapseSegSelector addItemWithTitle:@"0"];
    [synapseSegSelector addItemWithTitle:@"1"];
    [synapseSegSelector addItemWithTitle:@"2"];
    [synapseSegSelector addItemWithTitle:@"3"];
    
    //pFastExc, pFastInh, pSlowExc, pSlowInh,
    [synapseTypeSelector addItemWithTitle:@"pFastExc"];
    [synapseTypeSelector addItemWithTitle:@"pFastInh"];
    [synapseTypeSelector addItemWithTitle:@"pSlowExc"];
    [synapseTypeSelector addItemWithTitle:@"pSlowInh"];
    [synapseTypeSelector addItemWithTitle:@"Not Defined"];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    [neuronSelector addItemWithTitle:@"cellElevator"];
    [neuronSelector addItemWithTitle:@"cellSwing"];
    [neuronSelector addItemWithTitle:@"cellDepressor"];
    [neuronSelector addItemWithTitle:@"cellStance"];
    [neuronSelector addItemWithTitle:@"cellCoord"];
    [neuronSelector addItemWithTitle:@"cellProtractor"];
    [neuronSelector addItemWithTitle:@"cellRetractor"];
    [neuronSelector addItemWithTitle:@"cellExtensor"];
    [neuronSelector addItemWithTitle:@"cellFlexor"];
    [neuronSelector addItemWithTitle:@"cellF"];
    [neuronSelector addItemWithTitle:@"cellB"];
    [neuronSelector addItemWithTitle:@"cellLL"];
    [neuronSelector addItemWithTitle:@"cellLT"];
    [neuronSelector addItemWithTitle:@"cellModCom"];
    [neuronSelector addItemWithTitle:@"cellH"];
    [neuronSelector addItemWithTitle:@"cellPcn"];
    
}

- (id)initWithDefaultWindowNib
{
    if (!self){
        self = [super initWithWindowNibName:@"ParameterSetter"];
        if (self)
        {
            NSLog(@"Executing InitWithDefaultWIndowNib in Parameter Setter");
        }
        else
        {
            NSLog(@"Warning! Could not load ParameterSetter nib.\n");
        }
    }
    return self;
}

- (void) awakeFromNib {
    [alphaTextBox setDelegate:self];
    [sigmaTextBox setDelegate:self];
    [sigmaETextBox setDelegate:self];
    [sigmaITextBox setDelegate:self];
    [betaETextBox setDelegate:self];
    [betaITextBox setDelegate:self];
    [IdcTextBox setDelegate:self];
    [xRPTextBox setDelegate:self];
    [GammaTextBox setDelegate:self];
    [gStrengthTextBox setDelegate:self];
    
    long side = [neuronSideSelector indexOfSelectedItem];
    long seg = [neuronSegSelector indexOfSelectedItem];
    long neuronToChange = [neuronSelector indexOfSelectedItem];
    long synapseSelected = [synapseTypeSelector indexOfSelectedItem];
    
    [xRPTextBox setStringValue: [NSString stringWithFormat:@"%f",ReturnCurrentSynapseParams( synapseSelected, side, seg, 0)]];
    [GammaTextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentSynapseParams( synapseSelected, side, seg, 1)]];
    [gStrengthTextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentSynapseParams( synapseSelected, side, seg, 2)]];
    
    //Filling neuron text boxes
    [alphaTextBox setStringValue: [NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 0)]];
    [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 1)]];
    [sigmaETextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 2)]];
    [sigmaITextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 3)]];
    [betaETextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 4)]];
    [betaITextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 5)]];
    [IdcTextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 6)]];
    
    NSLog(@"Parameter Setter AwakeFromNIB completed");
    
}

- (IBAction)UpdateTextBoxes:(id)sender {
    long side = [neuronSideSelector indexOfSelectedItem];
    long seg = [neuronSegSelector indexOfSelectedItem];
    long neuronToChange = [neuronSelector indexOfSelectedItem];
    long synapseSelected = [synapseTypeSelector indexOfSelectedItem];
    
    [xRPTextBox setStringValue: [NSString stringWithFormat:@"%f",ReturnCurrentSynapseParams( synapseSelected, side, seg, 0)]];
    [GammaTextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentSynapseParams( synapseSelected, side, seg, 1)]];
    [gStrengthTextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentSynapseParams( synapseSelected, side, seg, 2)]];
    
    //Filling neuron text boxes
    [alphaTextBox setStringValue: [NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 0)]];
    [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 1)]];
    [sigmaETextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 2)]];
    [sigmaITextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 3)]];
    [betaETextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 4)]];
    [betaITextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 5)]];
    [IdcTextBox setStringValue:[NSString stringWithFormat:@"%f",ReturnCurrentNeuronParams( neuronToChange, side, seg, 6)]];
}


//Re-Written Code for Parameter Setter
- (IBAction)ResetParameters:(id)sender {
    CreateParamFile();
    
}

- (IBAction)SetNeuronParameters:(id)sender {
    double a = [alphaTextBox doubleValue];
    double s = [sigmaTextBox doubleValue];
    double sE = [sigmaETextBox doubleValue];
    double sI = [sigmaITextBox doubleValue];
    double bE = [betaETextBox doubleValue];
    double bI = [betaITextBox doubleValue];
    double Idc = [IdcTextBox doubleValue];
    long side = [neuronSideSelector indexOfSelectedItem];
    long seg = [neuronSegSelector indexOfSelectedItem];
    long neuronNumber = [neuronSelector indexOfSelectedItem];
    
    
    setNeuronParams(neuronNumber, side, seg, a, s, sE, sI, bE, bI, Idc);
    SaveAllParams();
}

- (IBAction)SetSynapseParameters:(id)sender {
    double xRp = [xRPTextBox doubleValue];
    double Gamma = [GammaTextBox doubleValue];
    double gStrength = [gStrengthTextBox doubleValue];
    long side = [synapseSideSelector indexOfSelectedItem];
    long seg = [synapseSegSelector indexOfSelectedItem];
    long synapseNumber = [synapseSelector indexOfSelectedItem];
    setSynapseParams(synapseNumber, side, seg, xRp, Gamma, gStrength);
    SaveAllParams();
    
}








/*
//Holdover or Reference code from SynapseController
//this will set the parameter
- (IBAction)ConfigureDefaultSynapseParams:(id)sender {
    switch ([synapseTypeSelector indexOfSelectedItem]) {
        case 0: //fastExc
            xRPTextBox.doubleValue = -0.0;
            GammaTextBox.doubleValue = 0.9;
            gStrengthTextBox.doubleValue = 0.1;
            break;
        case 1: //FastInh
            xRPTextBox.doubleValue = -2.2;
            GammaTextBox.doubleValue = 0.9;
            gStrengthTextBox.doubleValue = 1;
            break;
        case 2: //SlowExc
            xRPTextBox.doubleValue = 2.2;
            GammaTextBox.doubleValue = 0.995;
            gStrengthTextBox.doubleValue = 1;
            break;
        case 3: //SlowInh
            xRPTextBox.doubleValue = -2.2;
            GammaTextBox.doubleValue = 0.995;
            gStrengthTextBox.doubleValue = 1;
            break;
        default:
            xRPTextBox.doubleValue = 0.;
            GammaTextBox.doubleValue = 0.;            gStrengthTextBox.doubleValue = 0.;
            break;
    }
    
}

//make a library to map where each synapse should be
-(int)synapseMapping: (int)synapseID{
    //input synapseID    //get the mapping
    return 0;
}

- (IBAction)updateSynapseParams:(id)sender {
    //0 = pFastExc
    //1 = pFastInh
    //2 = pSlowExc
    //3 = pSlowInh
    switch ([synapseSelector indexOfSelectedItem]) {
        case 0: //pInhSegCoordEle //not defined
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 1: //pExcSegContEleCoord //not defined
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 2: //pExcIntRosEleCoord //not defined
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 3://pExcIntCaudEleCoord //not defined
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 4://pInhIntFSwing
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 5://pInhIntFStance
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 6://pInhIntBSwing
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 7://pInhIntBStance
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 8://pInhIntLLSwing
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 9://pInhIntLLStance
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 10://pInhIntLTSwing
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 11://pInhIntLTStance
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 12://pExcIntRosEleCoord
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 13://pExcIntRCaudEleCoord
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 14://pExcSegEleContraLat
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 15://pInhSegEleDep
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 16://pInhSegEleStance
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 17://pInhSegStanceSwing
            [synapseTypeSelector selectItemAtIndex:3];
            break;
        case 18://pExcSegStanceProt
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 19://pExcSegStanceRet
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 20://pExcSegStanceExt
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 21://pExcSegStanceFlx
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 22://pExcSegSwingProt
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 23://pExcSegSwingRet
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 24://pExcSegSwingExt
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 25://pExcSegSwingFlx
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 26://pExcHLYL
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 27://pExcHLRL
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 28://pExcYLFR
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 29://pExcRLFR
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 30://pExcHRYR
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 31://pExcHRRR
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 32://pExcYRFL
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 33://pExcRRFL
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 34://pExcRSLeft
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 35://pExcRSRight
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 36://pExcSegPcnDep
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 37://pExcC
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 38://pInhF
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 39://pExcB
            [synapseTypeSelector selectItemAtIndex:4];
            break;
        case 40://pExcModComEle
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 41://pExcModComDep
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 42://pExcModComSwing
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 43://pExcModComStance
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 44://pExcForRet
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 45://pExcBackProt
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 46://pExcLLFlx
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 47://pExcLTExt
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 48://pExcForModCom
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 49://pExcBackModCom
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 50://pExcLLModCom            [synapseTypeSelector selectItemAtIndex:2];
            break;
        case 51://pExcLTModCom
            [synapseTypeSelector selectItemAtIndex:2];
            break;
        default:
            [synapseTypeSelector selectItemAtIndex:4];
            break;
    }
    [self ConfigureDefaultSynapseParams:synapseTypeSelector];
}

- (IBAction)setSynapseParams:(id)sender {
    NSLog(@"synapse edit");
    int size = 1; //default
    int numCell = 1; //default
    
    synapseID = malloc(numCell*sizeof(int));
    side =malloc(numCell*sizeof(int));
    seg = malloc(numCell*sizeof(int));
    //those are the array of params for each cell
    gamma = malloc(numCell*sizeof(double));
    xrp = malloc(numCell*sizeof(double));
    gStrength = malloc(numCell*sizeof(double));
    
    synapseID[0] = [synapseSelector indexOfSelectedItem];
    side[0] = [neuronSideSelector indexOfSelectedItem];
    seg[0] = [neuronSegSelector indexOfSelectedItem];
    
    gamma[0] = [GammaTextBox doubleValue];
    xrp[0] = [xRPTextBox doubleValue];
    gStrength[0] = [gStrengthTextBox doubleValue];
    
    //call the func to edit synapse
    editSynapseParam(synapseID, side, seg, xrp, gamma, gStrength, size);
}
*/
/*
- (IBAction)saveSynapseParams:(id)sender {
    saveParamsToFile(1);
}
*/
@end
