//
//  AVAnimatorMedia.h
//
//  Created by Moses DeJong on 3/18/09.
//
//  License terms defined in License.txt.
//
//  This file defines the media object that is rendered to a view
//  via AVAnimatorView. The media object handles all the details
//  of frame rate, the current frame, and so on. The view provides
//  a way to render the media in the window system.

#import <UIKit/UIKit.h>

#import "AVAnimatorMediaRendererProtocol.h"

#define AVAnimator30FPS (1.0/30)
#define AVAnimator24FPS (1.0/24)
#define AVAnimator15FPS (1.0/15)
#define AVAnimator10FPS (1.0/10)
#define AVAnimator5FPS (1.0/5)

// AVAnimatorPreparedToAnimateNotification is delivered after resources
// have been loaded and the view is ready to animate.

// AVAnimatorDidStartNotification is delivered when an animation starts (once for each loop)
// AVAnimatorDidStopNotification is delivered when an animation stops (once for each loop)

// AVAnimatorDidPauseNotification is deliverd when the animation is paused, for example
// if a call comes in to the iPhone or when the pause button in movie controls is pressed.

// AVAnimatorDidUnpauseNotification is devliered when a pause is undone, so playing agan

// AVAnimatorDoneNotification is invoked when done animating, if a number of loops were
// requested then the done notification is delivered once all the loops have been played.

#define AVAnimatorPreparedToAnimateNotification @"AVAnimatorPreparedToAnimateNotification"
#define AVAnimatorDidStartNotification @"AVAnimatorDidStartNotification"
#define AVAnimatorDidStopNotification @"AVAnimatorDidStopNotification"
#define AVAnimatorDidPauseNotification @"AVAnimatorDidPauseNotification"
#define AVAnimatorDidUnpauseNotification @"AVAnimatorDidUnpauseNotification"
#define AVAnimatorDoneNotification @"AVAnimatorDoneNotification"

@class AVAudioPlayer;
@class NSURL;
@class AVFrameDecoder;
@class AVResourceLoader;

typedef enum AVAnimatorPlayerState {
	ALLOCATED = 0,
	LOADED = 1,
	PREPPING = 2,
	READY = 3,
	ANIMATING = 4,
	STOPPED = 5,
	PAUSED = 6
} AVAnimatorPlayerState;

@interface AVAnimatorMedia : NSObject {
@private

	id<AVAnimatorMediaRendererProtocol> m_renderer;
  
	AVResourceLoader *m_resourceLoader;
	AVFrameDecoder *m_frameDecoder;
  
	NSTimeInterval m_animatorFrameDuration;
	NSUInteger m_animatorNumFrames;
	NSUInteger m_animatorRepeatCount;
  
	NSURL *m_animatorAudioURL;	
	UIImage *m_prevFrame;
	UIImage *m_nextFrame;
  
	NSTimer *m_animatorPrepTimer;
	NSTimer *m_animatorReadyTimer;
	NSTimer *m_animatorDecodeTimer;
	NSTimer *m_animatorDisplayTimer;
	
	NSInteger m_currentFrame;
	NSUInteger m_repeatedFrameCount;
  
	AVAudioPlayer *m_avAudioPlayer;
	id m_originalAudioDelegate;
	id m_retainedAudioDelegate;
  NSDate *m_audioSimulatedStartTime;
  NSDate *m_audioSimulatedNowTime;
  NSDate *m_audioPlayerFallbackStartTime;
  NSDate *m_audioPlayerFallbackNowTime;
  
	AVAnimatorPlayerState m_state;
  
  // This time stores an offset from the original start time
  // at the moment the pause command is invoked.
  NSTimeInterval m_pauseTimeInterval;
  
	NSTimeInterval m_animatorMaxClockTime;
	NSTimeInterval m_animatorDecodeTimerInterval;
  
//	CGSize m_renderSize;
    
	// Becomes TRUE the first time the state changes to READY
	// and stays TRUE after that. This flag is needed to handle
	// the case where the player is stopped before it becomes
	// ready to animate. A change from STOPPED to ANIMATING
	// is only valid if the state has been READY already.
  
	BOOL m_isReadyToAnimate;
  
	// Set to TRUE if startAnimator is called after the
	// prepare phase is complete.
  
	BOOL m_startAnimatorWhenReady;
  
	// Set to TRUE once the second frame has been decoded

	BOOL m_decodedSecondFrame;

	// Set to TRUE when decodedSecondFrame becomes TRUE,
	// then set to false during the first decode operation.
  
	BOOL m_ignoreRepeatedFirstFrameReport;
  
	// Set to TRUE once the last frame has been decoded
  
	BOOL m_decodedLastFrame;
  
	// Set to TRUE when the audio clock time reports
	// a zero time after frames have been decoded.
	// This happens when the audio that goes along
	// with an animation is as long as the time
	// needed to display the frames. Clock time
	// is reported in terms of the fallback clock
	// in this case.
  
	BOOL reportTimeFromFallbackClock;
}

// public properties

@property (nonatomic, retain) AVResourceLoader *resourceLoader;
@property (nonatomic, retain) AVFrameDecoder *frameDecoder;

@property (nonatomic, assign) NSTimeInterval animatorFrameDuration;
@property (nonatomic, assign) NSUInteger animatorNumFrames;

@property (nonatomic, assign) NSUInteger animatorRepeatCount;

// TRUE when the animator has an audio track. This property is not set until the
// resource loaded is done loading and AVAnimatorPreparedToAnimateNotification
// has been delivered.
@property (nonatomic, readonly) BOOL hasAudio;

// static ctor : create media object in autorelease pool
+ (AVAnimatorMedia*) aVAnimatorMedia;

- (void) startAnimator;
- (void) stopAnimator;

- (BOOL) isAnimatorRunning;
- (BOOL) isInitializing;
- (void) doneAnimator;

- (void) pause;
- (void) unpause;
- (void) rewind;

- (void) prepareToAnimate;

// Display the given animator frame, in the range [1 to N]
// where N is the largest frame number. Note that this method
// should only be called when the animator is not running.

- (void) showFrame: (NSInteger) frame;

// These next two method should be invoked from a renderer to signal
// when this media item is attached to and detached from a renderer.

- (void) attachToRenderer:(id<AVAnimatorMediaRendererProtocol>)renderer;

- (void) detachFromRenderer:(id<AVAnimatorMediaRendererProtocol>)renderer;

@end
