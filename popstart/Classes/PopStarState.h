#ifndef __POPSTAR_STATE_H__
#define __POPSTAR_STATE_H__

#include "cocos2d.h"

enum POPSTAR_STATE
{
    POPSTAR_STATE_INIT,		// 初始化状态
    POPSTAR_STATE_NORMAL,	// 正常状态
    POPSTAR_STATE_PASS,		// 过关状态
    POPSTAR_STATE_OVER,		// 未过关状态
};

class PopStar;

// PopStar状态基类
class PopStarState
{
public:
    PopStarState(PopStar* ps);
    virtual ~PopStarState();
    
    virtual void enter() = 0;				// 当状态被进入时执行这个
    virtual void execute(float delta) = 0;	// 状态正常更新
    virtual void exit() = 0;				// 当状态退出时执行这个
    
protected:
    POPSTAR_STATE	state;
    PopStar*		popStar;
    float			elapsedTime;
};

// 游戏初始化状态
class GameInitState : public PopStarState
{
public:
    GameInitState(PopStar* ps);
    
    virtual void enter();
    virtual void execute(float delta);
    virtual void exit();
};

// 正常游戏状态
class GameRunningState : public PopStarState
{
public:
    GameRunningState(PopStar* ps);
    
    virtual void enter();
    virtual void execute(float delta);
    virtual void exit();
};

class GamePassLevelState : public PopStarState
{
public:
    GamePassLevelState(PopStar* ps);
    
    virtual void enter();
    virtual void execute(float delta);
    virtual void exit();
};

class GameOverState : public PopStarState
{
public:
    GameOverState(PopStar* ps);
    
    virtual void enter();
    virtual void execute(float delta);
    virtual void exit();
};

#endif // __POPSTAR_STATE_H__