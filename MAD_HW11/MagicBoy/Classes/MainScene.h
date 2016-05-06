#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer {
public:
	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(MainScene);
	void startGameCallback(Ref * pSender);
	void quitGameCallback(Ref * pSender);
};

#endif // __MAIN_SCENE_H__
