#ifndef __S_H__
#define __S_H__

#include <string>
#include <cocos2d.h>

class S {
public:
	static std::string get(const char * id);
private:
	static cocos2d::ValueVector xmlVector;
};

#endif // __S_H__