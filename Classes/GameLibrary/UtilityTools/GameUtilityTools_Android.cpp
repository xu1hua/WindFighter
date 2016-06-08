//
//  GameUtilityTools_Android.cpp
//  SquareClear
//
//  Created by 徐徐 on 16/6/6.
//
//
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <stdio.h>
#include "platform/android/jni/JniHelper.h"
#include "cocos2d.h"
#include "GameUtilityTools.h"

USING_NS_CC;
const char* GameUilityTools::CreateGuidString()
{
    jstring jstr;
    JniMethodInfo t;
    if(!JniHelper::getStaticMethodInfo(t, "org.cocos2dx.cpp.AppUtility", "CreateGuidString", "()Ljava/lang/String;"))
    {
        CCLOG("org.cocos2dx.cpp.AppUtility CreateGuidString: error to get methodInfo");
        return "";
    }
    jstr = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
    std::string _uuid = JniHelper::jstring2string(jstr);
    t.env->DeleteLocalRef(jstr);
    return _uuid.c_str();
}

#endif