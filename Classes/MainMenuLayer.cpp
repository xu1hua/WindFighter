//
//  MainMenuLayer.cpp
//  SquareClear
//
//  Created by 徐徐 on 16/5/12.
//
//

#include "MainMenuLayer.h"
#include "ui/UIButton.h"
#include "Language.h"
#include "GamePlayScene.h"
#include "MapMakerScene.h"
//#include "storage/local-storage/LocalStorage.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"
#include "SquareBaseplateLayer.h"
#include "GameLibrary/Sqlite3Database/GameDB.h"
USING_NS_CC;
MainMenuLayer::MainMenuLayer()
{
    
}

MainMenuLayer::~MainMenuLayer()
{
    
}


bool MainMenuLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
	MenuItemFont::setFontName("fonts/arial.ttf");
	MenuItemFont::setFontSize(40);

	auto menuItemSelectMap = MenuItemFont::create(std::string(LocalizedCStringByKey("start_game")));
	menuItemSelectMap->setCallback(
		[=](Ref*)
	{

		static_cast<LayerMultiplex*>(_parent)->switchTo(1);
	}
		);


	auto menuItemMapMaker = MenuItemFont::create(std::string(LocalizedCStringByKey("map_maker")));
	menuItemMapMaker->setCallback(
		[](Ref*)
	{
		auto scene = Scene::create();
		scene->addChild(MapMakerScene::create());
		Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, scene));

	}
	);


	auto menu = Menu::create(menuItemSelectMap, menuItemMapMaker, nullptr);
	menu->alignItemsVerticallyWithPadding(20);

	auto s = Director::getInstance()->getWinSize();
	addChild(menu);
	menu->setPosition(Vec2(s.width / 2, s.height / 2));
	


    return true;
}


SelectLevelMenuLayer::SelectLevelMenuLayer()
{

}
SelectLevelMenuLayer::~SelectLevelMenuLayer()
{

}
//注意：当字符串为空时，也会返回一个空字符串  
void split(std::string& s, std::string& delim, std::vector< std::string >* ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		ret->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		ret->push_back(s.substr(last, index - last));
	}
}
bool SelectLevelMenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	MenuItemFont::setFontName("fonts/arial.ttf");
	MenuItemFont::setFontSize(40);
	Vector<MenuItem*>* menuItemList =new Vector < MenuItem* >() ;

	auto menuItemBack = MenuItemFont::create(std::string(LocalizedCStringByKey("back")));
	menuItemBack->setCallback(
		[=](Ref*)
	{

		static_cast<LayerMultiplex*>(_parent)->switchTo(0);
	}
	);
	menuItemList->pushBack(menuItemBack);

	GameDB::getInstance()->loopMapTable(
		[menuItemList](std::string uuid, std::string mapName, std::string mapBuffer)
	{
		rapidjson::Document _json;
		_json.Parse<0>(mapBuffer.c_str());
		if (_json.HasParseError())
		{
			CCLOG("SelectLevelMenuLayer::init() _json.HasParseError() mapName:%s,uuid:%s", mapName.c_str(),uuid.c_str());
		}
		else
		{
			if (_json.IsObject())
			{
				auto _mapName = mapName == "" ? uuid : mapName;
				auto menuItemMap = MenuItemFont::create(_mapName);
				menuItemMap->setCallback(
					[uuid](Ref * ref)
				{
					auto scene = Scene::create();
					scene->addChild(GamePlayScene::create(uuid));
					Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, scene));
				}
				);
				menuItemList->pushBack(menuItemMap);
			}
			else
			{
				//localStorageRemoveItem(mapName);
			}
		}
	}
		);

//	std::string path = FileUtils::getInstance()->getWritablePath();	
////	localStorageInit(path + "/map");
//	std::string mapNameList;
////	if (localStorageGetItem("namelist", &mapNameList))
//	{
//		std::vector< std::string >* namelist = new std::vector< std::string >();
//		std::string splitStr = "|";
//		split(mapNameList, splitStr, namelist);
//		for (auto mapName : *namelist)
//		{
//			std::string _mapBuffer;
////			if (localStorageGetItem(mapName, &_mapBuffer))
//			{
//				rapidjson::Document _json;
//				_json.Parse<0>(_mapBuffer.c_str());
//				if (_json.HasParseError())
//				{
////					localStorageRemoveItem(mapName);
//				}
//				else
//				{
//					if (_json.IsObject())
//					{
//						auto menuItemMap = MenuItemFont::create(mapName);
//						menuItemMap->setCallback(
//							[mapName](Ref * ref)
//						{
//							auto scene = Scene::create();
//							scene->addChild(GamePlayScene::create(mapName));
//							Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, scene));
//						}
//							);
//						menuItemList.pushBack(menuItemMap);
//					}
//					else
//					{
//						//localStorageRemoveItem(mapName);
//					}
//				}
//				//SquareBaseplateLayer::getInstance()->readMapBuf(_mapBuffer);
//			}
//			//else
//			//{
//			//	localStorageRemoveItem(mapName);
//			//}
//		}
//		namelist->clear();
//		delete namelist;
//		
//	}
//	localStorageFree();

	auto menu = Menu::createWithArray(*menuItemList);
	menuItemList->clear();
	auto s = Director::getInstance()->getWinSize();
	addChild(menu);
	menu->setPosition(Vec2(s.width / 2, s.height / 2));
	menu->alignItemsVerticallyWithPadding(20);
	return true;
}