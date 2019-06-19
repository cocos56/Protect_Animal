#pragma once
//#include "e:\cocosproject\hero\hero\cocos2d\cocos\2d\cclayer.h"
#include "cocos2d.h"
using namespace cocos2d;

class Loading : public Layer
{
public:
	static Scene * createScene();//创建场景（静态的类方法）
    virtual bool init();//构造函数（继承自父类，virtual虚函数）

	//创建layer（宏定义。里面传的参数是当前的类名）
	CREATE_FUNC(Loading);

	void startScene();
};
