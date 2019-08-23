//
//  ParameterClass.h
//  RoboLobsterSimu
//
//  Created by Spencer Brennessel on 8/23/19.
//  Copyright © 2019 HongQuan Do. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NeuronParameterClass : NSObject
{
    NSString *name;
    int type;
    int cell;
    int side;
    int seg;
    _Bool modified;
    double xp, xpp, mu, sigmaIn, betaIn;
    double sigmaDc, betaDc;
    double x, y, sigmaE, sigmaI, betaE, betaI, Idc;
    double alpha;
    double sigma;
    int spike;
    double yr, xr, x2, y2;
    double alphaInit;
}

@end


@interface SynapseParameterClass : NSObject
{
    NSString *name;
    int type;
    int cell;
    int side;
    int seg;
    double xRp;            //Reversal Potential
    double gamma;          //Time Constant
    double gStrength;
}

@end

NS_ASSUME_NONNULL_END
