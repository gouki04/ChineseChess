// MapManager.cpp: implementation of the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Game2.h"
#include "DibSection.h"
#include "MapManager.h"
#include "Coordinate.h"
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapManager::CMapManager()
{

}

CMapManager::~CMapManager()
{

}

//************************************
// Method:    Create
// FullName:  CMapManager::Create
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: CWnd *wnd
// Parameter: int width
// Parameter: int height
// Description: ������������
//************************************
BOOL CMapManager::Create(CWnd *wnd, int width, int height)
{
	Window = wnd;
	MapRect.SetRect(0, 0, width, height);
	ChessBoardRect.SetRect(OFFSET_X * GRID_WIDTH, OFFSET_Y * GRID_HEIGHT,
		(OFFSET_X + 9) * GRID_WIDTH, (OFFSET_Y + 10) * GRID_HEIGHT);
	
	/* ��ʼ����ͼ */
	int i, j;
	for (i=0; i<10; i++)
	{
		for (j=0; j<9; j++)
		{
			Map[i][j].Set();
		}
	}

	// ��ȡҪʹ�õ�ͼƬ
	if (!Cursor.LoadBmp("image\\Cursor.bmp")
	 || !MapParts.LoadBmp("image\\ChessBoard.bmp")
	 || !View.Create(640, 480, 24)
	 || !chess.LoadBmp("image\\Chess.bmp")
	 || !Button.LoadBmp("image\\Button.bmp")
	 || !MsButton.LoadBmp("image\\MsButton.bmp")
	 || !Black.LoadBmp("image\\Jiang.bmp")
	 || !Red.LoadBmp("image\\Shuai.bmp"))
	{
		return false;
	}

	/* ��������ͼ */
	MBlack.Set    (&Black,    IndexToPoint(13, 3),       CSize(7 * 32, 10 * 32), CPoint(0, 0));
	MRed.Set      (&Red,      IndexToPoint(13, 3),       CSize(7 * 32, 10 * 32), CPoint(0, 0));
	MBtStart.Set  (&Button,   IndexToPoint(3, 13),       CSize(64, 32),          CPoint(0, 0));
	MBtRedo.Set   (&Button,   CPoint(5.5 * 32, 13 * 32), CSize(64, 32),          CPoint(128, 64));
	MBtEnd.Set    (&Button,   IndexToPoint(8, 13),       CSize(64, 32),          CPoint(0, 32));
	MBtMsStart.Set(&MsButton, IndexToPoint(17, 0),       CSize(32, 32),          CPoint(0, 0));
	MBtMsPause.Set(&MsButton, IndexToPoint(18, 0),       CSize(32, 32),          CPoint(0, 32));
	MBtMsStop.Set (&MsButton, IndexToPoint(19, 0),       CSize(32, 32),          CPoint(0, 64));

	/* ��ʼ�� */
	DrawMap();
	DrawButton();
	CursorPos.x = 0;
	CursorPos.y = 0;
 	status = STATUS_END;
	midi.Open(Window);
	return TRUE;
}

//************************************
// Method:    ChangePlayer
// FullName:  CMapManager::ChangePlayer
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������Ҳ���
//************************************
void CMapManager::ChangePlayer()
{
	CulPlayer = !CulPlayer;
	
	DrawMap(CRect(12, 3, 20, 14));
	if (CulPlayer == PLAYER1)
	{
		MBlack.Draw(View);
	}
	else
	{
		MRed.Draw(View);
	}
}

//************************************
// Method:    StartGame
// FullName:  CMapManager::StartGame
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʼ��Ϸ����ʼ�����ݣ�
//************************************
void CMapManager::StartGame()
{
	if (!Player1.empty())
	{
		Player1.clear();
	}
	if (!Player2.empty())
	{
		Player2.clear();
	}
	Player1.resize(16);
	Player2.resize(16);
	
	int i, j;
	for (i=0; i<10; i++)
	{
		for (j=0; j<9; j++)
		{
			Map[i][j].Set();
		}
	}
	
	lastAction.status = false;
	InitChess();
	DrawMap();
	DrawChess();
	DrawButton();
	MBlack.Draw(View);

	CursorPos.x = 0;
	CursorPos.y = 0;
	SelChess = 0;
	status = STATUS_NONE;
	CulPlayer = PLAYER1;
}

//************************************
// Method:    ReDo
// FullName:  CMapManager::ReDo
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �������
//************************************
void CMapManager::ReDo()
{
	if(SelChess != 0)
	{
		ClearDist();
		SelChess->Draw(View);
	}
	
	Map[lastAction.From.y - OFFSET_Y][lastAction.From.x - OFFSET_X] = Map[lastAction.To.y - OFFSET_Y][lastAction.To.x - OFFSET_X];
	Map[lastAction.To.y - OFFSET_Y][lastAction.To.x - OFFSET_X].Set();
	
	Map[lastAction.From.y - OFFSET_Y][lastAction.From.x - OFFSET_X].sprite->SetMapIndex(lastAction.From);
	Map[lastAction.From.y - OFFSET_Y][lastAction.From.x - OFFSET_X].sprite->SetDrawPos(CPoint(lastAction.From.x * GRID_WIDTH, lastAction.From.y * GRID_HEIGHT));
	Map[lastAction.From.y - OFFSET_Y][lastAction.From.x - OFFSET_X].sprite->Draw(View);
	
	if(lastAction.Origin.sprite != 0)
	{
		Map[lastAction.To.y - OFFSET_Y][lastAction.To.x - OFFSET_X] = lastAction.Origin;
		lastAction.Origin.sprite->Draw(View);
	}
	else
	{
		DrawMap(lastAction.To.x, lastAction.To.y);
	}

	lastAction.status = false;
	SelChess = 0;
	status = STATUS_NONE;
	ChangePlayer();
	InvalidateRect(Window->m_hWnd, ChessBoardRect, FALSE);
}

//************************************
// Method:    EndGame
// FullName:  CMapManager::EndGame
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������Ϸ
//************************************
void CMapManager::EndGame()
{
	PostQuitMessage(WM_QUIT);
}

//************************************
// Method:    InitChess
// FullName:  CMapManager::InitChess
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʼ��ȫ������
//************************************
void CMapManager::InitChess()
{
	AddChess(0, 0, BLACK, CHE);
	AddChess(1, 0, BLACK, MA);
	AddChess(2, 0, BLACK, XIANG);
	AddChess(3, 0, BLACK, SHI);
	AddChess(4, 0, BLACK, JIANG);
	AddChess(5, 0, BLACK, SHI);
	AddChess(6, 0, BLACK, XIANG);
	AddChess(7, 0, BLACK, MA);
	AddChess(8, 0, BLACK, CHE);
	AddChess(1, 2, BLACK, PAO);
	AddChess(7, 2, BLACK, PAO);
	AddChess(0, 3, BLACK, BING);
	AddChess(2, 3, BLACK, BING);
	AddChess(4, 3, BLACK, BING);
	AddChess(6, 3, BLACK, BING);
	AddChess(8, 3, BLACK, BING);

	AddChess(0, 9, RED, CHE);
	AddChess(1, 9, RED, MA);
	AddChess(2, 9, RED, XIANG);
	AddChess(3, 9, RED, SHI);
	AddChess(4, 9, RED, JIANG);
	AddChess(5, 9, RED, SHI);
	AddChess(6, 9, RED, XIANG);
	AddChess(7, 9, RED, MA);
	AddChess(8, 9, RED, CHE);
	AddChess(1, 7, RED, PAO);
	AddChess(7, 7, RED, PAO);
	AddChess(0, 6, RED, BING);
	AddChess(2, 6, RED, BING);
	AddChess(4, 6, RED, BING);
	AddChess(6, 6, RED, BING);
	AddChess(8, 6, RED, BING);
}

//************************************
// Method:    AddChess
// FullName:  CMapManager::AddChess
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int x  �������꣨���ӣ�
// Parameter: int y  �������꣨���ӣ�
// Parameter: UINT color
// Parameter: UINT type
// Description: ��ָ��λ�����ָ��������
//************************************
void CMapManager::AddChess(int x, int y, UINT color, UINT type)
{
	CSize size;
	size.cx = 32;
	size.cy = 32;

	CPoint src;
	
	if (color == RED)
	{
		src.y = 32;	
	}
	else if (color == BLACK)
	{
		src.y = 0;
	}

	switch(type)
	{
	case CHE:
		src.x = 0;
		break;
	case MA:
		src.x = 32;
		break;
	case XIANG:
		src.x = 64;
	    break;
	case SHI:
		src.x = 96;
	    break;
	case JIANG:
		src.x = 128;
		break;
	case PAO:
		src.x = 160;
		break;
	case BING:
		src.x = 192;
	    break;
	default:
	    break;
	}

	if (color == BLACK)
	{
		Player1.push_back(CChess(&chess, CPoint((x + OFFSET_X) * GRID_WIDTH, 
			(y + OFFSET_Y) * GRID_HEIGHT), size, src, 10, color, type));
		CChess &s = Player1.back();
		Map[y][x].type |= CHESS;
 		Map[y][x].sprite = &s;
	}
 	else if (color == RED)
 	{
		Player2.push_back(CChess(&chess, CPoint((x + OFFSET_X) * GRID_WIDTH, 
			(y + OFFSET_Y) * GRID_HEIGHT), size, src, 10, color, type));
		CChess &s = Player2.back();
		Map[y][x].type |= CHESS;
 		Map[y][x].sprite = &s;
 	}
}

//************************************
// Method:    InitMapStatus
// FullName:  CMapManager::InitMapStatus
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������ͼ��MOVEDIST����
//************************************
void CMapManager::InitMapStatus()
{
	int i, j;
	for (i=0; i<10; i++)
	{
		for (j=0; j<9; j++)
		{
			Map[i][j].type &= ~MOVEDIST;
		}
	}
}

//************************************
// Method:    OnLButtonDown
// FullName:  CMapManager::OnLButtonDown
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point  �������꣨���أ�
// Description: ���������²�������Ϊ���̺Ͱ�ť��
//************************************
void CMapManager::OnLButtonDown(CPoint point)
{
	CPoint pos = PointToIndex(point);
	int cx = pos.x - OFFSET_X;
	int cy = pos.y - OFFSET_Y;

	/* �������� */
	if(ChessBoardRect.PtInRect(point))
	{
		switch (status) 
		{
		/* ��״̬�°����ӽ����ƶ�״̬ */
		case STATUS_NONE:	
			if (Map[cy][cx].type & CHESS) 
			{
				if((CulPlayer == PLAYER1 && 
					((CChess *)Map[cy][cx].sprite)->m_uColor == BLACK) ||
				   (CulPlayer == PLAYER2 && 
				    ((CChess *)Map[cy][cx].sprite)->m_uColor == RED))
				{
					SelChess = (CChess *)(Map[cy][cx].sprite);
					status = STATUS_MOVE;
					FindDist();
				}
			}
			break;

		/* �ƶ�״̬�°��� */
		case STATUS_MOVE:
			/* �������� */
			if (Map[cy][cx].type & CHESS)
			{
				/* ������������ */
				if(SelChess !=  (CChess *)Map[cy][cx].sprite)
				{
					/* ���±��������� */
					if(*SelChess == *((CChess *)Map[cy][cx].sprite))
					{
						ClearDist();
						SelChess->Draw(View);
						/* ����ѡ������ */
						SelChess = (CChess *)(Map[cy][cx].sprite);
						/* ��ǿ��ƶ�λ�� */
						InitMapStatus();
						FindDist();
					}
					/* ���¿��ԳԵĶԷ����ӣ�������״̬�� */
					else if (Map[cy][cx].type & MOVEDIST)
					{
						ClearDist();
						lastAction.From = SelChess->GetMapIndex();
						lastAction.To = pos;
						lastAction.Origin = Map[cy][cx];
						lastAction.status = true;
						DrawButton();

						bool isGameEnd = false;
						if (((CChess*)Map[cy][cx].sprite)->m_uType == JIANG)
							isGameEnd = true;

						/* �ƶ����� */
						Map[cy][cx] = Map[SelChess->GetMapIndex().y - OFFSET_Y][SelChess->GetMapIndex().x - OFFSET_X];
						Map[SelChess->GetMapIndex().y - OFFSET_Y][SelChess->GetMapIndex().x - OFFSET_X].Set();

						/* �������ӵ����� */
						SelChess->SetDrawPos(CPoint(pos.x * GRID_WIDTH, pos.y * GRID_HEIGHT));
						SelChess->SetMapIndex(CPoint(pos.x, pos.y));
						SelChess->Draw(View);
						status = STATUS_NONE;
						SelChess = 0;
						if (isGameEnd == true)
						{
							if(AfxMessageBox("���¿�ʼһ�̰ɣ�", MB_YESNO) == IDYES)
							{
								StartGame();
							}
							else
							{
								EndGame();
							}
						}
						else
						{
							ChangePlayer();
						}
						InitMapStatus();	
					}		
				}
				/* ����ԭ���ӣ�������״̬�� */
				else
				{
					ClearDist();
					SelChess->Draw(View);
					status = STATUS_NONE;
					SelChess = 0;
					InitMapStatus();
				}
			}
			/* ���¿��ƶ������򣨺�����״̬�� */
			else if (Map[cy][cx].type & MOVEDIST)
			{
				ClearDist();

				lastAction.From = SelChess->GetMapIndex();
				lastAction.To = pos;
				lastAction.Origin = Map[cy][cx];
				lastAction.status = true;
				DrawButton();
				
				/* �ƶ����� */
				Map[cy][cx] = Map[SelChess->GetMapIndex().y - OFFSET_Y][SelChess->GetMapIndex().x - OFFSET_X];
				Map[SelChess->GetMapIndex().y - OFFSET_Y][SelChess->GetMapIndex().x - OFFSET_X].Set();
				
				/* �������ӵ����� */
				SelChess->SetDrawPos(CPoint(pos.x * GRID_WIDTH, pos.y * GRID_HEIGHT));
				SelChess->SetMapIndex(CPoint(pos.x, pos.y));

				SelChess->Draw(View);
				status = STATUS_NONE;
				ChangePlayer();
				SelChess = 0;
				InitMapStatus();
			}
			/* ���������հ����򣨺�����״̬�� */
			else
			{
				ClearDist();
				SelChess->Draw(View);
				status = STATUS_NONE;
				SelChess = 0;
				InitMapStatus();
			}
			break;

		default:
			break;
		}
	}
	/* ��ʼ��ť */
	else if (MBtStart.PtIn(point))
	{
		DrawMap(CRect(3, 13, 4, 13));
		MBtStart.SetSrcPos(64, 0);
		MBtStart.Draw(View);	
	}
	/* ���尴ť */
	else if (MBtRedo.PtIn(point) && lastAction.status == true)
	{
		DrawMap(CRect(5, 13, 7, 13));
		MBtRedo.SetSrcPos(64, 64);
		MBtRedo.Draw(View);
	}
	/* ������ť*/
	else if (MBtEnd.PtIn(point))
	{
		DrawMap(CRect(8, 13, 9, 13));
		MBtEnd.SetSrcPos(64, 32);
		MBtEnd.Draw(View);
	}
	/* ���ֿ�ʼ��ť */
	else if (MBtMsStart.PtIn(point))
	{
		DrawMap(17, 0);
		MBtMsStart.SetSrcPos(32, 0);
		MBtMsStart.Draw(View);
	}
	/* ������ͣ��ť */
	else if (MBtMsPause.PtIn(point))
	{
		DrawMap(18, 0);
		MBtMsPause.SetSrcPos(32, 32);
		MBtMsPause.Draw(View);
	}
	/* ����ֹͣ��ť */
	else if (MBtMsStop.PtIn(point))
	{
		DrawMap(19, 0);
		MBtMsStop.SetSrcPos(32, 64);
		MBtMsStop.Draw(View);
	}
	InvalidateRect(Window->m_hWnd, MapRect, FALSE);
}

//************************************
// Method:    OnLButtonUp
// FullName:  CMapManager::OnLButtonUp
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point���������꣨���أ���
// Description: �������������������ڰ�ť������
//************************************
void CMapManager::OnLButtonUp(CPoint point)
{
	if (MBtStart.PtIn(point))
	{
		DrawMap(CRect(3, 13, 4, 13));
		MBtStart.SetSrcPos(128, 0);
		MBtStart.Draw(View);
		StartGame();	
	}
	else if (MBtRedo.PtIn(point) && lastAction.status == true)
	{
		DrawMap(CRect(5, 13, 7, 13));
		MBtRedo.SetSrcPos(128, 64);
		MBtRedo.Draw(View);
		ReDo();
	}
	else if (MBtEnd.PtIn(point))
	{
		DrawMap(CRect(8, 13, 9, 13));
		MBtEnd.SetSrcPos(0, 32);
		MBtEnd.Draw(View);
		if(AfxMessageBox("�����Ҫ�뿪��", MB_YESNO) == IDYES)
		{
			EndGame();
		}
	}
	else if (MBtMsStart.PtIn(point))
	{
		DrawMap(17, 0);
		MBtMsStart.SetSrcPos(0, 0);
		MBtMsStart.Draw(View);
		midi.Play("Midi\\sky.mid");
	}
	else if (MBtMsPause.PtIn(point))
	{
		DrawMap(18, 0);
		MBtMsPause.SetSrcPos(0, 32);
		MBtMsPause.Draw(View);
		midi.Stop();
	}
	else if (MBtMsStop.PtIn(point))
	{
		DrawMap(19, 0);
		MBtMsStop.SetSrcPos(0, 64);
		MBtMsStop.Draw(View);
		midi.Stop();
	}

	InvalidateRect(Window->m_hWnd, MapRect, FALSE);
}

//************************************
// Method:    OnMouseMove
// FullName:  CMapManager::OnMouseMove
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point  �������꣨���أ�
// Description: ����ƶ���������ʾ��꣩
//************************************
void CMapManager::OnMouseMove(CPoint point)
{
	if (ChessBoardRect.PtInRect(point))
	{
		switch (status) 
		{
		case STATUS_END:
		case STATUS_NONE:
			SetCursor(PointToIndex(point));
			break;
			
		case STATUS_MOVE:
			break;
		}
	}
}

//************************************
// Method:    OnRButtonDown
// FullName:  CMapManager::OnRButtonDown
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point  �������꣨���أ�
// Description: ����Ҽ����²����������ƶ�·����
//************************************
void CMapManager::OnRButtonDown(CPoint point)
{
	// �����Ҽ�ʱ�����ص�һ�׶ζ���
	switch (status) 
	{
	  case STATUS_MOVE:
		ClearDist();
		SelChess->Draw(View);
		status = STATUS_NONE;
		InitMapStatus();
		break;
	}
}

//************************************
// Method:    Draw
// FullName:  CMapManager::Draw
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CDC &dc
// Description: �ػ����
//************************************
void CMapManager::Draw(CDC &dc)
{
	if (View.IsOK())
		View.Draw(dc, 0, 0, View.Width(), View.Height());
}

//************************************
// Method:    DrawMap
// FullName:  CMapManager::DrawMap
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �ػ�������ͼ
//************************************
void CMapManager::DrawMap()
{
	View.Copy(MapParts, 0, CSize(MAP_WIDTH, MAP_HEIGHT), 0);
}

//************************************
// Method:    DrawMap
// FullName:  CMapManager::DrawMap
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int x  �������꣨���ӣ�
// Parameter: int y  �������꣨���ӣ�
// Description: �ػ��ͼ��ָ��λ��
//************************************
void CMapManager::DrawMap(int x, int y)
{
	CPoint point(x * GRID_WIDTH, y * GRID_HEIGHT);
	View.Copy(MapParts, point, CSize(GRID_WIDTH, GRID_HEIGHT), point);
}

//************************************
// Method:    DrawMap
// FullName:  CMapManager::DrawMap
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CRect rect
// Description: �ػ�ָ������
//************************************
void CMapManager::DrawMap(CRect rect)
{
	int i, j;
	for (i=rect.left; i<=rect.right; i++)
	{
		for (j=rect.top; j<=rect.bottom; j++)
		{
			DrawMap(i, j);
		}
	}
}

//************************************
// Method:    DrawChess
// FullName:  CMapManager::DrawChess
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������������
//************************************
void CMapManager::DrawChess()
{
	int i;
	for (i=0; i<Player1.size(); i++)
	{
		if (Player1[i].IsExit())
		{
			Player1[i].Draw(View, ChessBoardRect);
		}
	}
	for (i=0; i<Player2.size(); i++)
	{
		if (Player2[i].IsExit())
		{
			Player2[i].Draw(View, ChessBoardRect);
		}
	}
}

//************************************
// Method:    DrawChess
// FullName:  CMapManager::DrawChess
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int x �������꣨���ӣ�
// Parameter: int y �������꣨���ӣ�
// Description: ����ָ��λ�õ����ӣ������λ�������ӣ�
//************************************
void CMapManager::DrawChess(int x, int y)
{
	if (Map[y][x].type & CHESS)
	{
		Map[y][x].sprite->Draw(View, ChessBoardRect);
	}
}

//************************************
// Method:    DrawButton
// FullName:  CMapManager::DrawButton
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ���Ƹ���ť
//************************************
void CMapManager::DrawButton()
{
	MBtStart.SetSrcPos(0, 0);
	MBtStart.Draw(View);
	if (lastAction.status == true)
	{
		MBtRedo.SetSrcPos(0, 64);
		MBtRedo.Draw(View);
	}
	else
	{
		MBtRedo.SetSrcPos(128, 64);
		MBtRedo.Draw(View);
	}
	MBtEnd.SetSrcPos(0, 32);
	MBtEnd.Draw(View);

	MBtMsStart.Draw(View);
	MBtMsPause.Draw(View);
	MBtMsStop.Draw(View);
}

//************************************
// Method:    DrawCursor
// FullName:  CMapManager::DrawCursor
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �������
//************************************
void CMapManager::DrawCursor()
{
	View.Mix(Cursor, CPoint(CursorPos.x * GRID_WIDTH, CursorPos.y * GRID_HEIGHT), 
		CSize(32, 32), CPoint(0, 32));
	InvalidateRect(Window->m_hWnd, &MapRect, FALSE);
}

//************************************
// Method:    SetCursor
// FullName:  CMapManager::SetCursor
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point  �������꣨���ӣ�
// Description: ��ָ��λ�û������
//************************************
void CMapManager::SetCursor(CPoint point)
{
	if (CursorPos != point) 
	{
		ClearCursor();
		CursorPos = point;
		DrawCursor();	
	}
}

//************************************
// Method:    ClearCursor
// FullName:  CMapManager::ClearCursor
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �������
//************************************
void CMapManager::ClearCursor()
{
	DrawMap(CursorPos.x, CursorPos.y);
	DrawChess(CursorPos.x - OFFSET_X, CursorPos.y - OFFSET_Y);
	CursorPos.x = CursorPos.y = 0;
}

//************************************
// Method:    FindDist
// FullName:  CMapManager::FindDist
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʾ�ƶ�·��
//************************************
void CMapManager::FindDist()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;
 	View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 32));

	switch(SelChess->m_uType)
	{
	case CHE:
		FindDistChe();
		break;
	case MA:
		FindDistMa();
		break;
	case XIANG:
		FindDistXiang();
	    break;
	case SHI:
		FindDistShi();
	    break;
	case JIANG:
		FindDistJiang();
		break;
	case PAO:
		FindDistPao();
		break;
	case BING:
		FindDistBing();
	    break;
	default:
	    break;
	}
	InvalidateRect(Window->m_hWnd, &MapRect, FALSE);
}

//************************************
// Method:    ClearDist
// FullName:  CMapManager::ClearDist
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �����ƶ�·��
//************************************
void CMapManager::ClearDist()
{
	switch(SelChess->m_uType)
	{
	case CHE:
		ClearDistChe();
		break;
	case MA:
		ClearDistMa();
		break;
	case XIANG:
		ClearDistXiang();
	    break;
	case SHI:
		ClearDistShi();
	    break;
	case JIANG:
		ClearDistJiang();
		break;
	case PAO:
		ClearDistPao();
		break;
	case BING:
		ClearDistBing();
		break;
	default:
	    break;
	}		
	InvalidateRect(Window->m_hWnd, &MapRect, FALSE);
}

//************************************
// Method:    FindDistChe
// FullName:  CMapManager::FindDistChe
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʾ���������ƶ�·��
//************************************
void CMapManager::FindDistChe()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;
	while(CheckDist(x, --y));

	y = SelChess->GetMapIndex().y;
	while(CheckDist(++x, y));

	x = SelChess->GetMapIndex().x;
	while(CheckDist(x, ++y));

	y = SelChess->GetMapIndex().y;
	while(CheckDist(--x, y));
}

//************************************
// Method:    ClearDistChe
// FullName:  CMapManager::ClearDistChe
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �������������ƶ�·��
//************************************
void CMapManager::ClearDistChe()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;
	DrawMap(x, y);
	while(ClearDist(x, --y));

	y = SelChess->GetMapIndex().y;
	while(ClearDist(++x, y));

	x = SelChess->GetMapIndex().x;
	while(ClearDist(x, ++y));

	y = SelChess->GetMapIndex().y;
	while(ClearDist(--x, y));
}

//************************************
// Method:    FindDistMa
// FullName:  CMapManager::FindDistMa
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʾ�������ƶ�·��
//************************************
void CMapManager::FindDistMa()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	int cx = x - OFFSET_X;
	int cy = y - OFFSET_Y;

	if(ChessBoardRect.PtInRect(IndexToPoint(x, y - 1)) &&
		((Map[cy - 1][cx].type & CHESS) == 0))
	{
		CheckDist(x - 1, y - 2);
		CheckDist(x + 1, y - 2);
	}

	if(ChessBoardRect.PtInRect(IndexToPoint(x + 1, y)) &&
		((Map[cy][cx + 1].type & CHESS) == 0))
	{
		CheckDist(x + 2, y - 1);
		CheckDist(x + 2, y + 1);
	}

	if(ChessBoardRect.PtInRect(IndexToPoint(x, y + 1)) &&
		((Map[cy + 1][cx].type & CHESS) == 0))
	{
		CheckDist(x + 1, y + 2);
		CheckDist(x - 1, y + 2);
	}

	if(ChessBoardRect.PtInRect(IndexToPoint(x - 1, y)) &&
		((Map[cy][cx - 1].type & CHESS) == 0))
	{
		CheckDist(x - 2, y + 1);
		CheckDist(x - 2, y - 1);
	}
}

//************************************
// Method:    ClearDistMa
// FullName:  CMapManager::ClearDistMa
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �����������ƶ�·��
//************************************
void CMapManager::ClearDistMa()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	DrawMap(x, y);

	ClearDist(x - 1, y - 2);
	ClearDist(x + 1, y - 2);
	ClearDist(x + 2, y - 1);
	ClearDist(x + 2, y + 1);
	ClearDist(x + 1, y + 2);
	ClearDist(x - 1, y + 2);
	ClearDist(x - 2, y + 1);
	ClearDist(x - 2, y - 1);
}

//************************************
// Method:    FindDistXiang
// FullName:  CMapManager::FindDistXiang
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʾ�����ࡱ���ƶ�·��
//************************************
void CMapManager::FindDistXiang()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	int cx = x - OFFSET_X;
	int cy = y - OFFSET_Y;

	CRect rect;
	if (SelChess->m_uColor == BLACK)
	{
		rect.SetRect(OFFSET_X * GRID_WIDTH, OFFSET_Y * GRID_HEIGHT, 
			(OFFSET_X + 9) * GRID_WIDTH, (OFFSET_Y + 5) * GRID_HEIGHT);
	}
	else
	{
		rect.SetRect(OFFSET_X * GRID_WIDTH, (OFFSET_Y + 5) * GRID_HEIGHT,
			(OFFSET_X + 9) * GRID_WIDTH, (OFFSET_Y + 10) * GRID_HEIGHT);
	}

	if(rect.PtInRect(IndexToPoint(x - 2, y - 2)) &&
		((Map[cy - 1][cx - 1].type & CHESS) == 0))
	{
		CheckDist(x - 2, y - 2);
	}

	if(rect.PtInRect(IndexToPoint(x + 2, y - 2)) &&
		((Map[cy - 1][cx + 1].type & CHESS) == 0))
	{
		CheckDist(x + 2, y - 2);
	}

	if(rect.PtInRect(IndexToPoint(x + 2, y + 2)) &&
		((Map[cy + 1][cx + 1].type & CHESS) == 0))
	{
		CheckDist(x + 2, y + 2);
	}

	if(rect.PtInRect(IndexToPoint(x - 2, y + 2)) &&
		((Map[cy + 1][cx - 1].type & CHESS) == 0))
	{
		CheckDist(x - 2, y + 2);
	}
}

//************************************
// Method:    ClearDistXiang
// FullName:  CMapManager::ClearDistXiang
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ���������ࡱ���ƶ�·��
//************************************
void CMapManager::ClearDistXiang()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	DrawMap(x, y);

 	ClearDist(x - 2, y - 2);
	ClearDist(x + 2, y - 2);
	ClearDist(x + 2, y + 2);
	ClearDist(x - 2, y + 2);
}

//************************************
// Method:    FindDistShi
// FullName:  CMapManager::FindDistShi
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʾ��ʿ���ˡ����ƶ�·��
//************************************
void CMapManager::FindDistShi()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	CRect rect;
	if (SelChess->m_uColor == BLACK)
	{
		rect.SetRect((OFFSET_X + 3) * GRID_WIDTH, OFFSET_Y * GRID_HEIGHT,
			(OFFSET_X + 6) * GRID_WIDTH, (OFFSET_Y + 3) * GRID_HEIGHT);
	}
	else
	{
		rect.SetRect((OFFSET_X + 3) * GRID_WIDTH, (OFFSET_Y + 7) * GRID_HEIGHT,
			(OFFSET_X + 6) * GRID_WIDTH, (OFFSET_Y + 10) * GRID_HEIGHT);
	}

	if(rect.PtInRect(IndexToPoint(x - 1, y - 1)))
	{
		CheckDist(x - 1, y - 1);
	}

	if(rect.PtInRect(IndexToPoint(x + 1, y - 1)))
	{
		CheckDist(x + 1, y - 1);
	}

	if(rect.PtInRect(IndexToPoint(x + 1, y + 1)))
	{
		CheckDist(x + 1, y + 1);
	}

	if(rect.PtInRect(IndexToPoint(x - 1, y + 1)) )
	{
		CheckDist(x - 1, y + 1);
	}
}

//************************************
// Method:    ClearDistShi
// FullName:  CMapManager::ClearDistShi
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������ʿ���ˡ����ƶ�·��
//************************************
void CMapManager::ClearDistShi()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	DrawMap(x, y);

	ClearDist(x - 1, y - 1);
	ClearDist(x + 1, y - 1);
	ClearDist(x + 1, y + 1);
	ClearDist(x - 1, y + 1);
}

//************************************
// Method:    FindDistJiang
// FullName:  CMapManager::FindDistJiang
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʾ������˧�����ƶ�·��
//************************************
void CMapManager::FindDistJiang()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	int cx = x - OFFSET_X;
	int cy = y - OFFSET_Y;

	CRect rect;
	if (SelChess->m_uColor == BLACK)
	{
		rect.SetRect((OFFSET_X + 3) * GRID_WIDTH, OFFSET_Y * GRID_HEIGHT,
			(OFFSET_X + 6) * GRID_WIDTH, (OFFSET_Y + 3) * GRID_HEIGHT);
	}
	else
	{
		rect.SetRect((OFFSET_X + 3) * GRID_WIDTH, (OFFSET_Y + 7) * GRID_HEIGHT,
			(OFFSET_X + 6) * GRID_WIDTH, (OFFSET_Y + 10) * GRID_HEIGHT);
	}

	if(rect.PtInRect(IndexToPoint(x, y - 1)))
	{
		CheckDist(x, y - 1);
	}

	if(rect.PtInRect(IndexToPoint(x + 1, y)))
	{
		CheckDist(x + 1, y);
	}

	if(rect.PtInRect(IndexToPoint(x, y + 1)))
	{
		CheckDist(x, y + 1);
	}

	if(rect.PtInRect(IndexToPoint(x - 1, y)))
	{
		CheckDist(x - 1, y);
	}

	int increase;
	if (SelChess->m_uColor == BLACK)
	{
		increase = 1;
	}
	else
	{
		increase = -1;
	}

	for (y += increase, cy += increase; 
		ChessBoardRect.PtInRect(IndexToPoint(x, y)); 
		y += increase, cy += increase)
	{
		if(Map[cy][cx].type & CHESS)
		{
			if (((CChess*)Map[cy][cx].sprite)->m_uType == JIANG)
			{
				Map[cy][cx].type |= MOVEDIST;
				View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 96));
			}
			break;
		}
	}
}

//************************************
// Method:    ClearDistJiang
// FullName:  CMapManager::ClearDistJiang
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ����������˧�����ƶ�·��
//************************************
void CMapManager::ClearDistJiang()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	DrawMap(x, y);

	ClearDist(x, y - 1);
	ClearDist(x + 1, y);
	ClearDist(x, y + 1);
	ClearDist(x - 1, y);
	
	if (SelChess->m_uColor == BLACK)
	{
		ClearDist(x, 10);
		ClearDist(x, 11);
		ClearDist(x, 12);
	}
	else
	{
		ClearDist(x, 3);
		ClearDist(x, 4);
		ClearDist(x, 5);
	}
}

//************************************
// Method:    FindDistPao
// FullName:  CMapManager::FindDistPao
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʾ���ڡ����ƶ�·��
//************************************
void CMapManager::FindDistPao()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;
	
	int cx = x - OFFSET_X;
	int cy = y - OFFSET_Y;
	while(ChessBoardRect.PtInRect(IndexToPoint(x, --y)) && 
		(Map[--cy][cx].type & CHESS) == 0)
	{
		Map[cy][cx].type = MOVEDIST;
		View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 64));
	}
	if (ChessBoardRect.PtInRect(IndexToPoint(x, y)) && 
		(Map[cy][cx].type & CHESS))
	{
		while (ChessBoardRect.PtInRect(IndexToPoint(x, --y)) && 
				(Map[--cy][cx].type & CHESS) == 0);
		if(ChessBoardRect.PtInRect(IndexToPoint(x, y)) &&
			(*((CChess*)Map[cy][cx].sprite) != *SelChess))
		{
			Map[cy][cx].type |= MOVEDIST;
			View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 96));
		}
	}

	y = SelChess->GetMapIndex().y;
	cy = y - OFFSET_Y;
	while(ChessBoardRect.PtInRect(IndexToPoint(++x, y)) && 
		(Map[cy][++cx].type & CHESS) == 0)
	{
		Map[cy][cx].type = MOVEDIST;
		View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 64));
	}
	if (ChessBoardRect.PtInRect(IndexToPoint(x, y)) && 
		(Map[cy][cx].type & CHESS))
	{
		while (ChessBoardRect.PtInRect(IndexToPoint(++x, y)) && 
				(Map[cy][++cx].type & CHESS) == 0);
		if(ChessBoardRect.PtInRect(IndexToPoint(x, y)) &&
			(*((CChess*)Map[cy][cx].sprite) != *SelChess))
		{
			Map[cy][cx].type |= MOVEDIST;
			View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 96));
		}
	}

	x = SelChess->GetMapIndex().x;	
	cx = x - OFFSET_X;
	while(ChessBoardRect.PtInRect(IndexToPoint(x, ++y)) && 
		(Map[++cy][cx].type & CHESS) == 0)
	{
		Map[cy][cx].type = MOVEDIST;
		View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 64));
	}
	if (ChessBoardRect.PtInRect(IndexToPoint(x, y)) && 
		(Map[cy][cx].type & CHESS))
	{
		while (ChessBoardRect.PtInRect(IndexToPoint(x, ++y)) && 
				(Map[++cy][cx].type & CHESS) == 0);
		if(ChessBoardRect.PtInRect(IndexToPoint(x, y)) &&
			(*((CChess*)Map[cy][cx].sprite) != *SelChess))
		{
			Map[cy][cx].type |= MOVEDIST;
			View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 96));
		}
	}

	y = SelChess->GetMapIndex().y;
	cy = y - OFFSET_Y;
	while(ChessBoardRect.PtInRect(IndexToPoint(--x, y)) && 
		(Map[cy][--cx].type & CHESS) == 0)
	{
		Map[cy][cx].type = MOVEDIST;
		View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 64));
	}
	if (ChessBoardRect.PtInRect(IndexToPoint(x, y)) && 
		(Map[cy][cx].type & CHESS))
	{
		while (ChessBoardRect.PtInRect(IndexToPoint(--x, y)) && 
				(Map[cy][--cx].type & CHESS) == 0);
		if(ChessBoardRect.PtInRect(IndexToPoint(x, y)) &&
			(*((CChess*)Map[cy][cx].sprite) != *SelChess))
		{
			Map[cy][cx].type |= MOVEDIST;
			View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 96));
		}
	}
}

//************************************
// Method:    ClearDistPao
// FullName:  CMapManager::ClearDistPao
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �������ڡ����ƶ�·��
//************************************
void CMapManager::ClearDistPao()
{	
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;
	DrawMap(x, y);
	
	while(ChessBoardRect.PtInRect(IndexToPoint(x, --y)))
	{
		ClearDist(x, y);
	}

	y = SelChess->GetMapIndex().y;
	while(ChessBoardRect.PtInRect(IndexToPoint(++x, y)))
	{
		ClearDist(x, y);
	}

	x = SelChess->GetMapIndex().x;
	while(ChessBoardRect.PtInRect(IndexToPoint(x, ++y)))
	{
		ClearDist(x, y);
	}

	y = SelChess->GetMapIndex().y;
	while(ChessBoardRect.PtInRect(IndexToPoint(--x, y)))
	{
		ClearDist(x, y);
	}
}

//************************************
// Method:    FindDistBing
// FullName:  CMapManager::FindDistBing
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʾ�������䡱���ƶ�·��
//************************************
void CMapManager::FindDistBing()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	CRect rect;
	if (SelChess->m_uColor == BLACK)
	{	
		CheckDist(x, y + 1);
 		rect.SetRect(OFFSET_X * GRID_WIDTH, (OFFSET_Y + 5) * GRID_HEIGHT,
			(OFFSET_X + 9) * GRID_WIDTH, (OFFSET_Y + 10) * GRID_HEIGHT);
		if (rect.PtInRect(IndexToPoint(x, y)))
 		{
			CheckDist(x + 1, y);
			CheckDist(x - 1, y);
		}
	}
	else
	{
		CheckDist(x, y - 1);
		rect.SetRect(OFFSET_X * GRID_WIDTH, OFFSET_Y * GRID_HEIGHT, 
			(OFFSET_X + 9) * GRID_WIDTH, (OFFSET_Y + 5) * GRID_HEIGHT);
		if (rect.PtInRect(IndexToPoint(x, y)))
 		{
			CheckDist(x + 1, y);
			CheckDist(x - 1 ,y);
		}
	}
}

//************************************
// Method:    ClearDistBing
// FullName:  CMapManager::ClearDistBing
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �����������䡱���ƶ�·��
//************************************
void CMapManager::ClearDistBing()
{
	int x = SelChess->GetMapIndex().x;
	int y = SelChess->GetMapIndex().y;

	DrawMap(x, y);

	ClearDist(x, y - 1);
	ClearDist(x + 1, y);
	ClearDist(x, y + 1);
	ClearDist(x - 1, y);
}

//************************************
// Method:    CheckDist
// FullName:  CMapManager::CheckDist
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: int x �������꣨���ӣ�
// Parameter: int y �������꣨���ӣ�
// Description: ���ָ��λ�ÿɷ��ƶ���������
//************************************
BOOL CMapManager::CheckDist(int x, int y)
{
	if(ChessBoardRect.PtInRect(IndexToPoint(x, y)))
	{
		if((Map[y - OFFSET_Y][x - OFFSET_X].type & CHESS) == 0)
		{
			Map[y - OFFSET_Y][x - OFFSET_X].type = MOVEDIST;
			View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 64));
			return TRUE;
		}
		else if ((*(CChess*)Map[y - OFFSET_Y][x - OFFSET_X].sprite) != *SelChess)
		{
			Map[y - OFFSET_Y][x - OFFSET_X].type |= MOVEDIST;
			View.Mix(Cursor, IndexToPoint(x, y), CSize(32, 32), CPoint(0, 96));
			return FALSE;
		}
	}
	return FALSE;
}

//************************************
// Method:    ClearDist
// FullName:  CMapManager::ClearDist
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: int x �������꣨���ӣ�
// Parameter: int y �������꣨���ӣ�
// Description: ����ָ��λ�õ��ƶ�·��
//************************************
BOOL CMapManager::ClearDist(int x, int y)
{
	if(ChessBoardRect.PtInRect(IndexToPoint(x, y)))
	{
		if((Map[y - OFFSET_Y][x - OFFSET_X].type & CHESS) == 0)
		{
			Map[y - OFFSET_Y][x - OFFSET_X].type = NONE;
			DrawMap(x, y);
			return TRUE;
		}
		else if ((*(CChess*)Map[y - OFFSET_Y][x - OFFSET_X].sprite) != *SelChess)
		{
			DrawMap(x, y);
			DrawChess(x - OFFSET_X, y - OFFSET_Y);
			return FALSE;
		}
	}
	return FALSE;
}
