//
//  Waveforms.m
//  robolobster
//
//  Created by HongQuan Do on 6/13/17.
//  Copyright © 2017 Joseph Ayers. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Waveforms.h"
#import <QuartzCore/QuartzCore.h>
#import "AppDelegate.h"


@implementation Waveforms
@synthesize ipbuf = _ipbuf;
@synthesize parambuf = _parambuf;
@synthesize result = _result;
@synthesize globalCounter;

-(id)init {
    self = [super init];
    if(self) {
        self.ipbuf = [[NSMutableArray alloc] init];
        self.parambuf = [[NSMutableArray alloc]init]; //array that stores alpha and sigma
        self->sTime = CACurrentMediaTime();
        
        globalCounter = 0;
        //        self.result  = [self runCalculation];
    }
    return self;
}

-(void)readMultipleArrays :(NSMutableArray *)cellArray :(double) offset :(double)duration {
    [_ipbuf removeAllObjects];
    [_parambuf removeAllObjects];
    //  NSLog(@"global counter = %d", globalCounter);
    for (int i = 0; i < [cellArray count]; i ++){
        NSString* cellName = [cellArray objectAtIndex:i]; //get cellName from array of cellName
        // NSLog(@"%@",cellName);
        cellPointer cell = [self convertCellNameToCellPointer:cellName]; //convert to enum type
        int samplesize = duration;
        int starttime = offset;
        [self readArray:cell:starttime:samplesize];
    }
    //  if (globalCounter < (int)(100000/samplesize-1)){ //100,000 is the max the array go to before it resets
    //      globalCounter++; //increase globalCounter
    //  }
    //  else{
    //     CFTimeInterval elapsedTime = CACurrentMediaTime() - self->sTime;
    //   NSLog(@"total elapse time to sample 100,0000 = %f s", elapsedTime);
    //      globalCounter = 0;} //stop
    
    
}

-(void) readArray :(cellPointer)cellName : (int) starttime :(int) samplesize{
    NSMutableArray*temp = [[NSMutableArray alloc] init];
    
    NSMutableArray*Params = [[NSMutableArray alloc] init];//array to store a,s
    
    NSMutableArray*indies = [self selectCell:cellName];
    int cellType = (int) [[indies objectAtIndex:0] integerValue];
    int cell = (int) [[indies objectAtIndex:1] integerValue];
    //CFTimeInterval startTime = CACurrentMediaTime();
    int invert = [(AppDelegate *)[[NSApplication sharedApplication] delegate] invertSign];
    int i;
    
    int a,b; //index to access params array
    if (cell <= 3){
        a = 0;
        b = cell;
    }
    else{
        a = 1;
        b = cell - 4;
    }
    
    if (cellType == 13){
        if (cell <= 4){
            a = 0;
            b = cell;
        }
        else{
            a = 1;
            b = cell - 4;
        }
    }
    switch (cellType) {
        case 0:
            //  NSLog( @"%f\n", xArrayElev[cell]);
            
            //keep this as an array for now in case in the future we want indiv params
            
            [Params addObject:[NSNumber numberWithDouble:alphaArrayElev[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayElev[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayElev[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayElev[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayElev[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayElev[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayElev[a][b]]];
            
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayElev[starttime][cell]]]; //put neg sign to invert trace
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayElev[starttime][cell]]]; //put neg sign to invert trace
                
                //  NSLog(@"%f", xArrayElev[starttime][cell]);
                starttime++;
            }
            break;
            
        case 1:
            [Params addObject:[NSNumber numberWithDouble:alphaArrayDep[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayDep[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayDep[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayDep[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayDep[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayDep[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayDep[a][b]]];
            
            
            
            //   NSLog( @"%f\n", xArrayDep[cell]);
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayDep[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayDep[starttime][cell]]];
                starttime++;
            }
            break;
        case 2:
            //   NSLog( @"%f\n", xArraySwing[cell]);
            [Params addObject:[NSNumber numberWithDouble:alphaArraySwing[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArraySwing[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArraySwing[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArraySwing[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArraySwing[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArraySwing[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArraySwing[a][b]]];
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArraySwing[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArraySwing[starttime][cell]]];
                
                starttime++;
            }
            break;
        case 3:
            //   NSLog( @"%f\n", xArrayStance[cell]);
            [Params addObject:[NSNumber numberWithDouble:alphaArrayStance[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayStance[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayStance[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayStance[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayStance[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayStance[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayStance[a][b]]];
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayStance[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayStance[starttime][cell]]];
                
                starttime++;
            }
            break;
        case 4:
            //   NSLog( @"%f\n", xArrayProt[cell]);
            [Params addObject:[NSNumber numberWithDouble:alphaArrayProt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayProt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayProt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayProt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayProt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayProt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayProt[a][b]]];
            
            
            for (i = 0; i < samplesize; i ++){
                
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayProt[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayProt[starttime][cell]]];
                
                starttime++;
            }
            break;
        case 5:
            //   NSLog( @"%f\n", xArrayRet[cell]);
            [Params addObject:[NSNumber numberWithDouble:alphaArrayRet[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayRet[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayRet[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayRet[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayRet[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayRet[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayRet[a][b]]];
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayRet[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayRet[starttime][cell]]];
                starttime++;
            }
            break;
        case 6:
            //   NSLog( @"%f\n", xArrayExt[cell]);
            [Params addObject:[NSNumber numberWithDouble:alphaArrayExt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayExt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayExt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayExt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayExt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayExt[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayExt[a][b]]];
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayExt[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayExt[starttime][cell]]];
                
                starttime++;
            }
            break;
        case 7:
            //NSLog( @"%f\n", xArrayFlex[0][cell]);
            [Params addObject:[NSNumber numberWithDouble:alphaArrayFlex[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayFlex[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayFlex[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayFlex[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayFlex[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayFlex[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayFlex[a][b]]];
            
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayFlex[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayFlex[starttime][cell]]];
                
                starttime++;
            }
            break;
        case 8:
            //NSLog( @"%f\n", xArrayCoord[cell]);
            [Params addObject:[NSNumber numberWithDouble:alphaArrayCoord[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayCoord[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayCoord[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayCoord[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayCoord[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayCoord[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayCoord[a][b]]];
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayCoord[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayCoord[starttime][cell]]];
                
                starttime++;
            }
            break;
            
        case 9: //cellF
            [Params addObject:[NSNumber numberWithDouble:alphaArrayF[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayF[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayF[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayF[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayF[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayF[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayF[cell][0]]];
            
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayF[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayF[starttime][cell]]];
                // NSLog(@"%f", xArrayF[starttime][cell]);
                
                starttime++;
            }
            
            break;
            
        case 10: //cellB
            [Params addObject:[NSNumber numberWithDouble:alphaArrayB[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayB[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayB[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayB[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayB[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayB[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayB[cell][0]]];
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayB[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayB[starttime][cell]]];
                
                starttime++;
            }
            
            break;
            
        case 11: //cellLL
            [Params addObject:[NSNumber numberWithDouble:alphaArrayLL[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayLL[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayLL[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayLL[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayLL[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayLL[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayLL[cell][0]]];
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayLL[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayLL[starttime][cell]]];
                
                starttime++;
            }
            
            break;
            
        case 12: //cellLT
            [Params addObject:[NSNumber numberWithDouble:alphaArrayLT[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayLT[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayLT[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayLT[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayLT[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayLT[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayLT[cell][0]]];
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayLT[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayLT[starttime][cell]]];
                
                starttime++;
            }
            
            break;
            
        case 13: //cellPcn
            [Params addObject:[NSNumber numberWithDouble:alphaArrayPcn[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayPcn[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayPcn[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayPcn[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayPcn[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayPcn[a][b]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayPcn[a][b]]];
            
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayPcn[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayPcn[starttime][cell]]];
                
                starttime++;
            }
            
            break;
            
        case 14: //cellModCom
            [Params addObject:[NSNumber numberWithDouble:alphaArrayModCom[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayModCom[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayModCom[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayModCom[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayModCom[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayModCom[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayModCom[cell][0]]];
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayModCom[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayModCom[starttime][cell]]];
                
                starttime++;
            }
            
            break;
            
        case 15: //cellH
            [Params addObject:[NSNumber numberWithDouble:alphaArrayH[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayH[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayH[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayH[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayH[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayH[cell][0]]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayH[cell][0]]];
            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayH[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayH[starttime][cell]]];
                
                starttime++;
            }
            
            break;
            
            
        default:
            break;
    }
    [_ipbuf addObject: temp];
    [_parambuf addObject:Params];
  //  CFTimeInterval elapsedTime = CACurrentMediaTime() - startTime;
   // NSLog(@"indiv elapse time = %f s", elapsedTime);
}

-(NSMutableArray*) selectCell:(cellPointer)cellName{
    //  NSLog(@"Select cell %d", cellName);
    if (cellName < 8){
        //NSLog(@"select Elevator");
        _cellIndex = (int)cellName;
        _cellTypeIndex = 0;
    }
    else if (cellName < 16){
        //   NSLog(@"select Swing");
        _cellIndex = (int)cellName - 8;
        _cellTypeIndex = 2;
        
    }
    else if (cellName < 24){
        //NSLog(@"select Depressor");
        _cellIndex = (int)cellName - 16;
        _cellTypeIndex = 1;
    }
    else if (cellName < 32){
        //   NSLog(@"select Stance");
        _cellIndex = (int)cellName - 24;
        _cellTypeIndex = 3;
    }
    else if (cellName < 40){
        //NSLog(@"select Coord");
        _cellIndex = (int)cellName - 32;
        _cellTypeIndex = 8;
        
    }
    else if (cellName < 48){
        //   NSLog(@"select Protractor");
        _cellIndex = (int)cellName - 40;
        _cellTypeIndex = 4;
    }
    else if (cellName < 56){
        //   NSLog(@"select Retractor");
        _cellIndex = (int)cellName - 48;
        _cellTypeIndex = 5;
    }
    else if (cellName < 64){
        //  NSLog(@"select Extensor");
        _cellIndex = (int)cellName - 56;
        _cellTypeIndex = 6;
    }
    else if (cellName < 72){
        // NSLog(@"select Flexor");
        _cellIndex = (int)cellName - 64;
        _cellTypeIndex = 7;
    }
    else if (cellName < 74){
        //F
        _cellIndex = (int)cellName - 72;
        _cellTypeIndex = 9; //8 is coord already ...
    }
    else if (cellName < 76){
        //B
        _cellIndex = (int)cellName - 74;
        _cellTypeIndex = 10;
    }
    else if (cellName < 78){
        //LL
        _cellIndex = (int)cellName - 76;
        _cellTypeIndex = 11;
    }
    else if (cellName < 80){
        //LT
        _cellIndex = (int)cellName - 78;
        _cellTypeIndex = 12;
    }
    else if (cellName < 90){
        //Pcn
        _cellIndex = (int)cellName - 80;
        _cellTypeIndex = 13;
    }
    else if (cellName < 92){
        //ModCom
        _cellIndex = (int)cellName - 90;
        _cellTypeIndex = 14;
    }
    else if (cellName < 94){
        //H
        _cellIndex = (int)cellName - 92;
        _cellTypeIndex = 15;
    }
    else{
        NSLog(@"Not yet supported, choose a different type");
        _cellIndex = -1;
        _cellTypeIndex = -1;
    }
    //put the cell Index and the cell Types in the return array !
    NSMutableArray* cellIndexArray = [[NSMutableArray alloc] init];
    [cellIndexArray addObject:[NSNumber numberWithInteger: _cellTypeIndex]];
    [cellIndexArray addObject: [NSNumber numberWithInteger:_cellIndex]];
    return cellIndexArray;
}


-(cellPointer) convertCellNameToCellPointer: (NSString*)cellName{
    NSArray *cells = [[NSArray alloc] initWithObjects:@"cellElevatorL1",
                      @"cellElevatorL2",
                      @"cellElevatorL3",
                      @"cellElevatorL4",
                      @"cellElevatorR1",
                      @"cellElevatorR2",
                      @"cellElevatorR3",
                      @"cellElevatorR4",
                      @"cellSwingL1",
                      @"cellSwingL2",
                      @"cellSwingL3",
                      @"cellSwingL4",
                      @"cellSwingR1",
                      @"cellSwingR2",
                      @"cellSwingR3",
                      @"cellSwingR4",
                      @"cellDepressorL1",
                      @"cellDepressorL2",
                      @"cellDepressorL3",
                      @"cellDepressorL4",
                      @"cellDepressorR1",
                      @"cellDepressorR2",
                      @"cellDepressorR3",
                      @"cellDepressorR4",
                      @"cellStanceL1",
                      @"cellStanceL2",
                      @"cellStanceL3",
                      @"cellStanceL4",
                      @"cellStanceR1",
                      @"cellStanceR2",
                      @"cellStanceR3",
                      @"cellStanceR4",
                      @"cellCoordL1",
                      @"cellCoordL2",
                      @"cellCoordL3",
                      @"cellCoordL4",
                      @"cellCoordR1",
                      @"cellCoordR2",
                      @"cellCoordR3",
                      @"cellCoordR4",
                      @"cellProtractorL1",
                      @"cellProtractorL2",
                      @"cellProtractorL3",
                      @"cellProtractorL4",
                      @"cellProtractorR1",
                      @"cellProtractorR2",
                      @"cellProtractorR3",
                      @"cellProtractorR4",
                      @"cellRetractorL1",
                      @"cellRetractorL2",
                      @"cellRetractorL3",
                      @"cellRetractorL4",
                      @"cellRetractorR1",
                      @"cellRetractorR2",
                      @"cellRetractorR3",
                      @"cellRetractorR4",
                      @"cellExtensorL1",
                      @"cellExtensorL2",
                      @"cellExtensorL3",
                      @"cellExtensorL4",
                      @"cellExtensorR1",
                      @"cellExtensorR2",
                      @"cellExtensorR3",
                      @"cellExtensorR4",
                      @"cellFlexorL1",
                      @"cellFlexorL2",
                      @"cellFlexorL3",
                      @"cellFlexorL4",
                      @"cellFlexorR1",
                      @"cellFlexorR2",
                      @"cellFlexorR3",
                      @"cellFlexorR4",
                      @"cellFL",
                      @"cellFR",
                      @"cellBL",
                      @"cellBR",
                      @"cellLLL",
                      @"cellLLR",
                      @"cellLTL",
                      @"cellLTR",
                      @"cellPcnLlow",
                      @"cellPcnLrosDown",
                      @"cellPcnLlevel",
                      @"cellPcnLrosUp",
                      @"cellPcnLpHigh",
                      @"cellPcnRlow",
                      @"cellPcnRrosDown",
                      @"cellPcnRlevel",
                      @"cellPcnRrosUp",
                      @"cellPcnRpHigh",
                      @"cellModComL",
                      @"cellModComR",
                      @"cellHL",
                      @"cellHR", nil];
    int item = (int) [cells indexOfObject:cellName];

    return (cellPointer)item;
    
}


@end


