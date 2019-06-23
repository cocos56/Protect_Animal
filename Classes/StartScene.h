#pragma execution_character_set("utf-8")

#pragma once
//#include "e:\cocosproject\hero\hero\cocos2d\cocos\2d\cclayer.h"
#include "cocos2d.h"
using namespace cocos2d;

class StartScene : public Layer
{
public:
	static Scene *createScene();
	virtual bool init();
	CREATE_FUNC(StartScene);
	void start();
	void setLayer();
	void helpLayer();
	//void startAnimate();
	void helpBack();
	void setMusic(Ref* sender);//Ref类是所有类的父类
	void setBack();
	void setEffect(Ref* sender);
	void adjustVolumn(Ref * sender); //调整音量

private:
	Size size;
	Sprite* helpSp;
	Menu* menu;
	Sprite* setBg;
};


