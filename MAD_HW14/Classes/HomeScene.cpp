#include "HomeScene.h"
#include "GameScene.h"
#include "Http.h"
#include "json/document.h"
#include "json/rapidjson.h"

Scene * HomeScene::createScene() {
	auto scene = Scene::create();
	auto hostLayer = HomeScene::create();
	scene->addChild(hostLayer);
	return scene;
}

bool HomeScene::init() {
	if (!Layer::init()) {
		return false;
	}

	vo = Director::getInstance()->getVisibleOrigin();
	vs = Director::getInstance()->getVisibleSize();

	auto mainLayer = Layer::create();
	addChild(mainLayer, 0);
	nameField = TextField::create("Username here...", "fonts/Marker Felt.ttf", 36);
	nameField->setPosition(Vec2(vo.x + vs.width / 2, vo.y + vs.height / 2 + 90));
	mainLayer->addChild(nameField);

	auto menuLayer = Menu::create();
	menuLayer->setPosition(vo.x, vo.y);
	addChild(menuLayer, 1);
	auto loginLabel = Label::createWithTTF("Login", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	loginButton = MenuItemLabel::create(loginLabel, CC_CALLBACK_1(HomeScene::loginButtonClick, this));
	loginButton->setPosition(vo.x + vs.width / 2, vo.y + vs.height / 2 - 90);
	menuLayer->addChild(loginButton);

	auto noticeLayer = Layer::create();
	noticeLayer->setAnchorPoint(Vec2(0, 0));
	addChild(noticeLayer, 2);
	noticeLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	noticeLabel->setPosition(vo.x + vs.width / 2, vo.y + 60);
	noticeLayer->addChild(noticeLabel);

	return true;
}

void HomeScene::loginButtonClick(Ref * sender) {
	if (nameField->getString().size() == 0) {
		showNotice("Username required");
		return;
	}
	std::string body("username=");
	body.append(nameField->getString());
	ChainHttpClient::create()
		->setUrl("http://localhost:8080/login")
		->setCharBufferContent(body.c_str())
		->POST(CC_CALLBACK_2(HomeScene::loginCallback, this));
}

void HomeScene::loginCallback(HttpClient * client, HttpResponse * response) {
	ChainHttpResponse wrappedResponse(response);
	rapidjson::Document d;
	d.Parse<0>(wrappedResponse.getDataString());
	if (!d.HasParseError() && d.IsObject() && d.HasMember("result") && d.HasMember("info")) {
		if (d["result"].GetBool()) {
			UserDefault::getInstance()->setStringForKey("SID", wrappedResponse.getHeader("Set-Cookie"));
			auto gameScene = GameScene::createScene();
			Director::getInstance()->pushScene(gameScene);
		} else {
			std::string notice("Login failed: ");
			notice.append(d["info"].GetString());
			showNotice(notice.c_str());
		}
	} else {
		showNotice("Bad response data");
	}
}

void HomeScene::showNotice(const char * notice) {
	noticeLabel->setString(notice);
	noticeLabel->runAction(Sequence::createWithTwoActions(FadeIn::create(1), FadeOut::create(1)));
}
