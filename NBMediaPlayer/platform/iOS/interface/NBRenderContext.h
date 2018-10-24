//
//  NBRenderContext.h
//  NBMediaPlayer
//
//  Created by liu enbao on 24/10/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#include <NBRendererInfo.h>

@protocol NBRenderProtocol

- (nb_status_t)prepareRendererCtx;

- (void)destroyRendererCtx;

- (nb_status_t)preRender:(NBRenderInfo*)info;

- (nb_status_t)postRender;

- (void*)getRendererCtx;

@end
