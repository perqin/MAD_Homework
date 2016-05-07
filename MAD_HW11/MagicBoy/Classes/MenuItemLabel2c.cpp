#include "MenuItemLabel2c.h"

MenuItemLabel2c * MenuItemLabel2c::create(cocos2d::Node * label, const cocos2d::ccMenuCallback & callbackPressed, const cocos2d::ccMenuCallback & callbackReleased) {
    MenuItemLabel2c * ret = new (std::nothrow) MenuItemLabel2c();
    ret->initWithLabel(label, (const cocos2d::ccMenuCallback &) nullptr);
    ret->setCallbacks(callbackPressed, callbackReleased);
    ret->autorelease();
    return ret;
}

MenuItemLabel2c * MenuItemLabel2c::create(cocos2d::Node * label) {
    MenuItemLabel2c * ret = new (std::nothrow) MenuItemLabel2c();
    ret->initWithLabel(label, (const cocos2d::ccMenuCallback &) nullptr);
    ret->setCallbacks((const cocos2d::ccMenuCallback &) nullptr, (const cocos2d::ccMenuCallback &) nullptr);
    ret->autorelease();
    return ret;
}

void MenuItemLabel2c::activate() {
    if (_enabled) {
        this->stopAllActions();
        this->setScale(_originalScale);
        //MenuItem::activate();
    }
}

void MenuItemLabel2c::selected() {
    cocos2d::MenuItemLabel::selected();
    // Call callback
    if (_enabled) {
        if (_callbackPressed) {
            _callbackPressed(this);
        }
#if CC_ENABLE_SCRIPT_BINDING
        if (cocos2d::kScriptTypeLua == _scriptType) {
            cocos2d::BasicScriptData data(this);
            cocos2d::ScriptEvent scriptEvent(cocos2d::kMenuClickedEvent, &data);
            cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
        }
#endif
    }
}

void MenuItemLabel2c::unselected() {
    cocos2d::MenuItemLabel::unselected();
    // Call callback
    if (_enabled) {
        if (_callback) {
            _callback(this);
        }
#if CC_ENABLE_SCRIPT_BINDING
        if (cocos2d::kScriptTypeLua == _scriptType) {
            cocos2d::BasicScriptData data(this);
            cocos2d::ScriptEvent scriptEvent(cocos2d::kMenuClickedEvent, &data);
            cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
        }
#endif
    }
}

void MenuItemLabel2c::setCallbacks(const cocos2d::ccMenuCallback & callbackPressed, const cocos2d::ccMenuCallback & callbackReleased) {
    _callbackPressed = callbackPressed;
    _callback = callbackReleased;
}
