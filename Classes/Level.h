#pragma execution_character_set("utf-8")

#pragma once
//#include "e:\cocosproject\hero\hero\cocos2d\cocos\2d\cclayer.h"
#include "cocos2d.h"
USING_NS_CC;

class SelectLevel :public Layer
{
public:
	static Scene * createScene();
	virtual bool  init();
	CREATE_FUNC(SelectLevel);
	void back();
	void goto_game(Ref *sender);
};


