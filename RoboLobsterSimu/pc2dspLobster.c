//---------------------------------------------------------------------------
//##################################################################################
//   Map-Based model of lobster CPG network
//              of ambulatory CPG
//                 ISL, NEU and Ariel inc.
//###################################################################################
// April 2016 Edits
// changed function fun() to computeMAPs()
// changed function calc() to calcSynapticCurrents()
// changed t to mainLoopIndex
// changed cellRS to cellModCom

/*  Meaning of Conditionals according to Mark Hunt
 The pc2dsp.c code is designed to run on either
 1) the PC
 2) the TI dsp
 If "runONdsp" is NOT defined then the other 2 defines come into play:
 1) send2dsp -- this one is used if it is desirable for the results of the PC simulation to be sent on to the dsp (sometimes it isn;mainLoopIndex -- e.g. debugging)
 2) write2files -- this one is used if it is desirable that the results of the simulation be written to a PC file
 3) drawOnOscilloscope  -- use to store data to arrays to draw on the Oscilloscope
 
 So all of the calculations are performed on the PC and the spike data may or may not be sent along to the dsp and may or may not be recorded in a PC based file.
 The function "sendSpikesToDsp()" is only used to write the spiking information from the PC to the dsp where it is then sent to the "driveSpikesToMuscles()" functions for actual output to the "muscle" driver circuits.
 If "runONdsp"   IS  defined then "send2dsp" and "write2files" will have no effect (at least shouldn't if the code is correct).  This is why they are conditional on "runONdsp" NOT begging defined.
 Since in this case the code is running directly on the dsp, the "send2dsp" function is not used and "driveSpikesToMuscles()"based is directly called.
 Re, "TEST_OUTPUTS" this is used to send a test pattern through the system.
 And "print_spikes" I am not too sure about but I believe it was used to switch on.off debug printout on the PC console.
 Revised command systems
 The pitch control commands comPcnVentral, comPcnDorsal and their currents, excPcnDorsal & excPcnVentral have been replaced by bilateral cell Pcn that hae 5 states  (pLow, rosDn, pLevel, rosUp, pHigh)
 that map on different synaptic strengths in the two segments. The roll command states (leftDown, rLevel, rightDown) superscede on pLow, pLevel and pHigh
 */

#include <stdio.h>    //Desktop
#include <stdlib.h>
#include <math.h>     //Desktop
#include "pc2dsp.h"
#include <unistd.h>
#include <time.h>
//#include "lb.h"
//void showMap(int index);
//#define putChar(chr)   uartPutch(0,chr)
//static int scount = 0;
/*
 extern int cmdTimeOut;                     // cmd timeout down counter
 extern int triggered;                      // non zero when 1st pkt received
 extern int comSegmentMask;                 // enable / disable segments
 */

//#include <std.h> //DSP start
//#include <log.h>
//#include <swi.h>
//#include <clk.h>
//#include <math.h>
//#include "volumecfg.h"
//#include "volume.h" //DSP stop
//---------------Parameters initialization-----------------------------------------------
//--------- Network Geometry (Three types of cells in each segment) ---------------------



void spikingNeuronInit(struct structSpiking *ptr) {
    ptr->mu = 0.0005;
    ptr->spike = 0;
    ptr->alpha = 4;
   // ptr->alpha = 4.0;//3.85;     //sets the type of neuron spiking (alpha < 4) or bursting (alphs > 4)
    ptr->sigma = 0;      //sets the baseline state of the neuron (quiet or spiking/ bursting)
//   ptr->sigma = 0.46;      //sets the baseline state of the neuron (quiet or spiking/ bursting)
    
    ptr->sigmaE = 1.0;     //sets the sensitivity to excitatory synaptic current
    ptr->sigmaI = 1.0;     //sets the sensitivity to inhibitory synaptic current
    ptr->sigmaDc = 1.0;    //sets the sensitivity to injected dc current
    ptr->betaE = 0.133;    //sets the transient responce to excitatory synaptic current
    ptr->betaI = 0.533;    //sets the transient responce to inhibitory synaptic current
    ptr->betaDc = 0.266;   //sets the transient responce to injected dc pulse
    ptr->Idc = 0;
    
    //--set initial state of neuron at the fixed point---
    ptr->xpp = -1 + ptr->sigma;
    ptr->xp = -1 + ptr->sigma;
    ptr->x = -1 + ptr->sigma;
    ptr->y = ptr->x - ptr->alpha/(1-ptr->x);
}  // end of Elevator structure

void burstingNeuronInit(struct structBursting *ptr) {
    ptr->mu = 0.0005;
    ptr->spike = 0;
    ptr->alpha = 5.45;     //sets the type of neuron spiking (alpha < 4) or bursting (alphs > 4)
    ptr->sigma = -0.26;      //sets the baseline state of the neuron (quiet or spiking/ bursting)
    ptr->sigmaE = 1.0;     //sets the sensitivity to excitatory synaptic current
    ptr->sigmaI = 1.0;     //sets the sensitivity to inhibitory synaptic current
    ptr->sigmaDc = 1.0;    //sets the sensitivity to injected dc current
    ptr->betaE = 0.133;    //sets the transient responce to excitatory synaptic current
    ptr->betaI = 0.533;    //sets the transient responce to inhibitory synaptic current
    ptr->betaDc = 0.266;   //sets the transient responce to injected dc pulse
    ptr->Idc = 0;
    
    //--set initial state of neuron at the fixed point---
    ptr->xpp = -1 + ptr->sigma;
    ptr->xp = -1 + ptr->sigma;
    ptr->x = -1 + ptr->sigma;
    ptr->y = ptr->x - ptr->alpha/(1-ptr->x);
}  // end of Elevator structure



void pacemakerNeuronInit(struct structEndogenousPacemaker *ptr) {
    ptr->mu = .0001;
//   ptr->alpha = 4.;
    ptr->alpha = 4.85;
    //2.3
    
    ptr->sigma = 2.3-sqrt(ptr->alpha)+0.0171159;
    
    //ptr->sigmaE = 1.0;     //sets the sensitivity to excitatory synaptic current
    //ptr->sigmaI = 1.0;     //sets the sensitivity to inhibitory synaptic current
    //ptr->sigmaDc = 1.0;    //sets the sensitivity to injected dc current
   // ptr->betaE = 1.33;    //sets the transient responce to excitatory synaptic current
    //ptr->betaI = 0.533;    //sets the transient responce to inhibitory synaptic current
   // ptr->betaDc = 0.266;   //sets the transient responce to injected dc pulse
   // ptr->Idc = 0;
    ptr->sigmaE = 1.0;     //sets the sensitivity to excitatory synaptic current
    ptr->sigmaI = 1.0;     //sets the sensitivity to inhibitory synaptic current
    ptr->sigmaDc = 1.0;    //sets the sensitivity to injected dc current
    ptr->betaE = 0.133;    //sets the transient responce to excitatory synaptic current
    ptr->betaI = 0.533;    //sets the transient responce to inhibitory synaptic current
    ptr->betaDc = 0.266;   //sets the transient responce to injected dc pulse
    ptr->Idc = 0;
    
    
    ptr->yr = -1*(2+ptr->alpha)/2;
    ptr->xr = 1-sqrt(ptr->alpha);
    ptr->alphaInit = sqrt(ptr->alpha);
    ptr->xp = -1+ptr->sigma+.01;
    ptr->xpp = -1+ptr->sigma+.01;
    if (ptr->sigma<0)
    {
        ptr->x2 = (-1+ptr->sigma)- ptr->alpha / (1-(-1+ptr->sigma));
    }
    else
        ptr->x2 = 1-2*ptr->alphaInit;
}


void pacemakerNeuronInit2(struct structEndogenousPacemaker *ptr) {
    ptr->mu = .0001;
    //   ptr->alpha = 4.;
    ptr->alpha = 4.35; //this change frequency while keeping duration
    //2.3
    
    ptr->sigma = 2.25-sqrt(ptr->alpha)+0.0171159;  //changing the constant change the duration
    
    //ptr->sigmaE = 1.0;     //sets the sensitivity to excitatory synaptic current
    //ptr->sigmaI = 1.0;     //sets the sensitivity to inhibitory synaptic current
    //ptr->sigmaDc = 1.0;    //sets the sensitivity to injected dc current
    // ptr->betaE = 1.33;    //sets the transient responce to excitatory synaptic current
    //ptr->betaI = 0.533;    //sets the transient responce to inhibitory synaptic current
    // ptr->betaDc = 0.266;   //sets the transient responce to injected dc pulse
    // ptr->Idc = 0;
    ptr->sigmaE = 1.0;     //sets the sensitivity to excitatory synaptic current
    ptr->sigmaI = 1.0;     //sets the sensitivity to inhibitory synaptic current
    ptr->sigmaDc = 1.0;    //sets the sensitivity to injected dc current
    ptr->betaE = 0.133;    //sets the transient responce to excitatory synaptic current
    ptr->betaI = 0.533;    //sets the transient responce to inhibitory synaptic current
    ptr->betaDc = 0.266;   //sets the transient responce to injected dc pulse
    ptr->Idc = 0;
    
    
    ptr->yr = -1*(2+ptr->alpha)/2;
    ptr->xr = 1-sqrt(ptr->alpha);
    ptr->alphaInit = sqrt(ptr->alpha);
    ptr->xp = -1+ptr->sigma+.01;
    ptr->xpp = -1+ptr->sigma+.01;
    if (ptr->sigma<0)
    {
        ptr->x2 = (-1+ptr->sigma)- ptr->alpha / (1-(-1+ptr->sigma));
    }
    else
        ptr->x2 = 1-2*ptr->alphaInit;
}


void calcSpikingNeuron(struct structSpiking *ptr,double cIe,double cIi) {
    ptr->betaIn = ptr->betaE * cIe + ptr->betaI * cIi + ptr->betaDc * ptr->Idc;
    ptr->sigmaIn = ptr->sigmaE * cIe + ptr->sigmaI * cIi + ptr->sigmaDc * ptr->Idc;
    if(ptr->xp <= 0.0) {
        ptr->x = ptr->alpha / (1.0 - ptr->xp) + ptr->y + ptr->betaIn;
        ptr->spike = 0;
    }
    else {
        if(ptr->xp <= ptr->alpha + ptr->y + ptr->betaIn && ptr->xpp <= 0.0) {
            ptr->x = ptr->alpha + ptr->y + ptr->betaIn;
            ptr->spike = 1;
        }
        else {
            ptr->x = -1;
            ptr->spike = 0;
        }
    }
    ptr->y = ptr->y - ptr->mu* (ptr->xp + 1.0) + ptr->mu * ptr->\
    sigma + ptr->mu * ptr->sigmaIn;
    ptr->xpp = ptr->xp;
    ptr->xp = ptr->x;
}

void calcBurstingNeuron(struct structBursting *ptr,double cIe,double cIi) {
    ptr->betaIn = ptr->betaE * cIe + ptr->betaI * cIi + ptr->betaDc * ptr->Idc;
    ptr->sigmaIn = ptr->sigmaE * cIe + ptr->sigmaI * cIi + ptr->sigmaDc * ptr->Idc;
    if(ptr->xp <= 0.0) {
        ptr->x = ptr->alpha / (1.0 - ptr->xp) + ptr->y + ptr->betaIn;
        ptr->spike = 0;
    }
    else {
        if(ptr->xp <= ptr->alpha + ptr->y + ptr->betaIn && ptr->xpp <= 0.0) {
            ptr->x = ptr->alpha + ptr->y + ptr->betaIn;
            ptr->spike = 1;
        }
        else {
            ptr->x = -1;
            ptr->spike = 0;
        }
    }
    ptr->y = ptr->y - ptr->mu* (ptr->xp + 1.0) + ptr->mu * ptr->\
    sigma + ptr->mu * ptr->sigmaIn;
    ptr->xpp = ptr->xp;
    ptr->xp = ptr->x;
}



void calcPacemakerNeuron(struct structEndogenousPacemaker *ptr,double c, double e) {
    //ptr->sigma = 0.17;
    ptr->x2= ptr->x2;
    ptr ->betaIn = ptr->betaE * c + ptr->betaI*e + ptr->betaDc*ptr->Idc; //add BetaE
    ptr ->sigmaIn = ptr ->sigmaE*c + ptr->sigmaI*e + ptr->sigmaDc*ptr->Idc;
    if(ptr->xp <= 0.0) {
        ptr->x= ptr->alpha / (1.0 - ptr->xp) + ptr->x2 + ptr->betaIn;
        ptr->spike= 0;
    }
    else {
        if(ptr->xp < ptr->alpha + ptr->x2 && ptr->xpp <= 0.0) {
            ptr->x= ptr->alpha + ptr->x2 + ptr->betaIn;
            ptr->spike= 1;
        }
        else {
            ptr->x= -1;
            ptr->spike= 0;
        }
    }
    if (ptr->xp<-1)
    {
        ptr->y2 = ptr->x2 - ptr->mu * (1+ ptr->xp) + ptr->mu * ptr->sigma + ptr->mu * ptr->sigmaIn;
    }
    else{
        if (ptr->x2 >= ptr->yr-0.001 && ptr->sigma<0.02)
        {
            ptr->y2 = ptr->x2 - ptr->mu*(0.4) + ptr->mu * ptr->sigmaIn;
        }
        else
            ptr->y2 = ptr->x2 - ptr->mu*(1+ptr->xp) + ptr->mu * ptr->sigma + ptr->mu * ptr->sigmaIn;
    }
    ptr->xpp= ptr->xp;
    ptr->xp= ptr->x;
    ptr->x2= ptr->y2;
}

/*void calcPacemakerNeuron(struct structEndogenousPacemaker *ptr,double c, double e) {
    //ptr->sigma = 0.17;
    ptr->x2= ptr->x2;
    ptr ->betaIn = ptr->betaE * c + ptr->betaI*e + ptr->betaDc*ptr->Idc; //add BetaE
    ptr ->sigmaIn = ptr ->sigmaE*c + ptr->sigmaI*e + ptr->sigmaDc*ptr->Idc;
    if(ptr->xp <= 0.0) {
        ptr->x= ptr->alpha / (1.0 - ptr->xp) + ptr->x2 + c;
        ptr->spike= 0;
    }
    else {
        if(ptr->xp < ptr->alpha + ptr->x2 && ptr->xpp <= 0.0) {
            ptr->x= ptr->alpha + ptr->x2 + c;
            ptr->spike= 1;
        }
        else {
            ptr->x= -1;
            ptr->spike= 0;
        }
    }
    if (ptr->xp<-1)
    {
        ptr->y2 = ptr->x2 - ptr->mu * (1+ ptr->xp) + ptr->mu * ptr->sigma + ptr->mu * e;
    }
    else{
        if (ptr->x2 >= ptr->yr-0.001 && ptr->sigma<0.02)
        {
            ptr->y2 = ptr->x2 - ptr->mu*(0.4) + ptr->mu * e;
        }
        else
            ptr->y2 = ptr->x2 - ptr->mu*(1+ptr->xp) + ptr->mu * ptr->sigma + ptr->mu * e;
    }
    ptr->xpp= ptr->xp;
    ptr->xp= ptr->x;
    ptr->x2= ptr->y2;
}*/

//----Elevator cell (Commisural interneurons described by the regular spiking neuron model)---


// ------ Code (structure) for synaptic connections -------------------------------------
// ---- various synaptic connections are defined using the parameters sets for ----------
// ---- {xRp - reverse potential & gamma - strength of the synapse}
struct structSynapses {
    double xRp;            //Reversal Potential
    double gamma;          //Time Constant
    double gStrength;      //Synaptic Strength
}
pFastExc, pFastInh, pSlowExc, pSlowInh,                                                              // SynapseTypes

pInhSegCoordEle[mmSide][mmSeg],                                                                      // Segmental Inhibition from an Coordinating Neuron to an Elevator
pExcSegContEleCoord[mmSide][mmSeg],                                                                  // Intersegmental Exitation from and elevator to a contralatersl coordinating neuron
pExcIntRosEleCoord[mmSide][mmSeg],pExcIntCaudEleCoord[mmSide][mmSeg],                                // Intersegmental Exitation from and elevator to a rostral or caudal ipsilateral coordinating neuron
pInhIntFSwing[mmSide], pInhIntFStance[mmSide], pInhIntBSwing[mmSide],pInhIntBStance[mmSide],         // Presynaptic Inhibition from a command to bifunctional interneuron synapse
pInhIntLLSwing[mmSide],pInhIntLLStance[mmSide],pInhIntLTSwing[mmSide],pInhIntLTStance[mmSide],       // Presynaptic Inhibition from a command to bifunctional interneuron synapse
pExcIntRosEleCoord[mmSide][mmSeg],  pExcIntRCaudEleCoord[mmSide][mmSeg],						     //Excitatory Synapses from Elevator to adjacent coordinating neurons
pExcSegEleContraLat[mmSide][mmSeg],   																 //Excitatory Synapses from Elevator to contralateral coordinating neurons
pInhSegEleDep[mmSide][mmSeg],pInhSegEleStance[mmSide][mmSeg],pInhSegStanceSwing[mmSide][mmSeg],      //Inhibitory Synapses from Elevator to Depressor and Swing Interneurons
pExcSegStanceProt[mmSide][mmSeg],pExcSegStanceRet[mmSide][mmSeg],pExcSegStanceExt[mmSide][mmSeg],pExcSegStanceFlx[mmSide][mmSeg],   //Excitatory Synapses from Stance to Bifuncitonal Motor Neurons
pExcSegSwingProt[mmSide][mmSeg], pExcSegSwingRet[mmSide][mmSeg], pExcSegSwingExt[mmSide][mmSeg],pExcSegSwingFlx[mmSide][mmSeg],     //Excitatory Synapses from Swing to Biunctional Neurons
pExcHLYL[mmSide][mmSeg],pExcHLRL[mmSide][mmSeg],pExcYLFR[mmSide][mmSeg],pExcRLFR[mmSide][mmSeg],
pExcHRYR[mmSide][mmSeg],pExcHRRR[mmSide][mmSeg],pExcYRFL[mmSide][mmSeg],pExcRRFL[mmSide][mmSeg],
pExcRSLeft[mmSide][mmSeg],pExcRSRight[mmSide][mmSeg],
pExcSegPcnDep[mmSide][mmSeg],																		//Synapse between pitch Command and segmental depressors
pExcC[mmSide][mmSeg], pInhF[mmSide][mmSeg], pExcB[mmSide][mmSeg],
pExcModComEle[mmSide][mmSeg],pExcModComDep[mmSide][mmSeg], pExcModComSwing[mmSide][mmSeg], pExcModComStance[mmSide][mmSeg],//Excitatory synapses from modulatory Commands to CPG Neurons
pExcForRet[mmSide][mmSeg], pExcBackProt[mmSide][mmSeg], pExcLLFlx[mmSide][mmSeg], pExcLTExt[mmSide][mmSeg],     //Excitatory Synapses from Directional Commands to propulsive synergies
pExcForModCom[mmSide], pExcBackModCom[mmSide], pExcLLModCom[mmSide], pExcLTModCom[mmSide];     //Excitatory Synapses from Directional Commands to ModCom

//pInhSegCinCin,pInhSegCinEin,pInhSegCinLin,pInhSegCinMot,pInhSegLinCin,pExcSegEinCin,pExcSegEinLin,pExcIntEinEin,pExcIntEinLin,pExcSegEinMot,pExcPcnDorsal,pExcPcnVentral,pExcRSLeft,pExcRSRight;

double inhF;
double excB;
double excC;
int collision;
int Bump;
int command;
int commandPrev;
int stop;
int xyz;
/*
 double excHLYL;					// Excitatory synaptic current from the 1-st layer to the 2-d layer of headingContrNetwork
 double excHLRL;                 //exc: exxcitatory
 double excYLFR;
 double excRLFR;
 double excHRYR;
 double excHRRR;
 double excYRFL;
 double excRRFL;
 double excRSLeft;					// Excitatory synaptic current from the 1-st layer to the 2-d layer of headingContrNetwork
 double excRSRight;
 */

//									----- Calculate the map-model for synaptic currents --------
void calcSynapticCurrents(double *I,
                          struct structSynapses *params,
                          double xPost,double spikes) {
    *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp);
    
   // *I = params->gamma * *I - params->gStrength * (xPost - params->xRp);

    //add chemotonic? maybe?
   ///this on not really
   // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) - params->gStrength*tanh(xPost-params->xRp);
   // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) - params->gStrength*(1/(1+exp(-xPost)));
   // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) + params->gStrength*tanh(xPost - params->xRp);

  //  *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) - params->gStrength*(1/(1+exp(xPost - params->xRp)));

     //  *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) - params->gStrength/(1+exp(-(xPost - params->xRp)));


} // end of the Synaptic structure

/*void calcModulatedCurrents(double *I, //This implements presynaptic inhibition, modParams is the moduatory neuron synapse
                           struct structSynapses *params,struct structSynapses *modParams,
                           double xPost,double spikes, double xModPost, double modSpikes) {
    double iMod;	//This is the synaptic current of the modulatory synapse
    double iMax = 1.0;  //We need a better value of this.
    double iGain;
    
    iMod = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - params->xRp);
  // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
    iGain = iMod/iMax;
    // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp) ;

    //add param gStrengh)
    
    *I = params->gamma * *I - params->gStrength *spikes * (xPost - params->xRp) - iGain *xModPost*(xPost - params->xRp);
    
    
//    calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellProtractor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);

    //  calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellSwing[iSide][iSeg].x,   cellF[iSide].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
    
} // end of the Modulated Synaptic structure
*/


void calcModulatedCurrents(double *I, //This implements presynaptic inhibition, modParams is the moduatory neuron synapse
                           struct structSynapses *params,struct structSynapses *modParams,
                           double xPost,double xModPost, double spikes, double modSpikes) {
  //  double iMod;	//This is the synaptic current of the modulatory synapse
  //  double iMax = 1.0;  //We need a better value of this.
  //  double iGain;
    
   // iMod = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
    // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
   // iGain = iMod/iMax;
    // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp) ;
    
    //add param gStrengh)
   // *I = params->gamma * *I - iGain *spikes* (xPost - params->xRp) ;
    
//play around here
    
    *I = (params->gamma + modParams->gamma) * *I - (params->gStrength + modParams->gStrength) *(spikes + modSpikes)* (xPost - params->xRp + xModPost - modParams->xRp);
   // *I = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
    //    calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellProtractor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
    
    //  calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellSwing[iSide][iSeg].x,   cellF[iSide].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
    
} // end of the Modulated Synaptic structure

//calcSynapticCurrents( &iInhSegEleDep[iSide][iSeg],    		&pInhSegEleDep[iSide][iSeg],  		cellElevator[iSide][iSeg].x,       spikesElevator[iSide][iSeg]/mmSeg);
//
/* Lamprey version with strength passed as an argument
 void calcSynapticCurrents(double *I,
 struct structSynapses *params,
 double gInh,double xPost,double spikes) {
 *I = params->gamma * *I - gInh * spikes * (xPost - params->xRp);
 } // end of the Synaptic structure8?
 */
// --------- END structures -------------------------------------------
// +++++++++++++++++++  MAIN PROGRAM +++++++++++++++++++++++++++++++++++++++++++

//----------external functions----------------------------------------------
void computeMAPs(double);    // see below
void computeLateralBias(double,double);
//    void srand(unsigned long seed);
//    double rand(void);

// #define RAND_MAX  0x7FFFU
FILE *f1, *f10, *f11, *f12, *f13, *f14, *f15, *f16, *f17, *f18,*f22, *f31, *f32;
//----------external classes (beginning of initialization)------------------
// These are now GLOBAL parameters:


//static double headExc;
static double moveExc, moveInh;
static double comBiasL, comBiasR;							//Heading control
static int comActionPrev, comPropDelay, comNN, comNNmax;
static int comInitArray[mmSeg + 2];
int spikeTime[mmSide]; //0=left,  = right formerly lSpikeTime, rSpikeTime;
int comAction, comDirection, comLeftBias, comRightBias;
int comPcnInclD, comPcnTarget, comPcnTargetD;                //Pitch Control
int LVhead;
int compin;
/*
 enum pitch {pLow, rosDn, pLevel, rosUp, pHigh};
 enum roll {leftDown, rLevel, rightDown};
 enum height {low, normal, high};
 */
#ifdef runONdsp
void initDriveSpikes(void);
void driveSpikesToMuscles(unsigned short sR2dsp,unsigned short sL2dsp,
                          unsigned short spikeTimel,unsigned short spikeTimeR[right]);
static unsigned short setSpike[] =
{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
//static short sL2dsp;
//static short sR2dsp;
static int sL2dsp;
static int sR2dsp;
static int updatedParams = 0;
static int counter = 0;
static int iteration = 0;
#endif // runONdsp
//---------------------------------------------------------------------------
#ifdef write2files
void putBits(unsigned int bits) {
    int ii;
    for(ii = 0;ii<5;ii++ ) {
        if(bits & 0x10) printf("1");
        else printf("_");
        bits = bits << 1;
    }
}
#else // not write2files
void putBits(unsigned int bits) {
    int ii;
    for(ii = 0;ii<5;ii++ ) {
        if(bits & 0x10) putStr(0,"1");
        else putStr(0,"_");
        bits = bits << 1;
    }
}
#endif // write2files
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void xmain()
{                // main for PC
    //  void lobster(int *input,int *output) {  // main for DSP
    //  unsigned long lrTime;            // low resolution time DSP
    //  unsigned long timeTemp;          // temporary timer value DSP
    //---------general parameters----------------------------------------------
    double mainLoopIndex = 0,tmax; // times - current and maximun numbers of iteration
    double R;  // External noise
    int ii,jj; // indecies (ii - defines a segment & jj - defines a cell within each segment)
    int iSide, iSeg; //Substitute iSide for jj iSeg for ii
    //  double scale;
    short sL2dsp;
    short sR2dsp;
#ifdef send2dsp
#define US unsigned short
    void sendSpikesToDsp(US left, US right, char lpw, char rpw);
    static US setSpike[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
    //        short sL2dsp;
    //        short sR2dsp;
#endif
#ifdef runONdsp
    initDriveSpikes();
#ifdef TEST_OUTPUTS
    {
        for(ii = 0;ii<400000000;ii++ )
        {
            driveSpikesToMuscles(0,0,1,1);
            driveSpikesToMuscles(0,0,1,1);
            driveSpikesToMuscles(0,0,1,1);
            driveSpikesToMuscles(0,0,1,1);
            driveSpikesToMuscles(0xFFFF,0xFFFF,2,2);
        }
    }
#endif // TEST_OUTPUTS
#endif // runONdsp
    tmax = 100000; // maximum nuber of iteretions to compute
    //    showMap(1);
    //----- Initialize cells --------
    

    //if (beginEditingParams == 1){ //if a edit flag ever been set to start edit neurons param
        
        //set multiple neuron here!
     //    setMultipleNeuronParam(globalCellName, globalSide, globalSeg, globalAlpha, globalSigma, globalSigmaE, globalSigmaI, globalBetaE, globalBetaI, globalIdc, globalSize);
        
        //this is for 1 neuron
      //  setNeuronParams(globalCellName, globalAlpha, globalSigma, globalSigmaE, globalSigmaI, globalBetaE, globalBetaI, globalIdc);

   // }
  //  else{
        //alloc some memory for the global
        globalAlpha = malloc(sizeof(double));
        globalSigma = malloc(sizeof(double));
        globalSigmaE = malloc(sizeof(double));
        globalSigmaI = malloc(sizeof(double));
        globalBetaE = malloc(sizeof(double));
        globalBetaI = malloc(sizeof(double));
        globalIdc= malloc(sizeof(double));
        globalCellName = malloc(sizeof(int));
        globalSide = malloc(sizeof(int));
        globalSeg = malloc(sizeof(int));
        for(iSide = 0;iSide < mmSide; ++iSide)
        {
            for(iSeg = 0;iSeg < mmSeg; ++iSeg)
            {
               // pacemakerNeuronInit( &cellElevator[iSide][iSeg] );
               // pacemakerNeuronInit( &cellSwing[iSide][iSeg] );
                //make it bursting for now :(
                burstingNeuronInit( &cellElevator[iSide][iSeg] );
                burstingNeuronInit( &cellSwing[iSide][iSeg] );
                
                
                pacemakerNeuronInit2(  &cellDepressor[iSide][iSeg] );
                pacemakerNeuronInit(  &cellStance[iSide][iSeg] );
                
                spikingNeuronInit(   &cellProtractor[iSide][iSeg] );
                spikingNeuronInit(   &cellRetractor[iSide][iSeg] );
                spikingNeuronInit(   &cellExtensor[iSide][iSeg] );
                spikingNeuronInit(   &cellFlexor[iSide][iSeg] );
                spikingNeuronInit(   &cellCoord[iSide][iSeg] );
                
                
                
            } //END for (iSeg = 0;iSeg < mmSeg; ++iSeg)
            spikingNeuronInit(&cellPcn[iSide][pLevel]);
            spikingNeuronInit(&cellModCom[iSide]);
            spikingNeuronInit(&cellH[iSide]);
            spikingNeuronInit(&cellF[iSide]);
            spikingNeuronInit(&cellB[iSide]);
            spikingNeuronInit(&cellLL[iSide]);
            spikingNeuronInit(&cellLT[iSide]);
            
        }
        
        
   // }
    //----Initialize comInitArray[ ]  = 0 --------
    
    for(ii = 0;ii < mmSeg + 2; ++ii)
    {
        comInitArray[ii] = 0; //Parameter block from LabView
    }
    //    showMap(2);
    comAction  = 0;
    comLeftBias = -1002;
    comRightBias = -1002;
    comActionPrev = comAction;
    comPropDelay = 40; //Propagation delay of command between the nearby segments
    comNNmax = (mmSeg + 1)*comPropDelay;
    spikeTime[left] = 0;
    spikeTime[right] = 0;
    comDirection = 1;
    comPcnInclD = 0;
    comPcnTarget = 0;		// No targer Signal -0, Taget is on -1
    comPcnTargetD = 0;
    //----------here we change some psrsmeters to introduce random variability ---------
    srand(6);
    /*
     pInhSegCoordEle     // Segmental Inhibition from an Coordinating Neuron to an Elevator
     pExcSegContEleCoord // Intersegmental Exitation from and elevator to a contralatersl coordinating neuron
     pExcIntRosEleCoord  // Intersegmental Exitation from and elevator to a rostral ipsilateral coordinating neuron
     pExcIntCaudEleCoord // Intersegmental Exitation from and elevator to a caudal ipsilateral coordinating neuron
     */
    
//•••••••••••••••••••••
    //Set up basic synapse prototypes
    //	pFastExc	Fast Excitatory Synapse
    //	pSlowExc	Slow Excitatory Synapse
    //	pFastInh	Fast Excitatory Synapse
    //	pSlowInh	Slow Excitatory Synapse
    // Set postsynaptic reversal potential  xRp = -0.0 ==> excitatory Synspse; xRp = -2.2 ==> inhibitory Synapse
    // Synaptic Strength Exc =>0.02(weak) 0.03,0.04, 0.05(strong).
    // Synaptic Time Constant 0.5(fast) 0.90,0.98, 0.99(slow)

    // initialize basic Synapse prototypestypes
  /*pFastExc.xRp = -0.0; pFastExc.gamma = 0.9;      pFastExc.gStrength = 0.05;
    pFastInh.xRp = -2.2; pFastInh.gamma = 0.9;      pFastInh.gStrength = 0.1;
    pSlowExc.xRp = -0.0; pSlowExc.gamma = 0.995;    pSlowExc.gStrength = 0.05;
    pSlowInh.xRp = -2.2; pSlowInh.gamma = 0.995;    pSlowInh.gStrength = 0.1;*/
    
    ///you changed it here
    pFastExc.xRp = -0.0; pFastExc.gamma = 0.9;      pFastExc.gStrength = 0.1;
    pFastInh.xRp = -2.2; pFastInh.gamma = 0.9;      pFastInh.gStrength = 1;
    pSlowExc.xRp = 2.2; pSlowExc.gamma = 0.995;    pSlowExc.gStrength = 1;
    pSlowInh.xRp = -2.2; pSlowInh.gamma = 0.995;    pSlowInh.gStrength = 1;
//•••••••••••••••••••••
    
    for(iSide = 0;iSide < mmSide; ++iSide)  //This loop initializes the parameters for synapses
    {
        // Presynaptic Inhibition from a command to bifunctional interneuron synapse
        pInhIntFSwing[iSide] = pSlowInh;
        pInhIntFStance[iSide] = pSlowInh;
        pInhIntBSwing[iSide] = pSlowInh;
        pInhIntBStance[iSide] = pSlowInh;
        pInhIntLLSwing[iSide] = pSlowInh;
        pInhIntLLStance[iSide] = pSlowInh;
        pInhIntLTSwing[iSide] = pSlowInh;
        pInhIntLTStance[iSide] = pSlowInh;
        //Now loop down the segments
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            // Inject some noise into the CPG neurons
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellElevator[iSide][iSeg].sigma = cellElevator[iSide][iSeg].sigma + R * 0.001 + 0.0005;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellDepressor[iSide][iSeg].sigma = cellDepressor[iSide][iSeg].sigma + R * 0.0001 - 0.0195;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellSwing[iSide][iSeg].sigma = cellSwing[iSide][iSeg].sigma + R * 0.0001 - 0.0195;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellStance[iSide][iSeg].sigma = cellStance[iSide][iSeg].sigma + R * 0.0001 - 0.0195;
            
            //------- Set the parameters for synapses {xRp and gamma} ---------
            
            //Synapse between pitch Command and segmental depressors
            pExcSegPcnDep[iSide][iSeg] = pSlowExc;
            
            // Excitory synapses between elevator synergies and ajacent coordinating neurons
            pExcIntRosEleCoord[iSide][iSeg] = pSlowExc;
            pExcIntRCaudEleCoord[iSide][iSeg] = pSlowExc;
            pExcSegEleContraLat[iSide][iSeg] = pSlowExc;
            
            //Internal inhibitory synapses of neuronal oscillator
            pInhSegEleDep[iSide][iSeg] = pSlowInh;
            pInhSegEleStance[iSide][iSeg] = pSlowInh;
            pInhSegStanceSwing[iSide][iSeg] = pSlowInh;
            
            // Excitatory Synapses from Swing/Stance interneruons to bifunctional synapses
            pExcSegStanceProt[iSide][iSeg] = pSlowExc;
            pExcSegStanceRet[iSide][iSeg] = pSlowExc;
            pExcSegStanceExt[iSide][iSeg] = pSlowExc;
            pExcSegStanceFlx[iSide][iSeg] = pSlowExc;
            pExcSegSwingProt[iSide][iSeg] = pSlowExc;
            pExcSegSwingRet[iSide][iSeg] = pSlowExc;
            pExcSegSwingExt[iSide][iSeg] = pSlowExc;
            pExcSegSwingFlx[iSide][iSeg] = pSlowExc;
            
            // Recruiting excitatory synapses from Walking commands to propulsive synergies
            pExcForRet[iSide][iSeg] = pSlowExc;
            pExcBackProt[iSide][iSeg] = pSlowExc;
            pExcLLFlx[iSide][iSeg] = pSlowExc;
            pExcLTExt[iSide][iSeg] = pSlowExc;
            
            // Recruiting excitatory synapses from Walking commands to ModCom
            pExcForModCom[iSide] = pSlowExc;
            pExcBackModCom[iSide] = pSlowExc;
            pExcLLModCom[iSide] = pSlowExc;
            pExcLTModCom[iSide] = pSlowExc;
            
            //Excitatory synapses from modulatory Commands to CPG Neurons
            pExcModComEle[iSide][iSeg] = pSlowExc;
            pExcModComDep[iSide][iSeg] = pSlowExc;
            pExcModComSwing[iSide][iSeg] = pSlowExc;
            pExcModComStance[iSide][iSeg] = pSlowExc;
            
            switch(pitch) {   //This sets up the gradients of synaptic strength between the pitch command neuron Pcn and the depresssor motor neurons for each segment
                    
                case pLow:
                    pExcSegPcnDep[iSide][iSeg].gStrength = 0.50;
                    break;
                    
                case pLevel:
                    switch (roll){
                        case leftDown:{
                            switch (iSide){
                                case left:
                                    pExcSegPcnDep[iSide][iSeg].gStrength = 0.50;
                                case right:
                                    pExcSegPcnDep[iSide][iSeg].gStrength = 0.75;
                            }
                        case rightDown:
                            switch (iSide){
                                case left:
                                    pExcSegPcnDep[iSide][iSeg].gStrength = 0.75;
                                case right:
                                    pExcSegPcnDep[iSide][iSeg].gStrength = 0.50;
                            }
                        case rLevel:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.75;
                        }
                    }
                    
                case pHigh:
                    pExcSegPcnDep[iSide][iSeg].gStrength = 0.95;
                    break;
                    
                case rosDn:{
                    switch (iSeg){
                        case 0:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.25;
                        case 1:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.50;
                        case 2:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.75;
                        case 3:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.95;
                            
                    }
                    
                case rosUp  :
                    switch (iSeg){
                        case 0:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.95;
                        case 1:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.75;
                        case 2:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.50;
                        case 3:
                            pExcSegPcnDep[iSide][iSeg].gStrength = 0.25;
                            
                    }
                    break;
                }
                    
                    
            }
        }
    }
    //----- Set synaptic currents to ZERO (initialization) -------
    /*
     inhF = 0;       excC = 0;           Bump = 0;
     command = 1;    commandPrev = 1;    stop = 0;       xyz = 0;
     excHLYL = 0;    excHLRL = 0;        excYLFR = 0;    excRLFR = 0;
     excHRYR = 0;    excHRRR = 0;        excYRFL = 0;    excRRFL = 0;
     excRSLeft = 0;  excRSRight = 0;
     
     */
    
#ifdef write2files
    //--------------open ALL output files -------------------------------------
    //    f10 = fopen("R:\\timeCin","w");
    //   f11 = fopen("R:\\timeEin","w");
    //   f12 = fopen("R:\\timeLin","w");
    //   f13 = fopen("R:\\timePcn","w"); //f13 = fopen("R:\\timeMot","w");
    //   f22 = fopen("R:\\sizes","w");
    
    //file 10 to 17 for 8 diff types
    /*  f10 = fopen("timeElev.txt","w");
     f11 = fopen("timeDep.txt","w");
     f12 = fopen("timeSwing.txt","w");
     f13 = fopen("timeStance.txt","w");
     f14 = fopen("timeProtractor.txt","w");
     f15 = fopen("timeRetractor.txt","w");
     f16 = fopen("timeExtensor.txt","w");
     f17 = fopen("timeFlexor.txt","w");
     f18 = fopen("timePcn.txt","w");
     //f13 = fopen("R:\\timeMot","w");
     */
    f22 = fopen("sizes.txt","w");
    
    //  f31 = fopen("R:\\parameters","r");
#endif
    
    //----------------CALCULATION----------------------------------------
    //  LOG_printf(&trace,"\n CALCULATION IN PROGRESS!!!: mainLoopIndex = %lf: tmax = %lf", mainLoopIndex,tmax);
#ifdef write2files
    //----Print sizes to files----------------
    fprintf(f22,"%i",mmSeg);
    fprintf(f22,"\n");
    fprintf(f22," %i",mmSeg2);
    fprintf(f22,"\n");
    fprintf(f22," %i",mmSeg2);
    fprintf(f22,"\n");
    fprintf(f22," %i",mmSeg2);
    fprintf(f22,"\n");
    fprintf(f22," %i",mmPcn2);
    fprintf(f22,"\n");
    fclose(f22);
#endif
    //  LOG_printf(&trace,"\n start simulation");
    // ---------Start iterations -------------------------------
    //  int r=3, c=4;
    //  int **arr;
    //  int count = 0,i,j;
    

    
    int ind = 0; //index to start loop of array of x
    while(1){ //run forever
    //while((int)mainLoopIndex < (int)tmax) {
        //change to mainLoopIndex <= tmax to stop forever loop
        
        //printf("%d",beginEditingParams);
        
        if (beginEditingParams == 1){ //if a edit flag ever been set to start edit neurons param
            
            //set multiple neuron here!
            setMultipleNeuronParam(globalCellName, globalSide, globalSeg, globalAlpha, globalSigma, globalSigmaE, globalSigmaI, globalBetaE, globalBetaI, globalIdc, globalSize);
            
            
        }
        
        globalLoopIndex = (int)mainLoopIndex; //this is to return for Obj C to see
        
        if ((int)mainLoopIndex == 999999999){//put a limit to rotate back.
            mainLoopIndex = 0; //reset
        }
        if (((int)mainLoopIndex) % 10000 == 0){
            //open a new stream/buffer every iteration, to delete the old files and basically creating a temp buffer stream
            //printf("open at %f\n", mainLoopIndex);
            f10 = fopen("timeElev","w");
            f11 = fopen("timeDep","w");
            f12 = fopen("timeSwing","w");
            f13 = fopen("timeStance","w");
            f14 = fopen("timeProtractor","w");
            f15 = fopen("timeRetractor","w");
            f16 = fopen("timeExtensor","w");
            f17 = fopen("timeFlexor","w");
            f18 = fopen("timePcn","w");
        }
        // while(1)        //What is this loop doing?
        //{                              // run contineously
        if(cmdTimeOut < 1)
        {                  // until cmd NOT received within
            // CMD_TIMEOUT_TIME ms
            sL2dsp = 0;           // set calm state
            sR2dsp = 0;
            for(jj = 0;jj < 100; ++jj)
            {
#ifdef send2dsp
                sendSpikesToDsp(sL2dsp,sR2dsp,0,0);
                printf("%d,%d\n",sL2dsp,sR2dsp);
#endif   // send2dsp
#ifdef runONdsp
                driveSpikesToMuscles(sR2dsp,sL2dsp,spikeTime[left],spikeTime[right]);
#endif	 //runONdsp
                triggered = 0;                       // wait for command
                cmdTimeOut = CMD_TIMEOUT_TIME;
            }
            comAction  = 0;
            comLeftBias = -1002;
            comRightBias = -1002;
        }
        mainLoopIndex++ ;
        if(comAction != comActionPrev)
        {     // New command arrives
            comNN = comNNmax;
        }
        if(comNN != 0 && comAction == 1)
        {
            if(comDirection > 0) ii = (int)((comNNmax-comNN)/(comPropDelay));
            if(comDirection < 0) ii = (int)(comNN/(comPropDelay));
            comInitArray[ii] = 1;
            comNN--;
        }
        if(comNN != 0 && comAction == 0)
        {
            if(comDirection > 0) ii = (int)((comNNmax-comNN)/(comPropDelay));
            if(comDirection < 0) ii = (int)(comNN/(comPropDelay));
            comInitArray[ii] = 0;
            comNN--;
        }
        comBiasL = comLeftBias/1000.0;		comBiasR = comRightBias/1000.0;
        moveExc = 0.4 + 1.0*(comBiasL + comBiasR);
        moveInh = -0.1 + 0.5*(comBiasL + comBiasR);
        //                comPcnVentral = - (comPcnInclD + comPcnTargetD*comPcnTarget)/100.0;
        //                comPcnDorsal = (comPcnInclD + comPcnTargetD*comPcnTarget)/100.0;
        
        //•••••••••••••• codefragment2 was here
        
        /*			int iType;  //initialize the three types of pitch neurons
         
         for(iType = 0; iType <mmSeg; ++iType){
         
         //				  cellPcn[0][iType].Idc = comPcnDorsal;
         cellPcn[1][iType].Idc = 0;
         //                cellPcn[2][iType].Idc = comPcnVentral;
         cellPcn[3][iType].Idc = 0;
         }
         //compin = compin/10;
         */
       // computeLateralBias(LVhead,compin);//(LVhead,0);//compin);
        computeMAPs(mainLoopIndex); // Calculate for one iteration of network
        comActionPrev = comAction;
        /* ••••••••••••••••••••••This is debugging code
         if(mainLoopIndex>10)        //Not clear how to change from having one output cellMot, to 6 for each leg.
         {
         #ifdef send2dsp
         sL2dsp = 0;
         sR2dsp = 0;
         sendSpikesToDsp(sL2dsp,sR2dsp,8,8);
         if(cellMot[0][1].spike == 1) sL2dsp |= setSpike[0];
         if(cellMot[0][3].spike == 1) sL2dsp |= setSpike[1];
         if(cellMot[0][5].spike == 1) sL2dsp |= setSpike[2];
         if(cellMot[0][7].spike == 1) sL2dsp |= setSpike[3];
         if(cellMot[0][9].spike == 1) sL2dsp |= setSpike[4];
         if(cellMot[mmMot2-1][1].spike == 1) sR2dsp |= setSpike[0];
         if(cellMot[mmMot2-1][3].spike == 1) sR2dsp |= setSpike[1];
         if(cellMot[mmMot2-1][5].spike == 1) sR2dsp |= setSpike[2];
         if(cellMot[mmMot2-1][7].spike == 1) sR2dsp |= setSpike[3];
         if(cellMot[mmMot2-1][9].spike == 1) sR2dsp |= setSpike[4];
         sendSpikesToDsp(sR2dsp,sL2dsp,spikeTime[left], spikeTime[right];
         //   printf("\n %04X %04X", sL2dsp, sR2dsp); // print current time to the screen
         #endif
         ••••••••••••••••••••^This is debugging code  */
        
#ifdef runONdsp    // This code turns off output to particular segments.
        sL2dsp = 0;
        sR2dsp = 0;
        /*                        if(comSegmentMask & 1)
         {
         if(cellMot[0][1].spike == 1) sL2dsp |= setSpike[0];
         if(cellMot[mmMot2-1][1].spike == 1) sR2dsp |= setSpike[0];
         }
         if(comSegmentMask & 2)
         {
         if(cellMot[0][3].spike == 1) sL2dsp |= setSpike[1];
         if(cellMot[mmMot2-1][3].spike == 1) sR2dsp |= setSpike[1];
         }
         if(comSegmentMask & 4)
         {
         if(cellMot[0][5].spike == 1) sL2dsp |= setSpike[2];
         if(cellMot[mmMot2-1][5].spike == 1) sR2dsp |= setSpike[2];
         }
         if(comSegmentMask & 8)
         {
         if(cellMot[0][7].spike == 1) sL2dsp |= setSpike[3];
         if(cellMot[mmMot2-1][7].spike == 1) sR2dsp |= setSpike[3];
         }
         if(comSegmentMask & 16)
         {
         if(cellMot[0][9].spike == 1) sL2dsp |= setSpike[4];
         if(cellMot[mmMot2-1][9].spike == 1) sR2dsp |= setSpike[4];
         }
         if(comSegmentMask & 32)
         {
         if(cellPcn[1][pLevel].spike == 1) sL2dsp |= setSpike[5];
         if(cellPcn[3][pLevel].spike == 1) sR2dsp |= setSpike[5];
         }
         */                        //#define print_spikes
#ifdef print_spikes
        putStr(0,"\n ");
        putBits(sR2dsp);
        putStr(0,"  ");
        putBits(sL2dsp);
#endif
        
        if(counter >= 2000)
        {
            putInt(0,comSegmentMask);       putChar(' ');
            putInt(0,comAction);            putChar(' ');
            putInt(0,comDirection);         putChar(' ');
            putInt(0,comLeftBias);          putChar(' ');
            putInt(0,comRightBias);         putChar(' ');
            putInt(0,spikeTime[left]);      putChar(' ');
            putInt(0,spikeTime[right]);     putChar(' ');
            putInt(0,comPcnInclD);          putChar(' ');
            putInt(0,comPcnTarget);         putChar(' ');
            putInt(0,comPcnTargetD);        putChar(' ');
            putInt(0,iteration);            putChar('\n');
            iteration ++;
            counter = 0;
        }
        else counter ++;
        
        comActionPrev = comAction;
        driveSpikesToMuscles(sR2dsp,sL2dsp,spikeTime[left],spikeTime[right]);
        if(triggered != 0) cmdTimeOut--;                // cmd timeout down counter
#endif // runONdsp
        // printf("%d %d %d %d %d %d \n", comInitArray[0], comInitArray[2], comInitArray[4], comInitArray[6], comInitArray[8], comInitArray[10]);
#ifdef write2files
        //----Print output to files----------------
        //   int r=3, c=4;
        //   int **arr;
        //   int count = 0,i,j;
        
        //   arr  = (int **)malloc(sizeof(int *) * r);
        //   arr[0] = (int *)malloc(sizeof(int) * c * r);
//        if(mainLoopIndex>10)
    //    {
            
         //should put this in its own ifdef so as not to confuse w write to file .... will do it soon
         //   xArrayElev = malloc(mmSeg*mmSide*sizeof(double));
         //   xArrayDep = malloc(mmSeg*mmSide*sizeof(double));
         //   xArraySwing = malloc(mmSeg*mmSide*sizeof(double));
         //   xArrayStance = malloc(mmSeg*mmSide*sizeof(double));
         //   xArrayProt = malloc(mmSeg*mmSide*sizeof(double));
         //   xArrayRet = malloc(mmSeg*mmSide*sizeof(double));
         //   xArrayExt = malloc(mmSeg*mmSide*sizeof(double));
         //   xArrayFlex = malloc(mmSeg*mmSide*sizeof(double));
            if (ind  ==  0){
                
                //alloc param array
                allocParamArray(); //alloc mem for alpha,sigma...
                int i;
                
                ////Alloc Memory for x array
                xArrayElev  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayElev[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                }
                
                xArrayDep  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayDep[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));

                }
            
                xArraySwing  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArraySwing[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                }
            
                xArrayStance  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayStance[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                }
            
                xArrayProt  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayProt[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                }
                
        
                xArrayRet  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayRet[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                }
                
            
                xArrayExt  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayExt[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                }

                
                xArrayFlex  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayFlex[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                }
                
                xArrayCoord  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayCoord[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                }
                
                //cellF   forward
                xArrayF = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayF[i] = (double *)malloc(mmSide * sizeof(double));
                }
                
                //cellB  backward
                xArrayB = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayB[i] = (double *)malloc(mmSide * sizeof(double));
                }
                
                //cellLL
                xArrayLL = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayLL[i] = (double *)malloc(mmSide * sizeof(double));
                }
                
                //cellLT
                xArrayLT = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayLT[i] = (double *)malloc(mmSide * sizeof(double));
                }
                
                //cellPcn
                xArrayPcn = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayPcn[i] = (double *)malloc(mmSide * pitchStates *sizeof(double));
                }
                
                //cellModCom
                xArrayModCom = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayModCom[i] = (double *)malloc(mmSide * sizeof(double));
                }
                
                //cellH
                xArrayH = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++){
                    xArrayH[i] = (double *)malloc(mmSide * sizeof(double));
                }
            }
        
            //done alloc memory
            int indy = 0;
            //printf("%f", mainLoopIndex);
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                for(iSeg = 0;iSeg < mmSeg; ++iSeg)
                {
                    //printf("%f",mainLoopIndex);
                    fprintf(f10," %lf", cellElevator[iSide][iSeg].x);
                    fprintf(f10," ");
                    xArrayElev[ind][indy] = cellElevator[iSide][iSeg].x; //test
                    alphaArrayElev[iSide][iSeg]= cellElevator[iSide][iSeg].alpha;
                    sigmaArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].sigma;
                    sigmaIArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].sigmaI;
                    sigmaEArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].sigmaE;
                    betaEArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].betaE;
                    betaIArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].betaI;
                    IdcArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].Idc;
                    
                    
                    fprintf(f11," %lf", cellDepressor[iSide][iSeg].x);
                    fprintf(f11,"") ;
                    xArrayDep[ind][indy] = cellDepressor[iSide][iSeg].x; //test
                    alphaArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].alpha;
                    sigmaArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].sigma;
                    sigmaIArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].sigmaI;
                    sigmaEArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].sigmaE;
                    betaEArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].betaE;
                    betaIArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].betaI;
                    IdcArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].Idc;

                    
                    fprintf(f12," %lf", cellSwing[iSide][iSeg].x);
                    fprintf(f12," ");
                    xArraySwing[ind][indy] = cellSwing[iSide][iSeg].x; //test
                    alphaArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].alpha;
                    sigmaArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].sigma;
                    sigmaIArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].sigmaI;
                    sigmaEArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].sigmaE;
                    betaEArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].betaE;
                    betaIArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].betaI;
                    IdcArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].Idc;
                    
                    fprintf(f13," %lf", cellStance[iSide][iSeg].x);
                    fprintf(f13," ");
                    xArrayStance[ind][indy] = cellStance[iSide][iSeg].x; //test
                    alphaArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].alpha;
                    sigmaArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].sigma;
                    sigmaIArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].sigmaI;
                    sigmaEArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].sigmaE;
                    betaEArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].betaE;
                    betaIArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].betaI;
                    IdcArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].Idc;
                    
                    
                    fprintf(f14," %lf", cellProtractor[iSide][iSeg].x);
                    fprintf(f14," ");
                    xArrayProt[ind][indy] = cellProtractor[iSide][iSeg].x; //test
                    alphaArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].alpha;
                    sigmaArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].sigma;
                    sigmaIArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].sigmaI;
                    sigmaEArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].sigmaE;
                    betaEArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].betaE;
                    betaIArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].betaI;
                    IdcArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].Idc;
                    
                    
                    fprintf(f15," %lf", cellRetractor[iSide][iSeg].x);
                    fprintf(f15," ");
                    xArrayRet[ind][indy] = cellRetractor[iSide][iSeg].x; //test
                    alphaArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].alpha;
                    sigmaArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].sigma;
                    sigmaIArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].sigmaI;
                    sigmaEArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].sigmaE;
                    betaEArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].betaE;
                    betaIArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].betaI;
                    IdcArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].Idc;
                    
                    
                    fprintf(f16," %lf", cellExtensor[iSide][iSeg].x);
                    fprintf(f16," ");
                    xArrayExt[ind][indy] = cellExtensor[iSide][iSeg].x; //test
                    alphaArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].alpha;
                    sigmaArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].sigma;
                    sigmaIArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].sigmaI;
                    sigmaEArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].sigmaE;
                    betaEArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].betaE;
                    betaIArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].betaI;
                    IdcArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].Idc;
                    
                    fprintf(f17," %lf", cellFlexor[iSide][iSeg].x);
                    fprintf(f17," ");
                    xArrayFlex[ind][indy] = cellFlexor[iSide][iSeg].x; //test
                    alphaArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].alpha;
                    sigmaArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].sigma;
                    sigmaIArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].sigmaI;
                    sigmaEArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].sigmaE;
                    betaEArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].betaE;
                    betaIArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].betaI;
                    IdcArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].Idc;
                    
                    xArrayCoord[ind][indy] = cellCoord[iSide][iSeg].x; //test
                    alphaArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].alpha;
                    sigmaArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].sigma;
                    sigmaIArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].sigmaI;
                    sigmaEArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].sigmaE;
                    betaEArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].betaE;
                    betaIArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].betaI;
                    IdcArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].Idc;
                    indy++;
                }
            }
            
            int indy2 = 0;
            ///do the command neuron here
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                xArrayF[ind][indy2] = cellF[iSide].x;
                alphaArrayF[iSide][0] = cellF[iSide].alpha;
                sigmaArrayF[iSide][0] = cellF[iSide].sigma;
                sigmaIArrayF[iSide][0] = cellF[iSide].sigmaI;
                sigmaEArrayF[iSide][0] = cellF[iSide].sigmaE;
                betaEArrayF[iSide][0] = cellF[iSide].betaE;
                betaIArrayF[iSide][0] = cellF[iSide].betaI;
                IdcArrayF[iSide][0] = cellF[iSide].Idc;
                
            
                xArrayB[ind][indy2] = cellB[iSide].x;
                alphaArrayB[iSide][0] = cellB[iSide].alpha;
                sigmaArrayB[iSide][0] = cellB[iSide].sigma;
                sigmaIArrayB[iSide][0] = cellB[iSide].sigmaI;
                sigmaEArrayB[iSide][0] = cellB[iSide].sigmaE;
                betaEArrayB[iSide][0] = cellB[iSide].betaE;
                betaIArrayB[iSide][0] = cellB[iSide].betaI;
                IdcArrayB[iSide][0] = cellB[iSide].Idc;
                
                xArrayLL[ind][indy2] = cellLL[iSide].x;
                alphaArrayLL[iSide][0] = cellLL[iSide].alpha;
                sigmaArrayLL[iSide][0] = cellLL[iSide].sigma;
                sigmaIArrayLL[iSide][0] = cellLL[iSide].sigmaI;
                sigmaEArrayLL[iSide][0] = cellLL[iSide].sigmaE;
                betaEArrayLL[iSide][0] = cellLL[iSide].betaE;
                betaIArrayLL[iSide][0] = cellLL[iSide].betaI;
                IdcArrayLL[iSide][0] = cellLL[iSide].Idc;
            
                
                xArrayLT[ind][indy2] = cellLT[iSide].x;
                alphaArrayLT[iSide][0] = cellLT[iSide].alpha;
                sigmaArrayLT[iSide][0] = cellLT[iSide].sigma;
                sigmaIArrayLT[iSide][0] = cellLT[iSide].sigmaI;
                sigmaEArrayLT[iSide][0] = cellLT[iSide].sigmaE;
                betaEArrayLT[iSide][0] = cellLT[iSide].betaE;
                betaIArrayLT[iSide][0] = cellLT[iSide].betaI;
                IdcArrayLT[iSide][0] = cellLT[iSide].Idc;
                
                xArrayModCom[ind][indy2] = cellModCom[iSide].x;
                alphaArrayModCom[iSide][0] = cellModCom[iSide].alpha;
                sigmaArrayModCom[iSide][0] = cellModCom[iSide].sigma;
                sigmaIArrayModCom[iSide][0] = cellModCom[iSide].sigmaI;
                sigmaEArrayModCom[iSide][0] = cellModCom[iSide].sigmaE;
                betaEArrayModCom[iSide][0] = cellModCom[iSide].betaE;
                betaIArrayModCom[iSide][0] = cellModCom[iSide].betaI;
                IdcArrayModCom[iSide][0] = cellModCom[iSide].Idc;
                
                xArrayH[ind][indy2] = cellH[iSide].x;
                alphaArrayH[iSide][0] = cellH[iSide].alpha;
                sigmaArrayH[iSide][0] = cellH[iSide].sigma;
                sigmaIArrayH[iSide][0] = cellH[iSide].sigmaI;
                sigmaEArrayH[iSide][0] = cellH[iSide].sigmaE;
                betaEArrayH[iSide][0] = cellH[iSide].betaE;
                betaIArrayH[iSide][0] = cellH[iSide].betaI;
                IdcArrayH[iSide][0] = cellH[iSide].Idc;
                
                indy2++;
            }
            
            
            int indy3 = 0;
            //do the Pcn neuron here, cause it's a special case
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                for (iSeg =0; iSeg < pitchStates; ++iSeg){
                    xArrayPcn[ind][indy3] = cellPcn[iSide][iSeg].x;
                    alphaArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].alpha;
                    sigmaArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].sigma;
                    sigmaIArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].sigmaI;
                    sigmaEArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].sigmaE;
                    betaEArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].betaE;
                    betaIArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].betaI;
                    IdcArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].Idc;
                    
                    indy3++;
                }
            
            }
            
            
            
        
            ind++;
            if (ind == IterNumChosen-1){
                ind = 0;
                //sleep(0.5*samplesizechosen); //0.5 is the refresh rate of
                ///////////DO I NEED TO FREE PARAM ARRAY AS WELL?
            free(xArrayDep);
            free(xArrayExt);
            free(xArrayFlex);
            free(xArrayProt);
            free(xArrayStance);
            free(xArraySwing);
            free(xArrayElev);
            free(xArrayRet);
            free(xArrayCoord);
            
            free(xArrayPcn);
            free(xArrayF);
            free(xArrayB);
            free(xArrayLL);
            free(xArrayLT);
            free(xArrayModCom);
            free(xArrayH);
                
            freeParamsArray();
            }
            
            fprintf(f10, "\n");
            fprintf(f11, "\n");
            fprintf(f12, "\n");
            fprintf(f13, "\n");
            fprintf(f14, "\n");
            fprintf(f15, "\n");
            fprintf(f16, "\n");
            fprintf(f17, "\n");
            
     //   }
        for(jj = 0; jj < mmPcn2; ++jj)
        {
            fprintf(f18," %lf", cellPcn[jj][pitch].x);//f13 now Mot and and f14 is Pcn
        }
        fprintf(f18,"\n");
        // printf("%3f \n", mainLoopIndex);
        if((int)(mainLoopIndex/(100))*(100) == mainLoopIndex)
        {
            // printf("calculating %f times\n", mainLoopIndex);
            // f31 = fopen("R:\\parameters","r");
            // f32 = fopen("R:\\commands","r");
            // fscanf(f31,"%d %d", &spikeTime[left], &spikeTime[right]);
            // fscanf(f32,"%d %d %d %d %d %d %d", &comAction, &comDirection, &comLeftBias, &comRightBias, &comPcnInclD, &comPcnTarget, &comPcnTargetD );
            // fclose(f31);
            // fclose(f32);
            //printf("%3f \n", mainLoopIndex);
            //printf("%d %d %d \n", comAction, comLeftBias, comRightBias);
            // printf("%d %d %d %d %d %d %d \n", comAction, comDirection, comLeftBias, comRightBias, comPcnInclD, comPcnTarget, comPcnTargetD );
            
        }
        if (((int) (mainLoopIndex)) % 10000 == 0  && ((int) (mainLoopIndex - 10000)) >= 0){ //after another 5000 iteration
            //-----------------close ALL files after 100 iterations to end buffer-----------------------------------
            // printf("close at %f\n", mainLoopIndex -1);
            fclose(f10); //Cin
            fclose(f11); //Ein
            fclose(f12); //Lin
            fclose(f13); //
            fclose(f14); //
            fclose(f15); //
            fclose(f16); //
            fclose(f17); //
            fclose(f18);
        }
#else
        /*      spikeTime[left] = 5;
         spikeTime[right] = 5;
         comAction = 1;
         comDirection = 1;
         comLeftBias = 5;
         comRightBias = 5;
         */
#endif
    }

    //END OF WHILE
    //--------------------END CALCULATION-------------------------------
    sL2dsp = 0;
    sR2dsp = 0;
    for(jj = 0; jj < 100; ++jj)
    {
#ifdef send2dsp
        sendSpikesToDsp(sL2dsp,sR2dsp,0,0);
        printf("%d,%d\n",sL2dsp,sR2dsp);
#endif   // send2dsp
#ifdef runONdsp
        driveSpikesToMuscles(sR2dsp,sL2dsp,spikeTime[left],spikeTime[right]);
#endif	 //runONdsp
    }
#ifdef write2files
    //-----------------close ALL files-----------------------------------
    fclose(f10); //Cin
    fclose(f11); //Ein
    fclose(f12); //Lin
    fclose(f13); //
    fclose(f14); //
    fclose(f15); //
    fclose(f16); //
    fclose(f17); //
    fclose(f18);
    //fclose(f13); //Mot
    //fclose(f14); //Pcn
    //  fclose(f22);
    //  fclose(f31);
#endif
    //  LOG_printf(&trace,"\n");
    //return 0;
}




void indicateSampleSize(int s){
    samplesizechosen = s;//get the sample size values chosen by Waveforms.m and put it in the global var, so C code knows how long to wait;
    printf("sample size = %d\n", samplesizechosen);
};


void indicateNumberOfIteration(int i){
    IterNumChosen = i;
    printf("iteration = %d\n", IterNumChosen);
}

void editParam(int *neuronName, int*side, int*seg, double *a, double *s, double *sE, double *sI, double *bE, double *bI, double *Idc, int size){
    //printf("%s\n",neuronName);
    printf("begin editing\n");
    int i;
    //free first
    free(globalAlpha);
    free(globalSigma);
    free(globalSigmaE);
    free(globalSigmaI);
    free(globalBetaE);
    free(globalBetaI);
    free(globalIdc);
    free(globalCellName);
    free(globalSeg);
    free(globalSide);
    //then alloc mem
    globalAlpha = malloc(size*sizeof(double));
    globalSigma = malloc(size*sizeof(double));
    globalSigmaE = malloc(size*sizeof(double));
    globalSigmaI = malloc(size*sizeof(double));
    globalBetaE = malloc(size*sizeof(double));
    globalBetaI = malloc(size*sizeof(double));
    globalIdc= malloc(size*sizeof(double));
    globalCellName = malloc(size*sizeof(int));
    globalSide = malloc(size*sizeof(int));
    globalSeg = malloc(size*sizeof(int));
    beginEditingParams = 1; //set flag for begin editing
    globalSize = size;
    
    for (i=0; i< size; i++){
    //printf("size = %d",size);
        //printf("fkkk");
  //  printf("%f",a[i]);
    globalAlpha[i] = a[i];
    globalSigma[i] = s[i];
    globalSigmaE[i] = sE[i];
    globalSigmaI[i] = sI[i];
    globalBetaE[i] = bE[i];
    globalBetaI[i] = bI[i];
    globalIdc[i] = Idc[i];
    globalCellName[i] = neuronName[i];
    globalSide[i] = side[i];
    globalSeg[i] = seg[i];
    }
   // setNeuronParams(neuronName, s, a);
}

int checkMainLoopIndex(){
    int i =globalLoopIndex;
    return i;
}

//this will determine what cell are chosen
/*int whatTypeofCell(char* neuronName){
    return 0; //let's assume always Spiking for now
    ///PLZZ EDIT, or whatever, prolly not gonna used
}*/


///add the multiple neuron params here//
void setMultipleNeuronParam(int* idArr, int* sideArr, int* segArr, double* aArr, double* sArr,double* sEArr, double* sIArr, double *bEArr, double *bIArr, double *IdcArr, int size){
    int i;
    for (i = 0; i < size; i++){
        //this will set the param for each neuron in the array
        setNeuronParams(idArr[i], sideArr[i], segArr[i],aArr[i], sArr[i], sEArr[i], sIArr[i], bEArr[i],bIArr[i], IdcArr[i]);
    }
}// change params

////end///////////

void setNeuronParams(int id, int side, int seg, double a, double s, double sE, double sI, double bE, double bI, double Idc){
    //identify which type it is?
   // if (id == 8){//Spiking
        //printf("Spiking edit\n");
        //struct structSpiking** cell = getSpike(neuronName);
       // struct structSpiking** cell;
       // cell = cellFlexor;
        //printf("%f\n",a);
    //    int iSide, iSeg;
   //     for(iSide = 0;iSide < mmSide; ++iSide)
    //    {
            //Now loop down the segments
     //       for(iSeg = 0;iSeg < mmSeg; ++iSeg)
     //       {
                if(id == 0){
                    cellElevator[side][seg].alpha = a;
                    cellElevator[side][seg].sigma = s;
                    cellElevator[side][seg].sigmaE = sE;
                    cellElevator[side][seg].sigmaI = sI;
                    cellElevator[side][seg].betaE = bE;
                    cellElevator[side][seg].betaI = bI;
                    cellElevator[side][seg].Idc = Idc;

                }
                else if(id == 1){
                    cellSwing[side][seg].alpha = a;
                    cellSwing[side][seg].sigma = s;
                    cellSwing[side][seg].sigmaE = sE;
                    cellSwing[side][seg].sigmaI = sI;
                    cellSwing[side][seg].betaE = bE;
                    cellSwing[side][seg].betaI = bI;
                    cellSwing[side][seg].Idc = Idc;
                }
                else if(id == 2){
                    cellDepressor[side][seg].alpha = a;
                    cellDepressor[side][seg].sigma = s;
                    cellDepressor[side][seg].sigmaE = sE;
                    cellDepressor[side][seg].sigmaI = sI;
                    cellDepressor[side][seg].betaE = bE;
                    cellDepressor[side][seg].betaI = bI;
                    cellDepressor[side][seg].Idc = Idc;

                }
                else if(id == 3){
                    cellStance[side][seg].alpha = a;
                    cellStance[side][seg].sigma = s;
                    cellStance[side][seg].sigmaE = sE;
                    cellStance[side][seg].sigmaI = sI;
                    cellStance[side][seg].betaE = bE;
                    cellStance[side][seg].betaI = bI;
                    cellStance[side][seg].Idc = Idc;
                    
                }
                else if(id == 4){
                    cellCoord[side][seg].alpha = a;
                    cellCoord[side][seg].sigma = s;
                    cellCoord[side][seg].sigmaE = sE;
                    cellCoord[side][seg].sigmaI = sI;
                    cellCoord[side][seg].betaE = bE;
                    cellCoord[side][seg].betaI = bI;
                    cellCoord[side][seg].Idc = Idc;
                }
                else if(id == 5){
                    cellProtractor[side][seg].alpha = a;
                    cellProtractor[side][seg].sigma = s;
                    cellProtractor[side][seg].sigmaE = sE;
                    cellProtractor[side][seg].sigmaI = sI;
                    cellProtractor[side][seg].betaE = bE;
                    cellProtractor[side][seg].betaI = bI;
                    cellProtractor[side][seg].Idc = Idc;
                }
                else if(id == 6){
                    cellRetractor[side][seg].alpha = a;
                    cellRetractor[side][seg].sigma = s;
                    cellRetractor[side][seg].sigmaE = sE;
                    cellRetractor[side][seg].sigmaI = sI;
                    cellRetractor[side][seg].betaE = bE;
                    cellRetractor[side][seg].betaI = bI;
                    cellRetractor[side][seg].Idc = Idc;
                }
                else if(id == 7){
                    cellExtensor[side][seg].alpha = a;
                    cellExtensor[side][seg].sigma = s;
                    cellExtensor[side][seg].sigmaE = sE;
                    cellExtensor[side][seg].sigmaI = sI;
                    cellExtensor[side][seg].betaE = bE;
                    cellExtensor[side][seg].betaI = bI;
                    cellExtensor[side][seg].Idc = Idc;
                }
                else if(id ==8){
                    cellFlexor[side][seg].alpha = a;
                    cellFlexor[side][seg].sigma = s;
                    cellFlexor[side][seg].sigmaE = sE;
                    cellFlexor[side][seg].sigmaI = sI;
                    cellFlexor[side][seg].betaE = bE;
                    cellFlexor[side][seg].betaI = bI;
                    cellFlexor[side][seg].Idc = Idc;
                    
                }

        //    }
      //  }

   // for(iSide = 0;iSide < mmSide; ++iSide)
   // {
        if (id == 9){
            cellF[side].alpha = a;
            cellF[side].sigma = s;
            cellF[side].sigmaE = sE;
            cellF[side].sigmaI = sI;
            cellF[side].betaE = bE;
            cellF[side].betaI = bI;
            cellF[side].Idc = Idc;
        }
        else if(id == 10){
            cellB[side].alpha = a;
            cellB[side].sigma = s;
            cellB[side].sigmaE = sE;
            cellB[side].sigmaI = sI;
            cellB[side].betaE = bE;
            cellB[side].betaI = bI;
            cellB[side].Idc = Idc;
        }
        else if(id == 11){
            cellLL[side].alpha = a;
            cellLL[side].sigma = s;
            cellLL[side].sigmaE = sE;
            cellLL[side].sigmaI = sI;
            cellLL[side].betaE = bE;
            cellLL[side].betaI = bI;
            cellLL[side].Idc = Idc;
        }
        else if (id == 12) {
            cellLT[side].alpha = a;
            cellLT[side].sigma = s;
            cellLT[side].sigmaE = sE;
            cellLT[side].sigmaI = sI;
            cellLT[side].betaE = bE;
            cellLT[side].betaI = bI;
            cellLT[side].Idc = Idc;
        }
        else if (id == 14){
            cellModCom[side].alpha = a;
            cellModCom[side].sigma = s;
            cellModCom[side].sigmaE = sE;
            cellModCom[side].sigmaI = sI;
            cellModCom[side].betaE = bE;
            cellModCom[side].betaI = bI;
            cellModCom[side].Idc = Idc;
        }
        else if (id == 15){
            cellH[side].alpha = a;
            cellH[side].sigma = s;
            cellH[side].sigmaE = sE;
            cellH[side].sigmaI = sI;
            cellH[side].betaE = bE;
            cellH[side].betaI = bI;
            cellH[side].Idc = Idc;
        }
   // }
   // for(iSide = 0;iSide < mmSide; ++iSide)
  //  {
   //     for (iSeg = 0; iSeg < pitchStates; ++iSeg){
            if (id == 13){
                //pcn
                cellPcn[side][seg].alpha = a;
                cellPcn[side][seg].sigma = s;
                cellPcn[side][seg].sigmaE = sE;
                cellPcn[side][seg].sigmaI = sI;
                cellPcn[side][seg].betaE = bE;
                cellPcn[side][seg].betaI = bI;
                cellPcn[side][seg].Idc = Idc;
            }
      //  }
    //}

    beginEditingParams = 0; //stop editing by turning the edit flag off
    

}// change alpha and sigma


// +++++++++++  Function to calculate the right hand sides for ALL maps +++++++++++++++
void computeMAPs(double mainLoopIndex)
{
    //printf("%d", beginEditingParams);
    clock_t t1, t2;
    t1 = clock();
    //        int ii,jj,ii1,iii;
    int iSeg, iSide;
    // ---  Total number of spikes generated by each groupe of neurons ------------
    // lobster spike variables
    double spikesElevator[mmSide][mmSeg];
    double spikesDepressor[mmSide][mmSeg];
    double spikesProtractor[mmSide][mmSeg];
    double spikesRetractor[mmSide][mmSeg];
    double spikesExtensor[mmSide][mmSeg];
    double spikesFlexor[mmSide][mmSeg];
    double spikesStance[mmSide][mmSeg];
    double spikesSwing[mmSide][mmSeg];
    double spikesModCom[mmSide];
    double spikesCoord[mmSide][mmSeg];
    //    double spikesMot[mmSide][mmSeg];
    double spikesPcn[mmSide];
    double spikesF[mmSide];
    double spikesB[mmSide];
    double spikesLL[mmSide];
    double spikesLT[mmSide];
    
    
    //These are the excitatory synaptic currents within a segmental CPG
    double  iExcSegSwingProt[mmSide][mmSeg],
    iExcSegSwingRet[mmSide][mmSeg],
    iExcSegSwingExt[mmSide][mmSeg],
    iExcSegSwingFlex[mmSide][mmSeg],
    iExcSegStanceProt[mmSide][mmSeg],
    iExcSegStanceRet[mmSide][mmSeg],
    iExcSegStanceExt[mmSide][mmSeg],
    iExcSegStanceFlex[mmSide][mmSeg];
    
    //These are the inhibitory synaptic currents withing a segmental CPG
    double  iInhSegEleDep[mmSide][mmSeg],       /*inhibition from elevator to depressor*/
    iInhSegEleStance[mmSide][mmSeg],    /*inhibition from elevator to stance   */
    iInhSegStanceSwing[mmSide][mmSeg];  /*inhibition from stance to swing      */
    
    
    //These are the excitatory synaptic currents from the intersegmental modulatory commmand to  segmental CPG interneurons
    double  iExcIntModComEle[mmSide][mmSeg],
    iExcIntModComDep[mmSide][mmSeg],
    iExcIntModComSwing[mmSide][mmSeg],
    iExcIntModComStance[mmSide][mmSeg];
    
    double 	iExcSegPcnDep[mmSide][mmSeg];																		//Synapse between pitch Command and segmental depressors
    
    
    // These are the excitatory synapsic connections from the directional commands to the propulsive synergies for walking in the four directions
    double  iExcForRet[mmSide][mmSeg],
    iExcBackProt[mmSide][mmSeg],
    iExcLLFlx[mmSide][mmSeg],
    iExcLTExt[mmSide][mmSeg];
    
    // These are the excitatory synapsic connections from the directional commands to the ModCom for walking in the four directions
    double  iExcForModCom[mmSide],
    iExcBackModCom[mmSide],
    iExcLLModCom[mmSide],
    iExcLTModCom[mmSide];
    
    
    // Presynaptic Inhibition from a command to bifunctional interneuron synapse
    double  iInhIntFSwing[mmSide],
    iInhIntFStance[mmSide],
    iInhIntBSwing[mmSide],
    iInhIntBStance[mmSide],
    iInhIntLLSwing[mmSide],
    iInhIntLLStance[mmSide],
    iInhIntLTSwing[mmSide],
    iInhIntLTStance[mmSide];
    
    double iInhSegCoordEle[mmSide][mmSeg];       // Segmental Inhibition from an Coordinating Neuron to an Elevator
    double iExcSegContEleCoord[mmSide][mmSeg];   // Intersegmental Exitation from and elevator to a contralatersl coordinating neuron
    double iExcIntRosEleCoord[mmSide][mmSeg];    // Intersegmental Exitation from and elevator to a rostral or caudal ipsilateral coordinating neuron
    double iExcIntCaudEleCoord[mmSide][mmSeg];   // Intersegmental Exitation from and elevator to a rostral or caudal ipsilateral coordinating neuron
    
    
    
    
    //   pInhIntFSwing[mmSide],pInhIntFStance[mmSide],pInhIntBSwing[mmSide],pInhIntBStance[mmSide],           //Presynaptic Inhibition from a command to bifunctional interneuron synapse
    //   pInhIntLLSwing[mmSide],pInhIntLLStance[mmSide],pInhIntLTSwing[mmSide],pInhIntLTStance[mmSide],      //Presynaptic Inhibition from a command to bifunctional interneuron synapse
    //   iExcIntRosEleCoord[mmSide][mmSeg],  iExcIntCaudEleCoord[mmSide][mmSeg],iExcSegContEleCoord[mmSide][mmSeg],   //Excitatory Synapses from Elevator to adjacent coordinating neurons
    // ---- Start current calculation with zero total for each cell --------
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)  //Zero the synaptic currents
        {
            //Zero the synaptic currents and spike counts
            iExcSegPcnDep[iSide][iSeg] = 0;
            iInhSegCoordEle[iSide][iSeg] = 0;        iExcSegContEleCoord[iSide][iSeg] = 0;    iExcIntRosEleCoord[iSide][iSeg] = 0;     iExcIntCaudEleCoord[iSide][iSeg] = 0;
            
            iExcSegSwingProt[iSide][iSeg] = 0;		iInhSegEleDep[iSide][iSeg] = 0;			iInhSegStanceSwing[iSide][iSeg] = 0;
            iExcSegStanceProt[iSide][iSeg] = 0;	    iExcSegStanceRet[iSide][iSeg] = 0;      iExcSegStanceExt[iSide][iSeg] = 0;	    iExcSegStanceFlex[iSide][iSeg] = 0;
            iExcSegSwingRet[iSide][iSeg] = 0;	    iExcSegSwingExt[iSide][iSeg] = 0;	    iExcSegSwingFlex[iSide][iSeg] = 0;
            iExcIntModComEle[iSide][iSeg] = 0;	    iExcIntModComDep[iSide][iSeg] = 0;	    iExcIntModComSwing[iSide][iSeg] = 0;	iExcIntModComStance[iSide][iSeg] = 0;
            iExcIntRosEleCoord[iSide][iSeg] = 0;        iExcIntCaudEleCoord[iSide][iSeg] = 0;        iExcSegContEleCoord[iSide][iSeg] = 0;
            //------- and initiate spike counter for each group of cells -------------------
            spikesElevator[iSide][iSeg] = 0;		spikesDepressor[iSide][iSeg] = 0;   spikesSwing[iSide][iSeg] = 0;           spikesStance[iSide][iSeg] = 0;  //CPG Neurons
            spikesProtractor[iSide][iSeg] = 0;      spikesRetractor[iSide][iSeg] = 0;   spikesExtensor[iSide][iSeg] = 0;        spikesFlexor[iSide][iSeg] = 0;  //Bifunctional Motor Neurons
            spikesCoord[iSide][iSeg] = 0;
        }
        iInhIntFSwing[iSide] = 0;   iInhIntFStance[iSide] = 0;  iInhIntBSwing[iSide] = 0;   iInhIntBStance[iSide] = 0;
        
        iInhIntLLSwing[iSide] = 0;  iInhIntLLStance[iSide] = 0; iInhIntLTSwing[iSide] = 0;  iInhIntLTStance[iSide] = 0;
        
        iExcForModCom[iSide] = 0;
        iExcLTModCom[iSide] = 0;
        iExcLLModCom[iSide] = 0;
        iExcBackModCom[iSide]=0;
        //------- and initiate spike counter for each group of cells -------------------
        spikesF[iSide] = 0;                     spikesB[iSide] = 0;                 spikesLL[iSide] = 0;                    spikesLT[iSide] = 0;            //Directional Commands
        spikesModCom[iSide] = 0;                spikesB[iSide] = 0;                 spikesLL[iSide] = 0;                    spikesLT[iSide] = 0;            //Directional Commands
        spikesPcn[iSide] = 0;
    }
    // --------- Calculating number of presynaptic spikes in each group of cells ---------------
    for(iSeg = 1;iSeg < mmSeg-1; ++iSeg)
    {
        for(iSide = 0;iSide < mmSide; ++iSide)
        {
            spikesElevator[iSide][iSeg]     = spikesElevator[iSide][iSeg]  + cellElevator[iSide][iSeg].spike  + cNextSeg * (cellElevator[iSide][iSeg-1].spike  + cellElevator[iSide][iSeg + 1].spike);
            spikesDepressor[iSide][iSeg]    = spikesDepressor[iSide][iSeg] + cellDepressor[iSide][iSeg].spike + cNextSeg * (cellDepressor[iSide][iSeg-1].spike + cellDepressor[iSide][iSeg + 1].spike);
            spikesSwing[iSide][iSeg]        = spikesSwing[iSide][iSeg]     + cellSwing[iSide][iSeg].spike     + cNextSeg * (cellSwing[iSide][iSeg-1].spike     + cellSwing[iSide][iSeg + 1].spike);
            spikesStance[iSide][iSeg]       = spikesStance[iSide][iSeg]    + cellStance[iSide][iSeg].spike    + cNextSeg * (cellStance[iSide][iSeg-1].spike    + cellStance[iSide][iSeg + 1].spike);
        }
        
    }  //END for (iSeg)
    iSeg = 0;
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        spikesElevator[iSide][iSeg]  = spikesElevator[iSide][iSeg]  + cellElevator[iSide][iSeg].spike   + cNextSeg*cellElevator[iSide][iSeg + 1].spike;
        spikesDepressor[iSide][iSeg] = spikesDepressor[iSide][iSeg] + cellDepressor[iSide][iSeg].spike  + cNextSeg*cellDepressor[iSide][iSeg + 1].spike;
        spikesSwing[iSide][iSeg]     = spikesSwing[iSide][iSeg]     + cellSwing[iSide][iSeg].spike      + cNextSeg*cellSwing[iSide][iSeg + 1].spike;
        spikesStance[iSide][iSeg]    = spikesStance[iSide][iSeg]    + cellStance[iSide][iSeg].spike     + cNextSeg*cellStance[iSide][iSeg + 1].spike;
    }
    iSeg = mmSeg-1;
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        spikesElevator[iSide][iSeg]  = spikesElevator[iSide][iSeg] 	+ cellElevator[iSide][iSeg].spike 	+ cNextSeg*cellElevator[iSide][iSeg-1].spike;
        spikesDepressor[iSide][iSeg] = spikesDepressor[iSide][iSeg] + cellDepressor[iSide][iSeg].spike 	+ cNextSeg*cellDepressor[iSide][iSeg-1].spike;
        spikesSwing[iSide][iSeg]     = spikesSwing[iSide][iSeg] 	+ cellSwing[iSide][iSeg].spike 		+ cNextSeg*cellSwing[iSide][iSeg-1].spike;
        spikesStance[iSide][iSeg]    = spikesStance[iSide][iSeg] 	+ cellStance[iSide][iSeg].spike 	+ cNextSeg*cellStance[iSide][iSeg-1].spike;
    }
    // --- Compute the number of spikes generated by EIN in each hemisphare -------- How many spikes were generated
    //This was the location of codeFragment3.c
    
    // void calcSynapticCurrents(double *I,   struct structSynapses *params,  double xPost,   double spikes)
    
    // ------ Calculation of synaptic currents recieved by each cell -------------------
    /*
     inhSegEleDep[mmSide][mmSeg]
     inhSegEleStance[mmSide][mmSeg]
     inhSegStanceSwing[mmSide][mmSeg]
     
     excSegStanceProt[mmSide][mmSeg]
     excSegStanceRet[mmSide][mmSeg]
     excSegStanceExt[mmSide][mmSeg]
     excSegStanceFlx[mmSide][mmSeg]
     
     excSegSwingProt[mmSide][mmSeg]
     excSegSwingRet[mmSide][mmSeg]
     excSegSwingExt[mmSide][mmSeg]
     excSegSwingFlx[mmSide][mmSeg]
     */
    for(iSide = 0;iSide < mmSide; ++iSide){
        
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)	//This loop calculates the internal CPG synapses
            
        {
           // calcSynapticCurrents( &iInhSegEleDep[iSide][iSeg],    		&pInhSegEleDep[iSide][iSeg],  		cellElevator[iSide][iSeg].x,       spikesElevator[iSide][iSeg]/mmSeg);
             calcSynapticCurrents( &iInhSegEleDep[iSide][iSeg],    		&pInhSegEleDep[iSide][iSeg],  		cellDepressor[iSide][iSeg].x,       spikesElevator[iSide][iSeg]);
            
          //  calcSynapticCurrents( &iInhSegEleStance[iSide][iSeg],    	&pInhSegEleStance[iSide][iSeg],  	cellElevator[iSide][iSeg].x,       spikesElevator[iSide][iSeg]/mmSeg);
             calcSynapticCurrents( &iInhSegEleStance[iSide][iSeg],    	&pInhSegEleStance[iSide][iSeg],  	cellStance[iSide][iSeg].x,       spikesElevator[iSide][iSeg]);
            
            
            
            
          //  calcSynapticCurrents( &iInhSegStanceSwing[iSide][iSeg],    	&pInhSegStanceSwing[iSide][iSeg],  	cellStance[iSide][iSeg].x,         spikesStance[iSide][iSeg]/mmSeg);
            calcSynapticCurrents( &iInhSegStanceSwing[iSide][iSeg],    	&pInhSegStanceSwing[iSide][iSeg],  	cellSwing[iSide][iSeg].x,         spikesStance[iSide][iSeg]);
            
            
            // These are the synapses within the core neuronal oscillator
            iInhSegEleDep[iSide][iSeg]         		= iInhSegEleDep[iSide][iSeg] * cCalSeg;
            iInhSegEleStance[iSide][iSeg]         	= iInhSegEleStance[iSide][iSeg] * cCalSeg;
            iInhSegStanceSwing[iSide][iSeg]         = iInhSegStanceSwing[iSide][iSeg] * cCalSeg;
            /*
             void calcModulatedCurrents(double *I, //This implements presynaptic inhibition, modParams is the moduatory neuron synapse
             struct structSynapses *params,struct structSynapses *modParams, double xPost,double spikes, double xModPost, double modSpikes) {
             pInhIntFSwing       // Inhibition from a Forward Command interneuron to a swing interneuron
             pInhIntFStance      // Inhibition from a Forward Command interneuron to a stance interneuron
             pInhIntBSwing       // Inhibition from a Backward Command interneuron to a swing interneuron
             pInhIntBStance      // Inhibition from a Backward Command interneuron to a stance interneuron
             pInhIntLLSwing       // Inhibition from a Leading Command interneuron to a swing interneuron
             pInhIntLLStance      // Inhibition from a Leading Command interneuron to a stance interneuron
             pInhIntLTSwing       // Inhibition from a Trailing Command interneuron to a swing interneuron
             pInhIntLTStance      // Inhibition from a Trailing Command interneuron to a stance interneuron
             */
            // These are the synapses coupling the swing interneron to the bifunctional motorneurons
            
            
           // calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellSwing[iSide][iSeg].x,   cellF[iSide].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
          
            calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellProtractor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
            
            
           // calcModulatedCurrents( &iExcSegSwingRet[iSide][iSeg],    &pExcSegSwingRet[iSide][iSeg],  &pInhIntBSwing[iSide],	 cellSwing[iSide][iSeg].x,   cellB[iSide].x,  spikesSwing[iSide][iSeg], spikesB[iSide]);
            calcModulatedCurrents( &iExcSegSwingRet[iSide][iSeg],    &pExcSegSwingRet[iSide][iSeg],  &pInhIntBSwing[iSide],	 cellRetractor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x,  spikesSwing[iSide][iSeg], spikesB[iSide]);
            
            
        //    calcModulatedCurrents( &iExcSegSwingExt[iSide][iSeg],    &pExcSegSwingExt[iSide][iSeg],  &pInhIntLTSwing[iSide], cellSwing[iSide][iSeg].x,   cellLT[iSide].x, spikesSwing[iSide][iSeg], spikesLT[iSide]);
            calcModulatedCurrents( &iExcSegSwingExt[iSide][iSeg],    &pExcSegSwingExt[iSide][iSeg],  &pInhIntLTSwing[iSide], cellExtensor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x, spikesSwing[iSide][iSeg], spikesLT[iSide]);

            
            
          //  calcModulatedCurrents( &iExcSegSwingFlex[iSide][iSeg],   &pExcSegSwingFlx[iSide][iSeg],  &pInhIntLLSwing[iSide], cellSwing[iSide][iSeg].x,   cellLL[iSide].x, spikesSwing[iSide][iSeg], spikesLL[iSide]);


            calcModulatedCurrents( &iExcSegSwingFlex[iSide][iSeg],   &pExcSegSwingFlx[iSide][iSeg],  &pInhIntLLSwing[iSide], cellFlexor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x, spikesSwing[iSide][iSeg], spikesLL[iSide]);
            
            
            ///SEARCH FOR ME!!!!!!!!!!!!!!
            // These are the synapses coupling the stance interneron to the bifunctional motorneurons
        //    calcModulatedCurrents( &iExcSegStanceProt[iSide][iSeg],   &pExcSegStanceProt[iSide][iSeg],  &pInhIntFStance[iSide],    	 cellStance[iSide][iSeg].x,  cellF[iSide].x,     spikesStance[iSide][iSeg], spikesF[iSide]);
            calcModulatedCurrents( &iExcSegStanceProt[iSide][iSeg],   &pExcSegStanceProt[iSide][iSeg],  &pInhIntFStance[iSide],    	 cellProtractor[iSide][iSeg].x,  cellStance[iSide][iSeg].x,     spikesStance[iSide][iSeg], spikesF[iSide]);
            
            
            ///SEARCH FOR ME!!!!!!!!!!!!!!
        //    calcModulatedCurrents( &iExcSegStanceRet[iSide][iSeg],	  &pExcSegStanceRet[iSide][iSeg],   &pInhIntBStance[iSide],      cellStance[iSide][iSeg].x,  cellB[iSide].x,     spikesStance[iSide][iSeg], spikesB[iSide]);
            calcModulatedCurrents( &iExcSegStanceRet[iSide][iSeg],	  &pExcSegStanceRet[iSide][iSeg],   &pInhIntBStance[iSide],      cellRetractor[iSide][iSeg].x,  cellStance[iSide][iSeg].x,     spikesStance[iSide][iSeg], spikesB[iSide]);

            
            
            
          //  calcModulatedCurrents( &iExcSegStanceExt[iSide][iSeg],    &pExcSegStanceExt[iSide][iSeg],   &pInhIntLTStance[iSide],     cellStance[iSide][iSeg].x,  cellLT[iSide].x,    spikesStance[iSide][iSeg], spikesLT[iSide]);
            calcModulatedCurrents( &iExcSegStanceExt[iSide][iSeg],    &pExcSegStanceExt[iSide][iSeg],   &pInhIntLTStance[iSide],     cellExtensor[iSide][iSeg].x,  cellStance[iSide][iSeg].x,    spikesStance[iSide][iSeg], spikesLT[iSide]);

            
            
          //  calcModulatedCurrents( &iExcSegStanceFlex[iSide][iSeg],   &pExcSegStanceFlx[iSide][iSeg],   &pInhIntLLStance[iSide], 	 cellStance[iSide][iSeg].x,  cellLL[iSide].x,    spikesStance[iSide][iSeg], spikesLL[iSide]);
            calcModulatedCurrents( &iExcSegStanceFlex[iSide][iSeg],   &pExcSegStanceFlx[iSide][iSeg],   &pInhIntLLStance[iSide], 	 cellFlexor[iSide][iSeg].x,  cellStance[iSide][iSeg].x,    spikesStance[iSide][iSeg], spikesLL[iSide]);
            

////////////////////IF UNSURE, COME BACK AND CHANGE WHAT's ABOVE HERE/////////////////////////////////////
          //COMMENT OUT MODCOM TO ELEV FOR NOW!!!
            
            // These are the synapses coupling the modulatory command interneron to the CPG neurons
        //     calcSynapticCurrents( &iExcIntModComEle[iSide][iSeg],    	&pExcModComEle[iSide][iSeg],  	    cellElevator[iSide][iSeg].x,       cellModCom[iSide].spike);
      
            
          //  calcSynapticCurrents( &iExcIntModComDep[iSide][iSeg],    	&pExcModComDep[iSide][iSeg],  	cellDepressor[iSide][iSeg].x,       cellModCom[iSide].spike);

            
     
        //    calcSynapticCurrents( &iExcIntModComSwing[iSide][iSeg],    	&pExcModComSwing[iSide][iSeg],  	cellSwing[iSide][iSeg].x,       cellModCom[iSide].spike);

            
         //   calcSynapticCurrents( &iExcIntModComStance[iSide][iSeg],    &pExcModComStance[iSide][iSeg],  	cellStance[iSide][iSeg].x,       cellModCom[iSide].spike);

            
            // These are the synapses coupling the postural command interneron to the depressor neurons
    //        calcSynapticCurrents( &iExcSegPcnDep[iSide][iSeg],    &pExcSegPcnDep[iSide][iSeg],  	cellPcn[iSide][pitch].x,       spikesPcn[iSide]/mmSeg);
            calcSynapticCurrents( &iExcSegPcnDep[iSide][iSeg],    &pExcSegPcnDep[iSide][iSeg],      cellDepressor[iSide][iSeg].x,       spikesPcn[iSide]/mmSeg);
            //pExcForRet, pExcBackProt, pExcLLFlx, pExcLTExt;
          //  calcSynapticCurrents( &iExcForRet[iSide][iSeg],          &pExcForRet[iSide][iSeg],           cellF[iSide].x,            spikesF[iSide]/mmSeg);  //Walking Command Neurons
            calcSynapticCurrents( &iExcForRet[iSide][iSeg],          &pExcForRet[iSide][iSeg],           cellRetractor[iSide][iSeg].x,            cellF[iSide].spike);  //Walking Command Neurons
            
           // calcSynapticCurrents( &iExcBackProt[iSide][iSeg],    	 &pExcBackProt[iSide][iSeg],         cellB[iSide].x,            spikesB[iSide]/mmSeg);
            calcSynapticCurrents( &iExcBackProt[iSide][iSeg],    	 &pExcBackProt[iSide][iSeg],         cellProtractor[iSide][iSeg].x,            cellB[iSide].spike);
            
           // calcSynapticCurrents( &iExcLLFlx[iSide][iSeg],           &pExcLLFlx[iSide][iSeg],            cellLL[iSide].x,           spikesLL[iSide]/mmSeg);
            calcSynapticCurrents( &iExcLLFlx[iSide][iSeg],           &pExcLLFlx[iSide][iSeg],            cellFlexor[iSide][iSeg].x,           spikesLL[iSide]/mmSeg);

           // calcSynapticCurrents( &iExcLTExt[iSide][iSeg],           &pExcLTExt[iSide][iSeg],            cellLT[iSide].x,           spikesLT[iSide]/mmSeg);
            calcSynapticCurrents( &iExcLTExt[iSide][iSeg],           &pExcLTExt[iSide][iSeg],            cellExtensor[iSide][iSeg].x,           spikesLT[iSide]/mmSeg);

            
          //Directional Command Neuron to ModCom
            calcSynapticCurrents( &iExcForModCom[iSide],       &pExcForModCom[iSide],           cellModCom[iSide].x,            cellF[iSide].spike);  //Walking Command Neurons
            
            calcSynapticCurrents( &iExcBackModCom[iSide],    	 &pExcBackModCom[iSide],         cellModCom[iSide].x,            cellB[iSide].spike);
            
            calcSynapticCurrents( &iExcLLModCom[iSide],           &pExcLLModCom[iSide],            cellModCom[iSide].x,             spikesLL[iSide]/mmSeg);
            
            calcSynapticCurrents( &iExcLTModCom[iSide],           &pExcLTModCom[iSide],            cellModCom[iSide].x,           spikesLT[iSide]/mmSeg);
            

            
            
            // These are the inhibitory synapses between the coordinating neurons and the segmental elevators and the segmental elevators and neighboring coordinating neurons
            if (iSeg == 1){                                     //This section calculates the impact of the Coordinating neurons on the elevators
                
                calcSynapticCurrents( &iInhSegCoordEle[iSide][iSeg],          &pInhSegCoordEle[iSide][iSeg],        cellElevator[iSide][iSeg].x,      spikesElevator[iSide][iSeg]);  //Coordinating Neurons
                calcSynapticCurrents( &iExcSegContEleCoord[iSide][iSeg],      &pExcSegContEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg].x,         spikesCoord[iSide][iSeg]);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntCaudEleCoord[iSide][iSeg],      &pExcIntCaudEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg+1].x,         spikesCoord[iSide][iSeg+1]);  //Coordinating Neurons
            }
            else if ((iSeg ==2)||(iSeg == 3))  {
                calcSynapticCurrents( &iInhSegCoordEle[iSide][iSeg],          &pInhSegCoordEle[iSide][iSeg],        cellElevator[iSide][iSeg].x,      spikesElevator[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcSegContEleCoord[iSide][iSeg],      &pExcSegContEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg].x,         spikesCoord[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntRosEleCoord[iSide][iSeg],       &pExcIntRosEleCoord[iSide][iSeg],     cellCoord[iSide][iSeg-1].x,         spikesCoord[iSide][iSeg-1]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntCaudEleCoord[iSide][iSeg],      &pExcIntCaudEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg+1].x,         spikesCoord[iSide][iSeg+1]/mmSeg);  //Coordinating Neurons
            }
            else if (iSeg == 4){
                calcSynapticCurrents( &iInhSegCoordEle[iSide][iSeg],          &pInhSegCoordEle[iSide][iSeg],        cellElevator[iSide][iSeg].x,      spikesElevator[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcSegContEleCoord[iSide][iSeg],      &pExcSegContEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg].x,         spikesCoord[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntRosEleCoord[iSide][iSeg],       &pExcIntRosEleCoord[iSide][iSeg],     cellCoord[iSide][iSeg-1].x,         spikesCoord[iSide][iSeg-1]/mmSeg);  //Coordinating Neurons
            }
            
            /*
             pInhSegCoordEle[mmSide][mmSeg],                                                                     // Segmental Inhibition from an Coordinating Neuron to an Elevator
             pExcSegContEleCoord[mmSide][mmSeg],                                                                 // Intersegmental Exitation from and elevator to a contralatersl coordinating neuron
             pExcIntRosEleCoord[mmSide][mmSeg],                                                                  // Intersegmental Exitation from and elevator to a rostral ipsilateral coordinating neuron
             pExcIntCaudEleCoord[mmSide][mmSeg],                                                                 // Intersegmental Exitation from and elevator to a caudal ipsilateral coordinating neuron
             */
        } //END for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        
    } // END: for(iSide = 0;iSide < mmSide; ++iSide) ...
    
    
    //    void calcSpikingNeuron(struct structSpiking *ptr,double cIe,double cIi) {
    //    void calcBurstingNeuron(struct structBursting *ptr,double cIe,double cIi)
    
    
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            calcSpikingNeuron(   &cellCoord[iSide][iSeg],      (iExcIntModComEle[iSide][iSeg]+ iExcSegContEleCoord[iSide][iSeg]+iExcIntRosEleCoord[iSide][iSeg]+iExcIntCaudEleCoord[iSide][iSeg]), 0);
          //  calcPacemakerNeuron( &cellElevator[iSide][iSeg],    iExcIntModComEle[iSide][iSeg],      iInhSegCoordEle[iSide][iSeg]);
            calcBurstingNeuron( &cellElevator[iSide][iSeg],    iExcIntModComEle[iSide][iSeg],      iInhSegCoordEle[iSide][iSeg]);
            
            //Be sure to add terms for postural command inputs to the depressors.
            calcPacemakerNeuron(  &cellDepressor[iSide][iSeg],   iExcIntModComDep[iSide][iSeg],      iInhSegEleDep[iSide][iSeg]);  //modify here
            calcPacemakerNeuron(  &cellStance[iSide][iSeg],      iExcIntModComStance[iSide][iSeg],   iInhSegEleStance[iSide][iSeg]);
           
            // calcPacemakerNeuron(  &cellSwing[iSide][iSeg],       iExcIntModComSwing[iSide][iSeg],    iInhSegStanceSwing[iSide][iSeg]);
            calcBurstingNeuron(  &cellSwing[iSide][iSeg],       iExcIntModComSwing[iSide][iSeg],    iInhSegStanceSwing[iSide][iSeg]);
            
            //add iExcBackProt
            calcSpikingNeuron(   &cellProtractor[iSide][iSeg],  iExcSegStanceProt[iSide][iSeg]+ iExcSegSwingProt[iSide][iSeg] + iExcBackProt[iSide][iSeg], 0);
            
            //add iExcForRet
            calcSpikingNeuron(   &cellRetractor[iSide][iSeg],   iExcSegStanceRet[iSide][iSeg] + iExcSegSwingRet[iSide][iSeg] + iExcForRet[iSide][iSeg],  0);
            
            //add iExcLTExt
            calcSpikingNeuron(   &cellExtensor[iSide][iSeg],    iExcSegStanceExt[iSide][iSeg] + iExcSegSwingExt[iSide][iSeg] + iExcLTExt[iSide][iSeg],  0);
            
            //add iExcLLFlx
            calcSpikingNeuron(   &cellFlexor[iSide][iSeg],      iExcSegStanceFlex[iSide][iSeg]+ iExcSegSwingFlex[iSide][iSeg] + iExcLLFlx[iSide][iSeg], 0);
            
            if (iSeg == 1){     //This section calculates the Coordinating neurons                                                                                  ••••••••••••••≤≤≤==This needs to be fixed
                
                calcSpikingNeuron(   &cellCoord[iSide][iSeg],      iExcSegStanceFlex[iSide][iSeg]+ iExcSegSwingFlex[iSide][iSeg], 0);
            }
            else if ((iSeg ==2)||(iSeg == 3))  {
            }
            else if (iSeg == 4){
            }
            
            
        } //end for iSeg
        calcSpikingNeuron(   &cellF[iSide],            0, 0);			//Forward Walking Command
        calcSpikingNeuron(   &cellB[iSide],            0, 0);			//Backward Walking Command
        calcSpikingNeuron(   &cellLL[iSide],           0, 0);			//Lateral Leading Command
        calcSpikingNeuron(   &cellLT[iSide],           0, 0);			//Lateral Trailing Command
        
        ///EDIT MODCOM ???
        calcSpikingNeuron(   &cellModCom[iSide],       iExcForModCom[iSide] + iExcBackModCom[iSide] + iExcLLModCom[iSide] + iExcLTModCom[iSide] , 0);           //Modulatory Command
        calcSpikingNeuron(   &cellPcn[iSide][rosUp],   0, 0);           //There is one on each side for rosDown, pLevel, rosUp
        calcSpikingNeuron(   &cellPcn[iSide][pLevel],  0, 0);
        calcSpikingNeuron(   &cellPcn[iSide][rosDn],   0, 0);
        calcSpikingNeuron(   &cellH[iSide],            0, 0);			//This is the heading command
    } // END: for iside
    t2 = clock();
    elapsed = t2 - t1;
} //End of fun


void allocParamArray(void){
    int i;
    alphaArrayElev = (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayDep = (double **)malloc(sizeof(double *) * mmSide);
    alphaArraySwing = (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayStance = (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayProt = (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayRet = (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayExt = (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayFlex = (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayCoord = (double **)malloc(sizeof(double *) * mmSide);
    
    sigmaArrayElev = (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayDep = (double **)malloc(sizeof(double *) * mmSide);
    sigmaArraySwing = (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayStance = (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayProt = (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayRet = (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayExt = (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayFlex = (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayCoord = (double **)malloc(sizeof(double *) * mmSide);
    
    sigmaEArrayElev = (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayDep = (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArraySwing = (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayStance = (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayProt = (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayRet = (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayExt = (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayFlex = (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayCoord = (double **)malloc(sizeof(double *) * mmSide);
    
    sigmaIArrayElev = (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayDep = (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArraySwing = (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayStance = (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayProt = (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayRet = (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayExt = (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayFlex = (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayCoord = (double **)malloc(sizeof(double *) * mmSide);
    
    betaEArrayElev = (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayDep = (double **)malloc(sizeof(double *) * mmSide);
    betaEArraySwing = (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayStance = (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayProt = (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayRet = (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayExt = (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayFlex = (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayCoord = (double **)malloc(sizeof(double *) * mmSide);
    
    betaIArrayElev = (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayDep = (double **)malloc(sizeof(double *) * mmSide);
    betaIArraySwing = (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayStance = (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayProt = (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayRet = (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayExt = (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayFlex = (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayCoord = (double **)malloc(sizeof(double *) * mmSide);
    
    IdcArrayElev = (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayDep = (double **)malloc(sizeof(double *) * mmSide);
    IdcArraySwing = (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayStance = (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayProt = (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayRet = (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayExt = (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayFlex = (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayCoord = (double **)malloc(sizeof(double *) * mmSide);
    
    alphaArrayF = (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayB= (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayLL= (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayLT= (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayPcn= (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayModCom= (double **)malloc(sizeof(double *) * mmSide);
    alphaArrayH= (double **)malloc(sizeof(double *) * mmSide);
    
    //sigma values
    sigmaArrayF= (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayB= (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayLL= (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayLT= (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayPcn= (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayModCom= (double **)malloc(sizeof(double *) * mmSide);
    sigmaArrayH= (double **)malloc(sizeof(double *) * mmSide);
    
    //sE
    sigmaEArrayF= (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayB= (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayLL= (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayLT= (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayPcn= (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayModCom= (double **)malloc(sizeof(double *) * mmSide);
    sigmaEArrayH= (double **)malloc(sizeof(double *) * mmSide);
    
    //sI
    sigmaIArrayF= (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayB= (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayLL= (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayLT= (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayPcn= (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayModCom= (double **)malloc(sizeof(double *) * mmSide);
    sigmaIArrayH= (double **)malloc(sizeof(double *) * mmSide);
    
    //bE
    betaEArrayF= (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayB= (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayLL= (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayLT= (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayPcn= (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayModCom= (double **)malloc(sizeof(double *) * mmSide);
    betaEArrayH= (double **)malloc(sizeof(double *) * mmSide);
    
    //bI
    betaIArrayF= (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayB= (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayLL= (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayLT= (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayPcn= (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayModCom= (double **)malloc(sizeof(double *) * mmSide);
    betaIArrayH= (double **)malloc(sizeof(double *) * mmSide);
    
    //Idc
    IdcArrayF= (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayB= (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayLL= (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayLT= (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayPcn= (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayModCom= (double **)malloc(sizeof(double *) * mmSide);
    IdcArrayH= (double **)malloc(sizeof(double *) * mmSide);
    
    
    for (i=0; i<mmSide; i++){
        alphaArrayElev[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayDep[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArraySwing[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayStance[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayProt[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayRet[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayExt[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayFlex[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayCoord[i] = (double *)malloc(mmSeg * sizeof(double));
        
        sigmaArrayElev[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayDep[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArraySwing[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayStance[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayProt[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayRet[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayExt[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayFlex[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayCoord[i] = (double *)malloc(mmSeg * sizeof(double));
        
        sigmaEArrayElev[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayDep[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArraySwing[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayStance[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayProt[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayRet[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayExt[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayFlex[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayCoord[i] = (double *)malloc(mmSeg * sizeof(double));
        
        sigmaIArrayElev[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayDep[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArraySwing[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayStance[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayProt[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayRet[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayExt[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayFlex[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayCoord[i] = (double *)malloc(mmSeg * sizeof(double));
        
        betaEArrayElev[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayDep[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArraySwing[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayStance[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayProt[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayRet[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayExt[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayFlex[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayCoord[i] = (double *)malloc(mmSeg * sizeof(double));
        
        betaIArrayElev[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayDep[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArraySwing[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayStance[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayProt[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayRet[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayExt[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayFlex[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayCoord[i] = (double *)malloc(mmSeg * sizeof(double));
        
        IdcArrayElev[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayDep[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArraySwing[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayStance[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayProt[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayRet[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayExt[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayFlex[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayCoord[i] = (double *)malloc(mmSeg * sizeof(double));
    }
    for (i=0; i<mmSide; i++){
        alphaArrayF[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayB[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayLL[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayLT[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayModCom[i] = (double *)malloc(mmSeg * sizeof(double));
        alphaArrayH[i] = (double *)malloc(mmSeg * sizeof(double));
        
        //sigma values
        sigmaArrayF[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayB[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayLL[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayLT[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayModCom[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaArrayH[i] = (double *)malloc(mmSeg * sizeof(double));
        
        //sE
        sigmaEArrayF[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayB[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayLL[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayLT[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayModCom[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaEArrayH[i] = (double *)malloc(mmSeg * sizeof(double));
        
        //sI
        sigmaIArrayF[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayB[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayLL[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayLT[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayModCom[i] = (double *)malloc(mmSeg * sizeof(double));
        sigmaIArrayH[i] = (double *)malloc(mmSeg * sizeof(double));
        
        //bE
        betaEArrayF[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayB[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayLL[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayLT[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayModCom[i] = (double *)malloc(mmSeg * sizeof(double));
        betaEArrayH[i] = (double *)malloc(mmSeg * sizeof(double));
        
        //bI
        betaIArrayF[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayB[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayLL[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayLT[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayModCom[i] = (double *)malloc(mmSeg * sizeof(double));
        betaIArrayH[i] = (double *)malloc(mmSeg * sizeof(double));
        
        //Idc
        IdcArrayF[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayB[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayLL[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayLT[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayModCom[i] = (double *)malloc(mmSeg * sizeof(double));
        IdcArrayH[i] = (double *)malloc(mmSeg * sizeof(double));

    }
    
    for (i=0; i < mmSide; i++){
        alphaArrayPcn[i] = (double *)malloc(pitchStates * sizeof(double));
        sigmaArrayPcn[i] = (double *)malloc(pitchStates * sizeof(double));
        sigmaEArrayPcn[i] = (double *)malloc(pitchStates * sizeof(double));
        sigmaIArrayPcn[i] = (double *)malloc(pitchStates * sizeof(double));
        betaEArrayPcn[i] = (double *)malloc(pitchStates * sizeof(double));
        betaIArrayPcn[i] = (double *)malloc(pitchStates * sizeof(double));
        IdcArrayPcn[i] = (double *)malloc(pitchStates * sizeof(double));
    }
}

void freeParamsArray(void){
    free(alphaArrayElev);
    free( alphaArrayDep);
    free(alphaArraySwing);
    free (alphaArrayStance);
    free (alphaArrayProt);
    free (alphaArrayRet);
    free (alphaArrayExt);
    free (alphaArrayFlex);
    free (alphaArrayCoord);
    
         free(alphaArrayF);
    free( alphaArrayB);
    free(alphaArrayLL);
    free(alphaArrayLT);
    free(alphaArrayPcn);
    free(alphaArrayModCom);
    free(alphaArrayH);
    
    
    
    //sigma values
    free(sigmaArrayElev);
    free(sigmaArrayDep);
    free(sigmaArraySwing);
    free(sigmaArrayStance);
   free(sigmaArrayProt);
    free(sigmaArrayRet);
    free(sigmaArrayExt);
    free(sigmaArrayFlex);
    free(sigmaArrayCoord);
    
    free(sigmaArrayF);
    free(sigmaArrayB);
    free(sigmaArrayLL);
    free(sigmaArrayLT);
    free(sigmaArrayPcn);
    free(sigmaArrayModCom);
    free(sigmaArrayH);
    
    //sE
    free(sigmaEArrayElev);
    free( sigmaEArrayDep);
    free(sigmaEArraySwing);
    free(sigmaEArrayStance);
    free(sigmaEArrayProt);
     free(sigmaEArrayRet);
    free(sigmaEArrayExt);
    free(sigmaEArrayFlex);
    free(sigmaEArrayCoord);
    
    free(sigmaEArrayF);
    free(sigmaEArrayB);
    free(sigmaEArrayLL);
    free(sigmaEArrayLT);
    free(sigmaEArrayPcn);
    free( sigmaEArrayModCom);
    free( sigmaEArrayH);
    
    //sI
    free(sigmaIArrayElev);
         free(sigmaIArrayDep);
    free(sigmaIArraySwing);
    free(sigmaIArrayStance);
    free(sigmaIArrayProt);
    free(sigmaIArrayRet);
    free(sigmaIArrayExt);
    free(sigmaIArrayFlex);
    free(sigmaIArrayCoord);
    
    free(sigmaIArrayF);
    free(sigmaIArrayB);
    free(sigmaIArrayLL);
         free(sigmaIArrayLT);
    free(sigmaIArrayPcn);
         free(sigmaIArrayModCom);
         free(sigmaIArrayH);
    
    //bE
    free(betaEArrayElev);
    free(betaEArrayDep);
    free(betaEArraySwing);
    free(betaEArrayStance);
    free(betaEArrayProt);
    free(betaEArrayRet);
    free(betaEArrayExt);
    free(betaEArrayFlex);
    free(betaEArrayCoord);
    
    free(betaEArrayF);
    free( betaEArrayB);
    free(betaEArrayLL);
    free(betaEArrayLT);
    free(betaEArrayPcn);
    free(betaEArrayModCom);
    free(betaEArrayH);
    
    //bI
    free(betaIArrayElev);
    free(betaIArrayDep);
    free(betaIArraySwing);
    free(betaIArrayStance);
    free(betaIArrayProt);
    free( betaIArrayRet);
    free( betaIArrayExt);
    free(  betaIArrayFlex);
    free( betaIArrayCoord);
    
    free(  betaIArrayF);
    free(  betaIArrayB);
    free( betaIArrayLL);
    free(  betaIArrayLT);
    free( betaIArrayPcn);
    free(  betaIArrayModCom);
    free(betaIArrayH);
    
    //Idc
    free(IdcArrayElev);
    free(  IdcArrayDep);
    free(  IdcArraySwing);
    free( IdcArrayStance);
    free(  IdcArrayProt);
    free(IdcArrayRet);
    free(  IdcArrayExt);
    free(  IdcArrayFlex);
    free( IdcArrayCoord);
    
   free(  IdcArrayF);
   free(  IdcArrayB);
    free(  IdcArrayLL);
    free(  IdcArrayLT);
    free( IdcArrayPcn);
    free(  IdcArrayModCom);
    free(  IdcArrayH);
    
    
}


//------------------------------------------------------------------------------
/*void computeLateralBias(double h, double c)
{
    double error = 0;
    double error1 = 0;
    double error2 = 0;
    double error3 = 0;
    double sig1 = 0;
    double sig2 = 0;
    double RSidc = 0;
    double RSidcL = 0;
    c = c/10;
    error = fabs(c-h);
    error1 = (error-180)*2;
    error2 = error-error1;
    if(error>180){
        sig1 = error2;}
    else{
        sig1 = error;}
    error3 = (h-c);
    if(error3<-180){
        sig2 = error2;}
    else{
        sig2 = error3;}
    if(sig2>180){
        RSidc = (-1*sig1);}
    else{
        RSidc = sig2;}
    RSidcL = (((-1*RSidc)/36) + 18);
    RSidc = ((RSidc/36) + 18);
    
    cellModCom[0].Idc = RSidc;
    cellModCom[2].Idc = 0;
    
    cellModCom[1].Idc = RSidcL;
    
    cellModCom[3].Idc = 0;
}
*/
