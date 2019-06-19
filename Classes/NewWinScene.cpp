#include "NewWinScene.h"
#include "HelloWorldScene.h"
#include "Level.h"
Scene* NewWinScene::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = NewWinScene::create();
	scene->addChild(layer);
	return scene;
}
bool NewWinScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	size = Director::getInstance()->getWinSize();

	Sprite* bg = Sprite::create("bg.png");
	auto scaleX = size.width / bg->getContentSize().width;
	auto scaleY = size.height / bg->getContentSize().height;
	bg->setScale(scaleX, scaleY);
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);

	auto vecbg = Sprite::create("vecbg.png");
	vecbg->setPosition(Vec2(size.width / 2, size.height / 2+140));
	this->addChild(vecbg);

	auto repeatButton = MenuItemImage::create("reapt.png", "reapt.png", CC_CALLBACK_0(NewWinScene::repeat, this));
	repeatButton->setPosition(Vec2(size.width / 2-50 , size.height / 2+80));
	auto menuButton = MenuItemImage::create("menus.png", "menus.png", CC_CALLBACK_0(NewWinScene::menus, this));
	menuButton->setPosition(Vec2(size.width / 2+80 , size.height / 2+80));
	menu = Menu::create(repeatButton, menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto dic = Dictionary::createWithContentsOfFile("text.xml");
	auto str3 = (__String *)(dic->objectForKey("win"));
	auto Notrecoverable = Label::createWithTTF(str3->getCString(), "fonts/youyuan.ttf", 40);
	Notrecoverable->setPosition(Vec2(size.width / 2 , size.height/2 +140));
	Notrecoverable->setColor(Color3B(225,225,0));
	this->addChild(Notrecoverable);

	return true;
}
void NewWinScene::repeat()
{
	Scene *scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(scene);
}
void NewWinScene::menus()
{
	Scene *scene = SelectLevel::createScene();
	Director::getInstance()->replaceScene(scene);
}