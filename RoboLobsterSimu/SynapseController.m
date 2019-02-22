//
//  SynapseController.m
//  RoboLobsterSimu
//
//  Created by Quan Do on 1/31/19.
//  Copyright © 2019 Quan Do. All rights reserved.
//

/*
pFastExc, pFastInh, pSlowExc, pSlowInh,                                                              // SynapseTypes
*/

#import "SynapseController.h"

@interface SynapseController ()

@end

@implementation SynapseController

//synthesize buttons on window
@synthesize synapseSelector;
@synthesize synapseTypeSelector;
@synthesize SideSelector;
@synthesize SegSelector;
@synthesize xRPTextBox;
@synthesize GammaTextBox;
@synthesize gStrengthTextBox;

- (NSString *)windowNibName {
    return @"SynapseController"; // this name tells AppKit which nib file to use
}

- (id)init
{
    self = [super initWithWindowNibName:@"SynapseController"];
    NSLog(@"Hello this is synapse controller");
    return self;
}

- (void)windowDidLoad {
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


    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

- (IBAction)setSynapseParams:(id)sender {
    
    
    
}

- (IBAction)setDefaultSynapseParams:(id)sender {
}
@end
