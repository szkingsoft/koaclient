#pragma once
#include "afxwin.h"


// CDlgWarnInfo �Ի���

class CDlgWarnInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgWarnInfo)

public:
	CDlgWarnInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWarnInfo();

// �Ի�������
	enum { IDD = IDD_DLG_WARNINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
