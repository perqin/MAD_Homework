#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UITextField.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::network;

class HomeScene : public Layer {
public:
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(HomeScene);
private:
	Vec2 vo;
	Size vs;

	TextField * nameField;
	MenuItemLabel * loginButton;
	Label * noticeLabel;

	void loginButtonClick(Ref * sender);
	void loginCallback(HttpClient * client, HttpResponse * response);
	void showNotice(const char * notice);
};

#endif // !__HOME_SCENE_H__
