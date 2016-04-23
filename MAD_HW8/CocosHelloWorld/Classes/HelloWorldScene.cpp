#include "HelloWorldScene.h"
#include "S.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.


	auto infoItem = MenuItemImage::create("InfoNormal.png", "InfoSelected.png", CC_CALLBACK_1(HelloWorld::menuInfoCallback, this));
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	infoItem->setPosition(Vec2(-closeItem->getContentSize().width - infoItem->getContentSize().width, 0));
	closeItem->setPosition(Vec2(-closeItem->getContentSize().width / 2, 0));

    // create menu, it's an autorelease object
    auto menu = Menu::create(infoItem, closeItem, NULL);
    menu->setPosition(Vec2(origin.x + visibleSize.width, origin.y + 20));
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

	auto label = Label::createWithTTF(S::get("my_name") + "\n14331304", "fonts/huawenxingkai.ttf", 24, Size::ZERO, cocos2d::TextHAlignment::CENTER);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("Background.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setScale(0.5);

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}

void HelloWorld::menuInfoCallback(Ref* pSender) {
	//TODO: Show sth.
	//This TODO will be done after I learn how to control other sprites in the scene.
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
