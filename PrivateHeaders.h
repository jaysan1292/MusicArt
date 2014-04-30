//
//  PrivateHeaders.h
//  MusicArt
//
//  Created by Jason Recillo <jaysan1292+cydia@gmail.com> on 29.04.2014.
//  Copyright (c) 2014 Jason Recillo <jaysan1292+cydia@gmail.com>. All rights reserved.
//

@interface UIImage ()
+ (UIImage*)imageNamed:(NSString*)name inBundle:(NSBundle*)bundle;
@end

@interface MPMediaItem ()
@property(copy, nonatomic) NSDate* dateAccessed;
@property(copy, nonatomic) NSDate* lastPlayedDate;
@property(copy, nonatomic) NSDate* lastSkippedDate;
@property(nonatomic) _Bool hasBeenPlayed;
@property(nonatomic) unsigned long long playCount;
@property(nonatomic) unsigned long long playCountSinceSync;
@property(nonatomic) unsigned long long rating;
@property(nonatomic) unsigned long long skipCount;
@property(nonatomic) unsigned long long skipCountSinceSync;
@property(readonly, nonatomic) _Bool isITunesU;
@property(readonly, nonatomic) _Bool isRental;
@property(readonly, nonatomic) _Bool isUsableAsRepresentativeItem;
@property(readonly, nonatomic) _Bool rememberBookmarkTime;
@property(readonly, nonatomic) double bookmarkTime;
@property(readonly, nonatomic) double effectiveStopTime;
@property(readonly, nonatomic) double playbackDuration;
@property(readonly, nonatomic) double startTime;
@property(readonly, nonatomic) double stopTime;
@property(readonly, nonatomic) MPMediaItemArtwork* artwork;
@property(readonly, nonatomic) NSArray* chapters;
@property(readonly, nonatomic) NSDate* releaseDate;
@property(readonly, nonatomic) NSString* albumArtist;
@property(readonly, nonatomic) NSString* albumTitle;
@property(readonly, nonatomic) NSString* artist;
@property(readonly, nonatomic) NSString* composer;
@property(readonly, nonatomic) NSString* effectiveAlbumArtist;
@property(readonly, nonatomic) NSString* genre;
@property(readonly, nonatomic) NSString* podcastTitle;
@property(readonly, nonatomic) NSString* title;
@property(readonly, nonatomic) unsigned long long albumTrackNumber;
@property(readonly, nonatomic) unsigned long long mediaType;
@property(readonly, nonatomic) unsigned long long year;
@end

@interface SBIconView : UIView
@end

@interface SBApplication : NSObject
- (id)bundleIdentifier;
@end

@interface SBApplicationIcon : NSObject
@end

@interface SBIconModel : NSObject
- (id)expectedIconForDisplayIdentifier:(NSString*)identifier;
@end

@interface SBIconController : NSObject
+ (id)sharedInstance;
- (SBIconModel*)model;
@end

@interface SBIconViewMap : NSObject
+ (instancetype)homescreenMap;
- (id)mappedIconViewForIcon:(id)icon;
@end

@interface SBFSpringAnimationFactory : NSObject
- (CABasicAnimation*)_animation;
@end

@interface SBAppSliderController : UIViewController
- (SBFSpringAnimationFactory*)_transitionAnimationFactory;
@end

@interface SBUIController : NSObject
+ (id)sharedInstance;
- (id)switcherController;
@end;
