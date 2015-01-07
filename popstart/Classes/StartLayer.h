#ifndef __START_LAYER_H__
#define __START_LAYER_H__

#include "cocos2d.h"

class StartLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(StartLayer);

	void onClickMenu(cocos2d::CCObject* obj);

private:
};

#endif // __START_LAYER_H__
