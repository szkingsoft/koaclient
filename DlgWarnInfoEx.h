#pragma once
#include "afxwin.h"


// CDlgWarnInfoEx 对话框

class CDlgWarnInfoEx : public CDialog
{
	DECLARE_DYNAMIC(CDlgWarnInfoEx)

public:
	CDlgWarnInfoEx(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWarnInfoEx();

// 对话框数据
	enum { IDD = IDD_DLG_WARNINFOEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strWarn;
	CEdit m_edtWarn;
	CButton m_btnAgree;

	void InitCtrl();
	void ReArrayWnd();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
