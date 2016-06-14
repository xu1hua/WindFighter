//
//  SquareGroup.h
//  SquareClear
//
//  Created by 徐徐 on 15/10/18.
//  一组方块组成形状的类
//

#ifndef __SquareClear__SquareGroup__
#define __SquareClear__SquareGroup__

#include <stdio.h>
#include "Square.h"
#include "ui/UIButton.h"

struct SquareInSquareGroup
{
	//当方块和基板重合时，group中的方块对应的基板上方块的index
	//不重合是为-1
	int indexInBaseplate;
	Square* square;

	SquareInSquareGroup(Square* sq)
	{
		indexInBaseplate = -1;
		square = sq;
	}
};
enum SquareGroupState
{
	SGS_ORIGIN = 0,
	SGS_SELECTED,
	SGS_UNSELECTED,
	SGS_PLACED,
};
class SquareGroup : public Layer
{
public:
    //方块组的类型，形状
    enum SQUAREGROUP_TYPE
    {
        ST_NONE = -1,
        ST_L,
        ST_L_TURN,
        ST_Z,
        ST_Z_TURN,
        ST_MAX
    };

	bool init() override;

	CREATE_FUNC(SquareGroup);

	SquareGroup();
	~SquareGroup();

	//设置方块组放块的大小，类型和颜色，默认黑色
    void SetSquareGroup(Vec2 squareSize,SquareGroup::SQUAREGROUP_TYPE type,Square::SQUARE_COLOR color = Square::SC_BLACK);

    //设置方块组的类型和颜色，默认黑色
    void SetGroupType(SQUAREGROUP_TYPE type, Square::SQUARE_COLOR color = Square::SC_BLACK);

	//设置放块组放块的大小
	void SetSquareSize(Vec2 size);

    SQUAREGROUP_TYPE getGroupType(){return _groupType;}

	std::vector<SquareInSquareGroup>* getGroupArray(){ return m_groupArray; }

    //绘制方块组
    void DrawGroup();

    //绘制方块组，指定长宽
	void DrawGroup(Vec2 squareSize);
	
    bool checkTouchInSelf_Parent(Touch *touch);    

	//检查group中的所有square是否可以放置入baseplate。可以的话在baseplate上画个框
	void setBaseplateFrameByGroup(bool force = false);

	void TurnLeft();
	void TurnRight();
	
	virtual void setArrowButtonVisible(bool flag);
	void setRightLeftArrowButtonVisible(bool flag);
	void setUpDownArrowButtonVisible(bool flag);
    // Overrides
    virtual bool onTouchBegan(Touch *touch, Event *event) override;

	//检查是否所有放块可以放入baseplate中
	bool checkGroupCanPlaced();

	//计算square在group中的相对锚点的坐标，返回左上角
	bool calcSquareIsTouched(Touch* touch);

	virtual void onTouchMoved(Touch *touch, Event *event) override;
	virtual void onTouchEnded(Touch *touch, Event *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
    
    static const int s_Width;
    static const int s_Height;
    
	CC_SYNTHESIZE(Square::SQUARE_COLOR, _groupColor, GroupColor);
	CC_SYNTHESIZE(SquareGroupState, _groupState, GroupState);
private:
    void CalcGroup(Square::SQUARE_COLOR color = Square::SC_BLACK);
	std::vector<SquareInSquareGroup> *m_groupArray;
    SQUAREGROUP_TYPE _groupType;
	Vec2 m_squareSize;
	cocos2d::DrawNode * m_drawNode;
	//左右箭头，调整group转向
	cocos2d::ui::Button * m_arrowButtonLeft;
	cocos2d::ui::Button * m_arrowButtonRight;
	//上下箭头，切换group类型
	cocos2d::ui::Button * m_arrowButtonUpward;
	cocos2d::ui::Button * m_arrowButtonDownward;
	int* m_groupShape;
};
#endif /* defined(__SquareClear__SquareGroup__) */
