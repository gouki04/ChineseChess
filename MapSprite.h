// MapSprite.h: interface for the CMapSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPSPRITE_H__444D1B38_0DE5_43B0_B13D_2DA3580703A6__INCLUDED_)
#define AFX_MAPSPRITE_H__444D1B38_0DE5_43B0_B13D_2DA3580703A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprite.h"
#include "coordinate.h"

class CMapSprite : public CSprite  
{
public:
	
	CMapSprite();
	CMapSprite(CDibSection* dib, CPoint pos, CSize size, CPoint src, int depth);
	virtual ~CMapSprite();
	
	CPoint GetMapIndex() const;
	void SetMapIndex(CPoint index);

	BOOL operator==(CPoint pt);
	BOOL operator<(const CMapSprite &x) const;

protected:
	CPoint m_ptMapIndex;
};


//************************************
// Method:    CMapSprite
// FullName:  CMapSprite::CMapSprite
// Access:    public 
// Returns:   
// Qualifier:
//************************************
inline CMapSprite::CMapSprite()
{

}

//************************************
// Method:    CMapSprite
// FullName:  CMapSprite::CMapSprite
// Access:    public 
// Returns:   
// Qualifier: :CSprite(dib, IndexToPoint(index), size, depth)
// Parameter: CDibSection* dib
// Parameter: CPoint index
// Parameter: CSize size
// Parameter: int depth
//************************************
inline CMapSprite::CMapSprite(CDibSection* dib, CPoint point, CSize size, CPoint src, int depth)
	:CSprite(dib, point, size, src, depth)
{
	m_ptMapIndex = PointToIndex(point);
}

//************************************
// Method:    ~CMapSprite
// FullName:  CMapSprite::~CMapSprite
// Access:    virtual public 
// Returns:   
// Qualifier:
//************************************
inline CMapSprite::~CMapSprite()
{

}

//************************************
// Method:    SetMapIndex
// FullName:  CMapSprite::SetMapIndex
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint index
//************************************
inline void CMapSprite::SetMapIndex(CPoint index)
{
	m_ptMapIndex = index;
}

//************************************
// Method:    GetMapIndex
// FullName:  CMapSprite::GetMapIndex
// Access:    public const 
// Returns:   CPoint
// Qualifier: const
//************************************
inline CPoint CMapSprite::GetMapIndex() const
{
	return m_ptMapIndex;
}

//************************************
// Method:    operator==
// FullName:  CMapSprite::operator==
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: CPoint pt
//************************************
inline BOOL CMapSprite::operator ==(CPoint pt)
{
	return m_ptMapIndex == pt;
}

//************************************
// Method:    operator<
// FullName:  CMapSprite::operator<
// Access:    public const 
// Returns:   bool
// Qualifier: const
// Parameter: const CMapSprite &x
//************************************
inline BOOL CMapSprite::operator <(const CMapSprite &x) const
{
	if (m_ptDrawPos.y + m_szSize.cy == x.m_ptDrawPos.y + x.m_szSize.cy)
	{
		if (m_ptDrawPos.x == x.m_ptDrawPos.x)
		{
			return m_iDepth < x.m_iDepth;
		}
		return m_ptDrawPos.x < x.m_ptDrawPos.x;
	}
	return m_ptDrawPos.y + m_szSize.cy < x.m_ptDrawPos.y + x.m_szSize.cy;
}

#endif // !defined(AFX_MAPSPRITE_H__444D1B38_0DE5_43B0_B13D_2DA3580703A6__INCLUDED_)
