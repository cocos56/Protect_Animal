#pragma execution_character_set("utf-8")
#pragma once

#include "QE.h"

class Connect : public WebSocket::Delegate
{
	QE_SINGLETON_H(Connect);
public:
public:
	static WebSocket* _ws;
	static string _addr;

	static void connect();
	static void createMsg();
	static void addMsg(JString key, JString value) { QJson::addMember(key, value); };
	static void addMsg(JString key, int value) { QJson::addMember(key, value); };
	static void sendMsg();

private:
	void initSocket();

	//委托协议方法
	virtual void onOpen(WebSocket* ws) override;
	virtual void onMessage(WebSocket* ws, const WebSocket::Data& data) override;
	virtual void onClose(WebSocket* ws) override;
	virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error) override;
};