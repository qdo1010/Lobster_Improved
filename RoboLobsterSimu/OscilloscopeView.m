//
//  OscilloscopeView.m
//  Roboplasm
//
//  Created by Joseph Ayers on 6/20/13.
//  Copyright (c) 2013 Northeastern Univeristy. All rights reserved.
//

#import "AppDelegate.h"
#import "OscilloscopeController.h"
#import "OscilloscopeView.h"
#import "Waveforms.h"
//#import "DSPSources.h"
//#import "signalProcessing.h"

@implementation OscilloscopeView
BOOL drawXGridFlag = YES;
BOOL drawYGridFlag = YES;
float xgMax = 10.0;
float ygMax = 10.0;
float xgMin = 0.0;
float ygMin = 0.0;
float xgScale = 1.0;
float ygScale = 1.0;
int xMinorLineCount  = 0;
int yMinorLineCount	 = 0;
int majorLineWidth	 = 1;
int minorLineWidth	 = 1;

//@synthesize oscilloscopeView;
@synthesize graphColors;
@synthesize wRect;
@synthesize gRect;

- (void)setDefaultScopeBounds{
    //Set Default Graph Bounds							//IMPORTANT NAMING CONVENTION
    xgMin = 0;   xgMax = 100;  xgScale = 1;				//Data Point = (g,h)  variables prefixed by g/h refer to data values
    ygMin = 0;   ygMax = 100;  ygScale = 1;				//PixelPoint = (x,y)  variables prefixed by x/y refer to pixel coordinates
}


- (void) awakeFromNib {
    
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    [[appDelegate analysisWindowController] setOscilloscopeView:self];
  //  NSLog(@"wtttttt");
}
- (id)initWithFrame:(NSRect)frame{
    //    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    
    self = [super initWithFrame:frame];
    if (self) {
        [self setDefaultScopeBounds];
        // Initialization code here.
        NSLog(@"scope");
        
        
    }
    
    return self;
}
- (void)drawXGrid:(NSRect)rect
{
    graphColors = [[NSColorList alloc] initWithName:@"Graph Colors"];
    if(drawXGridFlag == YES)
    {
        NSBezierPath *majorGridLines = [[NSBezierPath alloc] init];	//creates series of lines that will be gridlines on x
        NSBezierPath *minorGridLines = [[NSBezierPath alloc] init];
        //		NSBezierPath *tickMarks      = [[NSBezierPath alloc] init];
        
        const float xMax = NSMaxX(rect);	//bounds of graph - stored as constants
        const float xMin = NSMinX(rect);	// for preformance reasons(used often)
        const float yMax = NSMaxY(rect);
        const float yMin = NSMinY(rect);
        
        const float xratio = (xgMax - xgMin)/(xMax - xMin); //ratio ∆Data/∆Coordinate -> dg/dx
        const float xorigin = (0 - xgMin)/(xratio) + xMin; //x component of the origin
        const float xScale = xgScale / xratio; //transform scale
        
        float x;   //view coordinate x
        
        for(x = xorigin - (xorigin-xMin+1)/xScale*xScale; x <= xMax; x += xScale)	//Begin 1 scale unit outside of graph
        {																					//End 1 scale unit outside of graph
            [majorGridLines moveToPoint:NSMakePoint(x,yMax)];
            [majorGridLines lineToPoint:NSMakePoint(x,yMin)];
            
            float minor;
            
            for(minor=1; minor < xMinorLineCount; minor++)
            {
                [minorGridLines moveToPoint:NSMakePoint(x+minor/xMinorLineCount*xScale,yMax)];
                [minorGridLines lineToPoint:NSMakePoint(x+minor/xMinorLineCount*xScale,yMin)];
            }
        }
        
        // [[graphColors colorWithKey:@"xMajor"] set];
        [[NSColor whiteColor] set];
        //set color for grindlines
        [majorGridLines setLineWidth:majorLineWidth];			//make line width consistent with env. variable
        [majorGridLines stroke];
        
        //  [[graphColors colorWithKey:@"xMinor"] set];
        [[NSColor whiteColor] set];
        //set color for grindlines
        [minorGridLines setLineWidth:minorLineWidth];			//make line width consistent with env. variable
        [minorGridLines stroke];
    }
}

- (void)drawYGrid:(NSRect)rect
{
    graphColors = [[NSColorList alloc] initWithName:@"Graph Colors"];
    
    if(drawYGridFlag == YES)
    {
        NSBezierPath *majorGridLines = [[NSBezierPath alloc] init];	//creates series of lines that will be gridlines on y
        NSBezierPath *minorGridLines = [[NSBezierPath alloc] init];
        
        const float xMax = NSMaxX(rect);	//bounds of graph - stored as constants
        const float xMin = NSMinX(rect);	// for preformance reasons(used often)
        const float yMax = NSMaxY(rect);
        const float yMin = NSMinY(rect);
        const float yratio = (ygMax - ygMin)/(yMax - yMin); //ratio ∆Data/∆Coordinate -> dh/dy
        const float yorigin = (0 - ygMin)/(yratio) + yMin; //y component of the origin
        const float yScale = ygScale / yratio; //transform scale
        float  y;   //view coordinate y
        
        for(y = yorigin - (yorigin-yMin+1)/yScale*yScale; y <= yMax; y += yScale)	//Begin 1 scale unit outside of graph
        {																					//End 1 scale unit outside of graph
            [majorGridLines moveToPoint:NSMakePoint(xMax,y)];
            [majorGridLines lineToPoint:NSMakePoint(xMin,y)];
            
            float minor;
            
            for(minor=1; minor < yMinorLineCount; minor++)
            {
                [minorGridLines moveToPoint:NSMakePoint(xMax, y+minor/yMinorLineCount*yScale)];
                [minorGridLines lineToPoint:NSMakePoint(xMin, y+minor/yMinorLineCount*yScale)];
            }
        }
        
        
        //   [[graphColors colorWithKey:@"yMajor"] set];
        [[NSColor whiteColor] set];//set color for grindlines
        [majorGridLines setLineWidth:majorLineWidth];			//make line width consistent with env. variable
        [majorGridLines stroke];
        
        [[NSColor whiteColor] set];
        //  [[graphColors colorWithKey:@"yMinor"] set];				//set color for grindlines
        [minorGridLines setLineWidth:minorLineWidth];			//make line width consistent with env. variable
        [minorGridLines stroke];
    }
}


- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
    //[[self window] setFrameOrigin:NSMakePoint(0, dirtyRect.size.height)];
    [[self window] setAllowsConcurrentViewDrawing:YES];
    // [self setCanDrawConcurrently:YES];
    [NSGraphicsContext saveGraphicsState];
    if ([(AppDelegate *)[[NSApplication sharedApplication] delegate] switchColor] == 0){
        [[NSColor blackColor] setFill];
    }
    else{
        [[NSColor whiteColor] setFill];
    }
    [[NSBezierPath bezierPathWithRoundedRect:[self bounds] xRadius:0.0 yRadius:0.0] fill];
    // [NSGraphicsContext restoreGraphicsState];
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    int numTr = [[propertyValue ipbuf] count];
    
    // [self drawXGrid:dirtyRect];
    // [self drawYGrid:dirtyRect];
    NSMutableArray*container; //contain stuff to be drawn
    int width = dirtyRect.size.width;
    int height = dirtyRect.size.height/numTr;
    for (int j = 0 ; j < numTr; j ++){
        container = [[NSMutableArray alloc] init];
        // points = [[propertyValue ipbuf] objectAtIndex:j];
        // [container addObject: points];
        [container addObject: [NSNumber numberWithInteger:j]];
        [container addObject: [NSNumber numberWithInteger:width]];
        [container addObject: [NSNumber numberWithInteger:height]];
        [self performSelector: @selector(drawTraces:) withObject:container];
        [NSGraphicsContext saveGraphicsState];
    }
}


-(void) drawTraces :(NSMutableArray*) container{
    [NSGraphicsContext restoreGraphicsState];
    id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    int idy = [[container objectAtIndex:0] intValue];
    
    NSMutableArray*points = [[propertyValue ipbuf] objectAtIndex:idy];
    int width = [[container objectAtIndex:1] intValue];
    int height =  [[container objectAtIndex:2] intValue];
    int numberOfPoints = (int)[points count];
    //NSLog(@"%@", points);
    //int numberOfSineCurves = 30;
    //float cWidth = 1;
    float cWidth = ((float)width)/[(AppDelegate *)[[NSApplication sharedApplication] delegate] sweepDuration];
   // NSLog(@"duration of %d = %f", idy ,[[[(AppDelegate *)[[NSApplication sharedApplication] delegate] durationArray] objectAtIndex:idy] floatValue]);
   // float cWidth = 0.5;
    float cHeight = height/10;
    NSBezierPath *sinePath = [NSBezierPath bezierPath];
    float gain = [[[(AppDelegate *)[[NSApplication sharedApplication] delegate] traceGainArray] objectAtIndex:idy] floatValue];
    float offset = [[[(AppDelegate *)[[NSApplication sharedApplication] delegate] traceOffsetArray] objectAtIndex:idy] floatValue];
    
    [sinePath setLineWidth:1.0];
    //        [sinePath setFlatness:3.0];
    [sinePath moveToPoint:NSMakePoint(0*cWidth,  height/2-[[points objectAtIndex:0] floatValue]*cHeight*gain + height*([[propertyValue ipbuf] count] - idy -1) + offset)];
    
    NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:[NSFont fontWithName:@"Helvetica" size:26], NSFontAttributeName,[NSColor blueColor], NSForegroundColorAttributeName, nil];
    
    
    AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
    NSString*name = [[[appDelegate traceSelector] traceArraytobeSent] objectAtIndex:idy];

    
    NSAttributedString * currentText=[[NSAttributedString alloc] initWithString:name attributes: attributes];
    
    NSSize attrSize = [currentText size];
    [currentText drawAtPoint:NSMakePoint(0*cWidth, height/2-0.05*cHeight*gain + height*([[propertyValue ipbuf] count] - idy -1) + offset + height/5)];
    
    for (int i=0; i < numberOfPoints; i++)
    {
        if ([(AppDelegate *)[[NSApplication sharedApplication] delegate] switchColor] == 0){
            if ([(AppDelegate *)[[NSApplication sharedApplication] delegate] IDofCellChosen]== idy){
                [[NSColor redColor] set];
            }
            else{
                [[NSColor whiteColor] set];
            }
            
        }
        else{
            if ([(AppDelegate *)[[NSApplication sharedApplication] delegate] IDofCellChosen]== idy){
                [[NSColor redColor] set];
            }
            else{
            [[NSColor blackColor] set];
            }
        }
       // [[NSColor whiteColor] set];

        
        if (i < (numberOfPoints - 1)){
            [sinePath lineToPoint:NSMakePoint(i*cWidth, height/2 - [[points objectAtIndex:i+1] floatValue]*cHeight*gain  + height*([[propertyValue ipbuf] count] - idy -1) + offset )];
           // [sinePath setLineWidth:i];
        }
        //NSLog(@"%@",[points objectAtIndex:i+1]);
        
        
    }
    [sinePath stroke];
}
@end



////////////////////////OLD CODE///////////////////////////////////////


/*
 - (void)drawRect:(NSRect)rect   //mainly function calls to more complex implementation
 {
 // [super drawRect:rect];
 //  [NSGraphicsContext saveGraphicsState];
 
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
	if (![[appDelegate analysisWindowController] scopeInited])
 [[[appDelegate analysisWindowController] oscilloscopeController] initScopeSweep];
	//		return;
 
 //   NSRect cRect = [[[appDelegate analysisWindowController] myTabView] contentRect];
 NSLog(@"Executing drawRect from  Oscilloscope View");
 // 	NSLog(@"TabView contentRect %g , %g, w %d h %d\n", NSMinX(cRect), NSMinY(cRect), (int) NSWidth(cRect), (int) NSHeight(cRect));
 //	NSLog(@"Oscilloscope rect on drawRect Entry %g , %g, w %d h %d\n", NSMinX(rect), NSMinY(rect), (int) NSWidth(rect), (int) NSHeight(rect));
 //    [[[appDelegate analysisWindowController] myCurrentView] lockFocus];
 //    self.drawXGridFlag = YES;
 //     [delegate setDrawYGridFlag:YES];
 //[[[appDelegate analysisWindowController] graphViewController] setGRect:rect];
 
	
	if (![appDelegate drawChart]){	//Drawing The Oscilloscope Display
 NSLog(@"Executing Scope drawRect from  Oscilloscope View");
 [[NSColor blackColor] set];
 [NSBezierPath fillRect: rect];
 [self drawXGrid:rect];
 [self drawYGrid:rect];
 //     [NSGraphicsContext restoreGraphicsState];
 
 //            if ([[[appDelegate analysisWindowController] oscilloscopeController] doInitAndScale] != NO) {
 NSLog(@"does init and Scale");
 //            if(![[appDelegate analysisWindowController] scopeInited])
 //                [[[appDelegate analysisWindowController] oscilloscopeController] initScopeSweep];
 [[[appDelegate analysisWindowController] oscilloscopeController] scaleScopeSweep];
 [self drawScopeSweep];   //This woeks when set to self
 // }
 if ([appDelegate drawChart]){	//Drawing the Chart Display
 NSLog(@"Executing Chart drawRect from  Oscilloscope View");
 
 [[NSColor whiteColor] set];
 [NSBezierPath fillRect: rect];
 
 [self drawXGrid:rect];
 [self drawYGrid:rect];
 
 if ([[[appDelegate analysisWindowController] oscilloscopeController] doInitAndScale] != NO) {
 NSLog(@"does init and Scale");
 //                if(![[appDelegate analysisWindowController] scopeInited])
 //                   [[[appDelegate analysisWindowController] oscilloscopeController] initScopeSweep];
 [[[appDelegate analysisWindowController] oscilloscopeController] scaleScopeSweep];
 [self drawScopeSweep ];
 }
 [self drawScopeSweep];
 }
 
 //    [[[appDelegate analysisWindowController] myCurrentView] unlockFocus];
 //    NSLog(@"Finished drawRect in oscilloscopeView");
 }
 }
 */

/*
 - (void) drawScopeSweep             //: (NSRect)rect
 {
 //	doInitAndScale = NO;
 long minsamp, maxsamp;
 
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
 [[[appDelegate analysisWindowController] oscilloscopeController]setDrawSpikes: YES];
 [[[appDelegate analysisWindowController] oscilloscopeController]setDrawNoiseLevel: YES];
 long col, icol;
 float TraceMax = 0, TraceMin = 5000;
 //float DistMax = 0, DistMin = 5000;
 for (int i = 0; i <= [[[TraceSelector alloc] init] numTrace]; i++) {
 
 
 NSLog(@"Now drawing scope sweeps");
 //NSBezierPath* distPath = [NSBezierPath bezierPath];
 //NSBezierPath* proxPath = [NSBezierPath bezierPath];
 NSBezierPath* tracePath = [NSBezierPath bezierPath];
 [tracePath setLineWidth:2.0];
 
 // [distPath setLineWidth:2.0];
 // [proxPath setLineWidth:2.0];
 [tracePath moveToPoint:NSMakePoint(1, [[[[[appDelegate analysisWindowController] oscilloscopeController] traceOffset] objectAtIndex:i] floatValue])]; //get the offset at index i and move to point
 //  [distPath moveToPoint:NSMakePoint(1, [[[appDelegate analysisWindowController] oscilloscopeController] distOffset])];
 //[proxPath moveToPoint:NSMakePoint(1, [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset])];
 
 //	int i = 0;
 //	int j;
 
 if ([[[appDelegate analysisWindowController] oscilloscopeController] samplingOffset] > 0) {                 //TimeBase
 [[[appDelegate analysisWindowController] oscilloscopeController] setSamplingOffset:[[[appDelegate analysisWindowController] oscilloscopeController] samplingOffset]];	}
 
 if ([[[[appDelegate analysisWindowController] oscilloscopeController] traceOffset] objectAtIndex:i] > 0) {             //ProxOffset
 [[[appDelegate analysisWindowController] oscilloscopeController] setTraceOffset:[[[[appDelegate analysisWindowController] oscilloscopeController] traceOffset] objectAtIndex:i]];	}
 // if ([[[appDelegate analysisWindowController] oscilloscopeController] proxOffset] > 0) {             //ProxOffset
 // [[[appDelegate analysisWindowController] oscilloscopeController] setProxOffset:[[[appDelegate analysisWindowController] oscilloscopeController] proxOffset]];	}
 //   if ([[[appDelegate analysisWindowController] oscilloscopeController] distOffset] > 0) {             //DistOffset
 // [[[appDelegate analysisWindowController] oscilloscopeController] setDistOffset:[[[appDelegate analysisWindowController] oscilloscopeController] distOffset]];	}
 if ([[[[appDelegate analysisWindowController] oscilloscopeController] traceGain]  objectAtIndex:i]> 0) {               //ProxGain
 [[[appDelegate analysisWindowController] oscilloscopeController] setTraceGain:[[[[appDelegate analysisWindowController] oscilloscopeController] traceGain] objectAtIndex:i]];	}
 
 
 //    if ([[[appDelegate analysisWindowController] oscilloscopeController] proxGain] > 0) {               //ProxGain
 //  [[[appDelegate analysisWindowController] oscilloscopeController] setProxGain:[[[appDelegate analysisWindowController] oscilloscopeController] proxGain]];	}
 
 // if ([[[appDelegate analysisWindowController] oscilloscopeController] distOffset] > 0) {               //DistGain
 //  [[[appDelegate analysisWindowController] oscilloscopeController] setDistGain:[[[appDelegate analysisWindowController] oscilloscopeController] distGain]];	}
 
 
 if ([[[appDelegate analysisWindowController] oscilloscopeController]drawSpikes] == YES) {
 
 //   NSLog(@"proxGain is %f distGain is %f", [[[appDelegate analysisWindowController] oscilloscopeController] proxGain], [[[appDelegate analysisWindowController] oscilloscopeController] distGain]);
 // NSLog(@"proxOffset is %f distOffset is %f", [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset], [[[appDelegate analysisWindowController] oscilloscopeController] distOffset]);
 // NSLog(@"numColumns %ld samplingMaximum %ld", [[[appDelegate analysisWindowController] oscilloscopeController]numColumns],[[[appDelegate analysisWindowController] oscilloscopeController]samplingMaximum]);
 
 
 
 for (col = 1; col <= [[[appDelegate analysisWindowController] oscilloscopeController]numColumns]; col++) {     //Scale the Proximal Waveform
 TraceMax = -50000.;
 TraceMin = 50000.;
 minsamp = (col) * [[[appDelegate analysisWindowController] oscilloscopeController] sampsPerCol] + [[[appDelegate analysisWindowController] oscilloscopeController] samplingOffset];
 maxsamp = minsamp + (int)[[[appDelegate analysisWindowController] oscilloscopeController] sampsPerCol];
 if (maxsamp < [[[appDelegate analysisWindowController] oscilloscopeController]samplingMaximum]) {
 
 for (icol = minsamp; icol <= maxsamp; icol++) {
 TraceMax = [[[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i] objectAtIndex:icol] floatValue];
 if ([[[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i] objectAtIndex:icol + 1] floatValue] >= TraceMax) {
 TraceMax = [[[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i ] objectAtIndex:icol + 1] floatValue];
 
 }
 }
 
 for (icol = minsamp; icol <= maxsamp; icol++) {
 TraceMin = [[[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i] objectAtIndex:icol] floatValue];
 if ([[[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:i ] objectAtIndex:icol + 1] floatValue] <= TraceMin) {
 TraceMin = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:icol + 1] floatValue];
 }
 }
 
 [tracePath lineToPoint:NSMakePoint(col, TraceMin * [[[[[appDelegate analysisWindowController] oscilloscopeController] traceGain] objectAtIndex:i] floatValue] + [[[[[appDelegate analysisWindowController] oscilloscopeController] traceOffset] objectAtIndex:i] floatValue])];
 [tracePath lineToPoint:NSMakePoint(col, TraceMax * [[[[[appDelegate analysisWindowController] oscilloscopeController] traceGain] objectAtIndex:i] floatValue] + [[[[[appDelegate analysisWindowController] oscilloscopeController] traceOffset] objectAtIndex:i] floatValue])];
 //     [tracePath lineToPoint:NSMakePoint(col, TraceMax * [[[appDelegate analysisWindowController] oscilloscopeController] traceGain] + [[[appDelegate analysisWindowController] oscilloscopeController] traceOffset])];
 
 
 }
 }
 }       //End of draw spikes
 }  //end of for loop
 }
 
 */

/*
 - (void) drawScopeSweep             //: (NSRect)rect
 {
 //	doInitAndScale = NO;
	long minsamp, maxsamp;
 // [NSGraphicsContext saveGraphicsState];
 
 AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
 [[[appDelegate analysisWindowController] oscilloscopeController]setDrawSpikes: YES];
 [[[appDelegate analysisWindowController] oscilloscopeController]setDrawNoiseLevel: NO];
 [[NSColor blueColor] setFill];
 [[[appDelegate analysisWindowController] oscilloscopeController] setNumColumns: 754];
 
	long col, icol;
	float ProxMax = 0, ProxMin = 5000;
	float DistMax = 0, DistMin = 5000;
	
	NSLog(@"Now drawing scope sweeps");
 NSBezierPath* distPath = [NSBezierPath bezierPath];
	NSBezierPath* proxPath = [NSBezierPath bezierPath];
	[distPath setLineWidth:2.0];
	[proxPath setLineWidth:2.0];
	[distPath moveToPoint:NSMakePoint(1, [[[appDelegate analysisWindowController] oscilloscopeController] distOffset])];
	[proxPath moveToPoint:NSMakePoint(1, [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset])];
 
 //	int i = 0;
 //	int j;
 
	if ([[[appDelegate analysisWindowController] oscilloscopeController] samplingOffset] > 0) {                 //TimeBase
 [[[appDelegate analysisWindowController] oscilloscopeController] setSamplingOffset:[[[appDelegate analysisWindowController] oscilloscopeController] samplingOffset]];	}
	if ([[[appDelegate analysisWindowController] oscilloscopeController] proxOffset] > 0) {             //ProxOffset
 [[[appDelegate analysisWindowController] oscilloscopeController] setProxOffset:[[[appDelegate analysisWindowController] oscilloscopeController] proxOffset]];	}
	if ([[[appDelegate analysisWindowController] oscilloscopeController] distOffset] > 0) {             //DistOffset
 [[[appDelegate analysisWindowController] oscilloscopeController] setDistOffset:[[[appDelegate analysisWindowController] oscilloscopeController] distOffset]];	}
	if ([[[appDelegate analysisWindowController] oscilloscopeController] proxGain] > 0) {               //ProxGain
 [[[appDelegate analysisWindowController] oscilloscopeController] setProxGain:[[[appDelegate analysisWindowController] oscilloscopeController] proxGain]];	}
	if ([[[appDelegate analysisWindowController] oscilloscopeController] distOffset] > 0) {               //DistGain
 [[[appDelegate analysisWindowController] oscilloscopeController] setDistGain:[[[appDelegate analysisWindowController] oscilloscopeController] distGain]];	}
 
	
	//if ([[[appDelegate analysisWindowController] oscilloscopeController] drawSpikes] == YES) {
 NSLog(@"now drawing spike");
 
 NSLog(@"proxGain is %f distGain is %f", [[[appDelegate analysisWindowController] oscilloscopeController] proxGain], [[[appDelegate analysisWindowController] oscilloscopeController] distGain]);
 NSLog(@"proxOffset is %f distOffset is %f", [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset], [[[appDelegate analysisWindowController] oscilloscopeController] distOffset]);
 NSLog(@"numColumns %ld samplingMaximum %ld", [[[appDelegate analysisWindowController] oscilloscopeController] numColumns],[[[appDelegate analysisWindowController] oscilloscopeController] samplingMaximum]);
 [[NSColor redColor] set];
 
 for (col = 1; col <= [[[appDelegate analysisWindowController] oscilloscopeController]numColumns]; col++) {     //Scale the Proximal Waveform
 ProxMax = -50000.;
 ProxMin = 50000.;
 minsamp = (col) * [[[appDelegate analysisWindowController] oscilloscopeController] sampsPerCol] + [[[appDelegate analysisWindowController] oscilloscopeController] samplingOffset];
 maxsamp = minsamp + (int)[[[appDelegate analysisWindowController] oscilloscopeController] sampsPerCol];
 if (maxsamp < [[[appDelegate analysisWindowController] oscilloscopeController]samplingMaximum]) {
 
 for (icol = minsamp; icol <= maxsamp; icol++) {
 ProxMax = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:icol] floatValue];
 if ([[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:icol + 1] floatValue] >= ProxMax) {
 ProxMax = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:icol + 1] floatValue];
 
 }
 }
 
 for (icol = minsamp; icol <= maxsamp; icol++) {
 ProxMin = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:icol] floatValue];
 if ([[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:icol + 1] floatValue] <= ProxMin) {
 ProxMin = [[[[appDelegate traceWaveforms] ipbuf] objectAtIndex:icol + 1] floatValue];
 }
 }
 
 //  [proxPath lineToPoint:NSMakePoint(col, ProxMin * [[[appDelegate analysisWindowController] oscilloscopeController] proxGain] + [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset])];
 [proxPath lineToPoint:NSMakePoint(col, ProxMin + [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset])];
 //if not move to pts as well
 [proxPath stroke];
 [proxPath lineToPoint:NSMakePoint(col, ProxMax + [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset])];
 [proxPath stroke];
 }
 }
 //	}       //End of draw spikes
	
 
 
 //if ([[[appDelegate analysisWindowController] oscilloscopeController]drawNoiseLevel] == YES) {
 
 //    NSLog(@"Now drawing Noise Level");
	//	NSBezierPath* distNoise = [NSBezierPath bezierPath];
	//	NSBezierPath* proxNoise = [NSBezierPath bezierPath];
 //   NSPoint proxPoint, distPoint;
	//	[distNoise setLineWidth:2.0];
	//	[proxNoise setLineWidth:2.0];
 //		[proxNoise moveToPoint:NSMakePoint(0, [[appDelegate signalProcessing] proxTest] * [[[appDelegate analysisWindowController] oscilloscopeController] proxGain] + [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset])];
 //		[distNoise moveToPoint:NSMakePoint(0, [[appDelegate signalProcessing] distTest]// * [[[appDelegate analysisWindowController] oscilloscopeController] distGain] + [[[appDelegate analysisWindowController] oscilloscopeController] distOffset])];
 //		[[NSColor redColor] setStroke];
 
	//	for (col = 1; col <= [[[appDelegate analysisWindowController] oscilloscopeController]numColumns]; col++) {
 //       proxPoint = NSMakePoint(col, [[appDelegate signalProcessing] proxTest] * [[[appDelegate analysisWindowController] oscilloscopeController] proxGain] + [[[appDelegate analysisWindowController] oscilloscopeController] proxOffset]);
 //       distPoint = NSMakePoint(col, [[appDelegate signalProcessing] distTest] * [[[appDelegate analysisWindowController] oscilloscopeController] distGain] + [[[appDelegate analysisWindowController] oscilloscopeController] distOffset]);
 //	[distNoise lineToPoint:distPoint];
 //	[proxNoise lineToPoint:proxPoint];
 
 //}
 
	//	[distNoise stroke];
	//	[proxNoise stroke];
	//	[distNoise closePath];
	//	[proxNoise closePath];
 //  }
 //DrawTraces
 
 if (![appDelegate drawChart])
 [[NSColor redColor] setStroke];
 if ([appDelegate drawChart])
 [[NSColor blackColor] setStroke];
 
	[distPath stroke];
	[proxPath stroke];
	//[distPath closePath];
	//[proxPath closePath];
 NSLog(@"k>>?");
	
 }
 
 */
/*
 - (void)drawRect:(NSRect)dirtyRect
 {
 
 [super drawRect:dirtyRect];
 [NSGraphicsContext saveGraphicsState];
 [[NSColor blackColor] setFill];
 [[NSBezierPath bezierPathWithRoundedRect:[self bounds] xRadius:0.0 yRadius:0.0] fill];
 [NSGraphicsContext restoreGraphicsState];
 
 int width = dirtyRect.size.width;
 int height = dirtyRect.size.height;
 
 int numberOfSineCurves = 30;
 float cWidth = 20.0;
 float cHeight = height/3.5;
 for (int i=0; i<numberOfSineCurves; i++)
 {
 NSBezierPath *sinePath = [NSBezierPath bezierPath];
 [[NSColor redColor] set];
 [sinePath setLineWidth:1.0];
 [sinePath moveToPoint:NSMakePoint(i*5+10, height/2)];
 for (int j=0; j<width;j++)
 {
 [sinePath lineToPoint:CGPointMake((i*6+15)+j*cWidth+10, height/2 - (sin(2*3.14*j/20)*cHeight))];
 }
 [sinePath stroke];
 }
 }
 */


