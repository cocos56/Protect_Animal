#pragma execution_character_set("utf-8")

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
#include "string.h"
USING_NS_CC;
#include "ui\CocosGUI.h"
using namespace ui;
//创建带场景的layer
class gameScene : public cocos2d::Layer
{
public:
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(gameScene);

	Sprite* _spr;
	EditBox* _msgBox;
	ListView* _listView;

	//3.测试文本
	void testText();
	void textCallCack(Ref* sender, Widget::TouchEventType type);

	//4.测试按钮button
	void testButton();
	void btnClicked(Ref* sender, Widget::TouchEventType type);

	void testListView();


private:
	LoadingBar  *loadingBar;
    TMXTiledMap *_tileMap;//地图
    Sprite *_player;// 精灵
    Point _beginPoint;//起始点
    TMXLayer *_meta;// 障碍物
    TMXLayer *_fruit;// 西瓜
    __Array *_enemyArray;//存放敌人的数组
    __Array *_bulletArray;//存放子弹的数组
    void goon(Node *pSender);//添加敌人调用的函数
   
    //重写Layer的触摸事件，当用户触摸或在移动屏幕时发生
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    //用来更新显示英雄和敌人是否碰撞，敌人和子弹是否碰撞
    void updateGame(float f);


	int nums;
	int nums2;
	Size size;
    
    //用来判断地图是否左移，即需要则重新设置地图的中心点
    void setViewpointCenter(Point position);
    
    /*让英雄移动到点击的瓦块上，其中需要判断是墙等障碍物不能移动，是西瓜要洗掉（让西瓜删除），什么也没有可以移动过去,其中可借助meta图层操作 */
    void movePlayer(Point p);
    //当飞镖到达边界时移除飞镖
    void  targetFinish( Node *pSender);
    virtual void onExit();
    
};

//仅仅创建一个layer
class ButtonMenu:public Layer
{
public:
	void countDown(float f);
	__String  *str;
	int time;
    virtual bool init();
    CREATE_FUNC(ButtonMenu);
    void  change(Ref *obj);
};

#endif // __HELLOWORLD_SCENE_H__
