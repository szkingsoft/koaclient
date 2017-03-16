// PriceInfoWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "koaclient.h"
#include "PriceInfoWnd.h"


// CPriceInfoWnd

IMPLEMENT_DYNAMIC(CPriceInfoWnd, CListCtrl)

CPriceInfoWnd::CPriceInfoWnd()
{

}

CPriceInfoWnd::~CPriceInfoWnd()
{
}


BEGIN_MESSAGE_MAP(CPriceInfoWnd, CListCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
//	ON_WM_MOUSEHWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CPriceInfoWnd 消息处理程序




int CPriceInfoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CPriceInfoWnd::OnDestroy()
{
	CListCtrl::OnDestroy();	
}


void CPriceInfoWnd::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	CRect rectClient(0,0,cx,cy);
	m_PriceInfoObj.MoveTo(rectClient);
}


void CPriceInfoWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	m_PriceInfoObj.OnChar(nChar);
	RedrawWindow();
}


void CPriceInfoWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDown(nFlags, point);
	m_PriceInfoObj.OnLButtonDown(nFlags,point);
	RedrawWindow();
}



void CPriceInfoWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDblClk(nFlags, point);
	m_PriceInfoObj.OnLButtonDblClk(nFlags,point);
	RedrawWindow();
}


void CPriceInfoWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CMyMemDC memdc(&dc);
	m_PriceInfoObj.Draw(&memdc);
}

BOOL CPriceInfoWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

}

BOOL CPriceInfoWnd::UpdateData(char* pData,int nDataLen)
{
	//m_PriceInfoObj.UpdateData(pData,nDataLen);
	RedrawWindow();
	return TRUE;
}
SCodeInfo*	CPriceInfoWnd::GetCurSel()
{
	return m_PriceInfoObj.GetCurSelCodeInfo();
}

void CPriceInfoWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
//	CListCtrl::OnRButtonDown(nFlags, point);
	m_PriceInfoObj.OnLButtonDown(nFlags,point);
	RedrawWindow();
}


BOOL CPriceInfoWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//int nPageNum = zDelta/120;
	//if(nPageNum<0)
	//{
	//	m_PriceInfoObj.OnChar(VK_NEXT);
	//}
	//else
	//{
	//	m_PriceInfoObj.OnChar(VK_PRIOR);
	//}
	//return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
	::PostMessage(GetParent()->GetSafeHwnd(),WM_MOUSEWHEEL,MAKEWPARAM(nFlags,zDelta),MAKELPARAM(pt.x,pt.y));
	return 1;
}


void CPriceInfoWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CListCtrl::OnChar(nChar, nRepCnt, nFlags);
	if((nChar>='0')&&(nChar<='9') 
		|| (nChar>='A')&&(nChar<='Z')
		|| (nChar>='a')&&(nChar<='z'))
	{
		AfxGetMainWnd()->SendMessage(WM_CHAR, nChar,MAKELONG(0,nFlags));
	}
}
