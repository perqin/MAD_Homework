#include "GameScene.h"
#include "Utils.h"
#include "C.h"

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

	auto o = Director::getInstance()->getVisibleOrigin();
	auto vs = Director::getInstance()->getVisibleSize();

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
	backgroundLayer->setPosition(o.x, o.y);
	addChild(backgroundLayer, 0);
	// Background image
	auto background = Sprite::create("game-background.png");
	background->setPosition(o.x + vs.width / 2, o.y + vs.height / 2);
	backgroundLayer->addChild(background);

	// GameObjects layer
	auto gameObjectsLayer = Layer::create();
	gameObjectsLayer->setAnchorPoint(Vec2::ZERO);
	gameObjectsLayer->setPosition(o.x, o.y);
	addChild(gameObjectsLayer, 1);
	player = Sprite::create("player-craft.png");
	player->setPosition(o.x + vs.width / 2, o.y + 96);
	gameObjectsLayer->addChild(player);

	// Indicators layer
	auto indicatorsLayer = Layer::create();
	indicatorsLayer->setAnchorPoint(Vec2::ZERO);
	indicatorsLayer->setPosition(o.x, o.y);
	addChild(indicatorsLayer, 2);

	// Other initializations
	//playerMoveBy = MoveBy::create(1.0f / 60, Vec2::ZERO);

	// Init game runtimes
	controlState = 0;
	playerStates.velocity = Vec2::ZERO;
	playerStates.movingBox = Rect(o.x, o.y, vs.width, vs.height);

	// Init keyboard listener
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressedHandler, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleasedHandler, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	// Start update
	scheduleUpdate();

	return true;
}

void GameScene::update(float delta) {
	// Player movement
	//Vec2 oldPlayerVelocity = playerVelocity;
	if (ISSETBIT(controlState, CONTROL_PLAYER1_LEFT)) {
		playerStates.velocity.x = player->getPosition().x > playerStates.movingBox.getMinX() ? -C::PLAYER_VELOCITY_NORMAL_X : 0;
	} else if (ISSETBIT(controlState, CONTROL_PLAYER1_RIGHT)) {
		playerStates.velocity.x = player->getPosition().x < playerStates.movingBox.getMaxX() ? C::PLAYER_VELOCITY_NORMAL_X : 0;
	} else {
		playerStates.velocity.x = 0;
	}
	if (ISSETBIT(controlState, CONTROL_PLAYER1_UP)) {
		playerStates.velocity.y = player->getPosition().y < playerStates.movingBox.getMaxY() ? C::PLAYER_VELOCITY_NORMAL_Y : 0;
	} else if (ISSETBIT(controlState, CONTROL_PLAYER1_DOWN)) {
		playerStates.velocity.y = player->getPosition().y > playerStates.movingBox.getMinY() ? -C::PLAYER_VELOCITY_NORMAL_Y : 0;
	} else {
		playerStates.velocity.y = 0;
	}
	player->runAction(MoveBy::create(delta, playerStates.velocity * delta));
	//if (playerVelocity != oldPlayerVelocity) {
	//}
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
		UNSETBIT(controlState, CONTROL_PLAYER1_DOWN);
		SETBIT(controlState, CONTROL_PLAYER1_UP);
		break;
	case Controls::Player1Down:
		UNSETBIT(controlState, CONTROL_PLAYER1_UP);
		SETBIT(controlState, CONTROL_PLAYER1_DOWN);
		break;
	case Controls::Player1Left:
		UNSETBIT(controlState, CONTROL_PLAYER1_RIGHT);
		SETBIT(controlState, CONTROL_PLAYER1_LEFT);
		break;
	case Controls::Player1Right:
		UNSETBIT(controlState, CONTROL_PLAYER1_LEFT);
		SETBIT(controlState, CONTROL_PLAYER1_RIGHT);
		break;
	case Controls::Player1Fire:
		SETBIT(controlState, CONTROL_PLAYER1_FIRE);
		break;
	default:
		break;
	}
}

void GameScene::onControlUp(Controls c) {
	switch (c) {
	case Controls::Player1Up:
		UNSETBIT(controlState, CONTROL_PLAYER1_UP);
		break;
	case Controls::Player1Down:
		UNSETBIT(controlState, CONTROL_PLAYER1_DOWN);
		break;
	case Controls::Player1Left:
		UNSETBIT(controlState, CONTROL_PLAYER1_LEFT);
		break;
	case Controls::Player1Right:
		UNSETBIT(controlState, CONTROL_PLAYER1_RIGHT);
		break;
	case Controls::Player1Fire:
		UNSETBIT(controlState, CONTROL_PLAYER1_FIRE);
		break;
	default:
		break;
	}
}
