#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;


static cocos2d::Size designResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size smallResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages() {
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MadCraftWar", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("MadCraftWar");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    Size frameSize = glview->getFrameSize();
    if (frameSize.height > mediumResolutionSize.height) {
    // if the frame's height is larger than the height of medium size. 
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    } else if (frameSize.height > smallResolutionSize.height) {   
		// if the frame's height is larger than the height of small size.     
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    } else {
		// if the frame's height is smaller than the height of medium size.
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

	// Load game resource
	//SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("monster.png", Rect(0, 0, 48, 72)), "monster.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game-sheet.plist");
	// Explosion animation
	char frameName[64];
	Animation * animation = Animation::create();
	for (int i = 0; i < 15; ++i) {
		sprintf(frameName, "explosion-%d.png", i);
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	animation->setDelayPerUnit(0.1f);
	AnimationCache::getInstance()->addAnimation(animation, "ExplosionAnimation");

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("background.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("fire.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("explosion.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("hurt.wav");

    // create a scene. it's an autorelease object
    auto scene = GameScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
