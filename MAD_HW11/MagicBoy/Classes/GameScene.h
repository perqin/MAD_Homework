#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/UILoadingBar.h"

class GameScene : public cocos2d::Layer {
public:
    const static int ACTION_STOP_AT_RELEASE = 1;
	const static int SPRITE_MONSTER = 1;
	const static int SPRITE_MONSTER_DEAD = 2;
	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
    void backToMainScene(cocos2d::Ref * sender);
    void updateTimer(float delta);
	void handleControl(const char key);
    cocos2d::Vec2 playerMoveBy(const char key);
    void controlButtonPressed(cocos2d::Ref * sender, const char key);
    void controlButtonReleased(cocos2d::Ref * sender, const char key);
    void monsterGenerater(float delta);
	void update(float delta) override;
	void playerBeAttacked(cocos2d::Node * attacker);
	void playerAttack();
private:
	cocos2d::Sprite * player;
    cocos2d::ui::LoadingBar * hpBar;
    unsigned int hp;
    cocos2d::Label * timer;
    cocos2d::Label * add1s;
    unsigned int time;
	cocos2d::Layer * gameLayer;
	cocos2d::TMXTiledMap * tmx;
	cocos2d::Vec2 stair;
};

#endif // __GAME_SCENE_H__
