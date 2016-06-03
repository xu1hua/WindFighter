#include "CppSQLite3Query.h"
#include "CppSQLite3.h"

CppSQLite3Query::CppSQLite3Query()
{
	mpDB = 0;
	mpStmt = 0;
	mnCols = 0;
	mbEof = true;
}

CppSQLite3Query::CppSQLite3Query(sqlite3 *pdb, sqlite3_stmt *pStmt, bool bEof)
{
	mpDB = pdb;
	mpStmt = pStmt;
	mbEof = bEof;
	mnCols = sqlite3_column_count(pStmt);
}

CppSQLite3Query::CppSQLite3Query(const CppSQLite3Query &rQuery)
{
	mpStmt = rQuery.mpStmt;
	const_cast<CppSQLite3Query &>(rQuery).mpStmt = 0;
	mnCols = rQuery.mnCols;
	mbEof = rQuery.mbEof;
}

CppSQLite3Query& CppSQLite3Query::operator =(const CppSQLite3Query &rQuery)
{
	mpStmt = rQuery.mpStmt;
	const_cast<CppSQLite3Query &>(rQuery).mpStmt = 0;
	mnCols = rQuery.mnCols;
	mbEof = rQuery.mbEof;

	return *this;
}

CppSQLite3Query::~CppSQLite3Query()
{
	Finalize();
}

void CppSQLite3Query::CheckStmt()
{
	if (mpStmt == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid Stmt Pointer",
			DONT_DELETE_MSG);
	}
}

int CppSQLite3Query::FieldNums()
{
	CheckStmt();

	return mnCols;
}

//根据字段名返回列索引  
int CppSQLite3Query::FieldIndex(const char* szField)
{
	CheckStmt();

	if (szField)
	{
		for (int nField = 0; nField < mnCols; nField++)
		{
			//后面还有很多类似的函数，参数差不多，需要一个sqlite3_stmt*和列索引值，这应该是内部查询了之后返回的结果，而不是事先保存的  
			const char *szTemp = sqlite3_column_name(mpStmt, nField);
			if (strcmp(szTemp, szField) == 0)
			{
				return nField;
			}
		}
	}

	throw CppSQLite3Exception(CPPSQLITE_ERROR,
		"Invalid field name requested",
		DONT_DELETE_MSG);
}
const char* CppSQLite3Query::FieldName(int nField)
{
	CheckStmt();

	if (nField < 0 || nField > mnCols - 1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_name(mpStmt, nField);
}

int CppSQLite3Query::FieldDataType(int nField)
{
	CheckStmt();

	if (nField < 0 || nField > mnCols - 1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_type(mpStmt, nField);
}
const char* CppSQLite3Query::FieldDeclType(int nField)
{
	CheckStmt();

	if (nField < 0 || nField > mnCols - 1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_decltype(mpStmt, nField);
}

const char* CppSQLite3Query::FieldValue(int nField)
{
	CheckStmt();

	if (nField < 0 || nField > mnCols - 1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return (const char*)sqlite3_column_text(mpStmt, nField);

}

const char* CppSQLite3Query::FieldValue(const char *szField)
{
	int nField = FieldIndex(szField);
	return FieldValue(nField);
}

bool CppSQLite3Query::FieldIsNull(int nField)
{
	return (FieldDataType(nField) == SQLITE_NULL);
}

bool CppSQLite3Query::FieldIsNull(const char *szField)
{
	int nField = FieldIndex(szField);
	return (FieldDataType(nField) == SQLITE_NULL);
}

bool CppSQLite3Query::GetIntValue(int nField, int &rDest)
{
	if (FieldDataType(nField) == SQLITE_NULL)
	{
		return false;
	}
	else
	{
		rDest = sqlite3_column_int(mpStmt, nField);
		return true;
	}
}

bool CppSQLite3Query::GetIntValue(const char *szField, int &rDest)
{
	int nField = FieldIndex(szField);
	return GetIntValue(nField, rDest);
}

bool CppSQLite3Query::GetFloatValue(int nField, double &rDest)
{
	if (FieldDataType(nField) == SQLITE_NULL)
	{
		return false;
	}
	else
	{
		rDest = sqlite3_column_double(mpStmt, nField);
		return true;
	}
}
bool CppSQLite3Query::GetFloatValue(const char *szField, double &rDest)
{
	int nField = FieldIndex(szField);
	return GetFloatValue(nField, rDest);
}

bool CppSQLite3Query::GetStringValue(int nField, char *&rDest)
{
	if (FieldDataType(nField) == SQLITE_NULL)
	{
		return false;
	}
	else
	{
		rDest = const_cast<char *>((const char*)sqlite3_column_text(mpStmt, nField));
		return true;
	}
}
bool CppSQLite3Query::GetStringValue(const char *szField, char *&rDest)
{
	int nField = FieldIndex(szField);
	return GetStringValue(nField, rDest);
}

bool CppSQLite3Query::Eof()
{
	CheckStmt();

	return mbEof;
}

void CppSQLite3Query::NextRow()
{
	CheckStmt();

	int nRet = sqlite3_step(mpStmt);

	if (nRet == SQLITE_DONE)
	{
		// no rows  
		mbEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do  
	}
	else
	{
		nRet = sqlite3_finalize(mpStmt);
		mpStmt = 0;
		const char* szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, szError, DONT_DELETE_MSG);
	}
}

void CppSQLite3Query::Finalize()
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