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
    int numTr = [[propertyValue ipbuf] count] ;
    
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
    float cHeight = height/4.5;                     //Scale of Trace **************************
    NSBezierPath *sinePath = [NSBezierPath bezierPath];
    float gain = [[[(AppDelegate *)[[NSApplication sharedApplication] delegate] traceGainArray] objectAtIndex:idy] floatValue];
    float offset = [[[(AppDelegate *)[[NSApplication sharedApplication] delegate] traceOffsetArray] objectAtIndex:idy] floatValue];
    
    [sinePath setLineWidth:1.0];
    //        [sinePath setFlatness:3.0];
    [sinePath moveToPoint:NSMakePoint(0*cWidth,  height/2-[[points objectAtIndex:0] floatValue]*cHeight*gain + cHeight*([[propertyValue ipbuf] count] - idy -1) + offset)];
    
    NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:[NSFont fontWithName:@"Helvetica" size:16], NSFontAttributeName,[NSColor blueColor], NSForegroundColorAttributeName, nil];
    
    
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

