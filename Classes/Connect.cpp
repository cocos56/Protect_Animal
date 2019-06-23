#pragma execution_character_set("utf-8")

#include "Connect.h"

WebSocket* Connect::_ws = nullptr;
string Connect::_addr = "ws://127.0.0.1:56/";

QE_SINGLETON_CPP(Connect);

void Connect::connect() {
	if (!_ws) { Connect::getInstance()->initSocket(); return; };
}

void Connect::initSocket() {
	_ws = new WebSocket();
	if (!_ws->init(*this, _addr)) { CC_SAFE_DELETE(_ws); }
}

void Connect::onOpen(WebSocket* ws)
{
	//进行这样的判断是因为WebSocket对象没有setTag方法
	if (ws != _ws) { return; }
}

void Connect::onClose(WebSocket* ws)
{
	if (ws != _ws) { return; }
	_ws = nullptr;
}

void Connect::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
	if (ws != _ws) { return; }
	_ws->close();
}

void Connect::createMsg()
{
	QJson::emptyDoc();
}

void Connect::sendMsg()
{
	if (!_ws) { return; }
	CCLOG("send：%s", QJson::getString().c_str());
	_ws->send(QJson::getString());
}

void Connect::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
	if (ws != _ws) { return; }
	CCLOG("收到信息：%s", data.bytes);
	QJson::initDocWithString(data.bytes);
	const string msg = QJson::getString("msg");
	MessageSprite::addListViewElement(msg);
}