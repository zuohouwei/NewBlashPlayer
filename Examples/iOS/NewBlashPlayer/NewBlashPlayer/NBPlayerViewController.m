//
//  NBPlayerViewController.m
//  NewBlashPlayer
//
//  Created by liu enbao on 17/09/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#import "NBPlayerViewController.h"
#import <NBGenericGLView.h>
#import <NBAVPlayer.h>

@interface NBPlayerViewController () <NBAVPlayerDelegate> {
    NSTimer* _seekTime;
}
@property (weak, nonatomic) IBOutlet NBGenericGLView *nbGLView;
@property (strong, nonatomic) NBAVPlayer* nbAVPlayer;

@end

@implementation NBPlayerViewController

//- (void)loadView {
//    self.view = _nbGLView = [[NBGLView alloc] init];
//}

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
//    [_nbAVPlayer setVideoOutput:_nbGLView];
    [_nbGLView setPlayer:_nbAVPlayer];
    _nbAVPlayer.delegate = self;
    [_nbAVPlayer prepareAsync];
}

- (void)mediaPlayer:(NBAVPlayer *)player didPrepared:(id)arg {
    [_nbAVPlayer start];
    
    _seekTime = [NSTimer scheduledTimerWithTimeInterval:5 target:self selector:@selector(seekTimerFired:) userInfo:nil repeats:NO];
}

- (void)mediaPlayer:(NBAVPlayer *)player playbackComplete:(id)arg {
    NSLog(@"The seek time is complete");
    [_seekTime invalidate];
    _seekTime = nil;
    
    [self onBackClicked:nil];
}

- (void)mediaPlayer:(NBAVPlayer *)player error:(id)arg {
    [self onBackClicked:nil];
}

// back button clicked
- (IBAction)onBackClicked:(id)sender {
    [_nbAVPlayer stop];
    [_nbAVPlayer releaseAll];
    _nbAVPlayer = nil;
    
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)seekTimerFired:(id)sender {
    int64_t currPos = [_nbAVPlayer getCurrentPosition];
    int64_t duration = [_nbAVPlayer getDuration];
    
    NSLog(@"seek pos : %lld duration : %lld", currPos, duration);
  
//    [_nbAVPlayer pause];
//
//    [_seekTime invalidate];
//    _seekTime = nil;
//
//    _seekTime = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(setgravityTimerFired:) userInfo:nil repeats:NO];
//
//    [_nbAVPlayer seekTo:currPos + duration * 0.1];
//
//    [_nbAVPlayer seekTo:100];
}

- (void)setgravityTimerFired:(id)sender {
    [_nbGLView setVideoGravity:NBVideoGravityResizeAspectFill];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}

*/

@end
