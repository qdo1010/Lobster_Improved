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
    ptr->alpha = 4.0;//3.85;     //sets the type of neuron spiking (alpha < 4) or bursting (alphs > 4)
//    ptr->sigma = 0.26;      //sets the baseline state of the neuron (quiet or spiking/ bursting)
    ptr->sigma = 0.46;      //sets the baseline state of the neuron (quiet or spiking/ bursting)
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
    ptr->alpha = 5.5;     //sets the type of neuron spiking (alpha < 4) or bursting (alphs > 4)
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
    ptr->alpha = 5.0;
//    ptr->alpha = 4.60108;
    ptr->sigma = 2-sqrt(ptr->alpha)+0.0171159;
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
}

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
pExcForRet[mmSide][mmSeg], pExcBackProt[mmSide][mmSeg], pExcLLFlx[mmSide][mmSeg], pExcLTExt[mmSide][mmSeg];     //Excitatory Synapses from Directional Commands to propulsive synergies
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
} // end of the Synaptic structure

void calcModulatedCurrents(double *I, //This implements presynaptic inhibition, modParams is the moduatory neuron synapse
                           struct structSynapses *params,struct structSynapses *modParams,
                           double xPost,double spikes, double xModPost, double modSpikes) {
    double iMod;	//This is the synaptic current of the modulatory synapse
    double iMax = 1.0;  //We need a better value of this.
    double iGain;
    
    iMod = modParams->gamma * *I - modParams->gStrength * modSpikes * (xModPost - params->xRp);
    iGain = iMod/iMax;
    *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
    
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
    tmax = 399999; // maximum nuber of iteretions to compute
    //    showMap(1);
    //----- Initialize cells --------
    
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            pacemakerNeuronInit( &cellElevator[iSide][iSeg] );
            pacemakerNeuronInit( &cellSwing[iSide][iSeg] );
            
            burstingNeuronInit(  &cellDepressor[iSide][iSeg] );
            burstingNeuronInit(  &cellStance[iSide][iSeg] );
            
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
/*  pFastExc.xRp = -0.0; pFastExc.gamma = 0.9;      pFastExc.gStrength = 0.05;
    pFastInh.xRp = -2.2; pFastInh.gamma = 0.9;      pFastInh.gStrength = 0.1;
    pSlowExc.xRp = -0.0; pSlowExc.gamma = 0.995;    pSlowExc.gStrength = 0.05;
    pSlowInh.xRp = -2.2; pSlowInh.gamma = 0.995;    pSlowInh.gStrength = 0.1;*/
    
    pFastExc.xRp = -0.0; pFastExc.gamma = 0.9;      pFastExc.gStrength = 0.2;
    pFastInh.xRp = -2.2; pFastInh.gamma = 0.9;      pFastInh.gStrength = 0.2;
    pSlowExc.xRp = -0.0; pSlowExc.gamma = 0.999;    pSlowExc.gStrength = 0.2;
    pSlowInh.xRp = -2.2; pSlowInh.gamma = 0.999;    pSlowInh.gStrength = 0.2;
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

    while((int)mainLoopIndex < (int)tmax) {
        //change to mainLoopIndex <= tmax to stop forever loop
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
        computeLateralBias(LVhead,compin);//(LVhead,0);//compin);
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
        if(mainLoopIndex>10)
        {
            
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
            //previ = i;
                int i;
                xArrayElev  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++)
                    xArrayElev[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                
            
           // xArrayElev[0] = (double *)malloc(sizeof(double) * mmSide * mmSeg* 1000);
            
                xArrayDep  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++)
                    xArrayDep[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
           // xArrayDep[0] = (double *)malloc(sizeof(double) * mmSide * mmSeg*1000);
            
                xArraySwing  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++)
                    xArraySwing[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
           // xArraySwing[0] = (double *)malloc(sizeof(double) * mmSide * mmSeg*1000);
            
                xArrayStance  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++)
                    xArrayStance[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                //xArrayStance[0] = (double *)malloc(sizeof(double) * mmSide * mmSeg*1000);
            
                xArrayProt  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++)
                    xArrayProt[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                //xArrayProt[0] = (double *)malloc(sizeof(double) * mmSide * mmSeg*1000);
            
                xArrayRet  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++)
                    xArrayRet[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                //xArrayRet[0] = (double *)malloc(sizeof(double) * mmSide * mmSeg*1000);
            
                xArrayExt  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++)
                    xArrayExt[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                //xArrayExt[0] = (double *)malloc(sizeof(double) * mmSide * mmSeg*1000);
            
                xArrayFlex  = (double **)malloc(sizeof(double *) * IterNumChosen);
                for (i=0; i<IterNumChosen; i++)
                    xArrayFlex[i] = (double *)malloc(mmSide*mmSeg * sizeof(double));
                //xArrayFlex[0] = (double *)malloc(sizeof(double) * mmSide * mmSeg*1000);
            }
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
                    
                    fprintf(f11," %lf", cellDepressor[iSide][iSeg].x);
                    fprintf(f11,"") ;
                    xArrayDep[ind][indy] = cellDepressor[iSide][iSeg].x; //test
                    
                    fprintf(f12," %lf", cellSwing[iSide][iSeg].x);
                    fprintf(f12," ");
                    xArraySwing[ind][indy] = cellSwing[iSide][iSeg].x; //test
                    
                    fprintf(f13," %lf", cellStance[iSide][iSeg].x);
                    fprintf(f13," ");
                    xArrayStance[ind][indy] = cellStance[iSide][iSeg].x; //test
                    
                    fprintf(f14," %lf", cellProtractor[iSide][iSeg].x);
                    fprintf(f14," ");
                    xArrayProt[ind][indy] = cellProtractor[iSide][iSeg].x; //test
                    
                    fprintf(f15," %lf", cellRetractor[iSide][iSeg].x);
                    fprintf(f15," ");
                    xArrayRet[ind][indy] = cellRetractor[iSide][iSeg].x; //test
                    
                    
                    fprintf(f16," %lf", cellExtensor[iSide][iSeg].x);
                    fprintf(f16," ");
                    xArrayExt[ind][indy] = cellExtensor[iSide][iSeg].x; //test
                    
                    
                    fprintf(f17," %lf", cellFlexor[iSide][iSeg].x);
                    fprintf(f17," ");
                    xArrayFlex[ind][indy] = cellFlexor[iSide][iSeg].x; //test
                    
                    indy++;
                }
            }
            ind++;
            if (ind == IterNumChosen){
                ind = 0;
                sleep(0.5*samplesizechosen); //0.5 is the refresh rate of

            free(xArrayDep);
            free(xArrayExt);
            free(xArrayFlex);
            free(xArrayProt);
            free(xArrayStance);
            free(xArraySwing);
            free(xArrayElev);
            free(xArrayRet);

            }
            
            fprintf(f10, "\n");
            fprintf(f11, "\n");
            fprintf(f12, "\n");
            fprintf(f13, "\n");
            fprintf(f14, "\n");
            fprintf(f15, "\n");
            fprintf(f16, "\n");
            fprintf(f17, "\n");
            
        }
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

// +++++++++++  Function to calculate the right hand sides for ALL maps +++++++++++++++
void computeMAPs(double mainLoopIndex)
{
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
            calcSynapticCurrents( &iInhSegEleDep[iSide][iSeg],    		&pInhSegEleDep[iSide][iSeg],  		cellElevator[iSide][iSeg].x,       spikesElevator[iSide][iSeg]/mmSeg);
            calcSynapticCurrents( &iInhSegEleStance[iSide][iSeg],    	&pInhSegEleStance[iSide][iSeg],  	cellElevator[iSide][iSeg].x,       spikesElevator[iSide][iSeg]/mmSeg);
            calcSynapticCurrents( &iInhSegStanceSwing[iSide][iSeg],    	&pInhSegStanceSwing[iSide][iSeg],  	cellStance[iSide][iSeg].x,         spikesStance[iSide][iSeg]/mmSeg);
            
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
            calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellSwing[iSide][iSeg].x,   cellF[iSide].x,  spikesSwing[iSide][iSeg]/mmSeg, spikesF[iSide]);
            calcModulatedCurrents( &iExcSegSwingRet[iSide][iSeg],    &pExcSegSwingRet[iSide][iSeg],  &pInhIntBSwing[iSide],	 cellSwing[iSide][iSeg].x,   cellB[iSide].x,  spikesSwing[iSide][iSeg]/mmSeg, spikesB[iSide]);
            calcModulatedCurrents( &iExcSegSwingExt[iSide][iSeg],    &pExcSegSwingExt[iSide][iSeg],  &pInhIntLTSwing[iSide], cellSwing[iSide][iSeg].x,   cellLT[iSide].x, spikesSwing[iSide][iSeg]/mmSeg, spikesLT[iSide]);
            calcModulatedCurrents( &iExcSegSwingFlex[iSide][iSeg],   &pExcSegSwingFlx[iSide][iSeg],  &pInhIntLLSwing[iSide], cellSwing[iSide][iSeg].x,   cellLL[iSide].x, spikesSwing[iSide][iSeg]/mmSeg, spikesLL[iSide]);
            
            // These are the synapses coupling the stance interneron to the bifunctional motorneurons
            calcModulatedCurrents( &iExcSegStanceProt[iSide][iSeg],   &pExcSegStanceProt[iSide][iSeg],  &pInhIntFStance[iSide],    	 cellStance[iSide][iSeg].x,  cellF[iSide].x,     spikesStance[iSide][iSeg]/mmSeg, spikesF[iSide]);
            calcModulatedCurrents( &iExcSegStanceRet[iSide][iSeg],	  &pExcSegStanceRet[iSide][iSeg],   &pInhIntBStance[iSide],      cellStance[iSide][iSeg].x,  cellB[iSide].x,     spikesStance[iSide][iSeg]/mmSeg, spikesB[iSide]);
            calcModulatedCurrents( &iExcSegStanceExt[iSide][iSeg],    &pExcSegStanceExt[iSide][iSeg],   &pInhIntLTStance[iSide],     cellStance[iSide][iSeg].x,  cellLT[iSide].x,    spikesStance[iSide][iSeg]/mmSeg, spikesLT[iSide]);
            calcModulatedCurrents( &iExcSegStanceFlex[iSide][iSeg],   &pExcSegStanceFlx[iSide][iSeg],   &pInhIntLLStance[iSide], 	 cellStance[iSide][iSeg].x,  cellLL[iSide].x,    spikesStance[iSide][iSeg]/mmSeg, spikesLL[iSide]);
            
            // These are the synapses coupling the modulatory command interneron to the CPG neurons
            calcSynapticCurrents( &iExcIntModComEle[iSide][iSeg],    	&pExcModComEle[iSide][iSeg],  	    cellModCom[iSide].x,       spikesModCom[iSide]/mmSeg);
            calcSynapticCurrents( &iExcIntModComDep[iSide][iSeg],    	&pExcSegSwingRet[iSide][iSeg],  	cellModCom[iSide].x,       spikesModCom[iSide]/mmSeg);
            calcSynapticCurrents( &iExcIntModComSwing[iSide][iSeg],    	&pExcSegSwingExt[iSide][iSeg],  	cellModCom[iSide].x,       spikesModCom[iSide]/mmSeg);
            calcSynapticCurrents( &iExcIntModComStance[iSide][iSeg],    &pExcSegSwingFlx[iSide][iSeg],  	cellModCom[iSide].x,       spikesModCom[iSide]/mmSeg);
            
            // These are the synapses coupling the postural command interneron to the depressor neurons
            calcSynapticCurrents( &iExcSegPcnDep[iSide][iSeg],    &pExcSegPcnDep[iSide][iSeg],  	cellPcn[iSide][pitch].x,       spikesPcn[iSide]/mmSeg);
            
            //pExcForRet, pExcBackProt, pExcLLFlx, pExcLTExt;
            calcSynapticCurrents( &iExcForRet[iSide][iSeg],          &pExcForRet[iSide][iSeg],           cellF[iSide].x,            spikesF[iSide]/mmSeg);  //Walking Command Neurons
            calcSynapticCurrents( &iExcBackProt[iSide][iSeg],    	 &pExcBackProt[iSide][iSeg],         cellB[iSide].x,            spikesB[iSide]/mmSeg);
            calcSynapticCurrents( &iExcLLFlx[iSide][iSeg],           &pExcLLFlx[iSide][iSeg],            cellLL[iSide].x,           spikesLL[iSide]/mmSeg);
            calcSynapticCurrents( &iExcLTExt[iSide][iSeg],           &pExcLTExt[iSide][iSeg],            cellLT[iSide].x,           spikesLT[iSide]/mmSeg);
            
            // These are the inhibitory synapses between the coordinating neurons and the segmental elevators and the segmental elevators and neighboring coordinating neurons
            if (iSeg == 1){                                     //This section calculates the impact of the Coordinating neurons on the elevators
                calcSynapticCurrents( &iInhSegCoordEle[iSide][iSeg],          &pInhSegCoordEle[iSide][iSeg],        cellElevator[iSide][iSeg].x,      spikesElevator[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcSegContEleCoord[iSide][iSeg],      &pExcSegContEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg].x,         spikesCoord[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntCaudEleCoord[iSide][iSeg],      &pExcIntCaudEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg+1].x,         spikesCoord[iSide][iSeg+1]/mmSeg);  //Coordinating Neurons
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
            calcPacemakerNeuron( &cellElevator[iSide][iSeg],    iExcIntModComEle[iSide][iSeg],      iInhSegCoordEle[iSide][iSeg]);
            //Be sure to add terms for postural command inputs to the depressors.
            calcBurstingNeuron(  &cellDepressor[iSide][iSeg],   iExcIntModComDep[iSide][iSeg],      iInhSegEleDep[iSide][iSeg]);
            calcBurstingNeuron(  &cellStance[iSide][iSeg],      iExcIntModComStance[iSide][iSeg],   iInhSegEleStance[iSide][iSeg]);
            calcPacemakerNeuron(  &cellSwing[iSide][iSeg],       iExcIntModComSwing[iSide][iSeg],    iInhSegStanceSwing[iSide][iSeg]);
            
            calcSpikingNeuron(   &cellProtractor[iSide][iSeg],  iExcSegStanceProt[iSide][iSeg]+ iExcSegSwingProt[iSide][iSeg], 0);
            calcSpikingNeuron(   &cellRetractor[iSide][iSeg],   iExcSegStanceRet[iSide][iSeg] + iExcSegSwingRet[iSide][iSeg],  0);
            calcSpikingNeuron(   &cellExtensor[iSide][iSeg],    iExcSegStanceExt[iSide][iSeg] + iExcSegSwingExt[iSide][iSeg],  0);
            calcSpikingNeuron(   &cellFlexor[iSide][iSeg],      iExcSegStanceFlex[iSide][iSeg]+ iExcSegSwingFlex[iSide][iSeg], 0);
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
        calcSpikingNeuron(   &cellModCom[iSide],       0, 0);           //Modulatory Command
        calcSpikingNeuron(   &cellPcn[iSide][rosUp],   0, 0);           //There is one on each side for rosDown, pLevel, rosUp
        calcSpikingNeuron(   &cellPcn[iSide][pLevel],  0, 0);
        calcSpikingNeuron(   &cellPcn[iSide][rosDn],   0, 0);
        calcSpikingNeuron(   &cellH[iSide],            0, 0);			//This is the heading command
    } // END: for iside
    t2 = clock();
    elapsed = t2 - t1;
} //End of Fun
//------------------------------------------------------------------------------
void computeLateralBias(double h, double c)
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

