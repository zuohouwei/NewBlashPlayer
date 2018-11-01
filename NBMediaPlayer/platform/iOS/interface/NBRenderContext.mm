//
//  NBRenderContext.c
//  NBMediaPlayer
//
//  Created by liu enbao on 24/10/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NBRenderContext.h"

void* getRendererCtx(NBRendererTarget* target) {
    return [(__bridge NSObject<NBRenderProtocol>*)target->params getRendererCtx];
}

nb_status_t prepareRendererCtx(NBRendererTarget* target, bool dummy) {
    return [(__bridge NSObject<NBRenderProtocol>*)target->params prepareRendererCtx];
}

void destroyRendererCtx(NBRendererTarget* target) {
    return [(__bridge NSObject<NBRenderProtocol>*)target->params destroyRendererCtx];
}

nb_status_t preRender(NBRendererTarget* target, NBRenderInfo* info) {
    return [(__bridge NSObject<NBRenderProtocol>*)target->params preRender:info];
}

nb_status_t postRender(NBRendererTarget* target) {
    return [(__bridge NSObject<NBRenderProtocol>*)target->params postRender];
}
