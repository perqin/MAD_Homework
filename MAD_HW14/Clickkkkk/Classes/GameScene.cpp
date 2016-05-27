#include "GameScene.h"

Scene * GameScene::createScene() {
	auto scene = Scene::create();
	auto hostLayer = GameScene::create();
	scene->addChild(hostLayer);
	return scene;
}

bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}

	vo = Director::getInstance()->getVisibleOrigin();
	vs = Director::getInstance()->getVisibleSize();

	// GameLayer
	auto gameLayer = Layer::create();
	addChild(gameLayer);
	auto button = Sprite::create("button.png");
	gameLayer->addChild(button);

	// Indicator layer
	auto indicatorsLayer = Layer::create();
	addChild(indicatorsLayer);
	timeLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	indicatorsLayer->addChild(timeLabel);
	scoreLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	indicatorsLayer->addChild(scoreLabel);

	// Menu
	auto menu = Menu::create();
	addChild(menu);
	auto submitLabel = Label::createWithTTF("上传分数", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	submitItem = MenuItemLabel::create(submitLabel, CC_CALLBACK_1(GameScene::submitItemClick, this));
	menu->addChild(submitItem);
	auto rankingLabel = Label::createWithTTF("查看排行", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	rankingItem = MenuItemLabel::create(rankingLabel, CC_CALLBACK_1(GameScene::rankingItemClick, this));
	menu->addChild(rankingItem);
	auto startLabel = Label::createWithTTF("开始", "fonts/Marker Felt.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	auto startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(GameScene::startItemClick, this));
	menu->addChild(startItem);

	// Touch listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::buttonTouch, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, button);

	return true;
}

void GameScene::startItemClick(Ref * sender) {
	gameInit();
	gameStart();
}

void GameScene::submitItemClick(Ref * sender) {}

void GameScene::rankingItemClick(Ref * sender) {}

bool GameScene::buttonTouch(Touch * touch, Event * unused) {
	setScore(score + 1);
	return true;
}

void GameScene::gameInit() {
	setScore(0);
	setTime(30);
}

void GameScene::gameStart() {
	submitItem->runAction(Hide::create());
	rankingItem->runAction(Hide::create());
	schedule(schedule_selector(GameScene::countdown), 1, CC_REPEAT_FOREVER, 1);
}

void GameScene::gameEnd() {
	unschedule(schedule_selector(GameScene::countdown));
	submitItem->runAction(Show::create());
	rankingItem->runAction(Show::create());
}

void GameScene::countdown(float delta) {
	setTime(time - 1);
	if (time <= 0) {
		gameEnd();
	}
}

void GameScene::setScore(int s) {
	score = s;
	scoreLabel->setString(std::to_string(score));
	// Animation
	scoreLabel->setScale(1.5);
	scoreLabel->runAction(Spawn::createWithTwoActions(ScaleTo::create(0.3f, 1), FadeIn::create(0.3f)));
}

void GameScene::setTime(int t) {
	time = t;
	timeLabel->setString(std::to_string(time));
}
