#pragma once


// CDlgAgentSetting �Ի���

class CDlgAgentSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgAgentSetting)

public:
	CDlgAgentSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAgentSetting();

// �Ի�������
	enum { IDD = IDD_DLG_AGENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL m_nAgentMode;
	BOOL m_bUseAgent;
};
