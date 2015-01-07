#ifndef __POPSTAR_STAR_H__
#define __POPSTAR_STAR_H__

#include "cocos2d.h"

class HelloWorld;
class Star : public cocos2d::CCSprite
{
public:
    Star(int index);
    
    static Star* create(int index);
    
    bool isHighLight();
    void setHighLight(bool hl);
    int  getIndex();
    void setPos(const cocos2d::CCPoint& pos);
    cocos2d::CCPoint getDestPos();
    void setDestPos(const cocos2d::CCPoint& pos);
    void draw();
    
    void onUpdate(float delta);
    
    virtual bool init();
    
private:
    std::string getPath();
    
private:
    int  index;
    bool highlight; // 是否高亮显示
    cocos2d::CCPoint destPos;
};

#endif // __POPSTAR_STAR_H__