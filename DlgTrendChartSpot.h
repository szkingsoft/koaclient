#pragma once
#include "analisysobj.h"

// CDlgTrendChartSpot 对话框

class CDlgTrendChartSpot : public CDialog
{
	DECLARE_DYNAMIC(CDlgTrendChartSpot)

public:
	CDlgTrendChartSpot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTrendChartSpot();

// 对话框数据
	enum { IDD = IDD_DLG_TRENDCHART_SPOT };

public:
	void SetWndPos(CRect &rcWnd);
	void OnUserCmd(unsigned int uiCmd);
	void OnUpdateUserCmd(CCmdUI *pCmdUI);
	//	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	int GetAnalisysMode(){return m_AnalisysObj.GetAnalisysMode();};
	SCodeInfo* GetCurStock()
	{
		return m_oCodeInfo;
	}

	void TextGp(Graphics &gpCach);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	UINT		  m_uiRedrawTimer;	
	UINT		  m_uiRequestTimer;
	CPoint		  m_pointCurStation;
	BOOL m_bRequestCode;
	SCodeInfo *m_oCodeInfo;
	CAnalisysObj  m_AnalisysObj;
	CCriticalSection m_lockUpdateData;

	VECSTRBTN  m_vecStrBtn;
	VECSTRBTN  m_vecDrawToolBar;
	int m_iCheckId;
	int m_iCheckTool;
	DRAWTOOLBAR m_stcDrawToolBar;
	void ReleaseVec(VECSTRBTN & vecRes);

	void InitCtrl();
	void InitBtnStr();
	void InitDrawTool();
	void InitDrawTool1();
	void CalcRect(CRect &rcWnd);
	void DrawBtnStr(Graphics &gpCach);
	STRBTN JudgePointIsBtnStr(CPoint ptPos);
	int JudgePointIsDrawToolBar(CPoint ptPos);
	STRBTN JudgePointIsDrawToolBar1(CPoint ptPos);

	void DrawToolBar(Graphics &gpCach);
	void DrawToolBar1(Graphics &gpCach);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnDataMessage(WPARAM wPar,LPARAM lPar);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnStockChange(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};