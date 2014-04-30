//
//  utils.h
//  jd-common
//
//  Created by Jason Recillo on 14.04.2014
//  Copyright (c) 2014 Jason Recillo. All rights reserved.
//


#define dispatch_seconds_from_now(sec) dispatch_time(DISPATCH_TIME_NOW, sec * NSEC_PER_SEC)
#define dispatch_after_seconds_on_default_queue(sec, block) dispatch_after(dispatch_seconds_from_now(sec), dispatch_get_main_queue(), block)
#define dispatch_async_on_global_queue_with_default_priority(block) dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), block)

#define ___post_notification(center, name) CFNotificationCenterPostNotification(center, name, NULL, NULL, true)
#define ___register_for_notifications_with_observer(center, name, observer, callback) CFNotificationCenterAddObserver(center, observer, callback, name, NULL, CFNotificationSuspensionBehaviorDeliverImmediately)
#define ___register_for_notifications(center, name, callback) ___register_for_notifications_with_observer(center, name, NULL, callback)
#define ___unregister_from_notifications(center, name, observer) CFNotificationCenterRemoveObserver(center, observer, name, NULL)

#define _DNC_ CFNotificationCenterGetDarwinNotifyCenter()
#define post_darwin_notification(name) ___post_notification(_DNC_, name)
#define register_for_darwin_notifications_with_observer(name, observer, callback) ___register_for_notifications_with_observer(_DNC_, name, observer, callback)
#define register_for_darwin_notifications(name, callback) ___register_for_notifications(_DNC_, name, callback)
#define unregister_from_darwin_notifications(name, observer) ___unregister_from_notifications(_DNC_, name, observer)

#define _LNC_ CFNotificationCenterGetLocalCenter()
#define post_local_notification(name) ___post_notification(_LNC_, name)
#define register_for_local_notifications_with_observer(name, observer, callback) ___register_for_notifications_with_observer(_LNC_, name, observer, callback)
#define register_for_local_notifications(name, callback) ___register_for_notifications(_LNC_, name, callback)
#define unregister_from_local_notifications(name, observer) ___unregister_from_notifications(_LNC_, name, observer)

#define _NSC_ [NSNotificationCenter defaultCenter]
#define post_nsnotification(name, sender, data) [_NSC_ postNotificationName:name object:sender userInfo:data]
#define register_for_nsnotifications_on_object(_name, _object, observer, callback) [_NSC_ addObserver:observer selector:callback name:_name object:_object]
#define register_for_nsnotifications(_name, observer, callback) register_for_nsnotifications_on_object(_name, nil, observer, callback)
#define unregister_from_nsnotifications_on_object(_name, _object, observer) [_NSC_ removeObserver:observer name:_name object:_object]
#define unregister_from_nsnotifications(_name, observer) unregister_from_nsnotifications_on_object(_name, nil, observer)

#define strFmt(str, args...) [NSString stringWithFormat:str, ##args]
#define NSStringFromBool(x) (x?@"YES":@"NO")
#define regex(pattern, opts, err) [NSRegularExpression regularExpressionWithPattern:pattern options:opts error:err]
#define regex_replace(regex, str, opts, rng, replace) [regex stringByReplacingMatchesInString:str options:opts range:rng withTemplate:replace]
#define strip_newlines(str) regex_replace(regex(@"\n", 0, nil), str, 0, NSMakeRange(0,[str length]), @"")
#define strip_whitespace(str) regex_replace(regex(@"\\s+", 0, nil), str, 0, NSMakeRange(0,[str length]), @" ")

#if __has_feature(objc_dictionary_literals)
	#define defaultString(dict, key, defaultValue)  (dict[key] ?  dict[key]       : defaultValue)
	#define defaults(dict, key, defaultValue, type) (dict[key] ? [dict[key] type] : defaultValue)
#else
	#define defaultString(dict, key, defaultValue)  ([dict objectForKey:key] ?  [dict objectForKey:key]       : defaultValue)
	#define defaults(dict, key, defaultValue, type) ([dict objectForKey:key] ? [[dict objectForKey:key] type] : defaultValue)
#endif

#define userDir(path) [NSHomeDirectory() stringByAppendingPathComponent:path]

#define OKDialog(title, msg, del) [[UIAlertView alloc] initWithTitle:title message:msg delegate:del cancelButtonTitle:@"OK" otherButtonTitles:nil]

#define UIColorWithPercentAndAlpha(r, g, b, a) [UIColor colorWithRed:r green:g blue:b alpha:a]
#define UIColorWithPercent(r, g, b)            UIColorWithPercentAndAlpha(r, g, b, 1.f)

#define dictWithFile(file) [NSDictionary dictionaryWithContentsOfFile:file]
#define mdictWithFile(file) [NSMutableDictionary dictionaryWithContentsOfFile:file]

#define ScaledCGSize(size, scale) CGSizeMake(size.width * scale, size.height * scale)
