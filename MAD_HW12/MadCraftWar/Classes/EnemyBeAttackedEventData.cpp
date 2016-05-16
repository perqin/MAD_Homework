#include "EnemyBeAttackedEventData.h"

EnemyBeAttackedEventData * EnemyBeAttackedEventData::create(Node * attacker, Node * enemy) {
	EnemyBeAttackedEventData * ret = new (std::nothrow) EnemyBeAttackedEventData();
	if (ret && ret->init(attacker, enemy)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

EnemyBeAttackedEventData::EnemyBeAttackedEventData() {}

EnemyBeAttackedEventData::~EnemyBeAttackedEventData() {}

bool EnemyBeAttackedEventData::init(Node * attacker, Node * enemy) {
	_attacker = attacker;
	_enemy = enemy;
	return true;
}

Node * EnemyBeAttackedEventData::getAttacker() const {
	return _attacker;
}

Node * EnemyBeAttackedEventData::getEnemy() const {
	return _enemy;
}
