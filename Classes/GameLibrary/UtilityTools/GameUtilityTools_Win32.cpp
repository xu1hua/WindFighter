#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "GameUtilityTools.h"
#include <string>
#include <stdio.h>
#include <objbase.h>
GameUilityTools* GameUilityTools::s_instance = nullptr;

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
const char* GameUilityTools::CreateGuidString()
{
	return (GuidToString(CreateGuid())).c_str();
}
#endif