#pragma once
#include "cocos2d.h"
USING_NS_CC;
class NewLoseScene :public cocos2d::Layer
{
public:
public:
	static Scene *createScene();
	virtual bool init();
	CREATE_FUNC(NewLoseScene);
	void repeat();
	void menus();
private:
	Size size;
	Sprite *bg;
	Menu* menu;
};

