#include "PopStarLayer.h"
#include "GameLevel.h"
#include "GameScene.h"
#include "Star.h"
#include "PopStarDataMgr.h"
#include "PopStarState.h"
#include "StartLayer.h"

#include "PopStar.h"

USING_NS_CC;

namespace
{
    const int STAR_WIDTH	= 64;
    const int STAR_HEIGHT	= 64;
}

PopStar::PopStar(PopStarLayer* layer)
{
    gameLayer = layer;
    //ZeroMemory(stars, sizeof(Star*) * ROW_NUM * COL_NUM);
    memset(stars, 0, sizeof(Star*) * ROW_NUM * COL_NUM);
    elapsedTime = 0;
    currentState = NULL;
    
    init();
}

PopStar::~PopStar()
{
    for (int row = 0; row < ROW_NUM; ++row)
    {
        for (int col = 0; col < COL_NUM; ++col)
        {
            Star* star = stars[row][col];
            if (stars[row][col])
            {
                gameLayer->removeChild(stars[row][col]);
                stars[row][col] = NULL;
            }
        }
    }
    
    if (currentState)
    {
        delete currentState;
        currentState = NULL;
    }
    
    gameLayer = NULL;
}

PopStar* PopStar::create(PopStarLayer* layer)
{
    PopStar* pRet = new PopStar(layer);
    if (pRet)
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool PopStar::init()
{
    if (!CCNode::init())
    {
        return false;
    }
    
    if (!gameLayer)
    {
        return false;
    }
    
    for (int row = 0; row < ROW_NUM; ++row)
    {
        for (int col = 0; col < COL_NUM; ++col)
        {
            int index = (rand() % 5) + 1;
            Star* star = Star::create(index);
            if (star)
            {
                star->setScale(2);
                //star->setContentSize(CCSize(STAR_WIDTH, STAR_HEIGHT));
                star->setPos(ccp(STAR_WIDTH*col + STAR_WIDTH/2, STAR_HEIGHT*row + STAR_HEIGHT/2 + row*20+col*5));
                star->setDestPos(ccp(STAR_WIDTH*col + STAR_WIDTH/2, STAR_HEIGHT*row + STAR_HEIGHT/2));
                gameLayer->addChild(star);
                
                stars[row][col] = star;
            }
        }
    }
    
    int level = getPopStarDataMgr().getLevel();
    gameLayer->onGuiEvent(EVENT_UPDATE_LEVEL, level);
    
    int score = getPopStarDataMgr().getScore();
    gameLayer->onGuiEvent(EVENT_UPDATE_SCORE, score);
    
    int historyScore = getPopStarDataMgr().getHistoryScore();
    gameLayer->onGuiEvent(EVENT_UPDATE_TOTAL_HISTORY_SCORE, historyScore);
    
    int historyLevelScore = getPopStarDataMgr().getHistoryLevelScoreByLevel(level);
    gameLayer->onGuiEvent(EVENT_UPDATE_LEVEL_HISTORY_SCORE, historyLevelScore);
    
    int targetScore = getPopStarDataMgr().getTargetScoreByLevel(level);
    gameLayer->onGuiEvent(EVENT_UPDATE_TARGET_SCORE, targetScore);
    
    currentState = new GameInitState(this);
    
    return true;
}

void PopStar::onClick(const cocos2d::CCPoint& pos)
{
    int row = pos.y / STAR_HEIGHT;
    int col = pos.x / STAR_WIDTH;
    if (row >= ROW_NUM || col >= COL_NUM)
    {
        return;
    }
    
    Star* star = stars[row][col];
    if (!star)
    {
        return;
    }
    
    if (star->isHighLight())
    {
        // 高亮则消除
        this->onReduce();
        
        gameLayer->onGuiEvent(EVENT_UPDATE_REDUCE_SCORE, 0);
        
    }
    else
    {
        // 选中则高亮
        SelectStar selectStar;
        selectStar.row = row;
        selectStar.col = col;
        selectStar.star = star;
        
        this->onSelect(selectStar);
    }
}

void PopStar::setScore(int score)
{
    if (!gameLayer)
    {
        return;
    }
    
    getPopStarDataMgr().setScore(score);
    gameLayer->onGuiEvent(EVENT_UPDATE_SCORE, score);
}

void PopStar::setLevel(int level)
{
    if (!gameLayer)
    {
        return;
    }
    
    getPopStarDataMgr().setLevel(level);
    gameLayer->onGuiEvent(EVENT_UPDATE_LEVEL, level);
}

void PopStar::setHistoryScore(int score)
{
    if (!gameLayer)
    {
        return;
    }
    
    getPopStarDataMgr().setHistoryScore(score);
    gameLayer->onGuiEvent(EVENT_UPDATE_TOTAL_HISTORY_SCORE, score);
}

void PopStar::setHistoryLevelScore(int score)
{
    if (!gameLayer)
    {
        return;
    }
    
    int level = getPopStarDataMgr().getLevel();
    getPopStarDataMgr().setHistoryLevelScore(level, score);
    gameLayer->onGuiEvent(EVENT_UPDATE_LEVEL_HISTORY_SCORE, score);
}

void PopStar::setTargetScore(int score)
{
    if (!gameLayer)
    {
        return;
    }
    
    gameLayer->onGuiEvent(EVENT_UPDATE_TARGET_SCORE, score);
}

void PopStar::onUpdate(float delta)
{
    for (int row = 0; row < ROW_NUM; ++row)
    {
        for (int col = 0; col < COL_NUM; ++col)
        {
            Star* star = stars[row][col];
            if (star)
            {
                star->onUpdate(delta);
            }
        }
    }
    
    if (currentState)
    {
        currentState->execute(delta);
    }
}

bool PopStar::isLevelEnd()
{
    for (int row = 0; row < ROW_NUM; ++row)
    {
        for (int col = 0; col < COL_NUM; ++col)
        {
            Star* star = stars[row][col];
            if (star == NULL)
            {
                continue;
            }
            
            int checkRow = row + 1;
            if (checkRow < ROW_NUM)
            {
                Star* checkStar = stars[checkRow][col];
                if ( checkStar && (star->getIndex() == checkStar->getIndex()) )
                {
                    return false;
                }
            }
            
            int checkCol = col + 1;
            if (checkCol < COL_NUM)
            {
                Star* checkStar = stars[row][checkCol];
                if ( checkStar && (star->getIndex() == checkStar->getIndex()) )
                {
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool PopStar::isGameOver()
{
    int gameLevel = getPopStarDataMgr().getLevel();
    int needScore = getPopStarDataMgr().getTargetScoreByLevel(gameLevel);
    int curScore = getPopStarDataMgr().getScore();
    return (curScore < needScore);
}

void PopStar::changeState(PopStarState* newState)
{
    if (!currentState || !newState)
    {
        return;
    }
    
    // 调用现有状态的退出方法
    currentState->exit();
    
    // 改变状态到新状态
    currentState = newState;
    
    // 调用新状态的进入方法
    currentState->enter();
}

bool PopStar::isInitFinish()
{
    Star* star = stars[ROW_NUM-1][COL_NUM-1];
    if (star)
    {
        CCPoint pos = star->getPosition();
        CCPoint destPos = star->getDestPos();
        if ( pos.equals(destPos) )
        {
            return true;
        }
    }
    
    return false;
}

void PopStar::onReduce()
{
    if (!gameLayer)
    {
        return;
    }
    
    int num = selectStars.size();
    int score = getPopStarDataMgr().getScore() + getPopStarDataMgr().getScoreByReduceNum(num);
    setScore(score);
    
    StarListIter iter;
    for (iter = selectStars.begin(); iter != selectStars.end(); ++iter)
    {
        gameLayer->removeChild(iter->star);
        stars[iter->row][iter->col] = NULL;
    }
    selectStars.clear();
    
    
    // 竖直调整
    for (int row = 0; row < ROW_NUM; ++row)
    {
        for (int col = 0; col < COL_NUM; ++col)
        {
            int tempRow = row;
            Star* star = stars[row][col];
            if (star)
            {
                continue;
            }
            else
            {
                while (!star && (tempRow < ROW_NUM))
                {
                    star = stars[tempRow][col];
                    if (star)
                    {
                        break;
                    }
                    ++tempRow;
                }
                
                if (star)
                {
                    stars[row][col] = stars[tempRow][col];
                    stars[tempRow][col] = NULL;
                }
            }
        }
    }
    
    while (isNeedHoriAdjust())
    {
        int endCol = getCheckEndCol();
        for (int col = 0; col <= endCol; ++col)
        {
            if (!stars[0][col])
            {
                for (int i = col; i < endCol; ++i)
                {
                    for (int row = 0; row < ROW_NUM; ++row)
                    {
                        stars[row][i] = stars[row][i+1];
                    }
                }
                
                for (int row = 0; row < ROW_NUM; ++row)
                {
                    stars[row][endCol] = NULL;
                }
                
                break;
            }
        }
    }
    
    
    // 刷新位置
    for (int row = 0; row < ROW_NUM; ++row)
    {
        for (int col = 0; col < COL_NUM; ++col)
        {
            Star* star = stars[row][col];
            if (star)
            {
                star->setDestPos(ccp(STAR_WIDTH*col + STAR_WIDTH/2, STAR_HEIGHT*row + STAR_HEIGHT/2));
            }
        }
    }
    
    if (isLevelEnd())
    {
        int num = getLeftStarNum();
        int score = getPopStarDataMgr().getScore() + getPopStarDataMgr().getScoreByLeftNum(num);
        setScore(score);
        
        int historyScore = getPopStarDataMgr().getHistoryScore();
        if (score > historyScore)
        {
            setHistoryScore(score);
        }
        
        int level = getPopStarDataMgr().getLevel();
        int historyLevelScore = getPopStarDataMgr().getHistoryLevelScoreByLevel(level);
        if (score > historyLevelScore)
        {
            setHistoryLevelScore(score);
        }
        
        
        if (isGameOver()) // 游戏结束
        {
            changeState( new GameOverState(this) );
        }
        else // 下一等级
        {
            changeState( new GamePassLevelState(this) );
        }
    }
}

void PopStar::onSelect(const SelectStar& selectStar)
{
    clearSelectStars();
    
    std::map<int, Star*> checkList;
    checkList.insert(std::make_pair(selectStar.row*REVERSE_NUM+selectStar.col, selectStar.star));
    
    int index = selectStar.star->getIndex();
    while (!checkList.empty())
    {
        std::map<int, Star*>::iterator iter;
        for (iter = checkList.begin(); iter != checkList.end(); )
        {
            int row = iter->first/REVERSE_NUM;
            int col = iter->first%REVERSE_NUM;
            
            // 分别检查上下左右的格子
            if (row < (ROW_NUM-1)) // 上
            {
                int checkRow = row+1;
                Star* temp = stars[checkRow][col];
                if (temp && (temp->getIndex() == index) && !temp->isHighLight())
                {
                    checkList.insert( std::make_pair((checkRow)*REVERSE_NUM + col, temp) );
                }
            }
            
            if (row > 0) // 下
            {
                int checkRow = row-1;
                Star* temp = stars[checkRow][col];
                if (temp && (temp->getIndex() == index) && !temp->isHighLight())
                {
                    checkList.insert( std::make_pair((checkRow)*REVERSE_NUM + col, temp) );
                }
            }
            
            if (col > 0) // 左
            {
                int checkCol = col-1;
                Star* temp = stars[row][checkCol];
                if (temp && (temp->getIndex() == index) && !temp->isHighLight())
                {
                    checkList.insert( std::make_pair(row*REVERSE_NUM + checkCol, temp) );
                }
            }
            
            if (col < (COL_NUM-1)) // 右
            {
                int checkCol = col+1;
                Star* temp = stars[row][checkCol];
                if (temp && (temp->getIndex() == index) && !temp->isHighLight())
                {
                    checkList.insert( std::make_pair(row*REVERSE_NUM + checkCol, temp) );
                }
            }
            
            Star* star = iter->second;
            if ((star->getIndex() == index) && !star->isHighLight())
            {
                star->setHighLight(true);
                SelectStar sstar;
                sstar.row = row;
                sstar.col = col;
                sstar.star = star;
                selectStars.push_back(sstar);
                
                checkList.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }
    }
    
    int selectStarNum = selectStars.size();
    if (selectStarNum == 1)
    {
        Star* star = selectStars.begin()->star;
        if (star)
        {
            star->setHighLight(false);
            selectStars.clear();
        }
        
        gameLayer->onGuiEvent(EVENT_UPDATE_REDUCE_SCORE, 0);
    }
    else
    {
        int score = getPopStarDataMgr().getScoreByReduceNum(selectStarNum);
        gameLayer->onGuiEvent(EVENT_UPDATE_REDUCE_SCORE, selectStarNum, score);
    }
}

void PopStar::clearSelectStars()
{
    // 取消之前选中的
    StarListIter iter;
    for (iter = selectStars.begin(); iter != selectStars.end(); ++iter)
    {
        iter->star->setHighLight(false);
    }
    selectStars.clear();
}

bool PopStar::isNeedHoriAdjust()
{
    bool needHoriAdjust = false; // 是否需要水平调整
    bool haveHole = false;
    for (int i = 0; i < COL_NUM; ++i)
    {
        Star* star = stars[0][i];
        if (!star)
        {
            haveHole = true;
        }
        else if (haveHole)
        {
            needHoriAdjust = true;
        }
    }
    
    return needHoriAdjust;
}

int PopStar::getCheckEndCol()
{
    int endCol = COL_NUM-1;
    while (endCol >= 0)
    {
        if (stars[0][endCol])
        {
            break;
        }
        --endCol;
    }
    
    return endCol;
}

int PopStar::getLeftStarNum()
{
    int num = 0;
    for (int row = 0; row < ROW_NUM; ++row)
    {
        for (int col = 0; col < COL_NUM; ++col)
        {
            if (stars[row][col])
            {
                ++num;
            }
        }
    }
    
    return num;
}

void PopStar::clearStarsOneByOne()
{
    for (int row = ROW_NUM-1; row >= 0; --row)
    {
        for (int col = COL_NUM-1; col >= 0; --col)
        {
            Star* star = stars[row][col];
            if (star)
            {
                gameLayer->removeChild(star);
                stars[row][col] = NULL;
                
                return;
            }
        }
    }
}

void PopStar::gotoNextLevel()
{
    int level = getPopStarDataMgr().getLevel() + 1;
    getPopStarDataMgr().setLevel(level);
    
    changeLayer( gameLayer, GameLevel::create() );
}

void PopStar::gotoStartLayer()
{
    getPopStarDataMgr().setScore(0);
    getPopStarDataMgr().setLevel(1);
    
    changeLayer( gameLayer, StartLayer::create() );
}