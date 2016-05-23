#include "BreakoutScene.h"
#include <string>
using namespace std;
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

void Breakout::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* Breakout::createScene() {
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Point(0, 0));

    auto layer = Breakout::create(scene->getPhysicsWorld());

    scene->addChild(layer);
    return scene;
}

bool Breakout::init(PhysicsWorld* world) {
    if (!Layer::init()) {
        return false;
    }

	this->setPhysicsWorld(world);
    visibleSize = Director::getInstance()->getVisibleSize();

    preloadMusic();
    //playBgm();

    addBackground();
    addEdge();
    addPlayer();

	addContactListener();
    addTouchListener();
    addKeyboardListener();

	timeLeft = 30;
	lifeLeft = 10;

    this->schedule(schedule_selector(Breakout::update), 1);
	this->schedule(schedule_selector(Breakout::timeCountdown), 1);

    return true;
}

Breakout * Breakout::create(PhysicsWorld * world) {
	Breakout* pRet = new(std::nothrow) Breakout();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

void Breakout::preloadMusic() {
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
    // 
}

void Breakout::playBgm() {
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
    // 
}

void Breakout::addBackground() {
    auto bgsprite = Sprite::create("black_hole_bg0.jpg");
    bgsprite->setPosition(visibleSize / 2);
    bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, visibleSize.height / bgsprite->getContentSize().height);
    this->addChild(bgsprite, 0);

    auto ps = ParticleSystemQuad::create("black_hole.plist");
    ps->setPosition(visibleSize / 2);
    this->addChild(ps);
}

void Breakout::addEdge() {
    auto edgeSp = Sprite::create();
    auto boundBody = PhysicsBody::createEdgeBox(visibleSize);
	boundBody->setDynamic(false);
    boundBody->setTag(0);
    edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    edgeSp->setPhysicsBody(boundBody);
	edgeSp->getPhysicsBody()->setCategoryBitmask(4);
    this->addChild(edgeSp);
}

void Breakout::addPlayer() {
    player = Sprite::create("player.png");
    player->setAnchorPoint(Vec2(0.5, 0.5));
    player->setPhysicsBody(PhysicsBody::createCircle(32));
	// Player uses 0x00000001
	player->getPhysicsBody()->setCategoryBitmask(1);
	// Player will collide with 0x00000002 and 0x00000004
	player->getPhysicsBody()->setCollisionBitmask(6);
	// Player only dispatch event when colliding with 0x00000002
	player->getPhysicsBody()->setContactTestBitmask(2);
    player->setPosition(visibleSize / 2);
    player->getPhysicsBody()->setTag(1);
    player->getPhysicsBody()->setAngularVelocityLimit(0);
    addChild(player);
}

void Breakout::addContactListener() {
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Breakout::onConcactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);
}

void Breakout::addTouchListener(){
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Breakout::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Breakout::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Breakout::onTouchEnded,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void Breakout::addKeyboardListener() {
    auto keboardListener = EventListenerKeyboard::create();
    keboardListener->onKeyPressed = CC_CALLBACK_2(Breakout::onKeyPressed, this);
    keboardListener->onKeyReleased = CC_CALLBACK_2(Breakout::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keboardListener, this);
}

bool Breakout::onTouchBegan(Touch *touch, Event *unused_event) {
    Vec2 postion = touch->getLocation();
    // do something
    //
    return true;
}

void Breakout::onTouchMoved(Touch *touch, Event *unused_event) {
    // TODO
    //
}

void Breakout::onTouchEnded(Touch *touch, Event *unused_event) {
    // TODO
    //
}

bool Breakout::onConcactBegan(PhysicsContact& contact) {
	if (lifeLeft <= 0 || timeLeft <= 0) return false;
	auto s1 = (Sprite *)contact.getShapeA()->getBody()->getNode();
	auto s2 = (Sprite *)contact.getShapeB()->getBody()->getNode();
	int multi = s1->getPhysicsBody()->getTag() * s2->getPhysicsBody()->getTag();
	if (multi > 9 && multi < 100) {
		auto ps = ParticleExplosion::create();
		ps->setStartColor(Color4F::ORANGE);
		ps->setEndColor(Color4F::YELLOW);
		if (s1->getPhysicsBody()->getTag() == 1) {
			ps->setPosition(s2->getPosition());
			enemys.eraseObject(s2->getPhysicsBody());
			s2->removeFromParent();
		} else {
			ps->setPosition(s1->getPosition());
			enemys.eraseObject(s1->getPhysicsBody());
			s1->removeFromParent();
		}
		this->addChild(ps);
		--lifeLeft;
		if (lifeLeft <= 0) {
			// Lose
			this->unschedule(schedule_selector(Breakout::update));
			this->unschedule(schedule_selector(Breakout::timeCountdown));
			auto loseLabel = Label::createWithTTF("YOU LOSE!!!", "fonts/Transformers Movie.ttf", 64, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
			loseLabel->setPosition(visibleSize / 2);
			loseLabel->runAction(FadeIn::create(0.5f));
			addChild(loseLabel, 10);
		} else {
			auto lifeLabel = Label::createWithTTF(std::to_string(lifeLeft), "fonts/Transformers Movie.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
			lifeLabel->setColor(Color3B::RED);
			lifeLabel->setPosition(visibleSize / 2);
			lifeLabel->runAction(Sequence::create(FadeOut::create(1), CallFuncN::create([](Node * n) {
				n->removeFromParent();
			}), nullptr));
			addChild(lifeLabel, 10);
		}
	}
	return true;
}

void Breakout::update(float f) {
    newEnemys();
}

void Breakout::newEnemys() {
	if (enemys.size() > 50) return;
    int newNum = 2;
    while (newNum--) {
        int type = 0;
        if (CCRANDOM_0_1() > 0.85) { type = 2; }
        else if (CCRANDOM_0_1() > 0.6) { type = 1; }

        Point location = Vec2(0, 0);
        switch (rand() % 4)
        {
        case 0:
            location.y = visibleSize.height;
            location.x = rand() % (int)(visibleSize.width);
            break;
        case 1:
            location.x = visibleSize.width;
            location.y = rand() % (int)(visibleSize.height);
            break;
        case 2:
            location.y = 0;
            location.x = rand() % (int)(visibleSize.width);
            break;
        case 3:
            location.x = 0;
            location.y = rand() % (int)(visibleSize.height);
            break;
        default:
            break;
        }
        addEnemy(type, location);
    }
}

void Breakout::addEnemy(int type, Point p) {
    char path[100];
    int tag;
    switch (type)
    {
    case 0:
        sprintf(path, "stone1.png");
        tag = 10;
        break;
    case 1:
        sprintf(path, "stone2.png");
        tag = 20;
        break;
    case 2:
        sprintf(path, "stone3.png");
        tag = 30;
        break;
    default:
        sprintf(path, "stone1.png");
        tag = 10;
        break;
    }
    auto re = Sprite::create(path);
    re->setPhysicsBody(PhysicsBody::createCircle(re->getContentSize().height / 2));
    re->setAnchorPoint(Vec2(0.5, 0.5));
    re->setScale(0.5, 0.5);
    re->setPosition(p);
	re->getPhysicsBody()->setCategoryBitmask(2);
	re->getPhysicsBody()->setCollisionBitmask(7);
	re->getPhysicsBody()->setContactTestBitmask(1);
    re->getPhysicsBody()->setTag(tag);
    if (rand() % 100 < 50) {
        re->getPhysicsBody()->setVelocity((player->getPosition() - p) * (0.25));
    }
    else {
        re->getPhysicsBody()->setVelocity((Point(rand() % (int)(visibleSize.width - 100) + 50, rand() % (int)(visibleSize.height - 100) + 50) - p) * (0.25));
    }
    re->getPhysicsBody()->setAngularVelocity(CCRANDOM_0_1() * 10);
    enemys.pushBack(re->getPhysicsBody());
    addChild(re);
}

void Breakout::timeCountdown(float delta) {
	if (timeLeft <= 0) {
		// Win
		this->unschedule(schedule_selector(Breakout::update));
		this->unschedule(schedule_selector(Breakout::timeCountdown));
		auto winLabel = Label::createWithTTF("YOU WIN!!!", "fonts/Transformers Movie.ttf", 64, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		winLabel->setPosition(visibleSize / 2);
		winLabel->runAction(FadeIn::create(0.5f));
		addChild(winLabel, 10);
	} else {
		--timeLeft;
		auto timeLabel = Label::createWithTTF(std::to_string(timeLeft), "fonts/Transformers Movie.ttf", 24, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		timeLabel->setPosition(visibleSize.width - 60, visibleSize.height - 30);
		timeLabel->runAction(Sequence::create(FadeOut::create(1), CallFuncN::create([](Node * n) {
			n->removeFromParent();
		}), nullptr));
		addChild(timeLabel, 10);
	}
}

void Breakout::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        player->getPhysicsBody()->setVelocity(Point(-200, player->getPhysicsBody()->getVelocity().y));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        player->getPhysicsBody()->setVelocity(Point(200, player->getPhysicsBody()->getVelocity().y));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        player->getPhysicsBody()->setVelocity(Point(player->getPhysicsBody()->getVelocity().x, 200));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        player->getPhysicsBody()->setVelocity(Point(player->getPhysicsBody()->getVelocity().x, -200));
        break;
    default:
        break;
    }
}

void Breakout::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(-200, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(200, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(0, 200));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(0, -200));
        break;
    default:
        break;
    }
}
