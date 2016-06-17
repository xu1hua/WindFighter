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
			sq->drawSquareWithFrame(m_drawColorList, Vec2(64, 64), Color4F::GREEN);
		}
		else
		{
			sq->drawSquareWithFrame(m_drawColorList, Vec2(64, 64), Color4F::WHITE);
		}
		
	}
}

bool SelectColorLayer::onTouchBegan(Touch *touch, Event *event)
{//todo 2016/6/17
	return false;
}

void SelectColorLayer::onTouchMoved(Touch *touch, Event *event)
{//todo 2016/6/17

}
