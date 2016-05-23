#include "EnemyHitPlayerEventData.h"

EnemyHitPlayerEventData * EnemyHitPlayerEventData::create(Node * enemy, Node * player) {
	EnemyHitPlayerEventData * ret = new (std::nothrow) EnemyHitPlayerEventData();
	if (ret && ret->init(enemy, player)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

EnemyHitPlayerEventData::EnemyHitPlayerEventData() {}

EnemyHitPlayerEventData::~EnemyHitPlayerEventData() {}

bool EnemyHitPlayerEventData::init(Node * enemy, Node * player) {
	_enemy = enemy;
	_player = player;
	return true;
}

Node * EnemyHitPlayerEventData::getEnemy() const {
	return _enemy;
}

Node * EnemyHitPlayerEventData::getPlayer() const {
	return _player;
}
