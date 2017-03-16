#pragma once
#include "TabLst.h"


// CDlgDrawTool �Ի���

class CDlgDrawTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgDrawTool)

public:
	CDlgDrawTool(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDrawTool();

// �Ի�������
	enum { IDD = IDD_DIG_DRAWTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
