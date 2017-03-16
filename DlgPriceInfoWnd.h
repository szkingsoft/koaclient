#pragma once
#include "GraphBtn.h"
#include "BaseInfoItem.h"

// CDlgPriceInfoWnd 对话框

class CDlgPriceInfoWnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgPriceInfoWnd)

public:
	CDlgPriceInfoWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPriceInfoWnd();

// 对话框数据
	enum { IDD = IDD_DLG_PRICEINFOWND };
public:
	BOOL UpdateData(char* pData,int nDataLen);
	int GetLastHight(){return m_iLastHight;}
private:
	void InitCtrl();
	void ArrayQuoteCard();
	CGraphBtn* AddQuoteCard();
	void UpdateQuoteCardLst();
	void UpdateQuoteCardLstDate(const SNowData stcNowData,BOOL bAdd = FALSE,CString strName=_T(""));
	SCodeInfo* GetCurSelCodeInfo(CGraphBtn *pBtn);
	//CGraphBtn m_btnPriceInfo;

	int m_iLastHight;
	std::map<CString,CGraphBtn*> m_mapQuoteCard;
	std::vector<CBaseInfoItem> m_vecInfoItem;
	void ClearMap();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnClickRc(WPARAM wPar,LPARAM lPar);
	LRESULT OnDbClickRc(WPARAM wPar,LPARAM lPar);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
};
