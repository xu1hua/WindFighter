//
//
//
//	Created by xuhua on 16/5/10
//  游戏的基板类，将方块置于基板的空格中
//
//

#include "SquareBaseplateLayer.h"
#include "Square.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"
#include "SquareGroup.h"
USING_NS_CC;
int baselist[] = 
{
	1, 1, 1, 1, 1, 1,
	1, 1, 0, 1, 1, 1,
	1, 1, 0, 1, 1, 1,
	1, 1, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1,
};

SquareBaseplateLayer* SquareBaseplateLayer::s_pSquareBaseplateLayer = NULL;
SquareBaseplateLayer::SquareBaseplateLayer()
:_baseSize(BaseSize(1,1))
,_squareSize(Vec2(1,1))
{
	if (s_pSquareBaseplateLayer)
	{
		assert(0);
	}
	s_pSquareBaseplateLayer = this;
	m_baseSquareList = new std::vector < BaseSquare >();
}

SquareBaseplateLayer::~SquareBaseplateLayer()
{
	for (auto sq : *m_baseSquareList)
	{
		delete sq.pSquare;
	}
	m_baseSquareList->clear();
	delete m_baseSquareList;
	s_pSquareBaseplateLayer = NULL;
    m_drawNode->clear();
    m_drawNodeGrid->clear();
	m_winCallBack = nullptr;
}

bool SquareBaseplateLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
    m_drawNode = DrawNode::create();
    addChild(m_drawNode, 1);
    
    m_drawNodeGrid = DrawNode::create();
    addChild(m_drawNodeGrid,0);
    
	this->scheduleUpdate();

    return true;
}

void SquareBaseplateLayer::readMapBuf(std::string buf)
{
	rapidjson::Document _json;
	_json.Parse<0>(buf.c_str());
	rapidjson::Value &versionValue = _json["version"];
	if (versionValue.GetInt() == 0)
	{
		rapidjson::Value &witdhValue = _json["map_witdh"];
		rapidjson::Value &heightValue = _json["map_height"];

		setBaseSize(BaseSize(witdhValue.GetInt(), heightValue.GetInt()));

		rapidjson::Value &mapBufValue = _json["map_buf"];
		std::string mapBuf = mapBufValue.GetString();
		for (int i = 0; i <= getBaseSize().width * getBaseSize().height -1; i++)
		{
			switch (mapBuf[i])
			{
			case '0':
				//上下翻转，因为gl坐标原点在左下。
				m_baseSquareList->push_back(BaseSquare(SquareBaseplateState::SQBS_EMPTY,
					new Square(i % getBaseSize().width, getBaseSize().height - 1 - i / getBaseSize().height, Square::SC_BLACK)
					));
				break;
			case '1':
				//上下翻转，因为gl坐标原点在左下。
				m_baseSquareList->push_back(BaseSquare(SquareBaseplateState::SQBS_MAP,
					new Square(i % getBaseSize().width, getBaseSize().height - 1 - i / getBaseSize().height, Square::SC_GRAY)
					));
				break;
			default:
				//上下翻转，因为gl坐标原点在左下。
				m_baseSquareList->push_back(BaseSquare(SquareBaseplateState::SQBS_EMPTY,
					new Square(i % getBaseSize().width, getBaseSize().height - 1 - i / getBaseSize().height, Square::SC_RED)
					));
				break;
			}
		}

	}
	else
	{
		return;
	}

}

void SquareBaseplateLayer::readMapBufTest()
{
    setBaseSize(BaseSize(6,6));
    for (int i = 0; i <= getBaseSize().width * getBaseSize().height; i++)
    {
        switch (baselist[i])
        {
            case 0:
                //上下翻转，因为gl坐标原点在左下。
                m_baseSquareList->push_back(BaseSquare(SquareBaseplateState::SQBS_EMPTY,
                                                       new Square(i % getBaseSize().width, getBaseSize().height - 1 - i / getBaseSize().height, Square::SC_BLACK)
                                                       ));
                break;
            case 1:
                //上下翻转，因为gl坐标原点在左下。
                m_baseSquareList->push_back(BaseSquare(SquareBaseplateState::SQBS_MAP,
                                                       new Square(i % getBaseSize().width, getBaseSize().height - 1 - i / getBaseSize().height, Square::SC_GRAY)
                                                       ));
                break;
            default:
                //上下翻转，因为gl坐标原点在左下。
                m_baseSquareList->push_back(BaseSquare(SquareBaseplateState::SQBS_EMPTY,
                                                       new Square(i % getBaseSize().width, getBaseSize().height - 1 - i / getBaseSize().height, Square::SC_RED)
                                                       ));
                break;
        }
    }
}

void SquareBaseplateLayer::createEmptyMap(BaseSize baseSize)
{
    setBaseSize(baseSize);
    for (int i = 0; i < getBaseSize().width * getBaseSize().height; i++)
    {
        //上下翻转，因为gl坐标原点在左下。
        m_baseSquareList->push_back(BaseSquare(SquareBaseplateState::SQBS_EMPTY,
                                               new Square(i % getBaseSize().width, getBaseSize().height - 1 - i / getBaseSize().height, Square::SC_GRAY)
                                               ));
    }
}

void SquareBaseplateLayer::drawBasesplate(Vec2 squareSize)
{
	setSquareSize(squareSize);
    createGrid();
	m_drawNode->clear();
    for(auto sq : *m_baseSquareList)
    {//todo 将绘制方法放在Square类中实现，加入描边功能
  //      Vec2 _origin =(Vec2(squareSize.x * sq->getIndexX(), squareSize.y * sq->getIndexY()));
		//Vec2 _dest = (Vec2(squareSize.x * (sq->getIndexX() + 1), squareSize.y * (sq->getIndexY() + 1)));
  //      
  //      m_drawNode->drawSolidRect(_origin, _dest, sq->getColor4F());
		if (sq.pSquare)
		{
			switch (sq.squareState)
			{
			case SQBS_EMPTY:
				break;
			case SQBS_MAP:
				sq.pSquare->drawSquare(m_drawNode, squareSize);
				break;
			case SQBS_FRAME:
				sq.pSquare->drawFrame(m_drawNode, squareSize,Color4F::YELLOW);
				break;
			case SQBS_SQUARE:
				sq.pSquare->drawFrame(m_drawNode, squareSize, Color4F::RED);
				break;
			default:
				break;
			}
			
		}
		
    }
}

void SquareBaseplateLayer::drawBasesplate()
{
	drawBasesplate(getSquareSize());
}

int SquareBaseplateLayer::getIndexByPos(cocos2d::Vec2 point)
{
	Vec2 pos =  convertToNodeSpace(point);
	if (pos.x < 0 || pos.x > getBaseSize().width * getSquareSize().x)
	{
		return -1;
	}
	if (pos.y < 0 || pos.y > getBaseSize().height * getSquareSize().y)
	{
		return -1;
	}

	unsigned int _indexX = (int)(pos.x / getSquareSize().x);
	unsigned int _indexY = getBaseSize().height - 1 - (int)(pos.y / getSquareSize().y);
	unsigned int _index = _indexY * getBaseSize().width + _indexX;
	if (_index >= m_baseSquareList->size())
	{
		return -1;
	}
	return _index;
	//if ((*m_baseSquareList)[_index].squareState == SquareBaseplateState::SQBS_EMPTY
	//	|| (*m_baseSquareList)[_index].squareState == SquareBaseplateState::SQBS_FRAME
	//	)
	//{
	//	return _index;
	//}
	//return -1;
}

void SquareBaseplateLayer::clearFrameSquare()
{
	for (auto &basesquare:*m_baseSquareList)
	{
		if (basesquare.squareState == SquareBaseplateState::SQBS_FRAME)
		{
			basesquare.squareState = SquareBaseplateState::SQBS_EMPTY;
		}
	}
}

void SquareBaseplateLayer::SetSquarePlaced(unsigned int index)
{
	if (index >= m_baseSquareList->size())
	{
		assert(0);
	}
	if ((*m_baseSquareList)[index].squareState == SquareBaseplateState::SQBS_EMPTY || (*m_baseSquareList)[index].squareState == SquareBaseplateState::SQBS_FRAME)
	{
		(*m_baseSquareList)[index].squareState = SquareBaseplateState::SQBS_SQUARE;
	}
	else
	{
		assert(0);
	}
	
}

void SquareBaseplateLayer::SetSquareEmpty(unsigned int index)
{
	if (index >= m_baseSquareList->size())
	{
		assert(0);
	}
	if ((*m_baseSquareList)[index].squareState == SquareBaseplateState::SQBS_SQUARE || (*m_baseSquareList)[index].squareState == SquareBaseplateState::SQBS_FRAME)
	{
		(*m_baseSquareList)[index].squareState = SquareBaseplateState::SQBS_EMPTY;
	}
	else
	{
		assert(0);
	}
}

void SquareBaseplateLayer::setSquareFrame(unsigned int index)
{
	if (index >= m_baseSquareList->size())
	{
		assert(0);
	}
	if ((*m_baseSquareList)[index].squareState == SquareBaseplateState::SQBS_EMPTY || (*m_baseSquareList)[index].squareState == SquareBaseplateState::SQBS_SQUARE)
	{
		(*m_baseSquareList)[index].squareState = SquareBaseplateState::SQBS_FRAME;
	}
	/*else
	{
	assert(0);
	}*/
}


SquareBaseplateState SquareBaseplateLayer::getSquareState(unsigned int index)
{
	if (index >= m_baseSquareList->size())
	{
		assert(0);
	}
	return (*m_baseSquareList)[index].squareState;
}


cocos2d::Vec2 SquareBaseplateLayer::getWorldPos(unsigned int index)
{
	if (index >= m_baseSquareList->size())
	{
		assert(0);
	}
	Vec2 squarePos = Vec2(index%getBaseSize().width * getSquareSize().x, (getBaseSize().height - 1 - index/getBaseSize().height) * getSquareSize().y);
	Vec2 worldPos = getParent()->convertToWorldSpace(getPosition() + squarePos);

	return worldPos;
}

void SquareBaseplateLayer::drawGrid(bool flag)
{
    m_drawNodeGrid->setVisible(flag);
}

void SquareBaseplateLayer::createGrid()
{
    m_drawNodeGrid->drawRect(Vec2(0,0), Vec2(getBaseSize().width*getSquareSize().x,getBaseSize().height*getSquareSize().y), Color4F::GRAY);
}

std::string SquareBaseplateLayer::getMapBuf()
{
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value ret(rapidjson::kObjectType);

	//version
	ret.AddMember("version", rapidjson::Value(0), allocator);

	ret.AddMember("map_witdh", rapidjson::Value(getBaseSize().width), allocator);

	ret.AddMember("map_height", rapidjson::Value(getBaseSize().height), allocator);

	//地图内容
	rapidjson::Value _mapBufKeyStrJson(rapidjson::kStringType);
	_mapBufKeyStrJson.SetString("map_buf", allocator);
	std::ostringstream _mapBuf;
	for (auto sq:*m_baseSquareList)
	{
		switch (sq.squareState)
		{
		case SQBS_EMPTY:
			_mapBuf << (int)(SquareBaseplateState::SQBS_MAP);
			break;
		case SQBS_SQUARE:
			_mapBuf << (int)(SquareBaseplateState::SQBS_EMPTY);
			break;
		default:
			_mapBuf << (int)(SquareBaseplateState::SQBS_EMPTY);
			break;
		}
		
		//sq.squareState
	}
	rapidjson::Value _mapBufValueStrJson(rapidjson::kStringType);
	_mapBufValueStrJson.SetString(_mapBuf.str().c_str(), allocator);
	ret.AddMember(_mapBufKeyStrJson, _mapBufValueStrJson, allocator);

	//group信息
	rapidjson::Value _groupListValueArrayJson(rapidjson::kArrayType);
	for (auto node : _parent->getChildren())
	{
		auto sg = dynamic_cast<SquareGroup*>(node);
		if (sg != nullptr)
		{
			rapidjson::Value _sgObj(rapidjson::kObjectType);
			_sgObj.AddMember("group_type", rapidjson::Value((int)(sg->getGroupType())), allocator);
			_sgObj.AddMember("group_color", rapidjson::Value((int)(sg->getGroupColor())), allocator);
			_groupListValueArrayJson.PushBack(_sgObj, allocator);
		}
	}
	ret.AddMember("group_list", _groupListValueArrayJson,allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	ret.Accept(writer);

	return buffer.GetString();


}

void SquareBaseplateLayer::update(float delta)
{
	drawBasesplate();
	checkWin();
}

bool SquareBaseplateLayer::checkWin()
{
	for (auto sq : *m_baseSquareList)
	{
		if (sq.squareState != SQBS_SQUARE && sq.squareState != SQBS_MAP)
		{
			return false;
		}
	}
	if (m_winCallBack)
	{
		m_winCallBack();
	}
	return true;

}
