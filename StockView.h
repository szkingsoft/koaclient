
// KOAClientView.h : CStockView 类的接口
//

#pragma once
#include "KOAClientDoc.h"
#include "analisysobj.h"
#include "DlgMainClient.h"
#include "DlgLockMain.h"

class CStockView : public CView
{
protected: // 仅从序列化创建
	CStockView();
	DECLARE_DYNCREATE(CStockView)

// 特性
public:
	CKOAClientDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void UpdateToolBarStatus();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CStockView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:	
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnDataMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStockChange(WPARAM wParam,LPARAM lParam);	
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

private:
	SCodeInfo    m_oCodeInfo; //个股信息结构	
	BOOL		 m_bRequestCode;//请求代码
	//CAnalisysObj  m_AnalisysObj;     //个股分析对象

	UINT		  m_uiRedrawTimer;	//重画句柄
	UINT		  m_uiRequestTimer;//重新请求数据句柄
	CPoint		  m_pointCurStation;//当前鼠标位置
	CRect		  m_TotalRect;		//整体大小

	int			  m_uiSubType;		//当前子标签

	CDlgMainClient m_DlgMainClient;

	CDlgLockMain m_dlgLockMain;
public:
	///菜单响应
	void		OnUserCmd(unsigned int uiCmd);
	void		OnUpdateUserCmd(CCmdUI *pCmdUI);
	void		ChangeMode(int nMode);
	SCodeInfo* GetCurStock();

	void SetFormulaCommand(char *pCommand);
	int GetAnalisysMode(){return 1;/*m_AnalisysObj.GetAnalisysMode();*/};
//	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnExitSizeMove();
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnLoginSuccess(WPARAM wPar,LPARAM lPar);
	afx_msg LRESULT OnLockMainClient(WPARAM wPar,LPARAM lPar);
};	

#ifndef _DEBUG  // KOAClientView.cpp 中的调试版本
inline CKOAClientDoc* CStockView::GetDocument() const
   { return reinterpret_cast<CKOAClientDoc*>(m_pDocument); }
#endif

