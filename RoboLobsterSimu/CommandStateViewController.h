//  CommandStateViewController.h
//  Roboplasm
//
//  Created by Brian King on 4/5/13.
//  Copyright (c) 2013 Northeastern Univeristy. All rights reserved.
//
#import <Foundation/Foundation.h>

#import <Cocoa/Cocoa.h>
@class CommandStateViewController;

//THIS IS WHERE I DECLARE THE PROTOCOL
@protocol CommandStateViewControllerDelegate<NSObject>
- (void)commandStateVC:(CommandStateViewController *)commandStateVC didUpdateParameters:(NSMutableDictionary *)parameters;
@end

@interface CommandStateViewController :NSWindowController

//- (id) delegate;
@property (nonatomic, assign) id<CommandStateViewControllerDelegate> delegate;

//@property (nonatomic, strong) Table *currentTable;
@property (nonatomic, strong) NSMutableDictionary *currentParameters;

//@property (nonatomic, strong) IBOutlet NSTextField *spdBox;

@property (strong) IBOutlet NSPopUpButton *SpeedTag;

- (IBAction)chooseSpeed:(id)sender;


//- (IBAction)setSpeed:(id)sender;

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
#define loaded          4

/*Yaw*/
#define hardLeft        1
#define easyLeft        2
#define mystraight      3
#define easyRight       4
#define hardRight       5


/*ChPitch*/
#define ChUp            1
#define cLevel          2
#define ChDn            3
#define ChLow           4


/*AntYaw*/
#define AtPro         1
#define AtNor         2
#define AtLat         3
#define AtRet         4


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

