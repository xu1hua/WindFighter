//
//	MapMakerScene.h
//  SquareClear
//
//  Created by 徐徐 on 16/5/13.
//  制作地图的scene
//

#ifndef MapMakerScene_h__
#define MapMakerScene_h__

#include "cocos2d.h"
class SquareGroup;
class MapMakerScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

    MapMakerScene();
    ~MapMakerScene();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void returnToMainMenuCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MapMakerScene);

private:
	void saveMapToFile();
	std::string m_guid;
	SquareGroup* m_lastSelectGroup;
};
#endif // MapMakerScene_h__