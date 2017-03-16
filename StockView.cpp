
// KOAClientView.cpp : CStockView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "KOAClient.h"
#endif

#include "KOAClientDoc.h"
#include "StockView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStockView

IMPLEMENT_DYNCREATE(CStockView, CView)

BEGIN_MESSAGE_MAP(CStockView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CStockView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(WM_STOCKCHANGE,OnStockChange)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
//	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_EXITSIZEMOVE()
//	ON_WM_SETCURSOR()
ON_MESSAGE(WM_NOTIFY_LOGIN_SUC,OnLoginSuccess)
ON_MESSAGE(WM_NOTIFY_LOCK,OnLockMainClient)
END_MESSAGE_MAP()

// CStockView 构造/析构

CStockView::CStockView()
{
	m_uiSubType = TechObj;
	memset(&m_oCodeInfo,0,sizeof(SCodeInfo));
}

CStockView::~CStockView()
{
}

BOOL CStockView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style   &= ~WS_BORDER;
	cs.style   &= ~WS_CAPTION;
	
	return CView::PreCreateWindow(cs);
	//cs.style &= WS_EX_CLIENTEDGE;
}

// CStockView 绘制

void CStockView::OnDraw(CDC* pDC)
{
	//CView::OnDraw(pDC);
	//COLORREF crBackground;
	//crBackground = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	//pDC->SetBkColor(crBackground);
	//CRect rcClt;
	//GetClientRect(&rcClt);

	////CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	//pDC->FillSolidRect(&rcClt,RGB(255,0,0));
	//CMyMemDC memdc(pDC,&m_TotalRect);
	//m_AnalisysObj.Draw(&memdc); //绘制个股分析视图
}


// CStockView 打印


void CStockView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CStockView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CStockView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CStockView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


void CStockView::OnContextMenu(CWnd*  pWnd , CPoint point)
{
	//m_AnalisysObj.MouseRBtnDown(this,point);
}


// CStockView 诊断

#ifdef _DEBUG
void CStockView::AssertValid() const
{
	CView::AssertValid();
}

void CStockView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKOAClientDoc* CStockView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKOAClientDoc)));
	return (CKOAClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CStockView 消息处理程序


void CStockView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CPublic::Instance()->RegistWnd(VIEWINFO_STOCK,this);
	//m_uiRedrawTimer = SetTimer(100,50,NULL);

	//m_uiRequestTimer = SetTimer(101,500,NULL);//半秒一次
	m_bRequestCode = TRUE;

	//m_AnalisysObj.Initialization();

	//ModifyStyleEx(WS_EX_CLIENTEDGE,   0,   TRUE);
	//this->ModifyStyle(0, WS_CLIPSIBLINGS);
	const SCodeInfo* pCode;
	pCode = CPublic::Instance()->GetDefaultStock();
	if(pCode)
	{
		OnStockChange((WPARAM)pCode,RealObj);
	}
}


void CStockView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_TotalRect = CRect (0,0,cx,cy);	
	//m_AnalisysObj.MoveTo(&m_TotalRect);

	CRect rectClient;
	GetClientRect(&rectClient);
	if (IsWindow(m_DlgMainClient.GetSafeHwnd()))
	{
		//m_DlgMainClient.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
		m_DlgMainClient.MoveWindow(rectClient);
		m_DlgMainClient.SetFocus();
	}
	if (IsWindow(m_dlgLockMain.GetSafeHwnd()))
	{
		m_dlgLockMain.MoveWindow(rectClient);
	}
}


void CStockView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CView::OnLButtonDblClk(nFlags, point);
// 	if(m_AnalisysObj.HitTest(point))
// 	{
// 		m_AnalisysObj.MouseLBtnDbClick(point);
// 		return;
// 	}
}


void CStockView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);
	//m_AnalisysObj.MouseMove(point);
// 	if(m_AnalisysObj.MouseMove(point))
// 		Invalidate();
}


void CStockView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);
// 	if(m_AnalisysObj.MouseLBtnDown(point)) //响应个股分析视图对象
// 	{
// 		Invalidate();
// 	}
}


void CStockView::OnLButtonUp(UINT nFlags, CPoint point)
{
	//if(m_AnalisysObj.MouseLBtnUp(point)) //响应个股分析视图对象
	//{
	//	Invalidate();
	//}
	CView::OnLButtonUp(nFlags, point);
}

LRESULT CStockView::OnDataMessage(WPARAM wParam,LPARAM lParam)
{
	//if(m_AnalisysObj.UpdateData((LPVOID)wParam,(int)lParam))
	//{
	//	RedrawWindow();
	//}
	return 1;
}

LRESULT CStockView::OnStockChange(WPARAM wParam,LPARAM lParam)
{
	const SCodeInfo* pCodeInfo = (const SCodeInfo*)wParam;

	if(NULL == pCodeInfo)return 0;
	memcpy(&m_oCodeInfo,pCodeInfo,sizeof(SCodeInfo));
	m_bRequestCode = TRUE;
	if(NULL != lParam)
	{
		m_uiSubType = (unsigned int)lParam;	
		//m_AnalisysObj.SetAnalisysMode(m_uiSubType==RealObj?0:1,FALSE);
	}
	return 1;
}

void CStockView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);
	if((m_uiRedrawTimer == nIDEvent))
	{
		CPoint point;
		GetCursorPos(&point);
		if(m_pointCurStation != point)
		{
			m_pointCurStation = point;
			ScreenToClient(&point);//得到当前鼠标位置
			//m_AnalisysObj.MouseMove(point);//) //响应个股分析视图对象			
		}
		Invalidate();
	}
	if(m_uiRedrawTimer == nIDEvent)
	{
		if(m_bRequestCode)
		{
			//m_AnalisysObj.SetStockInfo(&m_oCodeInfo);
			m_bRequestCode = FALSE;
		}
	}

	if (1025 == nIDEvent)
	{
		KillTimer(1025);
		if (IsWindow(m_DlgMainClient.GetSafeHwnd()))
		{
			m_DlgMainClient.ShowWindow(SW_HIDE);
		}
		if (IsWindow(m_dlgLockMain.GetSafeHwnd()))
		{
			m_dlgLockMain.ShowWindow(SW_SHOW);
		}	
	}
}

void CStockView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	//if(nChar == VK_NEXT)
	//{
	//	const SCodeInfo* pCode;
	//	pCode =CPublic::Instance()->GetNextStockInfo(&m_oCodeInfo);
	//	OnStockChange((WPARAM)pCode,NULL);
	//}
	//else if(nChar == VK_PRIOR)
	//{
	//	const SCodeInfo* pCode;
	//	pCode =CPublic::Instance()->GetPrevStockInfo(&m_oCodeInfo);
	//	OnStockChange((WPARAM)pCode,NULL);
	//}
	//else
	//{
	//	//m_AnalisysObj.OnChar(nChar);
	//}

}

void CStockView::OnDestroy()
{
	CPublic::Instance()->UnRegistWnd(VIEWINFO_STOCK,this);
	CView::OnDestroy();
}


BOOL CStockView::OnEraseBkgnd(CDC* pDC)
{
	//CRect rcClt;
	//GetClientRect(&rcClt);
	//Gdiplus::Bitmap cachBmp(rcClt.Width(),rcClt.Height());
	//Graphics graphicsDraw(pDC->m_hDC);
	//Graphics graphics(&cachBmp);
	//SolidBrush sbBk(Color(255,31,34,41));
	//graphics.FillRectangle(&sbBk,Rect(rcClt.left,rcClt.top,rcClt.Width(),rcClt.Height()));
	////Pen penBrd(Color(255,255,0,0));
	////graphics.DrawRectangle(&penBrd,Rect(rcClt.left,rcClt.top,rcClt.Width()-3,rcClt.Height()-1));
	//graphicsDraw.DrawImage(&cachBmp,0,0/*rcClt.top*/);
	//graphicsDraw.ReleaseHDC(pDC->m_hDC);
	CRect rcClt;
	GetClientRect(&rcClt);

	CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	//pDC->FillSolidRect(&rcClt,RGB(255,0,0));
	return TRUE;
	return TRUE;
//	return CView::OnEraseBkgnd(pDC);
}
void CStockView::OnUserCmd(unsigned int uiCmd)
{
	switch(uiCmd)
	{
	case ID_TECHANALY_PREVSTK:
		{
			OnKeyDown(VK_PRIOR,0,0);
		}
		break;
	case ID_TECHANALY_NEXTSTK:
		{
			OnKeyDown(VK_NEXT,0,0);
		}
		break;
	default:
		//m_AnalisysObj.OnUserCmd(uiCmd);
		break;
	}	
}

void CStockView::OnUpdateUserCmd(CCmdUI *pCmdUI)
{
	//m_AnalisysObj.OnUpdateUserCmd(pCmdUI);
}

void CStockView::ChangeMode(int nMode)
{
	//m_AnalisysObj.SetAnalisysMode(nMode);
}

SCodeInfo* CStockView::GetCurStock()
{
	return &m_oCodeInfo;
}

void CStockView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
}


BOOL CStockView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nPageNum = zDelta/120;
	if(nPageNum<0)
	{
		OnKeyDown(VK_NEXT,1,0);
	}
	else
	{
		OnKeyDown(VK_PRIOR,1,0);
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CStockView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnChar(nChar, nRepCnt, nFlags);
	if((nChar>='0')&&(nChar<='9') 
		|| (nChar>='A')&&(nChar<='Z')
		|| (nChar>='a')&&(nChar<='z'))
	{
		//AfxGetMainWnd()->SendMessage(WM_CHAR, nChar,MAKELONG(0,nFlags));
	}
	else
	{
		//m_AnalisysObj.OnChar(nChar);
	}
}
void CStockView::SetFormulaCommand(char *pCommand)
{
	//if( m_AnalisysObj.GetAnalisysMode() != TechObj)
	//{
	//	m_AnalisysObj.SetAnalisysMode(TechObj);
	//}
	//m_AnalisysObj.SetFormulaCommand(pCommand);
	//Invalidate();
}

int CStockView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	//CWnd *pWnd = AfxGetMainWnd();
	m_DlgMainClient.Create(CDlgMainClient::IDD,this);
	m_DlgMainClient.ShowWindow(SW_SHOW);

	m_dlgLockMain.Create(CDlgLockMain::IDD,this);
	m_dlgLockMain.ShowWindow(SW_HIDE);
	return 0;
}


void CStockView::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default

	CView::OnExitSizeMove();
}


//BOOL CStockView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	SetCursor(LoadCursor(NULL,IDC_SIZEWE)); 
//	return TRUE;;
//	return CView::OnSetCursor(pWnd, nHitTest, message);
//}

LRESULT CStockView::OnLoginSuccess(WPARAM wPar,LPARAM lPar)
{
	KillTimer(1025);
	//SetTimer(1025,(1000*60*30),NULL);

	return 1;
}
LRESULT CStockView::OnLockMainClient(WPARAM wPar,LPARAM lPar)
{
	int iLock = (int)wPar;
	if (iLock == 0)
	{
		CPublic::Instance()->SetLockMainClient(FALSE);
		if (IsWindow(m_DlgMainClient.GetSafeHwnd()))
		{
			m_DlgMainClient.ShowWindow(SW_SHOW);
		}
		if (IsWindow(m_dlgLockMain.GetSafeHwnd()))
		{
			m_dlgLockMain.ShowWindow(SW_HIDE);
		}
		//SetTimer(1025,(1000*60*30),NULL);
	}
	else if (iLock == 1)
	{
		CPublic::Instance()->SetLockMainClient(TRUE);
		KillTimer(1025);
		if (IsWindow(m_DlgMainClient.GetSafeHwnd()))
		{
			m_DlgMainClient.ShowWindow(SW_HIDE);
		}
		if (IsWindow(m_dlgLockMain.GetSafeHwnd()))
		{
			m_dlgLockMain.ShowWindow(SW_SHOW);
		}
	}
	
	return 1;
}

void CStockView::UpdateToolBarStatus()
{
	m_DlgMainClient.UpdateToolBarStatus();
}