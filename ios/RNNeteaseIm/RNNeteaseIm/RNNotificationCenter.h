//
//  RNNotificationCenter.h
//  RNNeteaseIm
//
//  Created by Dowin on 2017/5/24.
//  Copyright © 2017年 Dowin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RCTEventEmitter.h"
#import "RCTBridgeModule.h"

@interface RNNotificationCenter :  NSObject<RCTBridgeModule>
+ (instancetype)sharedCenter;
- (void)start;
@end
