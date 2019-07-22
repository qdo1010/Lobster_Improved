#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreAudio/CoreAudio.h>
#import <AudioUnit/AudioUnit.h>
//#import "CommandStateViewController.h"
@class Waveforms;
@class OscilloscopeView;
@class AppDelegate;

@interface OscilloscopeController: NSViewController

@property (readwrite) double proxGain;
@property (readwrite) double proxOffset;
@property (readwrite) double proxGainMax;

@property (readwrite)  long   samplingOffset;     //Sweep offset from origin in samples
@property (readwrite)  long   samplingTerminus;   //Sweep Terminus from origin in samples
@property (readwrite)  long   samplingMaximum;    //Duration of file in samples
@property (readwrite)  long   samplingDuration;   //Duration of Sweep in samples

@property (readwrite, nonatomic)  double samplingRate;       //SamplingRate in Hz
@property (readwrite, nonatomic)  long   numColumns;          //Number of columns in the display
@property (readwrite, nonatomic)  double sampsPerCol;        //Samples per Pixel

@property (readwrite)  double sweepOffset;        //Sweep offset from origin in Sec
@property (readwrite)  double sweepTerminus;      //Terminus of Sweep in Sec
@property (readwrite)  double sweepDuration;      //Duration of Sweep in Sec
@property (readwrite)  double sweepMaximum;       //Duration of file in Sec


- (IBAction)setlFor:(id)sender;
- (IBAction)setrFor:(id)sender;
- (IBAction)setlBack:(id)sender;
- (IBAction)setrBack:(id)sender;
- (IBAction)setlLead:(id)sender;
- (IBAction)setrLead:(id)sender;
- (IBAction)setlTrail:(id)sender;
- (IBAction)setrTrail:(id)sender;

    // These are not used:

    //@property (nonatomic, strong) IBOutlet NSTextField *rAntYBox;
    //@property (nonatomic, strong) IBOutlet NSTextField *ltChYBox;
    //@property (nonatomic, strong) IBOutlet NSTextField *rtChYBox;
    //- (IBAction)setLAntYaw:(id)sender;
    //- (IBAction)setLClawYaw:(id)sender;
    //- (IBAction)setRAntYaw:(id)sender;
    //- (IBAction)setRClawYaw:(id)sender;


/*
 @property (readwrite) int *cellID;
@property (readwrite) int *side;
@property (readwrite) int *seg;
*/

extern long elapsed;
@property (readwrite) double index;
@property (readwrite)  int traceIDchosen;      //Duration of file in Sec

/* Moved to Parameter Setter
@@property (readwrite) int *side;
@property (readwrite) int *seg;
@property (readwrite)  double *alpha;
@property (readwrite)  double *sigma;
@property (readwrite)  double *sigmaE;
@property (readwrite)  double *sigmaI;
@property (readwrite)  double *betaE;
@property (readwrite)  double *betaI;
@property (readwrite)  double *Idc;
 */


//@property (readwrite) int settingUpParams;


@property (readwrite,retain) NSMutableArray* AllTracesInfo;    //This is an array of traces
@property (readwrite,retain) NSMutableArray* IndividualTraceInfo;    //This is an array of traces


//@property (readwrite, getter=isDoInitAndScale)      BOOL doInitAndScale;
@property (readwrite)          BOOL assetInited;
@property (nonatomic, readwrite)          BOOL doInitAndScale;

@property (weak) IBOutlet NSSlider *sweepDurationSlider;

@property (weak) IBOutlet NSTextField *sweepDurationTextBox;

@property (weak) IBOutlet NSSlider *sweepOffsetSlider;

@property (weak) IBOutlet NSTextField *sweepOffsetTextBox;

@property (weak) IBOutlet NSTextField *traceGainTextBox;

@property (weak) IBOutlet NSTextField *traceOffsetTextBox;



@property (weak) IBOutlet NSSlider *traceOffsetSlider;

@property (weak) IBOutlet NSSlider *traceGainSlider;

/* Moved to Parameter Setter
@property (weak) IBOutlet NSTextField *cellNameTextBox; //cell Name
@property (weak) IBOutlet NSTextField *alphaTextBox;
@property (weak) IBOutlet NSTextField *sigmaTextBox;
@property (weak) IBOutlet NSTextField *sigmaETextBox;
@property (weak) IBOutlet NSTextField *sigmaITextBox;
@property (weak) IBOutlet NSTextField *betaETextBox;
@property (weak) IBOutlet NSTextField *betaITextBox;
@property (weak) IBOutlet NSTextField *IdcTextBox;
 */

@property (weak) IBOutlet NSPopUpButton *displayTraceID;

- (IBAction)changeSweepDuration:(id)sender;

- (IBAction)changeSweepOffset:(id)sender;

- (IBAction)changeTraceGain:(id)sender;

- (IBAction)changeTraceOffset:(id)sender;

- (IBAction)changeView:(id)sender;
/*
- (IBAction)setParams:(id)sender;
- (IBAction)loadParams:(id)sender;
- (IBAction)saveParams:(id)sender;
*/

- (IBAction)loadParamsWithName:(id)sender :(NSString*)filename :(int)speed;

- (void)beginSendingStuffToBeDrawn;

//GraphicsViewParameters
- (void) updateSweepValue;

- (void) initScopeSweep;

- (void) scaleScopeSweep;
    //- (void) drawScopeSweep;//: (NSRect)rect;
- (BOOL) doInitAndScale;
    //- (void) setOscilloscopeView:(OscilloscopeView *)view;
- (void) setDoInitAndScale:(BOOL)flag;

- (IBAction)chooseTraceID:(id)sender;

- (IBAction) invertProxGraph: (id)sender;

- (IBAction) checkControlCommand: (NSMutableDictionary*)params;

//Prototypes to remove warnings
void editParam(int *neuronName, unsigned long *side, unsigned long *seg, double *a, double *s, double *sE, double *sI, double *bE, double *bI, double *Idc,unsigned long int size);
//function that Objective C can call to write to file

@end


/*Roll*/
#define ltUp            1
#define rLevel         2
#define rtUp           3

/*Pitch*/
#define rosUp           1
#define pLevel          2
#define rosDn           3

/*Height*/
#define hHigh           1
#define hNor            2
#define hLow            3

/*speeds*/
#define mystop          5
#define slow            3
#define medium          2
#define high            1
#define loaded            4

/*Yaw*/
#define hardLeft        1
#define easyLeft        2
#define proceed         3
#define easyRight       4
#define hardRight       5


/*ChPitch*/
#define ChUp            1
#define cLevel          2
#define ChDn            3
#define ChLow           4

/*ltAntYaw*/
#define ltAtPro         1
#define ltAtNor         2
#define ltAtLat         3
#define ltAtRet         4

/*rtAntYaw*/
#define rtAtPro         1
#define rtAtNor         2
#define rtAtLat         3
#define rtAtRet         4


/*AbPitch*/
#define Ext             1
#define Ele             2
#define Norm            3
#define Dep             4
#define Flex            5


#define LeftLabel       4
#define lForCheck       5
#define lBackCheck      6
#define lLeadCheck      7
#define lTrailCheck     8

#define RightLabel      9
#define rForCheck       10
#define rBackCheck      11
#define rLeadCheck      12
#define rTrailCheck     13


#define lowIntensity    1
#define mediumIntensity  2
#define highIntensity   3

