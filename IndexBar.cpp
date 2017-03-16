// IndexBar.cpp : implementation file
//

#include "stdafx.h"
#include "koaclient.h"
#include "IndexBar.h"

#include "mainfrm.h"

#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexBar
#define RIGHTRECEVER 42
CIndexBar::CIndexBar()
{
}

CIndexBar::~CIndexBar()
{
}


BEGIN_MESSAGE_MAP(CIndexBar, CStatusBar)
	//{{AFX_MSG_MAP(CIndexBar)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexBar message handlers

int CIndexBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_ImageList.Create(IDB_STATUSBITMAP,16,16,RGB(0,0,0));

	m_uiRedrawTimer = SetTimer(10,1000,NULL);
	
	return 0;
}

void CIndexBar::OnDestroy() 
{
	CStatusBar::OnDestroy();
	KillTimer(m_uiRedrawTimer);
}

void CIndexBar::OnPaint() 
{
	CPaintDC pDC(this); // device context for painting
	CMyMemDC dc(&pDC,&m_TotalRect);
	//////////////////////////////////////////////////////////////////////////
	COLORREF BkColor;
	BkColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	dc.FillSolidRect(m_TotalRect,BkColor);//填充背景色
	//////////////////////////////////////////////////////////////////////////
	//画出分割线
	COLORREF clrBord;
	clrBord= GetSysColor(COLOR_INACTIVEBORDER);
	
	CPen DvidPen(PS_SOLID,2,clrBord);
	CPen *OldPen = dc.SelectObject(&DvidPen);

	dc.MoveTo(m_ScollTextRect.left,m_ScollTextRect.top);
	dc.LineTo(m_ScollTextRect.left,m_ScollTextRect.bottom);
	dc.MoveTo(m_ScollTextRect.right,m_ScollTextRect.top);
	dc.LineTo(m_ScollTextRect.right,m_ScollTextRect.bottom);
	dc.MoveTo(m_TotalRect.left,m_TotalRect.bottom);
	dc.LineTo(m_TotalRect.right,m_TotalRect.bottom);
	dc.MoveTo(m_ImgRect[0].right,m_ImgRect[0].top);
	dc.LineTo(m_ImgRect[0].right,m_ImgRect[0].bottom);
	dc.MoveTo(m_ImgRect[1].right,m_ImgRect[1].top);
	dc.LineTo(m_ImgRect[1].right,m_ImgRect[1].bottom);
	
	dc.SelectObject(OldPen);
	//////////////////////////////////////////////////////////////////////////
	//画出图标
	CPoint pt;
	for(int i= 0;i<2;i++)
	{
		pt.x = m_ImgRect[i].left+2;
		pt.y = m_ImgRect[i].top;
		if(CPublic::Instance()->GetDataSource()->IsOnline())
			m_ImageList.Draw(dc,i,pt,ILD_NORMAL);
		else
			m_ImageList.Draw(dc,i+2,pt,ILD_NORMAL);
	}
}

void CIndexBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CStatusBar::OnLButtonDblClk(nFlags, point);
}

BOOL CIndexBar::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CIndexBar::OnSize(UINT nType, int cx, int cy) 
{
	CStatusBar::OnSize(nType, cx, cy);
	
	m_TotalRect = CRect(0,0,cx,cy);
	m_ScollTextRect =m_TotalRect;
	m_ScollTextRect.right = m_TotalRect.Width()-RIGHTRECEVER;
	m_ImgRect[0]=m_ImgRect[1] = m_TotalRect;
	m_ImgRect[0].left = m_ImgRect[0].right - RIGHTRECEVER/2;
	m_ImgRect[1].right = m_ImgRect[0].left;
	m_ImgRect[1].left = m_ImgRect[1].right -RIGHTRECEVER/2;
}


void CIndexBar::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_uiRedrawTimer == nIDEvent)
	{
		Invalidate();
	}
	CStatusBar::OnTimer(nIDEvent);
}
