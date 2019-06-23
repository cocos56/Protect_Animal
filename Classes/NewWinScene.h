#pragma execution_character_set("utf-8")

#pragma once
#include "cocos2d.h"
USING_NS_CC;
class NewWinScene :public cocos2d::Layer
{
public:
	static Scene *createScene();
	virtual bool init();
	CREATE_FUNC(NewWinScene);
	void repeat();
	void menus();
private:
	Size size;
	Sprite *bg;
	Menu* menu;
};

