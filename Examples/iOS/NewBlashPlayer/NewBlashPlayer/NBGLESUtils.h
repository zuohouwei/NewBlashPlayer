//
//  NBGLESUtils.h
//  NewBlashPlayer
//
//  Created by liu enbao on 28/10/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <OpenGLES/ES2/gl.h>

@interface NBGLESUtils : NSObject

// Create a shader object, load the shader source string, and compile the shader.
//
+(GLuint)loadShader:(GLenum)type withString:(NSString *)shaderString;

+(GLuint)loadShader:(GLenum)type withFilepath:(NSString *)shaderFilepath;

//
///
/// Load a vertex and fragment shader, create a program object, link program.
/// Errors output to log.
/// vertexShaderFilepath Vertex shader source file path.
/// fragmentShaderFilepath Fragment shader source file path
/// return A new program object linked with the vertex/fragment shader pair, 0 on failure
//
+(GLuint)loadProgram:(NSString *)vertexShaderFilepath withFragmentShaderFilepath:(NSString *)fragmentShaderFilepath;

/**
 create VBO object
 @param target GL_ELEMENT_ARRAY_BUFFER, GL_ARRAY_BUFFER
 @param usage GL_***_DRAW
 @param datSize data len
 @param data data ptr
 @return vbo id
 */
+(GLuint)createVBO:(GLenum)target usage:(int)usage dataSize:(int)dataSize data:(void*)data;

/**
 create Texture2D Buffer
 @param format RGB GRBA ...
 @param width texture width
 @param height texture height
 @param data texture data
 @return texture buffer id
 */
+(GLuint)createTexture2D:(GLenum)format width:(int)width height:(int)height data:(void*)data;

@end
