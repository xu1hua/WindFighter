//
//  MainMenuLayer.h
//  SquareClear
//
//  Created by 徐徐 on 16/5/12.
//  游戏的主菜单Layer
//

#ifndef MainMenuLayer_h
#define MainMenuLayer_h

#include <stdio.h>
#include "cocos2d.h"

class MainMenuLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(MainMenuLayer);
    
    MainMenuLayer();
    ~MainMenuLayer();

    virtual bool init() override;
    
};

class SelectLevelMenuLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(SelectLevelMenuLayer);

	SelectLevelMenuLayer();
	~SelectLevelMenuLayer();

	virtual bool init() override;


};


#endif /* MainMenuLayer_h */
