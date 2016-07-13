#include "GameScene.h"
#include "Http.h"
#include "json/rapidjson.h"
#include "json/document.h"

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
	addChild(gameLayer, 0);
	button = Sprite::createWithSpriteFrameName("button.png");
	button->setPosition(vo.x + vs.width / 2, vo.y + vs.height / 2);
	button->setScale(200 / button->getContentSize().width);
	gameLayer->addChild(button);

	// Indicator layer
	indicatorsLayer = Layer::create();
	addChild(indicatorsLayer, 1);
	timeLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	timeLabel->setPosition(vo.x + vs.width / 2, vo.y + vs.height - 60);
	indicatorsLayer->addChild(timeLabel);
	scoreLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	scoreLabel->setPosition(vo.x + vs.width / 2, vo.y + 60);
	indicatorsLayer->addChild(scoreLabel);

	// Menu
	auto menu = Menu::create();
	menu->setPosition(vo.x, vo.y);
	addChild(menu, 2);
	auto submitLabel = Label::createWithTTF("UPLOAD", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	submitItem = MenuItemLabel::create(submitLabel, CC_CALLBACK_1(GameScene::submitItemClick, this));
	submitItem->setPosition(vo.x + vs.width / 2 - 120, vo.y + 60);
	menu->addChild(submitItem);
	auto rankingLabel = Label::createWithTTF("RANKING", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	rankingItem = MenuItemLabel::create(rankingLabel, CC_CALLBACK_1(GameScene::rankingItemClick, this));
	rankingItem->setPosition(vo.x + vs.width / 2 + 120, vo.y + 60);
	menu->addChild(rankingItem);
	auto startLabel = Label::createWithTTF("START", "fonts/Marker Felt.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(GameScene::startItemClick, this));
	startItem->setPosition(vo.x + vs.width / 2, vo.y + vs.height / 2);
	menu->addChild(startItem);

	// Ranking
	rankingLayer = Layer::create();
	addChild(rankingLayer, 3);
	auto background = Sprite::create("dark-mask.png");
	background->setPosition(vo.x + vs.width / 2, vo.y + vs.height / 2);
	background->setScale(vs.width / background->getContentSize().width, vs.height / background->getContentSize().height);
	rankingLayer->addChild(background);
	auto rankingTitle = Label::createWithTTF("RANKING", "fonts/Marker Felt.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	rankingTitle->setPosition(vo.x + vs.width / 2, vo.y + vs.height - 64);
	rankingLayer->addChild(rankingTitle);
	rankingList = Label::createWithTTF("", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	rankingList->setPosition(vo.x + vs.width / 2, vo.y + vs.height / 2 - 48);
	rankingLayer->addChild(rankingList);

	// Touch listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::screenTouch, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::screenReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	submitItem->runAction(Hide::create());
	rankingLayer->runAction(Hide::create());
	gameInit();

	showNotice("Welcome back!");

	return true;
}

void GameScene::startItemClick(Ref * sender) {
	if (rankingLayer->isVisible()) return;
	gameInit();
	gameStart();
}

void GameScene::submitItemClick(Ref * sender) {
	std::string buffer("score=");
	buffer.append(std::to_string(score));
	ChainHttpClient::create()
		->setUrl("http://localhost:8080/submit")
		->setCharBufferContent(buffer.c_str())
		->addHeader("Cookie", UserDefault::getInstance()->getStringForKey("SID").c_str())
		->POST(CC_CALLBACK_2(GameScene::scoreSubmitted, this));
	submitItem->setEnabled(false);
}

void GameScene::rankingItemClick(Ref * sender) {
	rankingLayer->runAction(Sequence::createWithTwoActions(Show::create(), FadeIn::create(0.5f)));
	ChainHttpClient::create()
		->setUrl("http://localhost:8080/rank")
		->addQuery("top", "7")
		->addHeader("Cookie", UserDefault::getInstance()->getStringForKey("SID").c_str())
		->GET(CC_CALLBACK_2(GameScene::rankingGotten, this));
}

bool GameScene::screenTouch(Touch * touch, Event * unused) {
	if (rankingLayer->isVisible()) {
		rankingLayer->runAction(Hide::create());
	} else {
		button->setSpriteFrame(SpriteFrameCache::getInstance()->getInstance()->getSpriteFrameByName("button-down.png"));
	}
	return true;
}

bool GameScene::screenReleased(Touch * touch, Event * unused) {
	if (!rankingLayer->isVisible()) {
		button->setSpriteFrame(SpriteFrameCache::getInstance()->getInstance()->getSpriteFrameByName("button.png"));
		if (!gameOn) return true;
		setScore(score + 1);
	}
	return true;
}

//bool GameScene::rankingBackgroundTouch(Touch * touch, Event * unused) {
//	rankingLayer->runAction(Sequence::createWithTwoActions(FadeOut::create(0.5f), Hide::create()));
//	return false;
//}

void GameScene::gameInit() {
	setScore(0);
	setTime(10);
	gameOn = false;
}

void GameScene::gameStart() {
	submitItem->runAction(Hide::create());
	rankingItem->runAction(Hide::create());
	startItem->runAction(Hide::create());
	schedule(schedule_selector(GameScene::countdown), 1, CC_REPEAT_FOREVER, 1);
	gameOn = true;
}

void GameScene::gameEnd() {
	gameOn = false;
	unschedule(schedule_selector(GameScene::countdown));
	submitItem->runAction(Show::create());
	rankingItem->runAction(Show::create());
	startItem->runAction(Sequence::createWithTwoActions(DelayTime::create(1), Show::create()));
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

void GameScene::updateRanking(const char * r) {
	rankingList->setString(r);
}

void GameScene::showNotice(const char * notice) {
	auto l = Label::createWithTTF(notice, "fonts/Marker Felt.ttf", 27, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	l->setAnchorPoint(Vec2(0.5f, 0));
	l->setPosition(vo.x + vs.width / 2, vo.y + vs.height);
	l->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -36)), MoveBy::create(3, Vec2(0, 0)), MoveBy::create(1, Vec2(0, 36)), nullptr));
	indicatorsLayer->addChild(l);
}

void GameScene::scoreSubmitted(HttpClient * client, HttpResponse * response) {
	submitItem->setEnabled(true);
	ChainHttpResponse wrappedResponse(response);
	rapidjson::Document d;
	d.Parse<0>(wrappedResponse.getDataString());
	if (!d.HasParseError() && d.IsObject() && d.HasMember("result") && d.HasMember("info")) {
		if (d["result"].GetBool()) {
			submitItem->runAction(Hide::create());
			showNotice("Score uploaded!");
		} else {
			std::string notice("Uploading failed: ");
			notice.append(d["info"].GetString());
			showNotice(notice.c_str());
		}
	} else {
		showNotice("Bad response data");
	}
}

void GameScene::rankingGotten(HttpClient * client, HttpResponse * response) {
	ChainHttpResponse wrappedResponse(response);
	rapidjson::Document d;
	d.Parse<0>(wrappedResponse.getDataString());
	if (!d.HasParseError() && d.IsObject() && d.HasMember("result") && d.HasMember("info")) {
		if (d["result"].GetBool()) {
			unsigned start = 1, end;
			std::string list(d["info"].GetString()), newList;
			while (start < list.size()) {
				end = list.find("|", start);
				if (end == std::string::npos) break;
				newList.append(list.substr(start, end - start));
				newList.push_back('\n');
				start = end + 1;
			}
			updateRanking(newList.c_str());
		} else {
			std::string notice("Uploading failed: ");
			notice.append(d["info"].GetString());
			showNotice(notice.c_str());
		}
	} else {
		showNotice("Bad response data");
	}
}
