// Chess.h: interface for the CChess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHESS_H__656F9858_93F3_4535_A099_1E1D92DF4A6B__INCLUDED_)
#define AFX_CHESS_H__656F9858_93F3_4535_A099_1E1D92DF4A6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapSprite.h"

class CChess : public CMapSprite  
{
public:
	CChess();
	CChess(CDibSection* dib, CPoint pos, CSize size, CPoint src, int depth);
	CChess(CDibSection* dib, CPoint pos, CSize size, CPoint src, int depth, 
		UINT color, UINT type);
	virtual ~CChess();

	bool IsExit();
	bool operator==(const CChess& x) const;
	bool operator!=(const CChess& x) const;

public:
	
	UINT m_uColor;
	UINT m_uType;
	bool m_bIsExit;
};


inline CChess::CChess(CDibSection* dib, CPoint pos, CSize size, CPoint src, int depth)
	:CMapSprite(dib, pos, size, src, depth)
{
	m_uColor = 0;
	m_uType = 0;
	m_bIsExit = true;
}

inline CChess::CChess(CDibSection* dib, CPoint pos, CSize size, CPoint src, int depth, 
	UINT color, UINT type):CMapSprite(dib, pos, size, src, depth)
{
	m_uColor = color;
	m_uType = type;
	m_bIsExit = true;
}

inline bool CChess::operator ==(const CChess & x) const
{
	return m_uColor == x.m_uColor;
}

inline bool CChess::operator !=(const CChess & x) const
{
	return m_uColor != x.m_uColor;
}

inline bool CChess::IsExit()
{
	return m_bIsExit;
}
#endif // !defined(AFX_CHESS_H__656F9858_93F3_4535_A099_1E1D92DF4A6B__INCLUDED_)
