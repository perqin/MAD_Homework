
#include "S.h"

cocos2d::ValueVector S::xmlVector = cocos2d::FileUtils::getInstance()->getValueVectorFromFile("strings.xml");


std::string S::get(const char * id) {
	auto txtMap = xmlVector.at(0).asValueMap();
	return txtMap.at(id).asString();
}
