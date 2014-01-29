//
//	DIB Section
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#include "StdAfx.h"
#include "DibSection.h"


// ************************************************************************
//
//  CDibSection 函数库：
//
//  Create()            - 创建位图
//  Destroy()           - 销毁位图
//  LoadBmp()           - 载入bmp文件并创建位图
//  SaveBmp()           - 保存Bmp文件
//  Copy()              - 复制指定区域到指定位置
//  Mix()               - 合成指定区域
//  FillRect()          - 填充指定区域
//
//  operator=
// ************************************************************************

CDibSection::CDibSection()
	: hBitmap(0), Bits(0)
{
}

CDibSection::CDibSection(const CDibSection &dib)
	: hBitmap(0), Bits(0)
{
	if (Create(dib.Width(), dib.Height(), dib.Depth()))
	{
		Copy(dib);
	}
}

CDibSection::~CDibSection()
{
	Destroy();
}

//************************************
// Method:    Create
// FullName:  CDibSection::Create
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: int width
// Parameter: int height
// Parameter: int depth
//************************************
BOOL CDibSection::Create(int width, int height, int depth)
{
	/* 先将原来数据销毁，防止内存泄露 */
	Destroy();

	/* 初始化位图的信息 */
	bytes_per_line = ScanBytes(width, depth);
	bytes_per_pixel = PixelBytes(depth);

	Header.Info.biSize			= sizeof(BITMAPINFOHEADER);
	Header.Info.biWidth			= width;
	Header.Info.biHeight		= height;
	Header.Info.biBitCount		= depth;
	Header.Info.biPlanes		= 1;
	Header.Info.biXPelsPerMeter	= 0;
	Header.Info.biYPelsPerMeter	= 0;
	Header.Info.biClrUsed		= 0;
	Header.Info.biClrImportant	= 0;
	Header.Info.biCompression	= depth == 24? BI_RGB: BI_BITFIELDS;
	Header.Info.biSizeImage		= bytes_per_line * height;

	/* 设定颜色遮罩 */
	switch (depth) {
	  case 16:
		Header.BitField[0] = 0x7c00;
		Header.BitField[1] = 0x03e0;
		Header.BitField[2] = 0x001f;
		break;

	  case 32:
		Header.BitField[0] = 0xff0000;
		Header.BitField[1] = 0x00ff00;
		Header.BitField[2] = 0x0000ff;
		break;

	  default:
		Header.BitField[0] = 0;
		Header.BitField[1] = 0;
		Header.BitField[2] = 0;
		break;
	}

	/* 创建位图 */
	HDC dc = ::GetDC(0);
	hBitmap = CreateDIBSection(dc, (BITMAPINFO *)&Header, DIB_RGB_COLORS, &Bits, NULL, 0);
	::ReleaseDC(0, dc);

	return hBitmap != 0;
}

//************************************
// Method:    Destroy
// FullName:  CDibSection::Destroy
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CDibSection::Destroy()
{
	if (hBitmap) {
		GdiFlush();
		::DeleteObject(hBitmap);
		hBitmap = 0;
	}
}

//************************************
// Method:    LoadBmp
// FullName:  CDibSection::LoadBmp
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: const char *path
//************************************
BOOL CDibSection::LoadBmp(const char *path)
{
	Destroy();

	hBitmap = (HBITMAP)::LoadImage(::GetModuleHandle(0), path, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (!hBitmap)
	{
		return FALSE;
	}

	DIBSECTION	dib;
	if (::GetObject(hBitmap, sizeof(DIBSECTION), &dib) != sizeof(DIBSECTION)) 
	{
		::DeleteObject(hBitmap);
		hBitmap = 0;
		return FALSE;
	}

	Header.Info = dib.dsBmih;
	for (int i=0; i<3; i++)
		Header.BitField[i] = dib.dsBitfields[i];

	bytes_per_pixel = PixelBytes(dib.dsBmih.biBitCount);
	bytes_per_line = ScanBytes(dib.dsBmih.biWidth, dib.dsBmih.biBitCount);
	Bits = dib.dsBm.bmBits;

	return TRUE;
}

//************************************
// Method:    SaveBmp
// FullName:  CDibSection::SaveBmp
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: const char *path
//************************************
BOOL CDibSection::SaveBmp(const char *path)
{
	TRY {
		CFile file(path, CFile::modeCreate | CFile::modeWrite);

		int length = bytes_per_line * Height();
		BITMAPFILEHEADER	header;
		memset(&header, 0, sizeof(header));
		header.bfType = (WORD)('M' << 8) | 'B';
		header.bfSize = sizeof(header) + Header.Info.biSize + length;
		header.bfOffBits = sizeof(header) + Header.Info.biSize;

		file.Write(&header, sizeof(header));
		file.Write(&Header.Info, Header.Info.biSize);
		if (Header.Info.biCompression == BI_BITFIELDS)
			file.Write(Header.BitField, sizeof(Header.BitField));
		file.Write(Bits, length);
		file.Close();
	}
	CATCH(CFileException, e) {
		return FALSE;
	}
	END_CATCH

	return TRUE;
}


//************************************
// Method:    Copy
// FullName:  CDibSection::Copy
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const CDibSection &dib
//************************************
void CDibSection::Copy(const CDibSection &dib)
{
	BitBlt(CDibDC(*this), 0, 0, dib.Width(), dib.Height(), CDibDC(dib), 0, 0, SRCCOPY);
}

//************************************
// Method:    Copy
// FullName:  CDibSection::Copy
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const CDibSection &dib
// Parameter: CPoint to
// Parameter: CSize size
// Parameter: CPoint from
//************************************
void CDibSection::Copy(const CDibSection &dib, CPoint to, CSize size, CPoint from)
{
	BitBlt(CDibDC(*this), to.x, to.y, size.cx, size.cy, CDibDC(dib), from.x, from.y, SRCCOPY);
}

//************************************
// Method:    Mix
// FullName:  CDibSection::Mix
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const CDibSection &dib
// Parameter: CPoint to
// Parameter: CSize size
// Parameter: CPoint from
// Parameter: COLORREF tc
//************************************
void CDibSection::Mix(const CDibSection &dib, CPoint to, CSize size, CPoint from, COLORREF tc)
{
	if (Depth() == dib.Depth()) 
	{
		switch (Depth()) 
		{
		  case 16:
			{
				BYTE r = GetRValue(tc);
				BYTE g = GetGValue(tc);
				BYTE b = GetBValue(tc);
				WORD color = (r << 7) & 0x7c00 | (g << 2) & 0x03e0 | (b >> 3) & 0x001f;

				for (int y = 0; y < size.cy; y++) 
				{
					WORD *p = (WORD *)GetBits(to.x, to.y + y);
					const WORD *q = (const WORD *)dib.GetBits(from.x, from.y + y);
					for (int x = 0; x < size.cx; x++) 
					{
						if (*q != color)
						{
							*p = *q;
						}
						p++;
						q++;
					}
				}
			}
			break;

		  case 24:
			{
				BYTE r = GetRValue(tc);
				BYTE g = GetGValue(tc);
				BYTE b = GetBValue(tc);

				for (int y = 0; y < size.cy; y++) 
				{
					BYTE *p = (BYTE *)GetBits(to.x, to.y + y);
					const BYTE *q = (const BYTE *)dib.GetBits(from.x, from.y + y);
					for (int x = 0; x < size.cx; x++) 
					{
						if (q[0] != b || q[1] != g || q[2] != r) 
						{
							p[0] = q[0];
							p[1] = q[1];
							p[2] = q[2];
						}
						p += 3;
						q += 3;
					}
				}
			}
			break;

		  case 32:
			{
				for (int y = 0; y < size.cy; y++) 
				{
					DWORD *p = (DWORD *)GetBits(to.x, to.y + y);
					const DWORD *q = (const DWORD *)dib.GetBits(from.x, from.y + y);
					for (int x = 0; x < size.cx; x++) 
					{
						if ((*q & 0xffffff) != tc)
						{
							*p = *q;
						}
						p++;
						q++;
					}
				}
			}
			break;
		}// end of switch 
	}// end of if
	else {
		CDibDC dst(*this);
		CDibDC src(dib);

		for (int y = 0; y < size.cy; y++) 
		{
			for (int x = 0; x < size.cx; x++) 
			{
				COLORREF c = src.GetPixel(from.x + x, from.y + y);
				if (c != tc)
				{
					dst.SetPixelV(to.x + x, to.y + y, c);
				}
			}
		}
	}// end of else
}

//************************************
// Method:    FillRect
// FullName:  CDibSection::FillRect
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const CRect &rect
// Parameter: COLORREF color
//************************************
void CDibSection::FillRect(const CRect &rect, COLORREF color)
{
	CDibDC dc(*this);
	dc.SetBkColor(color);
	dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, 0, 0, 0);
}

//************************************
// Method:    operator=
// FullName:  CDibSection::operator=
// Access:    public 
// Returns:   CDibSection &
// Qualifier:
// Parameter: const CDibSection &dib
//************************************
CDibSection &CDibSection::operator=(const CDibSection &dib)
{
	if (this != &dib) {
		if (Create(dib.Width(), dib.Height(), dib.Depth()))
			Copy(dib);
	}
	return *this;
}