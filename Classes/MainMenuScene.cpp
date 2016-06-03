#include "MainMenuScene.h"
#include "MainMenuLayer.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MainMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

    auto mainMenuLayer = MainMenuLayer::create();
	auto selectLevelMenuLayer = SelectLevelMenuLayer::create();
	auto layMulitplex = LayerMultiplex::create(mainMenuLayer, selectLevelMenuLayer,nullptr);
	
	addChild(layMulitplex);
	//mainMenuLayer->release();
	//selectLevelMenuLayer->release();
    return true;
}