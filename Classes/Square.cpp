//
//  Square.cpp
//  SquareClear
//
//  Created by 徐徐 on 15/11/14.
//  
//

#include "Square.h"

void Square::drawSquare(cocos2d::DrawNode* drawNode, Vec2 squareSize)
{
	Vec2 _leftBottom = Vec2(squareSize.x * this->getIndexX(), squareSize.y * this->getIndexY());
	Vec2 _squareLeftBottom = _leftBottom + Vec2(1, 1);

	Vec2 _rightTop = Vec2(squareSize.x * (this->getIndexX() + 1), squareSize.y * (this->getIndexY() + 1));
	Vec2 _squareRightTop = _rightTop - Vec2(1, 1);

	drawNode->drawSolidRect(_squareLeftBottom, _squareRightTop, this->getColor4F());
	if (this->getFrameColor().a != 0)
	{
		drawNode->drawRect(_leftBottom, _rightTop, this->getFrameColor());
	}
}

void Square::drawSquareWithFrame(cocos2d::DrawNode* drawNode, Vec2 squareSize, Color4F frameColor)
{
	Vec2 _leftBottom = Vec2(squareSize.x * this->getIndexX(), squareSize.y * this->getIndexY());
	Vec2 _squareLeftBottom = _leftBottom + Vec2(1, 1);

	Vec2 _rightTop = Vec2(squareSize.x * (this->getIndexX() + 1), squareSize.y * (this->getIndexY() + 1));
	Vec2 _squareRightTop = _rightTop - Vec2(1, 1);

	drawNode->drawSolidRect(_squareLeftBottom, _squareRightTop, this->getColor4F());
	drawNode->drawRect(_leftBottom, _rightTop, frameColor);

}

void Square::drawFrame(cocos2d::DrawNode* drawNode, Vec2 squareSize, Color4F frameColor)
{
	Vec2 _leftBottom = Vec2(squareSize.x * this->getIndexX(), squareSize.y * this->getIndexY());
	//Vec2 _squareLeftBottom = _leftBottom + Vec2(1, 1);

	Vec2 _rightTop = Vec2(squareSize.x * (this->getIndexX() + 1), squareSize.y * (this->getIndexY() + 1));
	//Vec2 _squareRightTop = _rightTop - Vec2(1, 1);

	//drawNode->drawSolidRect(_squareLeftBottom, _squareRightTop, this->getColor4F());
	drawNode->drawRect(_leftBottom, _rightTop, frameColor);
}

cocos2d::Vec2 Square::getCenterPointInGroup(Vec2 squareSize)
{
	Vec2 _centerPoint = Vec2(squareSize.x * (this->getIndexX() + 0.5), squareSize.y * (this->getIndexY() + 0.5));

	return _centerPoint;	
}
