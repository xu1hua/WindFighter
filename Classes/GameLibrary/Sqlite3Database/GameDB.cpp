#include "GameDB.h"
#include "CppSQLite3Query.h"
#include "CppSQLite3DB.h"
#include "CppSQLite3Table.h"
USING_NS_CC;

GameDB* GameDB::s_gameDB = nullptr;

GameDB::GameDB()
{
	m_DBName = "map.db";
	m_DBPath = FileUtils::getInstance()->getWritablePath();
	m_DB = new CppSQLite3DB();
	if (!FileUtils::getInstance()->isFileExist(m_DBPath + m_DBName))
	{
		auto dbFile = FileUtils::getInstance()->fullPathForFilename(m_DBName);
		Data dbData = FileUtils::getInstance()->getDataFromFile(dbFile);
		FileUtils::getInstance()->writeDataToFile(dbData, m_DBPath + m_DBName);
	}
	CCLOG("GameDB database path: %s/%s",m_DBPath.c_str(),m_DBName.c_str());
}

GameDB::~GameDB()
{
	delete m_DB;
}

GameDB* GameDB::getInstance()
{
	if (s_gameDB == nullptr)
	{
		s_gameDB = new GameDB();		
	}
	return s_gameDB;
}

void GameDB::selectMap(std::string uuid)
{
	m_DB->Open(std::string(m_DBPath + "/" + m_DBName).c_str());

	std::ostringstream select_str;
	select_str << "select * from map_table where uuid = \'" << uuid << "\'";

	CppSQLite3Query query = m_DB->ExecQuery(select_str.str().c_str());
	query.Finalize();
	m_DB->Close();

}

bool GameDB::checkMapExist(std::string uuid)
{
	m_DB->Open(std::string(m_DBPath + "/" + m_DBName).c_str());

	std::ostringstream select_str;
	select_str << "select * from map_table where uuid = \'" << uuid << "\'";

	CppSQLite3Query query = m_DB->ExecQuery(select_str.str().c_str());
	bool flag = query.Eof();
	query.Finalize();
	m_DB->Close();
	if (flag)
	{
		return false;
	}
	return true;
}

int GameDB::insertMap(std::string uuid, std::string name, std::string buffer, std::string author)
{
	if (checkMapExist(uuid))
	{
		CCLOG("GameDB::insertMap map:%s exist", uuid.c_str());
		return 0;
	}
	m_DB->Open(std::string(m_DBPath + "/" + m_DBName).c_str());

	std::ostringstream insert_str;
	insert_str << "INSERT INTO map_table (uuid,name,buffer,author) VALUES('"
		<< uuid << "\',\'" << name << "\',\'" << buffer << "\',\'" << author << "\')";
	int result = m_DB->ExecDML(insert_str.str().c_str());
	m_DB->Close();
	if (result != 1)
	{
		CCLOG("GameDB::insertMap insert error");
	}
	return result;
}

int GameDB::updateMap(std::string uuid, std::string buffer)
{
	if (!checkMapExist(uuid))
	{
		CCLOG("GameDB::updateMap map:%s not exist", uuid.c_str());
		return 0;
	}
	m_DB->Open(std::string(m_DBPath + "/" + m_DBName).c_str());

	m_DB->Close();

	int result = 0;

	if (result != 1)
	{
		CCLOG("GameDB::insertMap insert error");
	}
	return result;
}

std::string GameDB::getMapBuffer(std::string uuid)
{
	m_DB->Open(std::string(m_DBPath + "/" + m_DBName).c_str());

	std::ostringstream select_str;
	select_str << "select buffer from map_table where uuid = \'" << uuid << "\'";

	CppSQLite3Table ct = m_DB->GetTable(select_str.str().c_str());
	
	std::string buffer = "";
	if(ct.NumOfRows() == 1)
	{
		ct.SetRow(0);
		char * buf;
		ct.GetStringField(0, buf);
		buffer = buf;
	}
	ct.finalize();
	m_DB->Close();
	return buffer;
}

void GameDB::loopMapTable(const DBLoopMapCallback& callback)
{
	m_DB->Open(std::string(m_DBPath + "/" + m_DBName).c_str());

	std::ostringstream select_str;
	select_str << "select uuid,name,buffer from map_table ";
	CppSQLite3Table ct = m_DB->GetTable(select_str.str().c_str());
	char * uuid, *mapName,*mapBuffer;
	for (int row = 0; row < ct.NumOfRows(); row++)
	{
		ct.SetRow(row);
		ct.GetStringField(0, uuid);
		ct.GetStringField(1, mapName);
		ct.GetStringField(2, mapBuffer);
		callback(uuid, mapName, mapBuffer);
	}

	ct.finalize();
	m_DB->Close();
}

