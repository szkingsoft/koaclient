#pragma once
#include "afxwin.h"


// CDlgWarnInfo 对话框

class CDlgWarnInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgWarnInfo)

public:
	CDlgWarnInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWarnInfo();

// 对话框数据
	enum { IDD = IDD_DLG_WARNINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void ReArrayWnd();
public:
	virtual BOOL OnInitDialog();
	CString m_strWarnInfo;
	CEdit m_WARNINFO;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_btnAgree;
};
