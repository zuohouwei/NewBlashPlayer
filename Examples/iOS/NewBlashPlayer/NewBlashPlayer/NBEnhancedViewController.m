//
//  NBEnhancedViewController.m
//  NewBlashPlayer
//
//  Created by liu enbao on 28/10/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#import "NBEnhancedViewController.h"
#import "NBEnhancedGLView.h"
#import <NBAVPlayer.h>
#import "NBGLESUtils.h"

static GLfloat vertices[] = {
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // 右上
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // 左下
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // 左下
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  // 左上
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // 右上
};

@interface NBEnhancedViewController () <NBGLRenderer,
                                        NBAVPlayerDelegate,
                                        NBGLTextureAvailable> {
    // the program handle
    GLuint _programHandle;
    
    // the position handle
    GLuint          _vbo;
    GLuint          _texture;
    int             _vertCount;
}

@property (strong, nonatomic) IBOutlet NBEnhancedGLView *nbEnhancedView;
@property (strong, nonatomic) NBAVPlayer* nbAVPlayer;

@end

@implementation NBEnhancedViewController

- (void)loadView {
    [super loadView];
    [_nbEnhancedView setRenderer:self];
    [_nbEnhancedView setTextureAvailable:self];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    _nbAVPlayer = [[NBAVPlayer alloc] init];
//    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"/Users/liuenbao/Desktop/SharedFiles/DAS0N1_Good_Bye.mp4"] params:nil];
//    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"/Users/liuenbao/Desktop/SharedFiles/LoveTheWayYouLie.mp3"] params:nil];
    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"/Users/liuenbao/Desktop/SharedFiles/The_Innocents_01_01.mp4"] params:nil];
//    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"/Users/liuenbao/Desktop/SharedFiles/DAS0N1_Good_Bye_MPEG4.avi"] params:nil];
//    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"/Users/liuenbao/Desktop/SharedFiles/DAS0N1_Good_Bye_HEVC.mp4"] params:nil];
//    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"zip:/Users/liuenbao/Desktop/SharedFiles/DAS0N1_Good_Bye.mp4.zip"] params:nil];

//    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"http://127.0.0.1:8989/The_Innocents_01_01.mp4"] params:nil];
//    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"rtmp://127.0.0.1/live/sallar"] params:nil];
//    [_nbAVPlayer setDataSource:[NSURL URLWithString:@"rtsp://127.0.0.1:8554/Friends.mkv"] params:nil];
    [_nbEnhancedView setPlayer:_nbAVPlayer];
    _nbAVPlayer.delegate = self;
    [_nbAVPlayer prepareAsync];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

// back button clicked
- (IBAction)onBackClicked:(id)sender {
    [_nbAVPlayer stop];
    [_nbAVPlayer releaseAll];
    _nbAVPlayer = nil;
    
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)setupProgram
{
    // Load shaders
    //
    NSString * vertexShaderPath = [[NSBundle mainBundle] pathForResource:@"VertexShader"
                                                                  ofType:@"glsl"];
    NSString * fragmentShaderPath = [[NSBundle mainBundle] pathForResource:@"FragmentShader"
                                                                    ofType:@"glsl"];
    
    // Create program, attach shaders, compile and link program
    //
    _programHandle = [NBGLESUtils loadProgram:vertexShaderPath
                 withFragmentShaderFilepath:fragmentShaderPath];
    if (_programHandle == 0) {
        NSLog(@" >> Error: Failed to setup program.");
        return;
    }
    
    glUseProgram(_programHandle);
    
    // Get attribute slot from program
    //
//    _positionSlot = glGetAttribLocation(_programHandle, "vPosition");
}

- (void)setupVBO
{
    _vertCount = 6;
    
    // 创建VBO
    _vbo = [NBGLESUtils createVBO:GL_ARRAY_BUFFER usage:GL_STATIC_DRAW dataSize:sizeof(vertices) data:vertices];
    
    glEnableVertexAttribArray(glGetAttribLocation(_programHandle, "position"));
    glVertexAttribPointer(glGetAttribLocation(_programHandle, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL);
    
    glEnableVertexAttribArray(glGetAttribLocation(_programHandle, "texcoord"));
    glVertexAttribPointer(glGetAttribLocation(_programHandle, "texcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL+sizeof(GL_FLOAT)*3);
}

- (void)setupTexure
{
//    NSString *path = [[NSBundle mainBundle] pathForResource:@"wood" ofType:@"jpg"];
//
//    int width;
//    int height;
//
//    UIImage *image = [UIImage imageWithContentsOfFile:path];
//
//    width = image.size.width;
//    height = image.size.height;
//
//    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
//    void *imageData = malloc( height * width * 4 );
//    CGContextRef image_context = CGBitmapContextCreate( imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
//    CGColorSpaceRelease( colorSpace );
//    CGContextClearRect( image_context, CGRectMake( 0, 0, width, height ) );
//    CGContextTranslateCTM( image_context, 0, height - height );
//    CGContextDrawImage( image_context, CGRectMake( 0, 0, width, height ), image.CGImage );
//
//    // Create Texture
//    _texture = [NBGLESUtils createTexture2D:GL_RGBA width:width height:height data:imageData];
//
//    if (imageData != NULL) {
//        free(imageData);
//    }
    
    _texture = 0;
}

- (void)unsetupTexture {
    glDeleteTextures(1, &_texture);
}

- (void)unsetupVBO {
    _vertCount = 0;
    glDeleteBuffers(1, &_vbo);
}

- (void)unsetupProgram {
    glDeleteProgram(_programHandle);
}

- (void)glRenderCreated:(NBEnhancedGLView*)view {
    NSLog(@"glRenderCreated");
    [self setupProgram];
    [self setupVBO];
    [self setupTexure];
}

- (void)glRenderSizeChanged:(NBEnhancedGLView*)view width:(NSInteger)width height:(NSInteger)height {
    NSLog(@"glRenderSizeChanged");
}

- (void)glTextureAvailable:(int)texName {
    _texture = texName;
}

- (void)glRenderDrawFrame:(NBEnhancedGLView*)view {
    // 激活纹理
    glActiveTexture(GL_TEXTURE0);
    
    if (_texture != 0) {
        glBindTexture(GL_TEXTURE_2D, _texture);
        glUniform1i(glGetUniformLocation(_programHandle, "image"), 0);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, _vertCount);
}

- (void)glRenderDestroy:(NBEnhancedGLView*)view {
    NSLog(@"glRenderDestroy");
    [self unsetupTexture];
    [self unsetupVBO];
    [self unsetupProgram];
}

/**
 * Called when the player prepared.
 *
 * @param player The shared media player instance.
 * @param arg Not use.
 */
- (void)mediaPlayer:(NBAVPlayer *)player didPrepared:(id)arg {
     [_nbAVPlayer start];
}

/**
 * Called when the player playback completed.
 *
 * @param player The shared media player instance.
 * @param arg Not use.
 */
- (void)mediaPlayer:(NBAVPlayer *)player playbackComplete:(id)arg {
    
}

/**
 * Called when the player have error occur.
 *
 * @param player The shared media player instance.
 * @param arg Contain the detail error information.
 */
- (void)mediaPlayer:(NBAVPlayer *)player error:(id)arg {
    [self onBackClicked:nil];
}

- (void)viewWillAppear:(BOOL)animated {
    [_nbEnhancedView resume];
}

- (void)viewDidDisappear:(BOOL)animated {
    [_nbEnhancedView pause];
}

- (BOOL)shouldAutorotate {
    return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskLandscape;
}

- (BOOL)prefersStatusBarHidden {
    return NO;
}

@end
