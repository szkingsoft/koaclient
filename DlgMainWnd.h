#pragma once
//#include "WDialog.h"
//#include "DlgMainClient.h"
#include "DlgMainClient.h"
// CDlgMainWnd �Ի���
//class CDlgMainClient;
//class CAnalisysObj;
class CDlgMainWnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainWnd)

public:
	CDlgMainWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMainWnd();

// �Ի�������
	enum { IDD = IDD_DLG_MAINWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CDlgMainClient m_DlgMainClient;

	void InitCtrl();
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
