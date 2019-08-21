#ifndef _pc2dsp_h_
#define _pc2dsp_h_

//  #define runONdsp        // use when run the simulations of DSP disable send2dsp and write2files

#ifndef runONdsp
//    #define send2dsp	    // use if need to send spike data to DSP, comment it otherwise
#define write2files		// use if need to write data to files, comment it otherwise
#define drawOnOscilloscope  // use to store data to arrays to draw on the Oscilloscope
#endif // runONdsp

#define CMD_TIMEOUT_TIME      0x7FFFFFFF

// This is a test again
// Converted cell numbers mmElevator, mmDepessor, etc. to mmSeg, mmSeg2
/*
 extern int cmdTimeOut;                     // cmd timeout down counter
 extern int triggered;                      // non zero when 1st pkt received
 extern int comSegmentMask;                 // enable / disable segments
 */
//int cmdTimeOut;                     // cmd timeout down counter
//int triggered;                      // non zero when 1st pkt received
//int comSegmentMask;                 // enable / disable segments

int cmdTimeOut= 0x7FFFFFFF;         // cmd timeout down counter
int triggered= 0;                   // non zero when 1st pkt received
int comSegmentMask= 0xFFFFFFFF;     // enable / disable segments

#define mmMot    1	            //  Number of Motor neurons in each hemisegment (about ??)
#define mmMot2   2*mmMot        //  Number of Motor neurons in each segment
#define	mmH	     2              //  Number of neurons in the RS control network (left/right)
#define mmH2     2*mmH          //  Headding neurons (Left/Right)
#define mmPcn    3              //  Number of neurons in the Pitch Control subNetwork (Ventral/Dorsal)
#define mmPcn2   2*mmPcn        //  Number of neurons in the Pitch Control Network (Ventral + Dorosal)
#define mmSeg    4              //  Number of Segments in a Spinal Cord model (110)
#define mmSeg2   2*mmSeg        //  Number of Segments in a Spinal Cord model (110)
#define mmSide    2             //  Number of Segments in a Spinal Cord model (110)
#define selfExcEinEin 0         // 0 Ein without Ein->Ein excitation within a segment, 1 - with ..
#define pitchStates 5			// pLow, rosDn, pLevel, rosUp, pHigh

#define TotalNeuronNumber 94
#define TotalSynapseNumber 336

//------------- Define the length of intersegmental connections --------------------------

#define cNextSeg   0.2 // leak segmental connectivity to nearby segments (0.2)
#define cCalInt    0.4 // Calibrate synapses between segments
#define cCalSeg    0.8 // Calibrate synapses within each segment
//********************* Cells classes *********************************************

enum  {left,right} side;
enum {pLow, rosDn, pLevel, rosUp, pHigh} pitch;
enum  {leftDown, rLevel, rightDown} roll;

// enum  {low, normal, high} height;
enum {
    cellElevatorL1,//EndogenousBursters
    cellElevatorL2,
    cellElevatorL3,
    cellElevatorL4,
    cellElevatorR1,
    cellElevatorR2,
    cellElevatorR3,
    cellElevatorR4,
    cellSwingL1,
    cellSwingL2,
    cellSwingL3,
    cellSwingL4,
    cellSwingR1,
    cellSwingR2,
    cellSwingR3,
    cellSwingR4,
    cellDepressorL1,//BurstingNeurons
    cellDepressorL2,
    cellDepressorL3,
    cellDepressorL4,
    cellDepressorR1,
    cellDepressorR2,
    cellDepressorR3,
    cellDepressorR4,
    cellStanceL1,
    cellStanceL2,
    cellStanceL3,
    cellStanceL4,
    cellStanceR1,
    cellStanceR2,
    cellStanceR3,
    cellStanceR4,
    cellCoordL1, //Segmental Spiking Neurons
    cellCoordL2,
    cellCoordL3,
    cellCoordL4,
    cellCoordR1,
    cellCoordR2,
    cellCoordR3,
    cellCoordR4,
    cellProtractorL1,
    cellProtractorL2,
    cellProtractorL3,
    cellProtractorL4,
    cellProtractorR1,
    cellProtractorR2,
    cellProtractorR3,
    cellProtractorR4,
    cellRetractorL1,
    cellRetractorL2,
    cellRetractorL3,
    cellRetractorL4,
    cellRetractorR1,
    cellRetractorR2,
    cellRetractorR3,
    cellRetractorR4,
    cellExtensorL1,
    cellExtensorL2,
    cellExtensorL3,
    cellExtensorL4,
    cellExtensorR1,
    cellExtensorR2,
    cellExtensorR3,
    cellExtensorR4,
    cellFlexorL1,
    cellFlexorL2,
    cellFlexorL3,
    cellFlexorL4,
    cellFlexorR1,
    cellFlexorR2,
    cellFlexorR3,
    cellFlexorR4,
    cellFL, //Forward Walking Command
    cellFR,
    cellBL, //Backward Walking Command
    cellBR,
    cellLLL, //Lateral Leading Command
    cellLLR,
    cellLTL, //Lateral Trailing Command
    cellLTR, 
    cellPcnLlow,	// There is one on each side for pLow, rosDown, pLevel, rosUp, pHigh The pitchState sets the synaptic strength in each segment/side
    cellPcnLrosDown,
    cellPcnLlevel,
    cellPcnLrosUp,
    cellPcnLpHigh,
    cellPcnRlow,	// There is one on each side for pLow, rosDown, pLevel, rosUp, pHigh The pitchState sets the synaptic strength in each segment/side
    cellPcnRrosDown,
    cellPcnRlevel,
    cellPcnRrosUp,
    cellPcnRpHigh,
    cellModComL,				// The is the modulatory command on each side
    cellModComR,				// The is the modulatory command on each side
    cellHL,
    cellHR,
    
} cellPointer;

enum {
    /*  //These have to stay commented until the old
    pCustom,
    pFastExc,
    pFastInh,
    pSlowExc,
    pSlowInh,
    */
    pInhSegCoordEleL1,
    pInhSegCoordEleL2,
    pInhSegCoordEleL3,
    pInhSegCoordEleL4,
    pInhSegCoordEleR1,
    pInhSegCoordEleR2,
    pInhSegCoordEleR3,
    pInhSegCoordEleR4,
    pExcSegContEleCoordL1,
    pExcSegContEleCoordL2,
    pExcSegContEleCoordL3,
    pExcSegContEleCoordL4,
    pExcSegContEleCoordR1,
    pExcSegContEleCoordR2,
    pExcSegContEleCoordR3,
    pExcSegContEleCoordR4,
    pExcIntRosEleCoordL1,
    pExcIntRosEleCoordL2,
    pExcIntRosEleCoordL3,
    pExcIntRosEleCoordL4,
    pExcIntRosEleCoordR1,
    pExcIntRosEleCoordR2,
    pExcIntRosEleCoordR3,
    pExcIntRosEleCoordR4,
    pExcIntCaudEleCoordL1,
    pExcIntCaudEleCoordL2,
    pExcIntCaudEleCoordL3,
    pExcIntCaudEleCoordL4,
    pExcIntCaudEleCoordR1,
    pExcIntCaudEleCoordR2,
    pExcIntCaudEleCoordR3,
    pExcIntCaudEleCoordR4,
    pInhIntFSwingL,
    pInhIntFSwingR,
    pInhIntFStanceL,
    pInhIntFStanceR,
    pInhIntBSwingL,
    pInhIntBSwingR,
    pInhIntBStanceL,
    pInhIntBStanceR,
    pInhIntLLSwingL,
    pInhIntLLSwingR,
    pInhIntLLStanceL,
    pInhIntLLStanceR,
    pInhIntLTSwingL,
    pInhIntLTSwingR,
    pInhIntLTStanceL,
    pInhIntLTStanceR,
    /*
     //This one was listed twice in the original structure, is that an error?
    pExcIntRosEleCoordL1,
    pExcIntRosEleCoordL2,
    pExcIntRosEleCoordL3,
    pExcIntRosEleCoordL4,
    pExcIntRosEleCoordR1,
    pExcIntRosEleCoordR2,
    pExcIntRosEleCoordR3,
    pExcIntRosEleCoordR4,
    */
    pExcIntRCaudEleCoordL1,
    pExcIntRCaudEleCoordL2,
    pExcIntRCaudEleCoordL3,
    pExcIntRCaudEleCoordL4,
    pExcIntRCaudEleCoordR1,
    pExcIntRCaudEleCoordR2,
    pExcIntRCaudEleCoordR3,
    pExcIntRCaudEleCoordR4,
    pExcSegEleContraLatL1,
    pExcSegEleContraLatL2,
    pExcSegEleContraLatL3,
    pExcSegEleContraLatL4,
    pExcSegEleContraLatR1,
    pExcSegEleContraLatR2,
    pExcSegEleContraLatR3,
    pExcSegEleContraLatR4,
    pInhSegEleDepL1,
    pInhSegEleDepL2,
    pInhSegEleDepL3,
    pInhSegEleDepL4,
    pInhSegEleDepR1,
    pInhSegEleDepR2,
    pInhSegEleDepR3,
    pInhSegEleDepR4,
    pInhSegEleStanceL1,
    pInhSegEleStanceL2,
    pInhSegEleStanceL3,
    pInhSegEleStanceL4,
    pInhSegEleStanceR1,
    pInhSegEleStanceR2,
    pInhSegEleStanceR3,
    pInhSegEleStanceR4,
    pInhSegStanceSwingL1,
    pInhSegStanceSwingL2,
    pInhSegStanceSwingL3,
    pInhSegStanceSwingL4,
    pInhSegStanceSwingR1,
    pInhSegStanceSwingR2,
    pInhSegStanceSwingR3,
    pInhSegStanceSwingR4,
    pExcSegStanceProtL1,
    pExcSegStanceProtL2,
    pExcSegStanceProtL3,
    pExcSegStanceProtL4,
    pExcSegStanceProtR1,
    pExcSegStanceProtR2,
    pExcSegStanceProtR3,
    pExcSegStanceProtR4,
    pExcSegStanceRetL1,
    pExcSegStanceRetL2,
    pExcSegStanceRetL3,
    pExcSegStanceRetL4,
    pExcSegStanceRetR1,
    pExcSegStanceRetR2,
    pExcSegStanceRetR3,
    pExcSegStanceRetR4,
    pExcSegStanceExtL1,
    pExcSegStanceExtL2,
    pExcSegStanceExtL3,
    pExcSegStanceExtL4,
    pExcSegStanceExtR1,
    pExcSegStanceExtR2,
    pExcSegStanceExtR3,
    pExcSegStanceExtR4,
    pExcSegStanceFlxL1,
    pExcSegStanceFlxL2,
    pExcSegStanceFlxL3,
    pExcSegStanceFlxL4,
    pExcSegStanceFlxR1,
    pExcSegStanceFlxR2,
    pExcSegStanceFlxR3,
    pExcSegStanceFlxR4,
    pExcSegSwingProtL1,
    pExcSegSwingProtL2,
    pExcSegSwingProtL3,
    pExcSegSwingProtL4,
    pExcSegSwingProtR1,
    pExcSegSwingProtR2,
    pExcSegSwingProtR3,
    pExcSegSwingProtR4,
    pExcSegSwingRetL1,
    pExcSegSwingRetL2,
    pExcSegSwingRetL3,
    pExcSegSwingRetL4,
    pExcSegSwingRetR1,
    pExcSegSwingRetR2,
    pExcSegSwingRetR3,
    pExcSegSwingRetR4,
    pExcSegSwingExtL1,
    pExcSegSwingExtL2,
    pExcSegSwingExtL3,
    pExcSegSwingExtL4,
    pExcSegSwingExtR1,
    pExcSegSwingExtR2,
    pExcSegSwingExtR3,
    pExcSegSwingExtR4,
    pExcSegSwingFlxL1,
    pExcSegSwingFlxL2,
    pExcSegSwingFlxL3,
    pExcSegSwingFlxL4,
    pExcSegSwingFlxR1,
    pExcSegSwingFlxR2,
    pExcSegSwingFlxR3,
    pExcSegSwingFlxR4,
    pExcHLYLL1,
    pExcHLYLL2,
    pExcHLYLL3,
    pExcHLYLL4,
    pExcHLYLR1,
    pExcHLYLR2,
    pExcHLYLR3,
    pExcHLYLR4,
    pExcHLRLL1,
    pExcHLRLL2,
    pExcHLRLL3,
    pExcHLRLL4,
    pExcHLRLR1,
    pExcHLRLR2,
    pExcHLRLR3,
    pExcHLRLR4,
    pExcYLFRL1,
    pExcYLFRL2,
    pExcYLFRL3,
    pExcYLFRL4,
    pExcYLFRR1,
    pExcYLFRR2,
    pExcYLFRR3,
    pExcYLFRR4,
    pExcRLFRL1,
    pExcRLFRL2,
    pExcRLFRL3,
    pExcRLFRL4,
    pExcRLFRR1,
    pExcRLFRR2,
    pExcRLFRR3,
    pExcRLFRR4,
    pExcHRYRL1,
    pExcHRYRL2,
    pExcHRYRL3,
    pExcHRYRL4,
    pExcHRYRR1,
    pExcHRYRR2,
    pExcHRYRR3,
    pExcHRYRR4,
    pExcHRRRL1,
    pExcHRRRL2,
    pExcHRRRL3,
    pExcHRRRL4,
    pExcHRRRR1,
    pExcHRRRR2,
    pExcHRRRR3,
    pExcHRRRR4,
    pExcYRFLL1,
    pExcYRFLL2,
    pExcYRFLL3,
    pExcYRFLL4,
    pExcYRFLR1,
    pExcYRFLR2,
    pExcYRFLR3,
    pExcYRFLR4,
    pExcRRFLL1,
    pExcRRFLL2,
    pExcRRFLL3,
    pExcRRFLL4,
    pExcRRFLR1,
    pExcRRFLR2,
    pExcRRFLR3,
    pExcRRFLR4,
    pExcRSLeftL1,
    pExcRSLeftL2,
    pExcRSLeftL3,
    pExcRSLeftL4,
    pExcRSLeftR1,
    pExcRSLeftR2,
    pExcRSLeftR3,
    pExcRSLeftR4,
    pExcRSRightL1,
    pExcRSRightL2,
    pExcRSRightL3,
    pExcRSRightL4,
    pExcRSRightR1,
    pExcRSRightR2,
    pExcRSRightR3,
    pExcRSRightR4,
    pExcSegPcnDepL1,
    pExcSegPcnDepL2,
    pExcSegPcnDepL3,
    pExcSegPcnDepL4,
    pExcSegPcnDepR1,
    pExcSegPcnDepR2,
    pExcSegPcnDepR3,
    pExcSegPcnDepR4,
    pExcCL1,
    pExcCL2,
    pExcCL3,
    pExcCL4,
    pExcCR1,
    pExcCR2,
    pExcCR3,
    pExcCR4,
    pInhFL1,
    pInhFL2,
    pInhFL3,
    pInhFL4,
    pInhFR1,
    pInhFR2,
    pInhFR3,
    pInhFR4,
    pExcBL1,
    pExcBL2,
    pExcBL3,
    pExcBL4,
    pExcBR1,
    pExcBR2,
    pExcBR3,
    pExcBR4,
    pExcModComEleL1,
    pExcModComEleL2,
    pExcModComEleL3,
    pExcModComEleL4,
    pExcModComEleR1,
    pExcModComEleR2,
    pExcModComEleR3,
    pExcModComEleR4,
    pExcModComDepL1,
    pExcModComDepL2,
    pExcModComDepL3,
    pExcModComDepL4,
    pExcModComDepR1,
    pExcModComDepR2,
    pExcModComDepR3,
    pExcModComDepR4,
    pExcModComSwingL1,
    pExcModComSwingL2,
    pExcModComSwingL3,
    pExcModComSwingL4,
    pExcModComSwingR1,
    pExcModComSwingR2,
    pExcModComSwingR3,
    pExcModComSwingR4,
    pExcModComStanceL1,
    pExcModComStanceL2,
    pExcModComStanceL3,
    pExcModComStanceL4,
    pExcModComStanceR1,
    pExcModComStanceR2,
    pExcModComStanceR3,
    pExcModComStanceR4,
    pExcForRetL1,
    pExcForRetL2,
    pExcForRetL3,
    pExcForRetL4,
    pExcForRetR1,
    pExcForRetR2,
    pExcForRetR3,
    pExcForRetR4,
    pExcBackProtL1,
    pExcBackProtL2,
    pExcBackProtL3,
    pExcBackProtL4,
    pExcBackProtR1,
    pExcBackProtR2,
    pExcBackProtR3,
    pExcBackProtR4,
    pExcLLFlxL1,
    pExcLLFlxL2,
    pExcLLFlxL3,
    pExcLLFlxL4,
    pExcLLFlxR1,
    pExcLLFlxR2,
    pExcLLFlxR3,
    pExcLLFlxR4,
    pExcLTExtL1,
    pExcLTExtL2,
    pExcLTExtL3,
    pExcLTExtL4,
    pExcLTExtR1,
    pExcLTExtR2,
    pExcLTExtR3,
    pExcLTExtR4,
    pExcForModComL,
    pExcForModComR,
    pExcBackModComL,
    pExcBackModComR,
    pExcLLModComL,
    pExcLLModComR,
    pExcLTModComL,
    pExcLTModComR
} synapsePointer;

char * synapseLabels[] = {
    "pCustom",
    "pFastExc",
    "pFastInh",
    "pSlowExc",
    "pSlowInh",
    "pInhSegCoordEleL1",
    "pInhSegCoordEleL2",
    "pInhSegCoordEleL3",
    "pInhSegCoordEleL4",
    "pInhSegCoordEleR1",
    "pInhSegCoordEleR2",
    "pInhSegCoordEleR3",
    "pInhSegCoordEleR4",
    "pExcSegContEleCoordL1",
    "pExcSegContEleCoordL2",
    "pExcSegContEleCoordL3",
    "pExcSegContEleCoordL4",
    "pExcSegContEleCoordR1",
    "pExcSegContEleCoordR2",
    "pExcSegContEleCoordR3",
    "pExcSegContEleCoordR4",
    "pExcIntRosEleCoordL1",
    "pExcIntRosEleCoordL2",
    "pExcIntRosEleCoordL3",
    "pExcIntRosEleCoordL4",
    "pExcIntRosEleCoordR1",
    "pExcIntRosEleCoordR2",
    "pExcIntRosEleCoordR3",
    "pExcIntRosEleCoordR4",
    "pExcIntCaudEleCoordL1",
    "pExcIntCaudEleCoordL2",
    "pExcIntCaudEleCoordL3",
    "pExcIntCaudEleCoordL4",
    "pExcIntCaudEleCoordR1",
    "pExcIntCaudEleCoordR2",
    "pExcIntCaudEleCoordR3",
    "pExcIntCaudEleCoordR4",
    "pInhIntFSwingL",
    "pInhIntFSwingR",
    "pInhIntFStanceL",
    "pInhIntFStanceR",
    "pInhIntBSwingL",
    "pInhIntBSwingR",
    "pInhIntBStanceL",
    "pInhIntBStanceR",
    "pInhIntLLSwingL",
    "pInhIntLLSwingR",
    "pInhIntLLStanceL",
    "pInhIntLLStanceR",
    "pInhIntLTSwingL",
    "pInhIntLTSwingR",
    "pInhIntLTStanceL",
    "pInhIntLTStanceR",
    /*
     //This one was listed twice in the original structure, is that an error?
    "pExcIntRosEleCoordL1",
    "pExcIntRosEleCoordL2",
    "pExcIntRosEleCoordL3",
    "pExcIntRosEleCoordL4",
    "pExcIntRosEleCoordR1",
    "pExcIntRosEleCoordR2",
    "pExcIntRosEleCoordR3",
    "pExcIntRosEleCoordR4",
    */
    "pExcIntRCaudEleCoordL1",
    "pExcIntRCaudEleCoordL2",
    "pExcIntRCaudEleCoordL3",
    "pExcIntRCaudEleCoordL4",
    "pExcIntRCaudEleCoordR1",
    "pExcIntRCaudEleCoordR2",
    "pExcIntRCaudEleCoordR3",
    "pExcIntRCaudEleCoordR4",
    "pExcSegEleContraLatL1",
    "pExcSegEleContraLatL2",
    "pExcSegEleContraLatL3",
    "pExcSegEleContraLatL4",
    "pExcSegEleContraLatR1",
    "pExcSegEleContraLatR2",
    "pExcSegEleContraLatR3",
    "pExcSegEleContraLatR4"
    "pInhSegEleDepL1",
    "pInhSegEleDepL2",
    "pInhSegEleDepL3",
    "pInhSegEleDepL4",
    "pInhSegEleDepR1",
    "pInhSegEleDepR2",
    "pInhSegEleDepR3",
    "pInhSegEleDepR4",
    "pInhSegEleStanceL1",
    "pInhSegEleStanceL2",
    "pInhSegEleStanceL3",
    "pInhSegEleStanceL4",
    "pInhSegEleStanceR1",
    "pInhSegEleStanceR2",
    "pInhSegEleStanceR3",
    "pInhSegEleStanceR4",
    "pInhSegStanceSwingL1",
    "pInhSegStanceSwingL2",
    "pInhSegStanceSwingL3",
    "pInhSegStanceSwingL4",
    "pInhSegStanceSwingR1",
    "pInhSegStanceSwingR2",
    "pInhSegStanceSwingR3",
    "pInhSegStanceSwingR4",
    "pExcSegStanceProtL1",
    "pExcSegStanceProtL2",
    "pExcSegStanceProtL3",
    "pExcSegStanceProtL4",
    "pExcSegStanceProtR1",
    "pExcSegStanceProtR2",
    "pExcSegStanceProtR3",
    "pExcSegStanceProtR4",
    "pExcSegStanceRetL1",
    "pExcSegStanceRetL2",
    "pExcSegStanceRetL3",
    "pExcSegStanceRetL4",
    "pExcSegStanceRetR1",
    "pExcSegStanceRetR2",
    "pExcSegStanceRetR3",
    "pExcSegStanceRetR4",
    "pExcSegStanceExtL1",
    "pExcSegStanceExtL2",
    "pExcSegStanceExtL3",
    "pExcSegStanceExtL4",
    "pExcSegStanceExtR1",
    "pExcSegStanceExtR2",
    "pExcSegStanceExtR3",
    "pExcSegStanceExtR4",
    "pExcSegStanceFlxL1",
    "pExcSegStanceFlxL2",
    "pExcSegStanceFlxL3",
    "pExcSegStanceFlxL4",
    "pExcSegStanceFlxR1",
    "pExcSegStanceFlxR2",
    "pExcSegStanceFlxR3",
    "pExcSegStanceFlxR4",
    "pExcSegSwingProtL1",
    "pExcSegSwingProtL2",
    "pExcSegSwingProtL3",
    "pExcSegSwingProtL4",
    "pExcSegSwingProtR1",
    "pExcSegSwingProtR2",
    "pExcSegSwingProtR3",
    "pExcSegSwingProtR4",
    "pExcSegSwingRetL1",
    "pExcSegSwingRetL2",
    "pExcSegSwingRetL3",
    "pExcSegSwingRetL4",
    "pExcSegSwingRetR1",
    "pExcSegSwingRetR2",
    "pExcSegSwingRetR3",
    "pExcSegSwingRetR4",
    "pExcSegSwingExtL1",
    "pExcSegSwingExtL2",
    "pExcSegSwingExtL3",
    "pExcSegSwingExtL4",
    "pExcSegSwingExtR1",
    "pExcSegSwingExtR2",
    "pExcSegSwingExtR3",
    "pExcSegSwingExtR4",
    "pExcSegSwingFlxL1",
    "pExcSegSwingFlxL2",
    "pExcSegSwingFlxL3",
    "pExcSegSwingFlxL4",
    "pExcSegSwingFlxR1",
    "pExcSegSwingFlxR2",
    "pExcSegSwingFlxR3",
    "pExcSegSwingFlxR4",
    "pExcHLYLL1",
    "pExcHLYLL2",
    "pExcHLYLL3",
    "pExcHLYLL4",
    "pExcHLYLR1",
    "pExcHLYLR2",
    "pExcHLYLR3",
    "pExcHLYLR4",
    "pExcHLRLL1",
    "pExcHLRLL2",
    "pExcHLRLL3",
    "pExcHLRLL4",
    "pExcHLRLR1",
    "pExcHLRLR2",
    "pExcHLRLR3",
    "pExcHLRLR4",
    "pExcYLFRL1",
    "pExcYLFRL2",
    "pExcYLFRL3",
    "pExcYLFRL4",
    "pExcYLFRR1",
    "pExcYLFRR2",
    "pExcYLFRR3",
    "pExcYLFRR4",
    "pExcRLFRL1",
    "pExcRLFRL2",
    "pExcRLFRL3",
    "pExcRLFRL4",
    "pExcRLFRR1",
    "pExcRLFRR2",
    "pExcRLFRR3",
    "pExcRLFRR4",
    "pExcHRYRL1",
    "pExcHRYRL2",
    "pExcHRYRL3",
    "pExcHRYRL4",
    "pExcHRYRR1",
    "pExcHRYRR2",
    "pExcHRYRR3",
    "pExcHRYRR4",
    "pExcHRRRL1",
    "pExcHRRRL2",
    "pExcHRRRL3",
    "pExcHRRRL4",
    "pExcHRRRR1",
    "pExcHRRRR2",
    "pExcHRRRR3",
    "pExcHRRRR4",
    "pExcYRFLL1",
    "pExcYRFLL2",
    "pExcYRFLL3",
    "pExcYRFLL4",
    "pExcYRFLR1",
    "pExcYRFLR2",
    "pExcYRFLR3",
    "pExcYRFLR4",
    "pExcRRFLL1",
    "pExcRRFLL2",
    "pExcRRFLL3",
    "pExcRRFLL4",
    "pExcRRFLR1",
    "pExcRRFLR2",
    "pExcRRFLR3",
    "pExcRRFLR4",
    "pExcRSLeftL1",
    "pExcRSLeftL2",
    "pExcRSLeftL3",
    "pExcRSLeftL4",
    "pExcRSLeftR1",
    "pExcRSLeftR2",
    "pExcRSLeftR3",
    "pExcRSLeftR4",
    "pExcRSRightL1",
    "pExcRSRightL2",
    "pExcRSRightL3",
    "pExcRSRightL4",
    "pExcRSRightR1",
    "pExcRSRightR2",
    "pExcRSRightR3",
    "pExcRSRightR4",
    "pExcSegPcnDepL1",
    "pExcSegPcnDepL2",
    "pExcSegPcnDepL3",
    "pExcSegPcnDepL4",
    "pExcSegPcnDepR1",
    "pExcSegPcnDepR2",
    "pExcSegPcnDepR3",
    "pExcSegPcnDepR4",
    "pExcCL1",
    "pExcCL2",
    "pExcCL3",
    "pExcCL4",
    "pExcCR1",
    "pExcCR2",
    "pExcCR3",
    "pExcCR4",
    "pInhFL1",
    "pInhFL2",
    "pInhFL3",
    "pInhFL4",
    "pInhFR1",
    "pInhFR2",
    "pInhFR3",
    "pInhFR4",
    "pExcBL1",
    "pExcBL2",
    "pExcBL3",
    "pExcBL4",
    "pExcBR1",
    "pExcBR2",
    "pExcBR3",
    "pExcBR4",
    "pExcModComEleL1",
    "pExcModComEleL2",
    "pExcModComEleL3",
    "pExcModComEleL4",
    "pExcModComEleR1",
    "pExcModComEleR2",
    "pExcModComEleR3",
    "pExcModComEleR4",
    "pExcModComDepL1",
    "pExcModComDepL2",
    "pExcModComDepL3",
    "pExcModComDepL4",
    "pExcModComDepR1",
    "pExcModComDepR2",
    "pExcModComDepR3",
    "pExcModComDepR4",
    "pExcModComSwingL1",
    "pExcModComSwingL2",
    "pExcModComSwingL3",
    "pExcModComSwingL4",
    "pExcModComSwingR1",
    "pExcModComSwingR2",
    "pExcModComSwingR3",
    "pExcModComSwingR4",
    "pExcModComStanceL1",
    "pExcModComStanceL2",
    "pExcModComStanceL3",
    "pExcModComStanceL4",
    "pExcModComStanceR1",
    "pExcModComStanceR2",
    "pExcModComStanceR3",
    "pExcModComStanceR4",
    "pExcForRetL1",
    "pExcForRetL2",
    "pExcForRetL3",
    "pExcForRetL4",
    "pExcForRetR1",
    "pExcForRetR2",
    "pExcForRetR3",
    "pExcForRetR4",
    "pExcBackProtL1",
    "pExcBackProtL2",
    "pExcBackProtL3",
    "pExcBackProtL4",
    "pExcBackProtR1",
    "pExcBackProtR2",
    "pExcBackProtR3",
    "pExcBackProtR4",
    "pExcLLFlxL1",
    "pExcLLFlxL2",
    "pExcLLFlxL3",
    "pExcLLFlxL4",
    "pExcLLFlxR1",
    "pExcLLFlxR2",
    "pExcLLFlxR3",
    "pExcLLFlxR4",
    "pExcLTExtL1",
    "pExcLTExtL2",
    "pExcLTExtL3",
    "pExcLTExtL4",
    "pExcLTExtR1",
    "pExcLTExtR2",
    "pExcLTExtR3",
    "pExcLTExtR4",
    "pExcForModComL",
    "pExcForModComR",
    "pExcBackModComL",
    "pExcBackModComR",
    "pExcLLModComL",
    "pExcLLModComR",
    "pExcLTModComL",
    "pExcLTModComR"
};

char * traceLabels[] = {
    "cellElevatorL1",
    "cellElevatorL2",
    "cellElevatorL3",
    "cellElevatorL4",
    "cellElevatorR1",
    "cellElevatorR2",
    "cellElevatorR3",
    "cellElevatorR4",
    "cellSwingL1",
    "cellSwingL2",
    "cellSwingL3",
    "cellSwingL4",
    "cellSwingR1",
    "cellSwingR2",
    "cellSwingR3",
    "cellSwingR4",
    "cellDepressorL1",
    "cellDepressorL2",
    "cellDepressorL3",
    "cellDepressorL4",
    "cellDepressorR1",
    "cellDepressorR2",
    "cellDepressorR3",
    "cellDepressorR4",
    "cellStanceL1",
    "cellStanceL2",
    "cellStanceL3",
    "cellStanceL4",
    "cellStanceR1",
    "cellStanceR2",
    "cellStanceR3",
    "cellStanceR4",
    "cellCoordL1",
    "cellCoordL2",
    "cellCoordL3",
    "cellCoordL4",
    "cellCoordR1",
    "cellCoordR2",
    "cellCoordR3",
    "cellCoordR4",
    "cellProtractorL1",
    "cellProtractorL2",
    "cellProtractorL3",
    "cellProtractorL4",
    "cellProtractorR1",
    "cellProtractorR2",
    "cellProtractorR3",
    "cellProtractorR4",
    "cellRetractorL1",
    "cellRetractorL2",
    "cellRetractorL3",
    "cellRetractorL4",
    "cellRetractorR1",
    "cellRetractorR2",
    "cellRetractorR3",
    "cellRetractorR4",
    "cellExtensorL1",
    "cellExtensorL2",
    "cellExtensorL3",
    "cellExtensorL4",
    "cellExtensorR1",
    "cellExtensorR2",
    "cellExtensorR3",
    "cellExtensorR4",
    "cellFlexorL1",
    "cellFlexorL2",
    "cellFlexorL3",
    "cellFlexorL4",
    "cellFlexorR1",
    "cellFlexorR2",
    "cellFlexorR3",
    "cellFlexorR4",
    "cellFL",
    "cellFR",
    "cellBL",
    "cellBR",
    "cellLLL",
    "cellLLR",
    "cellLTL",
    "cellLTR",
    "cellPcnLlow",
    "cellPcnLrosDown",
    "cellPcnLlevel",
    "cellPcnLrosUp",
    "cellPcnLpHigh",
    "cellPcnRlow",
    "cellPcnRrosDown",
    "cellPcnRlevel",
    "cellPcnRrosUp",
    "cellPcnRpHigh",
    "cellModComL",
    "cellModComR",
    "cellHL",
    "cellHR"
};

/*
enum {
    
} synapsePointer;
*/

enum{
    BurstingNeuron,
    SpikingNeuron,
    PacemakerNeuron
};

typedef struct neuronStruc{
    char *name;
    int type;
    int side;
    int seg;
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, sigmaE, sigmaI, betaE, betaI, Idc;
    double alpha;
    double sigma;
    int spike;
    double yr, xr, x2, y2;
    double alphaInit;
} Neuron;

Neuron Neurons[TotalNeuronNumber];

typedef struct synaspeStruc{
    char *name;
    int type;
    int side;
    int seg;
    double xRp;            //Reversal Potential
    double gamma;          //Time Constant
    double gStrength;
} Synapse;

Synapse Synapses[TotalSynapseNumber];


int numCells = 92;
//x array

double** xArrayElev;
double** xArrayDep;
double** xArraySwing;
double** xArrayStance;
double** xArrayProt;
double** xArrayRet;
double** xArrayExt;
double** xArrayFlex;
double** xArrayCoord;

//command neuron array;
double** xArrayF;               //Forward Walking Command
double** xArrayB;                //Backward Walking Command
double** xArrayLL;               //Lateral Leading Command
double** xArrayLT;               //Lateral Trailing Command
double** xArrayPcn; //[mmSide][pitchStates],	// There is one on each side for pLow, rosDown, pLevel, rosUp, pHigh The pitchState sets the synaptic strength in each segment/side
double** xArrayModCom;				// The is the modulatory command on each side
double** xArrayH;  //This is the hight command and would range fractionate and control the postural commands cellPcnLow cellPcnLevel and cellPcnHigh on both sides.


//////////Arrays that will store the Neuron parameters, to be sent to and received by Objective C code to display to users
//alpha values
/*
double** alphaArrayElev;
double** alphaArrayDep;
double** alphaArraySwing;
double** alphaArrayStance;
double** alphaArrayProt;
double** alphaArrayRet;
double** alphaArrayExt;
double** alphaArrayFlex;
double** alphaArrayCoord;

double** alphaArrayF;
double** alphaArrayB;
double** alphaArrayLL;
double** alphaArrayLT;
double** alphaArrayPcn;
double** alphaArrayModCom;
double** alphaArrayH;

//sigma values
double** sigmaArrayElev;
double** sigmaArrayDep;
double** sigmaArraySwing;
double** sigmaArrayStance;
double** sigmaArrayProt;
double** sigmaArrayRet;
double** sigmaArrayExt;
double** sigmaArrayFlex;
double** sigmaArrayCoord;

double** sigmaArrayF;
double** sigmaArrayB;
double** sigmaArrayLL;
double** sigmaArrayLT;
double** sigmaArrayPcn;
double** sigmaArrayModCom;
double** sigmaArrayH;

//sE
double** sigmaEArrayElev;
double** sigmaEArrayDep;
double** sigmaEArraySwing;
double** sigmaEArrayStance;
double** sigmaEArrayProt;
double** sigmaEArrayRet;
double** sigmaEArrayExt;
double** sigmaEArrayFlex;
double** sigmaEArrayCoord;

double**  sigmaEArrayF;
double**  sigmaEArrayB;
double**  sigmaEArrayLL;
double**  sigmaEArrayLT;
double**  sigmaEArrayPcn;
double**  sigmaEArrayModCom;
double**  sigmaEArrayH;

//sI
double**  sigmaIArrayElev;
double**  sigmaIArrayDep;
double**  sigmaIArraySwing;
double**  sigmaIArrayStance;
double**  sigmaIArrayProt;
double**  sigmaIArrayRet;
double**  sigmaIArrayExt;
double**  sigmaIArrayFlex;
double**  sigmaIArrayCoord;

double**  sigmaIArrayF;
double**  sigmaIArrayB;
double**  sigmaIArrayLL;
double**  sigmaIArrayLT;
double**  sigmaIArrayPcn;
double**  sigmaIArrayModCom;
double**  sigmaIArrayH;

//bE
double**  betaEArrayElev;
double**  betaEArrayDep;
double**  betaEArraySwing;
double**  betaEArrayStance;
double**  betaEArrayProt;
double**  betaEArrayRet;
double**  betaEArrayExt;
double**  betaEArrayFlex;
double**  betaEArrayCoord;

double**  betaEArrayF;
double**  betaEArrayB;
double**  betaEArrayLL;
double**  betaEArrayLT;
double**  betaEArrayPcn;
double**  betaEArrayModCom;
double**  betaEArrayH;

//bI
double**  betaIArrayElev;
double**  betaIArrayDep;
double**  betaIArraySwing;
double**  betaIArrayStance;
double**  betaIArrayProt;
double**  betaIArrayRet;
double**  betaIArrayExt;
double**  betaIArrayFlex;
double**  betaIArrayCoord;

double**  betaIArrayF;
double**  betaIArrayB;
double**  betaIArrayLL;
double**  betaIArrayLT;
double**  betaIArrayPcn;
double**  betaIArrayModCom;
double**  betaIArrayH;

//Idc
double**  IdcArrayElev;
double**  IdcArrayDep;
double**  IdcArraySwing;
double**  IdcArrayStance;
double**  IdcArrayProt;
double**  IdcArrayRet;
double**  IdcArrayExt;
double**  IdcArrayFlex;
double**  IdcArrayCoord;

double**  IdcArrayF;
double**  IdcArrayB;
double**  IdcArrayLL;
double**  IdcArrayLT;
double**  IdcArrayPcn;
double**  IdcArrayModCom;
double**  IdcArrayH;
*/
//Struct to define param for spiking neurons
//Old structure designs with seperate structs
/*
struct structSpiking {
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, alpha, sigma, sigmaE, sigmaI, betaE, betaI, Idc;
    int spike;
}
cellF[mmSide],                  //Forward Walking Command
cellB[mmSide],                  //Backward Walking Command
cellLL[mmSide],                 //Lateral Leading Command
cellLT[mmSide],                 //Lateral Trailing Command
cellPcn[mmSide][pitchStates],	// There is one on each side for pLow, rosDown, pLevel, rosUp, pHigh The pitchState sets the synaptic strength in each segment/side
cellModCom[mmSide],				// The is the modulatory command on each side
cellH[mmSide],
cellCoord[mmSide][mmSeg],       //Coordinating neuron for each segment
cellProtractor[mmSide][mmSeg],
cellRetractor[mmSide][mmSeg],
cellExtensor[mmSide][mmSeg],
cellFlexor[mmSide][mmSeg];
//cellMot[mmSide][mmSeg],

//Struct to define param for bursting neurons
//Swing and Elevator are Bursting Neurons?
struct structBursting {
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, alpha, sigma, sigmaE, sigmaI, betaE, betaI, Idc;
    int spike;
}
cellSwing[mmSide][mmSeg],
cellElevator[mmSide][mmSeg];

//Struct to define param for endogenous pacemaker neurons
//Depressor and Stance are Endogenous Pacemaker?
struct structEndogenousPacemaker {
    double alpha, sigma, mu;
    double yr, xr, x2, y2;
    double x, xp, xpp, sigmaIn, betaIn; //added sigma In and betaIn
    double sigmaE, sigmaI, betaE, betaI, Idc; //added
    double sigmaDc, betaDc; //added
    double alphaInit;
    int spike;
}
cellDepressor[mmSide][mmSeg],
cellStance[mmSide][mmSeg];
*/

// New structure design

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

paramStruct
//Spiking Neurons
cellF[mmSide],                  //Forward Walking Command
cellB[mmSide],                  //Backward Walking Command
cellLL[mmSide],                 //Lateral Leading Command
cellLT[mmSide], cellPcn[mmSide][pitchStates],                //Lateral Trailing Command   // There is one on each side for pLow, rosDown, pLevel, rosUp, pHigh The pitchState sets the synaptic strength in each segment/side
cellModCom[mmSide],                // The is the modulatory command on each side
cellH[mmSide],
cellCoord[mmSide][mmSeg],       //Coordinating neuron for each segment
cellProtractor[mmSide][mmSeg],
cellRetractor[mmSide][mmSeg],
cellExtensor[mmSide][mmSeg],
cellFlexor[mmSide][mmSeg],


//Bursting Neurons
cellSwing[mmSide][mmSeg],
cellElevator[mmSide][mmSeg],


//Endogenous Pacemaker
cellDepressor[mmSide][mmSeg],
cellStance[mmSide][mmSeg],


//Synapses
pCustom,  //custom synapse
pFastExc, pFastInh, pSlowExc, pSlowInh,                                                              // SynapseTypes
pInhSegCoordEle[mmSide][mmSeg],                                                                      // Segmental Inhibition from an Coordinating Neuron to an Elevator
pExcSegContEleCoord[mmSide][mmSeg],                                                                  // Intersegmental Exitation from and elevator to a contralatersl coordinating neuron
pExcIntRosEleCoord[mmSide][mmSeg],pExcIntCaudEleCoord[mmSide][mmSeg],                                // Intersegmental Exitation from and elevator to a rostral or caudal ipsilateral coordinating neuron
pInhIntFSwing[mmSide], pInhIntFStance[mmSide], pInhIntBSwing[mmSide],pInhIntBStance[mmSide],         // Presynaptic Inhibition from a command to bifunctional interneuron synapse
pInhIntLLSwing[mmSide],pInhIntLLStance[mmSide],pInhIntLTSwing[mmSide],pInhIntLTStance[mmSide],       // Presynaptic Inhibition from a command to bifunctional interneuron synapse
pExcIntRosEleCoord[mmSide][mmSeg],  pExcIntRCaudEleCoord[mmSide][mmSeg],                             //Excitatory Synapses from Elevator to adjacent coordinating neurons
pExcSegEleContraLat[mmSide][mmSeg],                                                                    //Excitatory Synapses from Elevator to contralateral coordinating neurons
pInhSegEleDep[mmSide][mmSeg],pInhSegEleStance[mmSide][mmSeg],pInhSegStanceSwing[mmSide][mmSeg],      //Inhibitory Synapses from Elevator to Depressor and Swing Interneurons
pExcSegStanceProt[mmSide][mmSeg],pExcSegStanceRet[mmSide][mmSeg],pExcSegStanceExt[mmSide][mmSeg],pExcSegStanceFlx[mmSide][mmSeg],   //Excitatory Synapses from Stance to Bifuncitonal Motor Neurons
pExcSegSwingProt[mmSide][mmSeg], pExcSegSwingRet[mmSide][mmSeg], pExcSegSwingExt[mmSide][mmSeg],pExcSegSwingFlx[mmSide][mmSeg],     //Excitatory Synapses from Swing to Biunctional Neurons
pExcHLYL[mmSide][mmSeg],pExcHLRL[mmSide][mmSeg],pExcYLFR[mmSide][mmSeg],pExcRLFR[mmSide][mmSeg],
pExcHRYR[mmSide][mmSeg],pExcHRRR[mmSide][mmSeg],pExcYRFL[mmSide][mmSeg],pExcRRFL[mmSide][mmSeg],
pExcRSLeft[mmSide][mmSeg],pExcRSRight[mmSide][mmSeg],
pExcSegPcnDep[mmSide][mmSeg],                                                                        //Synapse between pitch Command and segmental depressors
pExcC[mmSide][mmSeg], pInhF[mmSide][mmSeg], pExcB[mmSide][mmSeg],
pExcModComEle[mmSide][mmSeg],pExcModComDep[mmSide][mmSeg], pExcModComSwing[mmSide][mmSeg], pExcModComStance[mmSide][mmSeg],//Excitatory synapses from modulatory Commands to CPG Neurons
pExcForRet[mmSide][mmSeg], pExcBackProt[mmSide][mmSeg], pExcLLFlx[mmSide][mmSeg], pExcLTExt[mmSide][mmSeg],     //Excitatory Synapses from Directional Commands to propulsive synergies
pExcForModCom[mmSide], pExcBackModCom[mmSide], pExcLLModCom[mmSide], pExcLTModCom[mmSide];

void xmain(void); //this is the main C function that will always run when program starts

int IterNumChosen = 1000000;

void indicateNumberOfIteration(int i); //function to change IterNumChosen


int globalLoopIndex;

//cell
double *globalSigma;
double *globalAlpha; //params alpha beta for bursting and spiking neuron
double *globalSigmaE;
double *globalSigmaI;
double *globalBetaE;
double *globalBetaI;
double *globalIdc;

unsigned long int globalSize;
int *globalCellName;
unsigned long *globalSide;
unsigned long *globalSeg;

//synapse
unsigned long int *globalSynapseName;

double *globalXrp;
double *globalGamma;
double *globalgStrength;

//flag
int beginEditingParams = 0;//we have not edited the params

//flag
int beginEditingSynapse = 0; //we have not edited the synapse

//flag to write to file
int writeToFile = 0;


//function to set synapse params
void setSynapseParams(unsigned long int id, unsigned long side, unsigned long seg, double xrp, double gamma, double gStrength);

//function to set multiple synapse params
void setMultipleSynapseParams(unsigned long int* idArr, unsigned long *sideArr, unsigned long *segArr, double* xrpArr, double* gammaArr, double* gStrengthArr, unsigned long int size);

//function to set neuron params
void setNeuronParams(int id, unsigned long side, unsigned long seg, double a, double s, double sE, double sI, double bE, double bI, double Idc); // change params

//function to set multiple neuron params
void setMultipleNeuronParams(int* idArr, unsigned long *sideArr, unsigned long *segArr,  double* aArr, double* sArr,double* sEArr, double* sIArr, double *bEArr, double *bIArr, double *IdcArr, unsigned long int size); // change params


//function that Objective C can call to edit synapse
void editSynapseParam(unsigned long int *synapseName, unsigned long *side, unsigned long *seg,double *xrp, double *gamma, double *gStrength, int size);
//function that Objective C can call to edit neurons
void editParam(int *neuronName, unsigned long *side, unsigned long *seg, double *a, double *s, double *sE, double *sI, double *bE, double *bI, double *Idc, unsigned long int size);
//function that Objective C can call to write to file



void SaveAllParams(void);

void CreateParamFile (void);

void LoadAllParams (void);

void CreateReadableParams (void);

//int whatTypeofCell(char* neuronName);
long elapsed;

//void allocParamArray(void); //alloc mem for param array
//void freeParamsArray(void); //free param array

#endif // _pc2dsp_h_
