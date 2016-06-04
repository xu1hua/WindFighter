#pragma once  
#include <cstdio>  
#include <cstring>  
#include "../sqlite/sqlite3.h"  
class CppSQLite3Query;
class CppSQLite3Statement;
class CppSQLite3Table;
class CppSQLite3DB
{
private:
	sqlite3 *mpDB;
	sqlite3_stmt *mpStmt;

	void CheckDB();
	CppSQLite3DB(const CppSQLite3DB &rDB);
	CppSQLite3DB& operator = (const CppSQLite3DB &rDB);

	sqlite3_stmt* Compile(const char *szSQL);

public:
	CppSQLite3DB();
	~CppSQLite3DB();

	void Open(const char *szFileName);
	void Close();

	int ExecDML(const char *szSQL);
	CppSQLite3Query ExecQuery(const char *szSQL);

	CppSQLite3Table GetTable(const char *szSQL);

	CppSQLite3Statement CompileStatement(const char *szSQL);

};