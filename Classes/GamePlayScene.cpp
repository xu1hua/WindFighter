//
//  GamePlayScene.cpp
//  SquareClear
//
//  Created by 徐徐 on 15/10/19.
//
//

#include "GamePlayScene.h"
#include "GamePlayLayer.h"
#include "MainMenuScene.h"
USING_NS_CC;

Scene* GamePlayScene::createScene(std::string mapName)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = GamePlayScene::create(mapName);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GamePlayScene::init(std::string mapName)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GamePlayScene::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
	auto gamePlayLayer = GamePlayLayer::create(mapName);
	gamePlayLayer->setAnchorPoint(Vec2(0, 0));
	gamePlayLayer->setPosition(Vec2(0, 0));
	this->addChild(gamePlayLayer);
    
    return true;
}


void GamePlayScene::menuCloseCallback(Ref* pSender)
{
	auto scene = Scene::create();
	scene->addChild(MainMenuScene::create());
	Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, scene));

//    Director::getInstance()->end();
//    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
}
