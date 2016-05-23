#ifndef __ENEMY_BE_ATTACKED_EVENT_DATA_H__
#define __ENEMY_BE_ATTACKED_EVENT_DATA_H__

#include "cocos2d.h"

USING_NS_CC;

class EnemyBeAttackedEventData : public Ref {
public:
	static EnemyBeAttackedEventData * create(Node * attacker = nullptr, Node * enemy = nullptr);
	EnemyBeAttackedEventData();
	~EnemyBeAttackedEventData();
	bool init(Node * attacker = nullptr, Node * enemy = nullptr);
	Node * getAttacker() const;
	Node * getEnemy() const;
private:
	Node * _attacker;
	Node * _enemy;
};

#endif // !__ENEMY_BE_ATTACKED_EVENT_DATA_H__
