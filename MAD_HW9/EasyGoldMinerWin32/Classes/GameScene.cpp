#include "GameScene.h"

USING_NS_CC;

Scene * GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    // Well this 'layer' is actually a Layer not a Scene, though its class name is GameScene.
    // So this layer is the root layer of this scene, like this:
    // Scene (scene)
    // - Layer (layer, which is GameScene instance)
    //   - stone layer
    //   - mouse layer
    //   - ...
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// On "init" you need to initialize your instance
bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    //add touch listener
    EventListenerTouchOneByOne * listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Add level-background
    auto levelBackground = Sprite::create("level-background-0.jpg");
    levelBackground->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(levelBackground, 0);
    
    // MouseLayer
    mouseLayer = Layer::create();
    mouseLayer->setAnchorPoint(Vec2(0, 0));
    mouseLayer->setPosition(0, Director::getInstance()->getVisibleSize().height / 2);
    this->addChild(mouseLayer, 2);
    mouse = Sprite::createWithSpriteFrameName("mouse-0.png");
    Animate * mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
    mouse->runAction(RepeatForever::create(mouseAnimate));
    mouse->setPosition(visibleSize.width / 2, 0);
    mouseLayer->addChild(mouse);
    
    // StoneLayer
    stoneLayer = Layer::create();
    stoneLayer->setAnchorPoint(Vec2(0, 0));
    stoneLayer->setPosition(0, 0);
    this->addChild(stoneLayer, 2);
    stone = Sprite::create("stone.png");
    stone->setPosition(560, 480);
    stoneLayer->addChild(stone);

    // Add shoot button
    auto shootLabel = Label::createWithTTF("SHOOT", "fonts/Marker Felt.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    auto shootItem = MenuItemLabel::create(shootLabel, CC_CALLBACK_1(GameScene::shootMenuCallback, this));
    shootItem->setPosition(origin.x + visibleSize.width - shootItem->getContentSize().width - 60, origin.y + visibleSize.height - shootItem->getContentSize().height - 40);
    auto menu = Menu::create(shootItem, NULL);
    menu->setAnchorPoint(Vec2(0, 0));
    menu->setPosition(origin.x, origin.y);
    this->addChild(menu, 3);

    return true;
}

bool GameScene::onTouchBegan(Touch * touch, Event * unusedEvent) {
    auto location = touch->getLocation();
    auto cheese = Sprite::create("cheese.png");
    cheese->setPosition(location);
    this->addChild(cheese, 1);
    auto cheeseFade = TargetedAction::create(cheese, FadeOut::create(3));
    auto cheeseRemoved = TargetedAction::create(cheese, CallFuncN::create(CC_CALLBACK_1(GameScene::removeCheese, this)));
    mouse->stopAllActions();
    mouse->runAction(Sequence::create(MoveTo::create(1, Vec2(location.x, location.y - Director::getInstance()->getVisibleSize().height / 2)), cheeseFade, cheeseRemoved, NULL));
    return true;
}

void GameScene::shootMenuCallback(Ref * pSender) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto stone = Sprite::create("stone.png");
    stone->setPosition(this->stone->getPosition());
    stone->runAction(Sequence::create(MoveTo::create(0.4f, Vec2(mouse->getPosition().x, mouse->getPosition().y + visibleSize.height / 2)),
        CallFuncN::create(CC_CALLBACK_1(GameScene::removeStone, this)),
        NULL
    ));
    stoneLayer->addChild(stone);

	Vec2 gone(RandomHelper::random_real(origin.x, origin.x + visibleSize.width), RandomHelper::random_real(origin.y - visibleSize.height / 2, origin.y + visibleSize.height / 2));
	mouse->stopAllActions();
	mouse->runAction(MoveTo::create(0.5, gone));

	auto gem = Sprite::create("diamond.png");
	gem->setPosition(Vec2(mouse->getPosition().x, mouse->getPosition().y + visibleSize.height / 2));
	this->addChild(gem, 1);
}

void GameScene::removeCheese(Node * chs) {
    this->removeChild(chs);
}

void GameScene::removeStone(Node * stn) {
    stoneLayer->removeChild(stn);
}
