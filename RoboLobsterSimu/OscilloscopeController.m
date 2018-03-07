
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
typedef id MovieAudioExtractionRef;

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

@synthesize cellNameTextBox;
@synthesize alphaTextBox;
@synthesize sigmaTextBox;
@synthesize sigmaETextBox;
@synthesize sigmaITextBox;
@synthesize betaETextBox;
@synthesize betaITextBox;
@synthesize IdcTextBox;

@synthesize traceIDchosen;
@synthesize AllTracesInfo;
@synthesize IndividualTraceInfo;
@synthesize displayTraceID;


@synthesize alpha;
@synthesize sigma;
@synthesize sigmaE;
@synthesize sigmaI;
@synthesize betaE;
@synthesize betaI;
@synthesize Idc;
@synthesize cellID;

@synthesize settingUpParams; //load param value into array
@synthesize setParamsButton;
@synthesize firstTimeChangeParams;

-(id) init
{
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
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
    
    int numCell = [[[appDelegate traceSelector] traceArraytobeSent] count];
    
    //init the array that will contains input param values
    //cell ID tell C code what type of cell it is
    cellID = malloc(numCell*sizeof(int));
    
    //those are the array of params for each cell
    alpha = malloc(numCell*sizeof(double));
    sigma = malloc(numCell*sizeof(double));
    sigmaE = malloc(numCell*sizeof(double));
    sigmaI = malloc(numCell*sizeof(double));
    betaE = malloc(numCell*sizeof(double));
    betaI = malloc(numCell*sizeof(double));
    Idc = malloc(numCell*sizeof(double));
    
    settingUpParams = 1; //first time setting up
    
    for (int i = 0; i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i ++){
        
        //get all Cell Name and convert those names to ID
        NSString*cellName = [[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:i];
        cellID[i] = [self convertNameToId:cellName];
        
        
        
        [displayTraceID addItemWithTitle:[NSString stringWithFormat:@"%d", i]];
        
        
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
        
    
    }
    
    //Set sigma and alpha to what they really are
    
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    
    //this is the param id that contains sigma and alpha
  //  NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:traceIDchosen];
  //  [alphaTextBox setStringValue:[[params objectAtIndex:0] stringValue]];
    
  //  [sigmaTextBox setStringValue:[[params objectAtIndex:1] stringValue]];
  //  [alphaTextBox setStringValue:[NSString stringWithFormat:@"%f",5.5]];
  //  [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",1.2]];
    
    //make it bidirectional later
    
    //set trace name first//
    [cellNameTextBox setStringValue:[[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:0]];
    
    [traceGainTextBox setDelegate:self];
    [sweepDurationTextBox setDelegate:self];
    [sweepOffsetTextBox setDelegate:self];
    [traceOffsetTextBox setDelegate:self];
    
    
    [alphaTextBox setDelegate:self];
    [sigmaTextBox setDelegate:self];
    [sigmaETextBox setDelegate:self];
    [sigmaITextBox setDelegate:self];
    [betaETextBox setDelegate:self];
    [betaITextBox setDelegate:self];
    [IdcTextBox setDelegate:self];
    
    [self setFirstTimeChangeParams:1]; //never change params
     NSTimer *t = [NSTimer scheduledTimerWithTimeInterval: 1                                                  target: self
                                                selector:@selector(beginSendingStuffToBeDrawn)
                                              userInfo: nil repeats:YES];
           // [self setOscilloscopeView:oscilloscopeView];
}



- (IBAction)chooseTraceID:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    traceIDchosen = [[[self displayTraceID] title] integerValue];
    
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
  

    //set trace ID in appDelegate
    [appDelegate setIDofCellChosen:traceIDchosen];
    
    //this is the param id that contains sigma and alpha
    NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:traceIDchosen];
    
    //init all params
    int numCell = [[[appDelegate traceSelector] traceArraytobeSent] count];


    
    //init params input values
    if (settingUpParams){
        for (int i = 0; i < numCell; i++){
            NSLog(@"howw many %d",i);
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
    

   // else{
        [alphaTextBox setStringValue: [NSString stringWithFormat:@"%f",alpha[traceIDchosen]]];
         [sigmaTextBox setStringValue:[NSString stringWithFormat:@"%f",sigma[traceIDchosen]]];
        [sigmaETextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaE[traceIDchosen]]];
        [sigmaITextBox setStringValue:[NSString stringWithFormat:@"%f",sigmaI[traceIDchosen]]];
        [betaETextBox setStringValue:[NSString stringWithFormat:@"%f",betaE[traceIDchosen]]];

        [betaITextBox setStringValue:[NSString stringWithFormat:@"%f",betaI[traceIDchosen]]];

        [IdcTextBox setStringValue:[NSString stringWithFormat:@"%f",Idc[traceIDchosen]]];

    
        
   // }
    
    
    
    
   // if (traceIDchosen == 0 || traceIDchosen == 1){
   //     [sigmaTextBox setEditable:(false)];
   // }
   // else{
   //     [sigmaTextBox setEditable:(true)];
   // }
    NSLog(@" choose this trace %d", traceIDchosen);
    [cellNameTextBox setStringValue:[[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:traceIDchosen]];

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
    traceIDchosen = [[[self displayTraceID] title] integerValue];
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
}

-(void) createWaveForm{
    xmain(); //run pc2dsp and write to temp files, automatically run in background async
    // NSLog(@"total elapse time for 1 iter = %f s", elapsedTime);
}



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

- (void)beginSendingStuffToBeDrawn {
    
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
   // if ([[appDelegate offsetArray] count] != [[[appDelegate traceSelector] traceArraytobeSent] count]  || ([[appDelegat/e durationArray] count] != [[[appDelegate traceSelector] traceArraytobeSent] count])){
     //           _startButton.enabled = NO;
      //      }
    //else{
        
      //  _startButton.enabled = YES;
       // NSLog(@"is it doing anything?");
        [appDelegate displaySampledWaveforms: [[appDelegate traceSelector] traceArraytobeSent]  : [appDelegate sweepOffset] :[appDelegate sweepDuration]];
    
        
    //}
  //  [displayTraceID removeAllItems];
  //  for (int i; i < [[[appDelegate traceSelector] traceArraytobeSent] count]; i ++){
  //      [displayTraceID addItemWithTitle:[NSString stringWithFormat:@"%d", i]];
  //  }
}
- (IBAction)changeView:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    if ([appDelegate switchColor] == 0){
    [appDelegate setSwitchColor: 1];
    }
    else{
        [appDelegate setSwitchColor:0];
    }
}



- (int)convertNameToId: (NSString*)name{
    int  c;
    if ([name containsString:@"Elevator"]){
        c = 0;
    }
    else if ([name containsString:@"Swing"]){
        c = 1;
    }
    else if ([name containsString:@"Depressor"]){
        c = 2;
    }
    else if ([name containsString:@"Stance"]){
        c = 3;
    }
    else if ([name containsString:@"Coord"]){
        c = 4;
    }
    else if ([name containsString:@"Protractor"]){
        c = 5;
    }
    else if ([name containsString:@"Retractor"]){
        c = 6;
    }
    else if ([name containsString:@"Extensor"]){
        c = 7;
    }
    else if ([name containsString:@"Flexor"]){
        c = 8;
    }
    else{
        c = 0;
    }

    return c;
}


- (IBAction)setParams:(id)sender {
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];

//    NSLog(@"%f",[alphaTextBox floatValue]);
  /*  double a = [alphaTextBox doubleValue];
    double s = [sigmaTextBox doubleValue];
    double sE = [sigmaETextBox doubleValue];
    double sI = [sigmaITextBox doubleValue];
    double bE = [betaETextBox doubleValue];
    double bI = [betaITextBox doubleValue];
    double Idc = [IdcTextBox doubleValue];*/
    traceIDchosen = [[[self displayTraceID] title] integerValue];
    
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    
    //set trace ID in appDelegate
    //this is the param id that contains sigma and alpha
    int numCell = [[[appDelegate traceSelector] traceArraytobeSent] count];

    for (int i = 0; i < numCell;i++){
        //loop through all the param array and change them with the users input param!
    NSMutableArray*params= [[propertyValue parambuf] objectAtIndex:i];
    [params replaceObjectAtIndex:0 withObject:[NSNumber numberWithDouble:alpha[i]]];
    [params replaceObjectAtIndex:1 withObject:[NSNumber numberWithDouble:sigma[i]]];
    [params replaceObjectAtIndex:2 withObject:[NSNumber numberWithDouble:sigmaE[i]]];
    [params replaceObjectAtIndex:3 withObject:[NSNumber numberWithDouble:sigmaI[i]]];
    [params replaceObjectAtIndex:4 withObject:[NSNumber numberWithDouble:betaE[i]]];
    [params replaceObjectAtIndex:5 withObject:[NSNumber numberWithDouble:betaI[i]]];
    [params replaceObjectAtIndex:6 withObject:[NSNumber numberWithDouble:Idc[i]]];
    }


    //Now send all the array to edit Param!
    editParam(cellID,alpha,sigma,sigmaE, sigmaI, betaE, betaI,Idc);
    
    if (firstTimeChangeParams == 1){
        [appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
        [self setFirstTimeChangeParams:0]; //now it's set
    }else{
        setParamsButton.enabled = false;
        int currentIndex = 0;
        while (currentIndex != 99999){ //should be tmax, not hardcoded btw
            currentIndex = 0;
            currentIndex = checkMainLoopIndex();
        } //xmain should end by now
        //sleep(0.1);
        
        ///Stabilize by making it run on foreground instead of being a background process, so it has to finish
        
        [appDelegate performSelector:@selector(createWaveForm) withObject:nil];
        //[appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
        setParamsButton.enabled = true;
        
    }
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
                int c = 0;
                
                for (NSString *row in rows){
                        NSArray* columns = [row componentsSeparatedByString:@","];
                        if ([name containsString:columns[0]]){ //if the column name is in text file...
                            NSLog(@"this is %@",columns[0]);
                        //so what name is it?
                            int c = [self convertNameToId:name];
                        //editParam(c,columns[1],columns[2],columns[3], columns[4], columns[5],columns[6],columns[7]);
                          //  NSLog(@"%d,%@,%@,%@,%@,%@,%@,%@\n",c,columns[1],columns[2],columns[3], columns[4], columns[5],columns[6],columns[7]);
                            cellID[i] = c;
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

            }
            //make changes to param
            editParam(cellID,alpha,sigma,sigmaE, sigmaI, betaE, betaI,Idc);

            if (firstTimeChangeParams == 1){
                [appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
                [self setFirstTimeChangeParams:0]; //now it's set
            }else{
                setParamsButton.enabled = false;
                int currentIndex = 0;
                while (currentIndex != 99999){ //should be tmax, not hardcoded btw
                    currentIndex = 0;
                    currentIndex = checkMainLoopIndex();
                } //xmain should end by now
                //sleep(0.1);
                
                ///Stabilize by making it run on foreground instead of being a background process, so it has to finish
                
                [appDelegate performSelector:@selector(createWaveForm) withObject:nil];
                //[appDelegate performSelectorInBackground:@selector(createWaveForm) withObject:nil];
                setParamsButton.enabled = true;
                
            }

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






///////////////////OLD CODE///////////////////

/*
 - (IBAction)changeGainStep:(id)sender {
 [gainStepView setIncrement: 0.1];
 [gainStepView setMaxValue: 10.0];
 
 [traceGainSlider setFloatValue:[sender floatValue]];
 [traceGainTextBox setFloatValue:[sender floatValue]];
 }*/

/*
 - (IBAction) resetSweepDuration: (id) sender{          //Reset Samples Per Column
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
 NSRect iRect = [[[appDelegate analysisWindowController] oscilloscopeView] bounds];
	[self setDoInitAndScale:NO];
 [self setNumColumns:        iRect.size.width];              //This is the width of the graphics window
 [self setSweepDuration:[sender doubleValue]];
 NSLog(@"resetSweepDuration returns %8.3f",[sender doubleValue]);
 
 [self setSamplingOffset:[self sweepOffset]*[self samplingRate]];
 long sTerm =   ([self sweepDuration]+[self sweepOffset])*[self samplingRate];
 if (sTerm > [self samplingMaximum]) {
 sTerm = [self samplingMaximum];
 }
 [self setSamplingTerminus:sTerm];
 [self setSamplingDuration:  [self samplingTerminus] - [self samplingOffset]];
 [self setSweepOffset:       [self samplingOffset]/[self samplingRate]];                           //This is the sweep onset in sec
 [self setSweepTerminus:     [self samplingTerminus]/[self samplingRate]];                         //This is the sweep terminus in sec.
 
 [self setSamplingDuration: [self samplingTerminus] - [self samplingOffset]];  //This is the duraion of the sweep in samples
	
 [self setSampsPerCol:[self samplingDuration]/[self numColumns]];
 
 [self scaleScopeSweep];
 NSLog(@"\nFollowing resetSweepDuration\n samplingMaximum:%ld \n numColumns:%ld \n samplingOffset:%ld \n samplingTerminus: %ld \n samplingDuration: %ld \n  samplingRate:%8.0f \n sampsPerCol:      %8.0f \n sweepOffset:%8.3f \n sweepTerminus:%8.3f \n  sweepDuration:%8.3f \n  sweepMaximum:%8.3f ",
 
 [self samplingMaximum],
 [self numColumns],
 [self samplingOffset],
 [self samplingTerminus],
 [self samplingDuration],
 [self samplingRate],
 [self sampsPerCol],
 [self sweepOffset],
 [self sweepTerminus],
 [self sweepDuration],
 [self sweepMaximum]);
 [[[appDelegate analysisWindowController] myCurrentView] setNeedsDisplay:YES];
 //[[self self] drawScopeSweep ];//]:rect];
 }*/

/*
 
 - (IBAction) resetGainProx: (id) sender{
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
	float percent;
	float ProxGain;
	float oscProxGainMax = [self proxGainMax];
	[self setDoInitAndScale:NO];
	percent = [sender intValue];
	ProxGain =  (oscProxGainMax / 100) * percent;
	[self setProxGain:ProxGain];
	
	[[[appDelegate analysisWindowController] myCurrentView] setNeedsDisplay:YES];
	
 }
 
 
 - (IBAction) resetOffsetProx: (id) sender{
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
	float percent;
	int ProxOffset;
	[self setDoInitAndScale:NO];
	percent = [sender intValue];
 NSRect  iRect = [[[appDelegate analysisWindowController] myCurrentView] bounds];
 //    NSRect irect = [[[[appDelegate analysisWindowController] myCurrentView] gRect];
	ProxOffset =  ((iRect.size.height / 2) / 100) * percent + iRect.size.height / 2;
	[self setProxOffset:ProxOffset];
	[[[appDelegate analysisWindowController] myCurrentView] setNeedsDisplay:YES];
	
 }*/


/*- (void) importAudioFromFile:(id)sender
 {
	_doInitAndScale = YES;
	_drawSpikes = YES;
	_drawClusteredSpikes = NO;
	_drawNoiseLevel = NO;
	_drawCluster = NO;
 }*/

/*   These are the sampling properties for the Oscilloscope and Chart displays
 long   samplingOffset;     //Sweep offset from origin in samples
 long   samplingTerminus;   //Sweep Terminus from origin in samples
 long   samplingMaximum;    //Duration of file in samples
 double samplingDuration;   //Duration of Sweep in samples
 
 double samplingRate;       //SamplingRate in Hz
 long   numColumns;          //Number of columns in the display
 double sampsPerCol;        //Samples per Pixel
 
 double sweepOffset;        //Sweep offset from origin in Sec
 double sweepTerminus;      //Terminus of Sweep in Sec
 double sweepDuration;      //Duration of Sweep in Sec
 double sweepMaximum;       //Duration of file in Sec
 */

/*
 - (void)initScopeSweep{                                         //Initialize [self with the current file data
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
 // [[appDelegate traceWaveforms] sendToBuffer:cellDepressorL2 :100];
	if (!self.assetInited)
 //		[self loadDSPSourceFromAsset:appDelegate.movieController.videoPlaybackController.asset];
	//    NSRect iRect = [[[appDelegate analysisWindowController] oscilloscopeView] bounds];
 //    [[self setNumColumns:        iRect.size.width];              //This is the width of the graphics window
 [self setNumColumns: 754];              //This is the width of the oscilloscopeView
 //  [self setSamplingMaximum:1000]; //added this too
	[self setSampsPerCol:(      _samplingMaximum/_numColumns)];          //This is the gain of the timebase in samples/col
	[self setSamplingOffset:    0];                             //This is the pointer for the  offset of the data sample
 [self setSamplingTerminus:  _samplingMaximum];                      //This is the pointer for the terminus of the data sample
 [self setSweepOffset:       0.0];                           //This is the sweep onset in sec
 [self setSweepTerminus:     _samplingMaximum/_samplingRate];         //This is the sweep terminus in sec.
 [self setSamplingDuration:  _samplingMaximum];
 NSLog(@"\nFollowing initScopeSweep\n samplingMaximum: %ld \n samplingDuration: %ld \n numColumns: %ld \n samplingOffset: %ld \n samplingTerminus: %ld \n samplingRate: %8.1f \n sampsPerCol: %8.1f \n sweepOffset: %8.3f \n sweepTerminus: %8.3f \n sweepDuration: %8.3f \n sweepMaximum: %8.3f ",
 [self samplingMaximum],
 [self samplingDuration],
 [self numColumns],
 [self samplingOffset],
 [self samplingTerminus],
 [self samplingRate],
 [self sampsPerCol],
 [self sweepOffset],
 [self sweepTerminus],
 [self sweepDuration],
 [self sweepMaximum]
 );
 [[appDelegate analysisWindowController] setScopeInited:YES];
 }*/
/*
 - (void)logScopeSweep{   //Log Contents of self with the current file data
 //    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
 NSLog(@"\nScope Sweep Parameters\n samplingMaximum: %ld \n samplingDuration: %ld \n numColumns: %ld \n samplingOffset: %ld \n samplingTerminus: %ld \n samplingRate: %8.1f \n sampsPerCol: %8.1f \n sweepOffset: %8.3f \n sweepTerminus: %8.3f \n sweepDuration: %8.3f \n sweepMaximum: %8.3f ",
 [self samplingMaximum],
 [self samplingDuration],
 [self numColumns],
 [self samplingOffset],
 [self samplingTerminus],
 [self samplingRate],
 [self sampsPerCol],
 [self sweepOffset],
 [self sweepTerminus],
 [self sweepDuration],
 [self sweepMaximum]
 );
 }*/
/*
 - (void)scaleScopeSweep{
	int samp;
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
 
	_maxIp = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:0] floatValue];
	_minIp = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:0] floatValue];
	
	for (samp = 0; samp < [[[appDelegate traceWaveforms] ipbuf] count]; samp++) {
 
 
 if ([[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:samp] floatValue] >= _maxIp) {
 _maxIp = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:samp] floatValue];
 NSLog(@" max = %f", _maxIp);
 }
 
 if ([[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:samp] floatValue] <= _minIp)
 {
 _minIp = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:samp] floatValue];
 NSLog(@" min = %f", _minIp);
 }
 }
 
	
 NSRect iRect = [[[appDelegate analysisWindowController] oscilloscopeView] bounds];
 
	if (_maxIp > _minIp * -1) {
 _proxGain = iRect.size.height / (_maxIp * 3);
 } else
 _proxGain = iRect.size.height / (_minIp * -3);
	proxGainMax = _proxGain;
 NSLog(@"In ScaleSweep: proxGain is %f ", _proxGain);
 
 }
 */

/*
 - (void)setDoInitAndScale:(BOOL)flag{
	_doInitAndScale = flag;
 }*/

#pragma mark --IBAction for Oscilloscope Drawer--
//Timebase Variables

/*   These are the sampling properties for the Oscilloscope and Chart displays
 long   samplingOffset;     //Sweep offset from origin in samples
 long   samplingTerminus;   //Sweep Terminus from origin in samples
 long   samplingMaximum;    //Duration of file in samples
 double samplingDuration;   //Duration of Sweep in samples
 double samplingRate;       //SamplingRate in Hz
 long   numColumns;         //Number of columns in the display
 double sampsPerCol;        //Samples per Pixel
 
 double sweepOffset;        //Sweep offset from origin in Sec
 double sweepTerminus;      //Terminus of Sweep in Sec
 double sweepDuration;      //Duration of Sweep in Sec
 double sweepMaximum;       //Duration of file in Sec
 */

#pragma mark --Display controls for timebase and waveform--

/*
 - (IBAction) resetSweepOffset: (id) sender{
 
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
 NSRect iRect = [[[appDelegate analysisWindowController] oscilloscopeView] bounds];
 [self setNumColumns:        iRect.size.width];              //This is the width of the graphics window
	[self setDoInitAndScale:    NO];
 [self setSweepOffset:       [sender doubleValue]];          //Read the slider value here
 [self setSamplingOffset:    [self sweepOffset]*[self samplingRate]];
 NSLog(@"resetSweepOffset returns %8.3f",[sender doubleValue]);
 
 //First Compute parameters in Seconds
 long sweepMaxDur = [self sweepMaximum] - [self sweepOffset];
 if([self sweepDuration] > sweepMaxDur)
 [self setSweepDuration:sweepMaxDur];
 [self setSweepTerminus:[self sweepOffset]+[self sweepDuration]];
 
 [self setSamplingOffset:    [self sweepOffset]*[self samplingRate]];   //  <====The slider returns the offset
 [self setSamplingTerminus: [self sweepTerminus]*[self samplingRate]];
 [self setSamplingDuration:  [self samplingTerminus] - [self samplingOffset]];
	[self setSampsPerCol:(      [self samplingDuration] / [self numColumns])]; //This is the gain of the timebase in samples/col
 [self scaleScopeSweep];
 
 NSLog(@"\nFollowing resetSweepOffset\n samplingMaximum: %ld \n numColumns: %ld \n samplingOffset: %ld \n samplingTerminus: %ld \n samplingDuration: %ld \n samplingRate: %8.1f \n sampsPerCol: %8.1f \n sweepOffset: %8.3f \n sweepTerminus: %8.3f \n sweepDuration: %8.3f \n sweepMaximum: %8.3f ",
 [self samplingMaximum],
 [self numColumns],
 [self samplingOffset],
 [self samplingTerminus],
 [self samplingDuration],
 [self samplingRate],
 [self sampsPerCol],
 [self sweepOffset],
 [self sweepTerminus],
 [self sweepDuration],
 [self sweepMaximum]
 );
	[[[appDelegate analysisWindowController] myCurrentView] setNeedsDisplay:YES];
 //[[self self] drawScopeSweep ];//]:rect];
	
 }*/
