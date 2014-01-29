// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__ED75CD31_5822_4618_A05E_EEC864F5AFFC__INCLUDED_)
#define AFX_SPRITE_H__ED75CD31_5822_4618_A05E_EEC864F5AFFC__INCLUDED_

#include "DibSection.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSprite  
{
public:
	CSprite();
	CSprite(CDibSection* dib, CPoint drawPos, CSize size, int depth = 0);
	CSprite(CDibSection* dib, CPoint drawPos, CSize size, CPoint srcPos, int depth = 0);
	virtual ~CSprite();

	void Draw(CDibSection &image, CRect rect);
	void Draw(CDibSection &image);

	BOOL PtIn(CPoint pt);

	void SetSrcPos(CPoint pos);
	void SetSrcPos(int x, int y);
	CRect GetRect() const;
	void SetDrawPos(CPoint pos);
	CPoint GetDrawPos() const;
	int GetDepth() const;
	void SetDepth(int depth);

	void Set(CDibSection* dib, CPoint drawPos, CSize size, CPoint srcPos);
	
public:
	int m_iDepth;
	CDibSection *m_dibDib;
	CSize m_szSize;
	CPoint m_ptSrcPos;
	CPoint m_ptDrawPos;
};


//************************************
// Method:    CSprite
// FullName:  CSprite::CSprite
// Access:    public 
// Returns:   
// Qualifier:
//************************************
inline CSprite::CSprite()
{
	m_dibDib = 0;
	m_ptDrawPos = 0;
	m_ptSrcPos = 0;
	m_szSize = 0;
	m_iDepth = 0;
}


//************************************
// Method:    CSprite
// FullName:  CSprite::CSprite
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: CDibSection* dib
// Parameter: CPoint drawPos
// Parameter: CSize size
// Parameter: int depth 
//************************************
inline CSprite::CSprite(CDibSection* dib, CPoint drawPos, CSize size, 
						int depth /* = 0 */)
{
	m_dibDib = dib;
	m_ptDrawPos = drawPos;
	m_szSize = size;
	m_iDepth = depth;
}

//************************************
// Method:    CSprite
// FullName:  CSprite::CSprite
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: CDibSection* dib
// Parameter: CPoint drawPos
// Parameter: CSize size
// Parameter: CPoint srcPos
// Parameter: int depth
//************************************
inline CSprite::CSprite(CDibSection* dib, CPoint drawPos, CSize size, 
						CPoint srcPos, int depth /* = 0 */)
{
	m_dibDib = dib;
	m_ptDrawPos = drawPos;
	m_ptSrcPos = srcPos;
	m_szSize = size;
	m_iDepth = depth;
}

//************************************
// Method:    ~CSprite
// FullName:  CSprite::~CSprite
// Access:    virtual public 
// Returns:   
// Qualifier:
//************************************
inline CSprite::~CSprite()
{

}

//************************************
// Method:    Set
// FullName:  CSprite::Set
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CDibSection *dib
// Parameter: CPoint drawPos
// Parameter: CSize size
// Parameter: CPoint srcPos
//************************************
inline void CSprite::Set(CDibSection *dib, CPoint drawPos, CSize size, CPoint srcPos)
{
	m_dibDib = dib;
	m_ptDrawPos = drawPos;
	m_ptSrcPos = srcPos;
	m_szSize = size;
}

//************************************
// Method:    GetDrawPos
// FullName:  CSprite::GetDrawPos
// Access:    public const 
// Returns:   CPoint
// Qualifier: const
//************************************
inline CPoint CSprite::GetDrawPos() const
{
	return m_ptDrawPos;
}

//************************************
// Method:    SetDrawPos
// FullName:  CSprite::SetDrawPos
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint pos
//************************************
inline void CSprite::SetDrawPos(CPoint pos)
{
	m_ptDrawPos = pos;
}

//************************************
// Method:    GetRect
// FullName:  CSprite::GetRect
// Access:    public const 
// Returns:   CRect
// Qualifier: const
//************************************
inline CRect CSprite::GetRect() const
{
	return CRect(m_ptDrawPos, m_szSize);
}

//************************************
// Method:    SetSrcPos
// FullName:  CSprite::SetSrcPos
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint pos
//************************************
inline void CSprite::SetSrcPos(CPoint pos)
{
	m_ptSrcPos = pos;
}

//************************************
// Method:    SetSrcPos
// FullName:  CSprite::SetSrcPos
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int x
// Parameter: int y
//************************************
inline void CSprite::SetSrcPos(int x, int y)
{
	m_ptSrcPos.x = x;
	m_ptSrcPos.y = y;
}

//************************************
// Method:    SetDepth
// FullName:  CSprite::SetDepth
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int depth
//************************************
inline void CSprite::SetDepth(int depth)
{
	m_iDepth = depth;
}

//************************************
// Method:    GetDepth
// FullName:  CSprite::GetDepth
// Access:    public 
// Returns:   int
// Qualifier:
//************************************
inline int CSprite::GetDepth() const
{
	return m_iDepth;
}

//************************************
// Method:    PtIn
// FullName:  CSprite::PtIn
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: CPoint pt
//************************************
inline BOOL CSprite::PtIn(CPoint pt)
{
	return CRect(m_ptDrawPos, m_szSize).PtInRect(pt);
}

#endif // !defined(AFX_SPRITE_H__ED75CD31_5822_4618_A05E_EEC864F5AFFC__INCLUDED_)
