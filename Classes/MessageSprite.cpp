#pragma execution_character_set("utf-8")

#include "MessageSprite.h"

EditBox* MessageSprite::_msgBox = nullptr;
ListView* MessageSprite::_listView = nullptr;
Sprite* MessageSprite::_spr = nullptr;
Menu* MessageSprite::_menu;

QE_SINGLETON_CPP(MessageSprite);

Sprite* MessageSprite::createSprite(int x, int y)
{
	_spr = Sprite::create("msgBg.png");
	_spr->setPosition(x, y);
	_instance->initEditBox();
	_instance->initListView();
	_instance->initMenu();
	_spr->addChild(_msgBox);
	_spr->addChild(_listView);
	return _spr;
}

void MessageSprite::initMenu()
{
	_menu = QMenu::createMenuSprite("send_normal.png", bind(&MessageSprite::send, _instance));
	_menu->setPosition(275,20);
	_spr->addChild(_menu);
}

void MessageSprite::send()
{
	Connect::createMsg();
	Connect::addMsg("msg", QE_ToJStr(_msgBox->getText()));
	Connect::sendMsg();
	//addListViewElement(_msgBox->getText());
	//CCLOG(_msgBox->getText()); 
	//_msgBox->setText("");
}

EditBox* MessageSprite::initEditBox()
{
	_msgBox = EditBox::create(Size(250, 30), Scale9Sprite::create("edit.png"));
	_msgBox->setPosition(Vec2(125, 20));
	_msgBox->setFontName(QE_Font);
	_msgBox->setFontSize(16);
	_msgBox->setFontColor(Color3B(255, 255, 255));
	_msgBox->setPlaceholderFontColor(Color3B(255, 0, 0));
	_msgBox->setPlaceHolder("");////当编辑框中没有任何字符（或输入字符前）的提示文本,即占位符
	_msgBox->setPlaceholderFontName(QE_Font);
	_msgBox->setPlaceholderFontSize(16);
	_msgBox->setMaxLength(30);   //设置文本框中文本的最大长度
	_msgBox->setInputMode(EditBox::InputMode::ANY);//输入键盘模式,任何符号
	_msgBox->setReturnType(EditBox::KeyboardReturnType::DONE);//设置软键盘中回车按钮的样子
	_msgBox->setDelegate(_instance);//开启委托
	return _msgBox;
}

ListView* MessageSprite::initListView()
{
	_listView = ListView::create();
	_listView->setDirection(ScrollView::Direction::BOTH);
	_listView->setBounceEnabled(true);//回弹效果
	_listView->setBackGroundImage("msgBg2.png");
	_listView->setBackGroundImageScale9Enabled(true);
	_listView->setContentSize(Size(300, 160));
	_listView->setPosition(Vec2(0,40));
	return _listView;
}

void MessageSprite::addListViewElement(const std::string msg)
{
	Layout* widget = Layout::create();
	widget->setContentSize(Size(240, 30));

	//创建RichText对象
	RichText* richText = RichText::create();
	//设置是否忽略用户定义的内容大小
	richText->ignoreContentAdaptWithSize(false);
	//设置内容大小
	richText->setContentSize(Size(500, 50));
	richText->setPosition(Vec2(0, 0));
	richText->setAnchorPoint(Vec2(0, 0));

	RichElementText* re2 = RichElementText::create(2, Color3B::RED, 255, msg, QE_Font, 20);
	richText->pushBackElement(re2);

	widget->addChild(richText);
	_listView->pushBackCustomItem(widget);
}

