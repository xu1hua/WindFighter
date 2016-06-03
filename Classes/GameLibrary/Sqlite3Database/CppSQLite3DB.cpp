#include "CppSQLite3Query.h"
#include "CppSQLite3.h"
#include "CppSQLite3Statement.h"
#include "CppSQLite3DB.h"
#include "CppSQLite3Table.h"

CppSQLite3DB::CppSQLite3DB()
{
	mpDB = 0;
}

CppSQLite3DB::CppSQLite3DB(const CppSQLite3DB &rDB)
{
	mpDB = rDB.mpDB;
}

CppSQLite3DB::~CppSQLite3DB()
{
	Close();
}

CppSQLite3DB& CppSQLite3DB::operator =(const CppSQLite3DB &rDB)
{
	mpDB = rDB.mpDB;
	return *this;
}

void CppSQLite3DB::CheckDB()
{
	if (!mpDB)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Database not open",
			DONT_DELETE_MSG);
	}
}

sqlite3_stmt* CppSQLite3DB::Compile(const char *szSQL)
{
	CheckDB();

	const char *szTail = 0;
	sqlite3_stmt *pStmt;

	int nRet = sqlite3_prepare(mpDB, szSQL, -1, &pStmt, &szTail);

	if (nRet != SQLITE_OK)
	{
		const char *szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
	}

	return pStmt;
}

void CppSQLite3DB::Open(const char *szFileName)
{
	int nRet = sqlite3_open(szFileName, &mpDB);
	if (nRet != SQLITE_OK)
	{
		const char *szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
	}
}

void CppSQLite3DB::Close()
{
	if (mpDB)
	{
		int nRet = sqlite3_close(mpDB);
		if (nRet == SQLITE_OK)
		{
			mpDB = 0; //一旦关闭数据库指针，要置为0，防止多次关闭出错  
		}
		else
		{
			throw CppSQLite3Exception(CPPSQLITE_ERROR,
				"Unable to close database",
				DONT_DELETE_MSG);
		}
	}
}

int CppSQLite3DB::ExecDML(const char *szSQL)
{
	CheckDB();

	char* szError = 0;

	int nRet = sqlite3_exec(mpDB, szSQL, 0, 0, &szError);

	if (nRet == SQLITE_OK)
	{
		return sqlite3_changes(mpDB);  //返回这个执行影响的行数  
	}
	else
	{
		throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
	}
}

CppSQLite3Query CppSQLite3DB::ExecQuery(const char *szSQL)
{
	CheckDB();

	//编译一条指针对象，用临时变量存储，传递给CppSQLite3Query后，  
	//这个临时sqlite3_stmt*对象会自动消失，最后只有一份保留在CppSQLite3Query中  
	sqlite3_stmt *pStmt = Compile(szSQL);

	int nRet = sqlite3_step(pStmt);

	if (nRet == SQLITE_DONE) //表明这个查询没有返回结果  
	{
		return CppSQLite3Query(mpDB, pStmt, true); //所以将第三个字段设置为true，表明到达查询结果末尾处  
	}
	else if (nRet == SQLITE_ROW) //这个查询中至少有一行记录  
	{
		return CppSQLite3Query(mpDB, pStmt, false); //false表明没有到达查询结果末尾  
	}
	else
	{
		nRet = sqlite3_finalize(pStmt);
		const char *szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
	}
}

CppSQLite3Table CppSQLite3DB::GetTable(const char *szSQL)
{
	CheckDB();

	char* szError = 0;
	char** paszResults = 0;
	int nRet;
	int nRows(0);
	int nCols(0);

	nRet = sqlite3_get_table(mpDB, szSQL, &paszResults, &nRows, &nCols, &szError);

	if (nRet == SQLITE_OK)
	{
		return CppSQLite3Table(paszResults, nRows, nCols);
	}
	else
	{
		throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
	}
}

CppSQLite3Statement CppSQLite3DB::CompileStatement(const char *szSQL)
{
	CheckDB();

	sqlite3_stmt *pStmt = Compile(szSQL);
	return CppSQLite3Statement(mpDB, pStmt);
}