
//  Waveforms.h
//  RoboLobster
//
//  Created by Joseph Ayers on 4/2/17.
//  Copyright © 2017 Joseph Ayers. All rights reserved.
//
/*
 #include <stdio.h>
 
 #ifndef Waveforms_h
 #define Waveforms_h
 #define numSamples = 5000;
 
//@interface Waveforms : NSObject
 
 
 double ibuf[8][5000];
 #endif * Waveforms_h */

//@end
//#include "pc2dsp.h"
#import <Cocoa/Cocoa.h>

@class AppDelegate;

@interface Waveforms : NSObject{
    CFTimeInterval sTime;

}



//@property (nonatomic, assign) double ibuf[8][5000];
typedef enum {
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




@property (nonatomic, retain) NSMutableArray *ipbuf;
@property (nonatomic, strong) NSMutableArray *parambuf;
@property (nonatomic, strong) NSMutableArray *result;

@property (readwrite) int cellIndex;
@property (readwrite) int cellTypeIndex;
@property (readwrite) int globalCounter;
//cpg
extern double** xArrayElev;
extern double** xArrayDep;
extern double** xArraySwing;
extern double** xArrayStance;
extern double** xArrayProt;
extern double** xArrayRet;
extern double** xArrayExt;
extern double** xArrayFlex;
extern double** xArrayCoord;

//command
extern double** xArrayF;
extern double** xArrayB;
extern double** xArrayLL;
extern double** xArrayLT;
extern double** xArrayModCom;
extern double** xArrayPcn;
extern double** xArrayH;

//alpha array
extern double** alphaArrayElev;
extern double** alphaArrayDep;
extern double** alphaArraySwing;
extern double** alphaArrayStance;
extern double** alphaArrayProt;
extern double** alphaArrayRet;
extern double** alphaArrayExt;
extern double** alphaArrayFlex;
extern double** alphaArrayCoord;

extern double** alphaArrayF;
extern double** alphaArrayB;
extern double** alphaArrayLL;
extern double** alphaArrayLT;
extern double** alphaArrayPcn;
extern double** alphaArrayModCom;
extern double** alphaArrayH;


//sigma arr
extern double** sigmaArrayElev;
extern double** sigmaArrayDep;
extern double** sigmaArraySwing;
extern double** sigmaArrayStance;
extern double** sigmaArrayProt;
extern double** sigmaArrayRet;
extern double** sigmaArrayExt;
extern double** sigmaArrayFlex;
extern double** sigmaArrayCoord;

extern double** sigmaArrayF;
extern double** sigmaArrayB;
extern double** sigmaArrayLL;
extern double** sigmaArrayLT;
extern double** sigmaArrayPcn;
extern double** sigmaArrayModCom;
extern double** sigmaArrayH;


//sE
extern double** sigmaEArrayElev;
extern double** sigmaEArrayDep;
extern double** sigmaEArraySwing;
extern double** sigmaEArrayStance;
extern double** sigmaEArrayProt;
extern double** sigmaEArrayRet;
extern double** sigmaEArrayExt;
extern double** sigmaEArrayFlex;
extern double** sigmaEArrayCoord;

extern double** sigmaEArrayF;
extern double** sigmaEArrayB;
extern double** sigmaEArrayLL;
extern double** sigmaEArrayLT;
extern double** sigmaEArrayPcn;
extern double** sigmaEArrayModCom;
extern double** sigmaEArrayH;


//sI
extern double** sigmaIArrayElev;
extern double** sigmaIArrayDep;
extern double** sigmaIArraySwing;
extern double** sigmaIArrayStance;
extern double** sigmaIArrayProt;
extern double** sigmaIArrayRet;
extern double** sigmaIArrayExt;
extern double** sigmaIArrayFlex;
extern double** sigmaIArrayCoord;

extern double** sigmaIArrayF;
extern double** sigmaIArrayB;
extern double** sigmaIArrayLL;
extern double** sigmaIArrayLT;
extern double** sigmaIArrayPcn;
extern double** sigmaIArrayModCom;
extern double** sigmaIArrayH;

//bE
extern double** betaEArrayElev;
extern double** betaEArrayDep;
extern double** betaEArraySwing;
extern double** betaEArrayStance;
extern double** betaEArrayProt;
extern double** betaEArrayRet;
extern double** betaEArrayExt;
extern double** betaEArrayFlex;
extern double** betaEArrayCoord;

extern double** betaEArrayF;
extern double** betaEArrayB;
extern double** betaEArrayLL;
extern double** betaEArrayLT;
extern double** betaEArrayPcn;
extern double** betaEArrayModCom;
extern double** betaEArrayH;

//bI
extern double** betaIArrayElev;
extern double** betaIArrayDep;
extern double** betaIArraySwing;
extern double** betaIArrayStance;
extern double** betaIArrayProt;
extern double** betaIArrayRet;
extern double** betaIArrayExt;
extern double** betaIArrayFlex;
extern double** betaIArrayCoord;

extern double** betaIArrayF;
extern double** betaIArrayB;
extern double** betaIArrayLL;
extern double** betaIArrayLT;
extern double** betaIArrayPcn;
extern double** betaIArrayModCom;
extern double** betaIArrayH;

//idc
extern double** IdcArrayElev;
extern double** IdcArrayDep;
extern double** IdcArraySwing;
extern double** IdcArrayStance;
extern double** IdcArrayProt;
extern double** IdcArrayRet;
extern double** IdcArrayExt;
extern double** IdcArrayFlex;
extern double** IdcArrayCoord;

extern double** IdcArrayF;
extern double** IdcArrayB;
extern double** IdcArrayLL;
extern double** IdcArrayLT;
extern double** IdcArrayPcn;
extern double** IdcArrayModCom;
extern double** IdcArrayH;

typedef void (^CaseBlock)(void);

//-(void) runCalculation;
-(NSMutableArray*) selectCell: (cellPointer)cellName;
//-(void) sendToBuffer:(cellPointer) cellName: (int) samplesize;
-(void) readArray :(cellPointer)cellName :(int) starttime :(int) samplesize;





-(void) readMultipleArrays : (NSMutableArray*) cellArray : (double)offset : (double)duration;
-(cellPointer) convertCellNameToCellPointer: (NSString*)cellName;



@end

