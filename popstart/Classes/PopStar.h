#ifndef __POPSTAR_H__
#define __POPSTAR_H__

#include "cocos2d.h"

class PopStarLayer;
class Star;

struct SelectStar
{
    int		row;
    int		col;
    Star*	star;
};

class PopStarState;

class PopStar : public cocos2d::CCNode
{
    enum
    {
        ROW_NUM = 10, // 行数
        COL_NUM = 10, // 列数
        
        REVERSE_NUM = 100, // 预留允许最大数
    };
    
    typedef std::list<SelectStar>	StarList;
    typedef StarList::iterator		StarListIter;
    
public:
    PopStar(PopStarLayer* layer);
    virtual ~PopStar();
    
    static PopStar* create(PopStarLayer* layer);
    bool init();
    
    void onClick(const cocos2d::CCPoint& pos);
    void onUpdate(float delta);
    
    void setScore(int score); // 设置当前分数
    void setLevel(int level);
    void setHistoryScore(int score);
    void setHistoryLevelScore(int score);
    void setTargetScore(int score);
    
    bool isLevelEnd();			// 本关是否结束
    bool isGameOver();			// 游戏是否结束
    
    void changeState(PopStarState* newState);
    bool isInitFinish();		// 是否初始化完成
    
    int  getLeftStarNum();		// 获取剩余的星星数
    void clearStarsOneByOne();
    void gotoNextLevel();		// 跳到下一关
    void gotoStartLayer();		// 返回到开始
    
private:
    void onReduce();
    void onSelect(const SelectStar& selectStar);
    void clearSelectStars();
    bool isNeedHoriAdjust();	// 是否需要水平调整
    int  getCheckEndCol();		// 获取需要水平调整的最大列数
    
private:
    PopStarLayer*	gameLayer;
    Star*			stars[ROW_NUM][COL_NUM];
    StarList		selectStars;
    
    PopStarState*	currentState;
    float			elapsedTime;
};

#endif // __POPSTAR_H__