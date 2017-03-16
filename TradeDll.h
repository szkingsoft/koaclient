#pragma once
class CSkinTransform;
class CTradeDll
{
public:
	CTradeDll(void);
	~CTradeDll(void);
	void Init();
private:
	HINSTANCE  m_dllHandle;
	// 接口的函数指针
	typedef  BOOL  (WINAPI *pDestoryDialog)();
	typedef	 HWND (WINAPI *pInitDialog)(HWND hWnd);
	typedef int (WINAPI *pMoveDlg)(CRect rRect);
	typedef int (WINAPI *pLink2Server)(HWND hWnd,int nFinishedMsg,
		const char* szUserID,const char* szUserPwd,
		const char* szIP,int nPort,
		const char* szIP1,int nPort1,
		const char* szIP2,int nPort2,
		const char* szIP3,int nPort3,
		const char* szIP4,int nPort4,
		BOOL bNeedCFCA);
	typedef void (WINAPI *pUnLink)();
//	typedef void (WINAPI *pKeyMessage)(WPARAM wParam,LPARAM lParam);
	typedef int (WINAPI* pQuickOrder)(int nMode, const char *pCode,double dbPrice,int indexWnd);
	typedef int (WINAPI* pGetCustomUnit)(const char *pCode,float& fBuyUnit,float& fSellUnit,int& nSort);
	typedef void (WINAPI* pUpdateQuote)(const char* pData,unsigned int uiDataLen,int indexWnd);
	typedef int (WINAPI* pMenuCmd)(int nMenu,int indexWnd);
	typedef bool (WINAPI* pChangePassword)(int nPwdType,const char* pOldPwd,const char* pNewPwd);
	typedef bool (WINAPI* pSHA1Encrypt)(const char* pSource,int nSourceLen,char* pDest,int& nDestLen);
	typedef void (WINAPI* pChangeRecvWnd)(HWND hWnd,int nFinishedMsg);
	typedef void (WINAPI* pSetInterfaceColor)(void* pColor);
	typedef bool (WINAPI* pGetUserStatus)();
	typedef void (WINAPI* pRefresh)();
	typedef BOOL (WINAPI* pIsNeedChangePwd)();
	typedef int  (WINAPI* pGetUserInfo)(char* szUserName,int iType);
	typedef int  (WINAPI* pGetUpdateLink)(char* szLink);
	typedef int  (WINAPI* pGetFundSignStatus)(char * cChannelCode);
	typedef int  (WINAPI* pGetAccessCtrl)();
	typedef int  (WINAPI* pGetCurSelTab)();
	typedef	 HWND (WINAPI *pInitWnd)(HWND hWnd,int iIndex,int iDisMode);
	typedef	 int (WINAPI *pMoveWnd)(CRect rRect,int iIndex);
	typedef  int (WINAPI *pNotifyTrade)(UINT iMsg, int indexWnd,void *pPar);
	typedef  void (WINAPI *pSetSkinAttribute)(CSkinTransform *pSkin);
	typedef  int (WINAPI *pGetQuoteSymbolAttrib)(QueteSymbolAttribute *stcQSA);
	typedef void (WINAPI *pRegQuoteDateChannel)(void * pShareBuf,CWnd *pNotifyWnd);
	typedef void (WINAPI *pGetTradePW)(char *pstrPw);
public:
	pInitDialog				m_pInitDlog;
	pMoveDlg				m_pMoveDlg;
	pDestoryDialog			m_pDestoryDlg;
	pLink2Server			m_pLink2Server;
	pUnLink					m_pUnLink;
//	pKeyMessage				m_pKeyMessage;
	pQuickOrder				m_pQuickOrder;
	pGetCustomUnit			m_pGetCustomUnit;
	pUpdateQuote			m_pUpdateQuote;
	pMenuCmd				m_pMenuCmd;
	pChangePassword			m_pChangePassword;
	pSHA1Encrypt			m_pSHA1Encrypt;
	pChangeRecvWnd			m_pChangeRecvWnd;
	pChangeRecvWnd          m_pRegDateManageWnd;
	pSetInterfaceColor		m_pSetInterfaceColor;
	pGetUserStatus			m_pGetUserStatus;
	pRefresh                m_pRefresh;
	pIsNeedChangePwd		m_pIsNeedChangePwd;
	pGetUserInfo			m_pGetUserInfo;
	pGetUpdateLink			m_pGetUpdateLink;
	pGetFundSignStatus		m_pGetFundSignStatus;
	pGetCurSelTab			m_pGetCurSelTab;
	pInitWnd                m_pInitWnd;
	pMoveWnd                m_pMoveWnd;
	pNotifyTrade            m_pNotifyTrade;
	pGetAccessCtrl          m_pGetAccessCtrl;
	pSetSkinAttribute       m_pSetSkinAttribute;
	pGetQuoteSymbolAttrib   m_pGetQuoteSymbolAttrib;
	BOOL					m_bInitDlg;
	pRegQuoteDateChannel    m_pRegQuoteDateChannel;
	pGetTradePW             m_pGetTradePW;
};

