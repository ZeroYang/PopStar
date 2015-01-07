#include "PopStar.h"
#include "../extensions/CocoStudio/gui/UIWidgets/UILabel.h"

#include "PopStarLayer.h"

USING_NS_CC;

PopStarLayer::~PopStarLayer()
{
	if (popStar)
	{
		delete popStar;
		popStar = NULL;
	}
}

// on "init" you need to initialize your instance
bool PopStarLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	srand( (unsigned)time( NULL ) );

	float scale = visibleSize.width/320.f;
	CCSprite* popStarBg = CCSprite::create("popstar_bg.png");
	popStarBg->setPosition(ccp(160*scale, 400*scale));
	popStarBg->setScale(scale);
	this->addChild(popStarBg, 0);

	historyTotalScore = CCLabelTTF::create("0", "Arial", 24);
	if (historyTotalScore)
	{
		historyTotalScore->setPosition(ccp(160*scale, 464*scale));
		this->addChild(historyTotalScore);
	}

	historyLevelScore = CCLabelTTF::create("0", "Arial", 24);
	if (historyLevelScore)
	{
		historyLevelScore->setPosition(ccp(160*scale, 437*scale));
		this->addChild(historyLevelScore);
	}

	gameLevel = CCLabelTTF::create("0", "Arial", 24);
	if (gameLevel)
	{
		gameLevel->setPosition(ccp(80*scale, 405*scale));
		this->addChild(gameLevel);
	}

	targetScore = CCLabelTTF::create("0", "Arial", 24);
	if (targetScore)
	{
		targetScore->setPosition(ccp(230*scale, 405*scale));
		this->addChild(targetScore);
	}

	curScore = CCLabelTTF::create("0", "Arial", 24);
	if (curScore)
	{
		curScore->setPosition(ccp(80*scale, 378*scale));
		this->addChild(curScore);
	}

	reduceScore = CCLabelTTF::create("", "Arial", 24);
	if (reduceScore)
	{
		reduceScore->setPosition(ccp(120*scale, 340*scale));
		reduceScore->setHorizontalAlignment(kCCTextAlignmentLeft);
		this->addChild(reduceScore);
	}

	popStar = new PopStar(this);

	this->schedule(schedule_selector(PopStarLayer::update));

	return true;
}

void PopStarLayer::onEnter()
{
	CCLayer::onEnter();

	this->setTouchEnabled(true);
}

void PopStarLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	CCPoint locInView = touch->getLocationInView();
	CCPoint loc = CCDirector::sharedDirector()->convertToGL(locInView);

	if (popStar)
	{
		popStar->onClick(loc);
	}
	
}

void PopStarLayer::update(float delta)
{
	if (!popStar)
	{
		return;
	}

	popStar->onUpdate(delta);
}

void PopStarLayer::onGuiEvent(GUI_EVENT_TYPE event, int nValue, unsigned int uValue)
{
	switch (event)
	{
	case EVENT_UPDATE_SCORE:
		{
			if (curScore)
			{
				char szBuf[32] = {0};
				//itoa(nValue, szBuf, 10);
				sprintf(szBuf, "%d", nValue);
				curScore->setString(szBuf);
			}
		}
		break;
	case EVENT_UPDATE_TARGET_SCORE:
		{
			if (targetScore)
			{
				char szBuf[32] = {0};
				//itoa(nValue, szBuf, 10);
				sprintf(szBuf, "%d", nValue);
				targetScore->setString(szBuf);
			}
		}
		break;
	case EVENT_UPDATE_LEVEL:
		{
			if (gameLevel)
			{
				char szBuf[32] = {0};
				//itoa(nValue, szBuf, 10);
				sprintf(szBuf, "%d", nValue);
				gameLevel->setString(szBuf);
			}
		}
		break;
	case EVENT_UPDATE_REDUCE_SCORE:
		{
			if (reduceScore)
			{
				if (nValue == 0)
				{
					reduceScore->setString("");
				}
				else
				{
					char szBuf[32] = {0};
					snprintf(szBuf, 32, "Number: %d Score: %u", nValue, uValue);
					reduceScore->setString(szBuf);
				}
			}
		}
		break;
	case EVENT_UPDATE_TOTAL_HISTORY_SCORE:
		{
			if (historyTotalScore)
			{
				char szBuf[32] = {0};
				snprintf(szBuf, 32, "%d", nValue);
				historyTotalScore->setString(szBuf);
			}
		}
		break;
	case EVENT_UPDATE_LEVEL_HISTORY_SCORE:
		{
			if (historyLevelScore)
			{
				char szBuf[32] = {0};
				snprintf(szBuf, 32, "%d", nValue);
				historyLevelScore->setString(szBuf);
			}
		}
		break;
	}
}
