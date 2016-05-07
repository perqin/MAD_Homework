#include "cocos2d.h"

class MenuItemLabel2c : public cocos2d::MenuItemLabel {
public:
    /** Creates a MenuItemLabel with a Label and a callback. */
    static MenuItemLabel2c * create(cocos2d::Node * label, const cocos2d::ccMenuCallback & callbackPressed, const cocos2d::ccMenuCallback & callbackReleased);
    /** Creates a MenuItemLabel with a Label. Target and selector will be nil. */
    static MenuItemLabel2c * create(cocos2d::Node * label);
    virtual void activate() override;
    virtual void selected() override;
    virtual void unselected() override;
    void setCallbacks(const cocos2d::ccMenuCallback & callbackPressed, const cocos2d::ccMenuCallback & callbackReleased);
protected:
    cocos2d::ccMenuCallback _callbackPressed;
};
