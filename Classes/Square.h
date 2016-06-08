//
//  Square.hpp
//  SquareClear
//
//  Created by 徐徐 on 15/11/14.
//  单个方块类，包含方块的颜色和坐标信息
//

#ifndef Square_h
#define Square_h

#include "cocos2d.h"
#include <stdio.h>
USING_NS_CC;
Color4F const s_squareColor[] =
{
    Color4F(1,0,0,1),
    Color4F(0,1,0,1),
    Color4F(0,0,1,1),
    Color4F(0,0,0,1),
    Color4F(1,1,1,1)
//    0xFF0000,
//    0x00FF00,
//    0x0000FF,
//    0x000000,
//    0xFFFFFF
};

class Square
{
public:
    //方块的颜色枚举
    enum SQUARE_COLOR
    {
        SC_NONE = -1,   //没有颜色
        SC_RED = 0,     //红
        SC_GREEN,
        SC_BLUE,
        SC_BLACK,
        SC_WHITE,
        //SC_OTHER,       //自定义颜色
        SC_MAX
    };
    Square()
    {
        _indexX = -1;
        _indexY = -1;
        _colorIndex = SC_NONE;
        _color4F = Color4F(0,0,0,0);
		_frameColor = Color4F(0, 0, 0, 0);
    }
    ~Square(){};
    Square(int x,int y,SQUARE_COLOR color,float alpha = 1)
    {
        _indexX = x;
        _indexY = y;
        _colorIndex = color;
        _color4F = s_squareColor[_colorIndex];
        _color4F.a = alpha;
    }
	void setXYIndex(int x_index, int y_index)
	{
		_indexX = x_index;
		_indexY = y_index;
	}

    SQUARE_COLOR GetColor()
    {
        return _colorIndex;
    }
    Color4F getColor4F()
    {
        return _color4F;
    }
    void SetColor(SQUARE_COLOR color)
    {
        _colorIndex = color;
        float alpha = _color4F.a;
        _color4F = s_squareColor[_colorIndex];
        _color4F.a = alpha;
    }
    void SetAlpha(float a)
    {
        _color4F.a = a;
    }

	//计算在group中的相对位置，返回左下角的位置
	cocos2d::Vec2 calcPosInGroup(Vec2 squareSize);
	void drawSquare(cocos2d::DrawNode* drawNode, Vec2 squareSize);
	void drawSquareWithFrame(cocos2d::DrawNode* drawNode, Vec2 squareSize, Color4F frameColor);
	void drawFrame(cocos2d::DrawNode* drawNode, Vec2 squareSize, Color4F frameColor);
	//计算sqaure的中心点在group中的坐标
	Vec2 getCenterPointInGroup(Vec2 squareSize);

	CC_SYNTHESIZE(int, _indexX, IndexX);
	CC_SYNTHESIZE(int, _indexY, IndexY);
	CC_SYNTHESIZE(Color4F, _frameColor, FrameColor);
private:

    SQUARE_COLOR _colorIndex;
    Color4F _color4F;

    
};


#endif /* Square_h */
