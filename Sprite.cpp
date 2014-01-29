// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Game2.h"
#include "Sprite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//************************************
// Method:    Draw
// FullName:  CSprite::Draw
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CDibSection &image
//************************************
void CSprite::Draw(CDibSection &image)
{
	image.Mix(*m_dibDib, m_ptDrawPos, m_szSize, m_ptSrcPos);
}

//************************************
// Method:    Draw
// FullName:  CSprite::Draw
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CDibSection &image
// Parameter: CRect rect
//************************************
void CSprite::Draw(CDibSection &image, CRect rect)
{
	CRect DrawRect(m_ptDrawPos, m_szSize);
	DrawRect &= rect;
	if (!DrawRect.IsRectEmpty())
	{
		CPoint src = m_ptSrcPos;
		if (m_ptDrawPos.x < DrawRect.left)
		{
			src.x += (DrawRect.left - m_ptDrawPos.x);
		}
		if (m_ptDrawPos.y < DrawRect.top)
		{
			src.y += (DrawRect.top - m_ptDrawPos.y);
		}
		image.Mix(*m_dibDib, CPoint(DrawRect.left, DrawRect.top), DrawRect.Size(), src);
	}
}



