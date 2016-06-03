#include "CppSQLite3Statement.h"
#include "CppSQLite3.h"
#include "CppSQLite3Query.h"

CppSQLite3Statement::CppSQLite3Statement()
{
	mpDB = 0;
	mpStmt = 0;
}

CppSQLite3Statement::CppSQLite3Statement(sqlite3 *pdb, sqlite3_stmt *pstmt)
{
	mpDB = pdb;
	mpStmt = pstmt;
}

CppSQLite3Statement::CppSQLite3Statement(const CppSQLite3Statement &rStatement)
{
	mpDB = rStatement.mpDB;
	mpStmt = rStatement.mpStmt;
	const_cast<CppSQLite3Statement &>(rStatement).mpStmt = 0;
}

CppSQLite3Statement::~CppSQLite3Statement()
{

}

CppSQLite3Statement& CppSQLite3Statement::operator = (const CppSQLite3Statement &rStatement)
{
	mpDB = rStatement.mpDB;
	mpStmt = rStatement.mpStmt;
	const_cast<CppSQLite3Statement &>(rStatement).mpStmt = 0;
	return *this;
}

int CppSQLite3Statement::ExecDML()
{
	CheckDB();
	CheckStmt();

	const char *szError = 0;

	int nRet = sqlite3_step(mpStmt);

	if (nRet == SQLITE_DONE)
	{
		int nRowChanged = sqlite3_changes(mpDB);  //返回受影响的行数  

		nRet = sqlite3_reset(mpStmt);  //重置sqlite3_stmt*对象  
		if (nRet != SQLITE_OK)
		{
			szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
		}

		return nRowChanged;
	}
	else
	{
		nRet = sqlite3_reset(mpStmt);
		szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
	}
}


CppSQLite3Query CppSQLite3Statement::ExecQuery()
{
	CheckDB();
	CheckStmt();

	int nRet = sqlite3_step(mpStmt);
	if (nRet == SQLITE_DONE)
	{
		return CppSQLite3Query(mpDB, mpStmt, true);//返回一个查询，没有查询结果，所以置为true  
	}
	else if (nRet == SQLITE_ROW)
	{
		return CppSQLite3Query(mpDB, mpStmt, false);//查询是有结果的，返回false代表这不允许删除次查询对象  
	}
	else
	{
		nRet = sqlite3_reset(mpStmt);
		const char* szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::CheckDB()
{
	if (mpDB == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Database not open",
			DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::CheckStmt()
{
	if (mpStmt == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Null stmt pointer",
			DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::Reset()
{
	if (mpStmt)
	{
		int nRet = sqlite3_reset(mpStmt);

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
		}
	}
}

void CppSQLite3Statement::Finalize()
{
	if (mpStmt)
	{
		int nRet = sqlite3_finalize(mpStmt);
		mpStmt = 0;

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
		}
	}
}