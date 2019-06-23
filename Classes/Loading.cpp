#pragma execution_character_set("utf-8")

#include "QE.h"
#include "Loading.h"
#include "StartScene.h"
#include "ui/CocosGUI.h"
using namespace ui;
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#include "Level.h"

//创建场景scene和层layer
Scene * Loading::createScene()
{
	Scene * scene = Scene::create();
	Layer * layer = Loading::create();
	scene->addChild(layer); //将层添加到场景上
	return scene;  //返回一个带层的场景
}
//进度条
bool Loading::init()
{
	//初始化父类的构造函数，是的话就操作，否的话就return false
	if (!Layer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getWinSize();//获取屏幕的大小
	Sprite * bg = Sprite::create("bg1.png");//背景

	//让背景图片与屏幕适配
	//auto与id的区别：auto是智能指针，可以自动识别后面参数的类型
	auto scaleX = size.width/bg->getContentSize().width;
	auto scaleY = size.height/bg->getContentSize().height;
	bg->setScale(scaleX,scaleY);//缩放
	bg->setPosition(Vec2(size.width/2,size.height/2));//设置位置
	this->addChild(bg);//添加父试图

	Label * up = Label::createWithSystemFont("Protect Animals","",40);
	up->setPosition(Vec2(size.width/2,size.height/2));
	up->setColor(Color3B(158,60,50));
	this->addChild(up);
	

	Sprite * timerSp = Sprite::create("loading.png");//创建精灵
	ProgressTimer * timer = ProgressTimer::create(timerSp);//进度条
	timer->setPosition(Vec2(size.width/2-20,size.height/2-100));//设置位置
	auto light=ParticleFlower::create();
	light->setPosition(timer->getPosition());
	this->addChild(light);
	/*
	setBarChangeRate(Vec2(1,0))和setMidpoint(Vec2(0,1))共同决定设置进度条的展开方向:
	第一个参数为0表示水平方向没有展开效果，为1表示水平方向有展开效果；
	第二个参数为0表示垂直方向没有展开效果，为1表示垂直方向有展开效果。
	(1,0):x轴方向从左向右展开；(0,1)从下向上展开；（0,0）没有展开效果；（1,1）从左下角向右向上展开。
	*/
	timer->setBarChangeRate(Vec2(1,0));
	timer->setMidpoint(Vec2(0,1));//设置基准点（展开的从左边开始）
	timer->setType(ProgressTimer::Type::BAR);//设置进度条类型（条形）
	timer->setPercentage(0);//设置进度条百分比 当前所显示的
	this->addChild(timer);

	//1秒 运动到100%
	ProgressTo * progressTo = ProgressTo::create(3,100);
	auto call = CallFunc::create(CC_CALLBACK_0(Loading::startScene,this));
	auto sequene = Sequence::create(progressTo,call,NULL);
	timer->runAction(sequene);

	Label * lable = Label::createWithSystemFont("Loading","",40);
	lable->setPosition(Vec2(size.width/2-20,size.height/2-100));
	this->addChild(lable);

	Connect::connect();

	return true;
}
//进入开始游戏界面
void Loading::startScene()
{
	//声明一个场景
	Scene * scene = StartScene::createScene();
	//翻页动画
	TransitionPageTurn * turn = TransitionPageTurn::create(1,scene,false);
	//替换场景
	Director::getInstance()->replaceScene(turn);
}
