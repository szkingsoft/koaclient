// DlgTrendChartSpot.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgTrendChartSpot.h"


// CDlgTrendChartSpot 对话框

IMPLEMENT_DYNAMIC(CDlgTrendChartSpot, CDialog)

	CDlgTrendChartSpot::CDlgTrendChartSpot(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTrendChartSpot::IDD, pParent)
{
	m_oCodeInfo = NULL;
	m_iCheckId = 0;
	m_iCheckTool = -1;
	m_stcDrawToolBar.Init();
	m_vecStrBtn.clear();
	m_vecDrawToolBar.clear();
}

CDlgTrendChartSpot::~CDlgTrendChartSpot()
{
	if (m_oCodeInfo)
	{
		delete m_oCodeInfo;
		m_oCodeInfo = NULL;
	}
	if (m_stcDrawToolBar.pImg)
	{
		delete m_stcDrawToolBar.pImg;
		m_stcDrawToolBar.pImg = NULL;
	}
	ReleaseVec(m_vecStrBtn);
	ReleaseVec(m_vecDrawToolBar);
}

void CDlgTrendChartSpot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTrendChartSpot, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_STOCKCHANGE,OnStockChange)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	//	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	//	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CDlgTrendChart 消息处理程序


BOOL CDlgTrendChartSpot::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTrendChartSpot::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rcTotalRect(0,0,cx,cy);	
	m_AnalisysObj.MoveTo(&rcTotalRect);
	// TODO: 在此处添加消息处理程序代码

}
void CDlgTrendChartSpot::InitCtrl()
{
	CPublic::Instance()->RegistWnd(12,this,2);
	m_uiRedrawTimer = SetTimer(100,50,NULL);

	m_uiRequestTimer = SetTimer(101,500,NULL);//半秒一次
	m_bRequestCode = TRUE;

	m_oCodeInfo = new SCodeInfo;
	strcpy(m_oCodeInfo->szCode,"AG1"); //名称
	m_oCodeInfo->cPriceUnit = '2';//价格位数
	m_AnalisysObj.Initialization();
	m_AnalisysObj.SetAnalisysMode(0,FALSE);
	m_AnalisysObj.SetType(1);
	//pCode = CPublic::Instance()->GetDefaultStock();
	if(m_oCodeInfo)
	{
		//OnStockChange((WPARAM)m_oCodeInfo,RealObj);
	}

	m_AnalisysObj.SetStockInfo(m_oCodeInfo);

	InitBtnStr();
	InitDrawTool1();
	//InitDrawTool();
}
LRESULT CDlgTrendChartSpot::OnDataMessage(WPARAM wPar,LPARAM lPar)
{
	m_lockUpdateData.Lock();
	
	//if((int)lPar != 0 && m_AnalisysObj.UpdateData((LPVOID)wPar,(int)lPar))
	//{
	//	RedrawWindow();
	//}
	m_lockUpdateData.Unlock();
	return 1;
}
void CDlgTrendChartSpot::SetWndPos(CRect &rcWnd)
{
	if (IsWindow(GetSafeHwnd()))
	{
		this->SetWindowPos(NULL, rcWnd.left, rcWnd.top,
			rcWnd.Width(), rcWnd.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}	
}
void CDlgTrendChartSpot::TextGp(Graphics &gpCach)
{
	//SolidBrush brush2(Color(110,255,0,0));
	//gpCach.FillRectangle(&brush2,0,0,200,200);
	return;
	SolidBrush brush(Color::Red);
	Point points[] = {Point(100,0),Point(200,200),Point(0,200)};//三角形顶点
	PathGradientBrush pathbrush(points,3);//创建路径渐变画刷
	Color colors[] = {Color::Red,Color::Green,Color::Blue};//定义参与渐变的颜色

	float pos[] = {0.0f,0.4f,1.0f};//设置合成点的位置
	//设置渐变过渡色
	pathbrush.SetInterpolationColors(colors,pos,3);
	//填充区域
	gpCach.FillRectangle(&pathbrush,0,0,200,200);
	//PointF centerpoint = pathbrush.CenterPoint;
	//g.FillEllipse(brush, centerpoint.X - 5, centerpoint.Y - 5, 10, 10);
}
void CDlgTrendChartSpot::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 
	//CDialog::OnPaint();

	//CDC *pDC = dc.a//GetDC();
	COLORREF crBackground;
	//crBackground = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	//dc.SetBkColor(RGB(31,34,41)/*crBackground*/);

	CRect rcClt;
	GetClientRect(&rcClt);
	CalcRect(rcClt);
	//CMyMemDC memdc(&dc,&rcClt);
	//m_AnalisysObj.Draw(&dc); //绘制个股分析视图
	//ReleaseDC(pDC);

	Gdiplus::Bitmap cachBmp(rcClt.Width(),rcClt.Height());

	//Graphics gpCach(&cachBmp);
	Graphics graphicsDraw(dc.m_hDC);
	graphicsDraw.SetSmoothingMode(SmoothingModeAntiAlias);
	HDC hdcMemory = CreateCompatibleDC(dc.m_hDC);
	HBITMAP hMemBitmap;//  = CreateCompatibleBitmap(dc.m_hDC,rcClt.Width(),rcClt.Height());
	cachBmp.GetHBITMAP(Color(255,0,0,0)/*CDateManage::Instance()->GetBkColor()*/,&hMemBitmap);
	SelectObject(hdcMemory,hMemBitmap);
	Graphics gpCach(hdcMemory);
	gpCach.SetSmoothingMode(SmoothingModeAntiAlias);//SmoothingModeHighQuality SmoothingModeAntiAlias
	//CDateManage::Instance()->DrawBkRect(gpCach,rcClt);
	SolidBrush sbrh(Color(255,0,0,0));
	gpCach.FillRectangle(&sbrh,Rect(rcClt.left,rcClt.top,rcClt.Width(),rcClt.Height()));
	m_AnalisysObj.Draw(CDC::FromHandle(hdcMemory)); //绘制个股分析视图

	TextGp(gpCach);
	//Graphics graphics(&cachBmp);
	//HPALETTE hPal = NULL;
	//HPALETTE hPal1 = ::SelectPalette(hdcMemory, hPal, FALSE); 
	//Gdiplus::Bitmap *pImgDraw = Gdiplus::Bitmap::FromHBITMAP(hMemBitmap,NULL);
	//Graphics gpCach(hdcMemory);
	//gpCach.SetSmoothingMode(SmoothingModeAntiAlias);//SmoothingModeHighQuality SmoothingModeAntiAlias
	//graphicsDraw.SetSmoothingMode(SmoothingModeAntiAlias);

	//DrawToolBar(gpCach);
	//Pen penDr(Color(255,0,0,255),1.0);
	//gpCach.DrawRectangle(&penDr,20,20,300,300);
	//
	Gdiplus::Bitmap *pImgDraw = Gdiplus::Bitmap::FromHBITMAP(hMemBitmap,NULL);
	graphicsDraw.DrawImage(pImgDraw,0,0);
	graphicsDraw.ReleaseHDC(dc.m_hDC);
	delete pImgDraw;
	//BitBlt(dc.m_hDC,0,0,rcClt.Width(),rcClt.Height(),hdcMemory,0,0/*rcClt.Width(),rcClt.Height()*/,SRCCOPY);
	DeleteObject(hMemBitmap);
	DeleteDC(hdcMemory);
}
void CDlgTrendChartSpot::DrawToolBar(Graphics &gpCach)
{
	if (m_AnalisysObj.GetAnalisysMode() == TechObj)
	{
		DrawBtnStr(gpCach);
		DrawToolBar1(gpCach);
		CDateManage::Instance()->DrawToolBar(gpCach,m_stcDrawToolBar);
	}
}
void CDlgTrendChartSpot::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnTimer(nIDEvent);

	if((m_uiRedrawTimer == nIDEvent))
	{
		CPoint point;
		GetCursorPos(&point);
		if(m_pointCurStation != point)
		{
			m_pointCurStation = point;
			ScreenToClient(&point);//得到当前鼠标位置
			m_AnalisysObj.MouseMove(point);//) //响应个股分析视图对象			
		}
		Invalidate();
	}
	if(m_uiRedrawTimer == nIDEvent)
	{
		if(m_bRequestCode)
		{
			m_AnalisysObj.SetStockInfo(m_oCodeInfo);
			m_bRequestCode = FALSE;
		}
	}
}


BOOL CDlgTrendChartSpot::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rcClt;
	//GetClientRect(&rcClt);

	//CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}
LRESULT CDlgTrendChartSpot::OnStockChange(WPARAM wParam,LPARAM lParam)
{
	const STUSymbolInfo*pCodeInfo = (const STUSymbolInfo*)wParam;

	if(NULL == pCodeInfo || NULL  == lParam)return 0;

	SCodeInfo* pCode = GetCurStock();
	INT	uiPage = GetAnalisysMode();
	if(0 == strcmp(pCodeInfo->m_szSymbol,pCode->szCode)&&(uiPage == (unsigned int)lParam))
	{
		return 0;
	}
	strcpy(m_oCodeInfo->szCode,pCodeInfo->m_szSymbol);
	m_oCodeInfo->cPriceUnit = pCodeInfo->m_cPriceUnit;
	//memcpy(m_oCodeInfo,pCodeInfo,sizeof(SCodeInfo));
	m_bRequestCode = TRUE;
	//if(NULL != lParam)
	{
		int m_uiSubType = (unsigned int)lParam;	
		m_AnalisysObj.SetAnalisysMode(m_uiSubType==RealObj?0:1,FALSE);

		if (m_uiSubType == TechObj)m_iCheckId = ID_TECHANALY_1MINLINE;
	}
	return 1;
}

void CDlgTrendChartSpot::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
	STRBTN stcBtn = JudgePointIsBtnStr(point);
	if (stcBtn.iBtnId > 0)
	{
		m_iCheckId = stcBtn.iBtnId;
		//InvalidateRect();
		AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(stcBtn.iBtnId, 0),0);
		//stcStrBtn::iCheckId = stcBtn.iBtnId;
		return ;
	}
	stcBtn = JudgePointIsDrawToolBar1(point);
	if (stcBtn.iBtnId > 0)
	{
		m_iCheckTool = stcBtn.iBtnId;
		AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(stcBtn.iBtnId, 0),0);
		return ;
	}
	int iItem = JudgePointIsDrawToolBar(point);
	if (iItem >= 0)
	{
		m_stcDrawToolBar.iCurSel = iItem;
		AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(m_stcDrawToolBar.iStartId+m_stcDrawToolBar.iCurSel, 0),0);
		return ;
	}
	int iSel = m_AnalisysObj.MouseLBtnDown(point);
	if(iSel) //响应个股分析视图对象
	{
		if (m_AnalisysObj.GetAnalisysMode() == TechObj&&iSel == 2)
		{
			m_iCheckId = ID_TECHANALY_1MINLINE;
		}
		//Invalidate();

	}
}


void CDlgTrendChartSpot::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	do 
	{
		STRBTN stcBtn = JudgePointIsBtnStr(point);
		if (stcBtn.iBtnId > 0)
			break;
		stcBtn = JudgePointIsDrawToolBar1(point);
		if (stcBtn.iBtnId > 0)
			break;
		int iItem = JudgePointIsDrawToolBar(point);
		if (iItem >= 0)
			break;
		if(m_AnalisysObj.MouseLBtnUp(point)) //响应个股分析视图对象
		{
			m_iCheckTool = -1;
			m_stcDrawToolBar.iCurSel = -1;
			Invalidate();
			CWnd* pView = (CWnd*)(CPublic::Instance()->GetView(11,2));
			if (pView&&IsWindow(pView->GetSafeHwnd()))
			{
				::PostMessage(pView->GetSafeHwnd(),WM_UPDATEDRAWTOOL,0,0);
			}
		}
	} while (0);
	CDialog::OnLButtonUp(nFlags, point);
}


BOOL CDlgTrendChartSpot::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nPageNum = zDelta/120;
	if(nPageNum<0)
	{
		OnKeyDown(VK_NEXT,1,0);
	}
	else
	{
		OnKeyDown(VK_PRIOR,1,0);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlgTrendChartSpot::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_AnalisysObj.MouseMove(point);
	CDialog::OnMouseMove(nFlags, point);
}


void CDlgTrendChartSpot::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnRButtonDown(nFlags, point);
}


void CDlgTrendChartSpot::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnRButtonUp(nFlags, point);
}


void CDlgTrendChartSpot::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	m_AnalisysObj.MouseRBtnDown(this,point);
}
void CDlgTrendChartSpot::OnUserCmd(unsigned int uiCmd)
{
	//uiCmd = ID_DRAWLINE_RECTANGLE;
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
		{
			m_AnalisysObj.OnUserCmd(uiCmd);
			if (m_AnalisysObj.GetAnalisysMode() == TechObj&&uiCmd >= ID_TECHANALY_1MINLINE &&uiCmd <= ID_TECHANALY_MONLINE)
			{
				m_iCheckId = uiCmd;
			}
			if (ID_DRAWLINE_DELETE == uiCmd)
			{
				m_iCheckTool = -1;
				m_stcDrawToolBar.iCurSel = -1;
				CWnd* pView = (CWnd*)(CPublic::Instance()->GetView(11,2));
				if (pView&&IsWindow(pView->GetSafeHwnd()))
				{
					::PostMessage(pView->GetSafeHwnd(),WM_UPDATEDRAWTOOL,0,0);
				}
			}
		}	
		break;
	}	
}
void CDlgTrendChartSpot::OnUpdateUserCmd(CCmdUI *pCmdUI)
{
	m_AnalisysObj.OnUpdateUserCmd(pCmdUI);
}
void CDlgTrendChartSpot::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	if(nChar == VK_NEXT)
	{
		const SCodeInfo* pCode;
		pCode =CPublic::Instance()->GetNextStockInfo(m_oCodeInfo);
		OnStockChange((WPARAM)pCode,NULL);
	}
	else if(nChar == VK_PRIOR)
	{
		const SCodeInfo* pCode;
		pCode =CPublic::Instance()->GetPrevStockInfo(m_oCodeInfo);
		OnStockChange((WPARAM)pCode,NULL);
	}
	else
	{
		m_AnalisysObj.OnChar(nChar);
	}

}

void CDlgTrendChartSpot::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonDblClk(nFlags, point);
	if(m_AnalisysObj.HitTest(point))
	{
		m_AnalisysObj.MouseLBtnDbClick(point);
		return;
	}
}


//void CDlgTrendChart::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialog::OnChar(nChar, nRepCnt, nFlags);
//	if((nChar>='0')&&(nChar<='9') 
//		|| (nChar>='A')&&(nChar<='Z')
//		|| (nChar>='a')&&(nChar<='z'))
//	{
//		AfxGetMainWnd()->SendMessage(WM_CHAR, nChar,MAKELONG(0,nFlags));
//	}
//	else
//	{
//		m_AnalisysObj.OnChar(nChar);
//	}
//}


BOOL CDlgTrendChartSpot::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nChar = pMsg->wParam;
		if(nChar == VK_NEXT)
		{
			const SCodeInfo* pCode;
			pCode =CPublic::Instance()->GetNextStockInfo(m_oCodeInfo);
			OnStockChange((WPARAM)pCode,NULL);
		}
		else if(nChar == VK_PRIOR)
		{
			const SCodeInfo* pCode;
			pCode =CPublic::Instance()->GetPrevStockInfo(m_oCodeInfo);
			OnStockChange((WPARAM)pCode,NULL);
		}
		else
		{
			m_AnalisysObj.OnChar(nChar);
		}
	}
	else if (WM_CHAR == pMsg->message)
	{
		UINT nChar = pMsg->wParam;
		if((nChar>='0')&&(nChar<='9') 
			|| (nChar>='A')&&(nChar<='Z')
			|| (nChar>='a')&&(nChar<='z'))
		{
			AfxGetMainWnd()->PostMessage(WM_CHAR, nChar,MAKELONG(0,0));
		}
		else
		{
			m_AnalisysObj.OnChar(nChar);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CDlgTrendChartSpot::InitBtnStr()
{
	ReleaseVec(m_vecStrBtn);
	//m_vecStrBtn.clear();
	STRBTN stcStrBtn;
	int iNum = 11;
	int x = 0;
	for (int i = 0; i < iNum; i++)
	{
		x = iNum-i-1;
		stcStrBtn.Init();
		if (x == 0)stcStrBtn.strText = _T("1");
		if (x == 1)stcStrBtn.strText = _T("5");
		if (x == 2)stcStrBtn.strText = _T("10");
		if (x == 3)stcStrBtn.strText = _T("15");
		if (x == 4)stcStrBtn.strText = _T("30");
		if (x == 5)stcStrBtn.strText = _T("1h");
		if (x == 6)stcStrBtn.strText = _T("2h");
		if (x == 7)stcStrBtn.strText = _T("4h");
		if (x == 8)stcStrBtn.strText = _T("日");
		if (x == 9)stcStrBtn.strText = _T("周");
		if (x == 10)stcStrBtn.strText = _T("月");
		stcStrBtn.iBtnId = ID_TECHANALY_1MINLINE + x;
		stcStrBtn.clrText = Color::MakeARGB(255,255,0,0);
		stcStrBtn.iTextSize = 11;
		m_vecStrBtn.push_back(stcStrBtn);
	}
	m_iCheckId = -1;
}
void CDlgTrendChartSpot::InitDrawTool1()
{
	ReleaseVec(m_vecDrawToolBar);
	STRBTN stcStrBtn;
	int iNum = 7;
	CString strImgPath = CDateManage::Instance()->GetImgPath();
	strImgPath += _T("DrawTool\\");
	CString strTemp;
	USES_CONVERSION;
	for (int x = (iNum-1); x >= 0; x--)
	{
		stcStrBtn.Init();
		////
		strTemp.Format(_T("DrawTool%d.png"),x+1);
		strTemp = strImgPath + strTemp;
		stcStrBtn.pImg = Image::FromFile(T2W(strTemp));
		/////
		stcStrBtn.iBtnId = ID_DRAWLINE_STOP + x;
		//stcStrBtn.clrText = Color::MakeARGB(255,255,0,0);
		//stcStrBtn.iTextSize = 11;
		m_vecDrawToolBar.push_back(stcStrBtn);
	}
	m_iCheckTool = -1;
}
void CDlgTrendChartSpot::InitDrawTool()
{
	if (m_stcDrawToolBar.pImg)
	{
		delete m_stcDrawToolBar.pImg;
		m_stcDrawToolBar.pImg = NULL;
	}

	CString strImgPath = CDateManage::Instance()->GetImgPath();
	strImgPath += _T("DrawTool2.png");
	USES_CONVERSION;
	Gdiplus::Image *pImg = Image::FromFile(T2W(strImgPath));
	if (!pImg) return;

	m_stcDrawToolBar.iEveryHight = 14;


	m_stcDrawToolBar.iStartId = ID_DRAWLINE_STOP;
	m_stcDrawToolBar.iTotalBtn = 7;
	m_stcDrawToolBar.iCurSel = -1;
	m_stcDrawToolBar.iEveryWith = pImg->GetWidth()/m_stcDrawToolBar.iTotalBtn;
	m_stcDrawToolBar.pImg = pImg;
	//m_stcDrawToolBar.rcItem = ;

}
void CDlgTrendChartSpot::CalcRect(CRect &rcWnd)
{
	if (m_vecStrBtn.size() <= 0)return;
	int iItemWith = 22;
	int iItemHight = 14;
	int iOffRight = 10;
	int iGapEve = 3;
	int iGapBottom = 2;
	VECSTRBTN::iterator vIt;

	int i = 0;
	CRect rcItem;
	for (vIt = m_vecStrBtn.begin();vIt != m_vecStrBtn.end();vIt++)
	{
		rcItem = CRect(0,0,0,0);
		if (rcWnd.Width() > 50)
		{
			rcItem.right = rcWnd.right - iOffRight - (iItemWith + iGapEve)*i;
			rcItem.left = rcItem.right - iItemWith;
			rcItem.bottom = rcWnd.bottom - iGapBottom;
			rcItem.top = rcItem.bottom - iItemHight;
		}
		vIt->rcBtn = rcItem;
		i++;
	}
	iItemWith = 23;
	for (vIt = m_vecDrawToolBar.begin();vIt != m_vecDrawToolBar.end();vIt++)
	{
		rcItem = CRect(0,0,0,0);
		if (rcWnd.Width() > 50)
		{
			rcItem.right = rcWnd.right - iOffRight - (iItemWith + iGapEve)*i;
			rcItem.left = rcItem.right - iItemWith;
			rcItem.bottom = rcWnd.bottom - iGapBottom;
			rcItem.top = rcItem.bottom - iItemHight;
		}
		vIt->rcBtn = rcItem;
		i++;
	}

	if (m_stcDrawToolBar.iTotalBtn <= 0)return;
	rcItem = CRect(0,0,0,0);
	if (rcWnd.Width() > 50)
	{
		rcItem.right = rcWnd.right - iOffRight - (iItemWith + iGapEve)*i;
		rcItem.left = rcItem.right - m_stcDrawToolBar.iEveryWith*m_stcDrawToolBar.iTotalBtn;
		rcItem.bottom = rcWnd.bottom - iGapBottom;
		rcItem.top = rcItem.bottom - m_stcDrawToolBar.iEveryHight;
	}
	m_stcDrawToolBar.rcItem = rcItem;
}
void CDlgTrendChartSpot::DrawBtnStr(Graphics &gpCach)
{
	STRBTN stcStrBtn;
	VECSTRBTN::iterator vIt;
	CRect rcItem;
	for (vIt = m_vecStrBtn.begin();vIt != m_vecStrBtn.end();vIt++)
	{
		CDateManage::Instance()->DrawBtnByString(gpCach,*vIt,m_iCheckId);
	}
}
void CDlgTrendChartSpot::DrawToolBar1(Graphics &gpCach)
{
	VECSTRBTN::iterator vIt;
	CRect rcItem;
	for (vIt = m_vecDrawToolBar.begin();vIt != m_vecDrawToolBar.end();vIt++)
	{
		CDateManage::Instance()->DrawBtnByString(gpCach,*vIt,m_iCheckTool);
	}
}
STRBTN  CDlgTrendChartSpot::JudgePointIsBtnStr(CPoint ptPos)
{
	STRBTN stcRet;
	stcRet.Init();
	VECSTRBTN::iterator vIt;
	for (vIt = m_vecStrBtn.begin();vIt != m_vecStrBtn.end();vIt++)
	{
		if (vIt->rcBtn.PtInRect(ptPos))
		{
			return *vIt;
		}
	}

	return stcRet;
}
STRBTN CDlgTrendChartSpot::JudgePointIsDrawToolBar1(CPoint ptPos)
{
	STRBTN stcRet;
	stcRet.Init();
	VECSTRBTN::iterator vIt;
	for (vIt = m_vecDrawToolBar.begin();vIt != m_vecDrawToolBar.end();vIt++)
	{
		if (vIt->rcBtn.PtInRect(ptPos))
		{
			return *vIt;
		}
	}

	return stcRet;
}
int CDlgTrendChartSpot::JudgePointIsDrawToolBar(CPoint ptPos)
{
	int iRet = -1;
	do 
	{
		if (m_stcDrawToolBar.iTotalBtn <= 0) break;
		if (m_stcDrawToolBar.rcItem.PtInRect(ptPos))
		{
			iRet = (ptPos.x-m_stcDrawToolBar.rcItem.left)/m_stcDrawToolBar.iEveryWith;
		}
	} while (0);
	return iRet;
}
void CDlgTrendChartSpot::ReleaseVec(VECSTRBTN & vecRes)
{
	VECSTRBTN::iterator vIt;
	for (vIt = vecRes.begin();vIt != vecRes.end();vIt++)
	{
		if (vIt->pImg)
		{
			delete vIt->pImg;
			vIt->pImg = NULL;
		}
	}
	vecRes.clear();
}