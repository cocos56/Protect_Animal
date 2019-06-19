#include "Level.h"
#include "ui/cocosGUI.h"
using namespace ui;
#include  "StartScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
using namespace CocosDenshion;



//创建场景和层
Scene *  SelectLevel::createScene()
{
	Scene * scene =Scene::create();
	Layer *layer=SelectLevel::create();
	scene->addChild(layer); //将层添加到场景上
	return scene; //输出一个带层的场景
}
bool SelectLevel::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto size=Director::getInstance()->getWinSize();
	Sprite *bg=Sprite::create("bg2.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);
	Button *backButton=Button::create("backarrow.png","backarrow.png");
	backButton->setPosition(Vec2(50,size.height - 50));
	//backButton->setScaleX(0.5);
	//backButton->setScaleY(0.5);
	backButton->addClickEventListener(CC_CALLBACK_0(SelectLevel::back,this));
	addChild(backButton);

	/*添加3个图片按钮，并把按钮添加到node节点中，再把node节点加到当前层中，并让节点执行跳跃动作，
	设置tag的目的是为了在执行回调时通过获取tag值，并把它保存到UserDefault.xml文件中，
	在调用Director::getInstance()->replaceScene(selectLevel::createScene());生成选关场景时，
	从UserDefault.xml文件中取出来，用来选择某一关的场景。
	*/
	auto node=Node::create();
	node->setPosition(Vec2(size.width/2,size.height/2));
	Button *player1=Button::create("scene11.png","scene11.png");
	player1->setPosition(Vec2(-80,100));
	player1->setScaleX(0.8);
	player1->setTag(1);
	player1->setScaleY(0.8);
	player1->addClickEventListener(CC_CALLBACK_1(SelectLevel::goto_game,this));
	Button *player2=Button::create("scene22.png","scene22.png");
	player2->setPosition(Vec2(150,100));
	player2->setScaleX(0.8);
	player2->setTag(2);
	player2->setScaleY(0.8);
	player2->addClickEventListener(CC_CALLBACK_1(SelectLevel::goto_game,this));
	Button *player3=Button::create("scene33.png","scene33.png");
	player3->setPosition(Vec2(-80,-150));
	player3->addClickEventListener(CC_CALLBACK_1(SelectLevel::goto_game,this));
	player3->setScaleX(0.8);
	player3->setTag(3);
	player3->setScaleY(0.8);
	Button *player4=Button::create("scene44.png","scene44.png");
	player4->setPosition(Vec2(150,-150));
	player4->setTag(4);
	player4->addClickEventListener(CC_CALLBACK_1(SelectLevel::goto_game,this));
	player4->setScaleX(0.8);
	player4->setScaleY(0.8);
	node->addChild(player1);
	node->addChild(player2);
	node->addChild(player3);
	node->addChild(player4);
	this->addChild(node);
	//时间，位置，高度，次数，产生跳跃的动作
	JumpTo *jumpTo=JumpTo::create(1,Vec2(size.width/2,size.height/2-20),50,2);
	node->runAction(jumpTo);
	return true;
}
void SelectLevel::back()
{
	Scene * scene=StartScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void SelectLevel::goto_game(Ref *sender)
{
	Button *button=(Button *)sender;
	int a = button->getTag();
	log("%d",a);
	UserDefault::getInstance()->setIntegerForKey("cha",a);
	Scene * scene=HelloWorld::createScene();
	Director::getInstance()->replaceScene(scene);
	//int tag=button->getTag();
	////章节信息用的也比较少，所以放到userdefault.xml中,而且数据不需要修改
	//UserDefault::getInstance()->setIntegerForKey("chapter",tag);
	////Director::getInstance()->replaceScene(selectLevel::createScene());
}
