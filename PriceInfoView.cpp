
#include "stdafx.h"
#include "MainFrm.h"
#include "PriceInfoView.h"
#include "Resource.h"
#include "KOAClient.h"


//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

CPriceInfoView::CPriceInfoView()
{
}

CPriceInfoView::~CPriceInfoView()
{
}

BEGIN_MESSAGE_MAP(CPriceInfoView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(WM_REDRAWWND,OnRedrawMsg)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPriceInfoView 消息处理程序

int CPriceInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle =  WS_CHILD | WS_VISIBLE|LVS_LIST| WS_CLIPCHILDREN|WM_NOTIFY;

	if (!m_PriceInfoWnd.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	CPublic::Instance()->RegistWnd(VIEWINFO_PRICEINFO,this);
	return 0;
}

void CPriceInfoView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
	//RecalcLayout();
}


void CPriceInfoView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu popMenu;
	if(NULL == pWnd) return ; //防止指针空	
	popMenu.LoadMenu(IDR_RIGHTMENU); //装载菜单资源
	CMenu* pMenu;
	pMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_REPORTTABLE);
	CLanguageInterface::Instance()->InitRightMenu(pMenu,POPSUBMENU_INDEX_REPORTTABLE);
	UINT uiCmd;
	uiCmd = pMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN|TPM_RETURNCMD
		,point.x,point.y,AfxGetMainWnd()); //弹出菜单
	popMenu.DestroyMenu(); //销毁菜单

	SCodeInfo* pCode;
	pCode = m_PriceInfoWnd.GetCurSel();
	CPublic::Instance()->CallStockMenu(uiCmd,pCode);
}

void CPriceInfoView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_PriceInfoWnd.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	m_PriceInfoWnd.SetFocus();
}

//void CPriceInfoView::OnPaint()
//{
//	CPaintDC dc(this); // 用于绘制的设备上下文
//	CMyMemDC memdc(&dc);
//	m_PriceInfo.Draw(&memdc);
//}

void CPriceInfoView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_PriceInfoWnd.SetFocus();
}

LRESULT CPriceInfoView::OnDataMessage(WPARAM wParam,LPARAM lParam)
{
	m_PriceInfoWnd.UpdateData((char*)wParam,(int)lParam);
	return 1;
}

void CPriceInfoView::OnClose()
{
	CPublic::Instance()->UnRegistWnd(VIEWINFO_PRICEINFO,this);
	CDockablePane::OnClose();
}

LRESULT CPriceInfoView::OnRedrawMsg(WPARAM wParam,LPARAM lParam)
{
	m_PriceInfoWnd.RedrawWindow();
	return 1;
}