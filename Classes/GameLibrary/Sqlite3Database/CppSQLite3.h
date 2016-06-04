#pragma once  
#include <cstdio>  
#include <cstring>  
#include "../sqlite/sqlite3.h"  

#define CPPSQLITE_ERROR 1000  //这个宏定义是为了区分库函数中定义的错误代码，专用于自己程序中的异常标示 
static const bool DONT_DELETE_MSG = false;
class CppSQLite3Exception
{
private:
	int mnErrCode;
	char *mpszErrMessage;

public:
	CppSQLite3Exception(const int nErrNode,
		const char *szErrMess,
		bool bDeleteMess);
	CppSQLite3Exception(const CppSQLite3Exception &e);
	~CppSQLite3Exception();

	const int ErrorCode() { return mnErrCode; }
	const char *ErrorMessage() { return mpszErrMessage; }

	static const char* ErrorCodeAsString(const int nErrCode);
};