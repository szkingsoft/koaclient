#pragma once
#include "afxwin.h"


// CDlgUpdateSelect 对话框

class CDlgUpdateSelect : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpdateSelect)

public:
	CDlgUpdateSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUpdateSelect();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPDATECHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comServer;
	CString   m_strSelectServer;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	map<CString,CString>* m_pMapInfo;
};
