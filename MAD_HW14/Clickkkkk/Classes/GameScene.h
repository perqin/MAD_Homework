#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public Layer {
public:
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
private:
	Vec2 vo;
	Size vs;
	int score;
	int time;

	Label * timeLabel;
	Label * scoreLabel;
	MenuItem * submitItem;
	MenuItem * rankingItem;

	void startItemClick(Ref * sender);
	void submitItemClick(Ref * sender);
	void rankingItemClick(Ref * sender);
	bool buttonTouch(Touch * touch, Event * unused);
	void gameInit();
	void gameStart();
	// Game ends because of time counted down to 0
	void gameEnd();
	void countdown(float delta);
	void setScore(int s);
	void setTime(int t);
};

#endif // !__GAME_SCENE_H__