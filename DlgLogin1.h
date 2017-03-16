#pragma once
#include "IdentifyCode.h"
#include "afxwin.h"
#include "PictureEx.h"
// CDlgLogin1 对话框

class CDlgLogin1 : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogin1)

public:
	CDlgLogin1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLogin1();

// 对话框数据
	enum { IDD = IDD_DLG_LOGIN1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtn();
	afx_msg void OnBnClickedBtnAgentsetting();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnSetTextMsg( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CIdentifyCode m_stcCheckCode;
	CString m_strCheckCode;
	int m_nServerSel;
	int m_nSelLanguage;
	CString m_strUserName;
	CString m_strPassword;
	CString	m_strMemPwd;
	BOOL m_bSaveAccount;
	CComboBox m_commServerList;
	CPictureEx m_ctrlGifTitle;
public:   

	void ReturnStart();

	void Open(int nMinPos = 0 ,int nMaxPos = 100);

	void Close();

	// 设置进度范围
	BOOL SetRange(int nMinPos, int nMaxPos);

	// 设置当前进度位置
	void SetPos(int nCurPos,const char *szPrompt = NULL);


protected:
	void SetButtonStat(int nCount,int *pEnableBtnIDList);
private:
	int m_nMinPos, m_nMaxPos;  //进度的范围
	int m_nCurPos;
	int m_nMovePos;
	BOOL	m_bLoginStatus;//登陆状态
public:
	CStatic m_editPrompt;
	afx_msg void OnStnClickedStaticCheckcode();
	int m_nNeedCFCA;
};
