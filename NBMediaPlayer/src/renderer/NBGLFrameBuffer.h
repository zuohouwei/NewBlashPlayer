//
//  NBFrameBuffer.h
//  NBMediaPlayer
//
//  Created by liu enbao on 28/10/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#ifndef NBFRAMEBUFFER_H
#define NBFRAMEBUFFER_H

#ifdef BUILD_TARGET_LINUX64
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#elif BUILD_TARGET_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif BUILD_TARGET_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

class NBGLFrameBuffer {
public:
    NBGLFrameBuffer();
    ~NBGLFrameBuffer();
    
    void setupFBO(int fboWidth, int fboHeight, int stage);
    void unsetupFBO();
    
    void bind();
    void unbind();
    
    GLuint getNativeTextureId();
    
private:
    // FBO variables
    GLuint mFboHandle;
    GLuint mDepthBuffer;
    GLuint mRenderBuffer;
    GLuint mFboTexture;
    int mFboWidth;
    int mFboHeight;
    
    GLint mDefaultFBO;
    GLint mDefaultRenderBuffer;
    
    int mStage;
};

#endif /* NBFRAMEBUFFER_H */
