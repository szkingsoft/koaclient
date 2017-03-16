#include "stdafx.h"
#include "TradeDll.h"


CTradeDll::CTradeDll(void)
{
	Init();
}


CTradeDll::~CTradeDll(void)
{
	if(NULL != m_dllHandle )
		FreeLibrary(m_dllHandle);
}

void CTradeDll::Init()
{
	m_dllHandle = ::LoadLibrary(_T("trade.dll"));
	DWORD dwError = GetLastError();
	if (0 == m_dllHandle)
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10015),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONERROR);
		return ;
	}
	m_pInitDlog = (pInitDialog)::GetProcAddress(m_dllHandle, "InitDialog");
	m_pMoveDlg = (pMoveDlg)::GetProcAddress(m_dllHandle, "MoveDlg");;
	m_pDestoryDlg = (pDestoryDialog)::GetProcAddress(m_dllHandle, "DestoryDialog");
	m_pLink2Server = (pLink2Server)::GetProcAddress(m_dllHandle,"Link2Server");
	m_pUnLink	=(pUnLink)::GetProcAddress(m_dllHandle,"UnLink");
//	m_pKeyMessage =(pKeyMessage)::GetProcAddress(m_dllHandle,"KeyMessage");
	m_pQuickOrder = (pQuickOrder)::GetProcAddress(m_dllHandle,"QuickOrder");
	m_pGetCustomUnit =(pGetCustomUnit)::GetProcAddress(m_dllHandle,"GetCustomUnit");
	m_pUpdateQuote = (pUpdateQuote)::GetProcAddress(m_dllHandle,"UpdateQuote");
	m_pMenuCmd = (pMenuCmd)::GetProcAddress(m_dllHandle,"MenuCmd");
	m_pChangePassword =(pChangePassword)::GetProcAddress(m_dllHandle,"ChangePassword");
	m_pSHA1Encrypt =(pSHA1Encrypt)::GetProcAddress(m_dllHandle,"SHA1Encrypt");
	m_pChangeRecvWnd = (pChangeRecvWnd)::GetProcAddress(m_dllHandle,"ChangeRecvWnd");
	m_pRegDateManageWnd = (pChangeRecvWnd)::GetProcAddress(m_dllHandle,"RegRecvWnd");
	m_pSetInterfaceColor=(pSetInterfaceColor)::GetProcAddress(m_dllHandle,"SetInterfaceColor");
	m_pGetUserStatus = (pGetUserStatus)::GetProcAddress(m_dllHandle,"GetUserStatus");
	m_pRefresh = (pRefresh)::GetProcAddress(m_dllHandle,"Refresh");
	m_pIsNeedChangePwd =(pIsNeedChangePwd)::GetProcAddress(m_dllHandle,"IsNeedChangePwd");
	m_pGetUserInfo = (pGetUserInfo)::GetProcAddress(m_dllHandle,"GetUserInfo");
	m_pGetUpdateLink = (pGetUpdateLink)::GetProcAddress(m_dllHandle,"GetUpdateLink");
	m_pGetFundSignStatus = (pGetFundSignStatus)::GetProcAddress(m_dllHandle,"GetFundSignStatus");
	m_pGetCurSelTab = (pGetCurSelTab)::GetProcAddress(m_dllHandle,"GetCurSelTab");
	m_pInitWnd = (pInitWnd)::GetProcAddress(m_dllHandle,"InitWnd");
	m_pMoveWnd = (pMoveWnd)::GetProcAddress(m_dllHandle,"MoveWnd");
	m_pNotifyTrade = (pNotifyTrade)::GetProcAddress(m_dllHandle,"NotifyTrade");
	m_pGetAccessCtrl = (pGetAccessCtrl)::GetProcAddress(m_dllHandle,"GetAccessCtrl");
	m_pSetSkinAttribute = (pSetSkinAttribute)::GetProcAddress(m_dllHandle,"SetSkinAttribute");
	m_pGetQuoteSymbolAttrib = (pGetQuoteSymbolAttrib)::GetProcAddress(m_dllHandle,"GetQuoteSymbolAttrib");
	m_pRegQuoteDateChannel = (pRegQuoteDateChannel)::GetProcAddress(m_dllHandle,"RegQuoteDateChannel");
	m_pGetTradePW = (pGetTradePW)::GetProcAddress(m_dllHandle,"GetTradePW");
	if(!m_pInitDlog||!m_pMoveDlg||!m_pDestoryDlg||!m_pLink2Server||!m_pUnLink||!m_pQuickOrder||!m_pGetCustomUnit
		||!m_pUpdateQuote||!m_pMenuCmd||!m_pChangePassword||!m_pSHA1Encrypt||!m_pChangeRecvWnd||!m_pSetInterfaceColor
		||!m_pGetUserStatus||!m_pRefresh||!m_pIsNeedChangePwd||!m_pGetUserInfo||!m_pGetUpdateLink||!m_pGetFundSignStatus
		||!m_pGetCurSelTab||!m_pInitWnd||!m_pMoveWnd||!m_pNotifyTrade||!m_pGetAccessCtrl||!m_pSetSkinAttribute
		||!m_pGetQuoteSymbolAttrib||!m_pRegQuoteDateChannel||!m_pGetTradePW)
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10016),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONERROR);
		return ;
	}
}


