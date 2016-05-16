#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Utils.h"
#include "C.h"
#include "EnemyBeAttackedEventData.h"
#include "EnemyHitPlayerEventData.h"

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

	/**
	Layers structure
	GameScene
	  + HostLayer
	    + BackgroundLayer
		+ GameObjectsLayer
		+ IndicatorsLayer
	*/

	// Background layer
	auto backgroundLayer = Layer::create();
	backgroundLayer->setAnchorPoint(Vec2::ZERO);
	backgroundLayer->setPosition(vo.x, vo.y);
	addChild(backgroundLayer, 0);
	// Background image
	auto background = Sprite::create("game-background.jpg");
	background->setPosition(vo.x + vs.width / 2, vo.y + vs.height / 2);
	background->setScale(vs.height / background->getContentSize().height);
	backgroundLayer->addChild(background);

	// GameObjects layer
	gameObjectsLayer = Layer::create();
	gameObjectsLayer->setAnchorPoint(Vec2::ZERO);
	gameObjectsLayer->setPosition(vo.x, vo.y);
	addChild(gameObjectsLayer, 1);
	player = Sprite::createWithSpriteFrameName("player-craft.png");
	player->setTag(PLAYER1);
	player->setPosition(vo.x + vs.width / 2, vo.y + 96);
	gameObjectsLayer->addChild(player);

	// Indicators layer
	indicatorsLayer = Layer::create();
	indicatorsLayer->setAnchorPoint(Vec2::ZERO);
	indicatorsLayer->setPosition(vo.x, vo.y);
	addChild(indicatorsLayer, 2);
	hpBarBackground = ui::Scale9Sprite::createWithSpriteFrameName("hp-bar-border.png", Rect(16, 16, 32, 32));
	hpBarBackground->setContentSize(Size(512, 64));
	hpBarBackground->setPosition(vo.x + 240 + 16, vo.y + vs.height - 48);
	indicatorsLayer->addChild(hpBarBackground);
	hpBar = ui::LoadingBar::create("hp-bar.png");
	hpBar->setPosition(Vec2(vo.x + 240 + 16, vo.y + vs.height - 48));
	indicatorsLayer->addChild(hpBar);
	scoreIndicator = Label::createWithTTF("", "fonts/Transformers Movie.ttf", 36, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	scoreIndicator->setPosition(vo.x + vs.width - 64, vo.y + vs.height - 48);
	indicatorsLayer->addChild(scoreIndicator);

	// Other initializations

	// Init game states
	gameStates.controlState = 0;
	gameStates.enemyGenerateCountdown = 0;
	gameStates.nextEnemyIntervalMin = 0;
	gameStates.nextEnemyIntervalMax = 300;

	// Init player states
	playerStates.velocity = Vec2::ZERO;
	playerStates.movingBox = Rect(vo.x, vo.y, vs.width, vs.height);
	playerStates.maxHp = 333;
	setHp(333);
	setScore(0);

	// Init keyboard listener
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressedHandler, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleasedHandler, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	// Init custom listeners
	Director::getInstance()->getEventDispatcher()->addCustomEventListener("EnemyBeAttacked", CC_CALLBACK_1(GameScene::onEnemyBeAttacked, this));
	Director::getInstance()->getEventDispatcher()->addCustomEventListener("PlayerBeAttacked", CC_CALLBACK_1(GameScene::onPlayerBeAttacked, this));
	Director::getInstance()->getEventDispatcher()->addCustomEventListener("EnemyHitPlayer", CC_CALLBACK_1(GameScene::onEnemyHitPlayer, this));

	// Start update
	scheduleUpdate();

	// Show game info
	showNotification("Don't lose the game,\nor it will quit!");

	// Play BGM
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);

	return true;
}

void GameScene::update(float delta) {
	// Player movement
	if (ISSETBIT(gameStates.controlState, CONTROL_PLAYER1_LEFT)) {
		playerStates.velocity.x = player->getPositionX() > playerStates.movingBox.getMinX() ? -C::PLAYER_VELOCITY_NORMAL_X : 0;
	} else if (ISSETBIT(gameStates.controlState, CONTROL_PLAYER1_RIGHT)) {
		playerStates.velocity.x = player->getPositionX() < playerStates.movingBox.getMaxX() ? C::PLAYER_VELOCITY_NORMAL_X : 0;
	} else {
		playerStates.velocity.x = 0;
	}
	if (ISSETBIT(gameStates.controlState, CONTROL_PLAYER1_UP)) {
		playerStates.velocity.y = player->getPositionY() < playerStates.movingBox.getMaxY() ? C::PLAYER_VELOCITY_NORMAL_Y : 0;
	} else if (ISSETBIT(gameStates.controlState, CONTROL_PLAYER1_DOWN)) {
		playerStates.velocity.y = player->getPositionY() > playerStates.movingBox.getMinY() ? -C::PLAYER_VELOCITY_NORMAL_Y : 0;
	} else {
		playerStates.velocity.y = 0;
	}
	player->runAction(MoveBy::create(delta, playerStates.velocity * delta));
	// Create new enemy craft countdown
	if (gameStates.enemyGenerateCountdown > 0) {
		--gameStates.enemyGenerateCountdown;
	} else {
		generateEnemy();
		getNextEnemyInterval();
	}
	// Overlay judgements
	while (!eventDataQueue.empty()) eventDataQueue.pop();
	for (auto b : playerBullets) {
		for (auto e : enemies) {
			if (e->getBoundingBox().intersectsRect(b->getBoundingBox())) {
				eventDataQueue.push(EnemyBeAttackedEventData::create(b, e));
			}
		}
	}
	while (!eventDataQueue.empty()) {
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EnemyBeAttacked", (void *)eventDataQueue.front());
		eventDataQueue.pop();
	}
	for (auto e : enemies) {
		if (player->getBoundingBox().intersectsRect(e->getBoundingBox())) {
			eventDataQueue.push((Ref *)EnemyHitPlayerEventData::create(e, player));
		}
	}
	while (!eventDataQueue.empty()) {
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EnemyHitPlayer", (void *)eventDataQueue.front());
		eventDataQueue.pop();
	}
}

GameScene::Controls GameScene::toControl(EventKeyboard::KeyCode keyCode) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_W:
		return Controls::Player1Up;
	case EventKeyboard::KeyCode::KEY_S:
		return Controls::Player1Down;
	case EventKeyboard::KeyCode::KEY_A:
		return Controls::Player1Left;
	case EventKeyboard::KeyCode::KEY_D:
		return Controls::Player1Right;
	case EventKeyboard::KeyCode::KEY_J:
		return Controls::Player1Fire;
	default:
		return Controls::Invalid;
	}
}

void GameScene::onKeyPressedHandler(EventKeyboard::KeyCode keyCode, Event * e) {
	onControlDown(toControl(keyCode));
}

void GameScene::onKeyReleasedHandler(EventKeyboard::KeyCode keyCode, Event * e) {
	onControlUp(toControl(keyCode));
}

void GameScene::onControlHit(Controls c) {}

void GameScene::onControlDown(Controls c) {
	switch (c) {
	case Controls::Player1Up:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_DOWN);
		SETBIT(gameStates.controlState, CONTROL_PLAYER1_UP);
		break;
	case Controls::Player1Down:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_UP);
		SETBIT(gameStates.controlState, CONTROL_PLAYER1_DOWN);
		break;
	case Controls::Player1Left:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_RIGHT);
		SETBIT(gameStates.controlState, CONTROL_PLAYER1_LEFT);
		break;
	case Controls::Player1Right:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_LEFT);
		SETBIT(gameStates.controlState, CONTROL_PLAYER1_RIGHT);
		break;
	case Controls::Player1Fire:
		SETBIT(gameStates.controlState, CONTROL_PLAYER1_FIRE);
		schedule(schedule_selector(GameScene::player1BulletEmitter), C::PLAYER_NORMAL_BULLET_INTERVAL, CC_REPEAT_FOREVER, 1.0f / 60);
		break;
	default:
		break;
	}
}

void GameScene::onControlUp(Controls c) {
	switch (c) {
	case Controls::Player1Up:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_UP);
		break;
	case Controls::Player1Down:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_DOWN);
		break;
	case Controls::Player1Left:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_LEFT);
		break;
	case Controls::Player1Right:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_RIGHT);
		break;
	case Controls::Player1Fire:
		UNSETBIT(gameStates.controlState, CONTROL_PLAYER1_FIRE);
		unschedule(schedule_selector(GameScene::player1BulletEmitter));
		break;
	default:
		break;
	}
}

void GameScene::player1BulletEmitter(float delta) {
	auto thisScene = this;
	Vec2 des(player->getPositionX(), vo.y + vs.height);
	auto bullet = Sprite::createWithSpriteFrameName("player-bullet-normal.png");
	bullet->setTag(PLAYER_BULLET_NORMAL);
	bullet->setPosition(player->getPositionX(), player->getPositionY() + player->getContentSize().height / 2 + bullet->getContentSize().height / 2);
	bullet->runAction(Sequence::createWithTwoActions(MoveTo::create((des.y - player->getPositionY()) / C::PLAYER_NORMAL_BULLET_VELOCITY_NORMAL_Y, des), CallFuncN::create([this](Node * node) {
		this->removePlayerBullet(node);
	})));
	attachPlayerBullet(bullet);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("fire.wav");
}

void GameScene::generateEnemy() {
	char enemyFileName[64];
	sprintf(enemyFileName, "enemy-normal-%d.png", RandomHelper::random_int(0, 5));
	auto enemy = Sprite::createWithSpriteFrameName(enemyFileName);
	enemy->setRotation(90);
	enemy->setPosition(RandomHelper::random_real(vo.x, vo.x + vs.width), vo.y + vs.height + enemy->getContentSize().height / 2);
	// Actions may be different
	enemy->setTag(ENEMY_NORMAL);
	Vec2 des;
	des.y = vo.y - enemy->getContentSize().height / 2;
	float x1 = enemy->getPositionX(), y1 = enemy->getPositionY(), x2 = player->getPositionX(), y2 = player->getPositionY();
	des.x = (x2 * (y1 - des.y) - x1 * (y2 - des.y)) / (y1 - y2);
	enemy->runAction(Sequence::createWithTwoActions(MoveTo::create((des - enemy->getPosition()).length() / C::NORMAL_ENEMY_VELOCITY_NORMAL, des), CallFuncN::create([this](Node * node) {
		this->removeEnemy(node);
	})));
	attachEnemy(enemy);
}

void GameScene::getNextEnemyInterval() {
	gameStates.enemyGenerateCountdown = RandomHelper::random_int(gameStates.nextEnemyIntervalMin, gameStates.nextEnemyIntervalMax);
}

void GameScene::onPlayerBeAttacked(EventCustom * e) {
	CCLOG("onPlayerBeAttacked");
}

void GameScene::onEnemyBeAttacked(EventCustom * e) {
	setScore(playerStates.score + 10);
	auto data = (EnemyBeAttackedEventData *)e->getUserData();
	attachExplosion(data->getEnemy()->getPosition());
	removePlayerBullet(data->getAttacker());
	removeEnemy(data->getEnemy());
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion.wav");
}

void GameScene::onEnemyHitPlayer(EventCustom * e) {
	auto data = (EnemyHitPlayerEventData *)e->getUserData();
	data->getPlayer()->runAction(Repeat::create(Sequence::createWithTwoActions(TintBy::create(0.1f, 128, 0, 0), TintBy::create(0.1f, 0, 0, 0)), 2));
	removeEnemy(data->getEnemy());
	setHp(playerStates.hp - 40);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hurt.wav");
}

void GameScene::attachEnemy(Node * e) {
	gameObjectsLayer->addChild(e);
	enemies.pushBack(e);
}

void GameScene::attachPlayerBullet(Node * b) {
	gameObjectsLayer->addChild(b);
	playerBullets.pushBack(b);
}

void GameScene::attachEnemyBullet(Node * b) {
	gameObjectsLayer->addChild(b);
	enemyBullets.pushBack(b);
}

void GameScene::removeEnemy(Node * e) {
	//e->stopAllActions();
	e->getParent()->removeChild(e);
	enemies.eraseObject((Sprite *)e);
}

void GameScene::removePlayerBullet(Node * b) {
	//b->stopAllActions();
	b->getParent()->removeChild(b);
	playerBullets.eraseObject(b);
}

void GameScene::removeEnemyBullet(Node * b) {
	//b->stopAllActions();
	b->getParent()->removeChild(b);
	enemyBullets.eraseObject((Sprite *)b);
}

void GameScene::attachExplosion(const Vec2 & pos) {
	auto explosion = Sprite::createWithSpriteFrameName("explosion-0.png");
	explosion->setPosition(pos);
	explosion->runAction(Sequence::createWithTwoActions(Animate::create(AnimationCache::getInstance()->getAnimation("ExplosionAnimation")), CallFuncN::create([](Node * n) {
		n->getParent()->removeChild(n);
	})));
	gameObjectsLayer->addChild(explosion);
}

void GameScene::setScore(int score) {
	if (playerStates.score / 50 < score / 50) {
		if (gameStates.nextEnemyIntervalMax > 70) {
			gameStates.nextEnemyIntervalMax -= 70;
			showNotification("LEVEL UP");
		}
	}
	playerStates.score = score;
	scoreIndicator->setString(std::to_string(score));
}

void GameScene::setHp(int hp) {
	if (hp > 0) {
		playerStates.hp = hp;
	} else {
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
	hpBar->setPercent((float)playerStates.hp / playerStates.maxHp * 100.0);
}

void GameScene::showNotification(const char * n) {
	auto notification = Label::createWithTTF(n, "fonts/Transformers Movie.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	notification->setPosition(vo.x + vs.width / 2, vo.y + vs.height / 2);
	notification->setOpacity(0);
	notification->runAction(Sequence::create(FadeIn::create(0.5f), TintBy::create(3, 0, 0, 0), FadeOut::create(0.5f), nullptr));
	indicatorsLayer->addChild(notification);
}
