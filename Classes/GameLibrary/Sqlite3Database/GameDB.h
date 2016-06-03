#ifndef _GAMEDB_H_
#define _GAMEDB_H_
#include <stdio.h>
#include "cocos2d.h"

class CppSQLite3Query;
class CppSQLite3DB;

typedef std::function<void(std::string uuid, std::string mapName,std::string mapBuffer)> DBLoopMapCallback;

class GameDB
{
private:
	GameDB();	
	static GameDB* s_gameDB;
	std::string m_DBName;
	std::string m_DBPath;
	CppSQLite3DB * m_DB;
public:
	~GameDB();
	static GameDB* getInstance();
	std::string getDBName(){ return m_DBName; }
	std::string getDBPath(){ return m_DBPath; }

	void selectMap(std::string uuid);
	bool checkMapExist(std::string uuid);
	int insertMap(std::string uuid, std::string name, std::string buffer, std::string author);
	int updateMap(std::string uuid, std::string buffer);

	std::string getMapBuffer(std::string uuid);

	void loopMapTable(const DBLoopMapCallback& callback);
};
#endif