// DpTable.cpp : implementation file
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DpTable.h"


// CDpTable

IMPLEMENT_DYNAMIC(CDpTable, CDockablePane)

CDpTable::CDpTable()
{

}

CDpTable::~CDpTable()
{
}


BEGIN_MESSAGE_MAP(CDpTable, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDpTable message handlers




int CDpTable::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_statTab.Create(_T("123"),SS_NOTIFY,CRect(0,0,30,100),this,1111);
	m_statTab.ShowWindow(SW_SHOW);

	return 0;
}

void CDpTable::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	CRect rectClient;
	GetClientRect(&rectClient);
	m_statTab.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	m_statTab.SetFocus();
}