operator=
// DlgPopMenu.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgPopMenu.h"

// CDlgPopMenu 对话框

IMPLEMENT_DYNAMIC(CDlgPopMenu, CDialog)

CDlgPopMenu::CDlgPopMenu(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPopMenu::IDD, pParent)
{
	m_bFalgTimer = FALSE;
	m_pResWnd = NULL;
	m_iWith = 0;
	m_iHight = 0;
	m_iGapH = 0;
	m_iGapW = 0;
	m_iItemNum = 0;
	m_iMenu = 0;
}

CDlgPopMenu::~CDlgPopMenu()
{
}

void CDlgPopMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_POPMENU, m_statPopMenu);
}


BEGIN_MESSAGE_MAP(CDlgPopMenu, CDialog)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_COMMAND_RANGE(ID_SYS_BEGIN,ID_SYS_END,OnResMenu)
	ON_COMMAND_RANGE(ID_CUSBEGIN,ID_CUSEND,OnResCustomMenu)
	ON_COMMAND_RANGE(ID_SKIN_BEGIN,ID_SKIN_END,OnResSkinChange)
	ON_COMMAND(ID_APP_ABOUT, &CDlgPopMenu::OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, &CDlgPopMenu::OnAppExit)
END_MESSAGE_MAP()


// CDlgPopMenu 消息处理程序


void CDlgPopMenu::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (IsWindow(m_statPopMenu.GetSafeHwnd()))
	{
		m_statPopMenu.MoveWindow(0,0,cx,cy);
	}
}


BOOL CDlgPopMenu::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgPopMenu::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow)
	{
		UpdateCtrl();
		SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		m_bFalgTimer = TRUE;
	}
}


void CDlgPopMenu::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
}


void CDlgPopMenu::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	ShowWindow(SW_HIDE);
}

BOOL CDlgPopMenu::IsInWnd(CPoint ptCur)
{
	CRect rcWnd;
	GetWindowRect(&rcWnd);

	return rcWnd.PtInRect(ptCur);
}


BOOL CDlgPopMenu::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (WM_MOUSEMOVE == pMsg->message)
	{

		if (m_bFalgTimer == TRUE)
		{
			SetTimer(1000,200,NULL);
			m_bFalgTimer = FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgPopMenu::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1000 == nIDEvent)
	{
		CPoint ptCur;
		GetCursorPos(&ptCur);
		if (!IsInWnd(ptCur))
		{
			ShowWindow(SW_HIDE);
			KillTimer(nIDEvent);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgPopMenu::InitCtrl(CWnd *pResWnd,int iItemWith,int iItemHight,int iItemGap,int iMenu)
{
	m_pResWnd = pResWnd;
	m_statPopMenu.ClearCtrl();
	m_statPopMenu.SetResWnd(pResWnd);
	m_statPopMenu.SetTabType(2);
	m_statPopMenu.SetItmeSize(iItemWith,iItemHight,iItemGap);
	m_statPopMenu.SetSelTab(0);

	m_iWith = iItemWith;
	m_iGapH = iItemGap;
	m_iItemNum = 0;
	m_iHight = iItemHight;
	m_iMenu = iMenu;
}
void CDlgPopMenu::SetCtrlDate()
{
}

void CDlgPopMenu::AddMenu(CString strText, int iMenuId)
{
	if (iMenuId <= 0||strText.IsEmpty())return;
	m_statPopMenu.AddItem(strText,iMenuId);
	m_iItemNum++;
}

void CDlgPopMenu::OnResMenu(UINT iId)
{
	ShowWindow(SW_HIDE);
}
void CDlgPopMenu::OnResCustomMenu(UINT iId)
{
	ShowWindow(SW_HIDE);
	if (iId == ID_CUSTOMTYPE_1)
	{
		CCustomUser_Type1 * pCusUser = (CCustomUser_Type1*)CPublic::Instance()->GetCustomUser();
		if (pCusUser)
		{
			AfxMessageBox(pCusUser->m_strCustomTips);
		}
	}
}

void CDlgPopMenu::SetPos(int iX, int iY)
{
	if (m_iItemNum <= 0)return;
	int iAllH = m_iHight*m_iItemNum + m_iGapH*(m_iItemNum-1);
	MoveWindow(iX,iY,m_iWith,iAllH);
}

void CDlgPopMenu::UpdateCtrl()
{
	do 
	{
		if (m_iMenu == 5)
		{
			std::vector<SKININFO> vecSkin;
			std::vector<SKININFO>::iterator itVec;
			CDateManage::Instance()->GetSkinInfo(vecSkin);
			for (itVec = vecSkin.begin(); itVec != vecSkin.end(); itVec++)
			{
				OnUpdateMenuInfo(ID_SKIN_BEGIN+itVec->iId);
			}
			break;
		}

		CMenu popMenu;
		CMenu* pMenu;
		popMenu.LoadMenu(128); 
		int iSubNum = popMenu.GetMenuItemCount();
		if (iSubNum <= 0 || iSubNum <= m_iMenu)break;;
		pMenu = popMenu.GetSubMenu(m_iMenu);
		int iSubItemNum = pMenu->GetMenuItemCount();
		if (iSubItemNum <= 0)break;
		UINT iState = 0xFFFFFFFF;
		int iItemId = 0;
		for (int x = 0; x < iSubItemNum; x++)
		{
			iItemId = pMenu->GetMenuItemID(x);
			OnUpdateMenuInfo(iItemId);
		}
	} while (0);
	m_statPopMenu.SetSelTab(0);
}
void CDlgPopMenu::OnAppAbout()
{

}
void CDlgPopMenu::OnAppExit()
{

}
void CDlgPopMenu::OnUpdateMenuInfo(UINT CmdUI)
{
	switch( CmdUI ) 
	{
	case ID_SKIN_BLACK:
		{
			m_statPopMenu.EnableItem(CmdUI,CPublic::Instance()->GetSkinType() != 1?TRUE:FALSE);
		}
		break;
	case ID_SKIN_WHITE:
		{
			m_statPopMenu.EnableItem(CmdUI,CPublic::Instance()->GetSkinType() != 2?TRUE:FALSE);
		}
		break;
	case ID_SYSTEM_DISCONNECTHOST: 
		{
			m_statPopMenu.EnableItem(CmdUI,CPublic::Instance()->GetDataSource()->IsOnline());
		}
		break;   
	case ID_SYSTEM_CONNECTTOHOST:
		{
			m_statPopMenu.EnableItem(CmdUI,!CPublic::Instance()->GetDataSource()->IsOnline());
		}
		break;
	case ID_NORMALORDER:
		{
			int iMarketType = CDateManage::Instance()->GetMarketType();
			if (iMarketType == 0)m_statPopMenu.EnableItem(CmdUI,FALSE);
			else m_statPopMenu.EnableItem(CmdUI,TRUE);
		}
		break;
	case ID_SYSTEM_PRICEORDER:
	case ID_SYSTEM_NEWORDER:
		{
			int iMarketType = CDateManage::Instance()->GetMarketType();
			BOOL bUser = FALSE;
			if (iMarketType == 0)
			{
				bUser = TRUE;
			}

			m_statPopMenu.EnableItem(CmdUI,bUser);
		}
		break;
	case ID_TRADE_INMONEY:
		{
			if(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
			{
				char cChannelCode[10] = {0};
				int iRet = CPublic::Instance()->GetTradeDll()->m_pGetFundSignStatus(cChannelCode);

				if (cChannelCode[0] == '7' || cChannelCode[0] == 'B'
					||cChannelCode[0] == '4' || cChannelCode[0] == 'C'
					|| cChannelCode[0] == '9' || cChannelCode[0] == 'A'
					|| cChannelCode[0] == 'E' || cChannelCode[0] == 'G'
					|| cChannelCode[0] == '1'|| cChannelCode[0] == '8')
					m_statPopMenu.EnableItem(CmdUI,TRUE);
				else
					m_statPopMenu.EnableItem(CmdUI,FALSE);
			}
			else
			{
				m_statPopMenu.EnableItem(CmdUI,FALSE);
			}
		}
		break;
	case ID_TRADE_OUTMONEY:
		{
			if(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
			{
				char cChannelCode[10] = {0};
				int iRet = CPublic::Instance()->GetTradeDll()->m_pGetFundSignStatus(cChannelCode);

				if (cChannelCode[0] == '7' || cChannelCode[0] == 'B'
					|| cChannelCode[0] == 'C' || cChannelCode[0] == '4'
					|| cChannelCode[0] == '9' || cChannelCode[0] == 'A'
					|| cChannelCode[0] == 'E' || cChannelCode[0] == 'G'
					|| cChannelCode[0] == '1'|| cChannelCode[0] == '8')
					m_statPopMenu.EnableItem(CmdUI,TRUE);
				else
					m_statPopMenu.EnableItem(CmdUI,FALSE);
			}
			else
			{
				m_statPopMenu.EnableItem(CmdUI,FALSE);
			}
		}
		break;
	case ID_TRADE_SIGN:
		if(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
		{
			char cChannelCode[10] = {0};
			int iRet = CPublic::Instance()->GetTradeDll()->m_pGetFundSignStatus(cChannelCode);

			if (cChannelCode[0] == '9' || cChannelCode[0] == 'F' || cChannelCode[0] == '7' || cChannelCode[0] == 'B' || cChannelCode[0] == '8')
			{
				if(0 == iRet)
				{
					m_statPopMenu.EnableItem(CmdUI,TRUE);
					break;
				}
			}
		}

		m_statPopMenu.EnableItem(CmdUI,FALSE);
		break;
	case ID_TRADE_TERMINATE:
		if(CPublic::Instance()->GetTradeDll()->m_pGetUserStatus())
		{
			char cChannelCode[10] = {0};
			int iRet = CPublic::Instance()->GetTradeDll()->m_pGetFundSignStatus(cChannelCode);

			if (cChannelCode[0] == '9' || cChannelCode[0] == 'A' || cChannelCode[0] == 'F' || cChannelCode[0] == '7' || cChannelCode[0] == 'B' || cChannelCode[0] == '8')
			{
				if(1 == iRet)
				{
					m_statPopMenu.EnableItem(CmdUI,TRUE);
					break;
				}
			}
		}

		m_statPopMenu.EnableItem(CmdUI,FALSE);
		break;
	case ID_TRADE_HOLD:
	case ID_TRADE_PRODUCT:
		{
			int iMarketType = CDateManage::Instance()->GetMarketType();
			BOOL bUser = FALSE;
			if (iMarketType == 0||iMarketType == 1)
			{
				bUser = TRUE;
			}

			m_statPopMenu.EnableItem(CmdUI,bUser);
		}
	default:
		{
			if (ID_SKIN_BEGIN <= CmdUI && CmdUI <= ID_SKIN_END)
			{
				m_statPopMenu.EnableItem(CmdUI,CPublic::Instance()->GetSkinType() != (CmdUI-ID_SKIN_BEGIN)?TRUE:FALSE);
			}
		}
		break;
	}
}
void CDlgPopMenu::OnResSkinChange(UINT iId)
{
	ShowWindow(SW_HIDE);
	int iSkin = iId - ID_SKIN_BEGIN;
	if (iSkin != CPublic::Instance()->GetSkinType())
	{
		CPublic::Instance()->WriteSkinType(iSkin);
		CDateManage::Instance()->InitSkinAttribute();
		AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(ID_TOOL_UPDATE,0),0);
		CWnd* pView = (CWnd*)(CPublic::Instance()->GetView(11,2));
		pView->PostMessage(WM_SKINCHANGE,0,0);

		CPublic::Instance()->GetTradeDll()->m_pNotifyTrade(WM_SKINCHANGE,0,0);
		CPublic::Instance()->GetTradeDll()->m_pNotifyTrade(WM_SKINCHANGE,1,0);
		CPublic::Instance()->GetTradeDll()->m_pNotifyTrade(WM_SKINCHANGE,2,0);
		CPublic::Instance()->GetTradeDll()->m_pNotifyTrade(WM_SKINCHANGE,3,0);
	}
}