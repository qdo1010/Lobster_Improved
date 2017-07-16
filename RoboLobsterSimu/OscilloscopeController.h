#import <Cocoa/Cocoa.h>
//#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
    //#import <QuickTime/QuickTime.h>
    //#import <QTKit/QTKit.h>
#import <CoreAudio/CoreAudio.h>
#import <AudioUnit/AudioUnit.h>

@class Waveforms;
@class OscilloscopeView;
@class AppDelegate;


@interface OscilloscopeController : NSViewController {
}

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



@property (weak) IBOutlet NSButton *startButton;


@property (weak) IBOutlet NSSlider *sweepDurationSlider;
@property (weak) IBOutlet NSTextField *sweepDurationTextBox;
@property (weak) IBOutlet NSSlider *sweepOffsetSlider;
@property (weak) IBOutlet NSTextField *sweepOffsetTextBox;

- (IBAction)changeSweepDuration:(id)sender;
- (IBAction)changeSweepOffset:(id)sender;
//- (IBAction)displaytextString:(id)sender;

- (IBAction)changeTraceGain:(id)sender;

- (IBAction)changeTraceOffset:(id)sender;
@property (weak) IBOutlet NSTextField *traceGainTextBox;
@property (weak) IBOutlet NSTextField *traceOffsetTextBox;
@property (weak) IBOutlet NSSlider *traceOffsetSlider;
@property (weak) IBOutlet NSSlider *traceGainSlider;
- (IBAction)changeView:(id)sender;


- (void)beginSendingStuffToBeDraw;

extern long elapsed;
@property (readwrite) double index;
@property (readwrite)  int traceIDchosen;       //Duration of file in Sec


@property (readwrite,retain) NSMutableArray* AllTracesInfo;    //This is an array of traces
@property (readwrite,retain) NSMutableArray* IndividualTraceInfo;    //This is an array of traces



//@property (readwrite, getter=isDoInitAndScale)      BOOL doInitAndScale;
@property (readwrite)          BOOL assetInited;
@property (nonatomic, readwrite)          BOOL doInitAndScale;
@property (readwrite)          BOOL drawSpikes;
@property (readwrite)          BOOL drawClusteredSpikes;
@property (readwrite)          BOOL drawNoiseLevel;
@property (readwrite)          BOOL drawCluster;

@property (readwrite) double maxIp;
@property (readwrite) double minIp;
@property (readwrite) double timeScale;
//GraphicsViewParameters


- (void) importAudioFromFile:(id)sender;
- (void) initScopeSweep;
- (void) scaleScopeSweep;
    //- (void) drawScopeSweep;//: (NSRect)rect;
- (BOOL) doInitAndScale;
    //- (void) setOscilloscopeView:(OscilloscopeView *)view;
- (void) setDoInitAndScale:(BOOL)flag;

@property (weak) IBOutlet NSPopUpButton *displayTraceID;
- (IBAction)chooseTraceID:(id)sender;
- (IBAction)changeGainStep:(id)sender;
@property (weak) IBOutlet NSStepper *gainStepView;

- (IBAction) resetSweepDuration: (id) sender;
- (IBAction) resetSweepOffset: (id) sender;
- (IBAction) resetGainProx: (id) sender;
- (IBAction) resetOffsetProx: (id) sender;
- (IBAction) invertProxGraph: (id)sender;
@end
