//
//  MusicArt.x
//  MusicArt
//
//  Created by Jason Recillo <jaysan1292+cydia@gmail.com> on 29.04.2014.
//  Copyright (c) 2014 Jason Recillo <jaysan1292+cydia@gmail.com>. All rights reserved.
//

#import <MediaPlayer/MediaPlayer.h>
#import "PrivateHeaders.h"
#import "jdc.h"

#define kIsPlaying (MPMusicPlayerController.iPodMusicPlayer.playbackState == MPMusicPlaybackStatePlaying)
#define kNowPlayingItem MPMusicPlayerController.iPodMusicPlayer.nowPlayingItem

#define kFadeSpeed 1.0
#define kAlbumViewTag 1598

NSString* _musicAppIdentifier = @"com.apple.Music";

BOOL Enabled  = YES;
BOOL Cooldown = NO;

SBIconView* _ourInstance = nil;

@interface MAMusicArtController : NSObject
+ (instancetype)sharedInstance;
- (BOOL)musicIconHasAlbumArt;
- (SBIconView*)_getMusicIconView;
- (void)_removeAlbumArtIcon;
- (void)hideAlbumArt;
- (void)nowPlayingItemDidChange:(id)notification;
- (void)playbackStateDidChange:(id)notification;
- (void)showAlbumArt;
- (void)updateAlbumArt;
@end

@implementation MAMusicArtController
+ (instancetype)sharedInstance {
	static id instance;
	static dispatch_once_t once;
	dispatch_once(&once, ^{
		instance = [[MAMusicArtController alloc] init];
	});
	return instance;
}
- (BOOL)musicIconHasAlbumArt {
	return [[self _getMusicIconView] viewWithTag:kAlbumViewTag] != nil;
}
- (SBIconView*)_getMusicIconView {
	return _ourInstance;
}
- (UIImageView*)_getAlbumArtIcon {
	return (UIImageView*)[[self _getMusicIconView] viewWithTag:kAlbumViewTag];
}
- (void)_removeAlbumArtIcon {
	SBIconView* icon = [self _getMusicIconView];
	if ([icon viewWithTag:kAlbumViewTag]) {
		[UIView animateWithDuration:kFadeSpeed
		        animations:^{[[icon viewWithTag:kAlbumViewTag] setAlpha:0.0];}
		        completion:^(BOOL f){[[icon viewWithTag:kAlbumViewTag] removeFromSuperview];}];
    }
}
- (void)nowPlayingItemDidChange:(id)notification {
	if (!Enabled) return;

	if (kIsPlaying){
		log(@"Playing");
		[self updateAlbumArt];
	} else {
		log(@"Not playing");
	}
}
- (void)playbackStateDidChange:(id)notification {
	if (!Enabled) return;

	switch (MPMusicPlayerController.iPodMusicPlayer.playbackState) {
		case MPMusicPlaybackStatePaused:
		case MPMusicPlaybackStateStopped:
			if (Cooldown) break;
			[self _removeAlbumArtIcon];
			break;
		case MPMusicPlaybackStatePlaying:
			[self updateAlbumArt];
			break;
		default:
			break;
	}
}
- (void)updateAlbumArt {
	if (!Enabled || !kIsPlaying || Cooldown) return;

	// We need a "cooldown" for this method, because iOS will usually send multiple MediaPlayer
	// notifications at once in some cases, causing the album art to either never appear, or never change
	Cooldown = YES;
	dispatch_after_seconds_on_default_queue(1.0, ^{Cooldown = NO;});

	info(@"Now Playing: %@ - %@", kNowPlayingItem.title, kNowPlayingItem.artist);

	SBIconView* icon = [self _getMusicIconView];
	CGSize iconSize = CGSizeMake(icon.frame.size.width, icon.frame.size.width);

	UIImageView* albumArtView = nil;
	UIImage* albumArt = [MPMusicPlayerController.iPodMusicPlayer.nowPlayingItem.artwork imageWithSize:iconSize];
	if (!albumArt) {
		// If the current song doesn't have album artwork, remove the view
		[self _removeAlbumArtIcon];
		return;
	}

	if ((albumArtView = (UIImageView*)[icon viewWithTag:kAlbumViewTag])) {
		// Crossfade between the old artwork and the new artwork
		CABasicAnimation* crossfade = [CABasicAnimation animationWithKeyPath:@"contents"];
		crossfade.duration  = kFadeSpeed;
		crossfade.fromValue = (__bridge id)albumArtView.image.CGImage;
		crossfade.toValue   = (__bridge id)albumArt.CGImage;
		[albumArtView.layer addAnimation:crossfade forKey:@"animateContents"];
		[albumArtView setImage:albumArt];
	} else {
		// Add the artwork view to the Music.app icon
		albumArtView = [[UIImageView alloc] initWithImage:albumArt];
		// Set a white background, for non-square art covers
		[albumArtView setBackgroundColor:[UIColor whiteColor]];
		[albumArtView setTag:kAlbumViewTag];
		[albumArtView setAlpha:0.0];
		[albumArtView.layer setMasksToBounds:YES];
		[albumArtView.layer setCornerRadius:2.5];
		[icon addSubview:albumArtView];
		[albumArtView setContentMode:UIViewContentModeScaleAspectFit];
		[albumArtView setFrame:CGRectMake(0, 0, iconSize.width, iconSize.height)];
		[UIView animateWithDuration:kFadeSpeed animations:^{[[icon viewWithTag:kAlbumViewTag] setAlpha:1.0];}];
	}
}
- (void)hideAlbumArt {
	UIImageView* albumArt = [self _getAlbumArtIcon];
	if (albumArt) {
		log(@"hide icon");
		CABasicAnimation* anim = [[[%c(SBUIController).sharedInstance switcherController] _transitionAnimationFactory] _animation];
		anim.fromValue = @(1.0);
		anim.toValue   = @(0.0);
		[albumArt.layer addAnimation:anim forKey:@"opacity"];
		[albumArt setAlpha:0.0];
	}
}
- (void)showAlbumArt {
	UIImageView* albumArt = [self _getAlbumArtIcon];
	if (albumArt) {
		log(@"show icon");
		CABasicAnimation* anim = [[[%c(SBUIController).sharedInstance switcherController] _transitionAnimationFactory] _animation];
		anim.fromValue = @(0.0);
		anim.toValue   = @(1.0);
		[albumArt.layer addAnimation:anim forKey:@"opacity"];
		[albumArt setAlpha:1.0];
	}
}
@end

%group main

%hook SBIconView
- (void)setIcon:(SBIcon*)icon {
	%orig;
	if ([_musicAppIdentifier isEqualToString:[icon nodeIdentifier]] && (&_ourInstance != &self)) {
		log_selector_with_message(@"%@", icon);
		// Our icon instance has changed, so remove the album art from the old view and add it to our current one
		// Calling 'viewWithTag' on the sharedApplication's keyWindow will return the correct view, wherever it is
		UIView* albumArtView = [[[UIApplication sharedApplication] keyWindow] viewWithTag:kAlbumViewTag];
		[albumArtView removeFromSuperview];
		[self addSubview:albumArtView];
		_ourInstance = self;
	}
}
%end

// Hide or show the album art when entering and leaving the music app, respectively
%hook SBApplication
- (void)willAnimateDeactivation:(_Bool)arg1 {
	%orig;
	if (![[self bundleIdentifier] isEqualToString:@"com.apple.Music"] || !Enabled || !kIsPlaying) return;
	[MAMusicArtController.sharedInstance showAlbumArt];
}
- (void)willAnimateActivation {
	%orig;
	if (![[self bundleIdentifier] isEqualToString:@"com.apple.Music"] || !Enabled || !kIsPlaying) return;
	[MAMusicArtController.sharedInstance hideAlbumArt];
}
%end

%hook SBLockScreenManager
- (void)_finishUIUnlockFromSource:(int)source withOptions:(id)options {
	%orig;
	if (!Enabled || !kIsPlaying) return;
	// In some cases, the album art may disappear after an unlock, so add it back
	if (![MAMusicArtController.sharedInstance musicIconHasAlbumArt])
		[MAMusicArtController.sharedInstance updateAlbumArt];
}
%end

%end

void preferenceChanged() {
	NSDictionary* prefs = dictWithFile(userDir(@"/Library/Preferences/com.jaysan1292.musicart.plist"));
	Enabled = defaults(prefs, @"enabled", YES, boolValue);
	log(@"Enabled? %@", NSStringFromBool(Enabled));

	if (Enabled) {
		[MAMusicArtController.sharedInstance updateAlbumArt];
	} else {
		[MAMusicArtController.sharedInstance _removeAlbumArtIcon];
	}
}

%ctor {
	___log_internal("", @"Init MusicArt");
	// Register preference changed notification listener
	register_for_darwin_notifications(CFSTR("com.jaysan1292.musicart.toggled"), (CFNotificationCallback)preferenceChanged);

	// Register for the MediaPlayer notifications
	register_for_nsnotifications_on_object(MPMusicPlayerControllerPlaybackStateDidChangeNotification, MPMusicPlayerController.iPodMusicPlayer, [MAMusicArtController sharedInstance], @selector(playbackStateDidChange:));
	register_for_nsnotifications_on_object(MPMusicPlayerControllerNowPlayingItemDidChangeNotification, MPMusicPlayerController.iPodMusicPlayer, [MAMusicArtController sharedInstance], @selector(nowPlayingItemDidChange:));

	// Make the music app send notifications
	[MPMusicPlayerController.iPodMusicPlayer beginGeneratingPlaybackNotifications];

	%init(main)
}
