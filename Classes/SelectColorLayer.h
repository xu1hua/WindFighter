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
#include "Square.h"
typedef std::function<void(Square::SQUARE_COLOR color)> ColorChangeCallback;


class SelectColorLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(SelectColorLayer);
	SelectColorLayer();
	~SelectColorLayer();
	virtual bool init() override;

	void setColorChangeListener(const ColorChangeCallback& callback) { m_colorChangeCallback = callback; }
	Square::SQUARE_COLOR getSelectedColor() const { return m_selectedColor; }
    cocos2d::Size getSquareSize(){return m_colorSquareSize; }

protected:
private:
    //颜色队列
	std::vector<Square * > * m_ColorList;
	cocos2d::DrawNode * m_drawColorList;
    //被选中的颜色
	Square::SQUARE_COLOR m_selectedColor;
	void drawColorLayer();

	ColorChangeCallback m_colorChangeCallback;
	// Overrides
	virtual bool onTouchBegan(Touch *touch, Event *event) override;
	virtual void onTouchMoved(Touch *touch, Event *event) override;
    
    //颜色方块的尺寸
    cocos2d::Size m_colorSquareSize;
};
#include <stdio.h>
#include "cocos2d.h"
#endif // SelectColorLayer_h__
