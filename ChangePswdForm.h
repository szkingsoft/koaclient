#pragma once
#include <afxwin.h>
#include "resource.h"
#include "afxwin.h"


// CChangePswdForm �Ի��� 

class CChangePswdForm : public CDialog
{
	DECLARE_DYNAMIC(CChangePswdForm)

public:
	CChangePswdForm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangePswdForm();

// �Ի�������
	enum { IDD = IDD_DLG_CHANGEPSWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	/// ���ȷ����ť
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedCancel();
public:	//�ؼ�
	CEdit m_edtCurPswd;
	CEdit m_edtNewPswd;
	CEdit m_edtNewPswd2;

	virtual BOOL OnInitDialog();
	CComboBox m_comPwdType;
	afx_msg void OnCbnSelchangeComboPwdtype();
};
