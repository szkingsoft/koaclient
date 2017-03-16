#pragma once
#include <afxwin.h>
#include "resource.h"
#include "afxwin.h"


// CChangePswdForm 对话框 

class CChangePswdForm : public CDialog
{
	DECLARE_DYNAMIC(CChangePswdForm)

public:
	CChangePswdForm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangePswdForm();

// 对话框数据
	enum { IDD = IDD_DLG_CHANGEPSWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	/// 点击确定按钮
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedCancel();
public:	//控件
	CEdit m_edtCurPswd;
	CEdit m_edtNewPswd;
	CEdit m_edtNewPswd2;

	virtual BOOL OnInitDialog();
	CComboBox m_comPwdType;
	afx_msg void OnCbnSelchangeComboPwdtype();
};
