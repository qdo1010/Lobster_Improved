//
//  ParameterSetter.m
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

//@interface ParameterSetter ()

//@end

@implementation ParameterSetter

//synthesize buttons on window
@synthesize synapseSelector;
@synthesize synapseTypeSelector;
@synthesize SideSelector;
@synthesize SegSelector;
@synthesize xRPTextBox;
@synthesize GammaTextBox;
@synthesize gStrengthTextBox;

@synthesize synapseID;
@synthesize gStrength;
@synthesize gamma;
@synthesize seg;
@synthesize side;
@synthesize xrp;

@synthesize cellNameTextBox;
@synthesize neuronTypeSelector;
@synthesize alphaTextBox;
@synthesize sigmaTextBox;
@synthesize sigmaETextBox;
@synthesize sigmaITextBox;
@synthesize betaETextBox;
@synthesize betaITextBox;
@synthesize IdcTextBox;


@synthesize alpha;
@synthesize sigma;
@synthesize sigmaE;
@synthesize sigmaI;
@synthesize betaE;
@synthesize betaI;
@synthesize Idc;
@synthesize cellID;
//@synthesize side;
//@synthesize seg;
@synthesize settingUpParams; //load param value into array
@synthesize setParamsButton;
@synthesize firstTimeChangeParams;
@synthesize neuronID;
@synthesize neuronChosen;


- (NSString *)windowNibName {
    return @"parameterSetter"; // this name tells AppKit which nib file to use
}

- (id)init
{
    self = [super initWithWindowNibName:@"parameterSetter"];
    NSLog(@"Parameter Setter Inited");
    return self;
}

- (void)windowDidLoad {
    NSLog(@"Executing Parameter Setter WindowDidLoad");
    
    [super windowDidLoad];
    [synapseSelector removeAllItems];
    [synapseTypeSelector removeAllItems];
    [SideSelector removeAllItems];
    [SegSelector removeAllItems];
    
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

    [SideSelector addItemWithTitle:@"0"];
    [SideSelector addItemWithTitle:@"1"];
    
    [SegSelector addItemWithTitle:@"0"];
    [SegSelector addItemWithTitle:@"1"];
    [SegSelector addItemWithTitle:@"2"];
    [SegSelector addItemWithTitle:@"3"];
    
    
    //pFastExc, pFastInh, pSlowExc, pSlowInh,
    [synapseTypeSelector addItemWithTitle:@"pFastExc"];
    [synapseTypeSelector addItemWithTitle:@"pFastInh"];
    [synapseTypeSelector addItemWithTitle:@"pSlowExc"];
    [synapseTypeSelector addItemWithTitle:@"pSlowInh"];
    [synapseTypeSelector addItemWithTitle:@"Not Defined"];
    
    [neuronID removeAllItems];
    [neuronID addItemWithTitle:@"cellElevator"];
    [neuronID addItemWithTitle:@"cellSwing"];
    [neuronID addItemWithTitle:@"cellDepressor"];
    [neuronID addItemWithTitle:@"cellStance"];
    [neuronID addItemWithTitle:@"cellCoord"];
    [neuronID addItemWithTitle:@"cellProtractor"];
    [neuronID addItemWithTitle:@"cellRetractor"];
    [neuronID addItemWithTitle:@"cellExtensor"];
    [neuronID addItemWithTitle:@"cellFlexor"];
    [neuronID addItemWithTitle:@"cellF"];
    [neuronID addItemWithTitle:@"cellB"];
    [neuronID addItemWithTitle:@"cellLL"];
    [neuronID addItemWithTitle:@"cellLT"];
    [neuronID addItemWithTitle:@"cellModCom"];
    [neuronID addItemWithTitle:@"cellH"];
    [neuronID addItemWithTitle:@"cellPcn"];
    
    

    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    NSLog(@"Parameter Setter window loaded");
    
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
    NSLog(@"Executing awakeFromNib in Parameter Setter");
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    unsigned long numCell = [[[appDelegate traceSelector] traceArraytobeSent] count];
//   [[appDelegate analysisWindowController] setOscilloscopeController:self];
 
cellID = malloc(numCell*sizeof(int));
side =malloc(numCell*sizeof(int));
seg = malloc(numCell*sizeof(int));
    //those are the array of params for each cell
alpha = malloc(numCell*sizeof(double));
sigma = malloc(numCell*sizeof(double));
sigmaE = malloc(numCell*sizeof(double));
sigmaI = malloc(numCell*sizeof(double));
betaE = malloc(numCell*sizeof(double));
betaI = malloc(numCell*sizeof(double));
Idc = malloc(numCell*sizeof(double));
    side =malloc(numCell*sizeof(int));
    seg = malloc(numCell*sizeof(int));
    settingUpParams = 1; //first time setting up, will be off forever
    
    for (int i = 0; i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i ++){
        
            //get all Cell Name and convert those names to ID
        NSString*cellName = [[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:i];
        NSMutableArray* chosenCellArray;
        chosenCellArray = [self convertNameToId:cellName];
            //        cellID[i] = [[chosenCellArray objectAtIndex:0] intValue];
        side[i] = [[chosenCellArray objectAtIndex:1] intValue];
        seg[i] = [[chosenCellArray objectAtIndex:2] intValue];
//        NSLog(@"%@", cellName);
            //set trace name first//
        [cellNameTextBox setStringValue:[[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:0]];
        
        [alphaTextBox setDelegate:self];
        [sigmaTextBox setDelegate:self];
        [sigmaETextBox setDelegate:self];
        [sigmaITextBox setDelegate:self];
        [betaETextBox setDelegate:self];
        [betaITextBox setDelegate:self];
        [IdcTextBox setDelegate:self];
        
        [self setFirstTimeChangeParams:1]; //never change params ever again, just do it once
    }
    /*
        //init params input values
        id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];

    if (settingUpParams){
        for (int i = 0; i < numCell; i++){
                //NSLog(@"howw many %d",i);
            NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:i];
            alpha[i] = [[params objectAtIndex:0] doubleValue];
            sigma[i] = [[params objectAtIndex:1] doubleValue];
            sigmaE[i] = [[params objectAtIndex:2] doubleValue];
            sigmaI[i] = [[params objectAtIndex:3] doubleValue];
            betaE[i] = [[params objectAtIndex:4] doubleValue];
            betaI[i] = [[params objectAtIndex:5] doubleValue];
            Idc[i] = [[params objectAtIndex:6] doubleValue];
            
        }
        settingUpParams = 0;
    }
     */
        //init text box too
    /* [alphaTextBox setStringValue:[[params objectAtIndex:0] stringValue]];
     [sigmaTextBox setStringValue:[[params objectAtIndex:1] stringValue]];
     [sigmaETextBox setStringValue:[[params objectAtIndex:2] stringValue]];
     [sigmaITextBox setStringValue:[[params objectAtIndex:3] stringValue]];
     [betaETextBox setStringValue:[[params objectAtIndex:4] stringValue]];
     [betaITextBox setStringValue:[[params objectAtIndex:5] stringValue]];
     [IdcTextBox setStringValue:[[params objectAtIndex:6] stringValue]];
     */
    
        //   settingUpParams = 0; //done setting up
        // }
    
    
        // else{
    [alphaTextBox setStringValue: [NSString stringWithFormat:@"%f",alpha[neuronChosen]]];
    [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",sigma[neuronChosen]]];
    [sigmaETextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaE[neuronChosen]]];
    [sigmaITextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaI[neuronChosen]]];
    [betaETextBox setStringValue:[NSString stringWithFormat:@"%f",betaE[neuronChosen]]];
    
    [betaITextBox setStringValue:[NSString stringWithFormat:@"%f",betaI[neuronChosen]]];
    
    [IdcTextBox setStringValue:[NSString stringWithFormat:@"%f",Idc[neuronChosen]]];
    
    NSLog(@"Parameter Setter AwakeFromNIB completed");
    
}

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
            GammaTextBox.doubleValue = 0.;
            gStrengthTextBox.doubleValue = 0.;
            break;
    }
    
}
        //This will change the cell name to ID
    - (NSMutableArray*)convertNameToId: (NSString*)name{ //add Left RIGHT TO CPG TOO....
        int  c;
        int side;
        int seg;
        NSMutableArray* chosenCell = [[NSMutableArray alloc] init];
        if ([name containsString:@"Elevator"]){
            c = 0;
        }
        else if ([name containsString:@"Swing"]){
            c = 1;
        }
        else if ([name containsString:@"Depressor"]){
            c = 2;
        }
        else if ([name containsString:@"Stance"]){
            c = 3;
        }
        else if ([name containsString:@"Coord"]){
            c = 4;
        }
        else if ([name containsString:@"Protractor"]){
            c = 5;
        }
        else if ([name containsString:@"Retractor"]){
            c = 6;
        }
        else if ([name containsString:@"Extensor"]){
            c = 7;
        }
        else if ([name containsString:@"Flexor"]){
            c = 8;
        }
        else if ([name containsString:@"FL"] || [name containsString:@"FR"]){///NOOO
                                                                             ////NEED TO SEPARATE THE TWO!!!!
            c = 9;
        }
        else if ([name containsString:@"BL"] || [name containsString:@"BR"]){
                ////NEED TO SEPARATE THE TWO!!!!
            
            c = 10;
        }
        else if ([name containsString:@"LL"]){
            c = 11;
        }
        else if ([name containsString:@"LT"]){
            c = 12;
        }
        else if ([name containsString:@"Pcn"]){
            c = 13;
        }
        else if ([name containsString:@"ModCom"]){
            c = 14;
        }
        else if ([name containsString:@"HL"] ||[name containsString:@"HR"]){
                ////NEED TO SEPARATE THE TWO!!!!
            c = 15;
        }
        else{
            c = -1; //nothing
        }
        
        if ( c < 9){
            NSString *LR = [name substringFromIndex: [name length] - 2];
            NSString *code = [LR substringFromIndex: [LR length] - 1];
            
            if ([LR containsString:@"L"]){
                side = 0;
            }
            else{
                side = 1;
            }
            seg = [code intValue] - 1;//FIX
        }
        else if( c != 13){
            NSString *LR = [name substringFromIndex: [name length] - 1];
            if ([LR isEqual: @"L"]){
                side = 0;
            }
            else{
                side = 1;
            }
            seg = 0;
        }
        else{ //pcn
            side = -1; //not yet supported
            seg = -1; //not yet supported
        }
        [chosenCell addObject:[NSNumber numberWithInt:c]];
        [chosenCell addObject: [NSNumber numberWithInt: side]];
        [chosenCell addObject: [NSNumber numberWithInt: seg]];
        return chosenCell;
    }
    
        //this function control the SetParam button,
        //which will read what params are being chosen, and send the message to C code to edit those params!

- (IBAction)chooseNeuron:(id)sender {  //Formerly ChooseTraceID in OscilloscopeController
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    neuronChosen = (int) [[[self neuronID] title] integerValue];
    
//    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    
    
        //set trace ID in appDelegate
    [appDelegate setIDofCellChosen:neuronChosen];
    
        //this is the param id that contains sigma and alpha
        // NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:neuronChosen];
    
        //init all params
//    int numCell = (int) [[[appDelegate traceSelector] traceArraytobeSent] count];
    
    /* Moved to Parameter Setter
     //init params input values
     if (settingUpParams){
     for (int i = 0; i < numCell; i++){
     //NSLog(@"howw many %d",i);
     NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:i];
     alpha[i] = [[params objectAtIndex:0] doubleValue];
     sigma[i] = [[params objectAtIndex:1] doubleValue];
     sigmaE[i] = [[params objectAtIndex:2] doubleValue];
     sigmaI[i] = [[params objectAtIndex:3] doubleValue];
     betaE[i] = [[params objectAtIndex:4] doubleValue];
     betaI[i] = [[params objectAtIndex:5] doubleValue];
     Idc[i] = [[params objectAtIndex:6] doubleValue];
     
     }
     settingUpParams = 0;
     }
     //init text box too
     
     [alphaTextBox setStringValue:[[params objectAtIndex:0] stringValue]];
     [sigmaTextBox setStringValue:[[params objectAtIndex:1] stringValue]];
     [sigmaETextBox setStringValue:[[params objectAtIndex:2] stringValue]];
     [sigmaITextBox setStringValue:[[params objectAtIndex:3] stringValue]];
     [betaETextBox setStringValue:[[params objectAtIndex:4] stringValue]];
     [betaITextBox setStringValue:[[params objectAtIndex:5] stringValue]];
     [IdcTextBox setStringValue:[[params objectAtIndex:6] stringValue]];
     */
    
        //   settingUpParams = 0; //done setting up
        // }
    
    
        // else{
    //Commenting this out so I can try my version of getting the current parameters in
    //order to fill the text boxes.
    
    /*
     //WIP
    double* ParamArray = ReturnCurrentParams([neuronID indexOfSelectedItem], [SideSelector indexOfSelectedItem], [SegSelector indexOfSelectedItem]);
    
    [alphaTextBox setStringValue: [NSString stringWithFormat:@"%f",ParamArray[0]]];
    [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",ParamArray[1]]];
    [sigmaETextBox setStringValue:[NSString stringWithFormat:@"%f",ParamArray[2]]];
    [sigmaITextBox setStringValue:[NSString stringWithFormat:@"%f",ParamArray[3]]];
    [betaETextBox setStringValue:[NSString stringWithFormat:@"%f",ParamArray[4]]];
    
    [betaITextBox setStringValue:[NSString stringWithFormat:@"%f",ParamArray[5]]];
    
    [IdcTextBox setStringValue:[NSString stringWithFormat:@"%f",ParamArray[6]]];
     
     
     
    
    */
    
    
    [alphaTextBox setStringValue: [NSString stringWithFormat:@"%f",alpha[neuronChosen]]];
    [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",sigma[neuronChosen]]];
    [sigmaETextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaE[neuronChosen]]];
    [sigmaITextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaI[neuronChosen]]];
    [betaETextBox setStringValue:[NSString stringWithFormat:@"%f",betaE[neuronChosen]]];
    
    [betaITextBox setStringValue:[NSString stringWithFormat:@"%f",betaI[neuronChosen]]];
    
    [IdcTextBox setStringValue:[NSString stringWithFormat:@"%f",Idc[neuronChosen]]];
    

    
        // }
    
    
    
        // if (neuronChosen == 0 || neuronChosen == 1){
        //     [sigmaTextBox setEditable:(false)];
        // }
        // else{
        //     [sigmaTextBox setEditable:(true)];
        // }
    NSLog(@" choose this trace %d", neuronChosen);
    [cellNameTextBox setStringValue:[[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:neuronChosen]];
    
    
}

//(Spencer) Trying to create simpler Dictionary of current parameters
/*
- (void) loadParametersFromFile{
    int iSide;
    int iSeg;
    paramStruct input;
    FILE *paramFile;
    paramFile = fopen ("params.dat", "r");
    while(fread(&input, sizeof(paramStruct), 1, paramFile)){
        
    }

}
*/

    - (IBAction)setParams:(id)sender {
        AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
        
            //    NSLog(@"%f",[alphaTextBox floatValue]);
        /*  double a = [alphaTextBox doubleValue];
         double s = [sigmaTextBox doubleValue];
         double sE = [sigmaETextBox doubleValue];
         double sI = [sigmaITextBox doubleValue];
         double bE = [betaETextBox doubleValue];
         double bI = [betaITextBox doubleValue];
         double Idc = [IdcTextBox doubleValue];*/
        neuronChosen = (int)[[[self neuronID] title] integerValue];
        
        id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
        
            //set trace ID in appDelegate
            //this is the param id that contains sigma and alpha
        int numCell = (int) [[[appDelegate traceSelector] traceArraytobeSent] count];
        
        for (int i = 0; i < numCell;i++){
                //loop through all the param array and change them with the users input param!
            NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:i];
            [params replaceObjectAtIndex:0 withObject:[NSNumber numberWithDouble:alpha[i]]];
            [params replaceObjectAtIndex:1 withObject:[NSNumber numberWithDouble:sigma[i]]];
            [params replaceObjectAtIndex:2 withObject:[NSNumber numberWithDouble:sigmaE[i]]];
            [params replaceObjectAtIndex:3 withObject:[NSNumber numberWithDouble:sigmaI[i]]];
            [params replaceObjectAtIndex:4 withObject:[NSNumber numberWithDouble:betaE[i]]];
            [params replaceObjectAtIndex:5 withObject:[NSNumber numberWithDouble:betaI[i]]];
            [params replaceObjectAtIndex:6 withObject:[NSNumber numberWithDouble:Idc[i]]];
        }
        
        
            //Now send all the array to edit Param!
        editParam(cellID,side,seg,alpha,sigma,sigmaE, sigmaI, betaE, betaI,Idc,[[[appDelegate traceSelector] traceArraytobeSent] count]);
        
        /*
         if (firstTimeChangeParams == 1){
         [appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
         [self setFirstTimeChangeParams:0]; //now it's set
         }else{
         setParamsButton.enabled = false;
         // int currentIndex = 0;
         //  while (currentIndex != 9999){ //should be tmax, not hardcoded btw
         //     currentIndex = 0;
         //     currentIndex = checkMainLoopIndex();
         // } //xmain should end by now
         //sleep(0.1);
         
         ///Stabilize by making it run on foreground instead of being a background process, so it has to finish
         
         [appDelegate performSelector:@selector(createWaveForm) withObject:nil];
         //[appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
         setParamsButton.enabled = true;
         }*/
    }


//make a library to map where each synapse should be
-(int)synapseMapping: (int)synapseID{
    //input synapseID
    //get the mapping
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
        case 50://pExcLLModCom
            [synapseTypeSelector selectItemAtIndex:2];
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
    NSLog(@"executing SetSynapseParams");
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
    side[0] = [SideSelector indexOfSelectedItem];
    seg[0] = [SegSelector indexOfSelectedItem];
    
    gamma[0] = [GammaTextBox doubleValue];
    xrp[0] = [xRPTextBox doubleValue];
    gStrength[0] = [gStrengthTextBox doubleValue];
    
    //call the func to edit synapse
    editSynapseParam(synapseID, side, seg, xrp, gamma, gStrength, size);
}

- (IBAction)saveSynapseParams:(id)sender {
    SaveAllParams();
}

- (IBAction)resetParamsToDefault:(id)sender {
    CreateParamFile();
}
- (IBAction)setNeuronParams:(id)sender {
    
    //    NSLog(@"%f",[alphaTextBox floatValue]);
      double a = [alphaTextBox doubleValue];
     double s = [sigmaTextBox doubleValue];
     double sE = [sigmaETextBox doubleValue];
     double sI = [sigmaITextBox doubleValue];
     double bE = [betaETextBox doubleValue];
     double bI = [betaITextBox doubleValue];
     double Idc = [IdcTextBox doubleValue];
     long side = [SideSelector indexOfSelectedItem];
     long seg = [SegSelector indexOfSelectedItem];
     long neuronToChange = [neuronID indexOfSelectedItem];
    
    
    setNeuronParams(neuronToChange, side, seg, a, s, sE, sI, bE, bI, Idc);
    SaveAllParams();
     }

@end
