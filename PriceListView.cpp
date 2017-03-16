
#include "stdafx.h"
#include "mainfrm.h"
#include "PriceListView.h"
#include "Resource.h"
#include "KOAClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPriceListView

CPriceListView::CPriceListView()
{

}

CPriceListView::~CPriceListView()
{
}

BEGIN_MESSAGE_MAP(CPriceListView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(WM_REDRAWWND,OnRedrawMsg)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int CPriceListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE|LVS_LIST| WS_CLIPCHILDREN|WM_NOTIFY;;


	if (!m_PriceListWnd.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	AdjustLayout();
	CPublic::Instance()->RegistWnd(VIEWINFO_PRICELIST,this);

	return 0;
}

void CPriceListView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPriceListView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu popMenu;
	if(NULL == pWnd) return ; //防止指针空	
	CPoint ptClient;
	ptClient = point;
	ScreenToClient(&ptClient);
	m_PriceListWnd.RButtonDown(ptClient);
	popMenu.LoadMenu(IDR_RIGHTMENU); //装载菜单资源
	
	CMenu* pMenu;
	pMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_REPORTTABLE);
	CLanguageInterface::Instance()->InitRightMenu(pMenu,POPSUBMENU_INDEX_REPORTTABLE);
	pMenu->CheckMenuItem(ID_QUOTE_SHOWHIGHLOW,m_PriceListWnd.GetShowHighLowStatus()?MF_CHECKED:MF_UNCHECKED);
	pMenu->CheckMenuItem(ID_QUOTE_SHOWUPDATETIME,m_PriceListWnd.GetShowUpdateTimeStatus()?MF_CHECKED:MF_UNCHECKED);
	pMenu->CheckMenuItem(ID_QUOTE_SHOWOTHERQUOTE,m_PriceListWnd.GetShowOtherSymbol()?MF_CHECKED:MF_UNCHECKED);
	UINT uiCmd;
	uiCmd = pMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN|TPM_RETURNCMD
		,point.x,point.y,AfxGetMainWnd()); //弹出菜单
	popMenu.DestroyMenu(); //销毁菜单
	
	if((uiCmd == ID_QUOTE_SHOWUPDATETIME))
	{
		m_PriceListWnd.ShowHideUpdateTime();
	}
	else if(uiCmd == ID_QUOTE_SHOWHIGHLOW)
	{
		m_PriceListWnd.ShowHideHighLow();
	}
	else if(uiCmd == ID_QUOTE_SHOWOTHERQUOTE)
	{
		m_PriceListWnd.ShowHideOtherSymbol();
	}
	else
	{
		SCodeInfo* pCode;
		pCode = m_PriceListWnd.GetCurSel();
		CPublic::Instance()->CallStockMenu(uiCmd,pCode);
	}
	m_PriceListWnd.RedrawWindow();

}

void CPriceListView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	m_PriceListWnd.SetWindowPos(NULL, rectClient.left , rectClient.top , rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_PriceListWnd.SetFocus();
}

void CPriceListView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_PriceListWnd.SetFocus();
}


LRESULT CPriceListView::OnDataMessage(WPARAM wParam,LPARAM lParm)
{
//	m_ReportTable.UpdateData((LPVOID)wParam);
	m_PriceListWnd.UpdateData((char*)wParam,(int)lParm);
	return 1;
}

void CPriceListView::RequestData()
{
	if(CPublic::Instance()->GetStockTotal() <=0)return;
	SNormaHead  tRequest;
	tRequest.m_usIndex = CPublic::Instance()->AddRequestIndex();
	tRequest.m_usSize = 0;
	tRequest.m_usType = REPORTDATA;
	CPublic::Instance()->SendRequestData((char*)&tRequest,sizeof(SNormaHead));//请求报价表数据
}

void CPriceListView::OnClose()
{
	CPublic::Instance()->UnRegistWnd(VIEWINFO_PRICELIST,this);
	CDockablePane::OnClose();
}

LRESULT CPriceListView::OnRedrawMsg(WPARAM wParam,LPARAM lParam)
{
	m_PriceListWnd.Redraw();
	return 1;
}