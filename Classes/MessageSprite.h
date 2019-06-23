#pragma execution_character_set("utf-8")

#pragma once

#include "QE.h"

class MessageSprite: public EditBoxDelegate
{
	QE_SINGLETON_H(MessageSprite);
public:
	static Sprite* createSprite(int x = 0, int y = 0);
	static void addListViewElement(const std::string msg);
private:
	static Sprite* _spr;
	//Menu
	static Menu* _menu;
	void initMenu();
	void send();
	//EditBox
	static EditBox* _msgBox;
	EditBox* initEditBox();
	virtual void editBoxReturn(EditBox* editBox) {};

	//ListView
	static ListView* _listView;
	void testListView();
	ListView* initListView();
};