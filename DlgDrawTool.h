#pragma once
#include "TabLst.h"


// CDlgDrawTool 对话框

class CDlgDrawTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgDrawTool)

public:
	CDlgDrawTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDrawTool();

// 对话框数据
	enum { IDD = IDD_DIG_DRAWTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ResetSel(int iSel);
private:

	void InitCtrl();
	CTabLst m_statDrawTool;
public:
	afx_msg void OnBnClickedBtnLine();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
