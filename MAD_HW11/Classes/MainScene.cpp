#include "MainScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene * MainScene::createScene() {
	auto scene = Scene::create();
	auto hostLayer = MainScene::create();
	scene->addChild(hostLayer);
	return scene;
}

bool MainScene::init() {
	if (!Layer::init()) {
		return false;
	}

	auto vs = Director::getInstance()->getVisibleSize();
	auto o = Director::getInstance()->getVisibleOrigin();

	// background and title
	auto background = Sprite::create("background.jpg");
	background->setPosition(o.x + vs.width / 2, o.y + vs.height / 2);
	background->setScale(0.7f);
	this->addChild(background);

	// Menu items
	auto startGameLabel = Label::createWithTTF("START", "fonts/Transformers Movie.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	auto startGameItem = MenuItemLabel::create(startGameLabel, CC_CALLBACK_1(MainScene::startGameCallback, this));
	startGameItem->setPosition(o.x + vs.width - 100, o.y + 100);
	auto quitGameLabel = Label::createWithTTF("QUIT", "fonts/Transformers Movie.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	auto quitGameItem = MenuItemLabel::create(quitGameLabel, CC_CALLBACK_1(MainScene::quitGameCallback, this));
	quitGameItem->setPosition(o.x + vs.width - 100, o.y + 50);
	auto mainMenu = Menu::create(startGameItem, quitGameItem, NULL);
	mainMenu->setAnchorPoint(Vec2(0, 0));
	mainMenu->setPosition(0, 0);
	this->addChild(mainMenu);

	return true;
}

void MainScene::startGameCallback(Ref * pSender) {
	auto gameScene = GameScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(0.5, gameScene));
}

void MainScene::quitGameCallback(Ref * pSender) {
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
