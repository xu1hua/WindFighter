LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/BackgroundLayer.cpp \
				   ../../../Classes/GamePlayLayer.cpp \
				   ../../../Classes/GamePlayScene.cpp \
				   ../../../Classes/MainMenuLayer.cpp \
				   ../../../Classes/MainMenuScene.cpp \
				   ../../../Classes/MapMakerScene.cpp \
				   ../../../Classes/Square.cpp \
				   ../../../Classes/SquareBaseplateLayer.cpp \
				   ../../../Classes/SquareGroup.cpp \
				   ../../../Classes/GameLibrary/Sqlite3Database/CppSQLite3.cpp \
				   ../../../Classes/GameLibrary/Sqlite3Database/CppSQLite3Table.cpp \
				   ../../../Classes/GameLibrary/Sqlite3Database/CppSQLite3Query.cpp \
				   ../../../Classes/GameLibrary/Sqlite3Database/CppSQLite3Statement.cpp \
				   ../../../Classes/GameLibrary/Sqlite3Database/CppSQLite3DB.cpp \
				   ../../../Classes/GameLibrary/Sqlite3Database/GameDB.cpp \
				   ../../../Classes/GameLibrary/sqlite/sqlite3.c \
				   ../../../Classes/GameLibrary/UtilityTools/GameUtilityTools_Android.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Classes/GameLibrary/Sqlite3Database
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Classes/GameLibrary/sqlite
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Classes/GameLibrary/UtilityTools

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
