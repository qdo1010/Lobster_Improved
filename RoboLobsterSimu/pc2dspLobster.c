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
int speed = 1;
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


// Old initialization Functions, do not work with new structure design.
/*
void spikingNeuronInit(struct structSpiking *ptr) {
    ptr->mu = 0.0005;
    ptr->spike = 0;
    ptr->alpha = 4;
   // ptr->alpha = 4.0;//3.85;     //sets the type of neuron spiking (alpha < 4) or bursting (alphs > 4)
    ptr->sigma = 0;      //sets the baseline state of the neuron (quiet or spiking/ bursting)
   // ptr->sigma = 0.46;      //sets the baseline state of the neuron (quiet or spiking/ bursting)
    
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
}

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
*/

//New functions and required support functions.



void SetSpikeNeuroParam(spikingNeuron *ptr, int speed) {
    ptr->alpha = 4;
    ptr->sigma = 0;
    ptr->mu = 0.0005;
    ptr->spike = 0;
    //sets the type of neuron spiking (alpha < 4) or bursting (alphs > 4)
    //sets the baseline state of the neuron (quiet or spiking/ bursting)
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


void SetBurstNeuroParam(burstingNeuron *ptr, int speed) {
    ptr->alpha = 5.45;
    ptr->sigma = -0.26;
    ptr->mu = 0.0005;
    ptr->spike = 0;
    //sets the type of neuron spiking (alpha < 4) or bursting (alphs > 4)
    //sets the baseline state of the neuron (quiet or spiking/ bursting)
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


void SetPacemakerNeuroParam(pacemakerNeuron *ptr, int speed) {
    ptr->alpha = 4.85;
    ptr->sigma = 2.3 - sqrt(ptr->alpha) + 0.0171159;
    ptr->mu = .0001;
    //   ptr->alpha = 4.;
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

void SetPacemakerNeuroParam2(pacemakerNeuron *ptr, int speed) {
    ptr->alpha = 4.85;
    ptr->sigma = 2.3 - sqrt(ptr->alpha) + 0.0171159;
    ptr->mu = .0001;
    //   ptr->alpha = 4.;
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

void calcSpikingNeuronFunc(spikingNeuron *ptr,double cIe,double cIi) {
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

void calcBurstingNeuronFunc(burstingNeuron *ptr,double cIe,double cIi) {
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

void calcPacemakerNeuronFunc(pacemakerNeuron *ptr,double c, double e) {
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

//These functions are needed to access the new structure properly, you can call the functions in the same way as before
//ie. spikingNeuronInit(  &cellDepressor[iSide][iSeg] , speed); it just takes what you send to the function and then passes
//the inner structure to the function SetSpikeNeuroParam(); so that it functions with the new structure.
//To put it simply, you can use these functions in the exact same way as before, these functions just make sure that works.

void burstingNeuronInit(paramStruct *intermed, int speed) {
    SetBurstNeuroParam(&(intermed->burstingNeuron), speed);
}

void spikingNeuronInit(paramStruct *intermed, int speed) {
    SetSpikeNeuroParam(&(intermed->spikingNeuron), speed);
}

void pacemakerNeuronInit(paramStruct *intermed, int speed) {
    SetPacemakerNeuroParam(&(intermed->pacemakerNeuron), speed);
}

void pacemakerNeuronInit2(paramStruct *intermed, int speed) {
    SetPacemakerNeuroParam2(&(intermed->pacemakerNeuron), speed);
}

void calcSpikingNeuron(paramStruct *intermed, double cIe, double cIi) {
    calcSpikingNeuronFunc(&(intermed->spikingNeuron), cIe, cIi);
}

void calcBurstingNeuron(paramStruct *intermed, double cIe, double cIi) {
    calcBurstingNeuronFunc(&(intermed->burstingNeuron), cIe, cIi);
}

void calcPacemakerNeuron(paramStruct *intermed, double cIe, double cIi) {
    calcPacemakerNeuronFunc(&(intermed->pacemakerNeuron), cIe, cIi);
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

//Old synapse structure design, replaced by structure in .h file.
/*
struct structSynapses {
    double xRp;            //Reversal Potential
    double gamma;          //Time Constant
    double gStrength;      //Synaptic Strength
}
pCustom,  //custom synapse
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
*/
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
void calcSynapticCurrentsFunc(double *I,
                          synapse *params,
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

void calcSynapticCurrents(double *I, paramStruct *intermed, double xPost, double spikes) {
    calcSynapticCurrentsFunc( I, &(intermed->synapse), xPost, spikes);
}

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
/*
//calcModulatedCurrents Based on old structure
void calcModulatedCurrents(double *I, //This implements presynaptic inhibition, modParams is the moduatory neuron synapse
                           struct structSynapses *params,struct structSynapses *modParams,
                           double xPost,double xModPost, double spikes, double modSpikes) {
    double iMod;	//This is the synaptic current of the modulatory synapse
   // double iMax = 1.0;  //We need a better value of this.
   // double iGain;
    
    
    //Step 1: presypnaptic inhibition
    
  //]
    if ((int)spikes == 1){
        iMod = (-modSpikes*modParams->gStrength);
       // iMod = 0;
    }
    else{
//        iMod = (1 - 1000*spikes)*modSpikes*modParams->gStrength; //this makes it so it doesn't go over?
        iMod = (-modSpikes*modParams->gStrength);

        // iMod = (1-2*spikes)*modSpikes;
     //   iMod = 0;
    }
    //iMod = (1 - 4*spikes)*(modSpikes)*modParams->gStrength;
    
  //  iMod2 = *I + modParams->gStrength* modSpikes;
  //  }
  // else{
  //      iMod = 0;
  //  }
        // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
//        *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp) ;        // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) ;
  //  }
   // iMod = modSpikes;
   // iGain = (3 -  modParams->gamma* iMod2)*spikes;
  //  iGain = (1 - iMod/iMax)*spikes ;

    //Step 2: regular Inhibition or i guess regular excitation
   // *I =  iGain;
    
   // *I= params->gamma *iGain //this is the spiking coming from the Command Neurons, it should be fine
   // *I =params->gStrength*iMod*(xPost - params->xRp); //this is the original, which we use
    
    *I = -(-3*iMod*spikes*(xPost - params->xRp)); //this is to test whether we see Inhibition happening asymmetrically
    //.*I =  -params->gamma*spikes*(xPost - params->xRp);
    //.*I = 1;
   // *I = modSpikes;
 //   *I = -params->gamma*spikes*(xPost - params->xRp) - 10*iMod*spikes*(xPost - params->xRp); //this is spiking together
    //RIGHT now, we have the opposite of what we want. It's close tho!!!!!!
 
    
    
    //RIGHT NOW, COMMAND IS TOO HIGH. NEED TO CAP IT AT 0!!
    //THEN, MAKE EVERYTHING OPPOSITE ALSO
    
   // BUT PRETTY COOL?
    //HOW TO REMOVE THE EXTRA EXCITATORY
    
   // *I = params->gamma* iGain - params->gStrength*(iGain)*(xPost - params->xRp);

    
   // iMod = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
    // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
   //
 
    if ((int)modSpikes == 0){
        *I = 0;
    }
    else {
        iMod = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
        // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
        iGain = 1 - iMod;
        *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp) ;        // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) ;
    }
   
     */
    

   /* if ((int)modSpikes == 0){
        *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) ;;
    }
    else {
        *I = modParams->gamma* *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
   // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) ;
    }
    
    
  //  iGain = iMod/iMax;
  //  *I = params->gamma* iMod - params->gStrength* iMod*(xPost - params->xRp);
 //   else{
 //       *I = params->gamma * *I - params->gStrength* spikes * (xPost - params->xRp) + iGain;
 //   }
    //add param gStrengh)
 //   *I = params->gamma * *I - params->gStrength *spikes* (xPost - params->xRp) ;
    
//play around here
    
  //  *I = (params->gamma + modParams->gamma) * *I - (params->gStrength + modParams->gStrength) *(spikes * (xPost - params->xRp) + modSpikes*(xModPost - modParams->xRp));
   // *I = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
    //    calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellProtractor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
    
    //  calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellSwing[iSide][iSeg].x,   cellF[iSide].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
    
} // end of the Modulated Synaptic structure
*/

//calcModulatedCurrents Based on new structure:

void calcModulatedCurrentsFunc(double *I, //This implements presynaptic inhibition, modParams is the moduatory neuron synapse
                               synapse *params, synapse *modParams,
                               double xPost,double xModPost, double spikes, double modSpikes) {
    double iMod;    //This is the synaptic current of the modulatory synapse
    // double iMax = 1.0;  //We need a better value of this.
    // double iGain;
    
    
    //Step 1: presypnaptic inhibition
    
    //]
    if ((int)spikes == 1){
        iMod = (-modSpikes*modParams->gStrength);
        // iMod = 0;
    }
    else{
        //        iMod = (1 - 1000*spikes)*modSpikes*modParams->gStrength; //this makes it so it doesn't go over?
        iMod = (-modSpikes*modParams->gStrength);
        
        // iMod = (1-2*spikes)*modSpikes;
        //   iMod = 0;
    }
    //iMod = (1 - 4*spikes)*(modSpikes)*modParams->gStrength;
    
    //  iMod2 = *I + modParams->gStrength* modSpikes;
    //  }
    // else{
    //      iMod = 0;
    //  }
    // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
    //        *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp) ;        // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) ;
    //  }
    // iMod = modSpikes;
    // iGain = (3 -  modParams->gamma* iMod2)*spikes;
    //  iGain = (1 - iMod/iMax)*spikes ;
    
    //Step 2: regular Inhibition or i guess regular excitation
    // *I =  iGain;
    
    // *I= params->gamma *iGain //this is the spiking coming from the Command Neurons, it should be fine
    // *I =params->gStrength*iMod*(xPost - params->xRp); //this is the original, which we use
    
    *I = -(-3*iMod*spikes*(xPost - params->xRp)); //this is to test whether we see Inhibition happening asymmetrically
    //*I =  -params->gamma*spikes*(xPost - params->xRp);
    //*I = 1;
    // *I = modSpikes;
    //   *I = -params->gamma*spikes*(xPost - params->xRp) - 10*iMod*spikes*(xPost - params->xRp); //this is spiking together
    //RIGHT now, we have the opposite of what we want. It's close tho!!!!!!
    
    
    
    //RIGHT NOW, COMMAND IS TOO HIGH. NEED TO CAP IT AT 0!!
    //THEN, MAKE EVERYTHING OPPOSITE ALSO
    
    // BUT PRETTY COOL?
    //HOW TO REMOVE THE EXTRA EXCITATORY
    
    // *I = params->gamma* iGain - params->gStrength*(iGain)*(xPost - params->xRp);
    
    
    // iMod = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
    // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
    //
    /*
     if ((int)modSpikes == 0){
     *I = 0;
     }
     else {
     iMod = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
     // *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp);
     iGain = 1 - iMod;
     *I = params->gamma * *I - iGain * spikes * (xPost - params->xRp) ;        // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) ;
     }
     
     */
    
    
    /* if ((int)modSpikes == 0){
     *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) ;;
     }
     else {
     *I = modParams->gamma* *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
     // *I = params->gamma * *I - params->gStrength * spikes * (xPost - params->xRp) ;
     }
     
     */
    //  iGain = iMod/iMax;
    //  *I = params->gamma* iMod - params->gStrength* iMod*(xPost - params->xRp);
    //   else{
    //       *I = params->gamma * *I - params->gStrength* spikes * (xPost - params->xRp) + iGain;
    //   }
    //add param gStrengh)
    //   *I = params->gamma * *I - params->gStrength *spikes* (xPost - params->xRp) ;
    
    //play around here
    
    //  *I = (params->gamma + modParams->gamma) * *I - (params->gStrength + modParams->gStrength) *(spikes * (xPost - params->xRp) + modSpikes*(xModPost - modParams->xRp));
    // *I = modParams->gamma * *I - modParams->gStrength* modSpikes * (xModPost - modParams->xRp);
    //    calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellProtractor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
    
    //  calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellSwing[iSide][iSeg].x,   cellF[iSide].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
    
} // end of the Modulated Synaptic structure

void calcModulatedCurrents(double *I, paramStruct *params, paramStruct *modParams, double xPost, double xModPost, double spikes, double modSpikes) {
    calcModulatedCurrentsFunc( I, &(params->synapse), &(modParams->synapse), xPost, xModPost, spikes, modSpikes);
}

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

//param file
FILE *paramFile; //synapse
FILE *NparamFile; //neuron
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
    
        //alloc memory for synapse params
        globalSynapseName = malloc(sizeof(int));
        globalgStrength = malloc(sizeof(double));
        globalGamma = malloc(sizeof(double));
        globalXrp = malloc(sizeof(double));
    
    //This will check if this is the first time the code is runnins and no parameter file exists and if
    //so it will create a file with the parameters made based off the set values in the initialize functions.
    //If a parameter file exists it will instead load that file and set all neaurons and synapses based off the
    //values in the file.
    
    
     
    if( access( "params.dat", F_OK ) != -1 ) {
        LoadAllParams ();
        printf("Parameter file found, loading now...\n");
    } else {
        CreateParamFile ();
        printf("No parameter file found, creating one now...\n");
    }

    
    
    /*
        for(iSide = 0;iSide < mmSide; ++iSide)
        {
            for(iSeg = 0;iSeg < mmSeg; ++iSeg)
            {
               // pacemakerNeuronInit( &cellElevator[iSide][iSeg] );
               // pacemakerNeuronInit( &cellSwing[iSide][iSeg] );
                //make it bursting for now :(
                burstingNeuronInit( &cellElevator[iSide][iSeg] , speed);
                burstingNeuronInit( &cellSwing[iSide][iSeg] , speed);
                
                
                pacemakerNeuronInit2(  &cellDepressor[iSide][iSeg] , speed);
                pacemakerNeuronInit(  &cellStance[iSide][iSeg] , speed);
                
                spikingNeuronInit(   &cellProtractor[iSide][iSeg] , speed);
                spikingNeuronInit(   &cellRetractor[iSide][iSeg] , speed);
                spikingNeuronInit(   &cellExtensor[iSide][iSeg] , speed);
                spikingNeuronInit(   &cellFlexor[iSide][iSeg] , speed);
                spikingNeuronInit(   &cellCoord[iSide][iSeg] , speed);
                
                
                
            } //END for (iSeg = 0;iSeg < mmSeg; ++iSeg)
            spikingNeuronInit(&cellPcn[iSide][pLevel], speed);
            spikingNeuronInit(&cellModCom[iSide], speed);
            spikingNeuronInit(&cellH[iSide], speed);
            spikingNeuronInit(&cellF[iSide], speed);
            spikingNeuronInit(&cellB[iSide], speed);
            spikingNeuronInit(&cellLL[iSide], speed);
            spikingNeuronInit(&cellLT[iSide], speed);
            
        }
        
        */
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
    
    ///you can change it here
    //Synapses based on old structure
    /*
    pFastExc.xRp = -0.0; pFastExc.gamma = 0.9;      pFastExc.gStrength = 0.1;
    pFastInh.xRp = -2.2; pFastInh.gamma = 0.9;      pFastInh.gStrength = 1;
    pSlowExc.xRp = 2.2; pSlowExc.gamma = 0.995;    pSlowExc.gStrength = 1;
    pSlowInh.xRp = -2.2; pSlowInh.gamma = 0.995;    pSlowInh.gStrength = 1;
    
    //Synapses based on new Strucure
    pFastExc.synapse.xRp = -0.0; pFastExc.synapse.gamma = 0.9;      pFastExc.synapse.gStrength = 0.1;
    pFastInh.synapse.xRp = -2.2; pFastInh.synapse.gamma = 0.9;      pFastInh.synapse.gStrength = 1;
    pSlowExc.synapse.xRp = 2.2; pSlowExc.synapse.gamma = 0.995;    pSlowExc.synapse.gStrength = 1;
    pSlowInh.synapse.xRp = -2.2; pSlowInh.synapse.gamma = 0.995;    pSlowInh.synapse.gStrength = 1;
//•••••••••••••••••••••
    //TODO: tune individual synapse!
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
           // °°°°°°°°°°°°We need to integrate these initializations with the speed control
           // Inject some noise into the CPG neurons
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellElevator[iSide][iSeg].burstingNeuron.sigma = cellElevator[iSide][iSeg].burstingNeuron.sigma + R * 0.001 + 0.0005;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellDepressor[iSide][iSeg].pacemakerNeuron.sigma = cellDepressor[iSide][iSeg].pacemakerNeuron.sigma + R * 0.0001 - 0.0195;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellSwing[iSide][iSeg].burstingNeuron.sigma = cellSwing[iSide][iSeg].burstingNeuron.sigma + R * 0.0001 - 0.0195;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellStance[iSide][iSeg].pacemakerNeuron.sigma = cellStance[iSide][iSeg].pacemakerNeuron.sigma + R * 0.0001 - 0.0195;
            
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
            
            //Based on old stucture
            /*
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
    
            //Based on new structure
            switch(pitch) {   //This sets up the gradients of synaptic strength between the pitch command neuron Pcn and the depresssor motor neurons for each segment
                    
                case pLow:
                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                    break;
                    
                case pLevel:
                    switch (roll){
                        case leftDown:{
                            switch (iSide){
                                case left:
                                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                                case right:
                                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                            }
                        case rightDown:
                            switch (iSide){
                                case left:
                                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                                case right:
                                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                            }
                        case rLevel:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                        }
                    }
                    
                case pHigh:
                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.95;
                    break;
                    
                case rosDn:{
                    switch (iSeg){
                        case 0:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.25;
                        case 1:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                        case 2:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                        case 3:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.95;
                            
                    }
                    
                case rosUp  :
                    switch (iSeg){
                        case 0:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.95;
                        case 1:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                        case 2:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                        case 3:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.25;
                    }
                    break;
                }
            }
        }
    }
    */
    
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
   /*f10 = fopen("timeElev.txt","w");
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
        
    //fwrite for the new structure
        /*
    if (writeToFile == 1){
        FILE *paramFile;
        paramFile = fopen ("params.dat", "w");
        if (paramFile == NULL)
        {
            fprintf(stderr, "\nError opend file\n");
            exit (1);
        }
        for(iSide = 0;iSide < mmSide; ++iSide) {
            fwrite (&pInhIntFSwing[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhIntFStance[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhIntBSwing[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhIntBStance[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhIntLLSwing[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhIntLLStance[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhIntLTSwing[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhIntLTStance[iSide], sizeof(paramStruct), 1, paramFile);
            for(iSeg = 0;iSeg < mmSeg; ++iSeg)
            {
                fwrite (&pExcSegPcnDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcIntRosEleCoord[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcIntRCaudEleCoord[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegEleContraLat[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pInhSegEleDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pInhSegEleStance[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pInhSegStanceSwing[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegStanceProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegStanceRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegStanceExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegStanceFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegSwingProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegSwingRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegSwingExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegSwingFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcForRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcBackProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcLLFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcLTExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcForModCom[iSide], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcBackModCom[iSide], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcLLModCom[iSide], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcLTModCom[iSide], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcModComEle[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcModComDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcModComSwing[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcModComStance[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
                fwrite (&pExcSegPcnDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            }
        }
        
        
        // close file
        fclose (paramFile);
    }
        
        if (writeToFile == 1){
            printf("saving files\n");
            paramFile = fopen("params.dat", "w");
            for(iSide = 0;iSide < mmSide; ++iSide)  //This loop initializes the parameters for synapses
            {
                // Presynaptic Inhibition from a command to bifunctional interneuron synapse
                fprintf(paramFile,"pInhIntFSwing,");
                fprintf(paramFile,"side%d,",iSide);
                fprintf(paramFile,"%f,",pInhIntFSwing[iSide].gamma);
                fprintf(paramFile,"%f,",pInhIntFSwing[iSide].xRp);
                fprintf(paramFile,"%f\n",pInhIntFSwing[iSide].gStrength);

                fprintf(paramFile,"pInhIntFStance,");
                fprintf(paramFile,"side%d,",iSide);
                fprintf(paramFile,"%f,",pInhIntFStance[iSide].gamma);
                fprintf(paramFile,"%f,",pInhIntFStance[iSide].xRp);
                fprintf(paramFile,"%f\n",pInhIntFStance[iSide].gStrength);

                fprintf(paramFile,"pInhIntBSwing,");
                fprintf(paramFile,"side%d,",iSide);
                fprintf(paramFile,"%f,",pInhIntBSwing[iSide].gamma);
                fprintf(paramFile,"%f,",pInhIntBSwing[iSide].xRp);
                fprintf(paramFile,"%f\n",pInhIntBSwing[iSide].gStrength);

                fprintf(paramFile,"pInhIntBStance,");
                fprintf(paramFile,"side%d,",iSide);
                fprintf(paramFile,"%f,",pInhIntBStance[iSide].gamma);
                fprintf(paramFile,"%f,",pInhIntBStance[iSide].xRp);
                fprintf(paramFile,"%f\n",pInhIntBStance[iSide].gStrength);

                fprintf(paramFile,"pInhIntLLSwing,");
                fprintf(paramFile,"side%d,",iSide);
                fprintf(paramFile,"%f,",pInhIntLLSwing[iSide].gamma);
                fprintf(paramFile,"%f,",pInhIntLLSwing[iSide].xRp);
                fprintf(paramFile,"%f\n",pInhIntLLSwing[iSide].gStrength);
                
                fprintf(paramFile,"pInhIntLLStance,");
                fprintf(paramFile,"side%d,",iSide);
                fprintf(paramFile,"%f,",pInhIntLLStance[iSide].gamma);
                fprintf(paramFile,"%f,",pInhIntLLStance[iSide].xRp);
                fprintf(paramFile,"%f\n",pInhIntLLStance[iSide].gStrength);

                fprintf(paramFile,"pInhIntLTSwing,");
                fprintf(paramFile,"side%d,",iSide);
                fprintf(paramFile,"%f,",pInhIntLTSwing[iSide].gamma);
                fprintf(paramFile,"%f,",pInhIntLTSwing[iSide].xRp);
                fprintf(paramFile,"%f\n",pInhIntLTSwing[iSide].gStrength);

                fprintf(paramFile,"pInhIntLTStance,");
                fprintf(paramFile,"side%d,",iSide);
                fprintf(paramFile,"%f,",pInhIntLTStance[iSide].gamma);
                fprintf(paramFile,"%f,",pInhIntLTStance[iSide].xRp);
                fprintf(paramFile,"%f\n",pInhIntLTStance[iSide].gStrength);
                //Now loop down the segments
                for(iSeg = 0;iSeg < mmSeg; ++iSeg)
                {
                    //------- Set the parameters for synapses {xRp and gamma} ---------
                    
                    //Synapse between pitch Command and segmental depressors
                    fprintf(paramFile,"pExcSegPcnDep,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegPcnDep[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegPcnDep[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegPcnDep[iSide][iSeg].gStrength);
                    
                    // Excitory synapses between elevator synergies and ajacent coordinating neurons
                    fprintf(paramFile,"pExcIntRosEleCoord,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcIntRosEleCoord[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcIntRosEleCoord[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcIntRosEleCoord[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcIntRCaudEleCoord,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcIntRCaudEleCoord[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcIntRCaudEleCoord[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcIntRCaudEleCoord[iSide][iSeg].gStrength);

                    fprintf(paramFile,"pExcSegEleContraLat,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegEleContraLat[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegEleContraLat[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegEleContraLat[iSide][iSeg].gStrength);

                    fprintf(paramFile,"pInhSegEleDep,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pInhSegEleDep[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pInhSegEleDep[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pInhSegEleDep[iSide][iSeg].gStrength);
                    //Internal inhibitory synapses of neuronal oscillator

                    fprintf(paramFile,"pInhSegEleStance,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pInhSegEleStance[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pInhSegEleStance[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pInhSegEleStance[iSide][iSeg].gStrength);

                    fprintf(paramFile,"pInhSegStanceSwing,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pInhSegStanceSwing[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pInhSegStanceSwing[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pInhSegStanceSwing[iSide][iSeg].gStrength);
                    
                    // Excitatory Synapses from Swing/Stance interneruons to bifunctional synapses
                    fprintf(paramFile,"pExcSegStanceProt,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegStanceProt[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegStanceProt[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegStanceProt[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcSegStanceRet,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegStanceRet[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegStanceRet[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegStanceRet[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcSegStanceExt,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegStanceExt[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegStanceExt[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegStanceExt[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcSegStanceFlx,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegStanceFlx[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegStanceFlx[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegStanceFlx[iSide][iSeg].gStrength);

                    fprintf(paramFile,"pExcSegSwingProt,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegSwingProt[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegSwingProt[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegSwingProt[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcSegSwingRet,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegSwingRet[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegSwingRet[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegSwingRet[iSide][iSeg].gStrength);

                    fprintf(paramFile,"pExcSegSwingExt,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegSwingExt[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegSwingExt[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegSwingExt[iSide][iSeg].gStrength);

                    fprintf(paramFile,"pExcSegSwingFlx,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegSwingFlx[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegSwingFlx[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegSwingFlx[iSide][iSeg].gStrength);
                    
                    // Recruiting excitatory synapses from Walking commands to propulsive synergies
                    fprintf(paramFile,"pExcForRet,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcForRet[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcForRet[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcForRet[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcBackProt,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcBackProt[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcBackProt[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcBackProt[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcLLFlx,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcLLFlx[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcLLFlx[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcLLFlx[iSide][iSeg].gStrength);

                    fprintf(paramFile,"pExcLTExt,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcLTExt[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcLTExt[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcLTExt[iSide][iSeg].gStrength);
                    
                    
                    // Recruiting excitatory synapses from Walking commands to ModCom
                    fprintf(paramFile,"pExcForModCom,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcForModCom[iSide].gamma);
                    fprintf(paramFile,"%f,",pExcForModCom[iSide].xRp);
                    fprintf(paramFile,"%f\n",pExcForModCom[iSide].gStrength);
                    
                    fprintf(paramFile,"pExcBackModCom,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcBackModCom[iSide].gamma);
                    fprintf(paramFile,"%f,",pExcBackModCom[iSide].xRp);
                    fprintf(paramFile,"%f\n",pExcBackModCom[iSide].gStrength);
         
                    fprintf(paramFile,"pExcLLModCom,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcLLModCom[iSide].gamma);
                    fprintf(paramFile,"%f,",pExcLLModCom[iSide].xRp);
                    fprintf(paramFile,"%f\n",pExcLLModCom[iSide].gStrength);

                    fprintf(paramFile,"pExcLTModCom,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcLTModCom[iSide].gamma);
                    fprintf(paramFile,"%f,",pExcLTModCom[iSide].xRp);
                    fprintf(paramFile,"%f\n",pExcLTModCom[iSide].gStrength);
                    
                    //Excitatory synapses from modulatory Commands to CPG Neurons
                    fprintf(paramFile,"pExcModComEle,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcModComEle[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcModComEle[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcModComEle[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcModComDep,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcModComDep[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcModComDep[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcModComDep[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcModComSwing,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcModComSwing[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcModComSwing[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcModComSwing[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcModComStance,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcModComStance[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcModComStance[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcModComStance[iSide][iSeg].gStrength);
                    
                    fprintf(paramFile,"pExcSegPcnDep,");
                    fprintf(paramFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(paramFile,"%f,",pExcSegPcnDep[iSide][iSeg].gamma);
                    fprintf(paramFile,"%f,",pExcSegPcnDep[iSide][iSeg].xRp);
                    fprintf(paramFile,"%f\n",pExcSegPcnDep[iSide][iSeg].gStrength);
                    }
            }
            writeToFile = 0;
            fclose(paramFile);
        }
        */
        
        
        if (beginEditingParams == 1){ //if a edit flag ever been set to start edit neurons param
            
            //set multiple neuron here!
            setMultipleNeuronParams(globalCellName, globalSide, globalSeg, globalAlpha, globalSigma, globalSigmaE, globalSigmaI, globalBetaE, globalBetaI, globalIdc, globalSize);
            //After all neurons are edited the new parameters are saved to the parameter file to be loaded next time.
            SaveAllParams();
        }
        
        if (beginEditingSynapse == 1){
            setMultipleSynapseParams(globalSynapseName, globalSide, globalSeg, globalXrp, globalGamma, globalgStrength, globalSize);
            
             //After all synapses are edited the new parameters are saved to the parameter file to be loaded next time.
            SaveAllParams();
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
    //  if(mainLoopIndex>10)
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
    
        
        //Old structure
        /*
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
        
         */
        
        
        
         //New structure version
        
        int indy = 0;
        //printf("%f", mainLoopIndex);
        for(iSide = 0;iSide < mmSide; ++iSide)
        {
            for(iSeg = 0;iSeg < mmSeg; ++iSeg)
            {
                //printf("%f",mainLoopIndex);
                fprintf(f10," %lf", cellElevator[iSide][iSeg].burstingNeuron.x);
                fprintf(f10," ");
                xArrayElev[ind][indy] = cellElevator[iSide][iSeg].burstingNeuron.x; //test
                alphaArrayElev[iSide][iSeg]= cellElevator[iSide][iSeg].burstingNeuron.alpha;
                sigmaArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].burstingNeuron.sigma;
                sigmaIArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].burstingNeuron.sigmaI;
                sigmaEArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].burstingNeuron.sigmaE;
                betaEArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].burstingNeuron.betaE;
                betaIArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].burstingNeuron.betaI;
                IdcArrayElev[iSide][iSeg] = cellElevator[iSide][iSeg].burstingNeuron.Idc;
                
                fprintf(f11," %lf", cellDepressor[iSide][iSeg].pacemakerNeuron.x);
                fprintf(f11,"") ;
                xArrayDep[ind][indy] = cellDepressor[iSide][iSeg].pacemakerNeuron.x; //test
                alphaArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].pacemakerNeuron.alpha;
                sigmaArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].pacemakerNeuron.sigma;
                sigmaIArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].pacemakerNeuron.sigmaI;
                sigmaEArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].pacemakerNeuron.sigmaE;
                betaEArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].pacemakerNeuron.betaE;
                betaIArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].pacemakerNeuron.betaI;
                IdcArrayDep[iSide][iSeg] = cellDepressor[iSide][iSeg].pacemakerNeuron.Idc;
                
                fprintf(f12," %lf", cellSwing[iSide][iSeg].burstingNeuron.x);
                fprintf(f12," ");
                xArraySwing[ind][indy] = cellSwing[iSide][iSeg].burstingNeuron.x; //test
                alphaArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].burstingNeuron.alpha;
                sigmaArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].burstingNeuron.sigma;
                sigmaIArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].burstingNeuron.sigmaI;
                sigmaEArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].burstingNeuron.sigmaE;
                betaEArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].burstingNeuron.betaE;
                betaIArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].burstingNeuron.betaI;
                IdcArraySwing[iSide][iSeg] = cellSwing[iSide][iSeg].burstingNeuron.Idc;
                
                fprintf(f13," %lf", cellStance[iSide][iSeg].pacemakerNeuron.x);
                fprintf(f13," ");
                xArrayStance[ind][indy] = cellStance[iSide][iSeg].pacemakerNeuron.x; //test
                alphaArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].pacemakerNeuron.alpha;
                sigmaArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].pacemakerNeuron.sigma;
                sigmaIArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].pacemakerNeuron.sigmaI;
                sigmaEArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].pacemakerNeuron.sigmaE;
                betaEArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].pacemakerNeuron.betaE;
                betaIArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].pacemakerNeuron.betaI;
                IdcArrayStance[iSide][iSeg] = cellStance[iSide][iSeg].pacemakerNeuron.Idc;
                
                fprintf(f14," %lf", cellProtractor[iSide][iSeg].spikingNeuron.x);
                fprintf(f14," ");
                xArrayProt[ind][indy] = cellProtractor[iSide][iSeg].spikingNeuron.x; //test
                alphaArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].spikingNeuron.alpha;
                sigmaArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].spikingNeuron.sigma;
                sigmaIArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].spikingNeuron.sigmaI;
                sigmaEArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].spikingNeuron.sigmaE;
                betaEArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].spikingNeuron.betaE;
                betaIArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].spikingNeuron.betaI;
                IdcArrayProt[iSide][iSeg] = cellProtractor[iSide][iSeg].spikingNeuron.Idc;
                
                fprintf(f15," %lf", cellRetractor[iSide][iSeg].spikingNeuron.x);
                fprintf(f15," ");
                xArrayRet[ind][indy] = cellRetractor[iSide][iSeg].spikingNeuron.x; //test
                alphaArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].spikingNeuron.alpha;
                sigmaArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].spikingNeuron.sigma;
                sigmaIArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].spikingNeuron.sigmaI;
                sigmaEArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].spikingNeuron.sigmaE;
                betaEArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].spikingNeuron.betaE;
                betaIArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].spikingNeuron.betaI;
                IdcArrayRet[iSide][iSeg] = cellRetractor[iSide][iSeg].spikingNeuron.Idc;
                
                fprintf(f16," %lf", cellExtensor[iSide][iSeg].spikingNeuron.x);
                fprintf(f16," ");
                xArrayExt[ind][indy] = cellExtensor[iSide][iSeg].spikingNeuron.x; //test
                alphaArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].spikingNeuron.alpha;
                sigmaArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].spikingNeuron.sigma;
                sigmaIArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].spikingNeuron.sigmaI;
                sigmaEArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].spikingNeuron.sigmaE;
                betaEArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].spikingNeuron.betaE;
                betaIArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].spikingNeuron.betaI;
                IdcArrayExt[iSide][iSeg] = cellExtensor[iSide][iSeg].spikingNeuron.Idc;
                
                fprintf(f17," %lf", cellFlexor[iSide][iSeg].spikingNeuron.x);
                fprintf(f17," ");
                xArrayFlex[ind][indy] = cellFlexor[iSide][iSeg].spikingNeuron.x; //test
                alphaArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].spikingNeuron.alpha;
                sigmaArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].spikingNeuron.sigma;
                sigmaIArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].spikingNeuron.sigmaI;
                sigmaEArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].spikingNeuron.sigmaE;
                betaEArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].spikingNeuron.betaE;
                betaIArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].spikingNeuron.betaI;
                IdcArrayFlex[iSide][iSeg] = cellFlexor[iSide][iSeg].spikingNeuron.Idc;
                
                xArrayCoord[ind][indy] = cellCoord[iSide][iSeg].spikingNeuron.x; //test
                alphaArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].spikingNeuron.alpha;
                sigmaArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].spikingNeuron.sigma;
                sigmaIArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].spikingNeuron.sigmaI;
                sigmaEArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].spikingNeuron.sigmaE;
                betaEArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].spikingNeuron.betaE;
                betaIArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].spikingNeuron.betaI;
                IdcArrayCoord[iSide][iSeg] = cellCoord[iSide][iSeg].spikingNeuron.Idc;
                indy++;
                
            }
        }
        
        int indy2 = 0;
        ///do the command neuron here
        for(iSide = 0;iSide < mmSide; ++iSide)
        {
            xArrayF[ind][indy2] = cellF[iSide].spikingNeuron.x;
            alphaArrayF[iSide][0] = cellF[iSide].spikingNeuron.alpha;
            sigmaArrayF[iSide][0] = cellF[iSide].spikingNeuron.sigma;
            sigmaIArrayF[iSide][0] = cellF[iSide].spikingNeuron.sigmaI;
            sigmaEArrayF[iSide][0] = cellF[iSide].spikingNeuron.sigmaE;
            betaEArrayF[iSide][0] = cellF[iSide].spikingNeuron.betaE;
            betaIArrayF[iSide][0] = cellF[iSide].spikingNeuron.betaI;
            IdcArrayF[iSide][0] = cellF[iSide].spikingNeuron.Idc;
            
            
            xArrayB[ind][indy2] = cellB[iSide].spikingNeuron.x;
            alphaArrayB[iSide][0] = cellB[iSide].spikingNeuron.alpha;
            sigmaArrayB[iSide][0] = cellB[iSide].spikingNeuron.sigma;
            sigmaIArrayB[iSide][0] = cellB[iSide].spikingNeuron.sigmaI;
            sigmaEArrayB[iSide][0] = cellB[iSide].spikingNeuron.sigmaE;
            betaEArrayB[iSide][0] = cellB[iSide].spikingNeuron.betaE;
            betaIArrayB[iSide][0] = cellB[iSide].spikingNeuron.betaI;
            IdcArrayB[iSide][0] = cellB[iSide].spikingNeuron.Idc;
            
            xArrayLL[ind][indy2] = cellLL[iSide].spikingNeuron.x;
            alphaArrayLL[iSide][0] = cellLL[iSide].spikingNeuron.alpha;
            sigmaArrayLL[iSide][0] = cellLL[iSide].spikingNeuron.sigma;
            sigmaIArrayLL[iSide][0] = cellLL[iSide].spikingNeuron.sigmaI;
            sigmaEArrayLL[iSide][0] = cellLL[iSide].spikingNeuron.sigmaE;
            betaEArrayLL[iSide][0] = cellLL[iSide].spikingNeuron.betaE;
            betaIArrayLL[iSide][0] = cellLL[iSide].spikingNeuron.betaI;
            IdcArrayLL[iSide][0] = cellLL[iSide].spikingNeuron.Idc;
            
            
            xArrayLT[ind][indy2] = cellLT[iSide].spikingNeuron.x;
            alphaArrayLT[iSide][0] = cellLT[iSide].spikingNeuron.alpha;
            sigmaArrayLT[iSide][0] = cellLT[iSide].spikingNeuron.sigma;
            sigmaIArrayLT[iSide][0] = cellLT[iSide].spikingNeuron.sigmaI;
            sigmaEArrayLT[iSide][0] = cellLT[iSide].spikingNeuron.sigmaE;
            betaEArrayLT[iSide][0] = cellLT[iSide].spikingNeuron.betaE;
            betaIArrayLT[iSide][0] = cellLT[iSide].spikingNeuron.betaI;
            IdcArrayLT[iSide][0] = cellLT[iSide].spikingNeuron.Idc;
            
            xArrayModCom[ind][indy2] = cellModCom[iSide].spikingNeuron.x;
            alphaArrayModCom[iSide][0] = cellModCom[iSide].spikingNeuron.alpha;
            sigmaArrayModCom[iSide][0] = cellModCom[iSide].spikingNeuron.sigma;
            sigmaIArrayModCom[iSide][0] = cellModCom[iSide].spikingNeuron.sigmaI;
            sigmaEArrayModCom[iSide][0] = cellModCom[iSide].spikingNeuron.sigmaE;
            betaEArrayModCom[iSide][0] = cellModCom[iSide].spikingNeuron.betaE;
            betaIArrayModCom[iSide][0] = cellModCom[iSide].spikingNeuron.betaI;
            IdcArrayModCom[iSide][0] = cellModCom[iSide].spikingNeuron.Idc;
            
            xArrayH[ind][indy2] = cellH[iSide].spikingNeuron.x;
            alphaArrayH[iSide][0] = cellH[iSide].spikingNeuron.alpha;
            sigmaArrayH[iSide][0] = cellH[iSide].spikingNeuron.sigma;
            sigmaIArrayH[iSide][0] = cellH[iSide].spikingNeuron.sigmaI;
            sigmaEArrayH[iSide][0] = cellH[iSide].spikingNeuron.sigmaE;
            betaEArrayH[iSide][0] = cellH[iSide].spikingNeuron.betaE;
            betaIArrayH[iSide][0] = cellH[iSide].spikingNeuron.betaI;
            IdcArrayH[iSide][0] = cellH[iSide].spikingNeuron.Idc;
            
            indy2++;
        }
        
        
        int indy3 = 0;
        //do the Pcn neuron here, cause it's a special case
        for(iSide = 0;iSide < mmSide; ++iSide)
        {
            for (iSeg =0; iSeg < pitchStates; ++iSeg){
                xArrayPcn[ind][indy3] = cellPcn[iSide][iSeg].spikingNeuron.x;
                alphaArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].spikingNeuron.alpha;
                sigmaArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].spikingNeuron.sigma;
                sigmaIArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].spikingNeuron.sigmaI;
                sigmaEArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].spikingNeuron.sigmaE;
                betaEArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].spikingNeuron.betaE;
                betaIArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].spikingNeuron.betaI;
                IdcArrayPcn[iSide][iSeg] = cellPcn[iSide][iSeg].spikingNeuron.Idc;
                
                indy3++;
            }
            
        }

        
            ind++;
            if (ind == IterNumChosen-1){
                ind = 0;
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
            fprintf(f18," %lf", cellPcn[jj][pitch].spikingNeuron.x);//f13 now Mot and and f14 is Pcn
        }
        fprintf(f18,"\n");
        
        // printf("%3f \n", mainLoopIndex);
        
        /*
        
        if (writeToFile){
            FILE *NparamFile;
            NparamFile = fopen("neuronparams.dat", "w");
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                for(iSeg = 0;iSeg < mmSeg; ++iSeg)
                {
                    fwrite (&cellElevator[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    fwrite (&cellDepressor[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    fwrite (&cellSwing[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    fwrite (&cellStance[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    fwrite (&cellProtractor[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    fwrite (&cellRetractor[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    fwrite (&cellExtensor[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    fwrite (&cellFlexor[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    fwrite (&cellCoord[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                    
                }
            }
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                fwrite (&cellF[iSide], sizeof(paramStruct), 1, NparamFile);
                fwrite (&cellB[iSide], sizeof(paramStruct), 1, NparamFile);
                fwrite (&cellLL[iSide], sizeof(paramStruct), 1, NparamFile);
                fwrite (&cellLT[iSide], sizeof(paramStruct), 1, NparamFile);
                fwrite (&cellModCom[iSide], sizeof(paramStruct), 1, NparamFile);
                fwrite (&cellH[iSide], sizeof(paramStruct), 1, NparamFile);
            }
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                for (iSeg =0; iSeg < pitchStates; ++iSeg){
                    fwrite (&cellPcn[iSide][iSeg], sizeof(paramStruct), 1, NparamFile);
                }
            }
            fclose(NparamFile);
        }
        
        
        if (writeToFile){
            NparamFile = fopen("neuronparams.dat", "w");
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                for(iSeg = 0;iSeg < mmSeg; ++iSeg)
                {
                    
                    fprintf(NparamFile,"cellElevator,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellElevator[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellElevator[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellElevator[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellElevator[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellElevator[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellElevator[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellElevator[iSide][iSeg].Idc);

                    
                    fprintf(NparamFile,"cellDepressor,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellDepressor[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellDepressor[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellDepressor[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellDepressor[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellDepressor[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellDepressor[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellDepressor[iSide][iSeg].Idc);

                    fprintf(NparamFile,"cellSwing,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellSwing[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellSwing[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellSwing[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellSwing[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellSwing[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellSwing[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellSwing[iSide][iSeg].Idc);

                    fprintf(NparamFile,"cellStance,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellStance[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellStance[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellStance[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellStance[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellStance[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellStance[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellStance[iSide][iSeg].Idc);

                    fprintf(NparamFile,"cellProtractor,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellProtractor[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellProtractor[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellProtractor[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellProtractor[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellProtractor[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellProtractor[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellProtractor[iSide][iSeg].Idc);
                    
                    fprintf(NparamFile,"cellRetractor,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellRetractor[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellRetractor[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellRetractor[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellRetractor[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellRetractor[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellRetractor[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellRetractor[iSide][iSeg].Idc);
                    
                    fprintf(NparamFile,"cellExtensor,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellExtensor[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellExtensor[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellExtensor[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellExtensor[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellExtensor[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellExtensor[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellExtensor[iSide][iSeg].Idc);
                    
                    fprintf(NparamFile,"cellFlexor,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellFlexor[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellFlexor[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellFlexor[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellFlexor[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellFlexor[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellFlexor[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellFlexor[iSide][iSeg].Idc);
                    
                    fprintf(NparamFile,"cellCoord,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellCoord[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellCoord[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellCoord[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellCoord[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellCoord[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellCoord[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellCoord[iSide][iSeg].Idc);
                    
                }
            }
            
            ///do the command neuron here
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                fprintf(NparamFile,"cellF,");
                fprintf(NparamFile,"side%d,",iSide);
                fprintf(NparamFile,"%f,", cellF[iSide].alpha);
                fprintf(NparamFile,"%f,", cellF[iSide].sigma);
                fprintf(NparamFile,"%f,", cellF[iSide].sigmaI);
                fprintf(NparamFile,"%f,", cellF[iSide].sigmaE);
                fprintf(NparamFile,"%f,", cellF[iSide].betaE);
                fprintf(NparamFile,"%f,", cellF[iSide].betaI);
                fprintf(NparamFile,"%f\n", cellF[iSide].Idc);
                
                fprintf(NparamFile,"cellB,");
                fprintf(NparamFile,"side%d,",iSide);
                fprintf(NparamFile,"%f,", cellB[iSide].alpha);
                fprintf(NparamFile,"%f,", cellB[iSide].sigma);
                fprintf(NparamFile,"%f,", cellB[iSide].sigmaI);
                fprintf(NparamFile,"%f,", cellB[iSide].sigmaE);
                fprintf(NparamFile,"%f,", cellB[iSide].betaE);
                fprintf(NparamFile,"%f,", cellB[iSide].betaI);
                fprintf(NparamFile,"%f\n", cellB[iSide].Idc);
                
                fprintf(NparamFile,"cellLL,");
                fprintf(NparamFile,"side%d,",iSide);
                fprintf(NparamFile,"%f,", cellLL[iSide].alpha);
                fprintf(NparamFile,"%f,", cellLL[iSide].sigma);
                fprintf(NparamFile,"%f,", cellLL[iSide].sigmaI);
                fprintf(NparamFile,"%f,", cellLL[iSide].sigmaE);
                fprintf(NparamFile,"%f,", cellLL[iSide].betaE);
                fprintf(NparamFile,"%f,", cellLL[iSide].betaI);
                fprintf(NparamFile,"%f\n", cellLL[iSide].Idc);
                
                fprintf(NparamFile,"cellLT,");
                fprintf(NparamFile,"side%d,",iSide);
                fprintf(NparamFile,"%f,", cellLT[iSide].alpha);
                fprintf(NparamFile,"%f,", cellLT[iSide].sigma);
                fprintf(NparamFile,"%f,", cellLT[iSide].sigmaI);
                fprintf(NparamFile,"%f,", cellLT[iSide].sigmaE);
                fprintf(NparamFile,"%f,", cellLT[iSide].betaE);
                fprintf(NparamFile,"%f,", cellLT[iSide].betaI);
                fprintf(NparamFile,"%f\n", cellLT[iSide].Idc);
                
                fprintf(NparamFile,"cellModCom,");
                fprintf(NparamFile,"side%d,",iSide);
                fprintf(NparamFile,"%f,", cellModCom[iSide].alpha);
                fprintf(NparamFile,"%f,", cellModCom[iSide].sigma);
                fprintf(NparamFile,"%f,", cellModCom[iSide].sigmaI);
                fprintf(NparamFile,"%f,", cellModCom[iSide].sigmaE);
                fprintf(NparamFile,"%f,", cellModCom[iSide].betaE);
                fprintf(NparamFile,"%f,", cellModCom[iSide].betaI);
                fprintf(NparamFile,"%f\n", cellModCom[iSide].Idc);
                
                fprintf(NparamFile,"cellH,");
                fprintf(NparamFile,"side%d,",iSide);
                fprintf(NparamFile,"%f,", cellH[iSide].alpha);
                fprintf(NparamFile,"%f,", cellH[iSide].sigma);
                fprintf(NparamFile,"%f,", cellH[iSide].sigmaI);
                fprintf(NparamFile,"%f,", cellH[iSide].sigmaE);
                fprintf(NparamFile,"%f,", cellH[iSide].betaE);
                fprintf(NparamFile,"%f,", cellH[iSide].betaI);
                fprintf(NparamFile,"%f\n", cellH[iSide].Idc);
            }
            
            //do the Pcn neuron here, cause it's a special case
            for(iSide = 0;iSide < mmSide; ++iSide)
            {
                for (iSeg =0; iSeg < pitchStates; ++iSeg){
                    
                    fprintf(NparamFile,"cellPcn,");
                    fprintf(NparamFile,"side%d,seg%d,",iSide,iSeg);
                    fprintf(NparamFile,"%f,", cellPcn[iSide][iSeg].alpha);
                    fprintf(NparamFile,"%f,", cellPcn[iSide][iSeg].sigma);
                    fprintf(NparamFile,"%f,", cellPcn[iSide][iSeg].sigmaI);
                    fprintf(NparamFile,"%f,", cellPcn[iSide][iSeg].sigmaE);
                    fprintf(NparamFile,"%f,", cellPcn[iSide][iSeg].betaE);
                    fprintf(NparamFile,"%f,", cellPcn[iSide][iSeg].betaI);
                    fprintf(NparamFile,"%f\n", cellPcn[iSide][iSeg].Idc);
                }
            }
            fclose(NparamFile);
        }
        
        
        */
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


void saveParamsToFile(int flagWriteToFile){
    writeToFile = flagWriteToFile;
    printf("saving\n");
}


/**
This function will set the IterNumChosen Global Var, which tell xmain how big the array it should allocate to store the calculated spiking current values
 @param i is the iternation number, chosen by Objective C codes, aka users
 */
void indicateNumberOfIteration(int i){
    IterNumChosen = i;
    printf("iteration = %d\n", IterNumChosen);
}


//set Synapse Params
void setSynapseParams(int id, int side, int seg, double xrp, double gamma, double gStrength){
    //this will set the single synapse
    pCustom.synapse.gamma = gamma;
    pCustom.synapse.xRp = xrp;
    pCustom.synapse.gStrength = gStrength;
    if (id == 0){
        pInhSegCoordEle[side][seg] = pCustom;
    }
    else if(id == 1){
        pExcSegContEleCoord[side][seg] = pCustom;
    }
    else if(id == 2){
        pExcIntRosEleCoord[side][seg] = pCustom;
    }
    else if(id == 3){
        pExcIntCaudEleCoord[side][seg] = pCustom;
    }
    else if(id ==4){
        pInhIntFSwing[side] = pCustom;
    }
    else if(id == 5){
        pInhIntFStance[side] = pCustom;
    }
    else if(id == 6){
        pInhIntBSwing[side] = pCustom;
    }
    else if(id == 7){
        pInhIntBStance[side] = pCustom;
    }
    else if(id == 8){
        pInhIntLLSwing[side] = pCustom;
    }
    else if(id == 9){
        pInhIntLLStance[side] = pCustom;
    }
    else if(id == 10){
        pInhIntLTSwing[side] = pCustom;
    }
    else if(id == 11){
        pInhIntLTStance[side] = pCustom;
    }
    else if(id == 12){
        pExcIntRosEleCoord[side][seg] = pCustom;
    }
    else if(id == 13){
        pExcIntRCaudEleCoord[side][seg] = pCustom;
    }
    else if(id == 14){
        pExcSegEleContraLat[side][seg] = pCustom;
    }
    else if(id == 15){
        pInhSegEleDep[side][seg] = pCustom;
    }
    else if(id == 16){
        pInhSegEleStance[side][seg] = pCustom;
    }
    else if(id == 17){
        pInhSegStanceSwing[side][seg] = pCustom;
    }
    else if(id == 18){
        pExcSegStanceProt[side][seg] = pCustom;
    }
    else if(id == 19){
        pExcSegStanceRet[side][seg] = pCustom;
    }
    else if(id == 20){
        pExcSegStanceExt[side][seg] = pCustom;
    }
    else if(id == 21){
        pExcSegStanceFlx[side][seg] = pCustom;
    }
    else if(id == 22){
        pExcSegSwingProt[side][seg] = pCustom;
    }
    else if(id == 23){
        pExcSegSwingRet[side][seg] = pCustom;
    }
    else if(id == 24){
        pExcSegSwingExt[side][seg] = pCustom;
    }
    else if(id == 25){
        pExcSegSwingFlx[side][seg] = pCustom;
    }
    else if(id == 26){
        pExcHLYL[side][seg] = pCustom;
    }
    else if(id == 27){
        pExcHLRL[side][seg] = pCustom;
    }
    else if(id == 28){
        pExcYLFR[side][seg] = pCustom;
    }
    else if(id == 29){
        pExcRLFR[side][seg] = pCustom;
    }
    else if(id == 30){
        pExcHRYR[side][seg] = pCustom;
    }
    else if(id == 31){
        pExcHRRR[side][seg] = pCustom;
    }
    else if(id == 32){
        pExcYRFL[side][seg] = pCustom;
    }
    else if(id == 33){
        pExcRRFL[side][seg] = pCustom;
    }
    else if(id == 34){
        pExcRSLeft[side][seg] = pCustom;
    }
    else if(id == 35){
        pExcRSRight[side][seg] = pCustom;
    }
    else if(id == 36){
        pExcSegPcnDep[side][seg] = pCustom;
    }
    else if(id == 37){
        pExcC[side][seg] = pCustom;
    }
    else if(id == 38){
        pInhF[side][seg] = pCustom;
    }
    else if(id == 39){
        pExcB[side][seg] = pCustom;
    }
    else if(id == 40){
        pExcModComEle[side][seg] = pCustom;
    }
    else if(id == 41){
        pExcModComDep[side][seg] = pCustom;
    }
    else if(id == 42){
        pExcModComSwing[side][seg] = pCustom;
    }
    else if(id == 43){
        pExcModComStance[side][seg] = pCustom;
    }
    else if(id == 44){
        pExcForRet[side][seg] = pCustom;
    }
    else if(id == 45){
        pExcBackProt[side][seg] = pCustom;
    }
    else if(id == 46){
        pExcLLFlx[side][seg] = pCustom;
    }
    else if(id == 47){
        pExcLTExt[side][seg] = pCustom;
    }
    else if(id == 48){
        pExcForModCom[side] = pCustom;
    }
    else if(id == 49){
        pExcBackModCom[side] = pCustom;
    }
    else if(id == 50){
        pExcLLModCom[side] = pCustom;
    }
    else if(id == 51){
        pExcLTModCom[side] = pCustom;
    }
    beginEditingSynapse = 0;
}

//set multiple synapse
void setMultipleSynapseParams(int* idArr, int* sideArr, int* segArr, double* xrpArr, double* gammaArr, double* gStrengthArr, int size){
    //this will set the multiple individual synapse
    int i;
    for (i =0; i < size; i++){
        //this will set the param for each neuron in the array
        setSynapseParams(idArr[i], sideArr[i], segArr[i], xrpArr[i], gammaArr[i], gStrengthArr[i]);
    }
}

//edit Synapse!
void editSynapseParam(int *synapseName, int* side, int* seg, double* xrp, double* gamma, double* gStrength, int size){
    //this will edit the global variables
    int i =0;
    free(globalXrp);
    free(globalGamma);
    free(globalgStrength);
    free(globalSynapseName);
    free(globalSeg);
    free(globalSide);
    //then alloc mem
    globalXrp = malloc(size*sizeof(double));
    globalgStrength = malloc(size*sizeof(double));
    globalGamma = malloc(size*sizeof(double));
    globalSynapseName = malloc(size*sizeof(int));
    globalSide = malloc(size*sizeof(int));
    globalSeg = malloc(size*sizeof(int));
    beginEditingSynapse = 1; //set flag for begin editing
    globalSize = size;
    
    for (i=0; i< size; i++){
        //printf("size = %d",size);
        //printf("fkkk");
        //  printf("%f",a[i]);
        globalgStrength[i] = gStrength[i];
        globalGamma[i] = gamma[i];
        globalXrp[i] =xrp[i];
        globalSynapseName[i] = synapseName[i];
        globalSide[i] = side[i];
        globalSeg[i] = seg[i];
    }
}

///add the multiple neuron params here//
void setMultipleNeuronParams(int* idArr, int* sideArr, int* segArr, double* aArr, double* sArr,double* sEArr, double* sIArr, double *bEArr, double *bIArr, double *IdcArr, int size){
    int i;
    for (i = 0; i < size; i++){
        //this will set the param for each neuron in the array
        setNeuronParams(idArr[i], sideArr[i], segArr[i],aArr[i], sArr[i], sEArr[i], sIArr[i], bEArr[i],bIArr[i], IdcArr[i]);
    }
}//change params
////end////

void setNeuronParams(int id, int side, int seg, double a, double s, double sE, double sI, double bE, double bI, double Idc){
    //identify which type it is?
    //if (id == 8){//Spiking
        //printf("Spiking edit\n");
        //struct strufctSpiking** cell = getSpike(neuronName);
        //struct structSpiking** cell;
        //cell = cellFlexor;
        //printf("%f\n",a);
    //    int iSide, iSeg;
    //    for(iSide = 0;iSide < mmSide; ++iSide)
    //    {
            //Now loop down the segments
    //        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
    //        {
                if(id == 0){
                    cellElevator[side][seg].burstingNeuron.alpha = a;
                    cellElevator[side][seg].burstingNeuron.sigma = s;
                    cellElevator[side][seg].burstingNeuron.sigmaE = sE;
                    cellElevator[side][seg].burstingNeuron.sigmaI = sI;
                    cellElevator[side][seg].burstingNeuron.betaE = bE;
                    cellElevator[side][seg].burstingNeuron.betaI = bI;
                    cellElevator[side][seg].burstingNeuron.Idc = Idc;
                }
                else if(id == 1){
                    cellSwing[side][seg].burstingNeuron.alpha = a;
                    cellSwing[side][seg].burstingNeuron.sigma = s;
                    cellSwing[side][seg].burstingNeuron.sigmaE = sE;
                    cellSwing[side][seg].burstingNeuron.sigmaI = sI;
                    cellSwing[side][seg].burstingNeuron.betaE = bE;
                    cellSwing[side][seg].burstingNeuron.betaI = bI;
                    cellSwing[side][seg].burstingNeuron.Idc = Idc;
                }
                else if(id == 2){
                    cellDepressor[side][seg].pacemakerNeuron.alpha = a;
                    cellDepressor[side][seg].pacemakerNeuron.sigma = s;
                    cellDepressor[side][seg].pacemakerNeuron.sigmaE = sE;
                    cellDepressor[side][seg].pacemakerNeuron.sigmaI = sI;
                    cellDepressor[side][seg].pacemakerNeuron.betaE = bE;
                    cellDepressor[side][seg].pacemakerNeuron.betaI = bI;
                    cellDepressor[side][seg].pacemakerNeuron.Idc = Idc;
                }
                else if(id == 3){
                    cellStance[side][seg].pacemakerNeuron.alpha = a;
                    cellStance[side][seg].pacemakerNeuron.sigma = s;
                    cellStance[side][seg].pacemakerNeuron.sigmaE = sE;
                    cellStance[side][seg].pacemakerNeuron.sigmaI = sI;
                    cellStance[side][seg].pacemakerNeuron.betaE = bE;
                    cellStance[side][seg].pacemakerNeuron.betaI = bI;
                    cellStance[side][seg].pacemakerNeuron.Idc = Idc;
                }
                else if(id == 4){
                    cellCoord[side][seg].spikingNeuron.alpha = a;
                    cellCoord[side][seg].spikingNeuron.sigma = s;
                    cellCoord[side][seg].spikingNeuron.sigmaE = sE;
                    cellCoord[side][seg].spikingNeuron.sigmaI = sI;
                    cellCoord[side][seg].spikingNeuron.betaE = bE;
                    cellCoord[side][seg].spikingNeuron.betaI = bI;
                    cellCoord[side][seg].spikingNeuron.Idc = Idc;
                }
                else if(id == 5){
                    cellProtractor[side][seg].spikingNeuron.alpha = a;
                    cellProtractor[side][seg].spikingNeuron.sigma = s;
                    cellProtractor[side][seg].spikingNeuron.sigmaE = sE;
                    cellProtractor[side][seg].spikingNeuron.sigmaI = sI;
                    cellProtractor[side][seg].spikingNeuron.betaE = bE;
                    cellProtractor[side][seg].spikingNeuron.betaI = bI;
                    cellProtractor[side][seg].spikingNeuron.Idc = Idc;
                }
                else if(id == 6){
                    cellRetractor[side][seg].spikingNeuron.alpha = a;
                    cellRetractor[side][seg].spikingNeuron.sigma = s;
                    cellRetractor[side][seg].spikingNeuron.sigmaE = sE;
                    cellRetractor[side][seg].spikingNeuron.sigmaI = sI;
                    cellRetractor[side][seg].spikingNeuron.betaE = bE;
                    cellRetractor[side][seg].spikingNeuron.betaI = bI;
                    cellRetractor[side][seg].spikingNeuron.Idc = Idc;
                }
                else if(id == 7){
                    cellExtensor[side][seg].spikingNeuron.alpha = a;
                    cellExtensor[side][seg].spikingNeuron.sigma = s;
                    cellExtensor[side][seg].spikingNeuron.sigmaE = sE;
                    cellExtensor[side][seg].spikingNeuron.sigmaI = sI;
                    cellExtensor[side][seg].spikingNeuron.betaE = bE;
                    cellExtensor[side][seg].spikingNeuron.betaI = bI;
                    cellExtensor[side][seg].spikingNeuron.Idc = Idc;
                }
                else if(id ==8){
                    cellFlexor[side][seg].spikingNeuron.alpha = a;
                    cellFlexor[side][seg].spikingNeuron.sigma = s;
                    cellFlexor[side][seg].spikingNeuron.sigmaE = sE;
                    cellFlexor[side][seg].spikingNeuron.sigmaI = sI;
                    cellFlexor[side][seg].spikingNeuron.betaE = bE;
                    cellFlexor[side][seg].spikingNeuron.betaI = bI;
                    cellFlexor[side][seg].spikingNeuron.Idc = Idc;
                    
                }

        //    }
      //  }

   // for(iSide = 0;iSide < mmSide; ++iSide)
   // {
        if (id == 9){
            cellF[side].spikingNeuron.alpha = a;
            cellF[side].spikingNeuron.sigma = s;
            cellF[side].spikingNeuron.sigmaE = sE;
            cellF[side].spikingNeuron.sigmaI = sI;
            cellF[side].spikingNeuron.betaE = bE;
            cellF[side].spikingNeuron.betaI = bI;
            cellF[side].spikingNeuron.Idc = Idc;
        }
        else if(id == 10){
            cellB[side].spikingNeuron.alpha = a;
            cellB[side].spikingNeuron.sigma = s;
            cellB[side].spikingNeuron.sigmaE = sE;
            cellB[side].spikingNeuron.sigmaI = sI;
            cellB[side].spikingNeuron.betaE = bE;
            cellB[side].spikingNeuron.betaI = bI;
            cellB[side].spikingNeuron.Idc = Idc;
        }
        else if(id == 11){
            cellLL[side].spikingNeuron.alpha = a;
            cellLL[side].spikingNeuron.sigma = s;
            cellLL[side].spikingNeuron.sigmaE = sE;
            cellLL[side].spikingNeuron.sigmaI = sI;
            cellLL[side].spikingNeuron.betaE = bE;
            cellLL[side].spikingNeuron.betaI = bI;
            cellLL[side].spikingNeuron.Idc = Idc;
        }
        else if (id == 12) {
            cellLT[side].spikingNeuron.alpha = a;
            cellLT[side].spikingNeuron.sigma = s;
            cellLT[side].spikingNeuron.sigmaE = sE;
            cellLT[side].spikingNeuron.sigmaI = sI;
            cellLT[side].spikingNeuron.betaE = bE;
            cellLT[side].spikingNeuron.betaI = bI;
            cellLT[side].spikingNeuron.Idc = Idc;
        }
        else if (id == 14){
            cellModCom[side].spikingNeuron.alpha = a;
            cellModCom[side].spikingNeuron.sigma = s;
            cellModCom[side].spikingNeuron.sigmaE = sE;
            cellModCom[side].spikingNeuron.sigmaI = sI;
            cellModCom[side].spikingNeuron.betaE = bE;
            cellModCom[side].spikingNeuron.betaI = bI;
            cellModCom[side].spikingNeuron.Idc = Idc;
        }
        else if (id == 15){
            cellH[side].spikingNeuron.alpha = a;
            cellH[side].spikingNeuron.sigma = s;
            cellH[side].spikingNeuron.sigmaE = sE;
            cellH[side].spikingNeuron.sigmaI = sI;
            cellH[side].spikingNeuron.betaE = bE;
            cellH[side].spikingNeuron.betaI = bI;
            cellH[side].spikingNeuron.Idc = Idc;
        }
   // }
   // for(iSide = 0;iSide < mmSide; ++iSide)
  //  {
   //     for (iSeg = 0; iSeg < pitchStates; ++iSeg){
            if (id == 13){
                //pcn
                cellPcn[side][seg].spikingNeuron.alpha = a;
                cellPcn[side][seg].spikingNeuron.sigma = s;
                cellPcn[side][seg].spikingNeuron.sigmaE = sE;
                cellPcn[side][seg].spikingNeuron.sigmaI = sI;
                cellPcn[side][seg].spikingNeuron.betaE = bE;
                cellPcn[side][seg].spikingNeuron.betaI = bI;
                cellPcn[side][seg].spikingNeuron.Idc = Idc;
            }
      //  }
    //}
    beginEditingParams = 0; //stop editing by turning the edit flag off
}// change alpha and sigma

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

//One simple function to save all parameters to both the file that will be read
//By the program and the file that we can read to see what the parameters for each neuron and synapse are.
//Work in progress
void SaveAllParams() {
    int iSide;
    int iSeg;
    FILE *paramFile;
    paramFile = fopen ("params.dat", "w");
    if (paramFile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
    for(iSide = 0;iSide < mmSide; ++iSide) {
        fwrite (&pInhIntFSwing[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&pInhIntFStance[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&pInhIntBSwing[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&pInhIntBStance[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&pInhIntLLSwing[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&pInhIntLLStance[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&pInhIntLTSwing[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&pInhIntLTStance[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&cellF[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&cellB[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&cellLL[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&cellLT[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&cellModCom[iSide], sizeof(paramStruct), 1, paramFile);
        fwrite (&cellH[iSide], sizeof(paramStruct), 1, paramFile);
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            fwrite (&pExcSegPcnDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcIntRosEleCoord[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcIntRCaudEleCoord[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegEleContraLat[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhSegEleDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhSegEleStance[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pInhSegStanceSwing[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegStanceProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegStanceRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegStanceExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegStanceFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegSwingProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegSwingRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegSwingExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegSwingFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcForRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcBackProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcLLFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcLTExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcForModCom[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcBackModCom[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcLLModCom[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcLTModCom[iSide], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcModComEle[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcModComDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcModComSwing[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcModComStance[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&pExcSegPcnDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellElevator[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellDepressor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellSwing[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellStance[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellProtractor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellRetractor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellExtensor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellFlexor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fwrite (&cellCoord[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
        }
    }
  
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for (iSeg =0; iSeg < pitchStates; ++iSeg){
            fwrite (&cellPcn[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
        }
    }
    fclose (paramFile);
    CreateReadableParams ();
}


//Function to create the parameter file the very first time the program runs
void CreateParamFile () {
    int iSide, iSeg;
    double R;
    pFastExc.synapse.xRp = -0.0; pFastExc.synapse.gamma = 0.9;      pFastExc.synapse.gStrength = 0.1;
    pFastInh.synapse.xRp = -2.2; pFastInh.synapse.gamma = 0.9;      pFastInh.synapse.gStrength = 1;
    pSlowExc.synapse.xRp = 2.2; pSlowExc.synapse.gamma = 0.995;    pSlowExc.synapse.gStrength = 1;
    pSlowInh.synapse.xRp = -2.2; pSlowInh.synapse.gamma = 0.995;    pSlowInh.synapse.gStrength = 1;
    
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            // pacemakerNeuronInit( &cellElevator[iSide][iSeg] );
            // pacemakerNeuronInit( &cellSwing[iSide][iSeg] );
            //make it bursting for now :(
            burstingNeuronInit( &cellElevator[iSide][iSeg] , speed);
            burstingNeuronInit( &cellSwing[iSide][iSeg] , speed);
            
            
            pacemakerNeuronInit2(  &cellDepressor[iSide][iSeg] , speed);
            pacemakerNeuronInit(  &cellStance[iSide][iSeg] , speed);
            
            spikingNeuronInit(   &cellProtractor[iSide][iSeg] , speed);
            spikingNeuronInit(   &cellRetractor[iSide][iSeg] , speed);
            spikingNeuronInit(   &cellExtensor[iSide][iSeg] , speed);
            spikingNeuronInit(   &cellFlexor[iSide][iSeg] , speed);
            spikingNeuronInit(   &cellCoord[iSide][iSeg] , speed);
            
            // °°°°°°°°°°°°We need to integrate these initializations with the speed control
            // Inject some noise into the CPG neurons
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellElevator[iSide][iSeg].burstingNeuron.sigma = cellElevator[iSide][iSeg].burstingNeuron.sigma + R * 0.001 + 0.0005;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellDepressor[iSide][iSeg].pacemakerNeuron.sigma = cellDepressor[iSide][iSeg].pacemakerNeuron.sigma + R * 0.0001 - 0.0195;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellSwing[iSide][iSeg].burstingNeuron.sigma = cellSwing[iSide][iSeg].burstingNeuron.sigma + R * 0.0001 - 0.0195;
            
            R = 2.0 * rand()/(RAND_MAX + 1.0) - 1.0;
            cellStance[iSide][iSeg].pacemakerNeuron.sigma = cellStance[iSide][iSeg].pacemakerNeuron.sigma + R * 0.0001 - 0.0195;
            
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
                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                    break;
                    
                case pLevel:
                    switch (roll){
                        case leftDown:{
                            switch (iSide){
                                case left:
                                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                                case right:
                                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                            }
                        case rightDown:
                            switch (iSide){
                                case left:
                                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                                case right:
                                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                            }
                        case rLevel:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                        }
                    }
                    
                case pHigh:
                    pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.95;
                    break;
                    
                case rosDn:{
                    switch (iSeg){
                        case 0:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.25;
                        case 1:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                        case 2:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                        case 3:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.95;
                            
                    }
                    
                case rosUp  :
                    switch (iSeg){
                        case 0:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.95;
                        case 1:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.75;
                        case 2:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.50;
                        case 3:
                            pExcSegPcnDep[iSide][iSeg].synapse.gStrength = 0.25;
                    }
                    break;
                }
            }
        } //END for (iSeg = 0;iSeg < mmSeg; ++iSeg)
        spikingNeuronInit(&cellPcn[iSide][pLevel], speed);
        spikingNeuronInit(&cellModCom[iSide], speed);
        spikingNeuronInit(&cellH[iSide], speed);
        spikingNeuronInit(&cellF[iSide], speed);
        spikingNeuronInit(&cellB[iSide], speed);
        spikingNeuronInit(&cellLL[iSide], speed);
        spikingNeuronInit(&cellLT[iSide], speed);
        pInhIntFSwing[iSide] = pSlowInh;
        pInhIntFStance[iSide] = pSlowInh;
        pInhIntBSwing[iSide] = pSlowInh;
        pInhIntBStance[iSide] = pSlowInh;
        pInhIntLLSwing[iSide] = pSlowInh;
        pInhIntLLStance[iSide] = pSlowInh;
        pInhIntLTSwing[iSide] = pSlowInh;
        pInhIntLTStance[iSide] = pSlowInh;
        
    }
    
    SaveAllParams();
}


//One function to load all parameters from a file if the file already exists and

void LoadAllParams () {
    int iSide;
    int iSeg;
    FILE *paramFile;
    paramFile = fopen ("params.dat", "r");
    for(iSide = 0;iSide < mmSide; ++iSide) {
        fread (&pInhIntFSwing[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&pInhIntFStance[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&pInhIntBSwing[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&pInhIntBStance[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&pInhIntLLSwing[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&pInhIntLLStance[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&pInhIntLTSwing[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&pInhIntLTStance[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&cellF[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&cellB[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&cellLL[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&cellLT[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&cellModCom[iSide], sizeof(paramStruct), 1, paramFile);
        fread (&cellH[iSide], sizeof(paramStruct), 1, paramFile);
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            fread (&pExcSegPcnDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcIntRosEleCoord[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcIntRCaudEleCoord[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegEleContraLat[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pInhSegEleDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pInhSegEleStance[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pInhSegStanceSwing[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegStanceProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegStanceRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegStanceExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegStanceFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegSwingProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegSwingRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegSwingExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegSwingFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcForRet[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcBackProt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcLLFlx[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcLTExt[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcForModCom[iSide], sizeof(paramStruct), 1, paramFile);
            fread (&pExcBackModCom[iSide], sizeof(paramStruct), 1, paramFile);
            fread (&pExcLLModCom[iSide], sizeof(paramStruct), 1, paramFile);
            fread (&pExcLTModCom[iSide], sizeof(paramStruct), 1, paramFile);
            fread (&pExcModComEle[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcModComDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcModComSwing[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcModComStance[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&pExcSegPcnDep[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellElevator[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellDepressor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellSwing[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellStance[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellProtractor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellRetractor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellExtensor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellFlexor[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
            fread (&cellCoord[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
        }
    }
    
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for (iSeg =0; iSeg < pitchStates; ++iSeg){
            fread (&cellPcn[iSide][iSeg], sizeof(paramStruct), 1, paramFile);
        }
    }
    fclose (paramFile);
    CreateReadableParams ();
}

void CreateReadableParams () {
    int iSide, iSeg;
    FILE *paramTextFile;
    paramTextFile = fopen ("params.txt", "w");
    if (paramTextFile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            
            fprintf(paramTextFile,"cellElevator\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellElevator[iSide][iSeg].burstingNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellElevator[iSide][iSeg].burstingNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellElevator[iSide][iSeg].burstingNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellElevator[iSide][iSeg].burstingNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellElevator[iSide][iSeg].burstingNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellElevator[iSide][iSeg].burstingNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellElevator[iSide][iSeg].burstingNeuron.Idc);
            
            
            fprintf(paramTextFile,"cellDepressor\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellDepressor[iSide][iSeg].pacemakerNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellDepressor[iSide][iSeg].pacemakerNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellDepressor[iSide][iSeg].pacemakerNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellDepressor[iSide][iSeg].pacemakerNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellDepressor[iSide][iSeg].pacemakerNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellDepressor[iSide][iSeg].pacemakerNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellDepressor[iSide][iSeg].pacemakerNeuron.Idc);
            
            fprintf(paramTextFile,"cellSwing\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellSwing[iSide][iSeg].burstingNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellSwing[iSide][iSeg].burstingNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellSwing[iSide][iSeg].burstingNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellSwing[iSide][iSeg].burstingNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellSwing[iSide][iSeg].burstingNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellSwing[iSide][iSeg].burstingNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellSwing[iSide][iSeg].burstingNeuron.Idc);
            
            fprintf(paramTextFile,"cellStance\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellStance[iSide][iSeg].pacemakerNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellStance[iSide][iSeg].pacemakerNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellStance[iSide][iSeg].pacemakerNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellStance[iSide][iSeg].pacemakerNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellStance[iSide][iSeg].pacemakerNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellStance[iSide][iSeg].pacemakerNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellStance[iSide][iSeg].pacemakerNeuron.Idc);
            
            fprintf(paramTextFile,"cellProtractor\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellProtractor[iSide][iSeg].spikingNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellProtractor[iSide][iSeg].spikingNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellProtractor[iSide][iSeg].spikingNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellProtractor[iSide][iSeg].spikingNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellProtractor[iSide][iSeg].spikingNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellProtractor[iSide][iSeg].spikingNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellProtractor[iSide][iSeg].spikingNeuron.Idc);
            
            fprintf(paramTextFile,"cellRetractor\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellRetractor[iSide][iSeg].spikingNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellRetractor[iSide][iSeg].spikingNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellRetractor[iSide][iSeg].spikingNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellRetractor[iSide][iSeg].spikingNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellRetractor[iSide][iSeg].spikingNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellRetractor[iSide][iSeg].spikingNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellRetractor[iSide][iSeg].spikingNeuron.Idc);
            
            fprintf(paramTextFile,"cellExtensor\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellExtensor[iSide][iSeg].spikingNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellExtensor[iSide][iSeg].spikingNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellExtensor[iSide][iSeg].spikingNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellExtensor[iSide][iSeg].spikingNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellExtensor[iSide][iSeg].spikingNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellExtensor[iSide][iSeg].spikingNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellExtensor[iSide][iSeg].spikingNeuron.Idc);
            
            fprintf(paramTextFile,"cellFlexor\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellFlexor[iSide][iSeg].spikingNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellFlexor[iSide][iSeg].spikingNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellFlexor[iSide][iSeg].spikingNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellFlexor[iSide][iSeg].spikingNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellFlexor[iSide][iSeg].spikingNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellFlexor[iSide][iSeg].spikingNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellFlexor[iSide][iSeg].spikingNeuron.Idc);
            
            fprintf(paramTextFile,"cellCoord\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellCoord[iSide][iSeg].spikingNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellCoord[iSide][iSeg].spikingNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellCoord[iSide][iSeg].spikingNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellCoord[iSide][iSeg].spikingNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellCoord[iSide][iSeg].spikingNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellCoord[iSide][iSeg].spikingNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellCoord[iSide][iSeg].spikingNeuron.Idc);
            
        }
    }
    
    ///do the command neuron here
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        fprintf(paramTextFile,"cellF\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Alpha: %f\n", cellF[iSide].spikingNeuron.alpha);
        fprintf(paramTextFile,"Sigma: %f\n", cellF[iSide].spikingNeuron.sigma);
        fprintf(paramTextFile,"SigmaI: %f\n", cellF[iSide].spikingNeuron.sigmaI);
        fprintf(paramTextFile,"SigmaE: %f\n", cellF[iSide].spikingNeuron.sigmaE);
        fprintf(paramTextFile,"BetaE: %f\n", cellF[iSide].spikingNeuron.betaE);
        fprintf(paramTextFile,"BetaI: %f\n", cellF[iSide].spikingNeuron.betaI);
        fprintf(paramTextFile,"Idc: %f\n\n", cellF[iSide].spikingNeuron.Idc);
        
        fprintf(paramTextFile,"cellB\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Alpha: %f\n", cellB[iSide].spikingNeuron.alpha);
        fprintf(paramTextFile,"Sigma: %f\n", cellB[iSide].spikingNeuron.sigma);
        fprintf(paramTextFile,"SigmaI: %f\n", cellB[iSide].spikingNeuron.sigmaI);
        fprintf(paramTextFile,"SigmaE: %f\n", cellB[iSide].spikingNeuron.sigmaE);
        fprintf(paramTextFile,"BetaE: %f\n", cellB[iSide].spikingNeuron.betaE);
        fprintf(paramTextFile,"BetaI: %f\n", cellB[iSide].spikingNeuron.betaI);
        fprintf(paramTextFile,"Idc: %f\n\n", cellB[iSide].spikingNeuron.Idc);
        
        fprintf(paramTextFile,"cellLL\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Alpha: %f\n", cellLL[iSide].spikingNeuron.alpha);
        fprintf(paramTextFile,"Sigma: %f\n", cellLL[iSide].spikingNeuron.sigma);
        fprintf(paramTextFile,"SigmaI: %f\n", cellLL[iSide].spikingNeuron.sigmaI);
        fprintf(paramTextFile,"SigmaE: %f\n", cellLL[iSide].spikingNeuron.sigmaE);
        fprintf(paramTextFile,"BetaE: %f\n", cellLL[iSide].spikingNeuron.betaE);
        fprintf(paramTextFile,"BetaI: %f\n", cellLL[iSide].spikingNeuron.betaI);
        fprintf(paramTextFile,"Idc: %f\n\n", cellLL[iSide].spikingNeuron.Idc);
        
        fprintf(paramTextFile,"cellLT\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Alpha: %f\n", cellLT[iSide].spikingNeuron.alpha);
        fprintf(paramTextFile,"Sigma: %f\n", cellLT[iSide].spikingNeuron.sigma);
        fprintf(paramTextFile,"SigmaI: %f\n", cellLT[iSide].spikingNeuron.sigmaI);
        fprintf(paramTextFile,"SigmaE: %f\n", cellLT[iSide].spikingNeuron.sigmaE);
        fprintf(paramTextFile,"BetaE: %f\n", cellLT[iSide].spikingNeuron.betaE);
        fprintf(paramTextFile,"BetaI: %f\n", cellLT[iSide].spikingNeuron.betaI);
        fprintf(paramTextFile,"Idc: %f\n\n", cellLT[iSide].spikingNeuron.Idc);
        
        fprintf(paramTextFile,"cellModCom\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Alpha: %f\n", cellModCom[iSide].spikingNeuron.alpha);
        fprintf(paramTextFile,"Sigma: %f\n", cellModCom[iSide].spikingNeuron.sigma);
        fprintf(paramTextFile,"SigmaI: %f\n", cellModCom[iSide].spikingNeuron.sigmaI);
        fprintf(paramTextFile,"SigmaE: %f\n", cellModCom[iSide].spikingNeuron.sigmaE);
        fprintf(paramTextFile,"BetaE: %f\n", cellModCom[iSide].spikingNeuron.betaE);
        fprintf(paramTextFile,"BetaI: %f\n", cellModCom[iSide].spikingNeuron.betaI);
        fprintf(paramTextFile,"Idc: %f\n\n", cellModCom[iSide].spikingNeuron.Idc);
        
        fprintf(paramTextFile,"cellH\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Alpha: %f\n", cellH[iSide].spikingNeuron.alpha);
        fprintf(paramTextFile,"Sigma: %f\n", cellH[iSide].spikingNeuron.sigma);
        fprintf(paramTextFile,"SigmaI: %f\n", cellH[iSide].spikingNeuron.sigmaI);
        fprintf(paramTextFile,"SigmaE: %f\n", cellH[iSide].spikingNeuron.sigmaE);
        fprintf(paramTextFile,"BetaE: %f\n", cellH[iSide].spikingNeuron.betaE);
        fprintf(paramTextFile,"BetaI: %f\n", cellH[iSide].spikingNeuron.betaI);
        fprintf(paramTextFile,"Idc: %f\n\n", cellH[iSide].spikingNeuron.Idc);
    }
    
    //do the Pcn neuron here, cause it's a special case
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for (iSeg =0; iSeg < pitchStates; ++iSeg){
            
            fprintf(paramTextFile,"cellPcn\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Alpha: %f\n", cellPcn[iSide][iSeg].spikingNeuron.alpha);
            fprintf(paramTextFile,"Sigma: %f\n", cellPcn[iSide][iSeg].spikingNeuron.sigma);
            fprintf(paramTextFile,"SigmaI: %f\n", cellPcn[iSide][iSeg].spikingNeuron.sigmaI);
            fprintf(paramTextFile,"SigmaE: %f\n", cellPcn[iSide][iSeg].spikingNeuron.sigmaE);
            fprintf(paramTextFile,"BetaE: %f\n", cellPcn[iSide][iSeg].spikingNeuron.betaE);
            fprintf(paramTextFile,"BetaI: %f\n", cellPcn[iSide][iSeg].spikingNeuron.betaI);
            fprintf(paramTextFile,"Idc: %f\n\n", cellPcn[iSide][iSeg].spikingNeuron.Idc);
        }
    }
    for(iSide = 0;iSide < mmSide; ++iSide)  //This loop initializes the parameters for synapses
    {
        // Presynaptic Inhibition from a command to bifunctional interneuron synapse
        fprintf(paramTextFile,"pInhIntFSwing\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Gamma: %f\n",pInhIntFSwing[iSide].synapse.gamma);
        fprintf(paramTextFile,"xRp: %f\n",pInhIntFSwing[iSide].synapse.xRp);
        fprintf(paramTextFile,"gStrength: %f\n\n",pInhIntFSwing[iSide].synapse.gStrength);
        
        fprintf(paramTextFile,"pInhIntFStance\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Gamma: %f\n",pInhIntFStance[iSide].synapse.gamma);
        fprintf(paramTextFile,"xRp: %f\n",pInhIntFStance[iSide].synapse.xRp);
        fprintf(paramTextFile,"gStrength: %f\n\n",pInhIntFStance[iSide].synapse.gStrength);
        
        fprintf(paramTextFile,"pInhIntBSwing\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Gamma: %f\n",pInhIntBSwing[iSide].synapse.gamma);
        fprintf(paramTextFile,"xRp: %f\n",pInhIntBSwing[iSide].synapse.xRp);
        fprintf(paramTextFile,"gStrength: %f\n\n",pInhIntBSwing[iSide].synapse.gStrength);
        
        fprintf(paramTextFile,"pInhIntBStance\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Gamma: %f\n",pInhIntBStance[iSide].synapse.gamma);
        fprintf(paramTextFile,"xRp: %f\n",pInhIntBStance[iSide].synapse.xRp);
        fprintf(paramTextFile,"gStrength: %f\n\n",pInhIntBStance[iSide].synapse.gStrength);
        
        fprintf(paramTextFile,"pInhIntLLSwing\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Gamma: %f\n",pInhIntLLSwing[iSide].synapse.gamma);
        fprintf(paramTextFile,"xRp: %f\n",pInhIntLLSwing[iSide].synapse.xRp);
        fprintf(paramTextFile,"gStrength: %f\n\n",pInhIntLLSwing[iSide].synapse.gStrength);
        
        fprintf(paramTextFile,"pInhIntLLStance\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Gamma: %f\n",pInhIntLLStance[iSide].synapse.gamma);
        fprintf(paramTextFile,"xRp: %f\n",pInhIntLLStance[iSide].synapse.xRp);
        fprintf(paramTextFile,"gStrength: %f\n\n",pInhIntLLStance[iSide].synapse.gStrength);
        
        fprintf(paramTextFile,"pInhIntLTSwing\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Gamma: %f\n",pInhIntLTSwing[iSide].synapse.gamma);
        fprintf(paramTextFile,"xRp: %f\n",pInhIntLTSwing[iSide].synapse.xRp);
        fprintf(paramTextFile,"gStrength: %f\n\n",pInhIntLTSwing[iSide].synapse.gStrength);
        
        fprintf(paramTextFile,"pInhIntLTStance\n");
        fprintf(paramTextFile,"Side: %d\n",iSide);
        fprintf(paramTextFile,"Gamma: %f\n",pInhIntLTStance[iSide].synapse.gamma);
        fprintf(paramTextFile,"xRp: %f\n",pInhIntLTStance[iSide].synapse.xRp);
        fprintf(paramTextFile,"gStrength: %f\n\n",pInhIntLTStance[iSide].synapse.gStrength);
        //Now loop down the segments
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            //------- Set the parameters for synapses {xRp and gamma} ---------
            
            //Synapse between pitch Command and segmental depressors
            fprintf(paramTextFile,"pExcSegPcnDep\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegPcnDep[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegPcnDep[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegPcnDep[iSide][iSeg].synapse.gStrength);
            
            // Excitory synapses between elevator synergies and ajacent coordinating neurons
            fprintf(paramTextFile,"pExcIntRosEleCoord\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcIntRosEleCoord[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcIntRosEleCoord[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcIntRosEleCoord[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcIntRCaudEleCoord\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcIntRCaudEleCoord[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcIntRCaudEleCoord[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcIntRCaudEleCoord[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegEleContraLat\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegEleContraLat[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegEleContraLat[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegEleContraLat[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pInhSegEleDep\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pInhSegEleDep[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pInhSegEleDep[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pInhSegEleDep[iSide][iSeg].synapse.gStrength);
            //Internal inhibitory synapses of neuronal oscillator
            
            fprintf(paramTextFile,"pInhSegEleStance\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pInhSegEleStance[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pInhSegEleStance[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pInhSegEleStance[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pInhSegStanceSwing\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pInhSegStanceSwing[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pInhSegStanceSwing[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pInhSegStanceSwing[iSide][iSeg].synapse.gStrength);
            
            // Excitatory Synapses from Swing/Stance interneruons to bifunctional synapses
            fprintf(paramTextFile,"pExcSegStanceProt\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegStanceProt[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegStanceProt[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegStanceProt[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegStanceRet\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegStanceRet[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegStanceRet[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegStanceRet[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegStanceExt\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegStanceExt[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegStanceExt[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegStanceExt[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegStanceFlx\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegStanceFlx[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegStanceFlx[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegStanceFlx[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegSwingProt\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegSwingProt[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegSwingProt[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegSwingProt[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegSwingRet\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegSwingRet[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegSwingRet[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegSwingRet[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegSwingExt\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegSwingExt[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegSwingExt[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegSwingExt[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegSwingFlx\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegSwingFlx[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegSwingFlx[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegSwingFlx[iSide][iSeg].synapse.gStrength);
            
            // Recruiting excitatory synapses from Walking commands to propulsive synergies
            fprintf(paramTextFile,"pExcForRet\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcForRet[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcForRet[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcForRet[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcBackProt\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcBackProt[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcBackProt[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcBackProt[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcLLFlx\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcLLFlx[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcLLFlx[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcLLFlx[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcLTExt\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcLTExt[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcLTExt[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcLTExt[iSide][iSeg].synapse.gStrength);
            
            
            // Recruiting excitatory synapses from Walking commands to ModCom
            fprintf(paramTextFile,"pExcForModCom\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcForModCom[iSide].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcForModCom[iSide].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcForModCom[iSide].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcBackModCom\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcBackModCom[iSide].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcBackModCom[iSide].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcBackModCom[iSide].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcLLModCom\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcLLModCom[iSide].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcLLModCom[iSide].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcLLModCom[iSide].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcLTModCom\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcLTModCom[iSide].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcLTModCom[iSide].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcLTModCom[iSide].synapse.gStrength);
            
            //Excitatory synapses from modulatory Commands to CPG Neurons
            fprintf(paramTextFile,"pExcModComEle\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcModComEle[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcModComEle[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcModComEle[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcModComDep\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcModComDep[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcModComDep[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcModComDep[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcModComSwing\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcModComSwing[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcModComSwing[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcModComSwing[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcModComStance\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcModComStance[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcModComStance[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcModComStance[iSide][iSeg].synapse.gStrength);
            
            fprintf(paramTextFile,"pExcSegPcnDep\n");
            fprintf(paramTextFile,"Side: %d\nSeg: %d\n",iSide,iSeg);
            fprintf(paramTextFile,"Gamma: %f\n",pExcSegPcnDep[iSide][iSeg].synapse.gamma);
            fprintf(paramTextFile,"xRp: %f\n",pExcSegPcnDep[iSide][iSeg].synapse.xRp);
            fprintf(paramTextFile,"gStrength: %f\n\n",pExcSegPcnDep[iSide][iSeg].synapse.gStrength);
        }
    }
    
    fclose(paramTextFile);
}

void TempSpeedSolution(int speed){
    switch(speed){
        case 1:
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 4; j++){
                    cellElevator[i][j].burstingNeuron.alpha = 4;
                    cellElevator[i][j].burstingNeuron.sigma = -.5;
                    cellSwing[i][j].burstingNeuron.alpha = 4;
                    cellSwing[i][j].burstingNeuron.sigma = -.5;
                    cellDepressor[i][j].burstingNeuron.alpha = 4;
                    cellDepressor[i][j].burstingNeuron.sigma = -.5;
                    cellStance[i][j].burstingNeuron.alpha = 4;
                    cellStance[i][j].burstingNeuron.sigma = -.5;
                }
            break;
        case 2:
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 4; j++){
                    cellElevator[i][j].burstingNeuron.alpha = 7;
                    cellElevator[i][j].burstingNeuron.sigma = -.5;
                    cellSwing[i][j].burstingNeuron.alpha = 7;
                    cellSwing[i][j].burstingNeuron.sigma = -.5;
                    cellDepressor[i][j].burstingNeuron.alpha = 7;
                    cellDepressor[i][j].burstingNeuron.sigma = -.5;
                    cellStance[i][j].burstingNeuron.alpha = 7;
                    cellStance[i][j].burstingNeuron.sigma = -.5;
                }
            break;
        case 3:
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 4; j++){
                    cellElevator[i][j].burstingNeuron.alpha = 6.2;
                    cellElevator[i][j].burstingNeuron.sigma = -.2;
                    cellSwing[i][j].burstingNeuron.alpha = 6.2;
                    cellSwing[i][j].burstingNeuron.sigma = -.2;
                    cellDepressor[i][j].burstingNeuron.alpha = 6.2;
                    cellDepressor[i][j].burstingNeuron.sigma = -.2;
                    cellStance[i][j].burstingNeuron.alpha = 6.2;
                    cellStance[i][j].burstingNeuron.sigma = -.2;
                }
            break;
        case 4:
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 4; j++){
                    cellElevator[i][j].burstingNeuron.alpha = 5.8;
                    cellElevator[i][j].burstingNeuron.sigma = .1;
                    cellSwing[i][j].burstingNeuron.alpha = 5.8;
                    cellSwing[i][j].burstingNeuron.sigma = .1;
                    cellDepressor[i][j].burstingNeuron.alpha = 5.8;
                    cellDepressor[i][j].burstingNeuron.sigma = .1;
                    cellStance[i][j].burstingNeuron.alpha = 5.8;
                    cellStance[i][j].burstingNeuron.sigma = .1;
                }
            break;
    }
    SaveAllParams();
}



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
            spikesElevator[iSide][iSeg]     = spikesElevator[iSide][iSeg]  + cellElevator[iSide][iSeg].burstingNeuron.spike  + cNextSeg * (cellElevator[iSide][iSeg-1].burstingNeuron.spike  + cellElevator[iSide][iSeg + 1].burstingNeuron.spike);
            spikesDepressor[iSide][iSeg]    = spikesDepressor[iSide][iSeg] + cellDepressor[iSide][iSeg].pacemakerNeuron.spike + cNextSeg * (cellDepressor[iSide][iSeg-1].pacemakerNeuron.spike + cellDepressor[iSide][iSeg + 1].pacemakerNeuron.spike);
            spikesSwing[iSide][iSeg]        = spikesSwing[iSide][iSeg]     + cellSwing[iSide][iSeg].burstingNeuron.spike     + cNextSeg * (cellSwing[iSide][iSeg-1].burstingNeuron.spike     + cellSwing[iSide][iSeg + 1].burstingNeuron.spike);
            spikesStance[iSide][iSeg]       = spikesStance[iSide][iSeg]    + cellStance[iSide][iSeg].pacemakerNeuron.spike    + cNextSeg * (cellStance[iSide][iSeg-1].pacemakerNeuron.spike    + cellStance[iSide][iSeg + 1].pacemakerNeuron.spike);
        }
        
    }  //END for (iSeg)
    iSeg = 0;
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        spikesElevator[iSide][iSeg]  = spikesElevator[iSide][iSeg]  + cellElevator[iSide][iSeg].burstingNeuron.spike   + cNextSeg*cellElevator[iSide][iSeg + 1].burstingNeuron.spike;
        spikesDepressor[iSide][iSeg] = spikesDepressor[iSide][iSeg] + cellDepressor[iSide][iSeg].pacemakerNeuron.spike  + cNextSeg*cellDepressor[iSide][iSeg + 1].pacemakerNeuron.spike;
        spikesSwing[iSide][iSeg]     = spikesSwing[iSide][iSeg]     + cellSwing[iSide][iSeg].burstingNeuron.spike      + cNextSeg*cellSwing[iSide][iSeg + 1].burstingNeuron.spike;
        spikesStance[iSide][iSeg]    = spikesStance[iSide][iSeg]    + cellStance[iSide][iSeg].pacemakerNeuron.spike     + cNextSeg*cellStance[iSide][iSeg + 1].pacemakerNeuron.spike;
    }
    iSeg = mmSeg-1;
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        spikesElevator[iSide][iSeg]  = spikesElevator[iSide][iSeg] 	+ cellElevator[iSide][iSeg].burstingNeuron.spike 	+ cNextSeg*cellElevator[iSide][iSeg-1].burstingNeuron.spike;
        spikesDepressor[iSide][iSeg] = spikesDepressor[iSide][iSeg] + cellDepressor[iSide][iSeg].pacemakerNeuron.spike 	+ cNextSeg*cellDepressor[iSide][iSeg-1].pacemakerNeuron.spike;
        spikesSwing[iSide][iSeg]     = spikesSwing[iSide][iSeg] 	+ cellSwing[iSide][iSeg].burstingNeuron.spike 		+ cNextSeg*cellSwing[iSide][iSeg-1].burstingNeuron.spike;
        spikesStance[iSide][iSeg]    = spikesStance[iSide][iSeg] 	+ cellStance[iSide][iSeg].pacemakerNeuron.spike 	+ cNextSeg*cellStance[iSide][iSeg-1].pacemakerNeuron.spike;
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
             calcSynapticCurrents( &iInhSegEleDep[iSide][iSeg],    		&pInhSegEleDep[iSide][iSeg],  		cellDepressor[iSide][iSeg].pacemakerNeuron.x,       spikesElevator[iSide][iSeg]);
            
          //  calcSynapticCurrents( &iInhSegEleStance[iSide][iSeg],    	&pInhSegEleStance[iSide][iSeg],  	cellElevator[iSide][iSeg].x,       spikesElevator[iSide][iSeg]/mmSeg);
             calcSynapticCurrents( &iInhSegEleStance[iSide][iSeg],    	&pInhSegEleStance[iSide][iSeg],  	cellStance[iSide][iSeg].pacemakerNeuron.x,       spikesElevator[iSide][iSeg]);
            
            
          //  calcSynapticCurrents( &iInhSegStanceSwing[iSide][iSeg],    	&pInhSegStanceSwing[iSide][iSeg],  	cellStance[iSide][iSeg].x,         spikesStance[iSide][iSeg]/mmSeg);
            calcSynapticCurrents( &iInhSegStanceSwing[iSide][iSeg],    	&pInhSegStanceSwing[iSide][iSeg],  	cellSwing[iSide][iSeg].burstingNeuron.x,         spikesStance[iSide][iSeg]);
            
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
            
           
         /*   void calcModulatedCurrents(double *I, //This implements presynaptic inhibition, modParams is the moduatory neuron synapse
                                       struct structSynapses *params,struct structSynapses *modParams,
                                       double xPost,double xModPost, double spikes, double modSpikes) {
            */
            
           // calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntFSwing[iSide],  cellSwing[iSide][iSeg].x,   cellF[iSide].x,  spikesSwing[iSide][iSeg], spikesF[iSide]);
          
            //swing to prot, w B
           // calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntBSwing[iSide],  cellProtractor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x,  spikesSwing[iSide][iSeg], spikesB[iSide]);
            
            calcModulatedCurrents( &iExcSegSwingProt[iSide][iSeg],   &pExcSegSwingProt[iSide][iSeg], &pInhIntBSwing[iSide],  cellProtractor[iSide][iSeg].spikingNeuron.x,   cellSwing[iSide][iSeg].burstingNeuron.x,  cellB[iSide].spikingNeuron.spike, cellSwing[iSide][iSeg].burstingNeuron.spike);

            
           // calcModulatedCurrents( &iExcSegSwingRet[iSide][iSeg],    &pExcSegSwingRet[iSide][iSeg],  &pInhIntBSwing[iSide],	 cellSwing[iSide][iSeg].x,   cellB[iSide].x,  spikesSwing[iSide][iSeg], spikesB[iSide]);
         //   calcModulatedCurrents( &iExcSegSwingRet[iSide][iSeg],    &pExcSegSwingRet[iSide][iSeg],  &pInhIntFSwing[iSide],	 cellRetractor[iSide][iSeg].x,   cellSwing[iSide][iSeg].x,   cellSwing[iSide][iSeg].spike, cellF[iSide].spike);
            
         calcModulatedCurrents( &iExcSegSwingRet[iSide][iSeg],    &pExcSegSwingRet[iSide][iSeg],  &pInhIntFSwing[iSide],     cellRetractor[iSide][iSeg].spikingNeuron.x,   cellSwing[iSide][iSeg].burstingNeuron.x, cellF[iSide].spikingNeuron.spike,  cellSwing[iSide][iSeg].burstingNeuron.spike);
            
            
        //    calcModulatedCurrents( &iExcSegSwingExt[iSide][iSeg],    &pExcSegSwingExt[iSide][iSeg],  &pInhIntLTSwing[iSide], cellSwing[iSide][iSeg].x,   cellLT[iSide].x, spikesSwing[iSide][iSeg], spikesLT[iSide]);
        calcModulatedCurrents( &iExcSegSwingExt[iSide][iSeg],    &pExcSegSwingExt[iSide][iSeg],  &pInhIntLTSwing[iSide], cellExtensor[iSide][iSeg].spikingNeuron.x,   cellSwing[iSide][iSeg].burstingNeuron.x, cellLT[iSide].spikingNeuron.spike, cellSwing[iSide][iSeg].burstingNeuron.spike);

            
            
          //  calcModulatedCurrents( &iExcSegSwingFlex[iSide][iSeg],   &pExcSegSwingFlx[iSide][iSeg],  &pInhIntLLSwing[iSide], cellSwing[iSide][iSeg].x,   cellLL[iSide].x, spikesSwing[iSide][iSeg], spikesLL[iSide]);


            calcModulatedCurrents( &iExcSegSwingFlex[iSide][iSeg],   &pExcSegSwingFlx[iSide][iSeg],  &pInhIntLLSwing[iSide], cellFlexor[iSide][iSeg].spikingNeuron.x,   cellSwing[iSide][iSeg].burstingNeuron.x, cellLL[iSide].spikingNeuron.spike, cellSwing[iSide][iSeg].burstingNeuron.spike);
            
            
            ///SEARCH FOR ME!!!!!!!!!!!!!! I CHANGE THE SPIKE!!!!!!!!!!!!!
            // These are the synapses coupling the stance interneron to the bifunctional motorneurons
        //    calcModulatedCurrents( &iExcSegStanceProt[iSide][iSeg],   &pExcSegStanceProt[iSide][iSeg],  &pInhIntFStance[iSide],    	 cellStance[iSide][iSeg].x,  cellF[iSide].x,     spikesStance[iSide][iSeg], spikesF[iSide]);
  //          calcModulatedCurrents( &iExcSegStanceProt[iSide][iSeg],   &pExcSegStanceProt[iSide][iSeg],  &pInhIntFStance[iSide],    	 cellProtractor[iSide][iSeg].x,  cellStance[iSide][iSeg].x,     cellStance[iSide][iSeg].spike, cellF[iSide].spike);
            calcModulatedCurrents( &iExcSegStanceProt[iSide][iSeg],   &pExcSegStanceProt[iSide][iSeg],  &pInhIntFStance[iSide],         cellProtractor[iSide][iSeg].spikingNeuron.x,  cellStance[iSide][iSeg].pacemakerNeuron.x,     cellF[iSide].spikingNeuron.spike,cellStance[iSide][iSeg].pacemakerNeuron.spike);
            
            ///SEARCH FOR ME!!!!!!!!!!!!!!
        //    calcModulatedCurrents( &iExcSegStanceRet[iSide][iSeg],	  &pExcSegStanceRet[iSide][iSeg],   &pInhIntBStance[iSide],      cellStance[iSide][iSeg].x,  cellB[iSide].x,     spikesStance[iSide][iSeg], spikesB[iSide]);
         //   calcModulatedCurrents( &iExcSegStanceRet[iSide][iSeg],	  &pExcSegStanceRet[iSide][iSeg],   &pInhIntBStance[iSide],      cellRetractor[iSide][iSeg].x,  cellStance[iSide][iSeg].x,     cellStance[iSide][iSeg].spike, cellB[iSide].spike);

            calcModulatedCurrents( &iExcSegStanceRet[iSide][iSeg],      &pExcSegStanceRet[iSide][iSeg],   &pInhIntBStance[iSide],      cellRetractor[iSide][iSeg].spikingNeuron.x,  cellStance[iSide][iSeg].pacemakerNeuron.x,     cellB[iSide].spikingNeuron.spike,cellStance[iSide][iSeg].pacemakerNeuron.spike);


          //  calcModulatedCurrents( &iExcSegStanceExt[iSide][iSeg],    &pExcSegStanceExt[iSide][iSeg],   &pInhIntLTStance[iSide],     cellStance[iSide][iSeg].x,  cellLT[iSide].x,    spikesStance[iSide][iSeg], spikesLT[iSide]);
         //   calcModulatedCurrents( &iExcSegStanceExt[iSide][iSeg],    &pExcSegStanceExt[iSide][iSeg],   &pInhIntLTStance[iSide],     cellExtensor[iSide][iSeg].x,  cellStance[iSide][iSeg].x,    spikesStance[iSide][iSeg], spikesLT[iSide]);

            calcModulatedCurrents( &iExcSegStanceExt[iSide][iSeg],    &pExcSegStanceExt[iSide][iSeg],   &pInhIntLTStance[iSide],     cellExtensor[iSide][iSeg].spikingNeuron.x,  cellStance[iSide][iSeg].pacemakerNeuron.x,    cellLT[iSide].spikingNeuron.spike, cellStance[iSide][iSeg].pacemakerNeuron.spike);

            
            
          //  calcModulatedCurrents( &iExcSegStanceFlex[iSide][iSeg],   &pExcSegStanceFlx[iSide][iSeg],   &pInhIntLLStance[iSide], 	 cellStance[iSide][iSeg].x,  cellLL[iSide].x,    spikesStance[iSide][iSeg], spikesLL[iSide]);
            calcModulatedCurrents( &iExcSegStanceFlex[iSide][iSeg],   &pExcSegStanceFlx[iSide][iSeg],   &pInhIntLLStance[iSide], 	 cellFlexor[iSide][iSeg].spikingNeuron.x,  cellStance[iSide][iSeg].pacemakerNeuron.x,    cellLL[iSide].spikingNeuron.spike, cellStance[iSide][iSeg].pacemakerNeuron.spike);
            

////////////////////IF UNSURE, COME BACK AND CHANGE WHAT's ABOVE HERE/////////////////////////////////////
          //COMMENT OUT MODCOM TO ELEV FOR NOW!!!
            
            // These are the synapses coupling the modulatory command interneron to the CPG neurons
             calcSynapticCurrents( &iExcIntModComEle[iSide][iSeg],    	&pExcModComEle[iSide][iSeg],  	    cellElevator[iSide][iSeg].burstingNeuron.x,       cellModCom[iSide].spikingNeuron.spike);
      
            
            calcSynapticCurrents( &iExcIntModComDep[iSide][iSeg],    	&pExcModComDep[iSide][iSeg],  	cellDepressor[iSide][iSeg].pacemakerNeuron.x,       cellModCom[iSide].spikingNeuron.spike);

        
            calcSynapticCurrents( &iExcIntModComSwing[iSide][iSeg],    	&pExcModComSwing[iSide][iSeg],  	cellSwing[iSide][iSeg].burstingNeuron.x,       cellModCom[iSide].spikingNeuron.spike);

            
            calcSynapticCurrents( &iExcIntModComStance[iSide][iSeg],    &pExcModComStance[iSide][iSeg],  	cellStance[iSide][iSeg].pacemakerNeuron.x,       cellModCom[iSide].spikingNeuron.spike);

    // These are the synapses coupling the postural command interneron to the depressor neurons
    //        calcSynapticCurrents( &iExcSegPcnDep[iSide][iSeg],    &pExcSegPcnDep[iSide][iSeg],  	cellPcn[iSide][pitch].x,       spikesPcn[iSide]/mmSeg);
            calcSynapticCurrents( &iExcSegPcnDep[iSide][iSeg],    &pExcSegPcnDep[iSide][iSeg],      cellDepressor[iSide][iSeg].pacemakerNeuron.x,       spikesPcn[iSide]/mmSeg);
            //pExcForRet, pExcBackProt, pExcLLFlx, pExcLTExt;
            calcSynapticCurrents( &iExcForRet[iSide][iSeg],          &pExcForRet[iSide][iSeg],           cellF[iSide].spikingNeuron.x,            spikesF[iSide]/mmSeg);  //Walking Command Neurons
        //    calcSynapticCurrents( &iExcForRet[iSide][iSeg],          &pExcForRet[iSide][iSeg],           cellRetractor[iSide][iSeg].x,            cellF[iSide].spike);  //Walking Command Neurons
            
            calcSynapticCurrents( &iExcBackProt[iSide][iSeg],    	 &pExcBackProt[iSide][iSeg],         cellB[iSide].spikingNeuron.x,            spikesB[iSide]/mmSeg);
        //    calcSynapticCurrents( &iExcBackProt[iSide][iSeg],    	 &pExcBackProt[iSide][iSeg],         cellProtractor[iSide][iSeg].x,            cellB[iSide].spike);
            
            calcSynapticCurrents( &iExcLLFlx[iSide][iSeg],           &pExcLLFlx[iSide][iSeg],            cellLL[iSide].spikingNeuron.x,           spikesLL[iSide]/mmSeg);
        //    calcSynapticCurrents( &iExcLLFlx[iSide][iSeg],           &pExcLLFlx[iSide][iSeg],            cellFlexor[iSide][iSeg].x,           spikesLL[iSide]/mmSeg);

            calcSynapticCurrents( &iExcLTExt[iSide][iSeg],           &pExcLTExt[iSide][iSeg],            cellLT[iSide].spikingNeuron.x,           spikesLT[iSide]/mmSeg);
        //    calcSynapticCurrents( &iExcLTExt[iSide][iSeg],           &pExcLTExt[iSide][iSeg],            cellExtensor[iSide][iSeg].x,           spikesLT[iSide]/mmSeg);
            
          //Directional Command Neuron to ModCom
            calcSynapticCurrents( &iExcForModCom[iSide],       &pExcForModCom[iSide],           cellModCom[iSide].spikingNeuron.x,            cellF[iSide].spikingNeuron.spike);  //Walking Command Neurons
            
            calcSynapticCurrents( &iExcBackModCom[iSide],    	 &pExcBackModCom[iSide],         cellModCom[iSide].spikingNeuron.x,            cellB[iSide].spikingNeuron.spike);
            
            calcSynapticCurrents( &iExcLLModCom[iSide],           &pExcLLModCom[iSide],            cellModCom[iSide].spikingNeuron.x,             spikesLL[iSide]/mmSeg);
            
            calcSynapticCurrents( &iExcLTModCom[iSide],           &pExcLTModCom[iSide],            cellModCom[iSide].spikingNeuron.x,           spikesLT[iSide]/mmSeg);
            
  //COMMENt OUT DIRECTIONAL COMMAND TO MODCOM
            //THIS IS NON LINEAR
            
            // These are the inhibitory synapses between the coordinating neurons and the segmental elevators and the segmental elevators and neighboring coordinating neurons
            if (iSeg == 1){                                     //This section calculates the impact of the Coordinating neurons on the elevators
                
                calcSynapticCurrents( &iInhSegCoordEle[iSide][iSeg],          &pInhSegCoordEle[iSide][iSeg],        cellElevator[iSide][iSeg].burstingNeuron.x,      spikesElevator[iSide][iSeg]);  //Coordinating Neurons
                calcSynapticCurrents( &iExcSegContEleCoord[iSide][iSeg],      &pExcSegContEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg].spikingNeuron.x,         spikesCoord[iSide][iSeg]);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntCaudEleCoord[iSide][iSeg],      &pExcIntCaudEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg+1].spikingNeuron.x,         spikesCoord[iSide][iSeg+1]);  //Coordinating Neurons
            }
            else if ((iSeg ==2)||(iSeg == 3))  {
                calcSynapticCurrents( &iInhSegCoordEle[iSide][iSeg],          &pInhSegCoordEle[iSide][iSeg],        cellElevator[iSide][iSeg].burstingNeuron.x,      spikesElevator[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcSegContEleCoord[iSide][iSeg],      &pExcSegContEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg].spikingNeuron.x,         spikesCoord[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntRosEleCoord[iSide][iSeg],       &pExcIntRosEleCoord[iSide][iSeg],     cellCoord[iSide][iSeg-1].spikingNeuron.x,         spikesCoord[iSide][iSeg-1]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntCaudEleCoord[iSide][iSeg],      &pExcIntCaudEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg+1].spikingNeuron.x,         spikesCoord[iSide][iSeg+1]/mmSeg);  //Coordinating Neurons
            }
            
            else if (iSeg == 4){
                calcSynapticCurrents( &iInhSegCoordEle[iSide][iSeg],          &pInhSegCoordEle[iSide][iSeg],        cellElevator[iSide][iSeg].burstingNeuron.x,      spikesElevator[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcSegContEleCoord[iSide][iSeg],      &pExcSegContEleCoord[iSide][iSeg],    cellCoord[iSide][iSeg].spikingNeuron.x,         spikesCoord[iSide][iSeg]/mmSeg);  //Coordinating Neurons
                calcSynapticCurrents( &iExcIntRosEleCoord[iSide][iSeg],       &pExcIntRosEleCoord[iSide][iSeg],     cellCoord[iSide][iSeg-1].spikingNeuron.x,         spikesCoord[iSide][iSeg-1]/mmSeg);  //Coordinating Neurons
            }
            
            /*
             pInhSegCoordEle[mmSide][mmSeg],                                                                     // Segmental Inhibition from an Coordinating Neuron to an Elevator
             pExcSegContEleCoord[mmSide][mmSeg],                                                                 // Intersegmental Exitation from and elevator to a contralatersl coordinating neuron
             pExcIntRosEleCoord[mmSide][mmSeg],                                                                  // Intersegmental Exitation from and elevator to a rostral ipsilateral coordinating neuron
             pExcIntCaudEleCoord[mmSide][mmSeg],                                                                 // Intersegmental Exitation from and elevator to a caudal ipsilateral coordinating neuron
             */
        } //END for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        
    } // END: for(iSide = 0;iSide < mmSide; ++iSide) ...
    
    
    //    void calcSpikingNeuron(struct structSpiking *ptr,double cIe,double cIi)
    //    void calcBurstingNeuron(struct structBursting *ptr,double cIe,double cIi)
    
    
    for(iSide = 0;iSide < mmSide; ++iSide)
    {
        for(iSeg = 0;iSeg < mmSeg; ++iSeg)
        {
            calcSpikingNeuron(   &cellCoord[iSide][iSeg],      (iExcIntModComEle[iSide][iSeg]+ iExcSegContEleCoord[iSide][iSeg]+iExcIntRosEleCoord[iSide][iSeg]+iExcIntCaudEleCoord[iSide][iSeg]), 0);
          // calcPacemakerNeuron( &cellElevator[iSide][iSeg],    iExcIntModComEle[iSide][iSeg],      iInhSegCoordEle[iSide][iSeg]);
            calcBurstingNeuron( &cellElevator[iSide][iSeg],    iExcIntModComEle[iSide][iSeg],      iInhSegCoordEle[iSide][iSeg]);
            
            //Be sure to add terms for postural command inputs to the depressors.
            calcPacemakerNeuron(  &cellDepressor[iSide][iSeg],   iExcIntModComDep[iSide][iSeg],      iInhSegEleDep[iSide][iSeg]);  //modify here
            calcPacemakerNeuron(  &cellStance[iSide][iSeg],      iExcIntModComStance[iSide][iSeg],   iInhSegEleStance[iSide][iSeg]);
           
            // calcPacemakerNeuron(  &cellSwing[iSide][iSeg],       iExcIntModComSwing[iSide][iSeg],    iInhSegStanceSwing[iSide][iSeg]);
            calcBurstingNeuron(  &cellSwing[iSide][iSeg],       iExcIntModComSwing[iSide][iSeg],    iInhSegStanceSwing[iSide][iSeg]);
            
            //add iExcBackProt
        //    calcSpikingNeuron(   &cellProtractor[iSide][iSeg],  iExcSegStanceProt[iSide][iSeg]+ iExcSegSwingProt[iSide][iSeg] + iExcBackProt[iSide][iSeg], 0);
           calcSpikingNeuron(   &cellProtractor[iSide][iSeg],  iExcSegStanceProt[iSide][iSeg]+ iExcSegSwingProt[iSide][iSeg], 0);
            
            //add iExcForRet maybe?????? SEARch for me
       //     calcSpikingNeuron(   &cellRetractor[iSide][iSeg],   iExcSegStanceRet[iSide][iSeg] + iExcSegSwingRet[iSide][iSeg] + iExcForRet[iSide][iSeg],  0);
            calcSpikingNeuron(   &cellRetractor[iSide][iSeg],   iExcSegStanceRet[iSide][iSeg] + iExcSegSwingRet[iSide][iSeg],  0);

            
            //add iExcLTExt
            calcSpikingNeuron(   &cellExtensor[iSide][iSeg],    iExcSegStanceExt[iSide][iSeg] + iExcSegSwingExt[iSide][iSeg] ,  0);
            
            //add iExcLLFlx
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
        
        ///EDIT MODCOM ???
        //this should do the thresholding to calculate and pick the correct alpha and sigma for Modcom
        if (iExcForModCom[iSide] + iExcBackModCom[iSide] + iExcLLModCom[iSide] + iExcLTModCom[iSide] < 0.1){
        //Low
            cellModCom[iSide].spikingNeuron.alpha = 4.2;
            cellModCom[iSide].spikingNeuron.sigma= 0.2;
            
            
        }
        else if (iExcForModCom[iSide] + iExcBackModCom[iSide] + iExcLLModCom[iSide] + iExcLTModCom[iSide] < 0.5){
        //Med
            cellModCom[iSide].spikingNeuron.alpha = 5.4;
            cellModCom[iSide].spikingNeuron.sigma= 0.2;

        }
        else {
        //High
            cellModCom[iSide].spikingNeuron.alpha = 7;
            cellModCom[iSide].spikingNeuron.sigma= 0.2;

        }
        calcSpikingNeuron(   &cellModCom[iSide],  0 , 0);
        
        //calcSpikingNeuron(   &cellModCom[iSide],       iExcForModCom[iSide] + iExcBackModCom[iSide] + iExcLLModCom[iSide] + iExcLTModCom[iSide] , 0);           //Modulatory Command
        
        
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
