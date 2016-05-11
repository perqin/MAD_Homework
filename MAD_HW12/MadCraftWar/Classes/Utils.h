#ifndef __UTILS_H__
#define __UTILS_H__

#define SETBIT(x, y) x |= y
#define UNSETBIT(x, y) x &= ~y
// Note: y cannot be 0
#define ISSETBIT(x, y) ((x & y) == y)

#endif // !__UTILS_H__
