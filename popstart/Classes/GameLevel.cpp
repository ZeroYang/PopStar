#include "PopStarLayer.h"
#include "PopStarDataMgr.h"
#include "GameScene.h"

#include "GameLevel.h"

USING_NS_CC;


// on "init" you need to initialize your instance
bool GameLevel::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	char szData[32] = {0};
	int level = getPopStarDataMgr().getLevel();
	int score = getPopStarDataMgr().getTargetScoreByLevel(level);
	snprintf(szData, 32, "Level:%d", level);
	labelLevel = CCLabelTTF::create(szData, "Arial", 24);

	labelLevel->setPosition(ccp(origin.x + visibleSize.width/2 + 250,
		origin.y + visibleSize.height - labelLevel->getContentSize().height - 50));

	this->addChild(labelLevel, 1);

	snprintf(szData, 32, "Target Score:%d", score);
	labelTargetScore = CCLabelTTF::create(szData, "Arial", 24);

	labelTargetScore->setPosition(ccp(origin.x + visibleSize.width/2 + 500,
		origin.y + visibleSize.height - labelTargetScore->getContentSize().height - 150));

	this->addChild(labelTargetScore, 1);

	this->schedule(schedule_selector(GameLevel::onTimeUp), 4);

	CCPoint pos1 = labelLevel->getPosition();
	CCMoveTo* move1 = CCMoveTo::create( 1, ccp(pos1.x-250, pos1.y) );
	labelLevel->runAction(move1);

	CCPoint pos2 = labelTargetScore->getPosition();
	CCMoveTo* move2 = CCMoveTo::create( 2, ccp(pos2.x-500, pos2.y) );
	labelTargetScore->runAction(move2);

	return true;
}

void GameLevel::onTimeUp(float delta)
{
	changeLayer( this, PopStarLayer::create() );
}
