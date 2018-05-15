//
//  CommandStateViewController.h
//  Roboplasm
//
//  Created by Brian King on 4/5/13.
//  Copyright (c) 2013 Northeastern Univeristy. All rights reserved.
//

#import <Cocoa/Cocoa.h>
//#import "MovieControllerAnalysis.h"
//#import "Table.h"
//#import "sprites.h"

@protocol CommandStateViewControllerDelegate;

@interface CommandStateViewController :NSWindowController

@property (nonatomic, assign) id<CommandStateViewControllerDelegate> delegate;

//@property (nonatomic, strong) Table *currentTable;
@property (nonatomic, strong) NSMutableDictionary *currentParameters;

//@property (nonatomic, strong) IBOutlet SpriteView *spriteView;

@property (nonatomic, strong) IBOutlet NSTextField *pitchBox;
@property (nonatomic, strong) IBOutlet NSTextField *rollBox;
@property (nonatomic, strong) IBOutlet NSTextField *abdBox;
@property (nonatomic, strong) IBOutlet NSTextField *htBox;
@property (nonatomic, strong) IBOutlet NSTextField *ltAntYBox;
@property (nonatomic, strong) IBOutlet NSTextField *spdBox;
@property (nonatomic, strong) IBOutlet NSTextField *yawBox;
@property (nonatomic, strong) IBOutlet NSTextField *chPBox;
@property (nonatomic, strong) IBOutlet NSTextField *uroBox;


- (IBAction)setAbPitch:(id)sender;
- (IBAction)setChPitch:(id)sender;
- (IBAction)setHeight:(id)sender;
- (IBAction)setRoll:(id)sender;
- (IBAction)setSpeed:(id)sender;
- (IBAction)setUropods:(id)sender;
- (IBAction)setPitch:(id)sender;
- (IBAction)setYaw:(id)sender;
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

@protocol CommandStateViewControllerDelegate <NSObject>

- (void)commandStateVC:(CommandStateViewController *)commandStateVC didUpdateParameters:(NSMutableDictionary *)parameters;

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
#define mystraight      3
#define easyRight       4
#define hardRight       5


/*ChPitch*/
#define ChUp            1
#define cLevel          2
#define ChDn            3
#define ChLow           4

/*ltChYaw*/
#define ltChExt         1
#define ltChNor         2
#define ltMerSp         3
#define ltChLat         4

/*rtChYaw*/
#define rtChExt         1
#define rtChNor         2
#define rtMerSp         3
#define rtChLat         4

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

/*uro*/
#define uroFlare        1
#define uroNorm         2
#define uroAdd          3

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

