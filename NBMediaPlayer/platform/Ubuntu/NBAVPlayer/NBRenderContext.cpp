//
// Created by liuenbao on 18-9-15.
//

#include <NBRendererInfo.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <stdio.h>

static XVisualInfo *chooseVisual(NBRendererTarget* target) {
    XWindowAttributes winAttr = {0};
    XGetWindowAttributes(target->display, target->window, &winAttr);

    int attribs [ 100 ] ;
    int n = 0 ;

    // Request 24-bit color with alpha
    attribs [n++] = GLX_RGBA ;
    attribs [n++] = GLX_RED_SIZE   ; attribs [n++] = 8 ;
    attribs [n++] = GLX_GREEN_SIZE ; attribs [n++] = 8 ;
    attribs [n++] = GLX_BLUE_SIZE  ; attribs [n++] = 8 ;

    // Request 24-bit depth buffer
    attribs [n++] = GLX_DEPTH_SIZE ; attribs [n++] = 24 ;

    //    // Request 4 multisamples per pixel
    //    attribs [n++] = GLX_SAMPLE_BUFFERS ; attribs [n++] = 1 ;
    //    attribs [n++] = GLX_SAMPLES        ; attribs [n++] = NUM_SAMPLES ;

    // Request double-buffering
    attribs [n++] = GLX_DOUBLEBUFFER ;
    attribs [n++] = None ;

    return glXChooseVisual(target->display, target->screen, attribs);
}

void* getRendererCtx(NBRendererTarget* target) {
    return NULL;
}

nb_status_t prepareRendererCtx(NBRendererTarget* info) {

    XVisualInfo* visinfo = chooseVisual(info);
    if (visinfo == NULL) {
        return UNKNOWN_ERROR;
    }

    // Create an OpenGL context and attach it to our X window
    GLXContext context = glXCreateContext(info->display, visinfo, NULL, 1 ) ;

    if ( ! glXMakeCurrent(info->display, info->window, context ) )
        printf( "glXMakeCurrent failed.\n" );

    if ( ! glXIsDirect (info->display, glXGetCurrentContext() ) )
        printf( "Indirect GLX rendering context obtained\n" );

//    if ( ! glXMakeCurrent(info->display, info->window, context ) )
//        printf( "glXMakeCurrent failed.\n" );

    return OK;
}

void destroyRendererCtx(NBRendererTarget* info) {
    if (info == NULL) {
        return ;
    }

    GLXContext context = glXGetCurrentContext();

    glXMakeCurrent(info->display, None, NULL);

    glXDestroyContext(info->display, context);
}

nb_status_t preRender(NBRendererTarget* target, NBRenderInfo* info) {
    XWindowAttributes winAttr = {0};
    XGetWindowAttributes(target->display, target->window, &winAttr);

    info->x = winAttr.x;
    info->y = winAttr.y;
    info->width = winAttr.width;
    info->height = winAttr.height;

    return OK;
}

nb_status_t postRender(NBRendererTarget* target) {
    // Display it
    glXSwapBuffers(target->display, target->window);
    return OK;
}