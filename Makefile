export ARCHS = armv7 armv7s arm64
export TARGET = iphone:clang:latest:7.0
export THEOS_BUILD_DIR = Build
export THEOS_PACKAGE_DIR_NAME = Packages

include theos/makefiles/common.mk

TWEAK_NAME = MusicArt
MusicArt_CFLAGS = -fobjc-arc
MusicArt_FILES = MusicArt.x
MusicArt_FRAMEWORKS = Foundation QuartzCore UIKit MediaPlayer

include $(THEOS_MAKE_PATH)/tweak.mk

ifeq ($(DEBUG),1)
ADDITIONAL_LDFLAGS += -Wl,-map,$@.map -g -x c /dev/null -x none
endif

internal-stage::
	mkdir -p $(THEOS_STAGING_DIR)/Library/PreferenceLoader/Preferences
	cp $(THEOS_PROJECT_DIR)/prefs.plist $(THEOS_STAGING_DIR)/Library/PreferenceLoader/Preferences/MusicArt.plist

after-install::
	install.exec "killall -9 backboardd"
