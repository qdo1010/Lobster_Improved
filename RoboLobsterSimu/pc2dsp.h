#ifndef _pc2dsp_h_
#define _pc2dsp_h_

//  #define runONdsp        // use when run the simulations of DSP disable send2dsp and write2files

#ifndef runONdsp
//    #define send2dsp	    // use if need to send spike data to DSP, comment it otherwise
#define write2files		// use if need to write data to files, comment it otherwise
#define drawOnOscilloscope  // use to store data to arrays to draw on the Oscilloscope
#endif // runONdsp

#define CMD_TIMEOUT_TIME      0x7FFFFFFF


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
double** xArrayF; //Forward Walking Command

double** xArrayB;             //Backward Walking Command
double** xArrayLL;                //Lateral Leading Command
double** xArrayLT;                 //Lateral Trailing Command
double** xArrayPcn; //[mmSide][pitchStates],	// There is one on each side for pLow, rosDown, pLevel, rosUp, pHigh The pitchState sets the synaptic strength in each segment/side
double** xArrayModCom;				// The is the modulatory command on each side
double** xArrayH;  //This is the hight command and would range fractionate and control the postural commands cellPcnLow cellPcnLevel and cellPcnHigh on both sides.


//////////Parameters to show to users

//alpha values
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

struct structBursting {
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, alpha, sigma, sigmaE, sigmaI, betaE, betaI, Idc;
    int spike;
}
//cellDepressor[mmSide][mmSeg],
//cellStance[mmSide][mmSeg];
cellSwing[mmSide][mmSeg],
cellElevator[mmSide][mmSeg];


//no endogenous pacemaker for now
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
//cellSwing[mmSide][mmSeg],
//cellElevator[mmSide][mmSeg];

/*
struct structEndogenousPacemaker {
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, alpha, sigma, sigmaE, sigmaI, betaE, betaI, Idc;
    int spike;
}
cellSwing[mmSide][mmSeg],
cellElevator[mmSide][mmSeg];
*/
//////////end of dictionary
void xmain();
int samplesizechosen = 50; //init w 50 as the default for now
int IterNumChosen = 1000;
int checkMainLoopIndex();
void indicateSampleSize(int s);
void indicateNumberOfIteration(int i);


int globalLoopIndex;

double *globalSigma;
double *globalAlpha; //params alpha beta for bursting and spiking neuron
double *globalSigmaE;
double *globalSigmaI;
double *globalBetaE;
double *globalBetaI;
double *globalIdc;

int globalSize;
int *globalCellName;
int *globalSide;
int *globalSeg;

int cellChosen;


int beginEditingParams = 0;//we have not edit the params
//function to set params
void setNeuronParams(int id, int side, int seg, double a, double s, double sE, double sI, double bE, double bI, double Idc); // change params

//function to set multiple neuron params
void setMultipleNeuronParam(int* idArr,int *sideArr, int*segArr,  double* aArr, double* sArr,double* sEArr, double* sIArr, double *bEArr, double *bIArr, double *IdcArr, int size); // change params


void chooseCell(int cellID);

//function where Objective C can call
void editParam(int *neuronName, int* side, int*seg, double *a, double *s, double *sE, double *sI, double *bE, double *bI, double *Idc, int size);
//int whatTypeofCell(char* neuronName);
long elapsed;

void allocParamArray(void); //alloc mem for param array
void freeParamsArray(void); //free param array

void checkParamsChange(void); //how to check? check if user from UI make any changes
#endif // _pc2dsp_h_
