#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HomeScene : public Layer {
public:
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(HomeScene);
};

#endif // __HOME_SCENE_H__
