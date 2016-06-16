#include "SelectColorLayer.h"
#include "Square.h"
SelectColorLayer::SelectColorLayer()
{
	m_ColorList = new std::vector<Square * >();
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
		sq->drawSquareWithFrame(m_drawColorList, Vec2(64, 64), Color4F::WHITE);
	}
}
