#include "Star.h"

USING_NS_CC;

namespace
{
	const int MAX_STAR_NUM = 5;
	const float MOVE_SPEED = 300.0f;
	const std::string STAR_PATH[MAX_STAR_NUM] =
	{
		std::string("stars/red.png"),
		std::string("stars/yellow.png"),
		std::string("stars/green.png"),
		std::string("stars/blue.png"),
		std::string("stars/purple.png"),
	};

	void Clamp(int& val, int min, int max)
	{
		if (val < min)
		{
			val = min;
		}
		else if (val > max)
		{
			val = max;
		}
	}
}

Star::Star(int index)
{
	this->index = index;
	Clamp(this->index, 1, MAX_STAR_NUM);
	highlight = false;

	init();
}

Star* Star::create(int index)
{
	Star* pRet = new Star(index);
	std::string path = pRet->getPath();
	if (pRet && pRet->initWithFile(path.c_str()))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool Star::isHighLight()
{
	return highlight;
}

void Star::setHighLight(bool hl)
{
	highlight = hl;
}

int Star::getIndex()
{
	return index;
}

void Star::setPos(const cocos2d::CCPoint& pos)
{
	CCSprite::setPosition(pos);

	destPos = pos;
}

cocos2d::CCPoint Star::getDestPos()
{
	return destPos;
}

void Star::setDestPos(const cocos2d::CCPoint& pos)
{
	destPos = pos;
}

bool Star::init()
{
	if (!CCSprite::init())
	{
		return false;
	}

	return true;
}

void Star::draw(void)
{
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");

	CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");

	CC_NODE_DRAW_SETUP();

	if (highlight)
	{
		ccGLBlendFunc(GL_SRC_COLOR, GL_ONE);
	}
	else
	{
		ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	}


	ccGLBindTexture2D( m_pobTexture->getName() );
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );



#define kQuadSize sizeof(m_sQuad.bl)
#ifdef EMSCRIPTEN
	long offset = 0;
	setGLBufferData(&m_sQuad, 4 * kQuadSize, 0);
#else
	long offset = (long)&m_sQuad;
#endif // EMSCRIPTEN

	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	CHECK_GL_ERROR_DEBUG();


#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCPoint vertices[4]={
		ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
		ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
		ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
		ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = this->getTextureRect().size;
	CCPoint offsetPix = this->getOffsetPosition();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW



	CC_INCREMENT_GL_DRAWS(1);

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}

void Star::onUpdate(float delta)
{
	CCPoint curPos = getPosition();
	if ( curPos.equals(destPos) )
	{
		setPosition(destPos);
	}

	float speedX = 0.f;
	float speedY = 0.f;
	if ( fabs(curPos.x - destPos.x) < FLT_EPSILON )
	{
		speedX = 0.f;
	}
	else if (curPos.x < destPos.x)
	{
		speedX = MOVE_SPEED;
	}
	else
	{
		speedX = -MOVE_SPEED;
	}

	if ( fabs(curPos.y - destPos.y) < FLT_EPSILON )
	{
		speedY = 0.f;
	}
	else if (curPos.y < destPos.y)
	{
		speedY = MOVE_SPEED;
	}
	else
	{
		speedY = -MOVE_SPEED;
	}

	float tempPosX = curPos.x + delta * speedX;
	if (speedX > 0)
	{
		if (tempPosX > destPos.x)
		{
			tempPosX = destPos.x;
		}
	}
	else if (speedX < 0)
	{
		if (tempPosX < destPos.x)
		{
			tempPosX = destPos.x;
		}
	}

	float tempPosY = curPos.y + delta * speedY;
	if (speedY > 0)
	{
		if (tempPosY > destPos.y)
		{
			tempPosY = destPos.y;
		}
	}
	else if (speedY < 0)
	{
		if (tempPosY < destPos.y)
		{
			tempPosY = destPos.y;
		}
	}
	setPosition( ccp(tempPosX, tempPosY) );


	/*if ( ((int)curPos.x != (int)destPos.x) || ((int)curPos.y != (int)destPos.y) )
	{
		float disX = curPos.x - destPos.x;
		float disY = curPos.y - destPos.y;

		CCPoint newPos;
		newPos.x = curPos.x - disX * delta * MOVE_SPEED;
		newPos.y = curPos.y - disY * delta * MOVE_SPEED;
		setPosition(newPos);
	}*/
}


std::string Star::getPath()
{
	return STAR_PATH[index-1];
}
