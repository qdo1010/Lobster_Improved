					
#import "OscilloscopeController.h"//Hi Quan, this is my first edit. It's practice to see if I can create a branch an then open a pull request for you.
#import "AppDelegate.h"
#import "analysisWindowController.h"
//#import "GraphViewController.h"
//#import "GraphicsView.h"
#import "Waveforms.h"
//#import "DSPSources.h"
//#import "signalProcessing.h"
#import "OscilloscopeView.h"
//#import "VideoPlaybackController.h"

@implementation OscilloscopeController

    // These placeholders fill in for the 32bit Quicktime 7 Audio Extraction API
//typedef id MovieAudioExtractionRef;

#pragma mark -
#pragma mark Properties



@synthesize sweepDurationSlider;
@synthesize sweepDurationTextBox;
@synthesize sweepOffsetSlider;
@synthesize sweepOffsetTextBox;

@synthesize traceGainSlider;
@synthesize traceOffsetSlider;
@synthesize traceGainTextBox;
@synthesize traceOffsetTextBox;

@synthesize displayTraceID;
@synthesize traceIDchosen;
@synthesize AllTracesInfo;
@synthesize IndividualTraceInfo;

/*Moved To Parameter Setter
@synthesize cellNameTextBox;
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
@synthesize side;
@synthesize seg;
 */


//@synthesize settingUpParams; //load param value into array
//@synthesize setParamsButton;
//@synthesize firstTimeChangeParams;

-(id) init
{
   // AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
        //    [self setOscilloscopeView:oscilloscopeView];
    self  = [super init];
    
    
    if(self){
		//        int ProxOffset, DistOffset;
     //   NSRect  iRect = [[[appDelegate analysisWindowController] oscilloscopeView] bounds];
        NSLog(@"setting up");
       // [self   setProxOffset:(iRect.size.height / 3)];
       [self setTitle:@"Oscilloscope"];
      
    //}
       // [cellSelector removeAllItems];
       // [cellSelector addItemWithTitle:@"cellElevatorL1"];
    }
    return self;
    }

- (void) awakeFromNib {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    [[appDelegate analysisWindowController] setOscilloscopeController:self];
    NSLog(@"Executing awakeFromNib in Oscilloscope Controller");
    
   // AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    
    
    [displayTraceID removeAllItems];
    
    unsigned long int numCell = [[[appDelegate traceSelector] traceArraytobeSent] count];
    
    //init the array that will contains input param values
    //cell ID tell C code what type of cell it is
    /* Moved to Parameter Setter
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
        NSLog(@"%@", cellName);
     
        
        
        [displayTraceID addItemWithTitle:[NSString stringWithFormat:@"%d", i]];
       */
    [traceGainTextBox setDelegate:self];
    [sweepDurationTextBox setDelegate:self];
    [sweepOffsetTextBox setDelegate:self];
    [traceOffsetTextBox setDelegate:self];

        //init the scope values
        [appDelegate setSweepDuration:5000];
        [sweepDurationTextBox setStringValue:[NSString stringWithFormat:@"%f",0.001*elapsed*5000]];
        [sweepOffsetSlider setFloatValue:5000];
    
        [appDelegate setSweepOffset:0];
        [sweepOffsetTextBox setStringValue:[NSString stringWithFormat:@"%f",0.]];
        [sweepOffsetSlider setFloatValue: 0];

        
        [[appDelegate traceGainArray] addObject:[NSString stringWithFormat:@"%d", 1]];
        [traceGainTextBox setStringValue:[NSString stringWithFormat:@"%f",1.0]];
        [traceGainSlider setFloatValue:1];
        
        [[appDelegate traceOffsetArray] addObject:[NSString stringWithFormat:@"%d", 0]];
        [traceOffsetTextBox setStringValue:[NSString stringWithFormat:@"%f",0.]];
        [traceOffsetSlider setFloatValue:0];
        
    

    
    //Set sigma and alpha to what they really are
    //id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    
    //this is the param id that contains sigma and alpha
  //  NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:traceIDchosen];
  //  [alphaTextBox setStringValue:[[params objectAtIndex:0] stringValue]];
    
  //  [sigmaTextBox setStringValue:[[params objectAtIndex:1] stringValue]];
  //  [alphaTextBox setStringValue:[NSString stringWithFormat:@"%f",5.5]];
  //  [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",1.2]];
    
    //make it bidirectional later
    

 //   [self begin]
    [NSTimer scheduledTimerWithTimeInterval: 0.1                                                  target: self
                                                selector:@selector(beginSendingStuffToBeDrawn)
                                              userInfo: nil repeats:YES];
    
  //  [NSTimer scheduledTimerWithTimeInterval: 10                                                  target: self
   //                                             selector:@selector(createWaveForm)
   //                                             userInfo: nil repeats:YES];
           // [self setOscilloscope View:oscilloscopeView];
  //  [self chooseTraceID:displayTraceID]; //call this to init
    
    //set trace ID in appDelegate
    [appDelegate setIDofCellChosen:0];
    
    //this is the param id that contains sigma and alpha

}

- (IBAction)chooseTraceID:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    traceIDchosen = (int) [[[self displayTraceID] title] integerValue];
    
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    
    
        //set trace ID in appDelegate
    [appDelegate setIDofCellChosen:traceIDchosen];
    
        //this is the param id that contains sigma and alpha
        // NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:traceIDchosen];
    
        //init all params
    int numCell = (int) [[[appDelegate traceSelector] traceArraytobeSent] count];
    
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
    
  /*
        // else{
    [alphaTextBox setStringValue: [NSString stringWithFormat:@"%f",alpha[traceIDchosen]]];
    [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",sigma[traceIDchosen]]];
    [sigmaETextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaE[traceIDchosen]]];
    [sigmaITextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaI[traceIDchosen]]];
    [betaETextBox setStringValue:[NSString stringWithFormat:@"%f",betaE[traceIDchosen]]];
    
    [betaITextBox setStringValue:[NSString stringWithFormat:@"%f",betaI[traceIDchosen]]];
    
    [IdcTextBox setStringValue:[NSString stringWithFormat:@"%f",Idc[traceIDchosen]]];
    
 
    
        // }
    
      */
    
        // if (traceIDchosen == 0 || traceIDchosen == 1){
        //     [sigmaTextBox setEditable:(false)];
        // }
        // else{
        //     [sigmaTextBox setEditable:(true)];
        // }
    NSLog(@" choose this trace %d", traceIDchosen);
//    [cellNameTextBox setStringValue:[[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:traceIDchosen]];
    
}


- (IBAction) invertProxGraph: (id)sender{
	AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
	int i = [appDelegate invertSign];
    if (i == 0){
        [appDelegate setInvertSign:1];
    }
    else{
        [appDelegate setInvertSign:0];
    }
	/*
    for (i = 0; i < [[[appDelegate traceWaveforms] ipbuf] count]; i++) {
		
        if ([[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i] floatValue] < 0) {
            [[[appDelegate traceWaveforms] ipbuf] replaceObjectAtIndex:i withObject:[NSNumber numberWithFloat:-([[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i] floatValue])]];
            
        }
        else if ([[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i] floatValue] > 0) {
            [[[appDelegate traceWaveforms] ipbuf] replaceObjectAtIndex:i withObject:[NSNumber numberWithFloat:-([[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i] floatValue])]];
     
        }
	}*/
	//[[[appDelegate analysisWindowController] myCurrentView] setNeedsDisplay:YES];
}


  ///this is to change sweep duration, so to zoom in!
- (IBAction)changeSweepDuration:(id)sender {
    traceIDchosen = (int) [[[self displayTraceID] title] integerValue];
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    [sweepDurationSlider setMinValue:0];
    [sweepDurationSlider setMaxValue:10000];
   // NSLog(@"%d", [sweepDurationSlider intValue]);
    [sweepDurationTextBox setStringValue:[NSString stringWithFormat:@"%f",0.001*elapsed*[sweepDurationSlider intValue]]];
    
    //change duration array
    [appDelegate setSweepDuration:[sweepDurationSlider floatValue]];
    
    
    /*for (int i = 0; i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i ++){
    [[appDelegate durationArray] replaceObjectAtIndex:i withObject:[NSString stringWithFormat:@"%d",[sweepDurationSlider intValue]]];
    }*/
 //   sweepDurationTextBox.text =
}

- (IBAction)changeSweepOffset:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    [sweepOffsetSlider setMinValue:0];
    [sweepOffsetSlider setMaxValue:10000];
    // NSLog(@"%d", [sweepDurationSlider intValue]);
    [sweepOffsetTextBox setStringValue:[NSString stringWithFormat:@"%f",0.001*elapsed*[sweepOffsetSlider intValue]]];
   // NSLog(@"trace chosen = %d", traceIDchosen);
    
    [appDelegate setSweepOffset:[sweepOffsetSlider floatValue]];
    
   /* for (int i = 0; i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i ++){
        [[appDelegate offsetArray] replaceObjectAtIndex:i withObject:[NSString stringWithFormat:@"%d",[sweepOffsetSlider intValue]]];
    }*/
    
}
-(void) controlTextDidChange:(NSNotification *)notification{
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    
    //this is the param id that contains sigma and alpha
    NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:traceIDchosen];
    int i = traceIDchosen; //id
    
    
    if([notification object] == sweepOffsetTextBox){
        [sweepOffsetSlider setFloatValue:[sweepOffsetTextBox floatValue]/(0.001*elapsed)];
        //send the change
         [appDelegate setSweepOffset:[sweepOffsetSlider floatValue]];

    }
    else if ([notification object] == sweepDurationTextBox){
        [sweepDurationSlider setFloatValue:[sweepDurationTextBox floatValue]/(0.001*elapsed)];
        //send the change
        [appDelegate setSweepDuration:[sweepDurationSlider floatValue]];
        
    }
    else if ([notification object] == traceGainTextBox){
        [traceGainSlider setFloatValue:[traceGainTextBox floatValue]];
        //send the change
        [[appDelegate traceGainArray] replaceObjectAtIndex:traceIDchosen withObject:[NSString stringWithFormat:@"%f",[traceGainSlider floatValue]]];
    }
    else if ([notification object] == traceOffsetTextBox){
        [traceOffsetSlider setFloatValue:[traceOffsetTextBox floatValue]];
        //send the change
        [[appDelegate traceOffsetArray] replaceObjectAtIndex:traceIDchosen withObject:[NSString stringWithFormat:@"%f",[traceOffsetSlider floatValue]]];
    }
    
   /*
   else if ([notification object] == alphaTextBox){
            alpha[traceIDchosen] = [alphaTextBox doubleValue];
       [params replaceObjectAtIndex:0 withObject:[NSNumber numberWithDouble:alpha[i]]];
    }
    
   else if ([notification object] == sigmaTextBox){
       sigma[traceIDchosen] = [sigmaTextBox doubleValue];
       [params replaceObjectAtIndex:1 withObject:[NSNumber numberWithDouble:sigma[i]]];
   }
   else if ([notification object] == sigmaETextBox){
       sigmaE[traceIDchosen] = [sigmaETextBox doubleValue];
       [params replaceObjectAtIndex:2 withObject:[NSNumber numberWithDouble:sigmaE[i]]];
      
   }
   else if ([notification object] == sigmaITextBox){
       sigmaI[traceIDchosen] = [sigmaITextBox doubleValue];
       [params replaceObjectAtIndex:3 withObject:[NSNumber numberWithDouble:sigmaI[i]]];
       
   }
   else if ([notification object] == betaETextBox){
       betaE[traceIDchosen] = [betaETextBox doubleValue];
       [params replaceObjectAtIndex:4 withObject:[NSNumber numberWithDouble:betaE[i]]];
   }
   else if ([notification object] == betaITextBox){
       betaI[traceIDchosen] = [betaITextBox doubleValue];
       [params replaceObjectAtIndex:5 withObject:[NSNumber numberWithDouble:betaI[i]]];
   }
   else if ([notification object] == IdcTextBox){
       Idc[traceIDchosen] = [IdcTextBox doubleValue];
       [params replaceObjectAtIndex:6 withObject:[NSNumber numberWithDouble:Idc[i]]];
   }
    */
}

//-(void) createWaveForm{
//   [self performSelectorInBackground:@selector(runMain) withObject:nil];
   // xmain(); //run pc2dsp and write to temp files, automatically run in background async
    // NSLog(@"total elapse time for 1 iter = %f s", elapsedTime);
//}

//-(void) runMain{
   // int currentIndex = 0;
   //  while (currentIndex <= 10000){ //should be tmax, not hardcoded btw
   //      currentIndex = checkMainLoopIndex();
   //  } //xmain should end by now
 //   xmain();
//}

- (IBAction)changeTraceGain:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    [traceGainSlider setMinValue:0.1];
    [traceGainSlider setMaxValue:5];
    // NSLog(@"%d", [sweepDurationSlider intValue]);
    [traceGainTextBox setStringValue:[NSString stringWithFormat:@"%f",[traceGainSlider floatValue]]];

    //replace array value
     [[appDelegate traceGainArray] replaceObjectAtIndex:traceIDchosen withObject:[NSString stringWithFormat:@"%f",[traceGainSlider floatValue]]];
    
}


- (IBAction)changeTraceOffset:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    [traceOffsetSlider setMinValue:0];
    [traceOffsetSlider setMaxValue:1000];
    // NSLog(@"%d", [sweepDurationSlider intValue]);
    [traceOffsetTextBox setStringValue:[NSString stringWithFormat:@"%f", [traceOffsetSlider floatValue]]];
    
    //replace array value
    [[appDelegate traceOffsetArray] replaceObjectAtIndex:traceIDchosen withObject:[NSString stringWithFormat:@"%f",[traceOffsetSlider floatValue]]];
}

//This function will check for the Parameters being chosen, and tell the software what txt file to pick, to load in the necessary params
//It's not working
- (void)commandStateVC:(CommandStateViewController *)commandStateVC didUpdateParameters:(NSMutableDictionary *)parameters{
    NSLog(@"%@", [parameters valueForKey:@"lFor"]);
    int speed = -1;
    if ([[parameters valueForKey:@"spd"] intValue] == 1){
        NSLog(@"high");
        speed = 1;
    }
    else if([[parameters valueForKey:@"spd"] intValue] == 2){
        NSLog(@"medium");
        speed = 2;
    }
    else if([[parameters valueForKey:@"spd"] intValue] == 3){
        NSLog(@"slow");
        speed = 3;
    }
    else {
        NSLog(@"stop");
        speed = 4;
        //force CPG to all stop
        /*NSString * filePath = [[NSBundle mainBundle] pathForResource:@"Elevator"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
        filePath = [[NSBundle mainBundle] pathForResource:@"Depressor"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
        filePath = [[NSBundle mainBundle] pathForResource:@"Swing"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
        filePath = [[NSBundle mainBundle] pathForResource:@"Stance"
                                                              ofType:@""];
         [self loadParamsWithName:self : filePath :speed];*/
    }
    if ([[parameters valueForKey:@"lFor"]  boolValue] == true){
        //turn on left forward
        NSLog(@"walk forward left");
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"ForwardLeft"
                                                              ofType:@""];
        //load Params to call Forward left
        [self loadParamsWithName:self : filePath :speed];
    }
    else {
        NSLog(@"forward left off");
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"ForwardLeft"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :4]; //4 means turning off
    }
    if ([[parameters valueForKey:@"rFor"]  boolValue] == true){
        //turn on right forward
            //turn on left forward
            NSLog(@"walk forward right");
          //  [self loadParamsWithName:self :@"/Users/qdo/Lobster/RoboLobsterSimu/ForwardRight"];
            NSString * filePath = [[NSBundle mainBundle] pathForResource:@"ForwardRight"
                                                                  ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
        }
    else {
            NSLog(@"forward right off");
            NSString * filePath = [[NSBundle mainBundle] pathForResource:@"ForwardRight"
                                                                  ofType:@""];
        [self loadParamsWithName:self : filePath :4];
          //  [self loadParamsWithName:self :@"/Users/qdo/Lobster/RoboLobsterSimu/ForwardRightOff"];
        }

    if ([[parameters valueForKey:@"lBak"]  boolValue] == true){
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"BackwardLeft"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
        //turn on left backward
    }
    else{
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"BackwardLeft"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :4];
    }
    if ([[parameters valueForKey:@"rBak"]  boolValue] == true){
        //turn on right backward
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"BackwardRight"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
    }
    else {

        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"BackwardRight"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :4];
    }
    if ([[parameters valueForKey:@"lLed"]  boolValue] == true){
        //turn on left leading
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"LateralLeft"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
    }
    else {
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"LateralLeft"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :4];
    }
    if ([[parameters valueForKey:@"rLed"]  boolValue] == true){
        //turn on right leading
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"LateralRight"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
    }
    else {
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"LateralRight"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :4];
    }
    if ([[parameters valueForKey:@"lTra"]  boolValue] == true){
        //turn on left trailing
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"TrailingLeft"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
    }
    else {
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"TrailingLeft"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :4];
    }
    if ([[parameters valueForKey:@"rTra"]  boolValue] == true){
        //turn on right trailing
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"TrailingRight"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :speed];
    }
    else {
        NSString * filePath = [[NSBundle mainBundle] pathForResource:@"TrailingRight"
                                                              ofType:@""];
        [self loadParamsWithName:self : filePath :4];
    }

//    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    //UNCOMMENT TO RUN
//    editParam(cellID,side, seg,alpha,sigma,sigmaE, sigmaI, betaE, betaI,Idc, [[[appDelegate traceSelector] traceArraytobeSent] count]);

    /* if (firstTimeChangeParams == 1){
     [appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
     [self setFirstTimeChangeParams:0]; //now it's set
     }else{*/
//    setParamsButton.enabled = false;
  //  int currentIndex = 0;
  //  while (currentIndex != 99999){ //should be tmax, not hardcoded btw
  //      currentIndex = 0;
  //      currentIndex = checkMainLoopIndex();
  //  } //xmain should end by now
    //sleep(0.1);

    ///Stabilize by making it run on foreground instead of being a background process, so it has to finish

   // [appDelegate performSelector:@selector(createWaveForm) withObject:nil];
    //[appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
//    setParamsButton.enabled = true;

}

//this function will call Drawer to draw the trace
//as well as set up the correct label for the cell being selected
- (void)beginSendingStuffToBeDrawn {
    
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
   // if ([[appDelegate offsetArray] count] != [[[appDelegate traceSelector] traceArraytobeSent] count]  || ([[appDelegat/e durationArray] count] != [[[appDelegate traceSelector] traceArraytobeSent] count])){
     //           _startButton.enabled = NO;
      //      }
    //else{
        
      //  _startButton.enabled = YES;
       // NSLog(@"is it doing anything?");
        [appDelegate displaySampledWaveforms: [[appDelegate traceSelector] traceArraytobeSent]  : [appDelegate sweepOffset] :[appDelegate sweepDuration]];
/*    if (settingUpParams){   //This is now in Param's Setter
    
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:0];
    int numCell = (int) [[[appDelegate traceSelector] traceArraytobeSent] count];

        //init params input values
    
    for (int i = 0; i < numCell; i++){
        //NSLog(@"howw many %d",i);
        params= [[propertyValue parambuf] objectAtIndex:i];
        alpha[i] = [[params objectAtIndex:0] doubleValue];
        sigma[i] = [[params objectAtIndex:1] doubleValue];
        sigmaE[i] = [[params objectAtIndex:2] doubleValue];
        sigmaI[i] = [[params objectAtIndex:3] doubleValue];
        betaE[i] = [[params objectAtIndex:4] doubleValue];
        betaI[i] = [[params objectAtIndex:5] doubleValue];
        Idc[i] = [[params objectAtIndex:6] doubleValue];
        
    }

    settingUpParams = 0;

    
     
     [alphaTextBox setStringValue: [NSString stringWithFormat:@"%f",alpha[0]]];
     [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",sigma[0]]];
     [sigmaETextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaE[0]]];
     [sigmaITextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaI[0]]];
     [betaETextBox setStringValue:[NSString stringWithFormat:@"%f",betaE[0]]];
     
     [betaITextBox setStringValue:[NSString stringWithFormat:@"%f",betaI[0]]];
     
     [IdcTextBox setStringValue:[NSString stringWithFormat:@"%f",Idc[0]]];
     
     
     // NSLog(@" choose this trace %d", traceIDchosen);
     [cellNameTextBox setStringValue:[[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:0]];
    }
 */
    //}
  //  [displayTraceID removeAllItems];
  //  for (int i; i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i ++){
  //      [displayTraceID addItemWithTitle:[NSString stringWithFormat:@"%d", i]];
  //  }
}

//This function will switch the color of the viewer
- (IBAction)changeView:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    if ([appDelegate switchColor] == 0){
    [appDelegate setSwitchColor: 1];
    }
    else{
        [appDelegate setSwitchColor:0];
    }
}



//this function will load a Parameter file with all the parameters defined.
//it will also tell the software which line in the parameter file should be used. Each line corresponds to speed (Fast, Slow, Medium, or Stop)
- (IBAction)loadParamsWithName:(id)sender :(NSString*)filename :(int)speed {
    /*   NSArray *fileURLs = [NSArray arrayWithObjects:nil];
     [[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:fileURLs];
     
     [[NSWorkspace sharedWorkspace] selectFile:fileURLs inFileViewerRootedAtPath:nil];*/
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];

    //the trace waveform
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    NSURL *URL = [NSURL fileURLWithPath:filename];

 //   for( NSURL* URL in [openDlg URLs] )  // See #2, #4
  //      {
           // NSLog( @"%@", [URL path] );
            //NOW READ IN THE FILE and LOAD the PARAMS
            NSString *content = [NSString stringWithContentsOfFile:[URL path]
                                                          encoding:NSUTF8StringEncoding
                                                             error:NULL];
            NSLog(@"%@",content);
            NSArray* rows = [content componentsSeparatedByString:@"\n"];
            
            //loop thru all the trace and find them in the text files
            for (int i =0;i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i++){
                NSString* name = [[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:i];
                int c = 0;
                int tside;
                int tseg;
                for (NSString *row in rows){
                    NSArray* columns = [row componentsSeparatedByString:@","];
                    int col = 0;
                    NSString* spd;
                    if (speed == -1){ //no speed given
                        col = 0;
                        spd = columns[0]; //No need to do speed
                    }
                    else if (speed == 1){ //fast
                        col = 1;
                        spd = @"Fast";
                    }
                    else if (speed == 2){ //normal
                        col = 1;
                        spd = @"Medium";

                    }
                    else if (speed == 3){ //slow
                        col = 1;
                        spd = @"Slow";

                    }
                    else if (speed == 4){ //stop
                        col = 1;
                        spd = @"Stop";
                    }
                   // NSLog(@"this is speed %@", spd);
                   // NSLog(@"%@", columns[0]);
                    if ([columns[0] isEqualToString: spd]){ //if first column contains either the speed or the correct name
                        NSLog(@"HIIIIII");
                        NSLog(@"%@",columns[col]);
                        NSLog(@"%@",name);
/*          This is now in ParamsSetter
                        if ([name containsString:columns[col]]){ //if the column name is in text file...
                            NSLog(@"I found u %@",columns[col]);
                            //so what name is it?
                            NSMutableArray* idArray;
                            idArray = [self convertNameToId:name];
                            c = [[idArray objectAtIndex:0] intValue];
                            tside = [[idArray objectAtIndex:1] intValue];
                            tseg = [[idArray objectAtIndex:2] intValue];
                            NSLog(@"%d",c);
                            NSLog(@"%d",tside);
                            NSLog(@"%d",tseg);

                            //editParam(c,columns[1],columns[2],columns[3], columns[4], columns[5],columns[6],columns[7]);
                            //  NSLog(@"%d,%@,%@,%@,%@,%@,%@,%@\n",c,columns[1],columns[2],columns[3], columns[4], columns[5],columns[6],columns[7]);
                            cellID[i] = c;
                            side[i] = tside;
                            seg[i] = tseg;
                            alpha[i] = [columns[col+1] floatValue];
                            sigma[i] =[columns[col+2] floatValue];
                            sigmaE[i] =[columns[col+3] floatValue];
                            sigmaI[i] =[columns[col+4] floatValue];
                            betaE[i] =[columns[col+5] floatValue];
                            betaI[i] =[columns[col+6] floatValue];
                            Idc[i] =[columns[col+7] floatValue];
                            
                            NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:i];
                            [params replaceObjectAtIndex:0 withObject:[NSNumber numberWithDouble:alpha[i]]];
                            [params replaceObjectAtIndex:1 withObject:[NSNumber numberWithDouble:sigma[i]]];
                            [params replaceObjectAtIndex:2 withObject:[NSNumber numberWithDouble:sigmaE[i]]];
                            [params replaceObjectAtIndex:3 withObject:[NSNumber numberWithDouble:sigmaI[i]]];
                            [params replaceObjectAtIndex:4 withObject:[NSNumber numberWithDouble:betaE[i]]];
                            [params replaceObjectAtIndex:5 withObject:[NSNumber numberWithDouble:betaI[i]]];
                            [params replaceObjectAtIndex:6 withObject:[NSNumber numberWithDouble:Idc[i]]];
                            
                        }
 */
                    }
                    
                }
                
            }
            //make changes to param
          //  }
    
        //}
    
}



//load params load in a txt file and set the params
- (IBAction)loadParams:(id)sender {
 /*   NSArray *fileURLs = [NSArray arrayWithObjects:nil];
    [[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:fileURLs];
    
    [[NSWorkspace sharedWorkspace] selectFile:fileURLs inFileViewerRootedAtPath:nil];*/
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];

    //the trace waveform
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];

    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setCanChooseFiles:YES];
    [openDlg setCanChooseDirectories:YES];
    
    if ( [openDlg runModal] == NSModalResponseOK )  // See #1
    {
        for( NSURL* URL in [openDlg URLs] )  // See #2, #4
        {
            NSLog( @"%@", [URL path] );
            //NOW READ IN THE FILE and LOAD the PARAMS
            NSString *content = [NSString stringWithContentsOfFile:[URL path]
                                                          encoding:NSUTF8StringEncoding
                                                             error:NULL];
           // NSLog(@"%@",content);
            NSArray* rows = [content componentsSeparatedByString:@"\n"];

            //loop thru all the trace and find them in the text files
            for (int i =0;i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i++){
                NSString* name = [[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:i];
               // int c = 0;
/*
                for (NSString *row in rows){
                        NSArray* columns = [row componentsSeparatedByString:@","];
                        if ([name containsString:columns[0]]){ //if the column name is in text file...
                            NSLog(@"Hi there %@",columns[0]);
                            NSLog(@"yes, i found u");
                        //so what name is it?
                            NSMutableArray* chosenCellArray;
                            chosenCellArray = [self convertNameToId:name];
                            int c = [[chosenCellArray objectAtIndex:0] intValue];
                            int tside =[[chosenCellArray objectAtIndex:1] intValue];
                            int tseg =[[chosenCellArray objectAtIndex:2] intValue];

                        //editParam(c,columns[1],columns[2],columns[3], columns[4], columns[5],columns[6],columns[7]);
                          //  NSLog(@"%d,%@,%@,%@,%@,%@,%@,%@\n",c,columns[1],columns[2],columns[3], columns[4], columns[5],columns[6],columns[7]);
                            cellID[i] = c;
                            side[i]  = tside;
                            seg[i] = tseg;
                            alpha[i] = [columns[1] floatValue];
                            sigma[i] =[columns[2] floatValue];
                            sigmaE[i] =[columns[3] floatValue];
                            sigmaI[i] =[columns[4] floatValue];
                            betaE[i] =[columns[5] floatValue];
                            betaI[i] =[columns[6] floatValue];
                            Idc[i] =[columns[7] floatValue];
                            
                            NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:i];
                            [params replaceObjectAtIndex:0 withObject:[NSNumber numberWithDouble:alpha[i]]];
                            [params replaceObjectAtIndex:1 withObject:[NSNumber numberWithDouble:sigma[i]]];
                            [params replaceObjectAtIndex:2 withObject:[NSNumber numberWithDouble:sigmaE[i]]];
                            [params replaceObjectAtIndex:3 withObject:[NSNumber numberWithDouble:sigmaI[i]]];
                            [params replaceObjectAtIndex:4 withObject:[NSNumber numberWithDouble:betaE[i]]];
                            [params replaceObjectAtIndex:5 withObject:[NSNumber numberWithDouble:betaI[i]]];
                            [params replaceObjectAtIndex:6 withObject:[NSNumber numberWithDouble:Idc[i]]];
                            
                    }
                }
 */

            }
            //make changes to param
/*   This is now in ParameterSetter
 editParam(cellID,side, seg,alpha,sigma,sigmaE, sigmaI, betaE, betaI,Idc, [[[appDelegate traceSelector] traceArraytobeSent] count]);

            if (firstTimeChangeParams == 1){
                [appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
                [self setFirstTimeChangeParams:0]; //now it's set
            }else{
                setParamsButton.enabled = false;
             //   int currentIndex = 0;
             //   while (currentIndex != 9999){ //should be tmax, not hardcoded btw
             //       currentIndex = 0;
             //       currentIndex = checkMainLoopIndex();
             //   } //xmain should end by now
                //sleep(0.1);
                
                ///Stabilize by making it run on foreground instead of being a background process, so it has to finish
                
                [appDelegate performSelector:@selector(createWaveForm) withObject:nil];
                //[appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
                setParamsButton.enabled = true;
                
            }*/

        }
    }

}

- (IBAction)saveParams:(id)sender {
    
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setCanChooseFiles:YES];
    [openDlg setCanChooseDirectories:YES];
    
    if ( [openDlg runModal] == NSModalResponseOK )  // See #1
    {
        for( NSURL* URL in [openDlg URLs] )  // See #2, #4
        {
            NSLog( @"%@", [URL path] );
            //NOW READ IN THE FILE and LOAD the PARAMS
            NSMutableString *writeString = [NSMutableString stringWithCapacity:0]; //don't worry about the capacity, it will expand as necessary
            
            for (int i =0;i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i++){
                
                NSString* name = [[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:i];
                NSString* c;
                    //so what name is it?
                    if ([name containsString:@"Elevator"]){
                        c = @"Elevator";
                    }
                    else if ([name containsString:@"Swing"]){
                        c = @"Swing";
                    }
                    else if ([name containsString:@"Depressor"]){
                        c = @"Depressor";
                    }
                    else if ([name containsString:@"Stance"]){
                        c = @"Stance";
                    }
                    else if ([name containsString:@"Coord"]){
                        c = @"Coord";
                    }
                    else if ([name containsString:@"Protractor"]){
                        c = @"Protractor";
                    }
                    else if ([name containsString:@"Retractor"]){
                        c = @"Retractor";
                    }
                    else if ([name containsString:@"Extensor"]){
                        c = @"Extensor";
                    }
                    else if ([name containsString:@"Flexor"]){
                        c = @"Flexor";
                    }
                    else if ([name containsString:@"FL"] && ![name containsString:@"exor"]){
                        c = @"FL";
                    }
                    else if ([name containsString:@"FR"]){
                        c = @"FR";
                    }
                    else if ([name containsString:@"BL"]) {
                        c = @"BL";
                    }
                    else if ([name containsString:@"BR"]){
                        c = @"BR";
                    }
                    else if ([name containsString:@"LL"]){
                        c = @"LL";
                    }
                    else if ([name containsString:@"LT"]){
                        c = @"LT";
                    }
                    else if ([name containsString:@"Pcn"]){
                        c = @"Pcn";
                    }
                    else if ([name containsString:@"ModCom"]){
                        c = @"ModCom";
                    }
                    else if ([name containsString:@"HL"]){
                        c = @"HL";
                    }
                    else if ([name containsString:@"HR"]){
                        c = @"HR";
                    }


                id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];

                NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:i];
                NSString*a = [[params objectAtIndex:0] stringValue];
                NSString*s = [[params objectAtIndex:1] stringValue];
                NSString*sE = [[params objectAtIndex:2] stringValue];
                NSString*sI = [[params objectAtIndex:3] stringValue];
                NSString*bE = [[params objectAtIndex:4] stringValue];
                NSString*bI = [[params objectAtIndex:5] stringValue];
                NSString*Idc = [[params objectAtIndex:6] stringValue];
         
                [writeString appendString:[NSString stringWithFormat:@"%@,%@,%@,%@,%@,%@,%@,%@\n",c,a,s,sE,sI,bE,bI,Idc]]; //the \n will put a newline in
            }
        //Moved this stuff out of the loop so that you write the complete string once and only

            NSLog(@"writeString :%@",writeString);
            
            NSFileHandle *handle;
            handle = [NSFileHandle fileHandleForWritingAtPath: [URL path] ];
            //say to handle where's the file fo write
            [handle truncateFileAtOffset:[handle seekToEndOfFile]];
            //position handle cursor to the end of file
            [handle writeData:[writeString dataUsingEncoding:NSUTF8StringEncoding]];
        }
    }

}
@end


