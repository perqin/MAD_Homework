#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public Layer {
public:
	// Declarations
	typedef struct {
		Vec2 velocity;
		Rect movingBox;
	} PlayerStates;
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
private:
	Sprite * player;
	//MoveBy * playerMoveBy;
	//Rect playerMovingBox;
	// Game runtimes
	unsigned int controlState;
	PlayerStates playerStates;
};

#endif // __GAME_SCENE_H__
