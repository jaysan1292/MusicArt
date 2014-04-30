//
//  logging.h
//  jd-common
//
//  Created by Jason Recillo on 14.04.2014
//  Copyright (c) 2014 Jason Recillo. All rights reserved.
//

#ifndef kLogPrefix
#error kLogPrefix must be defined before including this header!
#endif

#ifdef DEBUG
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#else
#define ANSI_COLOR_RED
#define ANSI_COLOR_GREEN
#define ANSI_COLOR_YELLOW
#define ANSI_COLOR_BLUE
#define ANSI_COLOR_MAGENTA
#define ANSI_COLOR_CYAN
#define ANSI_COLOR_RESET
#endif

#ifndef kLogPrefixColor
#define kLogPrefixColor ANSI_COLOR_CYAN
#endif

#if defined(DEBUG) && !defined(kLogHideFilename)
#define ___log_internal(label, str, args...) NSLog(@"[%s-%s:%d%s]: %@", kLogPrefixColor kLogPrefix ANSI_COLOR_RESET, __FILE__, __LINE__, label, [NSString stringWithFormat:str, ##args])
#else
#define ___log_internal(label, str, args...) NSLog(@"[%s%s]: %@", kLogPrefixColor kLogPrefix ANSI_COLOR_RESET, label, [NSString stringWithFormat:str, ##args])
#endif

#ifdef DEBUG
#define log(str, args...) ___log_internal(" DEBUG", str, ##args)
#else
#define log(str, args...)
#endif
#define info(str, args...)  ___log_internal(ANSI_COLOR_GREEN  "  INFO" ANSI_COLOR_RESET, str, ##args)
#define warn(str, args...)  ___log_internal(ANSI_COLOR_YELLOW "  WARN" ANSI_COLOR_RESET, str, ##args)
#define error(str, args...) ___log_internal(ANSI_COLOR_RED    " ERROR" ANSI_COLOR_RESET, str, ##args)
#define log_selector_with_message(str, args...) log(@"(<%s: %p> %s) %@", object_getClassName(self), self, sel_getName(_cmd), [NSString stringWithFormat:str, ##args]);
#define log_selector() log_selector_with_message(@"")
