//
//  GamePlayLayer.cpp
//  SquareClear
//
//  Created by 徐徐 on 15/11/14.
//
//

#include "GamePlayLayer.h"
#include "SquareGroup.h"
#include "SquareBaseplateLayer.h"
//#include "storage/local-storage/LocalStorage.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"
#include "GameLibrary/Sqlite3Database/GameDB.h"
#include "Language.h"
USING_NS_CC;

Vec2 GamePlayLayer::s_squareSize = Vec2(32, 32);
GamePlayLayer* GamePlayLayer::s_pGamePlayLayer = NULL;
bool GamePlayLayer::init(std::string uuid)
{
    if(!Layer::init())
    {
        return false;
    }
	m_gameBoardHeight = m_squareRowCount * s_squareSize.y;
	m_gameBoardWidth = m_squareColumnCount * s_squareSize.x;
    m_BackgroundBoard = new int[m_squareRowCount * m_squareColumnCount];
    //test
    for(int i = 0;i < m_squareRowCount * m_squareColumnCount; i++)
    {
        m_BackgroundBoard[i] = 0;
        if(i > 14 && i < 100)
        {
            m_BackgroundBoard[i] = 1;
        }
    }
    
    auto s = Director::getInstance()->getWinSize();
    
    m_drawNode = DrawNode::create();
    addChild(m_drawNode, 10);

	m_winLabel = Label::createWithTTF("Success!!", "fonts/arial.ttf",48);
	m_winLabel->setPosition(Vec2(150,900));
	m_winLabel->setVisible(false);
	addChild(m_winLabel);
	m_pSquareBaseplateLayer = SquareBaseplateLayer::create();
	m_pSquareBaseplateLayer->setPosition(Vec2(50, 300));
    //m_pSquareBaseplateLayer->readMapBufTest();

	std::string path = FileUtils::getInstance()->getWritablePath();
//	localStorageInit(path + "/map");

	std::string _mapBuffer;

	_mapBuffer = GameDB::getInstance()->getMapBuffer(uuid);
	m_pSquareBaseplateLayer->readMapBuf(_mapBuffer);
	m_pSquareBaseplateLayer->setWinListener([=]()
	{
		m_winLabel->setVisible(true);
	});
//	localStorageFree();
	addChild(m_pSquareBaseplateLayer, 0);

	rapidjson::Document _json;
	_json.Parse<0>(_mapBuffer.c_str());
	rapidjson::Value &groupListValue = _json["group_list"];
	for (rapidjson::SizeType i = 0; i < groupListValue.Size(); i++)
	{
		rapidjson::Value& object = groupListValue[i];
		rapidjson::Value& groupType = object["group_type"];
		rapidjson::Value& groupColor = object["group_color"];

		auto sg = SquareGroup::create();
		sg->SetSquareGroup(s_squareSize, SquareGroup::SQUAREGROUP_TYPE(groupType.GetInt()), Square::SQUARE_COLOR(groupColor.GetInt()));
		sg->setPosition(Vec2((i %5) * s_squareSize.x * 4,  (i/5) * s_squareSize.y * 4));
		sg->setSquareGroupColor((Square::SQUARE_COLOR)groupColor.GetInt());
		sg->DrawGroup();
		addChild(sg, 1);

	}
	

	
	m_pSquareBaseplateLayer->drawBasesplate(s_squareSize);
    return true;
}
GamePlayLayer::GamePlayLayer()
{
	if (s_pGamePlayLayer == NULL)
	{
		s_pGamePlayLayer = this;
	}
	else
	{
		assert(0);
	}
}
GamePlayLayer::~GamePlayLayer()
{
    delete[] m_BackgroundBoard;
	s_pGamePlayLayer = NULL;
	m_pSquareBaseplateLayer = NULL;
}
//void GamePlayLayer::drawSquare()
//{
//    for(int i = 0;i < m_squareRowCount * m_squareColumnCount; i++)
//    {
//		float _x = (i % m_squareColumnCount) * s_squareSize.x;
//		float _y = (i / m_squareColumnCount) * s_squareSize.y;
//        if(m_BackgroundBoard[i] == 0)
//        {
//			m_drawNode->drawSolidRect(Vec2(_x, _y), Vec2(_x + s_squareSize.x, _y + s_squareSize.y), Color4F(1, 0, 0, 1));
//        }
//        else
//        {
//			m_drawNode->drawSolidRect(Vec2(_x, _y), Vec2(_x + s_squareSize.x, _y + s_squareSize.y), Color4F(0, 1, 0, 1));
//        }
//    }
//}
//
//
//void GamePlayLayer::drawSquareGroup(SquareGroup* sg,int x,int y)
//{
//    SquareMap* sm = sg->getGroupArray();
//    SquareMapIterator smit;
//    for(smit = sm->begin(); smit!=sm->end(); smit++)
//    {
//        drawOneSquare(x, y, smit->second);
//    }
//    
//}
//
//void GamePlayLayer::drawOneSquare(int x,int y,Square* sq)
//{
//	Vec2 _x = Director::getInstance()->convertToGL(Vec2(x + s_squareSize.x * sq->getIndexX(), y + s_squareSize.y * sq->getIndexY()));
//	Vec2 _y = Director::getInstance()->convertToGL(Vec2(x + s_squareSize.x * (sq->getIndexX() + 1), y + s_squareSize.y * (sq->getIndexY() + 1)));
//    m_drawNode->drawSolidRect(_x, _y, sq->getColor4F());
//}

