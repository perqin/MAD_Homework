#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UILoadingBar.h"
#include <queue>

USING_NS_CC;
using namespace std;

class GameScene : public Layer {
public:
	// Declarations
	typedef struct {
		Vec2 velocity;
		Rect movingBox;
		int hp;
		int maxHp;
		int score;
	} PlayerStates;
	typedef struct {
		unsigned int controlState;
		unsigned int enemyGenerateCountdown;
		unsigned int nextEnemyIntervalMin;
		unsigned int nextEnemyIntervalMax;
	} GameStates;
	enum Controls {
		Player1Up,
		Player1Down,
		Player1Left,
		Player1Right,
		Player1Fire,
		Invalid
	};
	// Constants
	const static unsigned int CONTROL_PLAYER1_UP = 1;
	const static unsigned int CONTROL_PLAYER1_DOWN = 2;
	const static unsigned int CONTROL_PLAYER1_LEFT = 4;
	const static unsigned int CONTROL_PLAYER1_RIGHT = 8;
	const static unsigned int CONTROL_PLAYER1_FIRE = 16;
	
	// 000 - 099 are for players
	const static int PLAYER1 = 0;
	// 100 - 199 are for enemy
	const static int ENEMY_NORMAL = 100;
	// 200 - 299 are for player's weapons
	const static int PLAYER_BULLET_NORMAL = 200;
	// 300 - 399 are for enemy's weapons
	const static int ENEMY_BULLET_NORMAL = 300;

	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	void update(float delta) override;

	Controls toControl(EventKeyboard::KeyCode keyCode);
	void onKeyPressedHandler(EventKeyboard::KeyCode keyCode, Event * e);
	void onKeyReleasedHandler(EventKeyboard::KeyCode keyCode, Event * e);
	void onControlHit(Controls c);
	void onControlDown(Controls c);
	void onControlUp(Controls c);
	void player1BulletEmitter(float delta);
	void generateEnemy();
	void getNextEnemyInterval();
	void onPlayerBeAttacked(EventCustom * e);
	void onEnemyBeAttacked(EventCustom * e);
	void onEnemyHitPlayer(EventCustom * e);
	void attachEnemy(Node * e);
	void attachPlayerBullet(Node * b);
	void attachEnemyBullet(Node * b);
	void removeEnemy(Node * e);
	void removePlayerBullet(Node * b);
	void removeEnemyBullet(Node * b);
	void attachExplosion(const Vec2 & pos);
	void setScore(int score);
	void setHp(int hp);
	void showNotification(const char * n);
private:
	// Frequently used data
	Vec2 vo;
	Size vs;
	// Game objects
	Layer * gameObjectsLayer;
	Layer * indicatorsLayer;
	Sprite * player;
	Vector<Node *> enemies;
	Vector<Node *> playerBullets;
	Vector<Node *> enemyBullets;
	Label * scoreIndicator;
	ui::LoadingBar * hpBar;
	ui::Scale9Sprite * hpBarBackground;
	// Game states
	PlayerStates playerStates;
	GameStates gameStates;
	queue<Ref *> eventDataQueue;
};

#endif // __GAME_SCENE_H__
