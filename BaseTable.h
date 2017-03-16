// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_BASETABLE_409F38C900AB_INCLUDED
#define _INC_BASETABLE_409F38C900AB_INCLUDED

//Function:图表基础类
#pragma pack(1)

struct SSubItem
{
	enum SUBITEMLINE
	{//划线
		TOP_LINE = 0x1000,
		BOTTOM_LINE= 0x2000,
		LEFT_LINE = 0x4000,
		RIGHT_LINE= 0x8000
	};
	SSubItem()
	{
		dwStyle = 0;
		crTextColor = -1;
		crTextBkColor = -1;
		dwItemImage = -1;
		pFont = NULL;
		dwExStyle = -1;
	}
	CString szItemText;//单元格信息
	DWORD   dwItemImage;//单元格图像(图像列表序号，-1表示无图像)
	COLORREF crTextColor;//单元格文字色(-1表示为用系统默认颜色)
	COLORREF crTextBkColor;//单元格背景色(-1表示为系统默认颜色)
	DWORD   dwStyle;//属性
	DWORD	dwExStyle;//扩展属性
	CFont*	pFont;//字体配置
};
typedef CArray<SSubItem,SSubItem&> CLineInfo;
typedef CArray<CLineInfo,CLineInfo&> CItemArray;

struct SColumnHead
{
	CString  szColumnText;//头文字信息
	DWORD dwItemImage;//单元格图像
	DWORD dwFormat;//显示格式
	DWORD dwColumnWide;//宽度	
};
typedef CArray<SColumnHead,SColumnHead&> CColumnArray;
#pragma pack()

class CBaseTable 
{
public:	

	void SetHoriBeginLine(int nBegin);
	// 保证指定列能显示出来,返回新的起始位置
	int EnsureColVisible(int nColIndex);
	int GetColumnNum();
	int GetFixedColumn();//得到固定列数量
	int GetCurrentHorBeignLine();//得到当前水平开始行
	int GetColumnWidth(int nCol);//得到指定列宽度
	BOOL DeleteAll();//清空所有内容
	int GetItemHeight(){return (int)m_nRowHeight;};//得到一行高度
	int	GetCurrentSelLine(){return m_nCurrentSel;};//得到当前选择行
	int	GetCurrentBeginLine(){return m_nCurrentBeginLine;};//得到当前开始行
	virtual void SetCurrentSelLine(int nSel){m_nCurrentSel = nSel;};//设置当前选中行
	int GetDisplayableLineNum();//得到可显示的行数
	virtual BOOL OnKeyDownEvent(UINT nChar);//键盘响应	
	
	CBaseTable();
	virtual ~CBaseTable();

	//////////////////////////////////////////////////////////////////////////
	//函数功能：插入或更新单元格(Cell)数据
	//函数参数: nItem 行号
	//          nSubItem 列号
	//          subItem 单元格(Cell)数据
	BOOL InsertItem(int nItem,int nSubItem,SSubItem subItem);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：删除一行
	//函数参数：nItem 行号
	//函数返回：成功为TRUE,错误为FALSE
	virtual BOOL DeleteItem(int nItem);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：画出图形
	//函数参数：pDC 画图设备指针
	//函数返回：无
	//函数异常：无
	virtual void Draw(CDC* pDC);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：确保指定行可见
	//函数参数：nItem 要显示的行数量
	//函数返回：成功为TRUE,错误为FALSE
	//函数异常：无
	virtual BOOL EnsureRowVisible(int nItem);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：得到当前的表格行数
	//函数参数：无
	//函数返回: 表格行数
	//函数异常: 无
	virtual int GetItemCount();
	//////////////////////////////////////////////////////////////////////////
	//函数功能：得到单元格文字
	//函数参数:nItem 行号
	//         nSubItem 列号
	//         lpszText 返回的文字
	//函数返回:成功为TRUE,错误为FALSE
	virtual CString GetItemText(int nItem, int nSubItem);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：得到当前选中行行号
	//函数参数：无
	//函数返回：当前选中行号
	//函数异常：无
	virtual int GetSelItem(){return m_nCurrentSel;};
	//////////////////////////////////////////////////////////////////////////
	//函数功能:得到鼠标点击位置(行)
	//函数参数：pt 鼠标点击点
	//函数返回：成功则返回当前选中行号，否则返回 -1
	//函数异常：无
	virtual int HitTest(CPoint pt);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：用于初始化列和设置属性，所有基层类必须重载此函
	//数
	//函数参数：无
	//函数返回:成功为TRUE,错误为FALSE
	//函数异常:无
	virtual BOOL Init() = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数功能：插入一列，用于初始化列表
	//函数参数：nCol 当前要插入的位置
	//　　　　　lpszColumnHead　列表头的名称
	//　　　　　nFormat    列文字的显示位置
	//　　　　　nWidth     列的宽度
	//函数返回：成功返回当前列的位置，否则为-1
	virtual int InsertColumn(int nCol, LPCTSTR lpszColumnHead, int nFormat = LVCFMT_LEFT, int nWidth = -1);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：修改单元格文字
	//函数参数: nItem   单元行
	//　　　　　nSubItem 单元列
	//　　　　　lpszText 单元文字
	//          nImage   单元格显示图像
	//          crText   单元文字颜色
	//函数返回：成功为TRUE,错误为FALSE
	//函数异常：无
	virtual BOOL InsertItem(int nItem, int nSubItem, LPCTSTR lpszText, int nImage = -1,
		                    COLORREF crText = -1,COLORREF crTextBkColor = -1,DWORD dwStyle = 0);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置表格背景色
	//函数参数: cr 设置的背景色
	//函数返回:无
	//函数异常:无
	virtual void SetBkColor(COLORREF cr){m_crBkColor = cr;};
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置画图区间
	//函数参数：rRect 画图区间
	//函数返回:无
	//函数异常:无
	virtual void MoveTo(CRect rRect);	

	void GetRect(CRect& rrectTable);
	
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置显示字体
	//函数参数：pFont 新字体
	//函数返回：旧字体(当前字体)
	virtual CFont* SetFont(CFont* pFont);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置表格显示的图像列表
	//函数参数: 设置的图像列表的指针
	//函数返回：前一个图像列表的指针
	//函数异常：GetLastError得到最后的错误
	virtual CImageList* SetImageList(CImageList* pImageList);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置每单元格的高度(行高)
	//函数参数：nHeight 单元格高度(行高)
	//函数返回:无
	//函数异常:无
	virtual void SetItemHeight(int nHeight){m_nRowHeight = (float)nHeight;};
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置显示风格
	//函数参数：显示风格
	//函数返回:无
	//函数异常：无
	virtual void SetStyle(DWORD dwStyle){m_dwStyle |= dwStyle;};
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置显示扩展风格
	//函数参数：显示风格
	//函数返回:无
	//函数异常：无
	virtual void SetExtendStyle(DWORD dwExStyle){m_dwExStyle |= dwExStyle;};
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置通用文字色
	//函数参数：cr 文字颜色
	//函数返回：无
	virtual void SetTextColor(COLORREF cr){m_crTextColor = cr;};
	//////////////////////////////////////////////////////////////////////////
	//函数功能：对列排序
	//函数参数：pData 当前排序的两列的数据
	//　　　　　dwDataNum数据长度
	//函数返回:0为失败，其他为成功
	virtual int SortItem(LPVOID pData, DWORD dwDataNum);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：传递数据，所有继承类必须重载此接口来设置数据
	//函数参数：lpData 多态数据流
	//函数返回:成功为TRUE,错误为FALSE
	//函数异常:无
	virtual BOOL UpdateData(LPVOID lpData,int nDataLen) = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置网格线参数
	//函数参数：crLine 线颜色
	//          nWidth 线宽度
	virtual void SetLineInfo(COLORREF crLine,int nWidth = 1){m_crLineColor = crLine;m_nLineWidth = nWidth;};
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置固定显示列数量
	//函数参数: nFixedLine 固定的列数
	void SetFixedColumn(int nFixedLine)
	{if((nFixedLine>=0)&&(nFixedLine<m_ArrColumn.GetSize()))
		m_nFixedColumn = nFixedLine;};
	//////////////////////////////////////////////////////////////////////////
	//函数功能：得到表格总宽度
	int GetTotalWidth();
	//////////////////////////////////////////////////////////////////////////
	//函数功能：得到表格当前宽度
	int GetCurWidth();
	//函数功能：弹出右键菜单
	virtual UINT PopupRBtnMenu(CWnd* pWnd,CPoint point,UINT uiPopMenuID,int iSel);
	//设置颜色和字体
	virtual void SetColorsAndFonts();

protected:

	void SetSortParam(int nSortColIndex,BOOL bDescOrder);	

	// 获取最后一页首行的位置索引 
	int GetLastPageBeginLine( int nTotalRowCount);
	
	int GetCurPageRowCount(int nCurBeginIndex,int nTotalRowCount);

	void SetDrawColumnLine(BOOL bDraw);

	void SetSortDirect(int nSortColumn,BOOL bDirect);

	void SetSeledRowFillSolidRect(BOOL bFill,COLORREF crFillColor = -1);

protected:
	COLORREF m_crBkColor;//系统背景色

	COLORREF m_crSelectColor;//选择色
	
	COLORREF m_crTextColor;//文字色

	CItemArray m_ArrItemInfo;//数据信息
	
	CColumnArray  m_ArrColumn;//Column信息

	COLORREF m_crTextBkColor;//文字背景色

	CFont* m_pFont; //文字字体

	DWORD m_dwStyle; //显示风格 完全兼容CListCtrl属性

	DWORD m_dwExStyle;//扩展显示属性 完全兼容CListCtrl扩展属性

	CRect m_DrawRect;	//画图区域

	int m_nCurrentSel;//当前选中行

	float m_nRowHeight; //单元格高度(行高)

	int m_nCurrentBeginLine;	//当前开始行

	int m_nCurrentShowLines;	//当前屏幕可显示行

	CImageList* m_pImageList;	//单元格使用图像列表

	COLORREF m_crLineColor;	//网格线颜色

	int      m_nLineWidth;	//网格线宽度

	CDWordArray	m_ArrdwItemOrder;//单元位置	
	
	BOOL m_bDescSort;    //是否从大到小排序方向
	int m_nSortColIndex; //当前排序列
protected:
	int		m_nFixedColumn;//左侧固定列
	int		m_nHoriBeiginLine;//水平开始位置
private:
	BOOL m_bActivate;	//是否为当前激活状态
	BOOL m_bDrawColumnLine;//画出列抬头线
	BOOL m_bSeledRowFillSolidRect; // 选中行是否使用实心矩形来高亮显示
};

#endif /* _INC_BASETABLE_409F38C900AB_INCLUDED */
