//
//  ViewController.m
//  RoboLobsterSimu
//
//  Created by HongQuan Do on 6/14/17.
//  Copyright © 2017 HongQuan Do. All rights reserved.
//

#import "ViewController.h"
#import "Waveforms.h"
#import "AppDelegate.h"
@implementation ViewController

//void xmain();

- (void)viewDidLoad {
    [super viewDidLoad];
    NSLog(@"View did load");
    
}
- (void) viewDidAppear{
    //[(AppDelegate *)[[NSApplication sharedApplication] delegate] awakeFromNib];
  //  AppDelegate *appDelegate = [[NSApplication sharedApplication] delegate];
 //   [[appDelegate analysisWindowController] setOscilloscopeView:self];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


- (IBAction)printData:(id)sender {
    //id propertyValue = [(AppDelegate *)[[NSApplication sharedApplication] delegate] traceWaveforms];
    //[propertyValue sendToBuffer:cellDepressorL2 :100];
    //NSLog(@"%@",[propertyValue ipbuf]);

    
}
@end
