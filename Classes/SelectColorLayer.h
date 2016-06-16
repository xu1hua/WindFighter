//
//  SelectColorLayer.h
//  
//
//  Created by 徐徐 on 16/6/16.
//  用来选择颜色的图层
//

#ifndef SelectColorLayer_h__
#define SelectColorLayer_h__
#include <stdio.h>
#include "cocos2d.h"
class Square;
class SelectColorLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(SelectColorLayer);
	SelectColorLayer();
	~SelectColorLayer();
	virtual bool init();

protected:
private:
	std::vector<Square * > * m_ColorList;
	cocos2d::DrawNode * m_drawColorList;
	void drawColorLayer();

};
#include <stdio.h>
#include "cocos2d.h"
#endif // SelectColorLayer_h__
