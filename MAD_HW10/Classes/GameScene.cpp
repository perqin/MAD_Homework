#include <string>
#include "ui/UIButton.h"
#include "ui/UILoadingBar.h"
#include "GameScene.h"
#include "Utils.h"

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
	
	// There will be 4 layers: Background, Game, Indicator, Control
	// Background
	auto backgroundLayer = Layer::create();
	backgroundLayer->setAnchorPoint(Vec2(0, 0));
	backgroundLayer->setPosition(o.x, o.y);
	auto background = Sprite::create("game-background.jpg");
	background->setPosition(o.x + vs.width / 2, o.y + vs.height / 2);
	backgroundLayer->addChild(background);
	this->addChild(backgroundLayer, 0);
	
	// Game
	auto gameLayer = Layer::create();
	gameLayer->setAnchorPoint(Vec2(0, 0));
	gameLayer->setPosition(o.x, o.y);
	this->addChild(gameLayer, 1);
	// Player
	player = Sprite::createWithSpriteFrameName("character-r-0.png");
	player->setPosition(o.x + vs.width / 2, o.y + vs.height / 2);
	gameLayer->addChild(player);
    // Cure and Suicide
    cure = Sprite::createWithSpriteFrameName("cure-0.png");
    gameLayer->addChild(cure);
    suicide = Sprite::createWithSpriteFrameName("suicide-0.png");
    gameLayer->addChild(suicide);

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

	// Control
	auto controlLayer = Layer::create();
	controlLayer->setAnchorPoint(Vec2(0, 0));
	controlLayer->setPosition(o.x, o.y);
	this->addChild(controlLayer, 3);
	// Buttons
	char buttonKeys[6][2] = { "U", "D", "L", "R", "J", "K" };
	float buttonPos[6][2] = {
		{ o.x + 108, o.y + 108 + 72 },
		{ o.x + 108, o.y + 108 - 72 },
		{ o.x + 108 - 72, o.y + 108 },
        { o.x + 108 + 72, o.y + 108 },
        { o.x + vs.width - 72 - 20, o.y + 72 - 20 },
        { o.x + vs.width - 72 + 20, o.y + 72 + 20 }
	};
    ui::Button * button;
    for (int i = 0; i < 6; ++i) {
        button = ui::Button::create();
        button->setPosition(Vec2(buttonPos[i][0], buttonPos[i][1]));
        button->setTitleText(buttonKeys[i]);
        button->setTitleFontSize(48);
        button->setTitleFontName("fonts/Transformers Movie.ttf");
        button->addTouchEventListener(CC_CALLBACK_2(GameScene::buttonTouched, this, buttonKeys[i][0]));
        controlLayer->addChild(button);
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

    // HP
    hp = 100;

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

void GameScene::handleControl(const char key) {
    Animate * animate;
    char animationName[32];
	switch (key) {
    case 'U':
    case 'D':
    case 'L':
    case 'R':
        sprintf(animationName, "Player%cAnimation", key);
        animate = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
        player->runAction(RepeatForever::create(MoveBy::create(1.0f / 60, playerMoveBy(key))));
        player->runAction(RepeatForever::create(animate));
        break;
    case 'J':
        animate = Animate::create(AnimationCache::getInstance()->getAnimation("CureAnimation"));
        cure->setPosition(player->getPosition());
        cure->runAction(Sequence::create(Spawn::create(Show::create(), animate, NULL), Hide::create(), NULL));
        hp += 10;
        if (hp > 100) hp = 100;
        hpBar->setPercent(hp);
        // Easter eggs
        add1s->setPosition(Vec2(player->getPositionX(), player->getPositionY() + 36));
        add1s->setOpacity(255);
        add1s->runAction(Spawn::create(FadeOut::create(1), MoveBy::create(1, Vec2(0, 48)), NULL));
        break;
    case 'K':
        animate = Animate::create(AnimationCache::getInstance()->getAnimation("SuicideAnimation"));
        suicide->setPosition(player->getPosition());
        suicide->runAction(Sequence::create(Spawn::create(Show::create(), animate, NULL), Hide::create(), NULL));
        hp -= 10;
        if (hp < 0) hp = 0;
        hpBar->setPercent(hp);
        break;
	default:
		break;
	}
}

void GameScene::buttonTouched(Ref * sender, cocos2d::ui::Widget::TouchEventType tet, const char key) {
    switch (tet) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        handleControl(key);
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        player->stopAllActions();
        break;
    default:
        break;
    }
}

Vec2 GameScene::playerMoveBy(const char key) {
    switch (key) {
    case 'U':
        return Vec2(0, 4);
    case 'D':
        return Vec2(0, -4);
    case 'L':
        return Vec2(-4, 0);
    case 'R':
        return Vec2(4, 0);
    default:
        return Vec2::ZERO;
    }
}
