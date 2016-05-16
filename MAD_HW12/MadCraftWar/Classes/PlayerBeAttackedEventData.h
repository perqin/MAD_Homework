#ifndef __PLAYER_BE_ATTACKED_EVENT_DATA_H__
#define __PLAYER_BE_ATTACKED_EVENT_DATA_H__

#include "cocos2d.h"

USING_NS_CC;

class PlayerBeAttackedEventData : public Ref {
public:
	static PlayerBeAttackedEventData * create();
	PlayerBeAttackedEventData();
	~PlayerBeAttackedEventData();
private:
};
#endif // !__PLAYER_BE_ATTACKED_EVENT_DATA_H__