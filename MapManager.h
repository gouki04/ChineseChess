// MapManager.h: interface for the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPMANAGER_H__C123EED2_D1C7_4246_8183_88E5AEB5C048__INCLUDED_)
#define AFX_MAPMANAGER_H__C123EED2_D1C7_4246_8183_88E5AEB5C048__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ***************************************************************
//  MapManager   version:  1.0   ? date: 08/05/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
//	Author: gouki04
// ***************************************************************

#include "Chess.h"
#include "MapSprite.h"
#include "Sprite.h"
#include "Midi.h"

#include <vector>
using namespace std;

/* ��ͼ����������Ϸ���������ࣩ */
class CMapManager  
{
protected:

	/* ��Ϸ��״̬ */
	enum	
	{
		STATUS_NONE    = 0,  // û��״̬
		STATUS_MOVE    = 1,  // �ƶ�״̬
		STATUS_START   = 2,
		STATUS_END     = 3,
	} ;

	enum 
	{
		PLAYER1 = 0,  // ���1
		PLAYER2 = 1,  // ���2
	};

	/* ��ͼ������ */
	enum	
	{
		NONE     = 0,       // ��
		CHESS    = 1 << 0,  // ���
		MOVEDIST = 1 << 1,  // ���ƶ����
	} ;

	/* ���ӵ���ɫ */
	enum
	{
		RED   = 1,
		BLACK = 2,
	};

	/* ���ӵ����� */
	enum
	{
		CHE   = 1,  // ��
		MA    = 2,  // ��
		XIANG = 3,  // ����
		SHI   = 4,  // ʿ����
		JIANG = 5,  // ����˧
		PAO   = 6,  // ��
		BING  = 7,  // ������
	};

	/************************************************************************/	
	/* ��ͼÿһ��Ļ�����Ϣ��												*/
	class MapData 
	{
	  public:
		MapData()
		{
			sprite = 0;
			type = NONE;
		}

		void Set()
		{
			sprite = 0;
			type = NONE;
		}
	  public:
		CMapSprite *sprite;		// �ڵ�ͼ�ϵ���ͼ���
		unsigned char type;		// �ڵ�ͼ����ʲô������ͼ
	} ;
	/************************************************************************/

	friend class MapData;

public:
	/* ����/�������� */
	CMapManager();
	virtual ~CMapManager();

	/* �������� */
	BOOL Create(CWnd *wnd, int width, int height);
	
	/* ״̬ת������ */
	void ChangePlayer();	
	void StartGame();
	void ReDo();
	void EndGame();
	
	/* ��ʼ������ */
	void InitMapStatus();
	void InitChess();	
	void AddChess(int x, int y, UINT color, UINT  type);
			
	/* ��Ϣ��Ӧ���� */
	void OnRButtonDown(CPoint point);
	void OnLButtonDown(CPoint point);
	void OnMouseMove(CPoint point);
	void OnLButtonUp(CPoint point);

	/* ���ƺ��� */
	void Draw(CDC &dc);
	void DrawMap();
	void DrawMap(int x, int y);
	void DrawMap(CRect rect);
	void DrawChess(int x, int y);
	void DrawChess();
	void DrawButton();
	void DrawCursor();
	void ClearCursor();
	void SetCursor(CPoint point);

	/* ��ʾ/�����ƶ�·�� */
	void FindDist();
	void ClearDist();

	BOOL CheckDist(int x, int y);
	BOOL ClearDist(int x, int y);
	
	void FindDistChe();
	void FindDistMa();
	void FindDistXiang();
	void FindDistShi();
	void FindDistJiang();
	void FindDistPao();
	void FindDistBing();
	
	void ClearDistChe();
	void ClearDistMa();
	void ClearDistXiang();
	void ClearDistShi();
	void ClearDistJiang();
	void ClearDistPao();
	void ClearDistBing();

protected:
	/* ͼƬ */
	CDibSection Button;    // ��ť
	CDibSection MsButton;  // ���ְ�ť
	CDibSection	Cursor;    // ���
	CDibSection	MapParts;  // ��ͼ
	CDibSection	View;      // ����ͼƬ�������ڴ���
	CDibSection chess;     // ����
	CDibSection Black;     // ��������
	CDibSection Red;       // ��Ԫ˧��
	CMidi midi;            // ��������

	CMapSprite  MBlack;    // ������ͼ
	CMapSprite  MRed;      // Ԫ˧��ͼ
	CMapSprite  MBtStart;  // ��ʼ��ť��ͼ
	CMapSprite  MBtRedo;   // ���尴ť��ͼ
	CMapSprite  MBtEnd;    // ������ť��ͼ
	CMapSprite  MBtMsStart;// �������ְ�ť��ͼ
	CMapSprite  MBtMsPause;// ��ͣ���ְ�ť��ͼ
	CMapSprite  MBtMsStop; // ֹͣ���ְ�ť��ͼ

	/* �̶����� */
	CRect	MapRect;         // ��������
	CRect   ChessBoardRect;  // ��������
	CRect   ButtonRect;      // ��ť����
	CWnd    *Window;         // ������
	vector<CChess> Player1;   // ���1������
	vector<CChess> Player2;   // ���2������

	/* ��Ϣ���� */
 	MapData Map[10][9];       // ��¼��ͼ��Ϣ�Ķ�ά����
	CChess *SelChess;         // ��ǰѡ�е�����
	int status;               // ��ǰ��Ϸ��״̬
	int CulPlayer;            // ��ǰ���
	CPoint	CursorPos;        // ��ǰ������꣨���ӣ�

	struct LastAction
	{
		CPoint From;   // ���һ���ƶ��Ǵ�Fromλ���Ƶ�Toλ��
		CPoint To;
		MapData Origin;  // �����ǵ�����
		bool status;     // ��ǰ�ܷ����
	};

	LastAction lastAction;     // ��¼���һ�εĶ��������ڻ��������
};

#endif // !defined(AFX_MAPMANAGER_H__C123EED2_D1C7_4246_8183_88E5AEB5C048__INCLUDED_)
