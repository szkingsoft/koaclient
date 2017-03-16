#pragma once
#include "HtmlShow.h"
#include "TranslucentBtn.h"

// CDlgShowUrl �Ի���

class CDlgShowUrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgShowUrl)

public:
	CDlgShowUrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgShowUrl();

// �Ի�������
	enum { IDD = IDD_DLG_SHOWURL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void ShowUrl(CString strUrl);
private:
	CTranslucentBtn m_btnMin;
	CTranslucentBtn m_btnMax;
	CTranslucentBtn m_btnClose;

	CHtmlShow m_dlgHtmlShow;
	void ReArrayWnd();
	void InitCtrl();
	void NotifyKOASys(UINT iMode);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnBnClickedBtnMax();
	afx_msg void OnBnClickedBtnClose();
};
