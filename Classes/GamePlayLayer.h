//
//  GamePlayLayer.h
//  SquareClear
//
//  Created by 徐徐 on 15/11/14.
//  游戏中的主要图层
//

#ifndef GamePlayLayer_h
#define GamePlayLayer_h

#include <stdio.h>
#include "cocos2d.h"
class SquareGroup;
class Square;
class SquareBaseplateLayer;
class GamePlayLayer : public cocos2d::Layer
{
public:
    //CREATE_FUNC(GamePlayLayer);
	static GamePlayLayer* create(std::string mapName)
	{ 
		GamePlayLayer *pRet = new(std::nothrow) GamePlayLayer();
		if (pRet && pRet->init(mapName))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}

	GamePlayLayer();
    ~GamePlayLayer();
    virtual bool init(std::string uuid);
    

    int m_gameBoardHeight;
    int m_gameBoardWidth;


	static cocos2d::Vec2 s_squareSize;
	static GamePlayLayer* getInstance(){ return s_pGamePlayLayer; }
	SquareBaseplateLayer* getSquareBaseplateLayer() const { return m_pSquareBaseplateLayer; }
	
protected:
    //void drawSquare();
    //void drawSquareGroup(SquareGroup* sg,int x,int y);
    //void drawOneSquare(int x,int y,Square* sq);
	
    int m_squareRowCount = 20;
    int m_squareColumnCount = 20;
    
	SquareBaseplateLayer* m_pSquareBaseplateLayer;
    //游戏板的位置
    //TODO
    int* m_BackgroundBoard;
    cocos2d::DrawNode * m_drawNode;

	static GamePlayLayer* s_pGamePlayLayer;
};
#endif /* GamePlayLayer_h */
