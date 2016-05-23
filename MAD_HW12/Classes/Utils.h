#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"

USING_NS_CC;

#define SETBIT(x, y) x |= y
#define UNSETBIT(x, y) x &= ~y
// Note: y cannot be 0
#define ISSETBIT(x, y) ((x & y) == y)

//class Utils {
//public:
//	static void removeFromParent(Node * node) {
//		node->getParent()->removeChild(node);
//	}
//};

#endif // !__UTILS_H__
