#include "StartLayer.h"

#include "GameScene.h"

USING_NS_CC;

CCScene* createScene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	StartLayer *layer = StartLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void changeLayer(CCLayer* curLayer, CCLayer* newLayer)
{
	if (!curLayer || !newLayer)
	{
		return;
	}

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if (scene)
	{
		scene->removeChild(curLayer);
		scene->addChild(newLayer);
	}
}
