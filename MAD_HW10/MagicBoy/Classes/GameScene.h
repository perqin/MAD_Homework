#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/UILoadingBar.h"

class GameScene : public cocos2d::Layer {
public:
	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
    void backToMainScene(cocos2d::Ref * sender);
    void updateTimer(float delta);
	void handleControl(const char key);
    void buttonTouched(Ref * sender, cocos2d::ui::Widget::TouchEventType tet, const char key);
    cocos2d::Vec2 playerMoveBy(const char key);
private:
	cocos2d::Sprite * player;
    cocos2d::Sprite * cure;
    cocos2d::Sprite * suicide;
    cocos2d::ui::LoadingBar * hpBar;
    unsigned int hp;
	cocos2d::Label * timer;
    unsigned int time;
};

#endif // __GAME_SCENE_H__
