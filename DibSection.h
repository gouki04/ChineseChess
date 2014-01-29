//
//	DIB Section
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef	__DibSection_h__
#define	__DibSection_h__

//
//	CDIBSection class
//
class CDibSection 
{
  public:
	/* 构造析构函数 */
	CDibSection();
	CDibSection(const CDibSection &dib);
	~CDibSection();

	/* 初始化及销毁函数 */
	BOOL Create(int width, int height, int depth);
	void Destroy();

	/* 载入bmp及保存bmp */
	BOOL LoadBmp(const char *path);
	BOOL SaveBmp(const char *path);

	/* 复制及合成函数 */
	void Copy(const CDibSection &dib);
	void Copy(const CDibSection &dib, CPoint to, CSize size, CPoint from);
	void Mix(const CDibSection &dib, CPoint to, CSize size, CPoint from,
		COLORREF transparent = RGB(0, 255, 0));

	/* 绘制函数 */
	void Draw(HDC dc, int x, int y, int w, int h, int ox=0, int oy=0);
	void Draw(HDC dc, const CRect &rect, CPoint point);
	void Draw(HDC dc, const CRect &rect);
	void FillRect(const CRect &rect, COLORREF color);

	/* 取像素函数 */
	const void *GetBits() const { return Bits; }
	void *GetBits() { return Bits; }
	const void *GetBits(int x, int y) const;
	void *GetBits(int x, int y);

	/* 取数据成员函数 */
	BOOL    IsOK()          const { return Bits != 0; }
	int     Width()         const { return Header.Info.biWidth; }
	int     Height()        const { return Header.Info.biHeight; }
	int     Depth()         const { return Header.Info.biBitCount; }
	int     BytesPerLine()  const { return bytes_per_line; }
	int     BytesPerPixel() const { return bytes_per_pixel; }
	HBITMAP Handle()        const { return hBitmap; }

	/* 重载=号操作符 */
	CDibSection &operator=(const CDibSection &dib);

	/* 取图片基本数据函数 */
	static int ScanBytes(int width, int depth);
	static int PixelBytes(int depth);

  protected:

	struct	{
		BITMAPINFOHEADER	Info;
		DWORD				BitField[3];
	} Header;

	HBITMAP	hBitmap;           //bmp文件句柄
	void    *Bits;             //bmp数据区指针
	int     bytes_per_line;    //一行的字节数
	int     bytes_per_pixel;   //一像素的字节数
} ;

/************************************************************************/
/* CDibDC 类                                                            */
class CDibDC: public CDC {
  public:
	/* 构造/析构函数 */
	CDibDC(const CDibSection &dib);
	CDibDC(const CDibSection &dib, HDC hDC);
	~CDibDC();

  protected:
	HDC     hScreenDC;    //设备DC句柄
	HGDIOBJ hOldBitmap;   //旧bmp的句柄
} ;
/************************************************************************/


// ************************************************************************
//  各内联函数如下：
//
//  CDibSection类：
//
//  PixelBytes()        - 得到一像素所占的字节数
//  ScanBytes()         - 得到一行所占的字节数
//  
//  GetBits()           - 得到指定像素点的指针的函数
//  GetBits()           - 
//
//  Draw()              - 重载的3个绘制函数
//  Draw()		        - 
//  Draw()		        - 
//
// ************************************************************************


/*************************************************************************
 *
 * 函数名称：
 *   CDibDC()
 *
 * 参数:
 *   const CDibSection &dib     - 被复制的CDibSection对象
 *
 * 返回值:
 *   void
 *
 * 说明:
 *   拷贝构造函数，用一个CDibSection对象初始化一个CDibDC对象
 *
 ************************************************************************/

inline CDibDC::CDibDC(const CDibSection &dib)
{
	hScreenDC = ::GetDC(0);
	HDC	memDC = ::CreateCompatibleDC(hScreenDC);
	hOldBitmap = ::SelectObject(memDC, dib.Handle());
	Attach(memDC);
}


/*************************************************************************
 *
 * 函数名称：
 *   CDibDC()
 *
 * 参数:
 *   const CDibSection &dib     - 被复制的CDibSection对象
 *   HDC hDC                    - 输出设备DC句柄
 *
 * 返回值:
 *   void
 *
 * 说明:
 *   拷贝构造函数
 *
 ************************************************************************/

inline CDibDC::CDibDC(const CDibSection &dib, HDC hDC)
{
	hScreenDC = 0;
	HDC	memDC = ::CreateCompatibleDC(hDC);
	hOldBitmap = ::SelectObject(memDC, dib.Handle());
	Attach(memDC);
}

/*************************************************************************
 *
 * 函数名称：
 *   ~CDibDC()
 *
 * 参数:
 *
 * 返回值:
 *   void
 *
 * 说明:
 *   析构函数
 *
 ************************************************************************/

inline CDibDC::~CDibDC()
{
	::GdiFlush();
	HDC memDC = Detach();
	::SelectObject(memDC, hOldBitmap);
	::DeleteDC(memDC);
	if (hScreenDC)
		::ReleaseDC(0, hScreenDC);
}


/*************************************************************************
 *
 * 函数名称：
 *   PixelBytes()
 *
 * 参数:
 *   int depth             - 图片深度（biBitCount每像素所占位数）
 *
 * 返回值:
 *   int                   - 返回一像素所占的字节数
 *
 * 说明:
 *   因为1字节包含8位，而小于1字节当1字节算，所以将（depth+7）/8即得结果
 *
 ************************************************************************/

inline int CDibSection::PixelBytes(int depth)
{
	return (depth + 7) / 8;
}


/*************************************************************************
 *
 * 函数名称：
 *   ScanBytes()
 *
 * 参数:
 *   int width             - 图片的宽度（以像素为单位）
 *   int depth             - 图片深度（biBitCount每像素所占位数）
 *
 * 返回值:
 *   int                   - 返回一行所占的字节数
 *
 * 说明:
 *   window规定一个扫描行所占的字节数必须为4的整数倍数，不足4的倍数则要对
 *   进行扩充。根据以下公式可以算得一行实际所占的字节数
 *
 ************************************************************************/

inline int CDibSection::ScanBytes(int width, int depth)
{
	return ((width * PixelBytes(depth) + 3) / 4) * 4;
}


/*************************************************************************
 *
 * 函数名称：
 *   GetBits() const
 *
 * 参数:
 *   int x                 - 像素点的x坐标
 *   int y                 - 像素点的y坐标
 *
 * 返回值:
 *   const void*           - 指向该像素点的指针
 *
 * 说明:
 *   因为BMP文件的坐标原点在左下角，而习惯上的坐标在左上角，所以取像素时必须
 *   将纵坐标改一下，及改为height - y
 *
 ************************************************************************/

inline const void *CDibSection::GetBits(int x, int y) const
{
	return (const void *)((const char *)GetBits()
		+ (Height() - y - 1) * bytes_per_line + x * bytes_per_pixel);
}


/*************************************************************************
 *
 * 函数名称：
 *   GetBits()
 *
 * 参数:
 *   int x                 - 像素点的x坐标
 *   int y                 - 像素点的y坐标
 *
 * 返回值:
 *   void*           - 指向该像素点的指针
 *
 * 说明:
 *   因为BMP文件的坐标原点在左下角，而习惯上的坐标在左上角，所以取像素时必须
 *   将纵坐标改一下，及改为height - y
 *
 ************************************************************************/

inline void *CDibSection::GetBits(int x, int y)
{
	return (void *)((char *)GetBits()
		+ (Height() - y - 1) * bytes_per_line + x * bytes_per_pixel);
}


/*************************************************************************
 *
 * 函数名称：
 *   Draw()
 *
 * 参数:
 *   HDC dc                - 输出设备DC的句柄
 *   int x                 - 输出位置的左上角的x坐标（对应于目标DC）
 *   int y                 - 输出位置的左上角的y坐标（对应于目标DC）
 *   int w                 - 绘制的区域大小的宽度
 *   int h                 - 绘制的区域大小的高度
 *   int ox                - 输出位置的左上角的x坐标（对应于源DC）
 *   int oy                - 输出位置的左上角的y坐标（对应于源DC）
 *
 * 返回值:
 *   void         
 *
 * 说明:
 *   直接使用CDC::BitBlt来绘制
 *
 ************************************************************************/

inline void CDibSection::Draw(HDC dc, int x, int y, int w, int h, int ox, int oy)
{
	BitBlt(dc, x, y, w, h, CDibDC(*this, dc), ox, oy, SRCCOPY);
}


/*************************************************************************
 *
 * 函数名称：
 *   Draw()
 *
 * 参数:
 *   HDC dc                - 输出设备DC的句柄
 *   const CRect& rect     - 输出的矩形区域（坐标对应于目标DC）
 *   CPoint point          - 输出位置的左上角坐标（对应于目标DC）
 *
 * 返回值:
 *   void         
 *
 * 说明:
 *   直接使用CDC::BitBlt来绘制
 *
 ************************************************************************/

inline void CDibSection::Draw(HDC dc, const CRect &rect, CPoint point)
{
	Draw(dc, rect.left, rect.top, rect.Width(), rect.Height(), point.x, point.y);
}


/*************************************************************************
 *
 * 函数名称：
 *   Draw()
 *
 * 参数:
 *   HDC dc                - 输出设备DC的句柄
 *   const CRect& rect     - 输出的矩形区域
 *
 * 返回值:
 *   void         
 *
 * 说明:
 *   参数中的rect的左上角坐标值对应于源DC以及目标DC，也就是说，只有当
 *   输出位置的左上角在源DC以及目标DC中是相同时才可以用此函数
 *
 ************************************************************************/

inline void CDibSection::Draw(HDC dc, const CRect &rect)
{
	Draw(dc, rect.left, rect.top, rect.Width(), rect.Height(), rect.left, rect.top);
}

#endif
