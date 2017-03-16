#pragma once


// CDlgAgentSetting 对话框

class CDlgAgentSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgAgentSetting)

public:
	CDlgAgentSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAgentSetting();

// 对话框数据
	enum { IDD = IDD_DLG_AGENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL m_nAgentMode;
	BOOL m_bUseAgent;
};
