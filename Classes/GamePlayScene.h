//
//  GamePlayScene.h
//  SquareClear
//
//  Created by 徐徐 on 15/10/19.
//
//

#ifndef __SquareClear__GamePlayScene__
#define __SquareClear__GamePlayScene__

#include <stdio.h>
#include "cocos2d.h"

class GamePlayScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(std::string mapName);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(std::string mapName);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    //CREATE_FUNC(GamePlayScene);
	static GamePlayScene* create(std::string mapName)
	{
		GamePlayScene *pRet = new(std::nothrow) GamePlayScene();
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
    
};
#endif /* defined(__SquareClear__GamePlayScene__) */
