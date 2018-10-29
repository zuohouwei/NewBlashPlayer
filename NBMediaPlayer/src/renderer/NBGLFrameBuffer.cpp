//
//  NBFrameBuffer.cpp
//  NBMediaPlayer
//
//  Created by liu enbao on 28/10/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#include "NBGLFrameBuffer.h"
#include "NBGLShaderUtils.h"
#include <NBLog.h>

#define LOG_TAG "NBGLFrameBuffer"

NBGLFrameBuffer::NBGLFrameBuffer()
    :mFboTexture(-1)
    ,mStage(0) {
}

NBGLFrameBuffer::~NBGLFrameBuffer() {
    
}

void NBGLFrameBuffer::setupFBO(int fboWidth, int fboHeight, int stage) {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mDefaultFBO);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &mDefaultRenderBuffer);
    OPENGLES2_CHECK();
    
    mFboWidth = fboWidth;
    mFboHeight = fboHeight;
    
    glGenFramebuffers(1, &mFboHandle);
    OPENGLES2_CHECK();
    
    glGenRenderbuffers(1, &mRenderBuffer);
    OPENGLES2_CHECK();
    
    mStage = stage;
    
    // use the created glcontext
    bind();
    
    // Create The RenderTexture
    NBLOG_DEBUG(LOG_TAG, "setupFBO fboWidth : %d fboHeight : %d", mFboWidth, mFboHeight);
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, mFboWidth, mFboHeight);
    OPENGLES2_CHECK();
    
    NBLOG_DEBUG(LOG_TAG, "setupFBO mRenderBuffer : %d", mRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mRenderBuffer);
    OPENGLES2_CHECK();
    
    glGenTextures(1, &mFboTexture);
    OPENGLES2_CHECK();
    
    glActiveTexture(GL_TEXTURE0+stage);
    OPENGLES2_CHECK();
    
    glBindTexture(GL_TEXTURE_2D, mFboTexture);
    OPENGLES2_CHECK();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    OPENGLES2_CHECK();
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mFboWidth, mFboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    OPENGLES2_CHECK();
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFboTexture, 0);
    OPENGLES2_CHECK();
    
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    OPENGLES2_CHECK();
    
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            NBLOG_ERROR(LOG_TAG, "fbo complete");
            break;
            
        case GL_FRAMEBUFFER_UNSUPPORTED:
            NBLOG_ERROR(LOG_TAG, "fbo unsupported");
            break;
        default:
            /* programming error; will fail on all hardware */
            NBLOG_ERROR(LOG_TAG, "Framebuffer Error : %x", status);
            break;
    }
    
    // restore the opengl es context
    unbind();
}

void NBGLFrameBuffer::unsetupFBO() {
    glDeleteTextures(1, &mFboTexture);
    OPENGLES2_CHECK();
    
    glDeleteFramebuffers(1, &mFboHandle);
    OPENGLES2_CHECK();
    
    glDeleteRenderbuffers(1, &mRenderBuffer);
    OPENGLES2_CHECK();
}

void NBGLFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, mFboHandle);
    OPENGLES2_CHECK();
    
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
    OPENGLES2_CHECK();
    
    glViewport(0,0, mFboWidth, mFboHeight);
    OPENGLES2_CHECK();
    
    glActiveTexture(GL_TEXTURE0+mStage);
    OPENGLES2_CHECK();
}

void NBGLFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFBO);
    OPENGLES2_CHECK();
    
    glBindRenderbuffer(GL_RENDERBUFFER, mDefaultRenderBuffer);
    OPENGLES2_CHECK();
}

GLuint NBGLFrameBuffer::getNativeTextureId() {
    return mFboTexture;
}
