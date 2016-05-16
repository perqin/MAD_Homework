#ifndef __ENEMY_HIT_PLAYER_EVENT_DATA_H__
#define __ENEMY_HIT_PLAYER_EVENT_DATA_H__

#include "cocos2d.h"

USING_NS_CC;

class EnemyHitPlayerEventData : public Ref {
public:
	static EnemyHitPlayerEventData * create(Node * enemy = nullptr, Node * player = nullptr);
	EnemyHitPlayerEventData();
	~EnemyHitPlayerEventData();
	bool init(Node * enemy = nullptr, Node * player = nullptr);
	Node * getEnemy() const;
	Node * getPlayer() const;
private:
	Node * _enemy;
	Node * _player;
};

#endif // !__ENEMY_HIT_PLAYER_EVENT_DATA_H__