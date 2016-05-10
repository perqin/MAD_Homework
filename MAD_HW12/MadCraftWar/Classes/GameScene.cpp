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

	return true;
}
