#pragma once
#include "PubZYQ.h"
#include "translateStruct.h"
#include "HtmlShow.h"
#include "afxwin.h"
// CDlgTrendChart_D 对话框
//yfj
#define  EVRNT_TEST        1000
#define  EVRNT_TEST_TM     1000
#define  RAND_MARK(x)      ((x==1)?1:-1)

class CDlgTrendChart_D : public CDialog
{
	DECLARE_DYNAMIC(CDlgTrendChart_D)

public:
	CDlgTrendChart_D(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTrendChart_D();

// 对话框数据
	enum { IDD = IDD_DLG_TRENDCHART_D };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void OnUpdateUserCmd(CCmdUI *pCmdUI);
	afx_msg void OnComRange(UINT Iid);
private:
	void InitCtrl();
	void ReArrayWnd();
	CString DoubleToString( double dblValue,int nDec );
private:
	BOOL QuickKeyResponse(WPARAM wPar,LPARAM lPar);
	BOOL QuickKey_TrendChart(UINT nChar);
	BOOL QuickKey_ChangeWnd(UINT nChar);
	BOOL QuickKey_Filter(UINT nChar);
public:
	int GetHPos(float fValue,CRect rcDraw,float fMaxValue,float fMinValue);
	ChartDate * GetChartDate(){return &stChartDate;}
	void SetDrawLineCallBcakFun();

	BOOL GetXFromTime(INT64 uiTime,int &xPos);
	BOOL GetTimeFromX(int xPos,INT64 &uiTime);
	BOOL GetYFromValue(float fValue,int &yPos);
	BOOL GetValueFromY(int yPos,float &fValue);
private:
	///////////////
	BOOL InValid(float x);
	UINT GetLineType(int nGSType);
	void CalcIndexDate(KDate *pstKDate,CString strKey = "MA");

	void DrawLinkLine(Graphics * gpCach,int iXPos,UINT uiIndex,DWORD dwclr,CRect rcDraw,int iXFrontPos,float fEqlFront,float fEqlCur,int iKEveryWith);
	void DrawIndexDate(UINT iGpNum, UINT iHPos);
	void DrawBarLine(Graphics * gpCach,int iXPos,UINT uiIndex,CRect rcDraw,float fCurVale,int iKEveryWith);
	//////////////////////
	ChartDate stChartDate;
	QuoteLstDate m_stQuoteLstDate;
	CCriticalSection m_lockmapHisTTDate;
	CFormulaFace	m_thCalc;
	CHtmlShow m_dlgHtmlShow;

	BOOL m_bTips;
	CPoint m_ptCur;
	BOOL m_bMousePress;

	void GetMaxMinPrice(float &iMaxPrice,float &iMinPrice,float &iYsPrice);
	void GetMaxMinVol(float &iMaxVol,float &iMinVol,float &iPJVol);
	HisDateSpot GetCurCorsPos(CPoint ptCur);
	CString FormatTime(DWORD dwTm,int iAdd);
	void GetTradeTimeInfo(int& iQuFenNum,int &iAllTmF);

	UINT GetNextTime(DWORD dwTm,int iAdd,int iType=0);
	UINT GetFrontTime(DWORD dwTm,int iAdd,int iType=0);
	INT64 GetNextTime(INT64 dwTm);
	INT64 GetFrontTime(INT64 dwTm);
	INT64 GetNextTimeEx(INT64 dwTm,int iAdd);
	
	void CaluPos(CRect rcTT,CRect rcVOl);

	void DrawTrendChart(Graphics * gpCach,CRect rcDraw);
	void DrawTimeTrend(Graphics * gpCach,CRect rcDraw);
	void DrawTimeTrend_Min(Graphics * gpCach,CRect rcDraw);
	void DrawTimeTrend_RightRc(Graphics * gpCach,CRect rcDraw);
	void DrawTimeTrend_RightRc_TopBtn(Graphics * gpCach,CRect rcDraw);
	void DrawTimeTrend_RightRc_TopPart(Graphics * gpCach,CRect rcDraw);
	void DrawTimeTrend_RightRc_Tab(Graphics * gpCach,CRect rcDraw);
	void DrawDealDetails(Graphics * gpCach,CRect rcDraw);
	BOOL HitTest_RightRc_TopBtn(CPoint &ptCur,UINT uiHit);
	void AddTestTTTabDate();

	void AddTabMainChange();
	void DrawTimeTrend_MainChange_Tab(Graphics * gpCach,CRect rcDraw);
	BOOL HitTest_MainChangeTab(CPoint ptCur,TabInfo *pstTabInfo);
	
	void DrawBk(Graphics * gpCach,CRect rcFill,DWORD clrFill);
	CRect DrawTTCoordinate(Graphics * gpCach,CRect rcFill,int iType = 0);
	CRect DrawVOLCoordinate(Graphics * gpCach,CRect rcFill,int iType = 0);
	void DrawTTGp(Graphics * gpCach,CRect rcDraw);
	void DrawCorssTips(Graphics * gpCach);


	BOOL IsInTradeTime(INT64 iCurTime,int Type = 0);
	int m_iType;

	void GetMaxMinPrice_KMAP(float &iMaxPrice,float &iMinPrice,float &iYsPrice);
	void GetMaxMinVol_KMAP(float &iMaxVol,float &iMinVol,float &iPJVol);
	void GetMaxMinOther_KMAP(float &iMaxOther,float &iMinOther,float &iYsOther);
	HisUnitK_ZYQ GetKCurCorsPos(CPoint ptCur,int &iPos);
	int CompValue(float ValueS,float ValueD);

	void DrawKMap(Graphics * gpCach,CRect rcDraw);
	CRect DrawMACoordinate(Graphics * gpCach,CRect rcDraw);
	CRect DrawVOLCoordinate_K(Graphics * gpCach,CRect rcDraw);
	CRect DrawOtherCoordinate(Graphics * gpCach,CRect rcDraw);

	void CaluPos_K(CRect rcDraw);

	void DrawKGP(Graphics * gpCach,CRect rcMA,CRect rcVol,CRect rcOther,CRect rcTime);
	void DrawKLine(Graphics * gpCach,int iXPos,CRect rcDraw,HisUnitK_ZYQ *pstcHisUK,int iKEveryWith,BOOL &bMarkMax,BOOL &bMarkMin);
	void DrawKVOL(Graphics * gpCach,int iXPos,CRect rcDraw,HisUnitK_ZYQ *pstcHisUK,int iKEveryWith);
	void DrawKTime(Graphics * gpCach,int iXPos,CRect rcDraw,HisUnitK_ZYQ *pstcHisUK,int iKEveryWith);
	void DrawKCorssTips(Graphics * gpCach);
	void DrawKMianIndex(Graphics * gpCach,CRect rcDraw);
	MainIndex HitTestMianIndex(CPoint ptCur,TVecMainIndex *pvecMainIndex);
	MainIndex HitTestMianIndex(CString strName,TVecMainIndex *pvecMainIndex);
	///////
	void AddQuoteHead(UINT iMode = 0);
	void AddTestQuoteDate();
	void AddTestQuoteChangeDate(int iMark);
	void CaluQuteLstRect(CRect rcDraw);
	BOOL HitTestQuoteLstItemClick(CPoint ptCur,UINT &uiItem,UINT &uiSubItem);
	UINT HitTestQuoteLst(CPoint ptCur,UINT &uiHit);

	UINT CalculateString(CString strRes,int iFontSize );
	void DrawQuoteLst(Graphics * gpCach,CRect rcDraw);
	void DrawQuoteLstHead(Graphics * gpCach,CRect rcDraw);
	void DrawQuoteLstItem(Graphics * gpCach,CRect rcDraw);
	void DrawQuoteLstTab(Graphics * gpCach,CRect rcDraw);
	void DrawHScroll(Graphics * gpCach,CRect rcDraw,HScrollInfo *pstHScrollInfo);

	UINT ChangeDisplayMode(UINT iMode = 1);
	BOOL IsValidRect(Graphics * gpCach,CRect rcDraw);
	void DrawTab(Graphics * gpCach,CRect rcDraw,TabInfo *pstTabInfo);
	void CaluRectTab(Graphics * gpCach,CRect rcDraw,TabInfo *pstTabInfo);
	BOOL HitTestTab(CPoint ptCur,TabInfo *pstTabInfo);
	UINT HitTestHScroll(CPoint ptCur,HScrollInfo *pstHScrollInfo);
private:
	CSpotQuoteDate * m_pSpotQuoteDate;
public:
	//void ChangeNextTTSymbol();
	//void ChangeFrontTTSymbol();
	void ChangeSymbol(int iDir = 0);

	void TransInitMarket_ZYQ(vector<STUMarketInfo_ZYQ_Client> & vecMarketInfo);
	void TransReportDate_ZYQ(STUReportDataResp_ZYQ_Client &stReportDataResp_ZYQ_Client);
	void TransReportDetailDate_ZYQ(STUReportDataDetailResp_ZYQ_Client &stReportDataDetail);
	void TransDealDetailDate_ZYQ(STUTraceUnit_ZYQ_Client &stTraceUnit_ZYQ);
	void TransTimeTrendDate_ZYQ(STUSTKMinUnit_ZYQ_Client &stTKMinUnit);
	void TransKDate_ZYQ(STUHisKUnit_ZYQ_Client &stHisKUnit_ZYQ);

	void TransPushReportDate_ZYQ(STURealDataPush_ZYQ_Client &stPushReportDate);
	void TransPushReportDetailDate_ZYQ(STURealDataDetailPush_ZYQ_Client &stPushReportDetailDate);

	void TransOpenMarket_ZYQ(STUOpenMarket_ZYQ_Client &stOpenMarketDate);
	void TransCloseMarket_ZYQ(STUCloseMarket_ZYQ_Client &stCloseMarketDate);

	int IsValidSymbol(CString strSymbolCode);
	UINT GetUDColor(int iDesDate,int iStandardDate);
	UINT GetUDColorF(double fDiffValue, double fStandardValue = (0.000000001));
	pQuoteItemLstDate GetCurSelQuoteItem(int iItem);
	CString GetCurSelQuoteSubItem(int iItem,int iSubItem);
	CString GetCurSelQuoteSubItem(pQuoteItemLstDate pQuoteItme,int iSubItem);
	pQuoteItemLstDate GetNextQuoteItem();
	pQuoteItemLstDate GetFrontQuoteItem();

	float getMaxDate(float fDate1,float fDate2);
	float getMinDate(float fDate1,float fDate2);
	void SetQuoteItemText(UINT uiItem,UINT uiSubItem,CString strText,UINT iMode=0,double fStandardValue=0.0);
public:
	void ReqKDate_ZYQ(UINT uiReqCycle = MINUTE1_HISK,UINT uiHasNum = 0,UINT uiReqNum = 500);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnDataMessage(WPARAM wPar, LPARAM lPar);
	afx_msg LRESULT OnChangeTrendChart(WPARAM wPar, LPARAM lPar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNotifySendByTrade(WPARAM wPar, LPARAM lPar);
	afx_msg LRESULT OnKeyGhostMsg(WPARAM wPar, LPARAM lPar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
};
