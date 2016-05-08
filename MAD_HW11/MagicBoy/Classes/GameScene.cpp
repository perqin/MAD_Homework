#include <string>
#include "ui/UIButton.h"
#include "ui/UILoadingBar.h"
#include "GameScene.h"
#include "Utils.h"
#include "MenuItemLabel2c.h"

USING_NS_CC;

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

	auto vs = Director::getInstance()->getVisibleSize();
	auto o = Director::getInstance()->getVisibleOrigin();
	
	// There will be 4 layers: Map, Game, Indicator, Control

	// Map
	tmx = TMXTiledMap::create("game_map.tmx");
	tmx->setPosition(o.x + vs.width / 2, o.y + vs.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(tmx, 0);
	TMXObjectGroup * objects = tmx->getObjectGroup("objects");
	ValueMap container = objects->getObject("stair");
	stair.x = container["centerX"].asFloat();
	stair.y = vs.height - container["centerY"].asFloat();
	
	// Game
	gameLayer = Layer::create();
	gameLayer->setAnchorPoint(Vec2(0, 0));
	gameLayer->setPosition(o.x, o.y);
	this->addChild(gameLayer, 1);
	// Player
	player = Sprite::createWithSpriteFrameName("character-r-0.png");
	player->setPosition(o.x + vs.width / 2, o.y + vs.height / 2);
	gameLayer->addChild(player);

	// Indicators
	auto indicatorsLayer = Layer::create();
	indicatorsLayer->setAnchorPoint(Vec2(0, 0));
	indicatorsLayer->setPosition(o.x, o.y);
	this->addChild(indicatorsLayer, 2);
	// Timer indicator
	timer = Label::createWithTTF("180", "fonts/Transformers Movie.ttf", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	timer->setPosition(o.x + vs.width / 2, o.y + vs.height - 64);
	indicatorsLayer->addChild(timer);
	// HP indicator
    hpBar = ui::LoadingBar::create("hp.png");
    hpBar->setPosition(Vec2(o.x + vs.width / 2, o.y + vs.height - 108));
    hpBar->setPercent(100);
    indicatorsLayer->addChild(hpBar);
    // Add1s indicator
    add1s = Label::createWithTTF("+1s", "fonts/arial.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    add1s->setPosition(-100, -100);
    indicatorsLayer->addChild(add1s);

    // New control layer with menu
    auto controlLayer = Menu::create();
    controlLayer->setPosition(o.x, o.y);
    addChild(controlLayer, 3);
    // Control buttons
	char buttonKeys[5][2] = { "W", "S", "A", "D", "J" };
	float buttonPos[5][2] = {
		{ o.x + 108, o.y + 108 + 72 },
		{ o.x + 108, o.y + 108 - 72 },
		{ o.x + 108 - 72, o.y + 108 },
        { o.x + 108 + 72, o.y + 108 },
        { o.x + vs.width - 108, o.y + 108 }
	};
    MenuItemLabel2c * controlButton;
    Label *  controlButtonLabel;
    for (int i = 0; i < 5; ++i) {
        controlButtonLabel = Label::createWithTTF(buttonKeys[i], "fonts/Transformers Movie.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
        controlButton = MenuItemLabel2c::create(controlButtonLabel, CC_CALLBACK_1(GameScene::controlButtonPressed, this, buttonKeys[i][0]), CC_CALLBACK_1(GameScene::controlButtonReleased, this, buttonKeys[i][0]));
        controlButton->setPosition(buttonPos[i][0], buttonPos[i][1]);
        controlLayer->addChild(controlButton);
    }

    // Menu layer
    auto backLabel = Label::createWithTTF("BACK", "fonts/Transformers Movie.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    auto backMenuItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(GameScene::backToMainScene, this));
    backMenuItem->setAnchorPoint(Vec2(1, 1));
    backMenuItem->setPosition(o.x + vs.width - 48, o.y + vs.height - 48);
    auto menu = Menu::create(backMenuItem, NULL);
    menu->setAnchorPoint(Vec2(0, 0));
    menu->setPosition(o.x, o.y);
    this->addChild(menu, 4);

    // Timer schedule
    time = 180;
    schedule(schedule_selector(GameScene::updateTimer), 1, CC_REPEAT_FOREVER, 0);
	schedule(schedule_selector(GameScene::monsterGenerater), 5, CC_REPEAT_FOREVER, 3);
	scheduleUpdate();

    // HP
    hp = 100;

	// Keyboard handler
	auto keyListener = EventListenerKeyboard::create();
	auto thisScene = this;
	keyListener->onKeyPressed = [thisScene](EventKeyboard::KeyCode keyCode, Event * vnt) {
		switch (keyCode) {
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			thisScene->handleControl('W');
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			thisScene->handleControl('S');
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			thisScene->handleControl('A');
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			thisScene->handleControl('D');
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_J:
			thisScene->handleControl('J');
			break;
		default:
			break;
		}
	};
	keyListener->onKeyReleased = [thisScene](EventKeyboard::KeyCode keyCode, Event * vnt) {
		switch (keyCode) {
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			thisScene->releaseControl('W');
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			thisScene->releaseControl('S');
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			thisScene->releaseControl('A');
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			thisScene->releaseControl('D');
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_J:
			thisScene->releaseControl('J');
			break;
		default:
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	keyState = 0;

	return true;
}

void GameScene::backToMainScene(cocos2d::Ref * sender) {
    Director::getInstance()->popScene();
}

void GameScene::updateTimer(float delta) {
    if (time <= 0) {
        timer->unschedule(schedule_selector(GameScene::updateTimer));
    } else {
        --time;
        timer->setString(std::to_string(time));
    }
}

unsigned int GameScene::getKeyFromChar(const char key) {
	switch (key) {
	case 'W':
		return KEY_W;
	case 'S':
		return KEY_S;
	case 'A':
		return KEY_A;
	case 'D':
		return KEY_D;
	default:
		return 0;
	}
}

void GameScene::handleControl(const char key) {
	Animate * animate;
	Action * moveAction, *animateAction;
	char animationName[32];
	switch (key) {
	case 'W':
	case 'S':
	case 'A':
	case 'D':
		SETBIT(keyState, getKeyFromChar(key));
		player->stopAllActionsByTag(ACTION_STOP_AT_RELEASE);
		sprintf(animationName, "Player%cAnimation", key);
		animate = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
		moveAction = RepeatForever::create(MoveBy::create(1.0f / 60, playerMoveBy(key)));
		moveAction->setTag(ACTION_STOP_AT_RELEASE);
		animateAction = RepeatForever::create(animate);
		animateAction->setTag(ACTION_STOP_AT_RELEASE);
		player->runAction(moveAction);
		player->runAction(animateAction);
		break;
	case 'J':
		SETBIT(keyState, KEY_J);
		playerAttack();
		break;
	default:
		break;
	}
}

void GameScene::releaseControl(const char key) {
	switch (key) {
	case 'W':
		UNSETBIT(keyState, KEY_W);
		break;
	case 'S':
		UNSETBIT(keyState, KEY_S);
		break;
	case 'A':
		UNSETBIT(keyState, KEY_A);
		break;
	case 'D':
		UNSETBIT(keyState, KEY_D);
		break;
	case 'J':
		UNSETBIT(keyState, KEY_J);
		break;
	default:
		break;
	}
	if (keyState == 0) {
		player->stopAllActionsByTag(ACTION_STOP_AT_RELEASE);
	}
}

Vec2 GameScene::playerMoveBy(const char key) {
    switch (key) {
    case 'W':
        return Vec2(0, 4);
    case 'S':
        return Vec2(0, -4);
    case 'A':
        return Vec2(-4, 0);
    case 'D':
        return Vec2(4, 0);
    default:
        return Vec2::ZERO;
    }
}

void GameScene::controlButtonPressed(cocos2d::Ref * sender, const char key) {
    handleControl(key);
}

void GameScene::controlButtonReleased(cocos2d::Ref * sender, const char key) {
	releaseControl(key);
}

void GameScene::monsterGenerater(float delta) {
    Vec2 monsterLocation;
    do {
        monsterLocation.x = RandomHelper::random_real(Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width);
        monsterLocation.y = RandomHelper::random_real(Director::getInstance()->getVisibleOrigin().y, Director::getInstance()->getVisibleOrigin().y + Director::getInstance()->getVisibleSize().height);
	} while (fabs(monsterLocation.x - player->getPositionX()) <= player->getContentSize().width / 2 + SpriteFrameCache::getInstance()->getSpriteFrameByName("monster.png")->getRect().size.width / 2
		&& fabs(monsterLocation.y - player->getPositionY()) <= player->getContentSize().height / 2 + SpriteFrameCache::getInstance()->getSpriteFrameByName("monster.png")->getRect().size.height / 2);
	auto monster = Sprite::createWithSpriteFrameName("monster.png");
	monster->setPosition(monsterLocation);
	monster->setTag(SPRITE_MONSTER);
	gameLayer->addChild(monster);
}

void GameScene::update(float delta) {
	// Check goback
	Rect r = player->getBoundingBox();
	if (r.containsPoint(stair)) {
		backToMainScene(this);
		return;
	}
	// Check player be attacked
	Vector<Node *>::iterator m;
	auto ms = gameLayer->getChildren();
	for (m = ms.begin(); m != ms.end(); ++m) {
		if ((*m)->getTag() == SPRITE_MONSTER) {
			if (player->getBoundingBox().intersectsRect((*m)->getBoundingBox())) {
				// Player be attacked
				playerBeAttacked(*m);
			} else {
				// Move monster
				auto direction = player->getPosition() - (*m)->getPosition();
				(*m)->runAction(MoveBy::create(delta, (direction / direction.length()) * 60 * delta));
			}
		}
	}
}

void GameScene::playerAttack() {
	// Find nearest monster
	Vector<Node *>::iterator m;
	auto ms = gameLayer->getChildren();
	float minLengthSquared = Vec2(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height).lengthSquared();
	Node * nearestNode = nullptr;
	for (m = ms.begin(); m != ms.end(); ++m) {
		if ((*m)->getTag() == SPRITE_MONSTER && ((*m)->getPosition() - player->getPosition()).lengthSquared() < minLengthSquared) {
			minLengthSquared = ((*m)->getPosition() - player->getPosition()).lengthSquared();
			nearestNode = *m;
		}
	}
	if (nearestNode == nullptr) return;
	// Show thunder
	auto animate = Animate::create(AnimationCache::getInstance()->getAnimation("SuicideAnimation"));
	auto thunder = Sprite::createWithSpriteFrameName("suicide-0.png");
	thunder->setPosition(nearestNode->getPosition());
	thunder->runAction(Sequence::create(Spawn::create(Show::create(), animate, NULL), Hide::create(), NULL));
	gameLayer->addChild(thunder);
	// Remove monster
	nearestNode->runAction(Sequence::create(FadeOut::create(0.3f), CallFuncN::create([](Node * n) {
		n->getParent()->removeChild(n);
	}), NULL));
	// Get HP back
	hp += 10;
	if (hp > 100) hp = 100;
	hpBar->setPercent(hp);
}

void GameScene::playerBeAttacked(cocos2d::Node * attacker) {
	attacker->setTag(SPRITE_MONSTER_DEAD);
	attacker->runAction(Sequence::create(FadeOut::create(0.3f), CallFuncN::create([](Node * n) {
		n->getParent()->removeChild(n);
	}), NULL));
	player->runAction(Repeat::create(Sequence::create(FadeOut::create(0.18f), FadeIn::create(0.18f), NULL), 3));
	hp -= 10;
	if (hp < 1) hp = 1;
	hpBar->setPercent(hp);
}
