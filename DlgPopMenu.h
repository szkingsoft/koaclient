#pragma once
#include "TabLst.h"

// CDlgPopMenu 对话框

typedef void (WINAPI *pResMenuFun)(void *pvoid ,UINT iMenuId);
class CDlgPopMenu : public CDialog
{
	DECLARE_DYNAMIC(CDlgPopMenu)

public:
	CDlgPopMenu(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPopMenu();

// 对话框数据
	enum { IDD = IDD_DLG_POPMENU };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bFalgTimer;

	CTabLst m_statPopMenu;

	CWnd* m_pResWnd;
	int m_iWith;
	int m_iHight;
	int m_iGapH;
	int m_iGapW;
	int m_iItemNum;
	int m_iMenu;

	void OnUpdateMenuInfo(UINT CmdUI);
public:
	void InitCtrl(CWnd *pResWnd=NULL,int iItemWith = 100,int iItemHight=30,int iItemGap=0,int iMenu=0);
	void SetCtrlDate();
	BOOL IsInWnd(CPoint ptCur);
	void AddMenu(CString strText, int iMenuId);

	void SetPos(int iX, int iY);
private:
	void UpdateCtrl();
public:
	afx_msg void OnResMenu(UINT iId);
	afx_msg void OnResCustomMenu(UINT iId);
	afx_msg void OnResSkinChange(UINT iId);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
};
