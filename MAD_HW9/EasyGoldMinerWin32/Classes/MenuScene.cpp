#include "MenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MenuScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init() {

    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto skyBackground = Sprite::create("menu-background-sky.jpg");
    skyBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
    this->addChild(skyBackground, 0);

    auto menuBackground = Sprite::create("menu-background.png");
    menuBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
    this->addChild(menuBackground, 0);

    auto title = Sprite::create("gold-miner-text.png");
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height + origin.y - 30));
    this->addChild(title, 1);

    auto miner = Sprite::create("menu-miner.png");
    miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
    this->addChild(miner, 1);

    auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
    Animate * legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
    leg->runAction(RepeatForever::create(legAnimate));
    leg->setPosition(110 + origin.x, origin.y + 102);
    this->addChild(leg, 1);

    auto normalStartSprite = Sprite::create("menu-start-gold.png");
    auto normalStartTitle = Sprite::create("start-0.png");
    normalStartTitle->setPosition(normalStartSprite->getContentSize().width / 2, normalStartSprite->getContentSize().height / 2);
    normalStartSprite->addChild(normalStartTitle);
    auto pressedStartSprite = Sprite::create("menu-start-gold.png");
    auto pressedStartTitle = Sprite::create("start-1.png");
    pressedStartTitle->setPosition(pressedStartSprite->getContentSize().width / 2, pressedStartSprite->getContentSize().height / 2);
    pressedStartSprite->addChild(pressedStartTitle);
    auto startButton = MenuItemSprite::create(normalStartSprite, pressedStartSprite, CC_CALLBACK_1(MenuScene::startButtonCallback, this));
    auto menu = Menu::create(startButton, NULL);
    menu->setPosition(origin.x + visibleSize.width - startButton->getContentSize().width / 2 - 20, origin.y + startButton->getContentSize().height / 2 + 20);
    this->addChild(menu, 2);

    return true;
}

void MenuScene::startButtonCallback(cocos2d::Ref * pSender) {
    auto gameScene = GameScene::createScene();
    Director::getInstance()->pushScene(TransitionFade::create(0.5, gameScene));
}
