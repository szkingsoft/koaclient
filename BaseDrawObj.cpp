#include "stdafx.h"
#include "BaseDrawObj.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBaseDrawObj::CBaseDrawObj()
{
	m_bActivate= FALSE;
	m_iType = 0;
}

CBaseDrawObj::~CBaseDrawObj()
{
}

void CBaseDrawObj::MoveTo(CRect rRect)
{
	m_DrawRect = rRect;
	RecalSubObjSize();
}


//函数功能：弹出右键菜单
//函数参数：pWnd - 窗口指针；point - 鼠标点击点
//          uiPopMenuID - 菜单ID；iSel - 菜单序号
//函数返回：选中菜单ID值
UINT CBaseDrawObj::PopupRBtnMenu(CWnd* pWnd,CPoint point,UINT uiPopMenuID,int iSel)
{
	CPoint scrnPoint;
	CMenu popMenu;
	UINT uiItemID;

	if(NULL == pWnd) return -1; //防止指针空

	scrnPoint = point;
	pWnd->ClientToScreen(&scrnPoint);
	popMenu.LoadMenu(uiPopMenuID); //装载菜单资源
	CLanguageInterface::Instance()->InitRightMenu(&popMenu,iSel);
	uiItemID = popMenu.GetSubMenu(iSel)->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN
		,scrnPoint.x,scrnPoint.y,AfxGetMainWnd()); //弹出菜单
	popMenu.DestroyMenu(); //销毁菜单
	return uiItemID;
}

void CBaseDrawObj::ActivateObj(BOOL bActivate)
{
	m_bActivate= bActivate;
}
