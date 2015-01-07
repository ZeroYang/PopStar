#include "Star.h"

#include "PopStarDataMgr.h"

USING_NS_CC;

namespace
{
    const int MAX_REDUCE_STAR_NUM = 30;
    int REDUCENUM_SCORE_MAP[MAX_REDUCE_STAR_NUM][2] =
    {
        {2, 20},
        {3, 45},
        {4, 80},
        {5, 125},
        {6, 180},
        {7, 245},
        {8, 320},
        {9, 405},
        {10, 500},
        {11, 605},
        {12, 720},
        {13, 845},
        {14, 980},
        {15, 1125},
        {16, 1280},
        {17, 1445},
        {18, 1620},
        {19, 1805},
        {20, 2000},
        {21, 2205},
        {22, 2420},
        {23, 2645},
        {24, 2880},
        {25, 3125},
        {26, 3380},
        {27, 3645},
        {28, 3920},
        {29, 4205},
        {30, 4500},
        {31, 4805}
    };
    const int MAX_LEFT_STAR_NUM = 10;
    const int LEFTNUM_SCORE_MAP[MAX_LEFT_STAR_NUM][2] =
    {
        {0, 2000},
        {1, 1980},
        {2, 1920},
        {3, 1820},
        {4, 1680},
        {5, 1500},
        {6, 1280},
        {7, 1020},
        {8, 720},
        {9, 380}
    };
    
    const std::string HISTORY_TOTAL_SCORE = std::string("historyTotalScore");	// 历史最高分
    const std::string HISTORY_MAX_LEVEL   = std::string("historyMaxLevel");		// 历史最高到达关数
    const std::string HISTORY_LEVEL_SCORE = std::string("historyLevelScore%d");	// 历史每关到达最高分数
}

PopStarDataMgr::PopStarDataMgr()
{
    gameLevel = 1;
    curScore = 0;
    
    historyScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(HISTORY_TOTAL_SCORE.c_str(), 0);
    
    int historyMaxLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(HISTORY_MAX_LEVEL.c_str(), 1);
    for (int i = 0; i < historyMaxLevel; ++i)
    {
        char szBuf[64] = {0};
        snprintf(szBuf, 64, HISTORY_LEVEL_SCORE.c_str(), i+1);
        int score = CCUserDefault::sharedUserDefault()->getIntegerForKey(szBuf, 0);
        historyLevelScores.push_back(score);
    }
    
    init();
}

PopStarDataMgr::~PopStarDataMgr()
{
}

bool PopStarDataMgr::init()
{
    reduceNumScoreMap.clear();
    for (int i = 0; i < MAX_REDUCE_STAR_NUM; ++i)
    {
        reduceNumScoreMap.insert( std::make_pair(REDUCENUM_SCORE_MAP[i][0], REDUCENUM_SCORE_MAP[i][1]) );
    }
    
    leftNumScoreMap.clear();
    for (int i = 0; i < MAX_LEFT_STAR_NUM; ++i)
    {
        leftNumScoreMap.insert( std::make_pair(LEFTNUM_SCORE_MAP[i][0], LEFTNUM_SCORE_MAP[i][1]) );
    }
    
    return true;
}

int PopStarDataMgr::getScore()
{
    return curScore;
}

void PopStarDataMgr::setScore(int score)
{
    curScore = score;
}

int PopStarDataMgr::getLevel()
{
    return gameLevel;
}

void PopStarDataMgr::setLevel(int level)
{
    gameLevel = level;
}

int PopStarDataMgr::getHistoryScore()
{
    return historyScore;
}

void PopStarDataMgr::setHistoryScore(int score)
{
    if (score > historyScore)
    {
        historyScore = score;
        CCUserDefault::sharedUserDefault()->setIntegerForKey(HISTORY_TOTAL_SCORE.c_str(), score);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(HISTORY_MAX_LEVEL.c_str(), gameLevel);
        CCUserDefault::sharedUserDefault()->flush();
    }
}

int PopStarDataMgr::getHistoryLevelScoreByLevel(int level)
{
    int score = 0;
    if (level <= (int)historyLevelScores.size())
    {
        score = historyLevelScores[level-1];
    }
    
    return score;
}

void PopStarDataMgr::setHistoryLevelScore(int level, int score)
{
    char szBuf[64] = {0};
    snprintf(szBuf, 64, HISTORY_LEVEL_SCORE.c_str(), level);
    if (level <= (int)historyLevelScores.size())
    {
        historyLevelScores[level-1] = score;
        CCUserDefault::sharedUserDefault()->setIntegerForKey(szBuf, score);
        CCUserDefault::sharedUserDefault()->flush();
    }
    else if (level == (historyLevelScores.size()+1))
    {
        historyLevelScores.push_back(score);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(szBuf, score);
        CCUserDefault::sharedUserDefault()->flush();
    }
    else
    {
        CCLog("setHistoryLevelScoreByLevel error, level=%d score=%d", level, score);
    }
}

int PopStarDataMgr::getScoreByReduceNum(int num)
{
    int score = 0;
    
    std::map<int, int>::iterator iter = reduceNumScoreMap.find(num);
    if (iter == reduceNumScoreMap.end())
    {
        if (num < 2)
        {
            return 0;
        }
        else if (num > MAX_REDUCE_STAR_NUM)
        {
            return 1000000;
        }
    }
    else
    {
        score = iter->second;
    }
    
    return score;
}

int PopStarDataMgr::getScoreByLeftNum(int num)
{
    int score = 0;
    std::map<int, int>::iterator iter = leftNumScoreMap.find(num);
    if (iter != leftNumScoreMap.end())
    {
        score = iter->second;
    }
    
    return score;
}

int PopStarDataMgr::getTargetScoreByLevel(int level)
{
    int score = 0;
    if (level == 1)
    {
        score = 1000;
    }
    else if (level == 2)
    {
        score = 3000;
    }
    else if ( (level >=3) && (level <= 10) )
    {
        score = 3000 + 3000 * (level - 2);
    }
    else
    {
        score = 27000 + 4000 * (level - 10);
    }
    
    return score;
}

PopStarDataMgr& getPopStarDataMgr()
{
    static PopStarDataMgr mgr;
    return mgr;
}