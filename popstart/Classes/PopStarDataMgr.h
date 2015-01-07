#ifndef __POPSTAR_DATA_MGR_H__
#define __POPSTAR_DATA_MGR_H__

class PopStarDataMgr
{
public:
    PopStarDataMgr();
    ~PopStarDataMgr();
    
    bool init();
    
    int  getScore();
    void setScore(int score); // 设置当前分数
    
    int  getLevel();
    void setLevel(int level);
    
    int  getHistoryScore();
    void setHistoryScore(int score);
    
    int  getHistoryLevelScoreByLevel(int level);
    void setHistoryLevelScore(int level, int score);
    
    int  getScoreByReduceNum(int num);		// 根据消除星星数获取得到分数
    int  getScoreByLeftNum(int num);		// 根据剩余星星数获取得到分数
    int  getTargetScoreByLevel(int level);	// 根据关卡获取目标分数
    
private:
    std::map<int, int> reduceNumScoreMap;
    std::map<int, int> leftNumScoreMap;
    
    int			curScore;
    int			gameLevel;
    int			historyScore;		// 历史最高分
    
    std::vector<int> historyLevelScores;
};

PopStarDataMgr& getPopStarDataMgr();

#endif // __POPSTAR_DATA_MGR_H__