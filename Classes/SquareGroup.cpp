//
//  SquareGroup.cpp
//  SquareClear
//
//  Created by 徐徐 on 15/10/18.
//
//

#include "SquareGroup.h"
#include "ui/UIButton.h"
#include "GamePlayLayer.h"
#include "SquareBaseplateLayer.h"

int const SquareGroup::s_Width = 4;
int const SquareGroup::s_Height = 4;
int const s_shapeList[SquareGroup::ST_MAX][SquareGroup::s_Width * SquareGroup::s_Height] =
{
    {
        0,1,0,0,
        0,1,0,0,
        0,1,1,0,
        0,0,0,0
    },
    {
        0,0,1,0,
        0,0,1,0,
        0,1,1,0,
        0,0,0,0
    },
    {
        0,0,0,0,
        1,1,0,0,
        0,1,1,0,
        0,0,0,0
    },
    {
        0,0,0,0,
        0,1,1,0,
        1,1,0,0,
        0,0,0,0
    }
};
bool SquareGroup::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_groupArray = new std::vector<SquareInSquareGroup>;

	m_drawNode = DrawNode::create();
	addChild(m_drawNode, 1);

	//左箭头
	m_arrowButtonLeft = cocos2d::ui::Button::create("ButtonArrowOrangeLeft.png");
	m_arrowButtonLeft->setAnchorPoint(Vec2(1,0.5));
	m_arrowButtonLeft->setPosition(Vec2(-m_squareSize.x,m_squareSize.y * 2));
	m_arrowButtonLeft->addTouchEventListener(
		[=](Ref*, cocos2d::ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				TurnLeft();
				DrawGroup();
			}
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:

				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
			case cocos2d::ui::Widget::TouchEventType::CANCELED:

				break;
			default:
				break;
			}
		}
	);
	addChild(m_arrowButtonLeft);
	//m_arrowButtonLeft->setVisible(true);
	//右箭头
	m_arrowButtonRight = cocos2d::ui::Button::create("ButtonArrowOrangeRight.png");
	m_arrowButtonRight->setAnchorPoint(Vec2(0,0.5));
	m_arrowButtonRight->setPosition(Vec2(m_squareSize.x * (s_Width + 1), m_squareSize.y * 2));
	m_arrowButtonRight->addTouchEventListener(
		[=](Ref*, cocos2d::ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			TurnRight();
			DrawGroup();
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:

			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		case cocos2d::ui::Widget::TouchEventType::CANCELED:

			break;
		default:
			break;
		}
	}
	);
	addChild(m_arrowButtonRight);
	//m_arrowButtonRight->setVisible(true);
	setRightLeftArrowButtonVisible(false);

	//上箭头
	m_arrowButtonUpward = cocos2d::ui::Button::create("ButtonArrowOrangeUp.png");
	m_arrowButtonUpward->setAnchorPoint(Vec2(0.5, 0));
	m_arrowButtonUpward->setPosition(Vec2(m_squareSize.x * (s_Width/2), m_squareSize.y * (s_Height +1)));
	m_arrowButtonUpward->addTouchEventListener(
		[=](Ref*, cocos2d::ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			SQUAREGROUP_TYPE _type = getGroupType();
			_type = SQUAREGROUP_TYPE((int)_type - 1);
			if (_type <= ST_NONE)
			{
				_type = SQUAREGROUP_TYPE(ST_MAX - 1);
			}
			SetGroupType(_type,getGroupColor());
			DrawGroup();
			setBaseplateFrameByGroup();
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:

			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		case cocos2d::ui::Widget::TouchEventType::CANCELED:

			break;
		default:
			break;
		}
	}
	);
	addChild(m_arrowButtonUpward);

	//下箭头
	m_arrowButtonDownward = cocos2d::ui::Button::create("ButtonArrowOrangeDown.png");
	m_arrowButtonDownward->setAnchorPoint(Vec2(0.5, 1));
	m_arrowButtonDownward->setPosition(Vec2(m_squareSize.x * (s_Width / 2), m_squareSize.y * (-1)));
	m_arrowButtonDownward->addTouchEventListener(
		[=](Ref*, cocos2d::ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			SQUAREGROUP_TYPE _type = getGroupType();
			_type = SQUAREGROUP_TYPE((int)_type + 1);
			if (_type >= ST_MAX)
			{
				_type = SQUAREGROUP_TYPE(ST_NONE + 1);
			}
			SetGroupType(_type, getGroupColor());
			DrawGroup();
			setBaseplateFrameByGroup();
		}
		break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:

			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		case cocos2d::ui::Widget::TouchEventType::CANCELED:

			break;
		default:
			break;
		}
	}
	);
	addChild(m_arrowButtonDownward);
	setUpDownArrowButtonVisible(false);


	return true;
}


SquareGroup::SquareGroup()
: _groupState(SquareGroupState::SGS_ORIGIN)
, _groupType(ST_NONE)
, _groupColor(Square::SC_BLACK)
{
	m_squareSize = GamePlayLayer::s_squareSize;
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(SquareGroup::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SquareGroup::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SquareGroup::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(SquareGroup::onTouchCancelled, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	listener->setSwallowTouches(true);
}

SquareGroup::~SquareGroup()
{
	for (auto sq : *m_groupArray)
	{
		delete sq.square;
	}
    m_groupArray->clear();
    delete m_groupArray;
    
}

void SquareGroup::SetSquareGroup(Vec2 squareSize, SquareGroup::SQUAREGROUP_TYPE type, Square::SQUARE_COLOR color /*= Square::SC_BLACK*/)
{
	SetSquareSize(squareSize);
	SetGroupType(type, color);
	
}

void SquareGroup::SetGroupType(SquareGroup::SQUAREGROUP_TYPE type, Square::SQUARE_COLOR color /*= Square::SC_BLACK*/)
{
    _groupType = type;
    CalcGroup(color);
}

void SquareGroup::CalcGroup(Square::SQUARE_COLOR color /*= Square::SC_BLACK*/)
{
	for (auto sq : *m_groupArray)
	{
		delete sq.square;
	}
	m_groupArray->clear();

	setGroupColor(color);
    assert(_groupType > ST_NONE && _groupType < ST_MAX);
    for(int i = 0; i < s_Width * s_Height; i++)
    {
        if(s_shapeList[_groupType][i] == 1)
        {//上下翻转，因为gl坐标原点在左下。
            m_groupArray->push_back(SquareInSquareGroup(new Square(i % s_Width, s_Height - 1 - i / s_Height, color)));
        }   
    }
}

void SquareGroup::DrawGroup()
{
	m_drawNode->clear();
    for (auto &sq : *m_groupArray)
    {
		//drawOneSquare(m_squareSize, sq.second);
		if (getGroupState() == SGS_SELECTED)
		{
			sq.square->drawSquareWithFrame(m_drawNode, m_squareSize,Color4F::BLUE);
		}
		else
		{
			sq.square->drawSquare(m_drawNode, m_squareSize);
		}

    }
}

void SquareGroup::DrawGroup(Vec2 squareSize)
{
	SetSquareSize(squareSize);
    DrawGroup();


}

bool SquareGroup::checkTouchInSelf_Parent(Touch *touch)
{
    //方案三
    //获得点击的OpenGL的世界坐标值
    Vec2 touchPoint = touch->getLocation();
    //将点击的位置转换成父元素坐标系中的相对坐标
    Vec2 pt = getParent()->convertToNodeSpace(touchPoint);
    //printf("pt.x=%.1f, pt.y=%.1f\n", pt.x, pt.y);
    //得到自己在父元素坐标系中的位置范围
    Rect rect=getBoundingBox();
    //printf("rect.l=%.1f, rect.b=%.1f, rect.r=%.1f, rect.t=%.1f\n",\
           rect.getMinX(), rect.getMinY(), rect.getMaxX(), rect.getMaxY());
    //判断是否点击落在自己的范围当中， 以上判断全是在父元素坐标系中进行计算
    if(rect.containsPoint(pt))
    {
        //printf("ccTouchBegan x=%.1f y=%.1f\n", touchPoint.x, touchPoint.y);
        return true;
    }
    return false;
}

bool SquareGroup::onTouchBegan(Touch *touch, Event *event)
{
    if(!checkTouchInSelf_Parent(touch))
    {
        if(getGroupState() == SGS_SELECTED)
        {
			
			setArrowButtonVisible(false);

			
			Vec2 worldPlacePos, localSquarePos;
			if (checkGroupCanPlaced())
			{
				for (auto &sq : *m_groupArray)
				{
					SquareBaseplateLayer::getInstance()->SetSquarePlaced(sq.indexInBaseplate);
				}
				auto sq = (*m_groupArray)[0];
				{
					//将group放入baseplate
					worldPlacePos = SquareBaseplateLayer::getInstance()->getWorldPos(sq.indexInBaseplate);

					localSquarePos = Vec2(sq.square->getIndexX() * m_squareSize.x
						, (sq.square->getIndexY()) * m_squareSize.y);
					
				}
				setPosition(worldPlacePos - localSquarePos);
				setGroupState(SGS_PLACED);
			}
			else
			{
				setGroupState(SGS_UNSELECTED);
			}

        }
		DrawGroup();
        return false;
    }
	
	if(calcSquareIsTouched(touch))
	{//如果点中group中的方块，才认为是选中的group
		setArrowButtonVisible(true);
		setBaseplateFrameByGroup(getGroupState() == SGS_PLACED);
		setGroupState(SGS_SELECTED);
		DrawGroup();
		return true;
	}

	return false;
    
    
}

bool SquareGroup::checkGroupCanPlaced()
{
	for (auto &sq : *m_groupArray)
	{

		if (sq.indexInBaseplate == -1)
		{
			return false;
			break;
		}
		SquareBaseplateState state = SquareBaseplateLayer::getInstance()->getSquareState(sq.indexInBaseplate);
		if (state != SQBS_EMPTY && state != SQBS_FRAME)
		{
			return false;
			break;
		}
	}
	return true;
}

bool SquareGroup::calcSquareIsTouched(Touch* touch)
{//todo
	//获得在本节点内的坐标
	Vec2 pt2 = this->convertTouchToNodeSpace(touch);
	for (auto sqs : *m_groupArray)
	{
		Vec2 pos = sqs.square->calcPosInGroup(m_squareSize);
		Rect rect = Rect(pos, Size(m_squareSize.x, m_squareSize.y));
		if (rect.containsPoint(pt2))
		{
			CCLOG("TOUCH TEST:%d,%d", sqs.square->getIndexX(),sqs.square->getIndexY());
			return true;
		}
	}
	return false;
}

void SquareGroup::onTouchMoved(Touch *touch, Event *event)
{
    if(getGroupState() != SGS_SELECTED)
    {
        return;
    }
//    if(!checkTouchInSelf_Parent(touch))
//    {
//        return;
//    }
    //获得点击的OpenGL的世界坐标值
    Vec2 touchPoint = touch->getLocation();
    //将点击的位置转换成父元素坐标系中的相对坐标
    Vec2 pt = getParent()->convertToNodeSpace(touchPoint);
	pt.x -= m_squareSize.x * 2;
    //pt.y -= m_squareHeight * 2;
    setPosition(pt);

	setBaseplateFrameByGroup();

}

void SquareGroup::onTouchEnded(Touch *touch, Event *event)
{
	if (getGroupState() != SGS_SELECTED)
    {
        return;
    }
    //m_drawNode->clear();
    //DrawGroup();
    
    return;
}

void SquareGroup::onTouchCancelled(Touch *touch, Event *event)
{
	if (getGroupState() != SGS_SELECTED)
    {
        return;
    }
    
    if(!checkTouchInSelf_Parent(touch))
    {
        return;
    }
    //setIsSelected(false);
    
    return;
}


void SquareGroup::TurnLeft()
{
	for (auto &sq : *m_groupArray)
	{
		sq.square->setXYIndex(s_Width - sq.square->getIndexY() - 1, sq.square->getIndexX());
	}

	setBaseplateFrameByGroup();

}

void SquareGroup::TurnRight()
{
	for (auto &sq : *m_groupArray)
	{
		sq.square->setXYIndex(sq.square->getIndexY(), s_Height - sq.square->getIndexX() - 1);
	}
	setBaseplateFrameByGroup();
}

void SquareGroup::SetSquareSize(Vec2 size)
{
	m_squareSize = size;
	setContentSize(Size(m_squareSize.x * SquareGroup::s_Width, m_squareSize.y * SquareGroup::s_Height));
	m_arrowButtonLeft->setPosition(Vec2(-m_squareSize.x, m_squareSize.y * 2));
	m_arrowButtonRight->setPosition(Vec2(m_squareSize.x * (s_Width + 1), m_squareSize.y * 2));
	m_arrowButtonUpward->setPosition(Vec2(m_squareSize.x * (s_Width / 2), m_squareSize.y * (s_Height + 1)));
	m_arrowButtonDownward->setPosition(Vec2(m_squareSize.x * (s_Width / 2), m_squareSize.y * (-1)));

}

void SquareGroup::setArrowButtonVisible(bool flag)
{
	setRightLeftArrowButtonVisible(flag);
}

void SquareGroup::setRightLeftArrowButtonVisible(bool flag)
{
	m_arrowButtonLeft->setVisible(flag);
	m_arrowButtonRight->setVisible(flag);
}

void SquareGroup::setUpDownArrowButtonVisible(bool flag)
{
	m_arrowButtonUpward->setVisible(flag);
	m_arrowButtonDownward->setVisible(flag);
}

void SquareGroup::setBaseplateFrameByGroup(bool force)
{
	SquareBaseplateLayer::getInstance()->clearFrameSquare();
	for (auto &sq : *m_groupArray)
	{
		Vec2 centerPos = sq.square->getCenterPointInGroup(m_squareSize);
		sq.indexInBaseplate = SquareBaseplateLayer::getInstance()->
			getIndexByPos(getParent()->convertToWorldSpace(getPosition() + centerPos));

		if (sq.indexInBaseplate != -1 
			&& (SquareBaseplateLayer::getInstance()->getSquareState(sq.indexInBaseplate) != SQBS_SQUARE
			|| force)
			)
		{
			SquareBaseplateLayer::getInstance()->setSquareFrame(sq.indexInBaseplate);
		}
	}
	//SquareBaseplateLayer::getInstance()->drawBasesplate(m_squareSize);
}