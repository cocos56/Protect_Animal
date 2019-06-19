#include "StartScene.h"
#include "ui/CocosGUI.h"
using namespace ui;
#include "SimpleAudioEngine.h"
#include "Level.h"
using namespace CocosDenshion;

//创建场景scene和层layer
Scene* StartScene::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = StartScene::create();
	scene->addChild(layer); //将层加载到场景上
	return scene; //返回一个带层的场景
}
bool StartScene::init()
{
	if(!Layer::init())
	{
		return false;
	}
	
	//Director单例，整个游戏只有一个导演在控制游戏
	size = Director::getInstance()->getWinSize();


	Sprite* bg = Sprite::create("bg2.png");
	auto scaleX = size.width/bg->getContentSize().width;
	auto scaleY = size.height/bg->getContentSize().height;
	bg->setScale(scaleX,scaleY);
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);
	//创建“开始游戏，游戏设置和游戏帮助”三个菜单选项（用图片创建Item）
	auto startButton = MenuItemImage::create("play.png","play.png",CC_CALLBACK_0(StartScene::start,this));
	startButton->setPosition(Vec2(size.width/2,size.height/2+70));
	auto setButton = MenuItemImage::create("set.png","set.png",CC_CALLBACK_0(StartScene::setLayer,this));
	setButton->setPosition(Vec2(size.width/2,size.height/2-50));
	auto helpButton = MenuItemImage::create("help.png","help.png",CC_CALLBACK_0(StartScene::helpLayer,this));
	helpButton->setPosition(Vec2(size.width/2,size.height/2-150));

	//创建菜单，把上面创建的三个菜单项添加到菜单中
	menu = Menu::create(startButton,setButton,helpButton,NULL);
	menu->setPosition(Vec2::ZERO);
	//把菜单添加到当前层中
	this->addChild(menu);

	//添加并播放蝴蝶飞舞的动画
	//startAnimate();

	//在当前层中添加雪花飘落的离子效果
	auto snow = ParticleSnow::create();
	snow->setPosition(Vec2(size.width/2,size.height));
	this->addChild(snow);

	//播放音乐（前提包含头文件，命名空间，是个单例）
	SimpleAudioEngine::getInstance()->playBackgroundMusic("backmusic.mp3");
	
	return true;
}
//“开始游戏”菜单的回调函数，点击即可进入选择角色选择关卡界面
void StartScene::start( )
{
	log("start");
	Scene *scene=SelectLevel::createScene();
	Director::getInstance()->replaceScene(scene);
}
//设置菜单的回调函数，其中添加背景音乐和音量大小的控制和ok按钮
void StartScene::setLayer( )
{
	//进入回调界面要求原来的菜单不可用
	menu->setEnabled(false);
	
	//添加设置面板
	setBg = Sprite::create("mianban.png");
	setBg->setPosition(Vec2(size.width/2,100));
	this->addChild(setBg);

	//音乐控制的开关菜单
	auto onMusic = MenuItemImage::create("duihao1.png","duihao1.png");
	auto offMusic = MenuItemImage::create("duihao2.png","duihao2.png");
	//auto Music = MenuItemToggle::create(onMusic);
	//Music->addSubItem(offMusic);
	//Music->setCallback(CC_CALLBACK_1(StartScene::setMusic,this));
	
	auto Music=MenuItemToggle::createWithCallback(CC_CALLBACK_1(StartScene::setMusic,this),onMusic,offMusic,NULL);

	
	Music->setPosition(Vec2(setBg->getContentSize().width/2,setBg->getContentSize().height/2+100));
	auto menu = Menu::create(Music,NULL);
	menu->setPosition(Vec2::ZERO);

	setBg->addChild(menu);

	//okButton
	//Button*okButton = Button::create("ok.png","ok.png","ok.png");
	Button*okButton = Button::create("ok.png","ok.png");
	okButton->addClickEventListener(CC_CALLBACK_0(StartScene::setBack,this));
	okButton->setPosition(Vec2(setBg->getContentSize().width/2,100));
	setBg->addChild(okButton);

	//“对号”菜单前面的Music标签Music
	Label* MusicLabel = Label::createWithSystemFont("Music","",30);
	MusicLabel->setColor(Color3B::BLUE);
	MusicLabel->setPosition(Vec2(setBg->getContentSize().width/2-100,setBg->getContentSize().height/2+100));
	setBg->addChild(MusicLabel);

	////音效控制开关菜单
	//auto onEffect = MenuItemImage::create("duihao1.png","duihao1.png");
	//auto offEffect = MenuItemImage::create("duihao2.png","duihao2.png");
	//auto Effect = MenuItemToggle::create(onEffect);
	//Effect->addSubItem(offEffect);
	////上面两条语句可以用替换auto Effect = MenuItemToggle::create(onEffect，offEffect，NULL);
	//Effect->setCallback(CC_CALLBACK_1(StartScene::setEffect,this));
	//Effect->setPosition(Vec2(setBg->getContentSize().width/2+150,setBg->getContentSize().height/2+100));
	//auto menu2 = Menu::create(Effect,NULL);
	//menu2->setPosition(Vec2::ZERO);
	//setBg->addChild(menu2);

	////“对号”菜单前面的标签Effect
	//Label* EffectLable = Label::createWithSystemFont("Effect","",30);
	//EffectLable->setColor(Color3B::BLUE);
	//EffectLable->setPosition(Vec2(setBg->getContentSize().width/2+60,setBg->getContentSize().height/2+100));
	//setBg->addChild(EffectLable);

	//添加滑动条
	auto slider=Slider::create();
	slider->loadBarTexture("SliderBar.png");
	slider->loadSlidBallTextures("aaa.png","aaa.png","aaa.png");
	slider->setPosition(Vec2(setBg->getContentSize().width/2,setBg->getContentSize().height/2));
	slider->setScale9Enabled(true);
	slider->setContentSize(Size(Vec2(300,40)));
	slider->setPercent(50);
	slider->addEventListener(CC_CALLBACK_1(StartScene::adjustVolumn,this)); //滑动条的监听回调，时刻检测滑动条的值得改变
	setBg->addChild(slider);
	//添加完设置界面内容后，让它从100的位置移动到界面的中央
	MoveTo* moveTo = MoveTo::create(0.5,Vec2(size.width/2,size.height/2));
	setBg->runAction(moveTo);
}
//调整音量
void StartScene::adjustVolumn(Ref * sender)
{
	Slider *slider=(Slider *)sender;
	log("%d",slider->getPercent());
	//滑动条的值为0-100之间的整数，而音量设置的值在0-1之间。故需要除以100.
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(slider->getPercent()/100);
}

//Ref是所有类的一个父类
void StartScene::setBack( )
{
	//设置里 确定 按钮
	menu->setEnabled(true);
	setBg->removeFromParent();
}
//设置音乐的开关状态，控制音乐的播放和暂停
void StartScene::setMusic(Ref* sender)
{
	MenuItemToggle *toggle=(MenuItemToggle *)sender;
	//获取菜单组的下标（0--off， 1--on）
	if(toggle->getSelectedIndex( )==1)
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}
	else
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}
//设置音效的开关状态，并把这个状态存放到缓存UserDefault.xml文件中，以备需要时播放音效。
//void StartScene::setEffect(Ref* sender)
//{
//	MenuItemToggle *toggle=(MenuItemToggle *)sender;
//	if(toggle->getSelectedIndex( )==1 )
//	{
//		/*长期保存的数据，需要存到json或xml文件，暂时保存的可存到缓存userdefault.xml文件中，
//		这里保存暂时的音效状态，下次再玩游戏时给这个数据无关
//		*/
//		UserDefault::getInstance()->setBoolForKey("onEffect",true);
//	}
//	else
//	{
//		UserDefault::getInstance()->setBoolForKey("onEffect",false);
//	}
//}
//游戏帮助按钮的回调
void StartScene::helpLayer()
{
	//关掉menu的点击事件（false不可点击）
	menu->setEnabled(false);

	//帮助界面，把帮助面板的精灵加到游戏设置层上，
	helpSp = Sprite::create("help_mianban.png");
	helpSp->setPosition(Vec2(size.width/2,size.height/2-200));
	this->addChild(helpSp);

	//在帮助层，创建Button（需要两个图片）
	Button* okButton = Button::create("ok.png","ok.png");

	//Button的监听方法，点击button按钮就返回到游戏设置界面
	okButton->addClickEventListener(CC_CALLBACK_0(StartScene::helpBack,this));
	okButton->setPosition(Vec2(helpSp->getContentSize().width/2,100));
	
	//把button按钮添加到帮助面板精灵上。
	helpSp->addChild(okButton);

	//帮助面板在0.8秒内从纵坐标100的位置移动到界面的中间位置。
	auto moveTo = MoveTo::create(0.5,Vec2(size.width/2,size.height/2));
	helpSp->runAction(moveTo);
}
//帮助界面中的ok按钮的回调函数，点击则返回到游戏设置界面
void StartScene::helpBack()
{
	//menu点击事件开启（true开启点击事件）
	menu->setEnabled(true);

	//把帮助界面从父试图中移除，则返回到游戏设置界面
	helpSp->removeFromParent();
}
