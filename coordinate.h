#ifndef	__coordinate_h__
#define	__coordinate_h__

// 矩形地图的坐标转换

/* 地图每一格的宽度与高度 */
enum	{
	MAP_WIDTH      = 640,
	MAP_HEIGHT     = 480,	
	GRID_WIDTH     = 32,
	GRID_HEIGHT    = 32,
	OFFSET_X       = 2,
	OFFSET_Y       = 3,
} ;

//************************************
// Method:    PointToIndex
// FullName:  PointToIndex
// Access:    public 
// Returns:   CPoint
// Qualifier:
// Parameter: int x
// Parameter: int y
//************************************
inline CPoint PointToIndex(int x, int y)
{
	return CPoint(x / GRID_WIDTH, y / GRID_WIDTH);
}

//************************************
// Method:    PointToIndex
// FullName:  PointToIndex
// Access:    public 
// Returns:   CPoint
// Qualifier:
// Parameter: CPoint point
//************************************
inline CPoint PointToIndex(CPoint point)
{
	return PointToIndex(point.x, point.y);
}

//************************************
// Method:    IndexToPoint
// FullName:  IndexToPoint
// Access:    public 
// Returns:   CPoint
// Qualifier:
// Parameter: int x
// Parameter: int y
//************************************
inline CPoint IndexToPoint(int x, int y)
{
	return CPoint(x * GRID_WIDTH, y * GRID_HEIGHT);
}

//************************************
// Method:    IndexToPoint
// FullName:  IndexToPoint
// Access:    public 
// Returns:   CPoint
// Qualifier:
// Parameter: CPoint point
//************************************
inline CPoint IndexToPoint(CPoint point)
{
	return IndexToPoint(point.x, point.y);
}

#endif
