#pragma once
//#include "WDialog.h"
//#include "DlgMainClient.h"
#include "DlgMainClient.h"
// CDlgMainWnd 对话框
//class CDlgMainClient;
//class CAnalisysObj;
class CDlgMainWnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainWnd)

public:
	CDlgMainWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMainWnd();

// 对话框数据
	enum { IDD = IDD_DLG_MAINWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CDlgMainClient m_DlgMainClient;

	void InitCtrl();
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
