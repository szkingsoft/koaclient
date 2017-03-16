// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "BaseTable.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBaseTable::CBaseTable()
{
	//m_crBkColor = GetSysColor(COLOR_WINDOW);
	//m_crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	//m_crTextBkColor = GetSysColor(COLOR_WINDOW);
	//m_crSelectColor = GetSysColor(COLOR_HIGHLIGHT);
	m_nCurrentSel = 0;//当前选中
	m_nCurrentBeginLine = 0;//开始行数
	m_nRowHeight = 18;//默认高度
	m_nSortColIndex = -1;
	m_bDescSort = TRUE; 
	m_pFont = NULL;
	m_pImageList = NULL;
	m_dwStyle = 0;
	m_dwExStyle = 0;
	m_bActivate = TRUE;
	//m_crLineColor = GetSysColor(COLOR_WINDOWTEXT);
	m_nLineWidth = 1;

	m_nFixedColumn = 0;
	m_nHoriBeiginLine = 0;
	m_ArrdwItemOrder.SetSize(0,20);

	m_bDrawColumnLine = TRUE;

	m_bSeledRowFillSolidRect = TRUE;

	//首先设置颜色和字体

	ZeroMemory(&m_DrawRect,sizeof(CRect));
}

CBaseTable::~CBaseTable()
{

}
//函数功能：删除单元
//函数参数：nItem 单元格编号
//函数返回：成功为TRUE,错误为FALSE
BOOL CBaseTable::DeleteItem(int nItem)
{
	if(nItem >= m_ArrItemInfo.GetSize())
		return FALSE;//无此行
	m_ArrItemInfo.RemoveAt(nItem);
	return TRUE;
}

//函数功能：画出图形
//函数参数：pDC 画图设备指针
//函数返回：无
//函数异常：无
void CBaseTable::Draw(CDC* pDC)
{
	SetColorsAndFonts();
    if( m_nCurrentBeginLine < 0)
		m_nCurrentBeginLine = 0;

	//////////////////////////////////////////////////////////////////////////
	//背景
	//填充背景区域
	pDC->FillSolidRect(m_DrawRect,m_crBkColor);//填充背景色
	//////////////////////////////////////////////////////////////////////////
	CFont* oldFont = NULL;
	if(m_pFont)
		oldFont = pDC->SelectObject(m_pFont);
	int iOldDrawMode;
	iOldDrawMode = pDC->SetBkMode(TRANSPARENT);

	CPen DvidePen(PS_SOLID,m_nLineWidth,m_crLineColor);
	CPen * oldPen = pDC->SelectObject(&DvidePen);
	//////////////////////////////////////////////////////////////////////////
	int nCount,nSubCount;//计数器
	CRect ItemRect;//单元格区间
	
	int nBeginHeight;
	nBeginHeight = 0;//开始高度
	int nLeftSize;//左侧尺寸
	pDC->SetTextColor(m_crTextColor);//设置文字颜色
	int nDrawFormat;
	int iStation;//当前单元格
	if(!(m_dwStyle&LVS_NOCOLUMNHEADER))
	{//画头信息	
		ItemRect.top = m_DrawRect.top;
		ItemRect.bottom = m_DrawRect.top+ m_nRowHeight;//头的位置
		nBeginHeight = m_nRowHeight;//高度
		nLeftSize = 0;

		for(nCount = 0;nCount<m_ArrColumn.GetSize();nCount++)
		{
			if(nCount >= m_nFixedColumn)
				iStation = nCount+m_nHoriBeiginLine;//加上右侧增量
			else
				iStation = nCount;
			if(iStation >= m_ArrColumn.GetSize())break;
			iStation = m_ArrdwItemOrder[iStation];//变换对应位置
			SColumnHead& column = m_ArrColumn[iStation];
			ItemRect.left = m_DrawRect.left+nLeftSize;
			ItemRect.right = ItemRect.left+ column.dwColumnWide;
			ItemRect.left +=2;
			ItemRect.right -= 2;
			if(ItemRect.left >m_DrawRect.right)break;//超长
			nDrawFormat = DT_SINGLELINE|DT_VCENTER;
			if(column.dwFormat == LVCFMT_LEFT)
				nDrawFormat |= DT_LEFT;
			else if(column.dwFormat == LVCFMT_CENTER)
				nDrawFormat |= DT_CENTER;
			else
				nDrawFormat |= DT_RIGHT;
			pDC->DrawText(column.szColumnText,ItemRect,nDrawFormat);
			if(m_pImageList&&(m_nSortColIndex == iStation))
			{//有图像并且列数相等
				CPoint BasePoint(ItemRect.right-4,ItemRect.top+5);
				m_pImageList->Draw(pDC,m_bDescSort,BasePoint,ILD_NORMAL);
			}
			if(m_bDrawColumnLine)
			{
				ItemRect.left -=2;
				ItemRect.right += 2;
				pDC->MoveTo(ItemRect.right,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.bottom-1);
				pDC->LineTo(ItemRect.left,ItemRect.bottom-1);
				pDC->LineTo(ItemRect.left,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.top);
			}
			nLeftSize  += column.dwColumnWide;//宽度
		}
	}
	CPoint ImagePoint;
	CFont* pOldSubFont;//子单元内部字体
	for(nCount = 0;nCount<(m_ArrItemInfo.GetSize()-m_nCurrentBeginLine);nCount++)
	{//画内部单元格
		ItemRect.top = m_DrawRect.top + nBeginHeight+(nCount/2)*m_nRowHeight;//高度
		ItemRect.bottom = ItemRect.top+ m_nRowHeight;//高度　
		if(ItemRect.bottom> m_DrawRect.bottom)
			break;//超过当前可显示行数
		nLeftSize = 0;	
		if((m_dwExStyle&LVS_EX_FULLROWSELECT)&&(nCount == m_nCurrentSel))
		{//当前选择
			ItemRect.left = m_DrawRect.left;
			ItemRect.right = m_DrawRect.right;	
			ItemRect.top ++;
	
			if( !m_bSeledRowFillSolidRect)
			{
				// 对当前选中行用外框显示
				CPen pen(PS_SOLID,1,m_crSelectColor);
				CPen* pOldPen = pDC->SelectObject(&pen);
				pDC->MoveTo(ItemRect.left,ItemRect.bottom);
				pDC->LineTo(ItemRect.right,ItemRect.bottom);
				
				pDC->MoveTo(ItemRect.left,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.top);
				
				pDC->SelectObject(pOldPen);
			}
			else
			{
				pDC->FillSolidRect(ItemRect,m_crSelectColor);
			}

		}
		CLineInfo & LineInfo = m_ArrItemInfo[m_nCurrentBeginLine+nCount];
		for(nSubCount= 0;nSubCount< LineInfo.GetSize();nSubCount++)
		{
			if(nSubCount >= m_nFixedColumn)
				iStation = nSubCount+m_nHoriBeiginLine;//加上右侧增量
			else
				iStation = nSubCount;

			if(iStation >= m_ArrColumn.GetSize())break;
			iStation = m_ArrdwItemOrder[iStation];//变换对应位置			
			SSubItem& subItem = LineInfo[iStation];
			SColumnHead& column = m_ArrColumn[iStation];
			ItemRect.left = m_DrawRect.left+nLeftSize;
			ItemRect.right = ItemRect.left+ column.dwColumnWide;
			ItemRect.left +=0;
			ItemRect.right -= 5;
			if(ItemRect.left >m_DrawRect.right)break;//超长
			//////////////////////////////////////////////////////////////////////////
			if(subItem.crTextBkColor != -1)//单独背景色
			{
				CRect ClientRect;
				ClientRect = ItemRect;
				ClientRect.right --;
				ClientRect.left ++;
				ClientRect.top++;
				ClientRect.bottom--;
				pDC->FillSolidRect(ClientRect,subItem.crTextBkColor);//设置背景色
			}
			if(subItem.crTextColor != -1)
				pDC->SetTextColor(subItem.crTextColor);//单独文字色
			else
				pDC->SetTextColor(m_crTextColor);//平常文字色
			//////////////////////////////////////////////////////////////////////////
			nDrawFormat = DT_SINGLELINE|DT_VCENTER;
			if(subItem.dwExStyle == -1)
			{//采用标准作业流程
				if(column.dwFormat == LVCFMT_LEFT)
					nDrawFormat |= DT_LEFT;
				else if(column.dwFormat== LVCFMT_CENTER)
					nDrawFormat |= DT_CENTER;
				else
					nDrawFormat |= DT_RIGHT;
			}
			else
			{//采用个性化流程
				if(subItem.dwExStyle& LVCFMT_CENTER)
					nDrawFormat |= DT_CENTER;
				else if(subItem.dwExStyle&LVCFMT_RIGHT)
					nDrawFormat |= DT_RIGHT;
				else nDrawFormat |= DT_LEFT;
			}

			CRect TextRect;
			TextRect = ItemRect;
			//////////////////////////////////////////////////////////////////////////
			if((m_pImageList)&&(subItem.dwItemImage != -1))
			{//标签
				ImagePoint = CPoint(ItemRect.left+2, ItemRect.top+5);
				m_pImageList->Draw(pDC,subItem.dwItemImage,ImagePoint,ILD_NORMAL);//画标记
			}
			TextRect.left +=5;
			if(subItem.pFont)
			{//有个性化字体
				pOldSubFont = pDC->SelectObject(subItem.pFont);//选择个性化字体
				pDC->DrawText(subItem.szItemText,TextRect,nDrawFormat);//文字信息
				pDC->SelectObject(pOldSubFont);//恢复字体
			}
			else
			{//采用默认字体
				pDC->DrawText(subItem.szItemText,TextRect,nDrawFormat);//文字信息				
			}

// 			if((m_pImageList)&&(subItem.dwItemImage != -1))
// 			{//标签
// 				ImagePoint = CPoint(ItemRect.right+2, ItemRect.top+5);
// 				m_pImageList->Draw(pDC,subItem.dwItemImage,ImagePoint,ILD_NORMAL);//画标记
// 			}
			ItemRect.left -=2;
			ItemRect.right += 2;

			if(subItem.dwStyle&SSubItem::TOP_LINE)
			{//单元格上侧画线
				pDC->MoveTo(ItemRect.left,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.top); 
			}
			if(subItem.dwStyle&SSubItem::LEFT_LINE)
			{//单元格左侧画线
				pDC->MoveTo(ItemRect.left,ItemRect.top);
				pDC->LineTo(ItemRect.left,ItemRect.bottom);
			}
			if(subItem.dwStyle&SSubItem::RIGHT_LINE)
			{//单元格右侧画线
				pDC->MoveTo(ItemRect.right,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.bottom);
			}
			if(subItem.dwStyle&SSubItem::BOTTOM_LINE)
			{//单元格下侧画线
				pDC->MoveTo(ItemRect.left,ItemRect.bottom);
				pDC->LineTo(ItemRect.right,ItemRect.bottom);
			}
			if(m_dwExStyle&LVS_EX_GRIDLINES)
			{//分割线
				pDC->MoveTo(ItemRect.right,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.bottom-1);
				pDC->LineTo(ItemRect.left,ItemRect.bottom-1);
			}
			nLeftSize  += column.dwColumnWide;//宽度
		}
	}
	if(oldFont)
	{
		pDC->SelectObject(oldFont);
	}

	pDC->SelectObject(oldPen);

	pDC->SetBkMode(iOldDrawMode);
}

//函数功能：确保指定行可见
//函数参数：nItem 要显示的行数量
//函数返回：成功为TRUE,错误为FALSE
//函数异常：无
BOOL CBaseTable::EnsureRowVisible(int nItem)
{
	if((nItem < 0)||(nItem >=  m_ArrItemInfo.GetSize()))
		return FALSE;//无此行

	int nDisplayableLineCount = GetDisplayableLineNum();
	if(nItem  < m_nCurrentBeginLine)
	{//要显示的行在上面
		while( nItem < m_nCurrentBeginLine)
		{
			m_nCurrentBeginLine --;//= nDisplayableLineCount;//向上翻动
		}
	}
	else if(nItem >= m_nCurrentBeginLine + nDisplayableLineCount)
	{//要显示的行在下面

		while(nItem >= m_nCurrentBeginLine+nDisplayableLineCount)
		{
			m_nCurrentBeginLine ++;//= nDisplayableLineCount;//向下翻动一屏
		}
	}
	return TRUE;
}

void CBaseTable::SetSortParam(int nSortColIndex,BOOL bDescOrder)
{
	m_nSortColIndex = nSortColIndex;
	m_bDescSort = bDescOrder;
}

//函数功能：得到当前的单元数量
//函数参数：无
//函数返回:单元数量
//函数异常:无
int CBaseTable::GetItemCount()
{
	return m_ArrItemInfo.GetSize();
}

//函数功能：得到单元格文字
//函数参数:nItem 行
//         nSubItem 列
//         lpszText 返回的文字
//函数返回:成功为TRUE,错误为FALSE
CString CBaseTable::GetItemText(int nItem, int nSubItem)
{
	if(m_ArrItemInfo.GetSize()<= nItem)return _T("");
	CLineInfo & line = m_ArrItemInfo[nItem];
	SSubItem& subItem = line[nSubItem];
	return subItem.szItemText;
}

//函数功能:得到鼠标点击位置
//函数参数：pt 鼠标点击点
//函数返回：成功则当前选中行，否则返回 -1
//函数异常：无
int CBaseTable::HitTest(CPoint pt)
{
	if(!m_DrawRect.PtInRect(pt))return -1;
	int iLine;
	iLine = (pt.y -m_DrawRect.top)/m_nRowHeight;
	if(m_dwStyle!=LVS_NOCOLUMNHEADER)
	{
		iLine--;//减去头
	}
	if(iLine >= m_ArrItemInfo.GetSize())return -1;

	return iLine;
}

//函数功能：插入一列，用于初始化列表
//函数参数：nCol 当前要插入的位置
//　　　　　lpszColumnHead　列表头的名称
//　　　　　nFormat    列文字的显示位置
//　　　　　nWidth     列的宽度
//函数返回：成功返回当前列的位置，否则为-1

int CBaseTable::InsertColumn(int nCol, LPCTSTR lpszColumnHead, int nFormat, int nWidth)
{
	SColumnHead columnhead;
	columnhead.szColumnText = lpszColumnHead;
	columnhead.dwFormat = nFormat;
	columnhead.dwItemImage = -1;
	columnhead.dwColumnWide = nWidth;
	m_ArrColumn.InsertAt(nCol,columnhead);	
	m_ArrdwItemOrder.Add(nCol);
	return (int)0;
}

//函数功能：修改单元格文字
//函数参数: nItem   单元行
//　　　　　nSubItem 单元列
//　　　　　lpszText 单元文字
//          nImage   单元格显示图像
//          crText   单元文字颜色
//函数返回：成功为TRUE,错误为FALSE
//函数异常：无
BOOL CBaseTable::InsertItem(int nItem, int nSubItem, LPCTSTR lpszText, int nImage,
							COLORREF crText,COLORREF crTextBkColor,DWORD dwStyle )
{
	SSubItem subItem;
	subItem.szItemText = lpszText;
	subItem.crTextBkColor = crTextBkColor;
	subItem.crTextColor = crText;
	subItem.dwItemImage = nImage;
	subItem.dwStyle = dwStyle;
	if(nItem >= m_ArrItemInfo.GetSize())
	{//设置
		m_ArrItemInfo.SetSize(nItem+1);
	}
	CLineInfo& Line = m_ArrItemInfo[nItem];//行
	if(nSubItem >= m_ArrColumn.GetSize())
	{//大于列数量
		return FALSE;
	}
	if(Line.GetSize() < m_ArrColumn.GetSize())
	{
		Line.SetSize(m_ArrColumn.GetSize());
	}
	Line.SetAt(nSubItem,subItem);//列	
	return TRUE;
}

CFont* CBaseTable::SetFont(CFont* pFont)
{
	//System
	CFont* oldFont = NULL;
	oldFont = m_pFont;
	m_pFont = pFont;
	return oldFont;
}

//函数功能：设置表格显示的图像列表
//函数参数: 设置的图像列表的指针
//函数返回：前一个图像列表的指针
//函数异常：GetLastError得到最后的错误
CImageList* CBaseTable::SetImageList(CImageList* pImageList)
{
	CImageList* oldImageList;
	oldImageList = m_pImageList;
	m_pImageList = pImageList;
	return oldImageList;
}


//函数功能：对列排序
//函数参数：pData 当前排序的两列的数据
//　　　　　dwDataNum数据长度
//函数返回:0为失败，其他为成功
int CBaseTable::SortItem(LPVOID pData, DWORD dwDataNum)
{
	return (int)0;
}
//函数功能：设置画图区间
//函数参数：rRect 画图区间
//函数返回:无
//函数异常:无
void CBaseTable::MoveTo(CRect rRect)
{
	m_DrawRect = rRect;
	m_nCurrentShowLines = m_DrawRect.Height()/m_nRowHeight;//当前可显示行数	
};

void CBaseTable::GetRect(CRect& rrectTable)
{
	rrectTable = m_DrawRect;
}

BOOL CBaseTable::InsertItem(int nItem,int nSubItem,SSubItem subItem)
{
	if(nItem >= m_ArrItemInfo.GetSize())
	{//设置
		m_ArrItemInfo.SetSize(nItem+1);
	}
	CLineInfo& Line = m_ArrItemInfo[nItem];//行
	if(nSubItem >= m_ArrColumn.GetSize())
	{//大于列数量
		return FALSE;
	}
	if(Line.GetSize() < m_ArrColumn.GetSize())
	{
		Line.SetSize(m_ArrColumn.GetSize());
	}
	Line.SetAt(nSubItem,subItem);//列	
	return TRUE;
}

BOOL CBaseTable::OnKeyDownEvent(UINT nChar)
{
	switch(nChar)
	{
	case VK_LEFT:
		{
			if(m_nHoriBeiginLine >0)
				m_nHoriBeiginLine --;
			else
				m_nHoriBeiginLine = 0;
		}
		break;
	case VK_RIGHT:
		{
			if(m_nHoriBeiginLine < m_ArrColumn.GetSize()-m_nFixedColumn-1)
				m_nHoriBeiginLine++;
			else
				m_nHoriBeiginLine = m_ArrColumn.GetSize()-m_nFixedColumn-1;
		}
		break;
	case VK_UP:
		{
			m_nCurrentSel --;
			if(m_nCurrentSel < 0)
			{
				int nLine = m_nCurrentSel+m_nCurrentBeginLine;
				if(nLine<0)nLine = 0;//m_ArrItemInfo.GetSize()-1;
				EnsureRowVisible(nLine);
				m_nCurrentSel = nLine-m_nCurrentBeginLine;
			}
		}
		break;
	case VK_DOWN:
		{
			m_nCurrentSel ++;
			
			int nCurPageRowCount = GetDisplayableLineNum();
			int nNewBeginLine = m_nCurrentSel+m_nCurrentBeginLine;
			if( m_nCurrentBeginLine >= GetLastPageBeginLine(m_ArrItemInfo.GetSize()))
			{
				nCurPageRowCount = m_ArrItemInfo.GetSize() - (GetLastPageBeginLine(m_ArrItemInfo.GetSize()));
				nNewBeginLine = 0;
			}
			
			if(m_nCurrentSel > nCurPageRowCount-1)
			{
				EnsureRowVisible(nNewBeginLine);
				m_nCurrentSel = nNewBeginLine-m_nCurrentBeginLine;
				if(m_nCurrentSel>=nCurPageRowCount)
				{
					m_nCurrentSel = nCurPageRowCount-1;
				}
			}			
		}
		break;
	case VK_NEXT:
		{
			int nDisplayableLineCount = GetDisplayableLineNum(); 
			m_nCurrentBeginLine += nDisplayableLineCount;
			int nTotalItemCount = m_ArrItemInfo.GetSize();
			if(m_nCurrentBeginLine > nTotalItemCount)
				m_nCurrentBeginLine = 0;
			if(m_nCurrentBeginLine > nTotalItemCount - nDisplayableLineCount)
			{
				m_nCurrentBeginLine = nTotalItemCount-nDisplayableLineCount;//GetLastPageBeginLine(nTotalItemCount);
			}
		}
		break;
	case VK_PRIOR:
		{
			m_nCurrentBeginLine -= GetDisplayableLineNum();
			if(m_nCurrentBeginLine<0)
			{
				m_nCurrentBeginLine = 0;//GetLastPageBeginLine(m_ArrItemInfo.GetSize());
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}

int CBaseTable::GetDisplayableLineNum()
{
	if(m_dwStyle == LVS_NOCOLUMNHEADER)
		return m_nCurrentShowLines;
	return m_nCurrentShowLines-1;
}

int CBaseTable::GetLastPageBeginLine( int nTotalRowCount)
{
	// 保证第一页显示从0开始	
	int nDisplayableLineCount = GetDisplayableLineNum();
	
	int nLastPageCount = nTotalRowCount % nDisplayableLineCount;
	if( nLastPageCount == 0)
		nLastPageCount = nDisplayableLineCount;
	
	return (nTotalRowCount - nLastPageCount);			
}

int CBaseTable::GetCurPageRowCount(int nCurBeginIndex,int nTotalRowCount)
{
	ASSERT(nCurBeginIndex <= nTotalRowCount);

	if( nTotalRowCount <= 0)
		return 0;
	
	int nDistance = nTotalRowCount - nCurBeginIndex;
	int nDisplayableLineCount = GetDisplayableLineNum();

	if( nDistance < GetDisplayableLineNum())
	{
		return nDistance;
	}
	else
	{
		return nDisplayableLineCount;
	}
}

BOOL CBaseTable::DeleteAll()
{
	m_ArrItemInfo.RemoveAll();
	return TRUE;
}

//函数功能：得到表格总宽度
//函数参数：无
//函数返回：表格总宽度
int CBaseTable::GetTotalWidth()
{
	int iRec;
	int nTotalWidth;

	nTotalWidth = 0;
	for(iRec = 0; iRec < m_ArrColumn.GetSize(); iRec++)
	{
		nTotalWidth += m_ArrColumn[iRec].dwColumnWide;
	}
	return nTotalWidth;
}

//函数功能：得到表格当前宽度
//函数参数：无
//函数返回：当前宽度
int CBaseTable::GetCurWidth()
{
/*	int iRec;
	int nCurWidth;

	nCurWidth = 0;
	bool bOver = false;
	for(iRec = 0; iRec < m_ArrColumn.GetSize() && !bOver; iRec++)
	{
		nCurWidth += m_ArrColumn[iRec].dwColumnWide;
		if(nCurWidth > m_DrawRect.Width())
			bOver = true;
	}
	if(bOver) --iRec;*/
	
	return m_DrawRect.Width();
}

int CBaseTable::GetColumnWidth(int nCol)
{
	if(nCol >= m_ArrColumn.GetSize())return 0;
	return m_ArrColumn[nCol].dwColumnWide;
}

int CBaseTable::GetCurrentHorBeignLine()
{
	return m_nHoriBeiginLine;
}

int CBaseTable::GetFixedColumn()
{
	return m_nFixedColumn;
}

int CBaseTable::GetColumnNum()
{
	return m_ArrColumn.GetSize();
}

void CBaseTable::SetHoriBeginLine(int nBegin)
{
	if(nBegin> 0)
	{
		//////////////////////////////////////////////////////////////////////////
		int nCount;
		for(nCount= m_nFixedColumn;nCount<m_ArrColumn.GetSize();nCount++)
		{
			nBegin -= GetColumnWidth(nCount);
			if(nBegin<0)break;
		}
		m_nHoriBeiginLine = nCount-1;
	}
	else
		m_nHoriBeiginLine = 0;
	//////////////////////////////////////////////////////////////////////////
	if(m_nHoriBeiginLine<0)
		m_nHoriBeiginLine = 0;
	if(m_nHoriBeiginLine >m_ArrColumn.GetSize() - m_nFixedColumn)
		m_nHoriBeiginLine = m_ArrColumn.GetSize() -m_nFixedColumn-1;
}

// 保证指定列能显示出来的开始列索引
int CBaseTable::EnsureColVisible(int nColIndex)
{
	// 获取指定列的实际排序顺序
	int nColOrder;
	for( nColOrder=0; nColOrder<m_ArrColumn.GetSize(); nColOrder++)
	{
		if( m_ArrdwItemOrder[nColOrder] == nColIndex)
			break;
	}	

	if( (nColOrder <= 0) 
		|| ( nColOrder > m_ArrColumn.GetSize()))
	{
		return 0;
	}

	// 除去固定列之后的宽度
	int nActiveWidth = GetCurWidth();
	int i;
	for(i = 0; i<m_nFixedColumn; i++)
	{
		nActiveWidth -= GetColumnWidth(i);
	}
	
	int nBeforeColWidthSum = 0;
	for(i= nColOrder; i>=m_nFixedColumn; i--)
	{
		nBeforeColWidthSum += GetColumnWidth(i);
		if(nBeforeColWidthSum >= nActiveWidth )
			break;
	}
	m_nHoriBeiginLine = i - m_nFixedColumn + 1;
    
	// 返回新的起始列的坐标位置
	nBeforeColWidthSum = 0;
	for( i = 0; i<m_nHoriBeiginLine + m_nFixedColumn; i++)
	{
		nBeforeColWidthSum += GetColumnWidth(i);
	}

	return nBeforeColWidthSum;
}

//函数功能：弹出右键菜单
//函数参数：pWnd - 窗口指针；point - 鼠标点击点
//          uiPopMenuID - 菜单ID；iSel - 菜单序号
//函数返回：选中菜单ID值
UINT CBaseTable::PopupRBtnMenu(CWnd* pWnd,CPoint point,UINT uiPopMenuID,int iSel)
{
	CPoint scrnPoint;
	CMenu popMenu;
	UINT uiItemID;

	if(NULL == pWnd) return -1; //防止指针空

	scrnPoint = point;
	pWnd->ClientToScreen(&scrnPoint);
	popMenu.LoadMenu(uiPopMenuID); //装载菜单资源
	CLanguageInterface::Instance()->InitRightMenu(&popMenu,iSel);
	CMenu* subMenu;
	subMenu = popMenu.GetSubMenu(iSel);
	
	//弹出菜单
	uiItemID = subMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN
		,scrnPoint.x,scrnPoint.y,AfxGetMainWnd()); //弹出菜单
	popMenu.DestroyMenu(); //销毁菜单
	return uiItemID;
}

//设置颜色和字体
void CBaseTable::SetColorsAndFonts()
{
	m_crBkColor = RGB(0,0,0);//CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_crSelectColor = CInterfaceManager::Instance()->GetSysColor(ID_GRIDROWSEL_COLOR);
	m_crTextColor = RGB(255,255,255);//CInterfaceManager::Instance()->GetSysColor(ID_GRIDFIXED_COLOR);
	m_crTextBkColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_crLineColor = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	m_pFont = CInterfaceManager::Instance()->GetSysFont(ID_TABLE_FONT);
}

void CBaseTable::SetSortDirect(int nSortColumn, BOOL bDirect)
{
	m_nSortColIndex = nSortColumn;
	m_bDescSort = bDirect;
}

void CBaseTable::SetDrawColumnLine(BOOL bDraw)
{
	m_bDrawColumnLine = bDraw;
}

void CBaseTable::SetSeledRowFillSolidRect(BOOL bFill,COLORREF crFillColor)
{
	m_bSeledRowFillSolidRect = bFill;
	if( crFillColor != -1)
	{
		m_crSelectColor = crFillColor;
	}
}