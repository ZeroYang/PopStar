#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include "cocos2d.h"

class GameLevel : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	//static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(GameLevel);

	void onTimeUp(float delta);

	//void onClickMenu(cocos2d::CCObject* obj);

private:
	cocos2d::CCLabelTTF* labelLevel;
	cocos2d::CCLabelTTF* labelTargetScore;
};

#endif // __GAME_LEVEL_H__
