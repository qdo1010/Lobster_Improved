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
    int cellType = [[indies objectAtIndex:0] integerValue];
    int cell = [[indies objectAtIndex:1] integerValue];
    //CFTimeInterval startTime = CACurrentMediaTime();
    int invert = [(AppDelegate *)[[NSApplication sharedApplication] delegate] invertSign];
    int i;
    //starttime = starttime ;
    //AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];n
    
  //  int invert = [appDelegate ]
    switch (cellType) {
        case 0:
            //  NSLog( @"%f\n", xArrayElev[cell]);
            
            //keep this as an array for now in case in the future we want indiv params
            
            [Params addObject:[NSNumber numberWithDouble:alphaArrayElev]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayElev]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            [Params addObject:[NSNumber numberWithDouble:0]];


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
            [Params addObject:[NSNumber numberWithDouble:alphaArrayDep]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayDep]];
           [Params addObject:[NSNumber numberWithDouble:sigmaEArrayDep]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayDep]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayDep]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayDep]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayDep]];


            
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
            [Params addObject:[NSNumber numberWithDouble:alphaArraySwing]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArraySwing]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            [Params addObject:[NSNumber numberWithDouble:0]];
            
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
            [Params addObject:[NSNumber numberWithDouble:alphaArrayStance]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayStance]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayStance]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayStance]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayStance]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayStance]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayStance]];

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
            [Params addObject:[NSNumber numberWithDouble:alphaArrayProt]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayProt]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayProt]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayProt]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayProt]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayProt]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayProt]];


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
            [Params addObject:[NSNumber numberWithDouble:alphaArrayRet]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayRet]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayRet]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayRet]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayRet]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayRet]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayRet]];

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
            [Params addObject:[NSNumber numberWithDouble:alphaArrayExt]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayExt]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayExt]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayExt]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayExt]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayExt]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayExt]];

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
            [Params addObject:[NSNumber numberWithDouble:alphaArrayFlex]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayFlex]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayFlex]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayFlex]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayFlex]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayFlex]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayFlex]];


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
            [Params addObject:[NSNumber numberWithDouble:alphaArrayCoord]];
            [Params addObject:[NSNumber numberWithDouble:sigmaArrayCoord]];
            [Params addObject:[NSNumber numberWithDouble:sigmaEArrayCoord]];
            [Params addObject:[NSNumber numberWithDouble:sigmaIArrayCoord]];
            [Params addObject:[NSNumber numberWithDouble:betaEArrayCoord]];
            [Params addObject:[NSNumber numberWithDouble:betaIArrayCoord]];
            [Params addObject:[NSNumber numberWithDouble:IdcArrayCoord]];

            for (i = 0; i < samplesize; i ++){
                if (invert)
                    [temp addObject: [NSNumber numberWithDouble: xArrayCoord[starttime][cell]]];
                else
                    [temp addObject: [NSNumber numberWithDouble: -xArrayCoord[starttime][cell]]];
                
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
    else{
        NSLog(@"Not yet supported, choose a different type");
        _cellIndex = -1;
        _cellTypeIndex = -1;
    }
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
                      @"cellLTRa",
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
    int item = [cells indexOfObject:cellName];
    // int i;
    /*    for ( i = 0; i < cellHR; i++){
     if (i == item){
     return (cellPointer)i;
     break;
     }
     }*/
    return (cellPointer)item;
    
    //return item;
}


@end


////////////////////OLD CODE/////////////////////
//}

/*
 -(void) ReadFromFile{
 //xmain();
 FILE *fid[8];
 char space[1024];
 _ibuf = [[NSMutableArray alloc] init];  //ibuf will contains 8 ipbufs, since there are 8 diff groups,
 //Elev
 //Dep
 //Swing
 //Stance
 //Protractor
 //Retractor
 //Extensor
 //Flexor
 fid[0] = fopen ( "timeElev" , "r" ); //read from temp files
 fid[1] = fopen ( "timeDep" , "r" ); //read from temp files
 fid[2] = fopen ( "timeSwing" , "r" ); //read from temp files
 fid[3] = fopen ( "timeStance" , "r" ); //read from temp files
 fid[4] = fopen ( "timeProtractor" , "r" ); //read from temp files
 fid[5] = fopen ( "timeRetractor" , "r" ); //read from temp files
 fid[6] = fopen ( "timeExtensor" , "r" ); //read from temp files
 fid[7] = fopen ( "timeFlexor" , "r" ); //read from temp files
 
 for (int i = 0; i < 8; i ++){
 if (fid[i] == NULL)
 printf("Error opening the file");
 else{
 NSMutableArray* tempArray = [[NSMutableArray alloc] init]; //ipbuf is a 2d matrix [5000][8]
 while (fgets(space, 1024, fid[i]) != NULL)
 {
 
 // NSLog(@"%s", space);
 NSString*data =[[NSString stringWithFormat:@"%s", space] stringByTrimmingCharactersInSet: [NSCharacterSet newlineCharacterSet]];
 
 //NSLog(@"%@", data);
 NSArray *words = [data componentsSeparatedByString: @" "];
 NSMutableArray* temp = [NSMutableArray arrayWithCapacity:[words count]];
 [temp addObjectsFromArray:words];
 [temp removeObject:@""];
 [tempArray addObject: temp];
 //[_ipbuf addObject: [[[NSString stringWithFormat:@"%s", space] stringByTrimmingCharactersInSet: [NSCharacterSet newlineCharacterSet]] componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]]];
 // [_ibuf addObject: _ipbuf];
 }
 [_ibuf addObject:tempArray];
 fclose(fid[i]);
 }
 }
 //NSLog(@"run");
 // NSLog(@"%@", _ibuf[2][0]);
 // NSLog(@"%@", _ipbuf[0][0]);
 //  return _ibuf;
 }
 
 */
