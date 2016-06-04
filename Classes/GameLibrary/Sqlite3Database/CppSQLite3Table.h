#pragma once  
#include <cstdio>  
#include <cstring>  
#include "../sqlite/sqlite3.h"

class CppSQLite3Table
{
private:
	int mnCurrentRow;
	int mnRows;
	int mnCols;
	char **mpaszResults;

	void CheckResluts();

public:
	CppSQLite3Table();
	CppSQLite3Table(const CppSQLite3Table &rTable);
	CppSQLite3Table(char **paszResults, int nRows, int nCols);
	~CppSQLite3Table();

	CppSQLite3Table& operator= (const CppSQLite3Table &rTable);

	int NumOfFields();
	int NumOfRows();

	const char* NameOfField(int nField);

	const char* ValueOfField(int nField);
	const char* ValueOfField(const char *szField);

	bool FieldIsNull(int nField);
	bool FieldIsNull(const char *szField);

	bool GetIntField(int nField, int &rDest);
	bool GetIntField(const char *szField, int &rDest);

	bool GetFloatField(int nField, double &rDest);
	bool GetFloatField(const char *szField, double &rDest);

	bool GetStringField(int nField, char *&rDest);
	bool GetStringField(const char *szField, char *&rDset);

	void SetRow(int nRow);

	void finalize();
};