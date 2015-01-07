#include "PopStar.h"

#include "PopStarState.h"

USING_NS_CC;

namespace
{
	const float REDUCE_INTERVAL_TIME = 0.3f;
}

PopStarState::PopStarState(PopStar* ps)
{
	popStar = ps;
	elapsedTime = 0.f;
}

PopStarState::~PopStarState()
{
	popStar = NULL;
}

GameInitState::GameInitState(PopStar* ps) : PopStarState(ps)
{
	state = POPSTAR_STATE_INIT;
}

void GameInitState::enter()
{

}

void GameInitState::execute(float delta)
{
	if (popStar && popStar->isInitFinish())
	{
		popStar->changeState( new GameRunningState(popStar) );
	}
}

void GameInitState::exit()
{

}

GameRunningState::GameRunningState(PopStar* ps) : PopStarState(ps)
{
	state = POPSTAR_STATE_NORMAL;
}

void GameRunningState::enter()
{

}

void GameRunningState::execute(float delta)
{

}

void GameRunningState::exit()
{

}

GamePassLevelState::GamePassLevelState(PopStar* ps) : PopStarState(ps)
{
	state = POPSTAR_STATE_PASS;
}

void GamePassLevelState::enter()
{

}

void GamePassLevelState::execute(float delta)
{
	if (!popStar)
	{
		return;
	}

	elapsedTime += delta;
	if (elapsedTime >= REDUCE_INTERVAL_TIME)
	{
		elapsedTime -= REDUCE_INTERVAL_TIME;
		popStar->clearStarsOneByOne();

		int num = popStar->getLeftStarNum();
		if (num == 0)
		{
			popStar->gotoNextLevel();
		}
	}
}

void GamePassLevelState::exit()
{

}

GameOverState::GameOverState(PopStar* ps) : PopStarState(ps)
{
	state = POPSTAR_STATE_OVER;
}

void GameOverState::enter()
{

}

void GameOverState::execute(float delta)
{
	if (!popStar)
	{
		return;
	}

	elapsedTime += delta;
	if (elapsedTime >= REDUCE_INTERVAL_TIME)
	{
		elapsedTime -= REDUCE_INTERVAL_TIME;
		popStar->clearStarsOneByOne();

		int num = popStar->getLeftStarNum();
		if (num == 0)
		{
			popStar->gotoStartLayer();
		}
	}
}

void GameOverState::exit()
{

}
