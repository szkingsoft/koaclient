#pragma once
#include "afxwin.h"


// CDlgUpdateSelect �Ի���

class CDlgUpdateSelect : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpdateSelect)

public:
	CDlgUpdateSelect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUpdateSelect();

// �Ի�������
	enum { IDD = IDD_DIALOG_UPDATECHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comServer;
	CString   m_strSelectServer;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	map<CString,CString>* m_pMapInfo;
};
