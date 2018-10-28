//
//  NBGLView.h
//  iOSNBMediaPlayer
//
//  Created by liu enbao on 17/09/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#import <UIKit/UIView.h>

typedef NS_ENUM(NSInteger, NBVideoGravity) {
    NBVideoGravityUnknow = -1,
    NBVideoGravityResizeAspect,
    NBVideoGravityResizeAspectFill,
    NBVideoGravityResize,
};

@class NBAVPlayer;

@interface NBGenericGLView : UIView

@property (nonatomic, retain) NBAVPlayer* player;

@property (nonatomic, assign) NBVideoGravity videoGravity;

@end
