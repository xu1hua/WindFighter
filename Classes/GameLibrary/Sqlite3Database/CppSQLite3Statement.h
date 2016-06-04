#pragma once  
#include <cstdio>  
#include <cstring>  
#include "../sqlite/sqlite3.h"  

class CppSQLite3Query;
class CppSQLite3Statement
{
private:
	sqlite3 *mpDB;
	sqlite3_stmt *mpStmt;

	void CheckDB();
	void CheckStmt();
public:
	CppSQLite3Statement();
	CppSQLite3Statement(sqlite3 *pdb, sqlite3_stmt *pstmt);
	CppSQLite3Statement(const CppSQLite3Statement &rStatement);
	CppSQLite3Statement& operator = (const CppSQLite3Statement &rStatement);
	~CppSQLite3Statement();

	int ExecDML();
	CppSQLite3Query ExecQuery();

	void Reset();

	void Finalize();
};