#pragma once
#include "PriceListWnd.h"
#include "StatLst.h"

// CDlgPriceList dialog

class CDlgPriceList : public CDialog
{
	DECLARE_DYNAMIC(CDlgPriceList)

public:
	CDlgPriceList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPriceList();

// Dialog Data
	enum { IDD = IDD_DLG_PRICELIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	vector<TCodeRowInfo> m_vectCodeRowInfo;
	vector<SUpdateData>	m_vecNowData;
	vector<SUpdateData>	m_vecShowData;
	int m_nTotalLine;

	SCodeInfo * m_pCurCodeInfo;

	int m_iRightWith;
	int m_iWndReArrayType;

	CRect m_rcLeftHide;
	CRect m_rcRightHide;

	void DrawLeftHide(int iStat = 0);
	void DrawRightHide(int iStat = 0);
public:
	void InitCtrl();
	void SetWndPos(CRect &rcWnd);
	//CPriceListWnd	m_PriceListWnd;

	/////////////
	int GetCurSel(const SCodeInfo* pSel);
	void SetLstCurSel(const SCodeInfo* pSel);
	SCodeInfo* GetNextSelCodeInfo(const SCodeInfo* pCurrentStock,int iMode = 1);
	SCodeInfo* GetCurSelCodeInfo(int iItem,int iSubItem);
	CCriticalSection m_lock;
	CStatLst m_statLst;
	BOOL	UpdateData2(LPVOID lpData,int nDataLen);
	void TransData2Table();
	void UpdateNowData(const SUpdateData* pNow,tagCodeRowInfo &rtRowInfo,int nImage);
	void UpdateReport(const vector<TCodeRowInfo> &vecCodeRowInfo);
	void UpdateReportItem(int nDisplayIndex,int nIndex,const TCodeRowInfo &tCurRow,int iUpDateItem=0);
	bool FindStation(SCodeInfo* pCode,int& nStation);
	void UpdateRealData(const StuRecvQuoteInfo* pReal,SUpdateData &oUpdateData);
public:
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParm);
	afx_msg LRESULT OnClickItem(WPARAM wParam,LPARAM lParm);
	afx_msg LRESULT OnDbClickItem(WPARAM wParam,LPARAM lParm);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnModifyTitleItem(WPARAM wPar,LPARAM lPar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSkinChange(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnStockChange(WPARAM wPar,LPARAM lPar);
};
