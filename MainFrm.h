
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "PriceListView.h"
#include "PriceInfoView.h"
#include "TradeWnd.h"
#include "DpTable.h"

#include "DlgMainClient.h"

#include "WWndLessButton.h"
#include "WRestoreButton.h"

//#include "DlgMainWnd.h"
#define IDB_FRAME_CLOSE       99
#define IDB_FRAME_MAXRESTORE  98
#define IDB_FRAME_MIN         97

#define EVENT_REFRESH         1230
#define EVENT_REFRESH_TM      1000*1
class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW /*| FWS_ADDTOTITLE*/, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
//	CMFCMenuBar			m_wndMenuBar;
//	CMFCToolBar			m_wndToolBar;
	CMFCToolBar			m_wndDrawLineBar; // 画线工具栏	
//	CMFCToolBar			m_wndFullScreenBar; //全屏显示切换工具栏
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CPriceListView    m_wndPriceListView;
	CPriceInfoView        m_wndPriceInfoView;
	CTradeWnd        m_wndTrade;
	/////////
	//CDpTable         m_TabDp;
	//CMFCToolBar		  m_ToolbarLeft; 
	//CMFCToolBarButton mfcBt;
	//CMFCToolBarButton mfcBt2;
	//void OnApplicationLook(UINT id);
	//CDockablePane* pTabbedBar;
	//CDlgMainClient m_DlgMainClient;
	BOOL ChangToolBar(UINT iMode);
	afx_msg LRESULT OnShowToolBar(WPARAM wParam,LPARAM lParam);
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTradeMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTaskManage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnMenuInfo(UINT nID);
	afx_msg void OnUpdateMenuInfo(CCmdUI* pCmdUI);
	afx_msg LRESULT OnKeyGhostMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnKeyGhostMessageZYQ(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateStatusBarPanes(CCmdUI* pCmdUI);
	afx_msg LRESULT OnLoginChangePwd(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
private:
	CBitmap m_bitmapLinked;				//连接状态用图标
	CBitmap m_bitmapUnlink;				//断链状态用图标


	int		m_nReconnectCount;//断线重连次数
	int		m_nTradeReconnectCount;//交易端短线重连
	UINT	m_uiCheckConnectTimer;
	BOOL    m_bInternetStatus;//网络状态


	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 2013/10/7
//	CMenu m_menuMain;
	////
	//CDlgMainWnd m_dlgMainWnd;
public:
	//全屏模式
	void FullScreenMode(bool bShow);//设置全屏模式



	BOOL m_bFullScreenMode;			    //当前状态
	CRect m_FullScreenRect;				//表示全屏显示时的窗口位置 
	WINDOWPLACEMENT m_OldWndPlacement;	//用来保存原窗口位置    
	BOOL	m_bReLogin;
public:
	BOOL		m_bShowDrawLineBar;//是否显示划线工具

	BOOL		m_bShowState;
	NOTIFYICONDATA m_SkyNID;

	void ShowDrawLineToolBar();
	void Save2Pic();//保存为图片
	void RegHotKey();//注册热键
	void UnRegHotKey();//注销热键
	void ShowMainFrm();
	void HideMainFrm();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	void ExitMainWnd(int iDs = 0);
	void ResetStatusBarUserInfo();
	void ResetStatusBarLinkStatus(int nIndex,BOOL bLinked);
private:
	void ResetStatusBar(void);

private:
	int m_iCaptionWith;
	CRect m_rcBorder;
	Image *m_pImgCaption;
	Image * m_pImgBk;
	Image * m_pImglogo;
	Image * m_pImgBorderL;
	Image * m_pImgBorderR;
	Image * m_pImgBorderB;
	DWORD m_clrCaptionBk;
	CToolTipCtrl	m_sysToolTipCtrl;
	WWndLessButton	m_sysCloseBtn;
	WRestoreButton m_sysMaxBtn;
	WWndLessButton	m_sysMinBtn;
	BOOL m_bRefresh;

	std::vector<WWndLessButton*> m_vecMenu;
	afx_msg void OnTitleMenu(UINT nID);
	void OnFrameClose();
	void OnFrameMin();
	void OnFrameMax();
	void OnFrameRestore();

	void InitCtrl();
	void InitCaption();
	void ReleasImg();

	void UpdateSysBtnStat(UINT nType);
	BOOL OnSetSysButtonPos();
	void CreateSysButton();
	void DrawCaption(CDC *pDC);
	void DrawBorder(CDC *pDC);
	void DrawSysButtons(CDC *pDC);
	void DrawNcPanit(CDC *pDC);

//	virtual void ActivateFrame(int nCmdShow = -1);

	void ClearFund();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
//	afx_msg void OnPaint();
	LRESULT OnLoginSuccess(WPARAM wPar,LPARAM lPar);
};


