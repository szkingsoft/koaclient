// PriceListWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "koaclient.h"
#include "PriceListWnd.h"


// CPriceListWnd

IMPLEMENT_DYNAMIC(CPriceListWnd, CListCtrl)

CPriceListWnd::CPriceListWnd()
{
	m_bShowUpdateTime = TRUE;
	m_bShowHighLow = TRUE;
	m_bShowOtherSymbol = TRUE;
}

CPriceListWnd::~CPriceListWnd()
{
}


BEGIN_MESSAGE_MAP(CPriceListWnd, CListCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
//	ON_WM_MOUSEHWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CPriceListWnd 消息处理程序

int CPriceListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ReportTable.Init();//初始化列表

	return 0;
}


void CPriceListWnd::OnDestroy()
{
	CListCtrl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void CPriceListWnd::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	CRect rectClient(0,0,cx,cy);
	m_ReportTable.MoveTo(rectClient);//报表对象
}


void CPriceListWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	m_ReportTable.OnChar(nChar);
	RedrawWindow();
}


void CPriceListWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDown(nFlags, point);
	m_ReportTable.OnLButtonDown(this,nFlags,point);
	RedrawWindow();
}

void CPriceListWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDblClk(nFlags, point);
	m_ReportTable.OnLButtonDblClk(this,nFlags,point);
	RedrawWindow();
}


void CPriceListWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CMyMemDC memdc((CDC*)&dc);
	m_ReportTable.Draw(&memdc); 
}

BOOL CPriceListWnd::UpdateData(char* pData,int nDataLen)
{	
	//m_ReportTable.UpdateData(pData,nDataLen);
	RedrawWindow();
//	Invalidate();
	return  TRUE;
}

BOOL CPriceListWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
//	return CListCtrl::OnEraseBkgnd(pDC);
}
SCodeInfo* CPriceListWnd::GetCurSel()
{
	return m_ReportTable.GetCurSelCodeInfo();
}

void CPriceListWnd::RButtonDown(CPoint point)
{
//	CListCtrl::OnRButtonDown(nFlags, point);
	m_ReportTable.OnLButtonDown(this,0,point);
	RedrawWindow();
}


BOOL CPriceListWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nPageNum = zDelta/120;
	if(nPageNum<0)
	{
		m_ReportTable.OnChar(VK_NEXT);
	}
	else
	{
		m_ReportTable.OnChar(VK_PRIOR);
	}
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}


void CPriceListWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CListCtrl::OnChar(nChar, nRepCnt, nFlags);
	if((nChar>='0')&&(nChar<='9') 
		|| (nChar>='A')&&(nChar<='Z')
		|| (nChar>='a')&&(nChar<='z'))
	{
		AfxGetMainWnd()->SendMessage(WM_CHAR, nChar,MAKELONG(0,nFlags));
	}
}

void	CPriceListWnd::ShowHideUpdateTime()
{
	m_bShowUpdateTime = !m_bShowUpdateTime;
	m_ReportTable.ShowTime(m_bShowUpdateTime);
}

void	CPriceListWnd::ShowHideHighLow()
{
	m_bShowHighLow = !m_bShowHighLow;
	m_ReportTable.ShowHighLowPrice(m_bShowHighLow);
}

void CPriceListWnd::ShowHideOtherSymbol()
{
	m_bShowOtherSymbol = !m_bShowOtherSymbol;
	m_ReportTable.ShowHideOtherQuote(m_bShowOtherSymbol);
}

void CPriceListWnd::Redraw()
{
	m_ReportTable.ResetTableColor();
	RedrawWindow();
}