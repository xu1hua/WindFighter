#include "SelectColorLayer.h"
#include "Square.h"
USING_NS_CC;
SelectColorLayer::SelectColorLayer()
{
	m_ColorList = new std::vector<Square * >();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SelectColorLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SelectColorLayer::onTouchMoved, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	listener->setSwallowTouches(true);
    
    m_colorSquareSize = Size(64,64);
}

SelectColorLayer::~SelectColorLayer()
{
	for (auto square : *m_ColorList)
	{
		delete square;
	}
	m_ColorList->clear();
	delete m_ColorList;

	m_drawColorList->clear();
}

bool SelectColorLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	for (int i = 0; i < Square::SC_MAX; i++)
	{
		m_ColorList->push_back(new Square(i,0,(Square::SQUARE_COLOR)i));
	}
	m_selectedColor = Square::SQUARE_COLOR(0);
	m_drawColorList = DrawNode::create();
	addChild(m_drawColorList);
	
	drawColorLayer();

	return true;
}

void SelectColorLayer::drawColorLayer()
{
	m_drawColorList->clear();
	for (auto sq : *m_ColorList)
	{
		if (sq->GetColor() == m_selectedColor)
		{
			sq->drawSquareWithFrame(m_drawColorList, getSquareSize(), Color4F::GREEN);
		}
		else
		{
			sq->drawSquareWithFrame(m_drawColorList, getSquareSize(), Color4F::WHITE);
		}
		
	}
}

bool SelectColorLayer::onTouchBegan(Touch *touch, Event *event)
{
	//获得在本节点内的坐标
    Vec2 pt2 = this->convertTouchToNodeSpace(touch);
    for (auto sq : *m_ColorList)
    {
        Vec2 pos = sq->calcPosInGroup(getSquareSize());
        Rect rect = Rect(pos, getSquareSize());
        if (rect.containsPoint(pt2))
        {
            CCLOG("COLOR TOUCH TEST:%d,%d", sq->getIndexX(),sq->getIndexY());
            m_selectedColor = sq->GetColor();
            if(m_colorChangeCallback)
            {
                m_colorChangeCallback(m_selectedColor);
            }
            drawColorLayer();
            return true;
        }
    }

	return false;
}

void SelectColorLayer::onTouchMoved(Touch *touch, Event *event)
{
	//获得在本节点内的坐标
    Vec2 pt2 = this->convertTouchToNodeSpace(touch);
    for (auto sq : *m_ColorList)
    {
        Vec2 pos = sq->calcPosInGroup(getSquareSize());
        Rect rect = Rect(pos, getSquareSize());
        if (rect.containsPoint(pt2))
        {
            if(m_selectedColor != sq->GetColor())
            {
                CCLOG("COLOR TOUCH TEST:%d,%d", sq->getIndexX(),sq->getIndexY());
                m_selectedColor = sq->GetColor();
                if(m_colorChangeCallback)
                {
                    m_colorChangeCallback(m_selectedColor);
                }

                drawColorLayer();
            }
        }
    }

}
