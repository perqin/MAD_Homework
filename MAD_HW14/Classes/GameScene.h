#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"

USING_NS_CC;
using namespace cocos2d::network;

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
	bool gameOn;

	Label * timeLabel;
	Label * scoreLabel;
	Label * rankingList;
	MenuItem * submitItem;
	MenuItem * rankingItem;
	MenuItem * startItem;
	Layer * indicatorsLayer;
	Layer * rankingLayer;
	Sprite * button;

	void startItemClick(Ref * sender);
	void submitItemClick(Ref * sender);
	void rankingItemClick(Ref * sender);
	bool screenTouch(Touch * touch, Event * unused);
	bool screenReleased(Touch * touch, Event * unused);
	//bool rankingBackgroundTouch(Touch * touch, Event * unused);
	void gameInit();
	void gameStart();
	// Game ends because of time counted down to 0
	void gameEnd();
	void countdown(float delta);
	void setScore(int s);
	void setTime(int t);
	void updateRanking(const char * r);
	void showNotice(const char * notice);
	void scoreSubmitted(HttpClient * client, HttpResponse * response);
	void rankingGotten(HttpClient * client, HttpResponse * response);
};

#endif // !__GAME_SCENE_H__
