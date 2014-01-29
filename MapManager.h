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

/* 地图管理器（游戏的主管理类） */
class CMapManager  
{
protected:

	/* 游戏的状态 */
	enum	
	{
		STATUS_NONE    = 0,  // 没有状态
		STATUS_MOVE    = 1,  // 移动状态
		STATUS_START   = 2,
		STATUS_END     = 3,
	} ;

	enum 
	{
		PLAYER1 = 0,  // 玩家1
		PLAYER2 = 1,  // 玩家2
	};

	/* 贴图的类型 */
	enum	
	{
		NONE     = 0,       // 空
		CHESS    = 1 << 0,  // 玩家
		MOVEDIST = 1 << 1,  // 可移动标记
	} ;

	/* 棋子的颜色 */
	enum
	{
		RED   = 1,
		BLACK = 2,
	};

	/* 棋子的类型 */
	enum
	{
		CHE   = 1,  // 车
		MA    = 2,  // 马
		XIANG = 3,  // 象，相
		SHI   = 4,  // 士，仕
		JIANG = 5,  // 将，帅
		PAO   = 6,  // 炮
		BING  = 7,  // 兵，卒
	};

	/************************************************************************/	
	/* 地图每一格的基本信息类												*/
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
		CMapSprite *sprite;		// 在地图上的贴图零件
		unsigned char type;		// 在地图上是什么类型贴图
	} ;
	/************************************************************************/

	friend class MapData;

public:
	/* 构造/析构函数 */
	CMapManager();
	virtual ~CMapManager();

	/* 创建窗口 */
	BOOL Create(CWnd *wnd, int width, int height);
	
	/* 状态转换函数 */
	void ChangePlayer();	
	void StartGame();
	void ReDo();
	void EndGame();
	
	/* 初始化函数 */
	void InitMapStatus();
	void InitChess();	
	void AddChess(int x, int y, UINT color, UINT  type);
			
	/* 消息响应函数 */
	void OnRButtonDown(CPoint point);
	void OnLButtonDown(CPoint point);
	void OnMouseMove(CPoint point);
	void OnLButtonUp(CPoint point);

	/* 绘制函数 */
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

	/* 显示/消除移动路径 */
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
	/* 图片 */
	CDibSection Button;    // 按钮
	CDibSection MsButton;  // 音乐按钮
	CDibSection	Cursor;    // 鼠标
	CDibSection	MapParts;  // 底图
	CDibSection	View;      // 所有图片都绘制在此上
	CDibSection chess;     // 棋子
	CDibSection Black;     // “将军”
	CDibSection Red;       // “元帅”
	CMidi midi;            // 背景音乐

	CMapSprite  MBlack;    // 将军贴图
	CMapSprite  MRed;      // 元帅贴图
	CMapSprite  MBtStart;  // 开始按钮贴图
	CMapSprite  MBtRedo;   // 悔棋按钮贴图
	CMapSprite  MBtEnd;    // 结束按钮贴图
	CMapSprite  MBtMsStart;// 播放音乐按钮贴图
	CMapSprite  MBtMsPause;// 暂停音乐按钮贴图
	CMapSprite  MBtMsStop; // 停止音乐按钮贴图

	/* 固定数据 */
	CRect	MapRect;         // 窗口区域
	CRect   ChessBoardRect;  // 棋盘区域
	CRect   ButtonRect;      // 按钮区域
	CWnd    *Window;         // 窗口类
	vector<CChess> Player1;   // 玩家1的棋子
	vector<CChess> Player2;   // 玩家2的棋子

	/* 信息数据 */
 	MapData Map[10][9];       // 记录地图信息的二维数组
	CChess *SelChess;         // 当前选中的棋子
	int status;               // 当前游戏的状态
	int CulPlayer;            // 当前玩家
	CPoint	CursorPos;        // 当前鼠标坐标（格子）

	struct LastAction
	{
		CPoint From;   // 最后一次移动是从From位置移到To位置
		CPoint To;
		MapData Origin;  // 被覆盖的数据
		bool status;     // 当前能否悔棋
	};

	LastAction lastAction;     // 记录最后一次的动作（用于悔棋操作）
};

#endif // !defined(AFX_MAPMANAGER_H__C123EED2_D1C7_4246_8183_88E5AEB5C048__INCLUDED_)
