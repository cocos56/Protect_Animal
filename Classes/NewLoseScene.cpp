#pragma execution_character_set("utf-8")

#include "QE.h"
#include "NewLoseScene.h"
#include "gameScene.h"
#include "Level.h"
Scene* NewLoseScene::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = NewLoseScene::create();
	scene->addChild(layer);
	return scene;
}
bool NewLoseScene::init()
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

	auto repeatButton = MenuItemImage::create("reapt.png", "reapt.png", CC_CALLBACK_0(NewLoseScene::repeat, this));
	repeatButton->setPosition(Vec2(size.width / 2 - 50, size.height / 2 +80));
	auto menuButton = MenuItemImage::create("menus.png", "menus.png", CC_CALLBACK_0(NewLoseScene::menus, this));
	menuButton->setPosition(Vec2(size.width / 2 + 80, size.height / 2 +80));
	menu = Menu::create(repeatButton, menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto dic = Dictionary::createWithContentsOfFile("text.xml");
	auto str = (__String *)(dic->objectForKey("score"));
	auto Notrecoverable = Label::createWithTTF(str->getCString(), QE_Font, 40);
	Notrecoverable->setPosition(Vec2(size.width / 2 -30, size.height / 2 +160));
	Notrecoverable->setColor(Color3B(225, 225, 0));
	this->addChild(Notrecoverable);

	int score = UserDefault::getInstance()->getIntegerForKey("score_");
	auto strsss = __String::createWithFormat("%d", score);
	auto labls = Label::createWithTTF(strsss->getCString(), QE_Font, 40);
	labls->setPosition(Vec2(size.width / 2 +30 , size.height / 2 + 160));
	labls->setColor(Color3B(225, 225, 0));
	this->addChild(labls);

	return true;
}
void NewLoseScene::repeat()
{
	Scene *scene = gameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
void NewLoseScene::menus()
{
	Scene *scene = SelectLevel::createScene();
	Director::getInstance()->replaceScene(scene);
}