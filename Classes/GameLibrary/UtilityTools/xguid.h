#ifndef XGUID_H
#define XGUID_H


#include <string>
#include <stdio.h>
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <objbase.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif


USING_NS_CC;

namespace XGUID
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	GUID CreateGuid()
	{
		GUID guid;

		CoCreateGuid(&guid);

		return guid;
	}

	std::string GuidToString(const GUID &guid)
	{
		char buf[64] = { 0 };

		_snprintf_s(

			buf,
			sizeof(buf),
			"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
		return std::string(buf);
	}
	std::string CreateGuidString()
	{
		return GuidToString(CreateGuid());
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	std::string CreateGuidString()
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
		return _uuid;
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif



}//namespace



#endif // XGUID_H