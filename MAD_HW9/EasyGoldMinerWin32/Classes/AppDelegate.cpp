#include "AppDelegate.h"
#include "MenuScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate()
{
}

// If you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
//static int register_all_packages()
//{
//	return 0; //flag for packages manager
//}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::createWithRect("EasyGoldMinerWin32", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);

    //Size frameSize = glview->getFrameSize();
    //// if the frame's height is larger than the height of medium size.
    //if (frameSize.height > mediumResolutionSize.height)
    //{
    //	director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    //}
    //// if the frame's height is larger than the height of small size.
    //else if (frameSize.height > smallResolutionSize.height)
    //{
    //	director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    //}
    //// if the frame's height is smaller than the height of medium size.
    //else
    //{
    //	director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    //}
    //
    //register_all_packages();

    // Load game resource
    char totalFrames;
    char frameName[32];
    Animation * animation;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("general-sheet.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("level-sheet.plist");
    // For leg
    totalFrames = 3;
    animation = Animation::create();
    for (int i = 0; i < totalFrames; ++i) {
        sprintf(frameName, "miner-leg-%d.png", i);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
    }
    animation->setDelayPerUnit(0.1f);
    AnimationCache::getInstance()->addAnimation(animation, "legAnimation");
    // For mouse
    totalFrames = 8;
    animation = Animation::create();
    for (int i = 0; i < totalFrames; ++i) {
        sprintf(frameName, "mouse-%d.png", i);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
    }
    animation->setDelayPerUnit(0.1f);
    AnimationCache::getInstance()->addAnimation(animation, "mouseAnimation");
    
    // turn off display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    FileUtils::getInstance()->addSearchPath("res");

    // create a scene. it's an autorelease object
    auto scene = MenuScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
