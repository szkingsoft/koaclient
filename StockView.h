
// KOAClientView.h : CStockView ��Ľӿ�
//

#pragma once
#include "KOAClientDoc.h"
#include "analisysobj.h"
#include "DlgMainClient.h"
#include "DlgLockMain.h"

class CStockView : public CView
{
protected: // �������л�����
	CStockView();
	DECLARE_DYNCREATE(CStockView)

// ����
public:
	CKOAClientDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void UpdateToolBarStatus();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CStockView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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
	SCodeInfo    m_oCodeInfo; //������Ϣ�ṹ	
	BOOL		 m_bRequestCode;//�������
	//CAnalisysObj  m_AnalisysObj;     //���ɷ�������

	UINT		  m_uiRedrawTimer;	//�ػ����
	UINT		  m_uiRequestTimer;//�����������ݾ��
	CPoint		  m_pointCurStation;//��ǰ���λ��
	CRect		  m_TotalRect;		//�����С

	int			  m_uiSubType;		//��ǰ�ӱ�ǩ

	CDlgMainClient m_DlgMainClient;

	CDlgLockMain m_dlgLockMain;
public:
	///�˵���Ӧ
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

#ifndef _DEBUG  // KOAClientView.cpp �еĵ��԰汾
inline CKOAClientDoc* CStockView::GetDocument() const
   { return reinterpret_cast<CKOAClientDoc*>(m_pDocument); }
#endif

