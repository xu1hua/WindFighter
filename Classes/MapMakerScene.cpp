#include "MapMakerScene.h"
#include "MainMenuScene.h"
#include "SquareGroup.h"
#include "SquareBaseplateLayer.h"
#include "Language.h"
//#include "storage/local-storage/LocalStorage.h"
#include "GameLibrary/Sqlite3Database/CppSQLite3DB.h"
#include "GameLibrary/UtilityTools/GameUtilityTools.h"
#include "GameLibrary/Sqlite3Database/GameDB.h"
#include "SelectColorLayer.h"
USING_NS_CC;

class SquareGroupMapMaker : public SquareGroup
{
public:
	CREATE_FUNC(SquareGroupMapMaker);

	bool init() override
	{
		if (!SquareGroup::init())
		{
			return false;
		}
		return true;
	}
	virtual void setArrowButtonVisible(bool flag) override
	{
		setRightLeftArrowButtonVisible(flag);
		setUpDownArrowButtonVisible(flag);
	}
};

Scene* MapMakerScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MapMakerScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

MapMakerScene::MapMakerScene()
{

}

MapMakerScene::~MapMakerScene()
{

}

bool MapMakerScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_lastSelectGroup = nullptr;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto s = Director::getInstance()->getWinSize();
	//input map name
	auto pTextField = TextFieldTTF::textFieldWithPlaceHolder(std::string(LocalizedCStringByKey("input_map_name")),
		"fonts/arial.ttf",
		48);
	addChild(pTextField,0,"tbMapName");
	pTextField->setPosition(Vec2(s.width / 2,s.height - 250));

	//输入事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [pTextField](Touch* touch, Event*)
	{
		auto beginPos = touch->getLocation();
		Rect rect;
		rect.size = pTextField->getContentSize();
		auto clicked = isScreenPointInRect(beginPos, Camera::getVisitingCamera(), pTextField->getWorldToNodeTransform(), rect, nullptr);
		if (clicked)
		{
			return true;
		}
		pTextField->detachWithIME();
		return false;
	};
	listener->onTouchEnded = [pTextField](Touch* touch, Event* event)
	{
		auto endPos = touch->getLocation();
		Rect rect;
		rect.size = pTextField->getContentSize();
		auto clicked = isScreenPointInRect(endPos, Camera::getVisitingCamera(), pTextField->getWorldToNodeTransform(), rect, nullptr);
		if (clicked)
		{
			pTextField->attachWithIME();
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//返回主菜单
	auto returnToMainMenuItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MapMakerScene::returnToMainMenuCallback, this)
		);
	returnToMainMenuItem->setPosition(Vec2(origin.x + visibleSize.width - returnToMainMenuItem->getContentSize().width / 2,
		origin.y + returnToMainMenuItem->getContentSize().height / 2));
	auto menu = Menu::create(returnToMainMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
    
	//baseplate
    auto baseplateLayer = SquareBaseplateLayer::create();
    baseplateLayer->createEmptyMap(BaseSize(15,15));
    baseplateLayer->drawBasesplate(Vec2(32,32));
    baseplateLayer->setPosition(Vec2(100,200));
    addChild(baseplateLayer);
    

    //菜单，创建group
    auto menuItemCreateGroup = MenuItemFont::create(LocalizedCStringByKey("create_group"));
    menuItemCreateGroup->setCallback(
    [=](Ref*)
    {
        auto squareGroup = SquareGroupMapMaker::create();
        squareGroup->setPosition(Vec2(100,100));
        squareGroup->SetSquareGroup(Vec2(32,32),SquareGroup::SQUAREGROUP_TYPE::ST_Z,Square::SQUARE_COLOR::SC_GREEN);
        squareGroup->DrawGroup();
		squareGroup->setSelectedListener(
			[=](SquareGroup* sg)
		{
			m_lastSelectGroup = sg;
		});
        this->addChild(squareGroup);
    }
    );
	//菜单，删除group
    auto menuItemDeleteSelectedGroup = MenuItemFont::create(LocalizedCStringByKey("delete_group"));
    menuItemDeleteSelectedGroup->setCallback(
    [=](Ref*)
    {
        for(Node* node:this->getChildren())
        {
			SquareGroupMapMaker* sg = dynamic_cast<SquareGroupMapMaker*>(node);
            if(sg != nullptr)
            {
                if(sg->getGroupState() == SGS_SELECTED)
                {
					assert(m_lastSelectGroup == sg);
					
					m_lastSelectGroup = nullptr;
                    this->removeChild(node);
                }
            }
        }
    }
    );
	//菜单，保存地图
    auto menuItemSaveMap = MenuItemFont::create(LocalizedCStringByKey("save_map"));
    menuItemSaveMap->setCallback(
    [=](Ref*)
    {
		saveMapToFile();
    }
    );
    //创建菜单组
    auto operationMenu = Menu::create(menuItemCreateGroup,menuItemDeleteSelectedGroup,menuItemSaveMap, NULL);
    operationMenu->alignItemsVerticallyWithPadding(20);
    
    addChild(operationMenu);
    operationMenu->setPosition(Vec2(s.width / 2, s.height - 100));
    
	//产生本地图的guid
	m_guid = std::string(GameUilityTools::CreateGuidString());

	//添加选择颜色的层
	auto colorLayer = SelectColorLayer::create();
	colorLayer->setPosition(Vec2(0, 0));
	colorLayer->setColorChangeListener(
		[=](Square::SQUARE_COLOR color){
		//todo xuhua
		if (m_lastSelectGroup)
		{
			m_lastSelectGroup->setSquareGroupColor(color);
			m_lastSelectGroup->DrawGroup();
		}
	}
		);
	addChild(colorLayer, 0);
	return true;
}

void MapMakerScene::returnToMainMenuCallback(cocos2d::Ref* pSender)
{
	auto scene = Scene::create();
	scene->addChild(MainMenuScene::create());
	Director::getInstance()->replaceScene(TransitionFlipY::create(0.5, scene));

}

void MapMakerScene::saveMapToFile()
{
	time_t t = time(0);
	char* mapSaveTimeChar = new char[10];
	sprintf(mapSaveTimeChar, "%ld", t);
	std::string mapName = (getChildByName<TextFieldTTF*>("tbMapName"))->getString();
	std::string path = FileUtils::getInstance()->getWritablePath();
	std::string buf = SquareBaseplateLayer::getInstance()->getMapBuf();

	GameDB::getInstance()->insertMap(m_guid, mapName, buf, "xuhua");




	//localStorageInit(path + "/map");
	////localStorageSetItem(reinterpret_cast<const char*>(data.getBytes()), "1111");
	//std::string mapNameList;
	//if (localStorageGetItem("namelist", &mapNameList))
	//{
	//	mapNameList = mapNameList + "|" + mapName;
	//}
	//else
	//{
	//	mapNameList = mapName;
	//}
	//localStorageSetItem("namelist", mapNameList);
	//std::string buf = SquareBaseplateLayer::getInstance()->getMapBuf();
	//localStorageSetItem(mapName, buf);
	//localStorageFree();
}
