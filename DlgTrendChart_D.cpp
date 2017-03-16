
// DlgTrendChart_D.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgTrendChart_D.h"
#include "inputkey.h"
#include "input.h"

CString strQuoteLstHeader1[] = {_T("编号"),_T("商品代码"),_T("商品名称"),_T("开盘价"),_T("最新价"),_T("现量"),
	_T("涨跌"),_T("幅度"),_T("最高价"),_T("最低价"),_T("昨收盘价"),_T("买价"),_T("买量"),_T("卖价"),_T("卖量"),
	_T("成交量"),_T("成交金额"),_T("均价"),_T("库存量"),_T("量比"),_T("委比")};
// CDlgTrendChart_D 对话框
///////////////
//////////////////////////////////////////////////////////////////////////
BOOL CDlgTrendChart_D::GetYFromValue(float fValue,int &yPos)
{
	CRect rcMa = stChartDate.stKDate.GetItemRect(0);
	float fMax,fMin,fMark;
	stChartDate.stKDate.GetMaxMinValue(fMax,fMin,fMark,0);

	yPos = GetHPos(fValue,rcMa,fMax,fMin);

	return TRUE;
}
int CALLBACK  GetYFromValue_ZYQ(double dValue)    //业务Y－>绘图Y
{
	DWORD dwBG = GetTickCount();
	CDlgTrendChart_D * pdlgTC = (CDlgTrendChart_D *)CPublic::Instance()->GetView(13,2);

	int iYPos = 0;
	pdlgTC->GetYFromValue(dValue,iYPos);
	return iYPos;
}
BOOL CDlgTrendChart_D::GetXFromTime(INT64 uiTime,int &xPos)
{
	CRect rcMa = stChartDate.stKDate.GetItemRect(0);
	///////////
	TMapT_N::iterator itMap = stChartDate.stKDate.mapT_N.find(uiTime);
	BOOL bRet = FALSE;
	do 
	{
		if (itMap != stChartDate.stKDate.mapT_N.end())
		{
			xPos = rcMa.left+itMap->second*stChartDate.stKDate.fKEveryWithAll;
			bRet = TRUE;
		}
	} while (0);

	return bRet;
}
int CALLBACK  GetXFromTime_ZYQ(CTime time)        //业务X－>绘图X
{
	DWORD dwBG = GetTickCount();
	CDlgTrendChart_D * pdlgTC = (CDlgTrendChart_D *)CPublic::Instance()->GetView(13,2);
	int iRet = 0;
	pdlgTC->GetXFromTime(time.GetTime(),iRet);

	return iRet;
}

BOOL CDlgTrendChart_D::GetValueFromY(int yPos,float &fValue)
{
	CRect rcMa = stChartDate.stKDate.GetItemRect(0);
	BOOL bRet = FALSE;
	do 
	{
		if((yPos >rcMa.bottom)||(yPos<rcMa.top))break;
		if (rcMa.Height() > 0)
		{
			float fMax,fMin,fMark;
			stChartDate.stKDate.GetMaxMinValue(fMax,fMin,fMark,0);
			fValue = fMin + (fMax- fMin)*(((float)(rcMa.bottom - yPos))/rcMa.Height());
			bRet = TRUE;
		}
	} while (0);

	return bRet;
}
double CALLBACK GetValueFromY_ZYQ(int y)         //绘图 Y－>业务Y
{
	DWORD dwBG = GetTickCount();
	CDlgTrendChart_D * pdlgTC = (CDlgTrendChart_D *)CPublic::Instance()->GetView(13,2);
	float fValue = 0;
	pdlgTC->GetValueFromY(y,fValue);
	return fValue;
}

BOOL CDlgTrendChart_D::GetTimeFromX(int xPos,INT64 &uiTime)
{
	CRect rcMa = stChartDate.stKDate.GetItemRect(0);
	uiTime = 0;
	BOOL bRet = FALSE;
	int iNum = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	TmapHisUnitK_ZYQ::iterator itMap;
	do 
	{
		if (xPos < rcMa.left || xPos> rcMa.right)break;
		if (stChartDate.stKDate.iDisplayNum <= 0 || stChartDate.stKDate.uiCurBeginTm <= 0)break;
		if ( iNum<= 0)break;

		int iIndex = (int)((xPos - rcMa.left)/stChartDate.stKDate.fKEveryWithAll);
		if (iIndex >= stChartDate.stKDate.iDisplayNum)
		{
			iIndex = (stChartDate.stKDate.iDisplayNum - 1);
		}
		itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(stChartDate.stKDate.pHPDate[iIndex].uiDate);
		if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end())
		{
			uiTime = itMap->second.uiDate;
			bRet = TRUE;
		}
	} while (0);
	return bRet;
}
CTime CALLBACK GetTimeFromX_ZYQ(int x)           //绘图 X－>业务X
{
	DWORD dwBG = GetTickCount();
	CDlgTrendChart_D * pdlgTC = (CDlgTrendChart_D *)CPublic::Instance()->GetView(13,2);
	INT64 uiRet = 0;
	pdlgTC->GetTimeFromX(x,uiRet);

	return uiRet;
}
void CALLBACK   GetKLineRect_ZYQ(CRect &rect)      
{
	DWORD dwBG = GetTickCount();
	CDlgTrendChart_D * pdlgTC = (CDlgTrendChart_D *)CPublic::Instance()->GetView(13,2);
	rect = pdlgTC->GetChartDate()->stKDate.GetItemRect(0);
}
double CALLBACK GetUnitLength_ZYQ()
{
	CDlgTrendChart_D * pdlgTC = (CDlgTrendChart_D *)CPublic::Instance()->GetView(13,2);
	double dbRet = pdlgTC->GetChartDate()->stKDate.fKEveryWithAll;
	return dbRet;
}

BOOL CALLBACK  GetHighLowFromTime_ZYQ(const CTime & time,double &nHigh,double &nLow)
{
	DWORD dwBG = GetTickCount();
	BOOL bRet = FALSE;
	nHigh = nLow = 0.0;
	CDlgTrendChart_D * pdlgTC = (CDlgTrendChart_D *)CPublic::Instance()->GetView(13,2);
	ChartDate *pstChartDate = pdlgTC->GetChartDate();
	TmapHisUnitK_ZYQ::iterator itMap;
	INT64 uiTime = time.GetTime();
	itMap = pstChartDate->stKDate.mapHisUnitK_ZYQ.find(uiTime);
	if (itMap != pstChartDate->stKDate.mapHisUnitK_ZYQ.end())
	{
		nHigh = itMap->second.fMaxPrice;
		nLow = itMap->second.fMinPrice;
	}
	return bRet;
}

void CALLBACK   GetCloseFromXSgmt_ZYQ( const int &uBeginX,const int &uEndX, CArray< CPoint,CPoint> &arPtClose )
{
	return;
}
BOOL CALLBACK   GetHighLowFromXSgmt_ZYQ( const UINT uBeginX, const UINT uEndX, CPoint &ptHigh, CPoint &ptLow )//获得X方向范围内的最高最低价对应的坐标点
{
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
////////////////////////

IMPLEMENT_DYNAMIC(CDlgTrendChart_D, CDialog)

CDlgTrendChart_D::CDlgTrendChart_D(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTrendChart_D::IDD, pParent)
{
	//m_pstcCSymbolData = NULL;
	m_bTips = FALSE;
	m_iType = 1;
	m_bMousePress = FALSE;
	m_ptCur = CPoint(0,0);

	m_pSpotQuoteDate = NULL;
	m_pSpotQuoteDate = new CSpotQuoteDate;
}

CDlgTrendChart_D::~CDlgTrendChart_D()
{
	if (m_pSpotQuoteDate)
	{
		delete m_pSpotQuoteDate;
		m_pSpotQuoteDate = NULL;
	}

	UnregisterHotKey(this->GetSafeHwnd(), VK_F10);
}

void CDlgTrendChart_D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTrendChart_D, CDialog)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(WM_CHANGTRENDCHART,OnChangeTrendChart)
	ON_MESSAGE(WM_NOTIFY_SEND,OnNotifySendByTrade)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	//ON_COMMAND(ID_, &CDlgOrdersList::OnDelSpotOrders)
	ON_MESSAGE(WM_KEYGHOSTMSG,OnKeyGhostMsg)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ANALYSIST_BEGIN,ID_ANALYSIST_END,OnUpdateUserCmd)
	ON_COMMAND_RANGE(ID_ANALYSIST_BEGIN,ID_ANALYSIST_END,OnComRange)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgTrendChart_D 消息处理程序


BOOL CDlgTrendChart_D::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTrendChart_D::InitCtrl()
{
	CPublic::Instance()->RegistWnd(13,this,2);

	stChartDate.Clear();
	m_stQuoteLstDate.Clear();
	AddTestQuoteDate();
	AddTestTTTabDate();

	stChartDate.stKDate.vecIndexDisplay.push_back("MA");
	stChartDate.stKDate.vecIndexDisplay.push_back("VOL");
	stChartDate.stKDate.vecIndexDisplay.push_back("MACD");//RSI

	MainIndex stMainIndex;
	stMainIndex.iIndexType = 0;
	stMainIndex.uiIndexID = 0;
	stMainIndex.strIndexName = "指标";
	stChartDate.stKDate.vecMainIndex.push_back(stMainIndex);
	stMainIndex.iIndexType = 1;
	stMainIndex.uiIndexID = 101;
	stMainIndex.strIndexName = "MACD";
	stChartDate.stKDate.vecMainIndex.push_back(stMainIndex);
	stMainIndex.iIndexType = 1;
	stMainIndex.uiIndexID = 102;
	stMainIndex.strIndexName = "RSI";
	stChartDate.stKDate.vecMainIndex.push_back(stMainIndex);
	stMainIndex.iIndexType = 1;
	stMainIndex.uiIndexID = 103;
	stMainIndex.strIndexName = "DMI";
	stChartDate.stKDate.vecMainIndex.push_back(stMainIndex);
	stMainIndex.iIndexType = 1;
	stMainIndex.uiIndexID = 104;
	stMainIndex.strIndexName = "WR";
	stChartDate.stKDate.vecMainIndex.push_back(stMainIndex);
	stMainIndex.iIndexType = 1;
	stMainIndex.uiIndexID = 105;
	stMainIndex.strIndexName = "KDJ";
	stChartDate.stKDate.vecMainIndex.push_back(stMainIndex);

	stChartDate.stKDate.uiCurSelIndex = 101;
	if (m_pSpotQuoteDate)
	{
		m_pSpotQuoteDate->SetHwnd(this->GetSafeHwnd());
		CPublic::Instance()->GetTradeDll()->m_pRegQuoteDateChannel(m_pSpotQuoteDate,this);
	}

	m_dlgHtmlShow.CreateShowWnd(0x1111,this);
	m_dlgHtmlShow.Navigate(_T("about:blank"), NULL, NULL);
	m_dlgHtmlShow.ShowWindow(SW_HIDE);

	AddTabMainChange();
}
void CDlgTrendChart_D::SetDrawLineCallBcakFun()
{
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetYFromValue)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetYFromValue(&GetYFromValue_ZYQ);
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetXFromTime)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetXFromTime(&GetXFromTime_ZYQ);
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetValueFromY)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetValueFromY(&GetValueFromY_ZYQ);
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetTimeFromX)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetTimeFromX(&GetTimeFromX_ZYQ);
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetUnitLength)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetUnitLength(&GetUnitLength_ZYQ);
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetKLineRect)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetKLineRect(&GetKLineRect_ZYQ);
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetHighLowFromTime)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetHighLowFromTime(&GetHighLowFromTime_ZYQ);
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetCloseFromXSgmt)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetCloseFromXSgmt(&GetCloseFromXSgmt_ZYQ);
	if (CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetHighLowFromXSgmt)
		CPublic::Instance()->GetDrawDll()->m_pSetCallbakGetHighLowFromXSgmt(&GetHighLowFromXSgmt_ZYQ);	

	if (!stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.IsEmpty())
	{
		USES_CONVERSION;
		CPublic::Instance()->GetDrawDll()->m_pSetCurSymbol(T2W(stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode));
	}
}
void CDlgTrendChart_D::ReArrayWnd()
{
	CRect rcClt;
	GetClientRect(&rcClt);
	if (IsWindow(m_dlgHtmlShow.GetSafeHwnd()))
	{
		m_dlgHtmlShow.MoveWindow(rcClt);
	}
}
UINT CDlgTrendChart_D::GetFrontTime(DWORD dwTm,int iAdd,int iType)
{
	UINT iRet = dwTm;
	do 
	{
		int iHour = (dwTm/10000);
		int iM= (dwTm%10000)/100;
		if (iType == 0)
		{
			if (iM >= 30)
			{
				if (iAdd%2==0)
				{
					iHour += iAdd/2;
				}
				else
				{
					iHour += (iAdd/2 + 1);
					iM = 0;
				}
			}
			else
			{
				if (iAdd%2==0)
				{
					iHour += iAdd/2;
				}
				else
				{
					iHour += (iAdd/2);
					iM = 30;
				}
			}
			iRet = iHour*10000 + iM*100;
		}
		else
		{
			iM -= iAdd;
			while(iM < 0)
			{
				iM = 60 + iM;
				iHour -= 1;
			}
			iRet = iHour*10000 + iM*100;
		}
	} while (0);
	return iRet;
}
INT64 CDlgTrendChart_D::GetNextTimeEx(INT64 dwTm,int iAdd)
{
	return 0;
}
UINT CDlgTrendChart_D::GetNextTime(DWORD dwTm,int iAdd,int iType)
{
	UINT iRet = dwTm;
	do 
	{
		int iHour = (dwTm/10000);
		int iM= (dwTm%10000)/100;
		if (iType == 0)
		{
			if (iM >= 30)
			{
				if (iAdd%2==0)
				{
					iHour += iAdd/2;
				}
				else
				{
					iHour += (iAdd/2 + 1);
					iM = 0;
				}
			}
			else
			{
				if (iAdd%2==0)
				{
					iHour += iAdd/2;
				}
				else
				{
					iHour += (iAdd/2);
					iM = 30;
				}
			}
			iRet = iHour*10000 + iM*100;
		}
		else
		{
			iM += iAdd;
			while(iM >= 60)
			{
				iM -= 60;
				iHour += 1;
			}
			iRet = iHour*10000 + iM*100;
		}
	} while (0);
	return iRet;
}
CString CDlgTrendChart_D::FormatTime(DWORD dwTm,int iAdd)
{
	CString strRet = _T("");
	do 
	{
		int iHour = (dwTm/10000);
		int iM= (dwTm%10000)/100;
		if (iM >= 30)
		{
			if (iAdd%2==0)
			{
				iHour += iAdd/2;
			}
			else
			{
				iHour += (iAdd/2 + 1);
				iM = 0;
			}
		}
		else
		{
			if (iAdd%2==0)
			{
				iHour += iAdd/2;
			}
			else
			{
				iHour += (iAdd/2);
				iM = 30;
			}
		}
		CString strTemp=_T("");
		((iHour < 10)?strTemp.Format(_T("0%d"),iHour):strTemp.Format(_T("%d"),iHour));
		strRet += strTemp;
		strRet += _T(":");
		((iM < 10)?strTemp.Format(_T("0%d"),iM):strTemp.Format(_T("%d"),iM));
		strRet += strTemp;
	} while (0);

	return strRet;
}
void CDlgTrendChart_D::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	DWORD dwBG = GetTickCount();
	CRect rcDraw,rcClt;
	GetClientRect(&rcClt);
	rcDraw = rcClt;
	HDC hMemDc = CreateCompatibleDC(dc.m_hDC);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(dc.m_hDC, rcDraw.Width(), rcDraw.Height());
	SelectObject(hMemDc,hMemBitmap);
	Graphics gpDraw(hMemDc);
	//gpDraw.SetSmoothingMode(/*SmoothingModeHighQuality*/SmoothingModeAntiAlias);

	
	dwBG = GetTickCount();

	DrawTrendChart(&gpDraw,rcDraw);

	
	dwBG = GetTickCount();

	if (m_iType == 3)
	{
		CDC *pMemDc = CDC::FromHandle(hMemDc);
		//CMyMemDC memdc(&dc,&rcClt);
		
		dwBG = GetTickCount();

		if(CPublic::Instance()->GetDrawDll()->m_pDraw)
			CPublic::Instance()->GetDrawDll()->m_pDraw(pMemDc);
	}
	
	dwBG = GetTickCount();

	::BitBlt(dc.m_hDC,0,0,rcDraw.Width(),rcDraw.Height(),
		hMemDc,0,0,SRCCOPY);
	
	dwBG = GetTickCount();

	DeleteObject(hMemBitmap);
	DeleteDC(hMemDc);
	
	
}

void CDlgTrendChart_D::DrawTimeTrend_Min(Graphics * gpCach,CRect rcDraw)
{
	//DrawBk(gpCach,rcDraw,Color::MakeARGB(255,0,0,0));
	//Pen pnRc(Color(255,255,0,0));
	//rcDraw.InflateRect(0,0,-5,-5);
	//gpCach->DrawRectangle(&pnRc,Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height()));

	//m_lockmapHisTTDate.Lock();
	CRect rcTT,rcVol;
	int iVolHi = 80;
	if (rcDraw.Height() < 140)
	{
		iVolHi = 0;
	}

	rcTT = rcDraw;
	rcTT.top += 5;
	rcTT.bottom -= iVolHi;
	//rcTT.right -= iRWith;
	rcTT = DrawTTCoordinate(gpCach,rcTT,1);

	rcVol = rcDraw;
	rcVol.top = rcTT.bottom;
	rcVol.bottom = (iVolHi==0)?rcVol.top:rcDraw.bottom;
	rcVol = DrawVOLCoordinate(gpCach,rcVol,1);

	CaluPos(rcTT,rcVol);
	DrawTTGp(gpCach,rcVol);
	//m_lockmapHisTTDate.Unlock();
}
void CDlgTrendChart_D::DrawTrendChart(Graphics * gpCach,CRect rcDraw)
{
	DWORD dwBG = GetTickCount();
	DrawBk(gpCach,rcDraw,Color::MakeARGB(255,0,0,0));

	
	int iRWith = 250;
	int iMTabH = 25;
	m_lockmapHisTTDate.Lock();
	CRect rcTT,rcView;
	rcView = rcTT = rcDraw;
	rcView.bottom -= iMTabH;
	rcView.right -= iRWith;
	rcTT.right -= iRWith;
	do 
	{
		BOOL bDrawRight = FALSE;
		if (m_iType == 2)
		{
			dwBG = GetTickCount();
			DrawTimeTrend(gpCach,rcView/*rcTT*/);
			bDrawRight = TRUE;

			
		}
		else if(m_iType == 3)
		{
			dwBG = GetTickCount();
			DrawKMap(gpCach,rcView/*rcTT*/);
			bDrawRight = TRUE;
			
		}
		else if(m_iType == 1)
		{
			dwBG = GetTickCount();
			DrawQuoteLst(gpCach,rcDraw);
			
		}
		
		if (bDrawRight)
		{
			dwBG = GetTickCount();
			Pen pnLine(Color(255,255,0,0),1.0);
			gpCach->DrawLine(&pnLine,rcDraw.left + 10,rcDraw.bottom - 5 - iMTabH,rcDraw.right - 5,rcDraw.bottom - 5 - iMTabH);

			CRect rcRight;
			rcRight.left = rcDraw.right - iRWith;
			rcRight.right = rcDraw.right;
			rcRight.top = rcDraw.top;
			rcRight.bottom = rcDraw.bottom - iMTabH;
			DrawTimeTrend_RightRc(gpCach,rcRight);

			

			CRect rcTab;
			rcTab.left = rcDraw.left + 10;
			rcTab.right = rcDraw.right - 5;
			rcTab.top = rcDraw.bottom - 4 - iMTabH;
			rcTab.bottom = rcDraw.bottom-10;
			stChartDate.stTabMain.rcTab = rcTab;

			gpCach->SetClip(Rect(rcTab.left,rcTab.top,rcTab.Width(),rcTab.Height()+2));
			SolidBrush sldrsh(Color(255,0,0,0));
			gpCach->FillRectangle(&sldrsh,Rect(rcTab.left,rcTab.top,rcTab.Width(),rcTab.Height()));
			DrawTimeTrend_MainChange_Tab(gpCach,rcTab);
			gpCach->ResetClip();
		}
	} while (0);

	m_lockmapHisTTDate.Unlock();
}
void CDlgTrendChart_D::DrawKMap(Graphics * gpCach,CRect rcDraw)
{
	//if (stChartDate.stKDate.mapHisUnitK_ZYQ.size()<=0)return;
	DWORD dwBG = GetTickCount();
	DWORD dwBG1 = GetTickCount();
	CRect rcDrawGp = rcDraw;
	int iOffL = 80;
	int iOffT = 5;
	int iOffR = 3;
	int iOffB = 25;
	int iOffBt = 15;
	iOffB += iOffBt;

	rcDrawGp.InflateRect(-iOffL,-iOffT,-iOffR,-iOffB);

	if (rcDrawGp.Width()<= 0 || rcDrawGp.Height() <= 0||
		!gpCach->IsVisible(Rect(rcDrawGp.left,rcDrawGp.top,rcDrawGp.Width(),rcDrawGp.Height())))return;
	Pen pnBord(Color(255,255,0,0),1.0);
	gpCach->DrawRectangle(&pnBord,Rect(rcDrawGp.left,rcDrawGp.top,rcDrawGp.Width(),rcDrawGp.Height()));

	//GetMaxMinPrice_KMAP(stChartDate.stKDate.fMaxPrice_K,stChartDate.stKDate.fMinPrice_K,stChartDate.stKDate.fYsPrice_K);
	CRect rcTemp = rcDrawGp;
	int iOffTmL = 3;
	int iOffTmR = 2;
	rcTemp.InflateRect(-iOffTmL,0,-iOffTmR,0);
	CaluPos_K(rcTemp);

	
	dwBG = GetTickCount();
	//m_rcAll_K = rcDrawGp;
	stChartDate.stKDate.rcAll_K = rcDrawGp;
	CRect rcMA,rcVOL_K,rcOther,rcTime,rcTips;
	rcMA = rcDrawGp;
	rcMA.bottom = rcDrawGp.top + rcDrawGp.Height()/2;
	rcMA = DrawMACoordinate(gpCach,rcMA);

	
	dwBG = GetTickCount();
	//stChartDate.stKDate.rcKTrend = rcMA;
	stChartDate.stKDate.SetItemRect(rcMA,0);

	rcVOL_K = rcDrawGp;
	rcVOL_K.top = rcMA.bottom;
	rcVOL_K.bottom = rcVOL_K.top + rcDrawGp.Height()/4;
	rcVOL_K = DrawVOLCoordinate_K(gpCach,rcVOL_K);

	
	dwBG = GetTickCount();
	//stChartDate.stKDate.rcVOLTrend = rcVOL_K;
	stChartDate.stKDate.SetItemRect(rcVOL_K,1);

	rcOther = rcDrawGp;
	rcOther.top = rcVOL_K.bottom;
	rcOther.bottom = rcDrawGp.bottom;
	rcOther = DrawOtherCoordinate(gpCach,rcOther);

	
	dwBG = GetTickCount();
	//stChartDate.stKDate.rcOther = rcOther;
	stChartDate.stKDate.SetItemRect(rcOther,2);

	rcTime = rcOther;
	rcTime.top = rcDrawGp.bottom;
	rcTime.bottom = rcTime.top + 18;
	rcTime.InflateRect(-iOffTmL,0,-iOffTmR,0);
	stChartDate.stKDate.rcTime = rcTime;

	rcTips = rcDraw;
	rcTips.right = rcMA.left-1;
	rcTips.left += 5;
	//rcTips.top = rcMA.top;
	stChartDate.stKDate.rcTips = rcTips;

	//
	
	dwBG = GetTickCount();
	DrawKGP(gpCach,rcMA,rcVOL_K,rcOther,rcTime);

	
	dwBG = GetTickCount();

	DrawKCorssTips(gpCach);

	
	dwBG = GetTickCount();

	CRect rcBottom;
	rcBottom = rcDrawGp;
	rcBottom.top = rcTime.bottom;
	rcBottom.bottom = rcDraw.bottom - 4;
	stChartDate.stKDate.rcIndex = rcBottom;

	DrawKMianIndex(gpCach,rcBottom);

	

	
	//dwBG = GetTickCount();
	//Pen pntm(Color(255,255,255,255),1.0);
	//gpCach->DrawRectangle(&pntm,RectF(rcBottom.left,rcBottom.top,rcBottom.Width(),rcBottom.Height()));
}
void CDlgTrendChart_D::DrawKMianIndex(Graphics * gpCach,CRect rcDraw)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0 ||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return;

	int iNum = stChartDate.stKDate.vecMainIndex.size();
	if (iNum <= 0)return;
	Pen pnGap(Color(255,255,0,0));
	gpCach->DrawLine(&pnGap,PointF(rcDraw.left,rcDraw.top),PointF(rcDraw.right,rcDraw.top));
	gpCach->DrawLine(&pnGap,PointF(rcDraw.left,rcDraw.top),PointF(rcDraw.left,rcDraw.bottom));
	gpCach->DrawLine(&pnGap,PointF(rcDraw.right,rcDraw.top),PointF(rcDraw.right,rcDraw.bottom));

	USES_CONVERSION;
	SolidBrush sbrFont(Color(255,233,233,233));
	SolidBrush sbrFill(Color(255,123,123,123));
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat formatC;
	formatC.SetAlignment(StringAlignmentCenter);
	int iStrWith = 0;
	int iOffLeft = rcDraw.left;
	CRect rcItem;
	rcItem = rcDraw;
	for (int i = 0; i < iNum; i++)
	{
		iStrWith = CalculateString(stChartDate.stKDate.vecMainIndex[i].strIndexName,9)+5;
		rcItem.left = iOffLeft;
		rcItem.right = iOffLeft + iStrWith;
		stChartDate.stKDate.vecMainIndex[i].rcIndex = rcItem;
		if (stChartDate.stKDate.uiCurSelIndex == stChartDate.stKDate.vecMainIndex[i].uiIndexID)
		{
			//gpCach->FillRectangle(&sbrFill,RectF(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height()));
			sbrFont.SetColor(Color(255,0,255,255));
		}
		else
		{
			sbrFont.SetColor(Color(255,233,233,233));
		}
		gpCach->DrawString(T2W(stChartDate.stKDate.vecMainIndex[i].strIndexName),-1,&ft,RectF(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height()),
			&formatC,&sbrFont);
		gpCach->DrawLine(&pnGap,PointF(rcItem.right,rcItem.top),PointF(rcItem.right,rcItem.bottom));
		iOffLeft += iStrWith;
	}
}
CRect CDlgTrendChart_D::DrawMACoordinate(Graphics * gpCach,CRect rcDraw)
{
	CRect rcRet;
	int iOffL = 0;
	int iOffT = 18;
	int iOffR = 0;
	int iOffB = 2;
	rcDraw.InflateRect(-iOffL,-iOffT,-iOffR,-iOffB);
	rcRet = rcDraw;
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0 ||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return rcRet;

	Color clrCd[] = {Color(255,255,0,0),Color(255,255,255,255),Color(255,255,0,0),Color(255,0,255,0)};
	USES_CONVERSION;
	Gdiplus::SolidBrush sbfont(clrCd[1]);
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format,formatl;
	format.SetAlignment(StringAlignmentFar);
	formatl.SetAlignment(StringAlignmentNear);
	Pen pnLine(clrCd[0],1.0);
	Pen pnLineY(clrCd[0],1.0);
	Pen pnLineYS(clrCd[0],2.0);
	pnLineY.SetDashStyle(DashStyleDash);

	float iYsDayPrice,iMaxPrice,iMinPrice;
	//iYsDayPrice = stChartDate.stKDate.fYsPrice_K;
	//iMaxPrice = stChartDate.stKDate.fMaxPrice_K;
	//iMinPrice = stChartDate.stKDate.fMinPrice_K;
	stChartDate.stKDate.GetMaxMinValue(iMaxPrice,iMinPrice,iYsDayPrice,0);

	int iCdXNum = 7;

	double iEveryPrice = (iMaxPrice - iMinPrice)/(iCdXNum-1);
	float iEveryHight = ((iMaxPrice - iMinPrice)/(iCdXNum-1))*((rcRet.Height())/(iMaxPrice - iMinPrice));

	CString strTemp;
	for (int i = 0; i < (iCdXNum);i++)
	{
		if (i == 0||i == (iCdXNum - 1))
		{
			continue;
		}
		gpCach->DrawLine(&pnLineY,PointF(rcRet.left,rcRet.top + iEveryHight*(i)),PointF(rcRet.right,rcRet.top + iEveryHight*(i)));
		strTemp.Format(_T("%.2f"),(iMaxPrice - iEveryPrice*(i)));
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRet.left - iOffL,rcRet.top + iEveryHight*(i) - 15,iOffL,15),&format,&sbfont);
	}
	int iBotton = rcRet.top + iEveryHight*(iCdXNum-1);
	rcRet.bottom = iBotton;
	gpCach->DrawLine(&pnLine,PointF(rcRet.left,iBotton),PointF(rcRet.right,iBotton));
	return rcRet;
}
CRect CDlgTrendChart_D::DrawVOLCoordinate_K(Graphics * gpCach,CRect rcDraw)
{
	CRect rcRet;
	int iOffL = 0;
	int iOffT = 18;
	int iOffR = 0;
	int iOffB = 2;
	rcDraw.InflateRect(-iOffL,-iOffT,-iOffR,-iOffB);
	rcRet = rcDraw;
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0 ||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return rcRet;

	Color clrCd[] = {Color(255,255,0,0),Color(255,255,255,255),Color(255,255,0,0),Color(255,0,255,0)};
	USES_CONVERSION;
	Gdiplus::SolidBrush sbfont(clrCd[1]);
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format,formatl;
	format.SetAlignment(StringAlignmentFar);
	formatl.SetAlignment(StringAlignmentNear);
	Pen pnLine(clrCd[0],1.0);
	Pen pnLineY(clrCd[0],1.0);
	Pen pnLineYS(clrCd[0],2.0);
	pnLineY.SetDashStyle(DashStyleDash);

	float iYsDayPrice,iMaxPrice,iMinPrice;
	////GetMaxMinPrice(iMaxPrice,iMinPrice,iYsDayPrice);
	//iMaxPrice = stChartDate.stKDate.fMaxVol_K;
	//iMinPrice = stChartDate.stKDate.fMinVol_K;
	//iYsDayPrice = (iMaxPrice - iMinPrice)/2;//stChartDate.stKDate.fPPVol_K;
	stChartDate.stKDate.GetMaxMinValue(iMaxPrice,iMinPrice,iYsDayPrice,1);

	int iCdXNum = 5;

	double iEveryPrice = (iMaxPrice - iMinPrice)/(iCdXNum-1);
	float iEveryHight = ((iMaxPrice - iMinPrice)/(iCdXNum-1))*((rcRet.Height())/(iMaxPrice - iMinPrice));

	CString strTemp;
	for (int i = 0; i < (iCdXNum);i++)
	{
		if (i == 0||i == (iCdXNum - 1))
		{
			continue;
		}
		gpCach->DrawLine(&pnLineY,PointF(rcRet.left,rcRet.top + iEveryHight*(i)),PointF(rcRet.right,rcRet.top + iEveryHight*(i)));
		strTemp.Format(_T("%.2f"),(iMaxPrice - iEveryPrice*(i)));
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRet.left - iOffL,rcRet.top + iEveryHight*(i) - 15,iOffL,15),&format,&sbfont);
	}
	int iBotton = rcRet.top + iEveryHight*(iCdXNum-1);
	rcRet.bottom = iBotton;
	gpCach->DrawLine(&pnLine,PointF(rcRet.left,iBotton),PointF(rcRet.right,iBotton));
	return rcRet;
}
CRect CDlgTrendChart_D::DrawOtherCoordinate(Graphics * gpCach,CRect rcDraw)
{
	CRect rcRet;
	int iOffL = 0;
	int iOffT = 18;
	int iOffR = 0;
	int iOffB = 2;
	rcDraw.InflateRect(-iOffL,-iOffT,-iOffR,-iOffB);
	rcRet = rcDraw;
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0 ||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return rcRet;

	Color clrCd[] = {Color(255,255,0,0),Color(255,255,255,255),Color(255,255,0,0),Color(255,0,255,0)};
	USES_CONVERSION;
	Gdiplus::SolidBrush sbfont(clrCd[1]);
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format,formatl;
	format.SetAlignment(StringAlignmentFar);
	formatl.SetAlignment(StringAlignmentNear);
	Pen pnLine(clrCd[0],1.0);
	Pen pnLineY(clrCd[0],1.0);
	Pen pnLineYS(clrCd[0],2.0);
	pnLineY.SetDashStyle(DashStyleDash);

	float iYsDayPrice,iMaxPrice,iMinPrice;
	////GetMaxMinPrice(iMaxPrice,iMinPrice,iYsDayPrice);
	//iMaxPrice = stChartDate.stKDate.fMaxOther_K;
	//iMinPrice = stChartDate.stKDate.fMinOther_K;
	//iYsDayPrice = (iMaxPrice - iMinPrice)/2;//stChartDate.stKDate.fPPOther_K;
	stChartDate.stKDate.GetMaxMinValue(iMaxPrice,iMinPrice,iYsDayPrice,2);

	int iCdXNum = 5;

	double iEveryPrice = (iMaxPrice - iMinPrice)/(iCdXNum-1);
	float iEveryHight = ((iMaxPrice - iMinPrice)/(iCdXNum-1))*((rcRet.Height())/(iMaxPrice - iMinPrice));

	CString strTemp;
	for (int i = 0; i < (iCdXNum);i++)
	{
		if (i == 0||i == (iCdXNum - 1))
		{
			continue;
		}
		gpCach->DrawLine(&pnLineY,PointF(rcRet.left,rcRet.top + iEveryHight*(i)),PointF(rcRet.right,rcRet.top + iEveryHight*(i)));
		strTemp.Format(_T("%.2f"),(iMaxPrice - iEveryPrice*(i)));
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRet.left - iOffL,rcRet.top + iEveryHight*(i) - 15,iOffL,15),&format,&sbfont);
	}
	int iBotton = rcRet.top + iEveryHight*(iCdXNum-1);
	rcRet.bottom = iBotton;
	//gpCach->DrawLine(&pnLine,PointF(rcRet.left,iBotton),PointF(rcRet.right,iBotton));
	return rcRet;
}
void CDlgTrendChart_D::DrawTimeTrend(Graphics * gpCach,CRect rcDraw)
{
	//if (stChartDate.stTrendDate.mapHisTTDate.size() <= 0)return;

	CRect rcTT,rcVol,rcTips;
	int iVolHi = rcDraw.Height()/3;//200
	rcTT = rcDraw;
	rcTT.bottom -= iVolHi;
	//m_rcTT.right -= iRWith;
	
	rcTT = DrawTTCoordinate(gpCach,rcTT);
	stChartDate.stTrendDate.rcTimeTrend = rcTT;

	rcVol = rcDraw;
	rcVol.top = rcTT.bottom;
	//m_rcVol.right -= iRWith;
	rcVol = DrawVOLCoordinate(gpCach,rcVol);
	stChartDate.stTrendDate.rcVolTrend = rcVol;

	rcTips = rcDraw;
	rcTips.right = rcTT.left-1;
	rcTips.left += 5;
	rcTips.top += 20;
	stChartDate.stTrendDate.rcTips = rcTips;

	CaluPos(rcTT,rcVol);
	DrawTTGp(gpCach,rcVol);

	DrawCorssTips(gpCach);
}
void CDlgTrendChart_D::DrawTimeTrend_RightRc(Graphics * gpCach,CRect rcDraw)
{
	Pen pnRc(Color(255,255,0,0));
	rcDraw.InflateRect(0,0,-5,-5);
	int iBntHight_b = 15;
	int iTopBtnHight = 30;
	int iTopPartHight = 19*15;

	//gpCach->DrawLine(&pnLine,rcDraw.left + 10,rcDraw.bottom - 4,rcDraw.right - 5,rcDraw.bottom - 4);
	gpCach->DrawRectangle(&pnRc,Rect(rcDraw.left,rcDraw.top+iTopBtnHight,rcDraw.Width(),rcDraw.Height()-iTopBtnHight));
	
	gpCach->DrawLine(&pnRc,PointF(rcDraw.left,rcDraw.bottom - iBntHight_b),PointF(rcDraw.right,rcDraw.bottom - iBntHight_b));
	gpCach->DrawLine(&pnRc,PointF(rcDraw.left,rcDraw.top + iTopPartHight + iTopBtnHight),PointF(rcDraw.right,rcDraw.top + iTopPartHight + iTopBtnHight));

	CRect rcTopBnt;
	rcTopBnt.left = rcDraw.left;
	rcTopBnt.right = rcDraw.right;
	rcTopBnt.top = rcDraw.top;
	rcTopBnt.bottom = rcTopBnt.top + iTopBtnHight;
	stChartDate.stTopPartDate.stTopBtn.rcTop = rcTopBnt;
	DrawTimeTrend_RightRc_TopBtn(gpCach,rcTopBnt);

	CRect rcTopPart;
	rcTopPart.left = rcDraw.left;
	rcTopPart.right = rcDraw.right;
	rcTopPart.top = rcTopBnt.bottom;
	rcTopPart.bottom = rcTopPart.top + iTopPartHight;
	if (rcTopPart.bottom >= rcDraw.bottom)
	{
		gpCach->SetClip(Rect(rcTopPart.left,rcTopPart.top,rcTopPart.Width(),rcDraw.bottom - rcTopPart.top));
	}
	else
	    gpCach->SetClip(Rect(rcTopPart.left,rcTopPart.top,rcTopPart.Width(),rcTopPart.Height()));
	DrawTimeTrend_RightRc_TopPart(gpCach,rcTopPart);
	gpCach->ResetClip();

	CRect rcMin;
	rcMin.left = rcDraw.left;
	rcMin.right = rcDraw.right;
	rcMin.top = rcTopPart.bottom/*rcDraw.top + iTopPartHight*/;
	rcMin.bottom = rcDraw.bottom - iBntHight_b;
	gpCach->SetClip(Rect(rcMin.left,rcMin.top,rcMin.Width(),rcMin.Height()));
	if (stChartDate.stTabInfo.uiCurSel == 0)
	{
		DrawTimeTrend_Min(gpCach,rcMin);
	}
	else
	{
		DrawDealDetails(gpCach,rcMin);
	}
	gpCach->ResetClip();

	//if (rcMin.top >= rcMin.bottom)return;

	CRect rcTab;
	rcTab.left = rcDraw.left;
	rcTab.right = rcDraw.right;
	rcTab.top = rcMin.bottom/*rcDraw.top + iTopPartHight*/;
	rcTab.bottom = rcDraw.bottom;
	stChartDate.stTabInfo.rcTab = rcTab;
	gpCach->SetClip(Rect(rcTab.left,rcTab.top,rcTab.Width(),rcTab.Height()));
	SolidBrush sldrsh(Color(255,0,0,0));
	gpCach->FillRectangle(&sldrsh,Rect(rcTab.left,rcTab.top,rcTab.Width(),rcTab.Height()));
	DrawTimeTrend_RightRc_Tab(gpCach,rcTab);
	gpCach->ResetClip();
}
void CDlgTrendChart_D::DrawDealDetails(Graphics * gpCach,CRect rcDraw)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return;

	int iItemHight = 15;
	if (rcDraw.Height() < iItemHight)return;
	int iDisplayNum = rcDraw.Height() / iItemHight;
	int iBSDirc = 20;
	int iEveryWith = (rcDraw.Width()-iBSDirc)/3;
	//int iHalfX = rcDraw.left + iEveryWith;
	CRect rcLeft,rcMid,rcRight,rcBSDirc;
	rcLeft = rcRight = rcMid = rcBSDirc = rcDraw;
	rcLeft.right = rcDraw.left + iEveryWith;
	rcMid.left = rcLeft.right;
	rcMid.right = rcMid.left + iEveryWith;
	rcRight.left = rcMid.right;
	rcRight.right = rcDraw.right - iBSDirc;
	rcBSDirc.left = rcRight.right;

	USES_CONVERSION;
	Color clrCd[] = {Color(255,0,255,255),Color(255,255,255,0)};
	Color clrUD[] = {Color(255,255,0,0),Color(255,0,255,0),Color(255,255,255,255)};
	Gdiplus::SolidBrush sbfontT(clrCd[0]);
	Gdiplus::SolidBrush sbfontVol(clrCd[1]);
	Gdiplus::SolidBrush sbfontUD(clrUD[0]);
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat formatF,formatN,formatC;
	formatF.SetAlignment(StringAlignmentFar);
	formatN.SetAlignment(StringAlignmentNear);
	formatC.SetAlignment(StringAlignmentNear);
	Pen pnLine(Color(255,255,0,0),1.0);

	int iOffY = 0;
	CString strTemp;
	CString strBDDirc[3] = {_T("B"),_T("S"),_T("--")};
	//int iDisplayMax = iDisplayNum > stChartDate.stDealDetailsInfo.iDisplayNum?stChartDate.stDealDetailsInfo.iDisplayNum:iDisplayNum;
	int iDisplayMax = stChartDate.stDealDetailsInfo.iDisplayNum = iDisplayNum;
	TvecDealDetails::reverse_iterator itVec;
	DealDetailsDate stDealDetailsDate;

	//int iNum11 = stChartDate.stDealDetailsInfo.vecDealDetails.size();
	//for (int i = 0; i < iDisplayMax; i++)
	int i = 0;
	for(i = 0,itVec = stChartDate.stDealDetailsInfo.vecDealDetails.rbegin();
		(itVec != stChartDate.stDealDetailsInfo.vecDealDetails.rend()&&i < iDisplayMax);(itVec++,i++))
	{
		stDealDetailsDate = (*itVec);
		//rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
		rcMid.top = rcRight.top = rcLeft.top = rcBSDirc.top = (rcDraw.top + iItemHight*i + iOffY);
		rcRight.bottom = rcLeft.bottom = rcMid.bottom = rcBSDirc.bottom = rcLeft.top + iItemHight - iOffY;

		sbfontUD.SetColor(clrUD[2]);
		strTemp.Format(_T("%d"),stDealDetailsDate.uiDateTime%1000000);
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
			&formatN,&sbfontUD);

		if (stDealDetailsDate.fPrice > stChartDate.stTopPartDate.fYesDPrice)
		{
			sbfontUD.SetColor(clrUD[0]);
		}
		else if (stDealDetailsDate.fPrice < stChartDate.stTopPartDate.fYesDPrice)
		{
			sbfontUD.SetColor(clrUD[1]);
		}
		else
			sbfontUD.SetColor(clrUD[2]);
		strTemp.Format(_T("%.2f"),stDealDetailsDate.fPrice);
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcMid.left,rcMid.top,rcMid.Width(),rcMid.Height()),
			&formatN,&sbfontUD);

		strTemp.Format(_T("%.2f"),stDealDetailsDate.fQty);
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
			&formatF,&sbfontVol);

		int iUD = (((stDealDetailsDate.cBSDirec > 2 || stDealDetailsDate.cBSDirec < 0))?2:stDealDetailsDate.cBSDirec);
		sbfontUD.SetColor(clrUD[iUD]);
		gpCach->DrawString(T2W(strBDDirc[iUD]),-1,&ft,RectF(rcBSDirc.left,rcBSDirc.top,rcBSDirc.Width(),rcBSDirc.Height()),
			&formatC,&sbfontUD);
		//gpCach->DrawLine(&pnLine,rcDraw.left,rcRight.bottom,rcDraw.right,rcRight.bottom);
	}
}

void CDlgTrendChart_D::DrawTimeTrend_MainChange_Tab(Graphics * gpCach,CRect rcDraw)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return;

	//CaluRectRightRc_Tab(gpCach,rcDraw);
	CaluRectTab(gpCach,rcDraw,&(stChartDate.stTabMain));

	DrawTab(gpCach,rcDraw,&(stChartDate.stTabMain));
}
void CDlgTrendChart_D::DrawTimeTrend_RightRc_Tab(Graphics * gpCach,CRect rcDraw)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return;
	
	//CaluRectRightRc_Tab(gpCach,rcDraw);
	CaluRectTab(gpCach,rcDraw,&(stChartDate.stTabInfo));

	DrawTab(gpCach,rcDraw,&(stChartDate.stTabInfo));
}
void CDlgTrendChart_D::DrawTab(Graphics * gpCach,CRect rcDraw,TabInfo *pstTabInfo)
{
	if (!pstTabInfo)return;

	USES_CONVERSION;
	Gdiplus::SolidBrush sbfont(Color(255,0,255,255));
	Gdiplus::Pen pnBrd(Color(255,255,0,0));
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format;
	//Color colors1[] = {Color(255,255,0,0),Color(255,255,0,0),Color(255,26,0,111),Color(255,137,110,110)};
	//Color colors2[] = {Color(255,168,168,168),Color(255,168,168,168),Color(255,201,17,17),Color(255,18,18,111)};
	Color colors1[] = {Color(255,66,66,66),Color(255,66,66,66),Color(255,66,66,66),Color(255,66,66,66)};
	Color colors2[] = {Color(255,0,0,0),Color(255,0,0,0),Color(255,0,0,0),Color(255,0,0,0)};
	int iColorNum = 4;
	float pos[] = {0.0f,0.3f,1.0f};
	format.SetAlignment(StringAlignmentCenter);

	CRect rcItem,rcTemp;
	CString strTemp = _T("");
	int iOffWith = 10;
	TmapTabItem::iterator itMap;
	for (itMap = pstTabInfo->mapTabItme.begin(); 
		itMap != pstTabInfo->mapTabItme.end(); itMap++)
	{
		rcItem = itMap->second.rcItem;
		//rcItem.InflateRect(-1,0,-1,-1);
		if (pstTabInfo->uiCurSel == itMap->second.uiItemId)
		{
			strTemp = itMap->second.strItemName;
			rcTemp = rcItem;
			continue;
		}
		Point points[] = {Point(rcItem.left,rcItem.top),Point(rcItem.right,rcItem.top),
			Point(rcItem.right - iOffWith/*rcItem.Width()/5*/,rcItem.bottom),Point(rcItem.left + iOffWith/*rcItem.Width()/5*/,rcItem.bottom)};
		PathGradientBrush pathbrush(points,4);
		//if (stTabInfo.uiCurSel == itMap->second.uiItemId)
		//{
		//	pathbrush.SetSurroundColors(colors1,&iColorNum);
		//}
		//else
			pathbrush.SetSurroundColors(colors2,&iColorNum);
		gpCach->FillRectangle(&pathbrush,RectF(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height()));
		gpCach->DrawString(T2W(itMap->second.strItemName),-1,&ft,RectF(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height()),
			&format,&sbfont);
		gpCach->DrawPolygon(&pnBrd,points,4);
	}	
	if (!strTemp.IsEmpty())
	{
		Point points[] = {Point(rcTemp.left,rcTemp.top),Point(rcTemp.right,rcTemp.top),
			Point(rcTemp.right - iOffWith/*rcTemp.Width()/5*/,rcTemp.bottom),Point(rcTemp.left + iOffWith/*rcTemp.Width()/5*/,rcTemp.bottom)};
		PathGradientBrush pathbrush(points,4);
		pathbrush.SetSurroundColors(colors1,&iColorNum);

		gpCach->FillRectangle(&pathbrush,RectF(rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height()));
		sbfont.SetColor(Color(255,240,240,240));
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height()),
			&format,&sbfont);
		gpCach->DrawPolygon(&pnBrd,points,4);
	}
}
void CDlgTrendChart_D::CaluRectTab(Graphics * gpCach,CRect rcDraw,TabInfo *pstTabInfo)
{
	if(!pstTabInfo)return;
	CRect rcItem = rcDraw;
	TmapTabItem::iterator itMap;
	pstTabInfo->rcTab = rcDraw;
	int i=0;
	int iOffX = rcDraw.left;
	int iOverlapWith = 10;
	for (itMap = pstTabInfo->mapTabItme.begin(); 
		itMap != pstTabInfo->mapTabItme.end(); itMap++,i++)
	{
		rcItem.left = iOffX;
		rcItem.right = rcItem.left + itMap->second.uiItemWith;
		itMap->second.rcItem = rcItem;
		iOffX = rcItem.right - iOverlapWith;
	}
}
void CDlgTrendChart_D::AddTabMainChange()
{
	const int iTabNum = 2;
	CString strDisplay[iTabNum] = {_T("分时走势"),_T("历史K线")};
	for (int i = 0; i < iTabNum; i++)
	{
		stChartDate.stTabMain.mapTabItme[i].uiItemWith = 80;
		stChartDate.stTabMain.mapTabItme[i].strItemName = strDisplay[i];
		stChartDate.stTabMain.mapTabItme[i].uiItemId = i;
	}
}
void CDlgTrendChart_D::AddTestTTTabDate()
{
	const int iTabNum = 2;
	CString strDisplay[iTabNum] = {_T("图"),_T("细")};
	for (int i = 0; i < iTabNum; i++)
	{
		stChartDate.stTabInfo.mapTabItme[i].uiItemWith = 50;
		stChartDate.stTabInfo.mapTabItme[i].strItemName = strDisplay[i];
		stChartDate.stTabInfo.mapTabItme[i].uiItemId = i;
	}
}
void CDlgTrendChart_D::DrawTimeTrend_RightRc_TopBtn(Graphics * gpCach,CRect rcDraw)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return;

	int iBtnWith = 20;
	stChartDate.stTopPartDate.stTopBtn.rcBtnLeft =  CRect(stChartDate.stTopPartDate.stTopBtn.rcTop.left,stChartDate.stTopPartDate.stTopBtn.rcTop.top,
		stChartDate.stTopPartDate.stTopBtn.rcTop.left + iBtnWith,stChartDate.stTopPartDate.stTopBtn.rcTop.bottom);
	stChartDate.stTopPartDate.stTopBtn.rcBtnRight = CRect(stChartDate.stTopPartDate.stTopBtn.rcTop.right - iBtnWith,stChartDate.stTopPartDate.stTopBtn.rcTop.top,
		stChartDate.stTopPartDate.stTopBtn.rcTop.right,stChartDate.stTopPartDate.stTopBtn.rcTop.bottom);

	rcDraw.InflateRect(-iBtnWith,0,-iBtnWith,0);
	
	USES_CONVERSION;
	Color clrSD(255,255,255,255);
	Color clrCd[] = {Color(255,255,255,0),Color(255,0,255,255)};
	Gdiplus::SolidBrush sbfont(clrCd[0]);
	Gdiplus::SolidBrush sbSD(clrSD);
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),12/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format,formatl;
	format.SetAlignment(StringAlignmentFar);
	formatl.SetAlignment(StringAlignmentNear);
	//Pen pnLine(clrCd[0],1.0);
	//gpCach->DrawRectangle(&pnLine,RectF(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height()));
	//pnLineY.SetDashStyle(DashStyleDash);
	CString strTemp = stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolName;
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcDraw.left,rcDraw.top+5,rcDraw.Width()/2,rcDraw.Height()-10),
		&format,&sbfont);

	strTemp = stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode;
	sbfont.SetColor(clrCd[1]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcDraw.left+rcDraw.Width()/2,rcDraw.top+5,rcDraw.Width()/2,rcDraw.Height()-10),
		&formatl,&sbfont);

	Gdiplus::Point ptLeft[3] = {Point(stChartDate.stTopPartDate.stTopBtn.rcBtnLeft.left+5,stChartDate.stTopPartDate.stTopBtn.rcBtnLeft.top + stChartDate.stTopPartDate.stTopBtn.rcBtnLeft.Height()/2),
		Point(stChartDate.stTopPartDate.stTopBtn.rcBtnLeft.right,stChartDate.stTopPartDate.stTopBtn.rcBtnLeft.top+5),Point(stChartDate.stTopPartDate.stTopBtn.rcBtnLeft.right,stChartDate.stTopPartDate.stTopBtn.rcBtnLeft.bottom-5)};
	Gdiplus::Point ptRight[3] = {Point(stChartDate.stTopPartDate.stTopBtn.rcBtnRight.right-5,stChartDate.stTopPartDate.stTopBtn.rcBtnRight.top + stChartDate.stTopPartDate.stTopBtn.rcBtnRight.Height()/2),
		Point(stChartDate.stTopPartDate.stTopBtn.rcBtnRight.left,stChartDate.stTopPartDate.stTopBtn.rcBtnRight.top+5),Point(stChartDate.stTopPartDate.stTopBtn.rcBtnRight.left,stChartDate.stTopPartDate.stTopBtn.rcBtnRight.bottom-5)};
	gpCach->FillPolygon(&sbSD,ptLeft,3);
	gpCach->FillPolygon(&sbSD,ptRight,3);
}
void CDlgTrendChart_D::DrawTimeTrend_RightRc_TopPart(Graphics * gpCach,CRect rcDraw)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return;

	int iItemHight = 15;
	int iHalfWith = rcDraw.Width()/2;
	int iHalfX = rcDraw.left + iHalfWith;
	CRect rcLeft,rcRight;
	rcLeft = rcRight = rcDraw;
	rcLeft.right = iHalfX;
	rcRight.left = iHalfX;

	CString strTitle[] = {_T("委比"),_T("委差"),//0,1   1
	_T("卖五"),_T("卖四"),_T("卖三"),_T("卖二"),_T("卖一"),//  2
	_T("买一"),_T("买二"),_T("买三"),_T("买四"),_T("买五"),//  3
	_T("最新价"),_T("均价"),//12,13   4
    _T("涨跌"),_T("开盘价"),//14,15   5
	_T("幅度"),_T("最高价"),//16,17   6
	_T("现量"),_T("最低价"),//18,19   7
	_T("总量"),_T("量比"),//20,21     8
	_T("成交额"),_T("换手率"),//22,23 9
	_T("昨收"),
	_T("内盘"),_T("外盘")};//24                  10
	USES_CONVERSION;
	Color clrCd[] = {Color(255,0,255,255),Color(255,255,255,0)};
	Color clrUD[] = {Color(255,255,255,255),Color(255,255,0,0),Color(255,0,255,0)};
	Gdiplus::SolidBrush sbfontT(clrCd[0]);
	Gdiplus::SolidBrush sbfontVol(clrCd[1]);
	Gdiplus::SolidBrush sbfontUD(clrUD[0]);
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat formatF,formatN;
	formatF.SetAlignment(StringAlignmentFar);
	formatN.SetAlignment(StringAlignmentNear);
	Pen pnLine(Color(255,255,0,0),1.0);

	int iOffY = 0;
	//1
	int i = 0;
	float fZeorSt = 0.0000000000001;
	CString strTemp;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[0]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fWibi);
	strTemp += _T("%");
	if (stChartDate.stTopPartDate.fWibi > fZeorSt)
	{
		sbfontUD.SetColor(clrUD[1]);
	}
	else if (stChartDate.stTopPartDate.fWibi < fZeorSt)
	{
		sbfontUD.SetColor(clrUD[2]);
	}
	else
		sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontUD);
	gpCach->DrawString(T2W(strTitle[1]),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fWeicha);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatF,&sbfontUD);
	gpCach->DrawLine(&pnLine,rcDraw.left,rcRight.bottom,rcDraw.right,rcRight.bottom);

	for (int x = 0; x < 5;x++)
	{
		i++;
		rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
		rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
		rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
		gpCach->DrawString(T2W(strTitle[2+x]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
			&formatN,&sbfontT);
		if (stChartDate.stTopPartDate.sVolPrice[4-x].fsellQuote.fPrice > stChartDate.stTopPartDate.fYesDPrice)
		{
			sbfontUD.SetColor(clrUD[1]);
		}
		else if (stChartDate.stTopPartDate.sVolPrice[4-x].fsellQuote.fPrice < stChartDate.stTopPartDate.fYesDPrice)
		{
			sbfontUD.SetColor(clrUD[2]);
		}
		else
			sbfontUD.SetColor(clrUD[0]);
		strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.sVolPrice[4-x].fsellQuote.fPrice);
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
			&formatF,&sbfontUD);
		strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.sVolPrice[4-x].fsellQuote.fQty);
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
			&formatF,&sbfontVol);
	}
	gpCach->DrawLine(&pnLine,rcDraw.left,rcRight.bottom,rcDraw.right,rcRight.bottom);

	for (int x = 0; x < 5;x++)
	{
		i++;
		rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
		rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
		rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
		gpCach->DrawString(T2W(strTitle[7+x]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
			&formatN,&sbfontT);
		strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.sVolPrice[x].fbuyQuote.fPrice);
		if (stChartDate.stTopPartDate.sVolPrice[x].fbuyQuote.fPrice > stChartDate.stTopPartDate.fYesDPrice)
		{
			sbfontUD.SetColor(clrUD[1]);
		}
		else if (stChartDate.stTopPartDate.sVolPrice[x].fbuyQuote.fPrice < stChartDate.stTopPartDate.fYesDPrice)
		{
			sbfontUD.SetColor(clrUD[2]);
		}
		else
			sbfontUD.SetColor(clrUD[0]);
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
			&formatF,&sbfontUD);
		strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.sVolPrice[x].fbuyQuote.fQty);
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
			&formatF,&sbfontVol);
	}
	gpCach->DrawLine(&pnLine,rcDraw.left,rcRight.bottom,rcDraw.right,rcRight.bottom);

	/////12
	i++;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[12]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fNewPrice);
	if (stChartDate.stTopPartDate.fNewPrice > stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[1]);
	}
	else if (stChartDate.stTopPartDate.fNewPrice < stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[2]);
	}
	else
		sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontUD);
	gpCach->DrawString(T2W(strTitle[13]),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fPinjunPrice);
	if (stChartDate.stTopPartDate.fPinjunPrice > stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[1]);
	}
	else if (stChartDate.stTopPartDate.fPinjunPrice < stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[2]);
	}
	else
		sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatF,&sbfontUD);
	///
	i++;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[14]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fZhangDie);
	if (stChartDate.stTopPartDate.fZhangDie > fZeorSt)
	{
		sbfontUD.SetColor(clrUD[1]);
	}
	else if (stChartDate.stTopPartDate.fZhangDie < fZeorSt)
	{
		sbfontUD.SetColor(clrUD[2]);
	}
	else
		sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontUD);
	gpCach->DrawString(T2W(strTitle[15]),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fOpenPrice);
	if (stChartDate.stTopPartDate.fOpenPrice > stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[1]);
	}
	else if (stChartDate.stTopPartDate.fOpenPrice < stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[2]);
	}
	else
		sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatF,&sbfontUD);

	///
	i++;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[16]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fFudu);
	strTemp += _T("%");
	if (stChartDate.stTopPartDate.fFudu > fZeorSt)
	{
		sbfontUD.SetColor(clrUD[1]);
	}
	else if (stChartDate.stTopPartDate.fFudu < fZeorSt)
	{
		sbfontUD.SetColor(clrUD[2]);
	}
	else
		sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontUD);
	gpCach->DrawString(T2W(strTitle[17]),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fMaxPrice);
	if (stChartDate.stTopPartDate.fMaxPrice > stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[1]);
	}
	else if (stChartDate.stTopPartDate.fMaxPrice < stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[2]);
	}
	else
		sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatF,&sbfontUD);
	/////
	i++;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[18]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fNowVol);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontVol);
	gpCach->DrawString(T2W(strTitle[19]),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fMinPrice);
	if (stChartDate.stTopPartDate.fMinPrice > stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[1]);
	}
	else if (stChartDate.stTopPartDate.fMinPrice < stChartDate.stTopPartDate.fYesDPrice)
	{
		sbfontUD.SetColor(clrUD[2]);
	}
	else
		sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatF,&sbfontUD);
	/////
	i++;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[20]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fTotalVol);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontVol);
	gpCach->DrawString(T2W(strTitle[21]),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fVolRatio);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatF,&sbfontVol);

	/////
	i++;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[22]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fDealMoney);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontVol);
	gpCach->DrawString(T2W(strTitle[23]),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fHuanshouRatio);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatF,&sbfontVol);

	i++;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[24]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fYesDPrice);
	sbfontUD.SetColor(clrUD[0]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontUD);

	gpCach->DrawLine(&pnLine,rcDraw.left,rcRight.bottom,rcDraw.right,rcRight.bottom);
	/////
	i++;
	rcRight.top = rcLeft.top = rcDraw.top + iItemHight*i;
	rcRight.bottom = rcLeft.bottom = rcLeft.top + iItemHight;
	rcRight.top = rcLeft.top = (rcLeft.top + iOffY);
	gpCach->DrawString(T2W(strTitle[25]),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fOutPan);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcLeft.left,rcLeft.top,rcLeft.Width(),rcLeft.Height()),
		&formatF,&sbfontVol);
	gpCach->DrawString(T2W(strTitle[26]),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatN,&sbfontT);
	strTemp.Format(_T("%.2f"),stChartDate.stTopPartDate.fInPan);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcRight.left,rcRight.top,rcRight.Width(),rcRight.Height()),
		&formatF,&sbfontVol);
}
void CDlgTrendChart_D::DrawBk(Graphics * gpCach,CRect rcFill,DWORD clrFill)
{
	if (rcFill.Width()<= 0 || rcFill.Height() <= 0 /*|| CDC::FromHandle(gpCach->GetHDC()) == NULL*/ ||
		!gpCach->IsVisible(Rect(rcFill.left,rcFill.top,rcFill.Width(),rcFill.Height())))return;

	Gdiplus::SolidBrush sbrzFill(clrFill);

	gpCach->FillRectangle(&sbrzFill,Rect(rcFill.left,rcFill.top,rcFill.Width(),rcFill.Height()));
}
CRect CDlgTrendChart_D::DrawTTCoordinate(Graphics * gpCach,CRect rcFill,int iType)
{
	CRect rcRet;
	int iOffL = 80;
	int iOffT = 20;
	int iOffR = 60;
	int iOffB = 20;
	if (iType == 1)
	{
		iOffL = 40;
		iOffT = 15;
		iOffR = 5;
		iOffB = 5;
	}
	rcFill.InflateRect(-iOffL,-iOffT,-iOffR,-iOffB);
	rcRet = rcFill;
	if (rcFill.Width()<= 0 || rcFill.Height() <= 0 /*|| CDC::FromHandle(gpCach->GetHDC()) == NULL*/ ||
		!gpCach->IsVisible(Rect(rcFill.left,rcFill.top,rcFill.Width(),rcFill.Height())))return rcRet;

	Color clrCd[] = {Color(255,255,0,0),Color(255,255,255,255),Color(255,255,0,0),Color(255,0,255,0)};
	USES_CONVERSION;
	Gdiplus::SolidBrush sbfont(clrCd[2]);
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format,formatl;
	format.SetAlignment(StringAlignmentFar);
	formatl.SetAlignment(StringAlignmentNear);
	Pen pnLine(clrCd[0],1.0);
	Pen pnLineY(clrCd[0],1.0);
	Pen pnLineYS(clrCd[0],2.0);
	pnLineY.SetDashStyle(DashStyleDash);
	
	float iYsDayPrice,iMaxPrice,iMinPrice;
	//GetMaxMinPrice(iMaxPrice,iMinPrice,iYsDayPrice);
	iYsDayPrice = stChartDate.stTrendDate.fYsPrice;
	iMaxPrice = stChartDate.stTrendDate.fMaxPrice;
	iMinPrice = stChartDate.stTrendDate.fMinPrice;
	/////////////////
	INT64 iMaxP,iMinP,iYsP;
	iMaxP = iMaxPrice *10000;
	iMinP = iMinPrice * 10000;
	iYsP = iYsDayPrice *10000;
	if (iMaxP == iMinP)
	{
		if (iYsP == iMaxP)
		{
			iMaxPrice += 3; 
			iMinPrice -= 3;
			if (iMinPrice <= 0.0000001)
			{
				iMinPrice = 0.01;
			}
		}
		else if(iYsP > iMaxP)
		{
			iMaxPrice += (iYsDayPrice - iMaxPrice)*5;
			iMaxPrice -= (iYsDayPrice - iMaxPrice)*5;
			if (iMinPrice <= 0.0000001)
			{
				iMinPrice = 0.01;
			}
		}
		else
		{
			iMaxPrice += (iMaxPrice - iYsDayPrice)*5;
			iMaxPrice -= (iMaxPrice - iYsDayPrice)*5;
			if (iMinPrice <= 0.0000001)
			{
				iMinPrice = 0.01;
			}
		}
	}
	//////////////////
	int iCdXNum = 13;
	if (iType == 1)
	{
		iCdXNum = 7;
	}
	
	double iEveryPrice = (iMaxPrice - iMinPrice)/(iCdXNum-1);
	//float iEveryHight = (rcFill.Height())/(iCdXNum-1);
	float iEveryHight = /*iEveryPrice*/((iMaxPrice - iMinPrice)/(iCdXNum-1))*((rcFill.Height())/(iMaxPrice - iMinPrice));
	if (iEveryHight < 20)
	{
		iCdXNum = 7;
		iEveryPrice = (iMaxPrice - iMinPrice)/(iCdXNum-1);
		iEveryHight = ((iMaxPrice - iMinPrice)/(iCdXNum-1))*((rcFill.Height())/(iMaxPrice - iMinPrice));
	}

	CString strTemp;
	for (int i = 0; i < (iCdXNum);i++)
	{
		if ((iMaxPrice - iEveryPrice*(i)) > iYsDayPrice)
		{
			sbfont.SetColor(clrCd[2]);
		}
		else if((iMaxPrice - iEveryPrice*(i)) < iYsDayPrice)
		{
			sbfont.SetColor(clrCd[3]);
		}
		else
		{
			sbfont.SetColor(clrCd[1]);
		}
		gpCach->DrawLine(&pnLine,PointF(rcFill.left,rcFill.top + iEveryHight*(i)),PointF(rcFill.right,rcFill.top + iEveryHight*(i)));
		strTemp.Format(_T("%.2f"),(iMaxPrice - iEveryPrice*(i)));
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcFill.left - iOffL,rcFill.top + iEveryHight*(i) - 15,iOffL,15),&format,&sbfont);
		if (iType == 0)
		{
			strTemp.Format(_T("%.2f"),((iMaxPrice - iEveryPrice*(i)) - iYsDayPrice)*100/iYsDayPrice);
			strTemp += _T("%");
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcFill.right,rcFill.top + iEveryHight*(i) - 15,iOffR,15),&formatl,&sbfont);
		}
	}
	int iBotton = rcFill.top + iEveryHight*(iCdXNum-1);
	gpCach->DrawLine(&pnLine,PointF(rcFill.left,rcFill.top),PointF(rcFill.left,iBotton));
	gpCach->DrawLine(&pnLine,PointF(rcFill.right,rcFill.top),PointF(rcFill.right,iBotton));

	rcRet = rcFill;
	rcRet.bottom = iBotton;
	/////
	int iCdYNum = 8;
	int iAllTm = 0;
	int iTimeNum = 1;
	GetTradeTimeInfo(iTimeNum,iAllTm);
	iCdYNum = iAllTm/30;

	//int iEveryWith = rcFill.Width()/(iCdYNum);

	float iEveryWith = (rcFill.Width()*1.0/(iAllTm))*30.0;//rcFill.Width()/(iCdYNum);
	
	for (int j = 0; j < (iCdYNum-1); j++)
	{
		gpCach->DrawLine(&pnLineY,PointF(rcFill.left + iEveryWith*(j+1),rcFill.top),PointF(rcFill.left + iEveryWith*(j+1),iBotton));
	}
	int iH = rcFill.Height()*((iMaxPrice - iYsDayPrice)/(iMaxPrice - iMinPrice));
	gpCach->DrawLine(&pnLineYS,rcFill.left,rcFill.top+iH,rcFill.right,rcFill.top+iH);
	strTemp.Format(_T("%.2f"),iYsDayPrice);
	sbfont.SetColor(clrCd[1]);
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcFill.left - iOffL,rcFill.top+iH,iOffL,15),&format,&sbfont);

	return rcRet;
}

CRect CDlgTrendChart_D::DrawVOLCoordinate(Graphics * gpCach,CRect rcFill,int iType)
{
	CRect rcRet;
	int iOffL = 80;
	int iOffT = 20;
	int iOffR = 60;
	int iOffB = 25;
	if (iType == 1)
	{
		iOffL = 40;
		iOffT = 15;
		iOffR = 5;
		iOffB = 5;
	}
	rcFill.InflateRect(-iOffL,-iOffT,-iOffR,-iOffB);

	rcRet = rcFill;
	if (rcFill.Width()<= 0 || rcFill.Height() <= 0 /*|| CDC::FromHandle(gpCach->GetHDC()) == NULL*/ ||
		!gpCach->IsVisible(Rect(rcFill.left,rcFill.top,rcFill.Width(),rcFill.Height())))return rcRet;

	Color clrCd[] = {Color(255,255,0,0),Color(255,0,255,0),Color(255,255,255,47),Color(255,255,255,255)};
	USES_CONVERSION;
	Gdiplus::SolidBrush sbfont(clrCd[2]);
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format,formatl,formatc;
	format.SetAlignment(StringAlignmentFar);
	formatl.SetAlignment(StringAlignmentNear);
	formatc.SetAlignment(StringAlignmentCenter);
	Pen pnLine(clrCd[0],1.0);
	Pen pnLineY(clrCd[0],1.0);
	Pen pnLineYS(clrCd[1],1.0);
	pnLineY.SetDashStyle(DashStyleDash);

	float iAvVol,iMaxVol,iMinVol;
	//GetMaxMinVol(iMaxVol,iMinVol,iAvVol);
	iMaxVol = stChartDate.stTrendDate.fMaxVol;
	iMinVol = stChartDate.stTrendDate.fMinVol;
	iAvVol = (iMaxVol - iMinVol)/2;//stChartDate.stTrendDate.fPPVol;

	//iAvVol = 300;
	//iMaxVol = 800;
	//iMinVol = 0;
	int iCdXNum = 5;
	if (iType == 1)
	{
		iCdXNum = 3;
	}

	double iEveryPrice = (iMaxVol - iMinVol)/(iCdXNum-1);
	float iEveryHight = /*iEveryPrice*/((iMaxVol - iMinVol)/(iCdXNum-1))*((rcFill.Height())/(iMaxVol - iMinVol));
	CString strTemp;
	for (int i = 0; i < (iCdXNum);i++)
	{
		{
			sbfont.SetColor(clrCd[2]);
		}
		gpCach->DrawLine(&pnLine,PointF(rcFill.left,rcFill.top + iEveryHight*(i)),PointF(rcFill.right,rcFill.top + iEveryHight*(i)));
		strTemp.Format(_T("%.2f"),(iMaxVol - iEveryPrice*(i)));
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcFill.left - iOffL,rcFill.top + iEveryHight*(i) - 15,iOffL,15),&format,&sbfont);
		//strTemp.Format(_T("%.2f"),((iMaxVol - iEveryPrice*(i)) - iAvVol)*100/iAvVol);
		//strTemp += _T("%");
		//gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcFill.right,rcFill.top + iEveryHight*(i) - 15,iOffR,15),&formatl,&sbfont);
	}
	int iBotton = rcFill.top + iEveryHight*(iCdXNum-1);
	rcRet = rcFill;
	rcRet.bottom = iBotton;
	/////
	/////
	int iCdYNum = 8;
	int iAllTm = 0;
	int iTimeNum = 1;
	GetTradeTimeInfo(iTimeNum,iAllTm);
	iCdYNum = iAllTm/30;
	gpCach->DrawLine(&pnLine,PointF(rcFill.left,rcFill.top),PointF(rcFill.left,iBotton));
	gpCach->DrawLine(&pnLine,PointF(rcFill.right,rcFill.top),PointF(rcFill.right,iBotton));
	if (iType == 0)
	{
		strTemp = _T("");
		if (iTimeNum > 0)
		{
			strTemp = FormatTime(stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[0].inBeginTime,0);
		}
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcFill.left-60,iBotton,120,15),
			&formatc,&sbfont);
		if (iTimeNum > 0)
		{
			strTemp = FormatTime(stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iTimeNum-1].inEndTime,0);
		}
		gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcFill.right-60,iBotton,120,15),
			&formatc,&sbfont);
	}
	/////////////


	float iEveryWith = (rcFill.Width()*1.0/(iAllTm))*30.0;//rcFill.Width()/(iCdYNum);

	int iT = 0;
	int in = 1;
	for (int j = 0; j < (iCdYNum-1); j++)
	{
		gpCach->DrawLine(&pnLineY,PointF(rcFill.left + iEveryWith*(j+1),rcFill.top),PointF(rcFill.left + iEveryWith*(j+1),iBotton));
		if (iType == 0)
		{
			if (iT < (iTimeNum-1))
			{
				if (GetNextTime(stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iT].inBeginTime,in) == stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iT].inEndTime)
				{
					strTemp = FormatTime(stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iT].inEndTime,0);
					iT++;
					strTemp += (_T("/") + FormatTime((stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iT].inBeginTime),0));
					in = 0;
				}
				else
					strTemp = FormatTime(stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iT].inBeginTime,in);
			}
			else
			{
				strTemp = FormatTime(stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iT].inBeginTime,in);
			}
			in++;
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcFill.left + iEveryWith*(j+1)-60,iBotton,120,15),
				&formatc,&sbfont);
		}
	}
	int iH = rcFill.Height()*((iMaxVol - iAvVol)/(iMaxVol - iMinVol));
	gpCach->DrawLine(&pnLineYS,rcFill.left,rcFill.top+iH,rcFill.right,rcFill.top+iH);

	return rcRet;
}

void CDlgTrendChart_D::GetTradeTimeInfo(int &iQuFenNum,int& iAllTmF)
{
	iQuFenNum = iAllTmF = 0;
	if (!stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.IsEmpty())
	{
		int iAllF = 0;
		int iEnd,iBg;
		iEnd = iBg = 0;
		for (int i = 0; i < stChartDate.stSymbolTradeInfo.stTradeTime.cTradeTimeCount;i++)
		{
			iEnd = (stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[i].inEndTime/10000)*60 + (stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[i].inEndTime%10000)/100;
			iBg = (stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[i].inBeginTime/10000)*60 + (stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[i].inBeginTime%10000)/100; 
			iAllF += (iEnd - iBg);
		}
		iQuFenNum = stChartDate.stSymbolTradeInfo.stTradeTime.cTradeTimeCount;
		iAllTmF = iAllF;
	}
}
void CDlgTrendChart_D::GetMaxMinVol(float &iMaxVol,float &iMinVol,float &iPJVol)
{
	iMaxVol = 0.000000000;
	iMinVol = 0.00;
	int iNum = stChartDate.stTrendDate.mapHisTTDate.size();
	TmapHisDateSpot::iterator itMap;
	if ( iNum<= 0)
	{
		iMaxVol = 10/*stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares*0.2*/;
		iMinVol = 0;
		iPJVol = iMaxVol/2;
		return;
	}

	float fPowVol = pow(10,(double)2);

	iPJVol = 225/fPowVol;
	//iPJVol = m_mapHisTTDate.begin()->second.stcTUHisZYQ.m_uiYesSettlPrice;
	for (itMap = stChartDate.stTrendDate.mapHisTTDate.begin(); itMap != stChartDate.stTrendDate.mapHisTTDate.end();itMap++)
	{
		if (itMap->second.stHisTTimeDate.fTrunoverQty/*/fPowVol*/ > iMaxVol)
		{
			iMaxVol = itMap->second.stHisTTimeDate.fTrunoverQty/*/fPowVol*/;
		}
		//if (itMap->second.stcTUHisZYQ.m_uiTrunoverQty/fPowVol < iMinVol)
		//{
		//	iMinVol = itMap->second.stcTUHisZYQ.m_uiTrunoverQty/fPowVol;
		//}
	}
	if (IF_FZ(iMaxVol))
	{
		iMaxVol = 10;
		iMinVol = 0;
		iPJVol = iMaxVol/2;
	}
}
void CDlgTrendChart_D::GetMaxMinPrice(float &iMaxPrice,float &iMinPrice,float &iYsPrice)
{
	iMaxPrice = 0.0;
	iMinPrice = 9999999.0;
	iYsPrice = stChartDate.stTopPartDate.fYesDPrice;
	int iNum = stChartDate.stTrendDate.mapHisTTDate.size();
	TmapHisDateSpot::iterator itMap;
	if ( iNum<= 0)
	{
		
		//iMaxPrice = iMinPrice = 0.0;
		if (IF_FZ(iYsPrice))
		{
			iYsPrice = 5;
		}
		iMaxPrice = iYsPrice*1.2;
		iMinPrice = iYsPrice*0.8;
		return;
	}
	float fPowPrice = pow(10,(double)stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit);

	iYsPrice = stChartDate.stTrendDate.mapHisTTDate.begin()->second.stHisTTimeDate.fYesSettlPrice/*/fPowPrice*/;
	for (itMap = stChartDate.stTrendDate.mapHisTTDate.begin(); itMap != stChartDate.stTrendDate.mapHisTTDate.end();itMap++)
	{
		if (itMap->second.stHisTTimeDate.fClosePrice/*/fPowPrice*/ > iMaxPrice)
		{
			iMaxPrice = itMap->second.stHisTTimeDate.fClosePrice/*/fPowPrice*/;
		}
		if (itMap->second.stHisTTimeDate.fClosePrice/*/fPowPrice*/ < iMinPrice)
		{
			iMinPrice = itMap->second.stHisTTimeDate.fClosePrice/*/fPowPrice*/;
		}
		//if (itMap->second.stHisTTimeDate.fAveragePrice > iMaxPrice)
		//{
		//	iMaxPrice = itMap->second.stHisTTimeDate.fAveragePrice/*/fPowPrice*/;
		//}
		//if (itMap->second.stHisTTimeDate.fAveragePrice < iMinPrice)
		//{
		//	iMinPrice = itMap->second.stHisTTimeDate.fAveragePrice/*/fPowPrice*/;
		//}
	}


	if (IF_FZ(iMaxPrice))
	{
		iYsPrice = 5;
		iMaxPrice = iYsPrice*1.2;
		iMinPrice = iYsPrice*0.8;
	}

	if (iMaxPrice == iMinPrice)
	{
		iMaxPrice = iMaxPrice*1.2;
		iMinPrice = iMinPrice*0.8;
	}
	else
	{
		float fDifMax = abs((float)(iMaxPrice - iYsPrice));
		float fDifMin = abs((float)(iMinPrice - iYsPrice));	
		if (fDifMax > fDifMin)
		{
			iMinPrice = iYsPrice - fDifMax;
			if (iMinPrice < (0.00000001)) iMinPrice  = (0.00000001);
		}
		else
		{
			iMaxPrice = iYsPrice + fDifMin;
		}
	}
}
HisUnitK_ZYQ CDlgTrendChart_D::GetKCurCorsPos(CPoint ptCur,int &iPos)
{
	HisUnitK_ZYQ stcHsUK;
	int iNum = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	TmapHisUnitK_ZYQ::iterator itMap;
	do 
	{
		if (ptCur.x < stChartDate.stKDate.rcTime.left || ptCur.x > stChartDate.stKDate.rcTime.right)break;
		if (stChartDate.stKDate.iDisplayNum <= 0 || stChartDate.stKDate.uiCurBeginTm <= 0)break;
		if ( iNum<= 0)break;

		int iIndex = (int)((ptCur.x - stChartDate.stKDate.rcTime.left)/stChartDate.stKDate.fKEveryWithAll);
		if (iIndex >= stChartDate.stKDate.iDisplayNum)
		{
			iIndex = (stChartDate.stKDate.iDisplayNum - 1);
		}
		iPos = iIndex;
		//for (itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(stChartDate.stKDate.uiCurBeginTm); itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end();itMap++)
		//{
		//	if (iIndex-- <= 0)
		//	{
		//		break;
		//	}
		//}
		itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(stChartDate.stKDate.pHPDate[iIndex].uiDate);
		if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end())
		{
			stcHsUK = itMap->second;
		}
	} while (0);

	return stcHsUK;
}
HisDateSpot CDlgTrendChart_D::GetCurCorsPos(CPoint ptCur)
{
	HisDateSpot stcHsDate;
	int iNum = stChartDate.stTrendDate.mapHisTTDate.size();
	TmapHisDateSpot::iterator itMap;
	do 
	{
		if ( iNum<= 0)break;
		if (ptCur.x < stChartDate.stTrendDate.rcVolTrend.left || ptCur.x > stChartDate.stTrendDate.rcVolTrend.right)break;

		int iAllTm = 0;
		int iTimeNum = 1;
		GetTradeTimeInfo(iTimeNum,iAllTm);

		float iEveryWith = stChartDate.stTrendDate.rcVolTrend.Width()*1.0/iAllTm;
		int iIndex = (ptCur.x - stChartDate.stTrendDate.rcVolTrend.left)/iEveryWith ;
		if (iIndex > iNum)break;

		for (itMap = stChartDate.stTrendDate.mapHisTTDate.begin(); itMap != stChartDate.stTrendDate.mapHisTTDate.end();itMap++)
		{
			if (iIndex-- <= 0)
			{
				break;
			}
		}
		if (itMap != stChartDate.stTrendDate.mapHisTTDate.end())
		{
			stcHsDate = itMap->second;
		}
	} while (0);
	return stcHsDate;
	
}
void CDlgTrendChart_D::DrawKGP(Graphics * gpCach,CRect rcMA,CRect rcVol,CRect rcOther,CRect rcTime)
{
	if (rcMA.Width()<= 0 || rcMA.Height() <= 0 ||
		!gpCach->IsVisible(Rect(rcMA.left,rcMA.top,rcMA.Width(),rcMA.Height())))return;

	int iNum = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	if (iNum <= 0 || stChartDate.stKDate.iDisplayNum <=0 || stChartDate.stKDate.uiCurBeginTm <= 0 ||
		stChartDate.stKDate.iMaxHPos < stChartDate.stKDate.iDisplayNum)return;

	TmapHisUnitK_ZYQ::iterator mapIt = stChartDate.stKDate.mapHisUnitK_ZYQ.find(stChartDate.stKDate.uiCurBeginTm);
	if (mapIt == stChartDate.stKDate.mapHisUnitK_ZYQ.end())return;

	int iTmGap = 100;
	int iGapNum = iTmGap/stChartDate.stKDate.fKEveryWithAll;
	BOOL bMarkMax,bMarkMin;
	bMarkMax = bMarkMin = FALSE;

	float fMaEqlLine[3][5] = {0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff};
	int iFPos[3][5] = {0,0,0,0,0};
	float fTemp1 ,fTemp2;
	CString strRT;
	CString strKey;
	//OutputDebugString(_T("*******************************"));
	for (int i = 0;mapIt != stChartDate.stKDate.mapHisUnitK_ZYQ.end()&& i < stChartDate.stKDate.iDisplayNum;(mapIt++,i++))
	{
		DrawKLine(gpCach,stChartDate.stKDate.pHPDate[i].iHoriPos,rcMA,&(mapIt->second),stChartDate.stKDate.iKEveryWith,bMarkMax,bMarkMin);
		DrawKVOL(gpCach,stChartDate.stKDate.pHPDate[i].iHoriPos,rcVol,&(mapIt->second),stChartDate.stKDate.iKEveryWith);

		if (i%iGapNum == 0)
		{
			DrawKTime(gpCach,stChartDate.stKDate.pHPDate[i].iHoriPos,rcTime,&(mapIt->second),stChartDate.stKDate.iKEveryWith);
		}
		for (int iGp = 0;iGp < 3;iGp++)
		{
			CString strTemp = _T("");
			strKey = stChartDate.stKDate.vecIndexDisplay[iGp];
			for (int iIndex = 0; iIndex < stChartDate.stKDate.MapIndexDateHead[strKey].uiCurves; iIndex++)
			{
				if (stChartDate.stKDate.MapIndexDateHead[strKey].uiEqualLineType[iIndex] == 0)
				{
					if (i == 0)
					{
						fMaEqlLine[iGp][iIndex] = mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex];
						iFPos[iGp][iIndex] = stChartDate.stKDate.pHPDate[i].iHoriPos;
						continue;
					}
					fTemp1 = fMaEqlLine[iGp][iIndex];
					fTemp2 = mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex];
					if (fMaEqlLine[iGp][iIndex] == 0x7fffffff /*|| fMaEqlLine[iGp][iIndex] == 0x00000000 */||
						mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex] == 0x7fffffff /*|| */
						/*mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex] == 0x00000000*/)
					{
						fMaEqlLine[iGp][iIndex] =mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex];
						iFPos[iGp][iIndex] = stChartDate.stKDate.pHPDate[i].iHoriPos;
						continue;
					}

					DrawLinkLine(gpCach,stChartDate.stKDate.pHPDate[i].iHoriPos,iGp,stChartDate.stKDate.MapIndexDateHead[strKey].dwclr[iIndex],
						stChartDate.stKDate.MapIndexDateHead[strKey].rcItem,iFPos[iGp][iIndex],fMaEqlLine[iGp][iIndex],mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex],stChartDate.stKDate.iKEveryWith);
					fMaEqlLine[iGp][iIndex] =mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex];
					iFPos[iGp][iIndex] = stChartDate.stKDate.pHPDate[i].iHoriPos;
				}
				else if (stChartDate.stKDate.MapIndexDateHead[strKey].uiEqualLineType[iIndex] == 1)
				{
					DrawBarLine(gpCach,stChartDate.stKDate.pHPDate[i].iHoriPos,iGp,stChartDate.stKDate.MapIndexDateHead[strKey].rcItem,
						mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex],stChartDate.stKDate.iKEveryWith);
				}
				strRT.Format(_T(" %d-%.3f,  "),stChartDate.stKDate.MapIndexDateHead[strKey].uiEqualLineType[iIndex],
					mapIt->second.mapIndexDate[strKey].fEqualLine[iIndex]);
				strTemp += strRT;
			}
			strTemp+= _T("\r\n");
			if (iGp == 2)
			{
				//OutputDebugString(strTemp);
			}
		}
	}
	//OutputDebugString(_T("*******************************"));
}
void CDlgTrendChart_D::DrawIndexDate(UINT iGpNum, UINT iHPos)
{
	//CString strTemp = _T("");
	//for (int iIndex = 0; iIndex < stChartDate.stKDate.MapIndexDateHead[stChartDate.stKDate.vecIndexDisplay[0]].uiCurves; iIndex++)
	//{
	//	if (stChartDate.stKDate.MapIndexDateHead[stChartDate.stKDate.vecIndexDisplay[0]].uiEqualLineType[iIndex] == LINKLINE)
	//	{
	//		if (i == 0)
	//		{
	//			fMaEqlLine[iIndex] = mapIt->second.mapIndexDate[stChartDate.stKDate.vecIndexDisplay[0]].fEqualLine[iIndex];
	//			iFPos[iIndex] = stChartDate.stKDate.pHoriPos[i];
	//			continue;
	//		}
	//		fTemp1 = fMaEqlLine[iIndex];
	//		fTemp2 = mapIt->second.mapIndexDate[stChartDate.stKDate.vecIndexDisplay[0]].fEqualLine[iIndex];
	//		if (fMaEqlLine[iIndex] == 0x7fffffff || fMaEqlLine[iIndex] == 0x00000000 ||
	//			mapIt->second.mapIndexDate[stChartDate.stKDate.vecIndexDisplay[0]].fEqualLine[iIndex] == 0x7fffffff || 
	//			mapIt->second.mapIndexDate[stChartDate.stKDate.vecIndexDisplay[0]].fEqualLine[iIndex] == 0x00000000)
	//		{
	//			fMaEqlLine[iIndex] =mapIt->second.mapIndexDate[stChartDate.stKDate.vecIndexDisplay[0]].fEqualLine[iIndex];
	//			iFPos[iIndex] = stChartDate.stKDate.pHoriPos[i];
	//			continue;
	//		}

	//		DrawLinkLine(gpCach,stChartDate.stKDate.pHoriPos[i],stChartDate.stKDate.MapIndexDateHead[stChartDate.stKDate.vecIndexDisplay[0]].dwclr[iIndex],
	//			stChartDate.stKDate.MapIndexDateHead[stChartDate.stKDate.vecIndexDisplay[0]].rcItem,iFPos[iIndex],fMaEqlLine[iIndex],mapIt->second.mapIndexDate[stChartDate.stKDate.vecIndexDisplay[0]].fEqualLine[iIndex],stChartDate.stKDate.iKEveryWith);
	//		fMaEqlLine[iIndex] =mapIt->second.mapIndexDate[stChartDate.stKDate.vecIndexDisplay[0]].fEqualLine[iIndex];
	//		iFPos[iIndex] = stChartDate.stKDate.pHoriPos[i];
	//	}
	//	strRT.Format(_T("%.3f  "),mapIt->second.mapIndexDate[stChartDate.stKDate.vecIndexDisplay[0]].fEqualLine[iIndex]);
	//	strTemp += strRT;
	//}
	//strTemp+= _T("\r\n");
	//OutputDebugString(strTemp);
}
int CDlgTrendChart_D::GetHPos(float fValue,CRect rcDraw,float fMaxValue,float fMinValue)
{
	if(fMaxValue == fMinValue)return rcDraw.top;
	int nStation;
	nStation = (int)(( ((double)(fValue - fMinValue))/((fMaxValue- fMinValue)))*rcDraw.Height());
	//if (nStation == 0) nStation = 1;
	nStation = rcDraw.bottom - nStation;
	if(nStation>rcDraw.bottom)nStation = rcDraw.bottom;
	if(nStation<rcDraw.top)nStation = rcDraw.top;
	return nStation;
}
void CDlgTrendChart_D::DrawKCorssTips(Graphics * gpCach)
{
	CRect rcMA,rcVOL_K,rcOther,rcTips;
	CRect rcMAT,rcVOL_KT,rcOtherT;
	int iTitleH = 18;
	//rcMA = stChartDate.stKDate.rcKTrend;
	//rcVOL_K = stChartDate.stKDate.rcVOLTrend;
	//rcOther = stChartDate.stKDate.rcOther;
	rcMA = stChartDate.stKDate.GetItemRect(0);
	rcMAT = CRect(rcMA.left,rcMA.top - iTitleH,rcMA.right,rcMA.top);
	rcVOL_K = stChartDate.stKDate.GetItemRect(1);
	rcVOL_KT = CRect(rcVOL_K.left,rcVOL_K.top - iTitleH,rcVOL_K.right,rcVOL_K.top);
	rcOther = stChartDate.stKDate.GetItemRect(2);
	rcOtherT = CRect(rcOther.left,rcOther.top - iTitleH,rcOther.right,rcOther.top);
	rcTips = stChartDate.stKDate.rcTips;
	CString strMAT,strVOLT,strOtherT,strKey;
	//strMAT = strVOLT = strOtherT = (stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode);
	//strMAT += _T("-");
	//strVOLT += _T("-");
	//strOtherT += _T("-");
	USES_CONVERSION;
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format,formatl,formatc;
	format.SetAlignment(StringAlignmentFar);
	formatl.SetAlignment(StringAlignmentNear);
	int iIndexLineNum = stChartDate.stKDate.vecIndexDisplay.size();
	CString strSymbol = stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolName + _T("[") + stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode + _T("]");
	for (int i = 0; i < 3;i++)
	{
		strKey = stChartDate.stKDate.vecIndexDisplay[i];
		if (i == 0)
		{
			strMAT = strSymbol + _T(" ") + stChartDate.stKDate.MapIndexDateHead[strKey].strTitle;
		}
		else if (i == 1)
		{
			strVOLT = strSymbol + _T(" ") + stChartDate.stKDate.MapIndexDateHead[strKey].strTitle;
		}
		else
			strOtherT = strSymbol + _T(" ") + stChartDate.stKDate.MapIndexDateHead[strKey].strTitle;
	}

	SolidBrush sbrTBK(Color(255,255,255,0));
	gpCach->DrawString(T2W(strMAT),-1,&ft,RectF(rcMAT.left,rcMAT.top,rcMAT.Width(),iTitleH),
		&formatl,&sbrTBK);
	gpCach->DrawString(T2W(strVOLT),-1,&ft,RectF(rcVOL_KT.left,rcVOL_KT.top,rcVOL_KT.Width(),iTitleH),
		&formatl,&sbrTBK);
	gpCach->DrawString(T2W(strOtherT),-1,&ft,RectF(rcOtherT.left,rcOtherT.top,rcOtherT.Width(),iTitleH),
		&formatl,&sbrTBK);

	rcMAT.left += (CalculateString(strMAT,9)+5);
	rcVOL_KT.left += (CalculateString(strVOLT,9)+5);
	rcOtherT.left += (CalculateString(strOtherT,9)+5);

	if (m_bTips)
	{
		Pen pnLine(Color(255,255,255,255));
		int iXPos = 0;
		//CPoint
		GetCursorPos(&m_ptCur);
		ScreenToClient(&m_ptCur);
		HisUnitK_ZYQ stcHisDate = GetKCurCorsPos(m_ptCur,iXPos);
		if (stcHisDate.uiDate > 0&&(iXPos >=0 && iXPos < stChartDate.stKDate.iDisplayNum)&&iXPos < stChartDate.stKDate.iMaxHPos)
		{
			CString strTemp,strTemp1;
			//////////////////
			for (int i = 0; i < 3;i++)
			{
				strKey = stChartDate.stKDate.vecIndexDisplay[i];
				strTemp = _T("");
				strTemp1 = _T("");
				for (int x = 0; x < stChartDate.stKDate.MapIndexDateHead[strKey].uiCurves;x++)
				{
					strTemp.Format(_T("=%.2f "),stcHisDate.mapIndexDate[strKey].fEqualLine[x]);
					strTemp1 = (stChartDate.stKDate.MapIndexDateHead[strKey].strEqlLineTitel[x] + strTemp);
					if (i == 0)
					{
						sbrTBK.SetColor(stChartDate.stKDate.MapIndexDateHead[strKey].dwclr[x]);
						gpCach->DrawString(T2W(strTemp1),-1,&ft,RectF(rcMAT.left,rcMAT.top,rcMAT.Width(),iTitleH),
							&formatl,&sbrTBK);
						rcMAT.left += (CalculateString(strTemp1,9)+5);
					}
					else if (i == 1)
					{
						if (x == 0)
						{
							//sbrTBK.SetColor(Color(255,255,255,0));
							continue;
						}
						else
						{
							sbrTBK.SetColor(stChartDate.stKDate.MapIndexDateHead[strKey].dwclr[x]);
						}
						gpCach->DrawString(T2W(strTemp1),-1,&ft,RectF(rcVOL_KT.left,rcVOL_KT.top,rcVOL_KT.Width(),iTitleH),
							&formatl,&sbrTBK);
						rcVOL_KT.left += (CalculateString(strTemp1,9)+5);
					}
					else
					{
						sbrTBK.SetColor(stChartDate.stKDate.MapIndexDateHead[strKey].dwclr[x]);
						gpCach->DrawString(T2W(strTemp1),-1,&ft,RectF(rcOtherT.left,rcOtherT.top,rcOtherT.Width(),iTitleH),
							&formatl,&sbrTBK);
						rcOtherT.left += (CalculateString(strTemp1,9)+5);
					}
				}	
			}
			/////////////////////////
			float fYsDayPrice,fMaxPrice,fMinPrice;
			stChartDate.stKDate.GetMaxMinValue(fMaxPrice,fMinPrice,fYsDayPrice,0);

			int iXOff = stChartDate.stKDate.pHPDate[iXPos].iHoriPos + stChartDate.stKDate.iKEveryWith/2;
			gpCach->DrawLine(&pnLine,PointF(iXOff,rcMA.top),PointF(iXOff,rcMA.bottom));
			gpCach->DrawLine(&pnLine,PointF(iXOff,rcVOL_K.top),PointF(iXOff,rcVOL_K.bottom));
			gpCach->DrawLine(&pnLine,PointF(iXOff,rcOther.top),PointF(iXOff,rcOther.bottom));
			int iClosePos = GetHPos(stcHisDate.fClosePrice,rcMA,fMaxPrice,fMinPrice);//收盘价位置
			gpCach->DrawLine(&pnLine,PointF(rcMA.left,iClosePos),PointF(rcMA.right,iClosePos));
	
			CString strDisPlay[] = {_T("时间"),_T("开盘价"),_T("最高价"),_T("最低价"),_T("收盘价"),_T("均价"),
				_T("换手率"),_T("涨幅"),_T("成交量"),_T("成交额")};
			int iNum = 10;
			int iEveryHight = 15;
			int iWith = 60;
			rcTips.bottom = rcTips.top + iEveryHight*iNum*2;

			Color clrBk[] = {Color(255,0,0,0),Color(255,190,190,190)};
			Color clrUpDown[] = {Color(255,255,255,255),Color(255,255,0,0),Color(255,0,255,0)};
			Color clrVol = Color(255,255,255,74);
			Color clrTitle = Color(255,0,255,255);
			SolidBrush sbrBk(clrBk[0]);
			Pen pnRc(clrBk[1],1.0);
			gpCach->FillRectangle(&sbrBk,Rect(rcTips.left,rcTips.top,rcTips.Width(),rcTips.Height()));
			gpCach->DrawRectangle(&pnRc,Rect(rcTips.left,rcTips.top,rcTips.Width(),rcTips.Height()));

			
			int i = 0;
			int y = 0;
			int iDc = stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit;
			float fPowPrice = pow(10,(double)iDc);
			float fPowVol = pow(10,(double)2);
			Gdiplus::SolidBrush sbfont(clrTitle);
			Gdiplus::SolidBrush sbUpDown(clrUpDown[0]);

			//Gdiplus::FontFamily fm(T2W(_T("宋体")));
			//Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
			//StringFormat format,formatl,formatc;
			//format.SetAlignment(StringAlignmentFar);
			//formatl.SetAlignment(StringAlignmentNear);
			strTemp = CDateManage::Instance()->FormatTimeY(stcHisDate.uiDate,0);
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top,rcTips.Width(),iEveryHight),
				&format,&sbfont);
			//strTemp.Format(_T("%lld"),stcHisDate.uiDate);
			strTemp = CDateManage::Instance()->FormatTimeY(stcHisDate.uiDate,1);//.Format(_T("%d/%d/%d %d:%d"),stcHisDate.uiDate/1000000);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbfont);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.fOpenPrice*1.0,iDc);
			sbUpDown.SetColor(clrUpDown[CompValue(stcHisDate.fBeforClosePrice,stcHisDate.fOpenPrice)]);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.fMaxPrice,iDc);
			sbUpDown.SetColor(clrUpDown[CompValue(stcHisDate.fBeforClosePrice,stcHisDate.fMaxPrice)]);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.fMinPrice,iDc);
			sbUpDown.SetColor(clrUpDown[CompValue(stcHisDate.fBeforClosePrice,stcHisDate.fMinPrice)]);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.fClosePrice,iDc);
			sbUpDown.SetColor(clrUpDown[CompValue(stcHisDate.fBeforClosePrice,stcHisDate.fClosePrice)]);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.fJunPrice,iDc);
			sbUpDown.SetColor(clrUpDown[CompValue(stcHisDate.fBeforClosePrice,stcHisDate.fJunPrice)]);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.fTurnoverRate,3);
			sbUpDown.SetColor(clrVol);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			float fZhangFu = stcHisDate.fClosePrice - stcHisDate.fBeforClosePrice;
			strTemp = DoubleToString(fZhangFu,3);
			sbUpDown.SetColor(clrUpDown[CompValue(0.00000001,fZhangFu)]);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.fTrunoverQty,2);
			sbUpDown.SetColor(clrVol);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.fTrunoverMoney,0);
			sbUpDown.SetColor(clrVol);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);
		}
	}
}
int CDlgTrendChart_D::CompValue(float ValueS,float ValueD)
{
	if (ValueD > ValueS)
	{
		return 1;
	}
	else if (ValueD < ValueS)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}
void CDlgTrendChart_D::DrawKTime(Graphics * gpCach,int iXPos,CRect rcDraw,HisUnitK_ZYQ *pstcHisUK,int iKEveryWith)
{
	if (pstcHisUK == NULL || iXPos < 0 || pstcHisUK->uiDate <= 0) return;

	int iOffHight = 3;
	int iFontWith = 100;
	int iFontHight = 15;
	int nMiddle = iXPos + iKEveryWith/2;
	USES_CONVERSION;
	Pen pnLine(Color(255,255,0,0));
	SolidBrush sbrFont(Color(255,233,233,233));
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat formatC;
	formatC.SetAlignment(StringAlignmentCenter);
	CString strTemp;
	strTemp = CDateManage::Instance()->FormatTimeY(pstcHisUK->uiDate,0);//Format(_T("%lld"),stcHisUK.uiDate%1000000);
	gpCach->DrawLine(&pnLine,PointF(nMiddle,rcDraw.top),PointF(nMiddle,rcDraw.top + iOffHight));
	gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(nMiddle - iFontWith/2,rcDraw.top + iOffHight,iFontWith,iFontHight),
		&formatC,&sbrFont);
}
void CDlgTrendChart_D::DrawKVOL(Graphics * gpCach,int iXPos,CRect rcDraw,HisUnitK_ZYQ *pstcHisUK,int iKEveryWith)
{
	if (pstcHisUK == NULL || iXPos < 0 || pstcHisUK->uiDate <= 0) return;

	float fYsDayPrice,fMaxPrice,fMinPrice;
	stChartDate.stKDate.GetMaxMinValue(fMaxPrice,fMinPrice,fYsDayPrice,1);

	Color clrUD[2] = {Color(255,255,0,0),Color(255,0,255,255)};
	Pen pnLine(clrUD[0]);
	SolidBrush sbrFill(clrUD[1]);
	int nVolBarBottomValue = rcDraw.bottom-2; 
	if (pstcHisUK->fTrunoverQty == 1.00)
	{
		int i = 0;
		i += 10;
	}
	int nVolTop = GetHPos(pstcHisUK->fTrunoverQty,rcDraw,fMaxPrice,fMinPrice);
	if(nVolTop >= nVolBarBottomValue)
	{
		nVolTop = nVolBarBottomValue-1;
	}
	if( nVolTop != nVolBarBottomValue)
	{
		Rect rcFill(iXPos,nVolTop,iKEveryWith,nVolBarBottomValue - nVolTop);
		if(pstcHisUK->fOpenPrice <= pstcHisUK->fClosePrice)
		{	
			gpCach->DrawRectangle(&pnLine,rcFill);
		}
		else
		{
			gpCach->FillRectangle(&sbrFill,rcFill);
		}
	}
}
void CDlgTrendChart_D::DrawLinkLine(Graphics * gpCach,int iXPos,UINT uiIndex,DWORD dwclr,CRect rcDraw,int iXFrontPos,float fEqlFront,float fEqlCur,int iKEveryWith)
{
	if (iXPos < 0 || iXFrontPos < 0)return;

	float fYsDayPrice,fMaxPrice,fMinPrice;
	stChartDate.stKDate.GetMaxMinValue(fMaxPrice,fMinPrice,fYsDayPrice,uiIndex);

	int nEqlFrontPos,nEqlCurPos;
	nEqlFrontPos = GetHPos(fEqlFront,rcDraw,fMaxPrice,fMinPrice);
	nEqlCurPos = GetHPos(fEqlCur,rcDraw,fMaxPrice,fMinPrice);

	Pen pnLine(dwclr);
	gpCach->DrawLine(&pnLine,Point(iXFrontPos + iKEveryWith/2,nEqlFrontPos),Point(iXPos + iKEveryWith/2,nEqlCurPos));
}
void CDlgTrendChart_D::DrawBarLine(Graphics * gpCach,int iXPos,UINT uiIndex,CRect rcDraw,float fCurVale,int iKEveryWith)
{
	if (iXPos < 0)return;
	float fBaseValue = 0.0000000;
	float fYsDayPrice,fMaxPrice,fMinPrice;
	stChartDate.stKDate.GetMaxMinValue(fMaxPrice,fMinPrice,fYsDayPrice,uiIndex);

	int nBasePos,nCurPos;
	nBasePos = GetHPos(fBaseValue,rcDraw,fMaxPrice,fMinPrice);
	nCurPos = GetHPos(fCurVale,rcDraw,fMaxPrice,fMinPrice);

	Pen pnLine(Color(255,255,0,0));
	if (nBasePos < nCurPos)
	{
		pnLine.SetColor(Color(255,0,255,0));
	}
	gpCach->DrawLine(&pnLine,Point(iXPos + iKEveryWith/2,nBasePos),Point(iXPos + iKEveryWith/2,nCurPos));
}
void CDlgTrendChart_D::DrawKLine(Graphics * gpCach,int iXPos,CRect rcDraw,HisUnitK_ZYQ *pstcHisUK,int iKEveryWith,BOOL &bMarkMax,BOOL &bMarkMin)
{
	if (pstcHisUK==NULL || iXPos < 0 || pstcHisUK->uiDate <= 0) return;

	DWORD clrUD = Color::MakeARGB(255,0,255,255);
	if(pstcHisUK->fOpenPrice <= pstcHisUK->fClosePrice)//上涨
		clrUD = Color::MakeARGB(255,255,0,0);
	//else//下跌
	//	clrLine =  CInterfaceManager::Instance()->GetSysColor(ID_KLINEDOWN_COLOR);

	Pen penLine(Color(clrUD),1.0);
	Gdiplus::SolidBrush sbrFill(Color((ARGB)clrUD));

	float fYsDayPrice,fMaxPrice,fMinPrice;
	stChartDate.stKDate.GetMaxMinValue(fMaxPrice,fMinPrice,fYsDayPrice,0);

	int nOpenStation,nCloseStation;//矩形区域
	nOpenStation = GetHPos(pstcHisUK->fOpenPrice,rcDraw,fMaxPrice,fMinPrice);//开盘价位置
	nCloseStation = GetHPos(pstcHisUK->fClosePrice,rcDraw,fMaxPrice,fMinPrice);//收盘价位置
	if(nOpenStation > nCloseStation)//上涨
	{//空心
		gpCach->DrawRectangle(&penLine,Rect(iXPos,nCloseStation,iKEveryWith,nOpenStation - nCloseStation));
	}
	else if(nOpenStation < nCloseStation)//下跌
	{
		gpCach->FillRectangle(&sbrFill,Rect(iXPos,nOpenStation,iKEveryWith,nCloseStation - nOpenStation));
	}
	else//相等
	{
		gpCach->DrawLine(&penLine,iXPos,nOpenStation,iXPos + iKEveryWith,nCloseStation);
	}
	int nMiddle;//矩形中间区域
	nMiddle = iXPos + iKEveryWith/2;
	int nHighStation,nLowStation;//最高最低价
	nHighStation = GetHPos(pstcHisUK->fMaxPrice,rcDraw,fMaxPrice,fMinPrice);
	nLowStation = GetHPos(pstcHisUK->fMinPrice,rcDraw,fMaxPrice,fMinPrice);
	if(nOpenStation< nCloseStation)
	{//交换位置
		int nTemp;
		nTemp = nOpenStation;
		nOpenStation = nCloseStation;
		nCloseStation = nTemp;
	}
	gpCach->DrawLine(&penLine,nMiddle,nHighStation,nMiddle,nCloseStation-1);
	gpCach->DrawLine(&penLine,nMiddle,nLowStation,nMiddle,nOpenStation+1);
	if (bMarkMax && bMarkMin)return;
	if (fMaxPrice == pstcHisUK->fMaxPrice && bMarkMax)return;
	if (fMinPrice == pstcHisUK->fMinPrice && bMarkMin)return;

	if (fMaxPrice == pstcHisUK->fMaxPrice || fMinPrice == pstcHisUK->fMinPrice)
	{
		USES_CONVERSION;
		Color clrMn(255,255,255,255);
		Gdiplus::SolidBrush sbfont(clrMn);
		Gdiplus::Pen pnLineMN(clrMn);
		Gdiplus::FontFamily fm(T2W(_T("宋体")));
		Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),10/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
		StringFormat formatF,formatN;
		formatF.SetAlignment(StringAlignmentFar);
		formatN.SetAlignment(StringAlignmentNear);
		CString strTemp;
		int iOffX = 15;
		int iOffY = 5;
		int iStrWith = 60;
		int iOffH = 0;
		int iOffHF = 0;
		int iOffHFS = 0;
		int iFontH = 15;
		if (fMaxPrice == pstcHisUK->fMaxPrice)
		{
			iOffH = nHighStation + iOffY;
			iOffHFS = nHighStation;
			strTemp.Format(_T("%.2f"),fMaxPrice);
			bMarkMax = TRUE;
		}
		else
		{
			iOffH = nLowStation - iOffY;
			iOffHFS = nLowStation;
			iOffHF =  - iFontH;
			strTemp.Format(_T("%.2f"),fMinPrice);
			bMarkMin = TRUE;
		}
		
		if (rcDraw.right - nMiddle < iStrWith)
		{
			gpCach->DrawLine(&pnLineMN,PointF(nMiddle,iOffHFS),PointF(nMiddle - iOffX,iOffH));
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(nMiddle - iOffX - iStrWith,iOffHFS + iOffHF - 1,iStrWith,iFontH),
				&formatF,&sbfont);
		}
		else
		{
			gpCach->DrawLine(&pnLineMN,PointF(nMiddle,iOffHFS),PointF(nMiddle + iOffX,iOffH));
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(nMiddle + iOffX,iOffHFS + iOffHF - 1,iStrWith,iFontH),
				&formatN,&sbfont);
		}
		
	}
}
void CDlgTrendChart_D::CaluPos_K(CRect rcDraw)
{
	//if (stChartDate.stKDate.uiCurBeginTm != 0)return;
	DWORD dwBG = GetTickCount();
	float fMaxV,fMinV,fMarkV;
	int iNum = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	std::map<UINT,HisDateSpot>::iterator itMap;
	if ( iNum<= 0)
	{
		GetMaxMinPrice_KMAP(fMaxV,fMinV,fMarkV);
		stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,0);
		GetMaxMinVol_KMAP(fMaxV,fMinV,fMarkV);
		stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,1);
		GetMaxMinOther_KMAP(fMaxV,fMinV,fMarkV);
		stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,2);
		return;
	}

	int iDisplayNum = rcDraw.Width()/stChartDate.stKDate.fKEveryWithAll;
	if (iDisplayNum <= 0)
	{
		stChartDate.stKDate.uiCurBeginTm = 0;
		stChartDate.stKDate.iDisplayNum = 0;
		return;
	}
	int iNum1 = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	//stChartDate.stKDate.uiCurBeginTm = stChartDate.stKDate.mapHisUnitK_ZYQ[113000].uiDate;//m_stcKDate_ZYQ.mapHisUnitK_ZYQ.begin()->first;

	if (stChartDate.stKDate.iMaxHPos < iDisplayNum)
	{
		stChartDate.stKDate.iMaxHPos = iDisplayNum;
		if (stChartDate.stKDate.pHPDate)
		{
			delete []stChartDate.stKDate.pHPDate;
			stChartDate.stKDate.pHPDate = NULL;
		}
		stChartDate.stKDate.pHPDate = new HoriPosDate[iDisplayNum];
	}
	stChartDate.stKDate.iDisplayNum = iDisplayNum;

	int iSize = (int)stChartDate.stKDate.fKEveryWithAll;
	if(iSize >66)
	{
		stChartDate.stKDate.iKEveryWith = 61;
		stChartDate.stKDate.fKEveryWithAll = 65;
	}
	else if(iSize>20)
		stChartDate.stKDate.iKEveryWith = (int)(stChartDate.stKDate.fKEveryWithAll/1.3);
	else if(iSize>17)
		stChartDate.stKDate.iKEveryWith = 15;
	else if(iSize >14)
		stChartDate.stKDate.iKEveryWith = 13;
	else if(iSize >12)
		stChartDate.stKDate.iKEveryWith = 11;
	else if(iSize > 10)
		stChartDate.stKDate.iKEveryWith = 9;
	else if(iSize >8)
		stChartDate.stKDate.iKEveryWith = 4;
	else if (iSize >5)
		stChartDate.stKDate.iKEveryWith = 5;
	else if(iSize >2)
		stChartDate.stKDate.iKEveryWith = 3;
	else stChartDate.stKDate.iKEveryWith = 1;

	if (stChartDate.stKDate.mapHisUnitK_ZYQ.size() > stChartDate.stKDate.iDisplayNum)
	{
		TmapHisUnitK_ZYQ::reverse_iterator rItMap = stChartDate.stKDate.mapHisUnitK_ZYQ.rbegin();
		int iDisplayNum = stChartDate.stKDate.iDisplayNum;
		while((iDisplayNum--) > 1)
		{
			if (rItMap != stChartDate.stKDate.mapHisUnitK_ZYQ.rend())
			{
				rItMap++;
			}	
		}

		if (stChartDate.stKDate.uiCurBeginTm == 0 || stChartDate.stKDate.uiCurBeginTm > rItMap->second.uiDate)
		{	
			stChartDate.stKDate.uiCurBeginTm = rItMap->second.uiDate;
		}
	}
	else/* if(stChartDate.stKDate.uiCurBeginTm == 0)*/
	{
		TmapHisUnitK_ZYQ::iterator itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin();
		if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end())
		{
			stChartDate.stKDate.uiCurBeginTm = itMap->second.uiDate;
		}
	}

	//if (stChartDate.stKDate.uiCurBeginTm != stChartDate.stKDate.pHPDate[0].uiDate)
	{
		stChartDate.stKDate.mapT_N.clear();
		TmapHisUnitK_ZYQ::iterator itMapT = stChartDate.stKDate.mapHisUnitK_ZYQ.find(stChartDate.stKDate.uiCurBeginTm);
		for (int i = 0; i < stChartDate.stKDate.iDisplayNum; i++)
		{
			stChartDate.stKDate.pHPDate[i].iHoriPos = rcDraw.left + (int)(i*stChartDate.stKDate.fKEveryWithAll);
			if (itMapT != stChartDate.stKDate.mapHisUnitK_ZYQ.end())
			{
				stChartDate.stKDate.pHPDate[i].uiDate = itMapT->second.uiDate;
				itMapT++;
			}
			else
			{
				stChartDate.stKDate.pHPDate[i].uiDate = 0;
			}
			if (stChartDate.stKDate.pHPDate[i].uiDate != 0)
			{
				stChartDate.stKDate.mapT_N[stChartDate.stKDate.pHPDate[i].uiDate] = i;
			}
		}
	}
	//GetMaxMinPrice_KMAP(stChartDate.stKDate.fMaxPrice_K,stChartDate.stKDate.fMinPrice_K,stChartDate.stKDate.fYsPrice_K);
	//GetMaxMinVol_KMAP(stChartDate.stKDate.fMaxVol_K,stChartDate.stKDate.fMinVol_K,stChartDate.stKDate.fPPVol_K);
	//GetMaxMinOther_KMAP(stChartDate.stKDate.fMaxOther_K,stChartDate.stKDate.fMinOther_K,stChartDate.stKDate.fPPOther_K);
	GetMaxMinPrice_KMAP(fMaxV,fMinV,fMarkV);
	stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,0);
	GetMaxMinVol_KMAP(fMaxV,fMinV,fMarkV);
	stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,1);
	GetMaxMinOther_KMAP(fMaxV,fMinV,fMarkV);
	stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,2);

	
}
void CDlgTrendChart_D::CaluPos(CRect rcTT,CRect rcVOl)
{
	DWORD dwBG = GetTickCount();
	
	int iNum = stChartDate.stTrendDate.mapHisTTDate.size();
	TmapHisDateSpot::iterator itMap;
	if ( iNum<= 0)return;
	
	int iAllTm = 0;
	int iTimeNum = 1;
	GetTradeTimeInfo(iTimeNum,iAllTm);


	float iEveryWith = rcTT.Width()*1.0/iAllTm;
	
	float iMaxPrice,iMinPrice,iYsPrice;
	//GetMaxMinPrice(iMaxPrice,iMinPrice,iYsPrice);
	iYsPrice = stChartDate.stTrendDate.fYsPrice;
	iMaxPrice = stChartDate.stTrendDate.fMaxPrice;
	iMinPrice = stChartDate.stTrendDate.fMinPrice;

	float iMaxVol,iMinVol,iPPVol;
	//GetMaxMinVol(iMaxVol,iMinVol,iPPVol);
	iMaxVol = stChartDate.stTrendDate.fMaxVol;
	iMinVol = stChartDate.stTrendDate.fMinVol;
	iPPVol = stChartDate.stTrendDate.fPPVol;
	float iEveryVolH = rcVOl.Height()/(iMaxVol - iMinVol);
	float iEveryPriceH = rcTT.Height()/(iMaxPrice-iMinPrice);

	float fPowPrice = pow(10,(double)stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit);
	float fPowVol = pow(10,(double)2);

	int i = 0;
	for (itMap = stChartDate.stTrendDate.mapHisTTDate.begin(); itMap != stChartDate.stTrendDate.mapHisTTDate.end();itMap++)
	{
		itMap->second.xPos = rcTT.left + i*iEveryWith;
		itMap->second.yTTPos = rcTT.top + iEveryPriceH*(iMaxPrice - itMap->second.stHisTTimeDate.fClosePrice/*/fPowPrice*/);
		itMap->second.yTTPos_Jun = rcTT.top + iEveryPriceH*(iMaxPrice - itMap->second.stHisTTimeDate.fAveragePrice/*/fPowPrice*/);
		if (itMap->second.yTTPos_Jun <  rcTT.top)itMap->second.yTTPos_Jun = rcTT.top + 1;
		else if(itMap->second.yTTPos_Jun >  rcTT.bottom)itMap->second.yTTPos_Jun = rcTT.bottom - 1;
		itMap->second.yVolPos = rcVOl.bottom - iEveryVolH*(itMap->second.stHisTTimeDate.fTrunoverQty/*/fPowVol*/ - iMinVol);
		i++;
	}

	
}
void CDlgTrendChart_D::DrawTTGp(Graphics * gpCach,CRect rcDraw)
{
	int iNum = stChartDate.stTrendDate.mapHisTTDate.size();
	TmapHisDateSpot::iterator itMap;
	HisDateSpot stcHsDate;
	 Color clrLine[] = {Color(255,255,255,255),Color(255,255,255,47)};
	Pen pnLine1(clrLine[0],1.0);
	Pen pnLine2(clrLine[1],1.0);
	PointF ptBefor1(0,0);
	PointF ptBefor2(0,0);
	PointF ptBefor3(0,0);
	for (itMap = stChartDate.stTrendDate.mapHisTTDate.begin(); itMap != stChartDate.stTrendDate.mapHisTTDate.end();itMap++)
	{
		stcHsDate = itMap->second;

		ptBefor3.X = stcHsDate.xPos;
		ptBefor3.Y = stcHsDate.yVolPos;
		gpCach->DrawLine(&pnLine2,PointF(ptBefor3.X,rcDraw.bottom),ptBefor3);
		if (ptBefor1.X  <= 0.0000)
		{
			ptBefor1.X = stcHsDate.xPos;
			ptBefor1.Y = stcHsDate.yTTPos;
			ptBefor2.X = stcHsDate.xPos;
			ptBefor2.Y = stcHsDate.yTTPos_Jun;
			continue;
		}
		gpCach->DrawLine(&pnLine1,ptBefor1,PointF(stcHsDate.xPos,stcHsDate.yTTPos));
		ptBefor1.X = stcHsDate.xPos;
		ptBefor1.Y = stcHsDate.yTTPos;

		gpCach->DrawLine(&pnLine2,ptBefor2,PointF(stcHsDate.xPos,stcHsDate.yTTPos_Jun));
		ptBefor2.X = stcHsDate.xPos;
		ptBefor2.Y = stcHsDate.yTTPos_Jun;
	}
}

void CDlgTrendChart_D::DrawCorssTips(Graphics * gpCach)
{
	if (m_bTips)
	{
		CRect rcTT,rcVol,rcTips;
		rcTT = stChartDate.stTrendDate.rcTimeTrend;
		rcVol = stChartDate.stTrendDate.rcVolTrend;
		rcTips = stChartDate.stTrendDate.rcTips;
		Pen pnLine(Color(255,255,255,255));
		HisDateSpot stcHisDate = GetCurCorsPos(m_ptCur);
		if (m_ptCur.x >= rcVol.left && m_ptCur.x <= rcVol.right && stcHisDate.stHisTTimeDate.iTradeDay > 0
			&&(m_ptCur.y >= rcVol.top && m_ptCur.y <= rcVol.bottom||m_ptCur.y >= rcTT.top && m_ptCur.y <= rcTT.bottom))
		{
			gpCach->DrawLine(&pnLine,PointF(stcHisDate.xPos,rcVol.top),PointF(stcHisDate.xPos,rcVol.bottom));
			gpCach->DrawLine(&pnLine,PointF(stcHisDate.xPos,rcTT.top),PointF(stcHisDate.xPos,rcTT.bottom));

			if (m_ptCur.y >= rcVol.top && m_ptCur.y <= rcVol.bottom)
			{
				gpCach->DrawLine(&pnLine,PointF(rcVol.left,m_ptCur.y),PointF(rcVol.right,m_ptCur.y));
			}
			else if (m_ptCur.y >= rcTT.top && m_ptCur.y <= rcTT.bottom)
			{
				gpCach->DrawLine(&pnLine,PointF(rcTT.left,m_ptCur.y),PointF(rcTT.right,m_ptCur.y));
			}
		//}
		//if (stcHisDate.stcTUHisZYQ.m_iTradeDay > 0)
		//{
			USES_CONVERSION;
			CString strDisPlay[] = {_T("时间"),_T("成交价"),_T("涨跌"),_T("均价"),_T("成交量"),_T("库存量")};
			int iNum = 6;
			int iEveryHight = 15;
			int iWith = 60;
			rcTips.bottom = rcTips.top + iEveryHight*iNum*2;

			Color clrBk[] = {Color(255,0,0,0),Color(255,190,190,190)};
			Color clrUpDown[] = {Color(255,255,255,255),Color(255,255,0,0),Color(255,0,255,0)};
			Color clrVol = Color(255,255,255,74);
			Color clrTitle = Color(255,0,255,255);
			SolidBrush sbrBk(clrBk[0]);
			Pen pnRc(clrBk[1],1.0);
			gpCach->FillRectangle(&sbrBk,Rect(rcTips.left,rcTips.top,rcTips.Width(),rcTips.Height()));
			gpCach->DrawRectangle(&pnRc,Rect(rcTips.left,rcTips.top,rcTips.Width(),rcTips.Height()));

			CString strTemp;
			int i = 0;
			int y = 0;
			int iDc = stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit;
			float fPowPrice = pow(10,(double)iDc);
			float fPowVol = pow(10,(double)2);
			Gdiplus::SolidBrush sbfont(clrTitle);
			Gdiplus::SolidBrush sbUpDown(clrUpDown[0]);

			Gdiplus::FontFamily fm(T2W(_T("宋体")));
			Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),9/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
			StringFormat format,formatl,formatc;
			format.SetAlignment(StringAlignmentFar);
			formatl.SetAlignment(StringAlignmentNear);
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = CDateManage::Instance()->FormatTimeY(stcHisDate.stHisTTimeDate.uiDateTime,0);
			gpCach->DrawString(T2W(strTemp
				),-1,&ft,RectF(rcTips.left,rcTips.top,rcTips.Width(),iEveryHight),
				&format,&sbfont);
			
			strTemp = CDateManage::Instance()->FormatTimeY(stcHisDate.stHisTTimeDate.uiDateTime,2);//.Format(_T("%lld"),stcHisDate.stHisTTimeDate.uiDateTime/100);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbfont);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.stHisTTimeDate.fClosePrice/**1.0/fPowPrice*/,iDc);
			sbUpDown.SetColor(clrUpDown[CompValue(stcHisDate.stHisTTimeDate.fYesSettlPrice,stcHisDate.stHisTTimeDate.fClosePrice)]);	
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString((stcHisDate.stHisTTimeDate.fClosePrice/*/fPowPrice*/ - stcHisDate.stHisTTimeDate.fYesSettlPrice/*/fPowPrice*/)*1.0,3);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.stHisTTimeDate.fAveragePrice/*/fPowPrice*/,iDc);
			sbUpDown.SetColor(clrUpDown[CompValue(stcHisDate.stHisTTimeDate.fYesSettlPrice,stcHisDate.stHisTTimeDate.fAveragePrice)]);	
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			sbUpDown.SetColor(clrVol);
			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.stHisTTimeDate.fTrunoverQty/*/fPowVol*/,2);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);

			i++;
			gpCach->DrawString(T2W(strDisPlay[i]),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&formatl,&sbfont);
			strTemp = DoubleToString(stcHisDate.stHisTTimeDate.fOrderQty/*/fPowVol*/,2);
			gpCach->DrawString(T2W(strTemp),-1,&ft,RectF(rcTips.left,rcTips.top+iEveryHight*y++,rcTips.Width(),iEveryHight),
				&format,&sbUpDown);
		}
	}
}
INT64 CDlgTrendChart_D::GetNextTime(INT64 dwTm)
{
	INT64 uiRet = dwTm;
	do 
	{
		TmapHisUnitK_ZYQ::iterator itMap;
		itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(dwTm);
		if (itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.end())break;
		itMap++;
		if (itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.end())break;

		uiRet = itMap->first;
	} while (0);

	return uiRet;
}
INT64 CDlgTrendChart_D::GetFrontTime(INT64 dwTm)
{
	INT64 uiRet = dwTm;
	do 
	{
		TmapHisUnitK_ZYQ::iterator itMap;
		itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(dwTm);
		if (itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.end()||
			itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.begin())break;
		itMap--;
		if (itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.end()/*||
			itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.begin()*/)break;

		uiRet = itMap->first;
	} while (0);

	return uiRet;
}

void CDlgTrendChart_D::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//DbgOutput(_T("CDlgTrendChart_D::OnMouseMove*****************BG"));
	DWORD dwBG = GetTickCount();
	if(m_iType == 3 && CPublic::Instance()->GetDrawDll()->m_pMouseMove)
	{
		//DbgOutput(_T("GetDrawDll()->m_pMouseMove000000:%d"),GetTickCount()-dwBG);
		dwBG = GetTickCount();
		m_ptCur = point;
		if ((*CPublic::Instance()->GetDrawDll()->m_pMouseMove)(point))
		{
			//DbgOutput(_T("GetDrawDll()->m_pMouseMove1111111:%d"),GetTickCount()-dwBG);
			dwBG = GetTickCount();
			RedrawWindow();
			//DbgOutput(_T("GetDrawDll()->m_pMouseMove222222:%d"),GetTickCount()-dwBG);
			return;
		}
	}
	
	if (m_bMousePress&&m_iType == 3&&stChartDate.stKDate.rcTime.PtInRect(point))
	{
		do 
		{
			HCURSOR hCur  =  LoadCursor( NULL  , IDC_SIZEWE ) ;
			::SetCursor(hCur);
			int iEveryWith = stChartDate.stKDate.fKEveryWithAll;
			UINT64 iBeginTm = stChartDate.stKDate.uiCurBeginTm;
			if (!m_bMousePress)break;
			////////////////////////////
			if ((point.x - m_ptCur.x) >= iEveryWith)
			{
				m_ptCur = point;

				iBeginTm = GetFrontTime(iBeginTm);
			}
			else if ((point.x - m_ptCur.x) <= -iEveryWith)
			{
				m_ptCur = point;
				iBeginTm = GetNextTime(iBeginTm);
			}
			stChartDate.stKDate.uiCurBeginTm = iBeginTm;
			TmapHisUnitK_ZYQ::iterator itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin();
			if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end() && iBeginTm == itMap->second.uiDate)
			{
				ReqKDate_ZYQ(stChartDate.stKDate.stKDateHead.uiCycle,stChartDate.stKDate.mapHisUnitK_ZYQ.size(),200);
			}
			RedrawWindow();

			//DbgOutput(_T("11ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssswwww111111111:%d"),GetTickCount()-dwBG);
		} while (0);
		return;
		//return TRUE;
	}

	if (m_iType == 2 && (stChartDate.stTrendDate.rcVolTrend.PtInRect(point)||stChartDate.stTrendDate.rcTimeTrend.PtInRect(point)))
	{
		m_ptCur = point;
		RedrawWindow();

		//DbgOutput(_T("2222222:%d"),GetTickCount()-dwBG);
	}
	else if (m_iType == 3 && stChartDate.stKDate.rcAll_K.PtInRect(point))
	{
		m_ptCur = point;
		RedrawWindow();
		//DbgOutput(_T("333333333:%d"),GetTickCount()-dwBG);
	}
	else if (m_iType == 1)
	{
		if (m_stQuoteLstDate.iPressDown == 4)
		{
			int iDiffWith = point.x - m_ptCur.x;
			float fUintWith = (m_stQuoteLstDate.stHScrollInfo.rcHScroll.Width()-m_stQuoteLstDate.stHScrollInfo.iBtnWith*2)*1.0/m_stQuoteLstDate.stHScrollInfo.iAllDisPlay;
			if (iDiffWith < 0 && (-iDiffWith) >= fUintWith)
			{
				if (m_stQuoteLstDate.stHeadLstDate.uiHItemBegin > (m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd+1))
				{
					m_ptCur = point;
					m_stQuoteLstDate.stHeadLstDate.uiHItemBegin--;
				}
			}
			else if (iDiffWith > 0 && (iDiffWith) >= fUintWith)
			{
				int iAllHeadNum = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.size();
				if ( (iAllHeadNum  - m_stQuoteLstDate.stHeadLstDate.uiHItemBegin) > (m_stQuoteLstDate.stHeadLstDate.uiDisplayNum - (m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd+1)))
				{
					m_ptCur = point;
					m_stQuoteLstDate.stHeadLstDate.uiHItemBegin++;
				}
			}
			RedrawWindow();
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CDlgTrendChart_D::HitTestTab(CPoint ptCur,TabInfo *pstTabInfo)
{
	BOOL bHit = FALSE;
	if(!pstTabInfo) return bHit;
	if (pstTabInfo->rcTab.PtInRect(ptCur))
	{
		itTmapTabItem itMap;
		for (itMap = pstTabInfo->mapTabItme.begin();itMap != pstTabInfo->mapTabItme.end();itMap++)
		{
			if (itMap->second.rcItem.PtInRect(ptCur))
			{
				bHit = TRUE;
				pstTabInfo->uiCurSel = itMap->second.uiItemId;
				break;
			}
		}
	}
	return bHit;
}
UINT CDlgTrendChart_D::HitTestHScroll(CPoint ptCur,HScrollInfo *pstHScrollInfo)
{
	UINT uiHit = 0;
	if (!pstHScrollInfo)return uiHit;

	if (pstHScrollInfo->rcHScroll.PtInRect(ptCur))
	{
		int iOffX = ptCur.x - pstHScrollInfo->rcHScroll.left;
		if (iOffX < pstHScrollInfo->iThumbLeft)
		{
			if (iOffX < pstHScrollInfo->iBtnWith)
			{
			}
			else
			{}

			uiHit = 1;
		}
		else if (iOffX > pstHScrollInfo->iThumbRight)
		{
			if (iOffX > (pstHScrollInfo->rcHScroll.right - pstHScrollInfo->iBtnWith))
			{
			}
			else{}
			uiHit = 3;
		}
		else
		{
			uiHit = 2;
		}
	}
	return uiHit;
}
void CDlgTrendChart_D::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::ReleaseCapture();  
	::ClipCursor(NULL);
	HCURSOR hCur  =  LoadCursor( NULL  , IDC_ARROW ) ;
	::SetCursor(hCur);
	m_bMousePress = FALSE;
	m_stQuoteLstDate.iPressDown = 0;
	if(m_iType == 3 && CPublic::Instance()->GetDrawDll()->m_pLButtonUp) 
		if((*CPublic::Instance()->GetDrawDll()->m_pLButtonUp)(point))RedrawWindow();

	CDialog::OnLButtonUp(nFlags, point);
}


void CDlgTrendChart_D::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReleaseCapture();
	m_stQuoteLstDate.iPressDown = 0;
	m_ptCur = point;
	ChangeDisplayMode(2);
	CDialog::OnLButtonDblClk(nFlags, point);
}
MainIndex CDlgTrendChart_D::HitTestMianIndex(CString strName,TVecMainIndex *pvecMainIndex)
{
	MainIndex stMainIndex;
	int iNum = pvecMainIndex->size();
	for (int i = 0; i < iNum; i++)
	{
		if ((*pvecMainIndex)[i].strIndexName.CompareNoCase(strName) == 0)
		{
			stMainIndex = (*pvecMainIndex)[i];
			break;
		}
	}
	return stMainIndex;
}
MainIndex CDlgTrendChart_D::HitTestMianIndex(CPoint ptCur,TVecMainIndex *pvecMainIndex)
{
	MainIndex stMainIndex;
	int iNum = pvecMainIndex->size();
	for (int i = 0; i < iNum; i++)
	{
		if ((*pvecMainIndex)[i].rcIndex.PtInRect(ptCur))
		{
			stMainIndex = (*pvecMainIndex)[i];
			break;
		}
	}
	return stMainIndex;
}
void CDlgTrendChart_D::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	do 
	{
		SetFocus();

		if(m_iType == 3&&CPublic::Instance()->GetDrawDll()->m_pLButtonDown&&
			(*CPublic::Instance()->GetDrawDll()->m_pLButtonDown)(point))
		{
			RedrawWindow();
			return;
		}

		if (m_iType == 3&&stChartDate.stKDate.rcTime.PtInRect(point))
		{
			m_ptCur = point;
			m_bMousePress = TRUE;
			CRect rcWnd = stChartDate.stKDate.rcTime;
			//CWnd *pWnd = CPublic::Instance()->GetView(10);
			//if (!pWnd)return FALSE;

			ClientToScreen(&rcWnd);
			::ClipCursor(&rcWnd);
			HCURSOR hCur  =  LoadCursor( NULL  , IDC_SIZEWE ) ;
			::SetCursor(hCur);
			::SetCapture(GetSafeHwnd());
			break ;
		}

		if (m_iType == 2 && (stChartDate.stTrendDate.rcVolTrend.PtInRect(point)||stChartDate.stTrendDate.rcTimeTrend.PtInRect(point)))
		{

			m_bTips = !m_bTips;
			m_ptCur = point;
			RedrawWindow();
			break;
		}
		if (m_iType == 3 && stChartDate.stKDate.rcIndex.PtInRect(point))
		{
			MainIndex stMainIndex = HitTestMianIndex(point,&(stChartDate.stKDate.vecMainIndex));
			if (stMainIndex.uiIndexID > 0 && stChartDate.stKDate.uiCurSelIndex != stMainIndex.uiIndexID)
			{
				stChartDate.stKDate.uiCurSelIndex = stMainIndex.uiIndexID;
				stChartDate.stKDate.vecIndexDisplay[2] = stMainIndex.strIndexName;
				CalcIndexDate(&(stChartDate.stKDate));
				RedrawWindow();
				break;
			}
		}
		if (m_iType == 3 && stChartDate.stKDate.rcAll_K.PtInRect(point))
		{
			m_bTips = !m_bTips;
			m_ptCur = point;
			RedrawWindow();
			break;
		}
		if ((m_iType == 3 || m_iType == 2))
		{
			UINT uiHit = 0;
			if (HitTestTab(point,&(stChartDate.stTabInfo)))
			{
				RedrawWindow();
				break;
			}
			if (HitTest_RightRc_TopBtn(point,uiHit))
			{
				if (uiHit == 1)
				{
					;
				}
				else if (uiHit == 3)
				{
					;
				}
				break;
			}
			if (HitTestTab(point,&(stChartDate.stTabMain)))
			{
				m_lockmapHisTTDate.Lock();
				if (stChartDate.stTabMain.uiCurSel == 0)
				{
					m_bTips = FALSE;
					m_iType = 2;
				}
				else if (stChartDate.stTabMain.uiCurSel == 1)
				{
					m_bTips = FALSE;
					m_iType = 3;
				}
				m_lockmapHisTTDate.Unlock();
				RedrawWindow();
				break;
			}
		}
		if (m_iType == 1)
		{
			UINT uiHit;
			UINT uiRet = HitTestQuoteLst(point,uiHit);
			if(uiRet)
			{
				if (uiRet == 4&&uiHit == 2)
				{
					m_ptCur = point;
					m_stQuoteLstDate.iPressDown = 4;
					SetCapture();
				}
				else if (uiRet == 3)
				{
					do 
					{
						m_stQuoteLstDate.iPressDown = 0;
						m_stQuoteLstDate.stQuoteItemLst.Clear();

						STUReportDataIndex_ZYQ stReportDataIndex_ZYQ;
						stReportDataIndex_ZYQ.m_usMarketType = uiHit;  
						stReportDataIndex_ZYQ.m_ucOrderBy = RPDATA_ORDER_BY_SYMOBL;            
						stReportDataIndex_ZYQ.m_ucSeqType = REDATA_SEQ_TYPE_ASC;               
						stReportDataIndex_ZYQ.m_usBeginPos = 1;               
						stReportDataIndex_ZYQ.m_usReqNum = 50;                
						stReportDataIndex_ZYQ.m_usReqDesSymoblNum = 0;        
						CDateManage_ZYQ::Instance()->ReqReportDate_ZYQ(&stReportDataIndex_ZYQ);
					} while (0);	
				}
				RedrawWindow();
				break;
			}
		}
	} while (0);
	CDialog::OnLButtonDown(nFlags, point);
}
BOOL CDlgTrendChart_D::HitTest_RightRc_TopBtn(CPoint &ptCur,UINT uiHit)
{
	BOOL bRet = FALSE;
	uiHit = 0;
	do 
	{
		if(!stChartDate.stTopPartDate.stTopBtn.rcTop.PtInRect(ptCur))break;

		bRet = TRUE;
		if (stChartDate.stTopPartDate.stTopBtn.rcBtnLeft.PtInRect(ptCur))
		{
			uiHit = 1;
			break;
		}
		if (stChartDate.stTopPartDate.stTopBtn.rcBtnRight.PtInRect(ptCur))
		{
			uiHit = 3;
			break;
		}
	} while (0);

	return bRet;
}
UINT CDlgTrendChart_D::HitTestQuoteLst(CPoint ptCur,UINT &uiHit)
{
	UINT uiRetHit = 0;
	uiHit = 0;
	do 
	{
		UINT uiItem,uiSubItem;
		if (HitTestQuoteLstItemClick(ptCur,uiItem,uiSubItem))
		{
			if (uiItem == 0)
			{
				m_stQuoteLstDate.stHeadLstDate.uiCurSort = uiSubItem;
				m_stQuoteLstDate.stHeadLstDate.bSort = !m_stQuoteLstDate.stHeadLstDate.bSort;
			}
			else
				m_stQuoteLstDate.stQuoteItemLst.uiCurSel = uiItem;

			uiHit = uiItem;
			uiRetHit = 1;
			break;
		}
		else if (HitTestTab(ptCur,&(m_stQuoteLstDate.stTabInfo)))
		{
			uiRetHit = 3;
			uiHit = m_stQuoteLstDate.stTabInfo.uiCurSel;
			//AddTestQuoteChangeDate(m_stQuoteLstDate.stTabInfo.uiCurSel);
			break;
		}
		
		uiHit = HitTestHScroll(ptCur,&(m_stQuoteLstDate.stHScrollInfo));
		if (uiHit > 0)
		{
			if (uiHit == 1)
			{
				if (m_stQuoteLstDate.stHeadLstDate.uiHItemBegin > (m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd+1))
				{
					m_stQuoteLstDate.stHeadLstDate.uiHItemBegin--;
				}
			}
			else if (uiHit == 3)
			{
				int iAllHeadNum = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.size();
				if ( (iAllHeadNum  - m_stQuoteLstDate.stHeadLstDate.uiHItemBegin) > (m_stQuoteLstDate.stHeadLstDate.uiDisplayNum - (m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd+1)))
				{
					m_stQuoteLstDate.stHeadLstDate.uiHItemBegin++;
				}
			}
			else
			{
			}

			uiRetHit = 4;
			break;
		}
	} while (0);
	return uiRetHit;
}
BOOL CDlgTrendChart_D::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_iType == 1)
	{
		if (zDelta > 0)
		{
			int uiItemIDBegin = m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin-m_stQuoteLstDate.stQuoteItemLst.uiItemDiplayMax;
			m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin = uiItemIDBegin > 0?uiItemIDBegin:1;
		}
		else
		{
			int uiItemIDBegin = m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin+m_stQuoteLstDate.stQuoteItemLst.uiItemDiplayMax;
			m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin = uiItemIDBegin > m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.size()?m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin:uiItemIDBegin;
		}
		RedrawWindow();
	}
	else if (m_iType == 2 || m_iType == 3)
	{
		ChangeSymbol(zDelta > 0?1:0);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CDlgTrendChart_D::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


void CDlgTrendChart_D::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == EVRNT_TEST)
	{
		m_lockmapHisTTDate.Lock();
		

		HisDateSpot stcHisTTDate;
		HisTTimeDate stcTuHisZYQ;
		stcTuHisZYQ.iTradeDay = 20141126;
		int iAllTm = 0;
		int iTimeNum = 1;
		GetTradeTimeInfo(iTimeNum,iAllTm);

		TmapHisDateSpot::reverse_iterator mapRIt;
		mapRIt = stChartDate.stTrendDate.mapHisTTDate.rbegin();
		int iBeginTm = 0;
		int iBeforPrcie,iBeforJunPrcie;
		if (mapRIt != stChartDate.stTrendDate.mapHisTTDate.rend())
		{
			iBeginTm = mapRIt->second.stHisTTimeDate.uiDateTime;
			iBeginTm = GetNextTime(iBeginTm,1,1);
			iBeforPrcie = mapRIt->second.stHisTTimeDate.fClosePrice;
			iBeforJunPrcie = mapRIt->second.stHisTTimeDate.fClosePrice;
		}
		else
		{
			iBeginTm = stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[0].inBeginTime;
			iBeforPrcie = 600;
			iBeforJunPrcie = 600;
		}

		if (!IsInTradeTime(iBeginTm))
		{
			if (iBeginTm >= stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iTimeNum-1].inEndTime)
			{
				stChartDate.stTrendDate.mapHisTTDate.clear();
				iBeginTm = stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[0].inBeginTime;
			}
			else
			{
				while(!IsInTradeTime(iBeginTm))
				{
					iBeginTm = GetNextTime(iBeginTm,1,1);
				}
			}
		}
		srand( (unsigned int)time(0) );
		stcTuHisZYQ.uiDateTime = iBeginTm;
		stcTuHisZYQ.fClosePrice = iBeforPrcie + (rand()%5)*RAND_MARK(rand()%2);     
		stcTuHisZYQ.fTrunoverQty = 86+rand()%500;           
		stcTuHisZYQ.fOrderQty = 200000;         
		stcTuHisZYQ.fAveragePrice = iBeforJunPrcie + (rand()%3)*RAND_MARK(rand()%2);  
		stcTuHisZYQ.fYesSettlPrice = 600;

		stcHisTTDate.stHisTTimeDate = stcTuHisZYQ;
		stChartDate.stTrendDate.mapHisTTDate[iBeginTm] = stcHisTTDate;

		GetMaxMinPrice(stChartDate.stTrendDate.fMaxPrice,stChartDate.stTrendDate.fMinPrice,stChartDate.stTrendDate.fYsPrice);
		GetMaxMinVol(stChartDate.stTrendDate.fMaxVol,stChartDate.stTrendDate.fMinVol,stChartDate.stTrendDate.fPPVol);

		m_lockmapHisTTDate.Unlock();
		RedrawWindow();
	}
	CDialog::OnTimer(nIDEvent);
}
BOOL CDlgTrendChart_D::IsInTradeTime(INT64 iCurTime,int Type)
{
	BOOL bRet = FALSE;
	int iAllTm = 0;
	int iTimeNum = 1;
	GetTradeTimeInfo(iTimeNum,iAllTm);
	if (Type == 1)
	{
		iCurTime = iCurTime%1000000;
	}

	for (int i = 0; i < iTimeNum; i ++)
	{
		if (iCurTime > stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[i].inBeginTime&&iCurTime <= stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[i].inEndTime)
		{
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}
CString CDlgTrendChart_D::DoubleToString( double dblValue,int nDec )
{
	CString strValue=_T("");
	switch(nDec)
	{
	case 0 :
		strValue.Format(_T("%.f"),dblValue);
		break;
	case 1 :
		strValue.Format(_T("%.1f"),dblValue);
		break;
	case 2 :
		strValue.Format(_T("%.2f"),dblValue);
		break;
	case 3 :
		strValue.Format(_T("%.3f"),dblValue);
		break;
	case 4 :
		strValue.Format(_T("%.4f"),dblValue);
		break;
	case 5 :
		strValue.Format(_T("%.5f"),dblValue);
		break;
	case 6 :
		strValue.Format(_T("%.6f"),dblValue);
		break;
	default :
		strValue.Format(_T("%.f"),dblValue);
		break;
	}
	if ( strValue.Find(_T(".")) == 0 )
	{
		strValue.Insert(0,_T("0")) ;
	}

	return strValue;
}
UINT CDlgTrendChart_D::ChangeDisplayMode(UINT iMode)
{
	m_lockmapHisTTDate.Lock();
	do 
	{
		if (iMode == 2&&m_iType!=1)break;
		else if (iMode == 2&&m_iType==1)
		{
			UINT uiHit;
			UINT uiRet = HitTestQuoteLst(m_ptCur,uiHit);
			if(uiRet)
			{
				if (uiRet == 1&&uiHit > 0)
				{
					//pQuoteItemLstDate pItem = GetCurSelQuoteItem(uiHit);
					CString strSymbolCode;
					strSymbolCode = GetCurSelQuoteSubItem(uiHit,1);
					if (strSymbolCode.IsEmpty()) break;//return m_bTips;

					stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket = m_stQuoteLstDate.stTabInfo.uiCurSel;
					stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode = strSymbolCode;
					if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stChartDate.stSymbolTradeInfo))
					{
						stChartDate.stSymbolTradeInfo.Clear();
						//return m_bTips;
						break;
					}

					stChartDate.stTopPartDate.Clear();
					stChartDate.stDealDetailsInfo.Clear();
					stChartDate.stTrendDate.Clear();
					stChartDate.stKDate.Clear();

					STUSimpleSymbol_ZYQ stSimpleSymbol_ZYQ;
					stSimpleSymbol_ZYQ.m_usMarketType = m_stQuoteLstDate.stTabInfo.uiCurSel;
					//memcpy(stSimpleSymbol_ZYQ.m_szSymbolCode)
					strcpy(stSimpleSymbol_ZYQ.m_szSymbolCode,strSymbolCode.GetBuffer(0));
					CDateManage_ZYQ::Instance()->ReqReportDateDetail_ZYQ(&stSimpleSymbol_ZYQ);

					m_bTips = FALSE;
					m_iType = 2;

					USES_CONVERSION;
					CPublic::Instance()->GetDrawDll()->m_pSetCurSymbol(T2W(stSimpleSymbol_ZYQ.m_szSymbolCode,strSymbolCode));
					//return m_bTips;
					break;
				}
			}
			//return m_bTips;
			break;
		}
		if (m_iType == 3)
		{
			m_bTips = FALSE;
			m_iType = 1;

			STUReportDataIndex_ZYQ stReportDataIndex_ZYQ;
			stReportDataIndex_ZYQ.m_usMarketType = m_stQuoteLstDate.stTabInfo.uiCurSel;  
			stReportDataIndex_ZYQ.m_ucOrderBy = m_stQuoteLstDate.stHeadLstDate.uiCurSort;            
			stReportDataIndex_ZYQ.m_ucSeqType = m_stQuoteLstDate.stHeadLstDate.bSort;                       
			stReportDataIndex_ZYQ.m_usBeginPos = m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin;          
			stReportDataIndex_ZYQ.m_usReqNum = 50;                 
			stReportDataIndex_ZYQ.m_usReqDesSymoblNum = 0; 
			CDateManage_ZYQ::Instance()->ReqReportDate_ZYQ(&stReportDataIndex_ZYQ);
		}
		else if (m_iType == 2)
		{
			m_bTips = FALSE;
			m_iType = 3;
		}
	} while (0);
	m_lockmapHisTTDate.Unlock();
	RedrawWindow();

	return m_bTips;
}

BOOL CDlgTrendChart_D::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_HOTKEY)
	{
		if (QuickKeyResponse(VK_F10,pMsg->lParam))return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (QuickKeyResponse(pMsg->wParam,pMsg->lParam))return TRUE;
	}
	else if (WM_CHAR == pMsg->message)
	{
		UINT nChar = pMsg->wParam;
		if((nChar>='0')&&(nChar<='9') 
			|| (nChar>='A')&&(nChar<='Z')
			|| (nChar>='a')&&(nChar<='z'))
		{
			AfxGetMainWnd()->PostMessage(WM_CHAR, nChar,MAKELONG(0,2));
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CDlgTrendChart_D::GetMaxMinPrice_KMAP(float &iMaxPrice,float &iMinPrice,float &iYsPrice)
{
	iMaxPrice = 0.0;
	iMinPrice = 9999999.0;
	iYsPrice = stChartDate.stTopPartDate.fYesDPrice;
	int iNum = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	TmapHisUnitK_ZYQ::iterator itMap;
	if ( iNum<= 0)
	{
		if (IF_FZ(iYsPrice))
		{
			iYsPrice = 5;
		}
		iMaxPrice = iYsPrice*1.2;
		iMinPrice = iYsPrice*0.8;
		return;
	}

	iYsPrice = 558;
	itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(stChartDate.stKDate.uiCurBeginTm);
	if (itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.end())itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin();

	int iDisplayNum = 0;
	for (; itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end();(itMap++,iDisplayNum++))
	{
		if (iDisplayNum >= stChartDate.stKDate.iDisplayNum)
		{
			break;
		}
		if (itMap->second.fMaxPrice > iMaxPrice)
		{
			iMaxPrice = itMap->second.fMaxPrice;
		}
		if (itMap->second.fMinPrice < iMinPrice)
		{
			iMinPrice = itMap->second.fMinPrice;
		}
	}
	if (IF_FZ(iMaxPrice))
	{
		iYsPrice = 5;
		iMaxPrice = iYsPrice*1.2;
		iMinPrice = iYsPrice*0.8;
	}
	if (iMaxPrice == iMinPrice)
	{
		iMaxPrice = iMaxPrice*1.2;
		iMinPrice = iMinPrice*0.8;
	}
}
void CDlgTrendChart_D::GetMaxMinVol_KMAP(float &iMaxVol,float &iMinVol,float &iPJVol)
{
	iMaxVol = 0.00;
	iMinVol = 0.00;
	int iNum = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	TmapHisUnitK_ZYQ::iterator itMap;
	if ( iNum<= 0)
	{
		iMaxVol = 10;
		iMinVol = 0;
		iPJVol = iMaxVol/2;
		return;
	}

	iPJVol = 662;

	itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(stChartDate.stKDate.uiCurBeginTm);
	if (itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.end())itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin();

	int iDisplayNum = 0;
	for (; itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end();(itMap++,iDisplayNum++))
	{
		if (iDisplayNum >= stChartDate.stKDate.iDisplayNum)
		{
			break;
		}
		if (itMap->second.fTrunoverQty > iMaxVol)
		{
			iMaxVol = itMap->second.fTrunoverQty;
		}
	}
	if (IF_FZ(iMaxVol))
	{
		iMaxVol = 10;
		iMinVol = 0;
		iPJVol = iMaxVol/2;
	}
}
void CDlgTrendChart_D::GetMaxMinOther_KMAP(float &iMaxOther,float &iMinOther,float &iYsOther)
{
	iMaxOther = 0x7fffffff;
	iMinOther = 0x7fffffff;
	iYsOther = 0.00;
	int iNum = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	TmapHisUnitK_ZYQ::iterator itMap;
	if ( iNum<= 0)
	{
		iMaxOther = 10;
		iMinOther = -10;
		iYsOther = 0.00;
		return;
	}
	if (stChartDate.stKDate.vecIndexDisplay.size() != 3)return;

	CString strKey = stChartDate.stKDate.vecIndexDisplay[2];
	TMapIndexDateHead::iterator itMapHead = stChartDate.stKDate.MapIndexDateHead.find(strKey);
	if (itMapHead == stChartDate.stKDate.MapIndexDateHead.end())return;

	UINT uiCurves = itMapHead->second.uiCurves;
	if (uiCurves <= 0 || uiCurves > 5)return;

	itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(stChartDate.stKDate.uiCurBeginTm);
	if (itMap == stChartDate.stKDate.mapHisUnitK_ZYQ.end())itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin();
	
	int iDisplayNum = 0;
	for (/*itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin()*/; itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end();(itMap++,iDisplayNum++))
	{
		if (iDisplayNum >= stChartDate.stKDate.iDisplayNum)
		{
			break;
		}
		for (int i = 0; i < uiCurves; i++)
		{
			if (itMap->second.mapIndexDate[strKey].fEqualLine[i] == 0x7fffffff)
				continue;
			if (iMaxOther == 0x7fffffff)
			{
				iMaxOther = itMap->second.mapIndexDate[strKey].fEqualLine[i];
			}
			if (iMinOther == 0x7fffffff)
			{
				iMinOther = itMap->second.mapIndexDate[strKey].fEqualLine[i];
			}
			if (iMaxOther < itMap->second.mapIndexDate[strKey].fEqualLine[i])
			{
				iMaxOther = itMap->second.mapIndexDate[strKey].fEqualLine[i];
			}
			if (iMinOther > itMap->second.mapIndexDate[strKey].fEqualLine[i])
			{
				iMinOther = itMap->second.mapIndexDate[strKey].fEqualLine[i];
			}
		}
	}

	if (iMaxOther == 0x7fffffff || iMinOther == 0x7fffffff)
	{
		iMaxOther = 10;
		iMinOther = -10;
		iYsOther = 0.00;
	}
	if (_isnan(iMaxOther) || _isnan(iMinOther))
	{
		iMaxOther = 10;
		iMinOther = -10;
		iYsOther = 0.00;
	}
}
LRESULT CDlgTrendChart_D::OnChangeTrendChart(WPARAM wPar, LPARAM lPar)
{
	return 1;
}
LRESULT CDlgTrendChart_D::OnDataMessage(WPARAM wPar, LPARAM lPar)
{
	return 1;
}
void CDlgTrendChart_D::AddTestQuoteChangeDate(int iMark)
{
	m_stQuoteLstDate.stQuoteItemLst.uiCurSel = 1;
	//m_stQuoteLstDate.stHeadLstDate.bSort = 0;
	m_stQuoteLstDate.stHeadLstDate.clrSort = Color::MakeARGB(255,255,0,0);

	//m_stQuoteLstDate.stHeadLstDate.uiCurSort = 0;
	m_stQuoteLstDate.stHeadLstDate.uiDisplayNum = 10;
	m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.clear();
	m_stQuoteLstDate.stHeadLstDate.uiHHight = 20;
	int iAllHead = 21;
	for (int i = 0; i < iAllHead; i++)
	{
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].clrSItemText = Color::MakeARGB(255,0,255,255);
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].strSItemText = strQuoteLstHeader1[i];
		if (i == 0)
		{
			m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSWith = 50;
		}
		else
			m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSWith = 100;
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSTextAlignmentType = 2;
	}
	m_stQuoteLstDate.stHeadLstDate.uiHItemBegin = 0;
	m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd = 4;

	m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.clear();
	int iAddItem = iMark * 20;
	CString strTemp;
	QuoteItemLstDate stQILDTemp;
	m_stQuoteLstDate.stQuoteItemLst.uiItemHight = 18;
	m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin = 1;

	for (int i = 0; i < iAddItem; i++)
	{
		stQILDTemp.uiItemHight = 18;
		stQILDTemp.uiItemID = i+1;
		stQILDTemp.mapQuoteLstSubItemDate.clear();
		for (int x = 0; x < iAllHead; x++)
		{
			stQILDTemp.mapQuoteLstSubItemDate[x].clrSItemText = Color::MakeARGB(255,255,255,0);
			if (x == 0)
				strTemp.Format(_T("%d"),i+1);
			else if (x == 1)
			{
				strTemp = m_stQuoteLstDate.stTabInfo.mapTabItme[m_stQuoteLstDate.stTabInfo.uiCurSel].strItemName;
			}
			else
				strTemp.Format(_T("%d%d%d%d%d%d"),x,x,x,x,x,x);
			stQILDTemp.mapQuoteLstSubItemDate[x].strSItemText = strTemp/*strQuoteLstHeader1[i]*/;
			stQILDTemp.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		}
		m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate[stQILDTemp.uiItemID] = stQILDTemp;
	}
}
void CDlgTrendChart_D::AddQuoteHead(UINT iMode)
{
	CString *pStrHead = NULL;
	int iAllHead = 0;
	{
		pStrHead = strQuoteLstHeader1;
		iAllHead = 21;
	}
	m_stQuoteLstDate.iPressDown = 0;
	m_stQuoteLstDate.stHeadLstDate.Init();
	m_stQuoteLstDate.stHeadLstDate.bSort = 0;
	m_stQuoteLstDate.stHeadLstDate.clrSort = Color::MakeARGB(255,255,0,0);
	m_stQuoteLstDate.stHeadLstDate.uiCurSort = 0;
	m_stQuoteLstDate.stHeadLstDate.uiDisplayNum = 0;
	m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.clear();
	m_stQuoteLstDate.stHeadLstDate.uiHHight = 20;
	for (int i = 0; i < iAllHead; i++)
	{
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].clrSItemText = Color::MakeARGB(255,0,255,255);
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].strSItemText = pStrHead[i];
		if (i == 0)
		{
			m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSWith = 80;
		}
		else
			m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSWith = 100;
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSTextAlignmentType = 2;
	}
	m_stQuoteLstDate.stHeadLstDate.uiHItemBegin = 0;
	m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd = 4;
	m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.clear();
	m_stQuoteLstDate.stQuoteItemLst.uiItemHight = 18;
}
void CDlgTrendChart_D::AddTestQuoteDate()
{
	CString *pStrHead = strQuoteLstHeader1;
	int iAllHead = 21;
	m_stQuoteLstDate.iPressDown = 0;
	m_stQuoteLstDate.stHeadLstDate.Init();
	m_stQuoteLstDate.stHeadLstDate.bSort = 0;
	m_stQuoteLstDate.stHeadLstDate.clrSort = Color::MakeARGB(255,255,0,0);
	m_stQuoteLstDate.stHeadLstDate.uiCurSort = 0;
	m_stQuoteLstDate.stHeadLstDate.uiDisplayNum = 0;
	m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.clear();
	m_stQuoteLstDate.stHeadLstDate.uiHHight = 20;
	for (int i = 0; i < iAllHead; i++)
	{
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].clrSItemText = Color::MakeARGB(255,0,255,255);
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].strSItemText = pStrHead[i];
		if (i == 0)
		{
			m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSWith = 80;
		}
		else
		   m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSWith = 100;
		m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate[i].iSTextAlignmentType = 2;
	}
	m_stQuoteLstDate.stHeadLstDate.uiHItemBegin = 0;
	m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd = 4;
	m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.clear();
	int iAddItem = 100;
	CString strTemp;
	QuoteItemLstDate stQILDTemp;
	m_stQuoteLstDate.stQuoteItemLst.uiItemHight = 18;
	int iWith = 50;
	m_stQuoteLstDate.stTabInfo.mapTabItme[0].strItemName = _T("所有商品");
	m_stQuoteLstDate.stTabInfo.mapTabItme[0].uiItemId = 0;
	m_stQuoteLstDate.stTabInfo.mapTabItme[0].uiItemWith = 90;
	m_stQuoteLstDate.stTabInfo.uiCurSel = 0;
	m_stQuoteLstDate.stHScrollInfo.iBtnWith = 16;
	m_stQuoteLstDate.stHScrollInfo.iAllDisPlay = 100;
	m_stQuoteLstDate.stHScrollInfo.iPageDisPlay = 100;
}

void CDlgTrendChart_D::CaluQuteLstRect(CRect rcDraw)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0)return;

	DWORD dwBG = GetTickCount();
	CRect rcItem;
	int iSliderHight = 16;
	int iSliderWith = 200;
	//int iTabHight = 20;
	rcItem = rcDraw;
	m_stQuoteLstDate.rcLst = rcDraw;
	rcItem = CRect(rcDraw.left,rcDraw.top,rcDraw.right,rcDraw.top+m_stQuoteLstDate.stHeadLstDate.uiHHight);
	m_stQuoteLstDate.stHeadLstDate.rcHItem = rcItem;
	
	//rcDraw.right -= iSliderHight;
	//rcDraw.bottom -= iSliderHight;
	m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem = CRect(rcDraw.left,m_stQuoteLstDate.stHeadLstDate.rcHItem.bottom,rcDraw.right,rcDraw.bottom-iSliderHight);
	m_stQuoteLstDate.stTabInfo.rcTab = CRect(rcDraw.left,m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem.bottom,
		rcDraw.right - iSliderWith,rcDraw.bottom);
	m_stQuoteLstDate.stHScrollInfo.rcHScroll = CRect(rcDraw.right-iSliderWith,rcDraw.bottom-iSliderHight,rcDraw.right,rcDraw.bottom);

	TmapQuoteLstSubItemDate::iterator itHMap;
	if (m_stQuoteLstDate.stHeadLstDate.uiHItemBegin <= m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd)
		m_stQuoteLstDate.stHeadLstDate.uiHItemBegin = m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd+1;
	itHMap = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.find(m_stQuoteLstDate.stHeadLstDate.uiHItemBegin);
	int iSumWith = 0;
	int iDisplayMax = 0;
	BOOL bCaluMaxDispaly = FALSE;
	for (itHMap = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.begin();
		itHMap != m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.end();itHMap++)
	{
		iDisplayMax++;
		iSumWith += itHMap->second.iSWith;
		if (iSumWith > m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem.Width())
		{
			bCaluMaxDispaly = TRUE;
			iDisplayMax -= 1;
			iSumWith -= itHMap->second.iSWith;
			break;
		}
		if ((iDisplayMax > m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd))
		{
			break;
		}
	}
	if (!bCaluMaxDispaly)
	{
		for (itHMap = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.find(m_stQuoteLstDate.stHeadLstDate.uiHItemBegin);
			itHMap != m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.end();itHMap++)
		{
			iDisplayMax++;
			iSumWith += itHMap->second.iSWith;
			if (iSumWith > m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem .Width())
			{
				bCaluMaxDispaly = TRUE;
				iDisplayMax -= 1;
				iSumWith -= itHMap->second.iSWith;
				break;
			}
		}
	}
	if (iDisplayMax <= 0)
	{
		iDisplayMax = 1;
	}
	int iHeadNum = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.size();
	m_stQuoteLstDate.stHeadLstDate.uiDisplayNum = iDisplayMax;
	m_stQuoteLstDate.stHeadLstDate.uiCorrectionWith = (m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem.Width() - iSumWith) / m_stQuoteLstDate.stHeadLstDate.uiDisplayNum;
	if (m_stQuoteLstDate.stHeadLstDate.uiCorrectionWith > 50)m_stQuoteLstDate.stHeadLstDate.uiCorrectionWith = 20;
	m_stQuoteLstDate.stHScrollInfo.iAllDisPlay = iHeadNum - m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd - 1;
	m_stQuoteLstDate.stHScrollInfo.iPageDisPlay = iDisplayMax - m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd - 1;
	float fUintWith = (m_stQuoteLstDate.stHScrollInfo.rcHScroll.Width()-m_stQuoteLstDate.stHScrollInfo.iBtnWith*2)*1.0/m_stQuoteLstDate.stHScrollInfo.iAllDisPlay;
	m_stQuoteLstDate.stHScrollInfo.iThumbLeft = m_stQuoteLstDate.stHScrollInfo.iBtnWith + fUintWith*(m_stQuoteLstDate.stHeadLstDate.uiHItemBegin - m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd-1);
	m_stQuoteLstDate.stHScrollInfo.iThumbRight = m_stQuoteLstDate.stHScrollInfo.iThumbLeft + fUintWith*m_stQuoteLstDate.stHScrollInfo.iPageDisPlay;
	if (m_stQuoteLstDate.stHScrollInfo.iThumbRight > (m_stQuoteLstDate.stHScrollInfo.rcHScroll.Width() - m_stQuoteLstDate.stHScrollInfo.iBtnWith))
	{
		m_stQuoteLstDate.stHScrollInfo.iThumbRight = m_stQuoteLstDate.stHScrollInfo.rcHScroll.Width() - m_stQuoteLstDate.stHScrollInfo.iBtnWith;
		m_stQuoteLstDate.stHScrollInfo.iThumbLeft = m_stQuoteLstDate.stHScrollInfo.iThumbRight - fUintWith*(m_stQuoteLstDate.stHeadLstDate.uiHItemBegin - m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd-1);
	}
	if (m_stQuoteLstDate.stHScrollInfo.iThumbLeft < m_stQuoteLstDate.stHScrollInfo.iBtnWith )
	{
		m_stQuoteLstDate.stHScrollInfo.iThumbLeft = m_stQuoteLstDate.stHScrollInfo.iBtnWith;
	}

	int iItemNum = m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.size();
	TmapQuoteItemLstDate::iterator itMap;
	QuoteItemLstDate stQILDTemp;
	int iDisplayItemNum = 0;
	
	rcItem = m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem;
	itMap = m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.find(m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin);
	for (; itMap != m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.end(); itMap++)
	{
		rcItem.bottom = rcItem.top + m_stQuoteLstDate.stQuoteItemLst.uiItemHight/*itMap->second.uiItemHight*/;
		itMap->second.rcItem = rcItem;
		//if (rcItem.bottom <= m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem.bottom)
		//{
		//	iDisplayItemNum++;
		//}
		rcItem.top = rcItem.bottom;
	}
	
	iDisplayItemNum = m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem.Height()/m_stQuoteLstDate.stQuoteItemLst.uiItemHight;
	m_stQuoteLstDate.stQuoteItemLst.uiItemDiplayMax = iDisplayItemNum;

	
}
BOOL CDlgTrendChart_D::IsValidRect(Graphics * gpCach,CRect rcDraw)
{
	if (rcDraw.Width()<= 0 || rcDraw.Height() <= 0||
		!gpCach->IsVisible(Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height())))return FALSE;
	return TRUE;
}
void CDlgTrendChart_D::DrawQuoteLstHead(Graphics * gpCach,CRect rcDraw)
{
	if (!IsValidRect(gpCach,rcDraw)||m_stQuoteLstDate.stHeadLstDate.uiDisplayNum == 0)return;

	Pen penBd(Color(255,255,0,0));
	SolidBrush sbrBk(Color(255,111,111,111));
	gpCach->FillRectangle(&sbrBk,Rect(rcDraw.left+1,rcDraw.top+1,rcDraw.Width()-1,rcDraw.Height()-1));
	gpCach->DrawLine(&penBd,Point(rcDraw.left,rcDraw.bottom),Point(rcDraw.right,rcDraw.bottom));

	USES_CONVERSION;
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),12/*stcSubItem.iTextSize*/,1/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format;
	SolidBrush sbrText(Color(255,255,255,255));

	int iIndex = 0;
	CRect rcSubItem = m_stQuoteLstDate.stHeadLstDate.rcHItem;
	TmapQuoteLstSubItemDate::iterator itMap;
	CString strDisplay;
	for (itMap = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.begin();
		itMap != m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.end();/*itMap++*/)
	{
		if (iIndex >= m_stQuoteLstDate.stHeadLstDate.uiDisplayNum)break;
		format.SetAlignment((StringAlignment)itMap->second.iSTextAlignmentType);
		strDisplay = itMap->second.strSItemText;
		if (m_stQuoteLstDate.stHeadLstDate.uiCurSort == itMap->first)
		{
			if (m_stQuoteLstDate.stHeadLstDate.bSort)
			{
				sbrText.SetColor(Color(255,255,0,0));
				strDisplay += _T("↑");
			}
			else
			{
				sbrText.SetColor(Color(255,0,255,0));
				strDisplay += _T("↓");
			}
		}
		else
		    sbrText.SetColor(Color(itMap->second.clrSItemText));
		rcSubItem.right = rcSubItem.left + itMap->second.iSWith + m_stQuoteLstDate.stHeadLstDate.uiCorrectionWith;
		gpCach->DrawString(T2W(strDisplay),-1,&ft,RectF(rcSubItem.left,rcSubItem.top,rcSubItem.Width(),rcSubItem.Height()),
			&format,&sbrText);
		rcSubItem.left = rcSubItem.right;

		iIndex++;
		if (itMap == m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.find(m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd))
		{
			itMap = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.find(m_stQuoteLstDate.stHeadLstDate.uiHItemBegin);
		}
		else
			itMap++;
	}
	//if (itMap != m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.end())
	//{
	//	Pen pnMore(Color(255,0,255,255));
	//	int iWith = 20;
	//	int iOffB = 2;
	//	int iOffWith = iWith/2 - iOffB*2;
	//	Point ptLine1[3] = {Point(m_stQuoteLstDate.stHeadLstDate.rcHItem.right-iWith/2+iOffB,m_stQuoteLstDate.stHeadLstDate.rcHItem.top + iOffB),
	//		Point(m_stQuoteLstDate.stHeadLstDate.rcHItem.right - iOffB,m_stQuoteLstDate.stHeadLstDate.rcHItem.top + m_stQuoteLstDate.stHeadLstDate.uiHHight/2),
	//		Point(m_stQuoteLstDate.stHeadLstDate.rcHItem.right-iWith/2+iOffB,m_stQuoteLstDate.stHeadLstDate.rcHItem.bottom - iOffB)};
	//	gpCach->DrawLines(&pnMore,ptLine1,3);

	//	Point ptLine2[3] = {Point(m_stQuoteLstDate.stHeadLstDate.rcHItem.right-iWith+iOffB,m_stQuoteLstDate.stHeadLstDate.rcHItem.top + iOffB),
	//		Point(m_stQuoteLstDate.stHeadLstDate.rcHItem.right-iWith/2-iOffB,m_stQuoteLstDate.stHeadLstDate.rcHItem.top + m_stQuoteLstDate.stHeadLstDate.uiHHight/2),
	//		Point(m_stQuoteLstDate.stHeadLstDate.rcHItem.right-iWith+iOffB,m_stQuoteLstDate.stHeadLstDate.rcHItem.bottom - iOffB)};
	//	gpCach->DrawLines(&pnMore,ptLine2,3);
	//}
}
BOOL CDlgTrendChart_D::HitTestQuoteLstItemClick(CPoint ptCur,UINT &uiItem,UINT &uiSubItem)
{
	BOOL bInLstClick = FALSE;
	uiItem = uiSubItem = 0;
	do 
	{
		if (!m_stQuoteLstDate.rcLst.PtInRect(ptCur))break;
		int iOffX = ptCur.x - m_stQuoteLstDate.rcLst.left;
		int iOffY = ptCur.y - m_stQuoteLstDate.rcLst.top;

		if (iOffY <= m_stQuoteLstDate.stHeadLstDate.uiHHight)
		{
			int iIndex = 0;
			int iSumWith = 0;
			uiItem = 0;
			TmapQuoteLstSubItemDate::iterator itMap;
			for (itMap = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.begin();
				itMap != m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.end();/*itMap++*/)
			{
				if (iIndex >= m_stQuoteLstDate.stHeadLstDate.uiDisplayNum)break;
				iSumWith += (itMap->second.iSWith + m_stQuoteLstDate.stHeadLstDate.uiCorrectionWith);
				if (iSumWith >= iOffX)
				{
					bInLstClick = TRUE;
					uiSubItem = itMap->first;
					break;
				}

				iIndex++;
				if (itMap == m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.find(m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd))
				{
					itMap = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.find(m_stQuoteLstDate.stHeadLstDate.uiHItemBegin);
				}
				else
					itMap++;
			}
			break;
		}

		if (m_stQuoteLstDate.stQuoteItemLst.rcLstDisplayItem.PtInRect(ptCur))
		{
			bInLstClick = TRUE;
			iOffY -= m_stQuoteLstDate.stHeadLstDate.uiHHight;
			int iHitClick = iOffY/m_stQuoteLstDate.stQuoteItemLst.uiItemHight/*+ ((iOffY%m_stQuoteLstDate.mapQuoteItemLstDate !=0)?1:0)*/;
			uiItem = m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin + iHitClick;
		}
	} while (0);

	return bInLstClick;
}

UINT CDlgTrendChart_D::CalculateString(CString strRes,int iFontSize )
{
	CRect rcCach(0,0,100,100);
	Gdiplus::Bitmap cachBmp(rcCach.Width(),rcCach.Height());
	Graphics grphCach(&cachBmp);

	Gdiplus::Font ft(FontFamily::GenericSansSerif(),iFontSize,0,UnitPoint);
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);

	USES_CONVERSION;
	Gdiplus::PointF ptf(0,0);
	Gdiplus::RectF rcfz;
	grphCach.MeasureString(T2W(strRes),-1,&ft,ptf,&format,&rcfz);

	return rcfz.Width;
}

void CDlgTrendChart_D::DrawQuoteLstItem(Graphics * gpCach,CRect rcDraw)
{
	if (!IsValidRect(gpCach,rcDraw)||m_stQuoteLstDate.stHeadLstDate.uiDisplayNum == 0)return;

	USES_CONVERSION;
	Gdiplus::FontFamily fm(T2W(_T("宋体")));
	Gdiplus::Font ft(/*&fm*/FontFamily::GenericSansSerif(),12/*stcSubItem.iTextSize*/,0/*FontStyleRegular*//*FontStyleBold*//*stcSubItem.iTextType*/,UnitPoint/*UnitPixel*/);
	StringFormat format;
	SolidBrush sbrText(Color(255,255,255,255));
	SolidBrush sbrSel(Color(255,0,0,255));

	TmapQuoteItemLstDate::iterator itMapItem;
	TmapQuoteLstSubItemDate::iterator itMapSubItem;
	int iDisplayItemNum = 0;
	int iIndex = 0;
	CRect rcSubItem;
	itMapItem = m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.find(m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin);
	for (;itMapItem != m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.end();itMapItem++)
	{
		iIndex = 0;
		rcSubItem = itMapItem->second.rcItem;
		if (iDisplayItemNum >= m_stQuoteLstDate.stQuoteItemLst.uiItemDiplayMax)break;

		if (m_stQuoteLstDate.stQuoteItemLst.uiCurSel == itMapItem->first)
		{
			gpCach->FillRectangle(&sbrSel,Rect(rcSubItem.left+1,rcSubItem.top,rcSubItem.Width()-1,rcSubItem.Height()));
		}
		for (itMapSubItem = itMapItem->second.mapQuoteLstSubItemDate.begin();
			itMapSubItem != itMapItem->second.mapQuoteLstSubItemDate.end();/*itMapSubItem++*/)
		{
			if (iIndex >= m_stQuoteLstDate.stHeadLstDate.uiDisplayNum)break;
			format.SetAlignment((StringAlignment)itMapSubItem->second.iSTextAlignmentType);
			sbrText.SetColor(Color(itMapSubItem->second.clrSItemText));
			rcSubItem.right = rcSubItem.left + m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.find(iIndex)->second.iSWith + m_stQuoteLstDate.stHeadLstDate.uiCorrectionWith;
			gpCach->DrawString(T2W(itMapSubItem->second.strSItemText),-1,&ft,RectF(rcSubItem.left,rcSubItem.top,rcSubItem.Width(),rcSubItem.Height()),
				&format,&sbrText);
			rcSubItem.left = rcSubItem.right;

			iIndex++;
			if (itMapSubItem == itMapItem->second.mapQuoteLstSubItemDate.find(m_stQuoteLstDate.stHeadLstDate.uiHKeepBackItemIDEnd))
			{
				itMapSubItem = itMapItem->second.mapQuoteLstSubItemDate.find(m_stQuoteLstDate.stHeadLstDate.uiHItemBegin);
			}
			else
				itMapSubItem++;
		}	

		iDisplayItemNum++;
	}
}
void CDlgTrendChart_D::DrawQuoteLstTab(Graphics * gpCach,CRect rcDraw)
{
	if (!IsValidRect(gpCach,rcDraw)||m_stQuoteLstDate.stHeadLstDate.uiDisplayNum == 0)return;

	CaluRectTab(gpCach,rcDraw,&(m_stQuoteLstDate.stTabInfo));
	DrawTab(gpCach,rcDraw,&(m_stQuoteLstDate.stTabInfo));
}
void CDlgTrendChart_D::DrawHScroll(Graphics * gpCach,CRect rcDraw,HScrollInfo *pstHScrollInfo)
{
	if (!IsValidRect(gpCach,rcDraw)||m_stQuoteLstDate.stHeadLstDate.uiDisplayNum == 0)return;


	int iHeigth = pstHScrollInfo->iBtnWith;
	int iThumbLeft = pstHScrollInfo->iThumbLeft;
	int iThumbRight = pstHScrollInfo->iThumbRight;

	CSkinAttribute_ScrollBar *pSkin = (CSkinAttribute_ScrollBar *)CDateManage::Instance()->GetSkinAttribute(5);
	CRect rcClt = pstHScrollInfo->rcHScroll;

	//Gdiplus::Bitmap cachBmp(rcClt.Width(),rcClt.Height());
	//Graphics graphicsDraw(pDC->m_hDC);
	//Graphics graphics(&cachBmp);
	//graphicsDraw.SetSmoothingMode(SmoothingModeHighQuality);
	//graphics.SetSmoothingMode(SmoothingModeHighQuality);
	SolidBrush srBk(pSkin->clrBkAll);
	Pen pnBd1(pSkin->clrAllLineOut);
	Pen pnBd2(pSkin->clrAllLineIn);
	Pen pnArrow(pSkin->clrArrow,2);
	gpCach->FillRectangle(&srBk,Rect(rcClt.left,rcClt.top,rcClt.Width(),rcClt.Height()));
	//gpCach->DrawRectangle(&pnBd1,Rect(rcClt.left,rcClt.top,rcClt.Width()-1,rcClt.Height()-1));
	//gpCach->DrawRectangle(&pnBd2,Rect(rcClt.left+1,rcClt.top+1,rcClt.Width()-3,rcClt.Height()-3));

	srBk.SetColor(pSkin->clrBtnBkNormal);
	pnBd1.SetColor(pSkin->clrLineIn);
	pnBd2.SetColor(pSkin->clrLineOut);
	gpCach->FillRectangle(&srBk,Rect(rcClt.left,rcClt.top,iHeigth,rcClt.Height()));
	gpCach->DrawRectangle(&pnBd1,Rect(rcClt.left,rcClt.top,iHeigth-1,rcClt.Height()-1));
	gpCach->DrawRectangle(&pnBd2,Rect(rcClt.left+1,rcClt.top+1,iHeigth-3,rcClt.Height()-3));
	int iOffH = 4;
	int ioffW = 6;
	Point ptArrow[] = {Point(rcClt.left+iHeigth-ioffW,rcClt.top+iOffH),Point(rcClt.left+ioffW,rcClt.top+rcClt.Height()/2),Point(rcClt.left+iHeigth-ioffW,rcClt.top + rcClt.Height()-iOffH)};
	gpCach->DrawLines(&pnArrow,ptArrow,3);

	srBk.SetColor(pSkin->clrBoxBkNormal);
	pnBd1.SetColor(pSkin->clrLineOut);
	pnBd2.SetColor(pSkin->clrLineIn);
	CRect rcBox(rcClt.left + iThumbLeft,rcClt.top,rcClt.left + iThumbRight,rcClt.bottom);
	rcBox.InflateRect(0,-1,0,-1);
	gpCach->FillRectangle(&srBk,Rect(rcBox.left,rcBox.top+1,rcBox.Width(),rcBox.Height()-2));
	gpCach->DrawRectangle(&pnBd1,Rect(rcBox.left,rcBox.top,rcBox.Width()-1,rcBox.Height()-1));
	gpCach->DrawRectangle(&pnBd2,Rect(rcBox.left+1,rcBox.top+1,rcBox.Width()-3,rcBox.Height()-3));

	srBk.SetColor(pSkin->clrBtnBkNormal);
	pnBd1.SetColor(pSkin->clrLineIn);
	pnBd2.SetColor(pSkin->clrLineOut);
	gpCach->FillRectangle(&srBk,Rect(rcClt.right-iHeigth,rcClt.top,iHeigth,rcClt.Height()));
	gpCach->DrawRectangle(&pnBd1,Rect(rcClt.right-iHeigth,rcClt.top,iHeigth-1,rcClt.Height()-1));
	gpCach->DrawRectangle(&pnBd2,Rect(rcClt.right-iHeigth+1,rcClt.top+1,iHeigth-3,rcClt.Height()-3));
	Point ptArrowR[] = {Point(rcClt.right-iHeigth+ioffW,rcClt.top + iOffH),Point(rcClt.right - ioffW,rcClt.top + rcClt.Height()/2),
		Point(rcClt.right-iHeigth+ioffW,rcClt.top + rcClt.Height()-iOffH)};
	gpCach->DrawLines(&pnArrow,ptArrowR,3);
}
void CDlgTrendChart_D::DrawQuoteLst(Graphics * gpCach,CRect rcDraw)
{
	if (!IsValidRect(gpCach,rcDraw))return;
	rcDraw.InflateRect(-5,-5,-5,-5);

	CaluQuteLstRect(rcDraw);
	Pen penBd(Color(255,255,0,0));
	gpCach->DrawRectangle(&penBd,Rect(rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height()));

	DrawQuoteLstHead(gpCach,m_stQuoteLstDate.stHeadLstDate.rcHItem);
	DrawQuoteLstItem(gpCach,rcDraw);
	
	DrawQuoteLstTab(gpCach,m_stQuoteLstDate.stTabInfo.rcTab);
	DrawHScroll(gpCach,m_stQuoteLstDate.stHScrollInfo.rcHScroll,&(m_stQuoteLstDate.stHScrollInfo));
}

void CDlgTrendChart_D::TransReportDate_ZYQ(STUReportDataResp_ZYQ_Client &stReportDataResp_ZYQ_Client)
{
	m_lockmapHisTTDate.Lock();
	do 
	{
		TmapSymbolClassInfo::iterator itMap;
		//itMap = m_stInitMarketDate.mapSymbolClass.find(stReportDataResp_ZYQ_Client.m_oRepIndex.m_usMarketType);
		//if (itMap == m_stInitMarketDate.mapSymbolClass.end())break;
		int iVReportDataNum = stReportDataResp_ZYQ_Client.m_vReportData.size();
		if (iVReportDataNum <= 0)break;
		
		m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.clear();
		int iIndexBg;
		iIndexBg = m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin = stReportDataResp_ZYQ_Client.m_oRepIndex.m_usBeginPos;
		m_stQuoteLstDate.stQuoteItemLst.uiCurSel = stReportDataResp_ZYQ_Client.m_oRepIndex.m_usBeginPos;

		QuoteItemLstDate stQuoteItemLstDate;
		QuoteLstSubItemDate stQuoteLstSubItemDate;
		STUReportDataResp_ZYQ_Client_Vec vecDate;

		//CString strQuoteLstHeader1[] = {_T("编号"),_T("商品代码"),_T("商品名称"),_T("开盘价"),_T("最新价"),_T("现量"),
		//	_T("涨跌"),_T("幅度"),_T("最高价"),_T("最低价"),_T("昨收盘价"),_T("买价"),_T("买量"),_T("卖价"),_T("卖量"),
		//	_T("成交量"),_T("成交金额"),_T("均价"),_T("库存量"),_T("量比"),_T("委比")};
		int iAllHead = m_stQuoteLstDate.stHeadLstDate.mapQuoteLstSubItemDate.size();
		int x = 0;
		CString strTemp;
		SymbolTradeInfo_ZYQ stSymbolTradeInfo_ZYQ;
		float fPowVol = pow(10,(double)2);
		float fPowPrice;// = pow(10,(double)iDc);

		DWORD dwclrNormal = Color::MakeARGB(255,255,255,0);
		DWORD dwclrIndex = Color::MakeARGB(255,201,201,201);
		DWORD dwclrUD[3] = {Color::MakeARGB(255,255,0,0),dwclrIndex,Color::MakeARGB(255,0,255,0)};

		double dbTemp,dbYesPrice;
		unsigned int uiYesPrice;
		UINT iUnit = 1;
		int m = 0;
		stcQuotePrice stQuotePrice;
		for (int i = 0; i < iVReportDataNum; i++)
		{
			vecDate = stReportDataResp_ZYQ_Client.m_vReportData[i];
			stSymbolTradeInfo_ZYQ.stSymbolInfo.cMarket = vecDate.m_oSSymobl.m_usMarketType;
			stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode = vecDate.m_oSSymobl.ssSymbolCode.c_str();
			if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stSymbolTradeInfo_ZYQ))continue;

			iUnit = stSymbolTradeInfo_ZYQ.stSymbolInfo.iAgreeunit;
			if (iUnit <= 0)
			{
				iUnit = 1;
			}
			//stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit = 2;
			//stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolName = _T("11111");
			fPowPrice = pow(10,(double)stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);

			stQuoteItemLstDate.mapQuoteLstSubItemDate.clear();
			stQuoteItemLstDate.uiItemID = iIndexBg+m;
			stQuoteItemLstDate.uiItemHight = 18;

			//dbYesPrice = vecDate.m_oReportPublicData.m_uiYesClosePrice*1.0/fPowPrice;
			uiYesPrice = vecDate.m_oReportPublicData.m_uiYesClosePrice;

			m++;
			x = 0;//编号
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
			strTemp.Format(_T("%d"),stQuoteItemLstDate.uiItemID);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = strTemp;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//1商品代码
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = vecDate.m_oSSymobl.ssSymbolCode.c_str();
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//2商品名称
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolName;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//3开盘价
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiOpenPrice,uiYesPrice)];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiOpenPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//4最新价
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiNewPrice,uiYesPrice)];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//5现量
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiNewQty*1.0/fPowVol,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//6涨跌
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_iPriceRiseDown,0)];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iPriceRiseDown*1.0/fPowVol,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//7幅度
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_iPriceRiseDownRate,0)];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iPriceRiseDownRate*1.0/fPowVol,2) + _T("%");
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//8最高价
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiMaxPrice,uiYesPrice)];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiMaxPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//9最低价
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiMinPrice,uiYesPrice)];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiMinPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//10昨收盘价
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
			if (vecDate.m_oReportPublicData.m_uiYesClosePrice <= 0)
			{
				stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = (_T("--"));
			}
			else
			    stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiYesClosePrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//11买价
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice,uiYesPrice)];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//12买量
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_buyVol.m_uiQty*1.0/fPowVol,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//13卖价
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice,uiYesPrice)];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//14卖量
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_sellVol.m_uiQty*1.0/fPowVol,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//15成交量
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiTotalTurnoverQty*1.0/fPowVol,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//16成交金额
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//17均价
			if (vecDate.m_oReportPublicData.m_uiTotalTurnoverQty > 0)
			{
				double dbJun = vecDate.m_oReportPublicData.m_iTotalTurnoverFunds*1.0/vecDate.m_oReportPublicData.m_uiTotalTurnoverQty/iUnit;
				strTemp = CDateManage::DoubleToString(dbJun,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
				stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(fPowPrice*dbJun,uiYesPrice)];
			}
			else
			{
				strTemp = (_T("--"));
				stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
			}
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = strTemp;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//18库存量
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[2];
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(stSymbolTradeInfo_ZYQ.stSymbolInfo.uiTotalShares*1.0/fPowVol,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//19量比
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iQtyRate*1.0/fPowVol,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			x++;//20委比
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iOrderRate*1.0/fPowVol,2);
			stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

			m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate[stQuoteItemLstDate.uiItemID] = stQuoteItemLstDate;

			////////////
			stQuotePrice.fPrice = vecDate.m_oReportPublicData.m_uiNewPrice*1.0/fPowPrice;
			stQuotePrice.fYesClosePrice = vecDate.m_oReportPublicData.m_uiYesClosePrice*1.0/fPowPrice;
			stQuotePrice.lastTick = vecDate.m_oTDateTime.m_iUpdateDataTime;

			stQuotePrice.stBS[0].stB.fPrice = vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice*1.0/fPowPrice;
			stQuotePrice.stBS[0].stB.fQty = vecDate.m_oQtyPriceV1.m_buyVol.m_uiQty*1.0/fPowVol;
			stQuotePrice.stBS[0].stS.fPrice = vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice*1.0/fPowPrice;
			stQuotePrice.stBS[0].stS.fQty = vecDate.m_oQtyPriceV1.m_sellVol.m_uiQty*1.0/fPowVol;
			m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate[stQuoteItemLstDate.uiItemID].stQuotePrice = stQuotePrice;
			if (m_pSpotQuoteDate&&stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode.CompareNoCase(m_pSpotQuoteDate->GetSymbol().szCode) == 0)
			{
				m_pSpotQuoteDate->SetQuotePrice(stQuotePrice);
				m_pSpotQuoteDate->NotifyRecvWnd(0,0);
			}
		}

	} while (0);

	m_lockmapHisTTDate.Unlock();
	RedrawWindow();
}

UINT CDlgTrendChart_D::GetUDColor(int iDesDate,int iStandardDate)
{
	if (iDesDate > iStandardDate)return 0;
	else if (iDesDate < iStandardDate)return 2;
	else return 1;
}
UINT CDlgTrendChart_D::GetUDColorF(double fDiffValue, double fStandardValue)
{
	if (fDiffValue > fStandardValue)return 0;
	else if (fDiffValue < fStandardValue)return 2;
	else return 1;
}
float CDlgTrendChart_D::getMaxDate(float fDate1,float fDate2)
{
	float fDiffDate = fDate1 - fDate2;
	if (fDiffDate > (0.000000001))return fDate1;
	else if (fDiffDate < (0.000000001))return fDate2;
	else return fDate1;
}
float CDlgTrendChart_D::getMinDate(float fDate1,float fDate2)
{
	float fDiffDate = fDate1 - fDate2;
	if (fDiffDate > (0.000000001))return fDate2;
	else if (fDiffDate < (0.000000001))return fDate1;
	else return fDate1;
}
void CDlgTrendChart_D::SetQuoteItemText(UINT uiItem,UINT uiSubItem,CString strText,UINT iMode,double fStandardValue)
{
	do 
	{
		if (uiItem <= 0)break;
		TmapQuoteItemLstDate::iterator itMap;
		itMap = m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.find(uiItem);
		if (itMap == m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.end())break;
		//1最新价2涨跌3均价4最高价5最低价6昨收价
		DWORD dwclrUD[3] = {Color::MakeARGB(255,255,0,0),Color::MakeARGB(255,201,201,201),Color::MakeARGB(255,0,255,0)};
		TmapQuoteLstSubItemDate::iterator itMapSub;
		itMapSub = itMap->second.mapQuoteLstSubItemDate.find(uiSubItem);
		if (itMapSub == itMap->second.mapQuoteLstSubItemDate.end())break;
		if (iMode == 1)
		{	
			itMapSub->second.clrSItemText = dwclrUD[GetUDColorF(atof(strText)-atof(itMapSub->second.strSItemText))];
			itMapSub->second.strSItemText = strText;
		}
		else if (iMode == 2)
		{
			{
				itMapSub->second.clrSItemText = dwclrUD[GetUDColorF(atof(strText))];	
			}
			itMapSub->second.strSItemText = strText;
		}
		else if (iMode == 3)
		{
			CString strNO = (_T("--"));
			if (strText.CompareNoCase(strNO) == 0)
			{
				itMapSub->second.clrSItemText = dwclrUD[1];	
			}
			else
			    itMapSub->second.clrSItemText = dwclrUD[GetUDColorF(atof(strText)-(fStandardValue))];
			itMapSub->second.strSItemText = strText;
		}
		else if (iMode == 4)
		{
			UINT iIndex = GetUDColorF(atof(strText)-atof(itMapSub->second.strSItemText));
			if (iIndex == 0)
			{
				itMapSub->second.strSItemText = strText;
				itMapSub->second.clrSItemText = dwclrUD[GetUDColorF(atof(strText)-(fStandardValue))];
			}
		}
		else if (iMode == 5)
		{
			UINT iIndex = GetUDColorF(atof(strText)-atof(itMapSub->second.strSItemText));
			if (iIndex == 2)
			{
				itMapSub->second.strSItemText = strText;
				itMapSub->second.clrSItemText = dwclrUD[GetUDColorF(atof(strText)-(fStandardValue))];
			}
		}
		else if (iMode == 6)
		{
			CString strNO = (_T("--"));
			if (itMapSub->second.strSItemText.CompareNoCase(CString(strNO)) == 0)
			{
				//UINT iIndex = GetUDColorF(atof(strText));
				itMapSub->second.strSItemText = strText;
			}	
		}
		else if(iMode == 7)
		{
			itMapSub->second.strSItemText = strText;
			itMapSub->second.clrSItemText = dwclrUD[1];
		}
		else
		{
			itMapSub->second.strSItemText = strText;
		}
	
	} while (0);
}
pQuoteItemLstDate CDlgTrendChart_D::GetCurSelQuoteItem(int iItem)
{
	if (iItem <= 0)return NULL;
	TmapQuoteItemLstDate::iterator itMap;
	itMap = m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.find(iItem);
	if (itMap == m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.end())return NULL;

	return &itMap->second;
}
CString CDlgTrendChart_D::GetCurSelQuoteSubItem(int iItem,int iSubItem)
{
	CString strTemp = _T("");
	if (iItem <=0 ||iSubItem < 0)return strTemp;

	pQuoteItemLstDate pstQuoteItemLstDate = GetCurSelQuoteItem(iItem);
	if (pstQuoteItemLstDate)
	{
		TmapQuoteLstSubItemDate::iterator itMapSub;
		itMapSub = pstQuoteItemLstDate->mapQuoteLstSubItemDate.find(iSubItem);
		if (itMapSub != pstQuoteItemLstDate->mapQuoteLstSubItemDate.end())
		{
			strTemp = itMapSub->second.strSItemText;
		}
	}
	return strTemp;
}
CString CDlgTrendChart_D::GetCurSelQuoteSubItem(pQuoteItemLstDate pQuoteItme,int iSubItem)
{
	CString strTemp = _T("");
	do 
	{
		if (pQuoteItme == NULL)break;
		TmapQuoteLstSubItemDate::iterator itMapSub;
		itMapSub = pQuoteItme->mapQuoteLstSubItemDate.find(iSubItem);
		if (itMapSub != pQuoteItme->mapQuoteLstSubItemDate.end())
		{
			strTemp = itMapSub->second.strSItemText;
		}
	} while (0);
	return strTemp;
}
void CDlgTrendChart_D::TransInitMarket_ZYQ(vector<STUMarketInfo_ZYQ_Client> & vecMarketInfo)
{
	const int iTabNum = vecMarketInfo.size();
	if (iTabNum <= 0)return;
	//CString strDisplay[iTabNum] = {_T("所有藏品"),_T("邮票"),_T("电话卡"),
	//	_T("邮资封片"),_T("指数"),_T("自选藏品"),_T("更多>>")};
	int iWith = 50;
	for (int i = 0; i < iTabNum; i++)
	{	
		m_stQuoteLstDate.stTabInfo.mapTabItme[vecMarketInfo[i].m_usMarketType].strItemName = vecMarketInfo[i].m_ssMarketName.c_str();
		m_stQuoteLstDate.stTabInfo.mapTabItme[vecMarketInfo[i].m_usMarketType].uiItemId = vecMarketInfo[i].m_usMarketType;
		m_stQuoteLstDate.stTabInfo.mapTabItme[vecMarketInfo[i].m_usMarketType].uiItemWith = 
			m_stQuoteLstDate.stTabInfo.mapTabItme[vecMarketInfo[i].m_usMarketType].strItemName.GetLength()*15+5;
	}
	//m_stQuoteLstDate.stTabInfo.mapTabItme[0].strItemName = _T("所有藏品");
	//m_stQuoteLstDate.stTabInfo.mapTabItme[0].uiItemId = 0;
	//m_stQuoteLstDate.stTabInfo.mapTabItme[0].uiItemWith = 90;
	//m_stQuoteLstDate.stTabInfo.uiCurSel = 0;

	//m_stQuoteLstDate.stTabInfo.mapTabItme[100].strItemName = _T("申购藏品");
	//m_stQuoteLstDate.stTabInfo.mapTabItme[100].uiItemId = 100;
	//m_stQuoteLstDate.stTabInfo.mapTabItme[100].uiItemWith = 90;

	//m_stQuoteLstDate.stHScrollInfo.iBtnWith = 16;
	//m_stQuoteLstDate.stHScrollInfo.iAllDisPlay = 100;
	//m_stQuoteLstDate.stHScrollInfo.iPageDisPlay = 100;
}
void CDlgTrendChart_D::TransReportDetailDate_ZYQ(STUReportDataDetailResp_ZYQ_Client &stReportDataDetail)
{
	m_lockmapHisTTDate.Lock();
	do 
	{
		if (/*stReportDataDetail.m_oSSymobl.m_usMarketType != stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket ||*/
			stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase(stReportDataDetail.m_oSSymobl.ssSymbolCode.c_str())!=0)
			break;

		stcQuotePrice stQuotePrice;
		UINT iUnit = stChartDate.stSymbolTradeInfo.stSymbolInfo.iAgreeunit;
		if (iUnit <= 0)
		{
			iUnit = 1;
		}
		//SymbolTradeInfo_ZYQ stSymbolTradeInfo_ZYQ;
		//if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stSymbolTradeInfo_ZYQ))continue;
		float fPowPrice = pow(10,(double)stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit);
		float fPowVol = pow(10,(double)2);

		stChartDate.stTopPartDate.Init();
		stChartDate.stTopPartDate.fWibi = stReportDataDetail.m_oReportPublicData.m_iOrderRate*1.0/fPowVol;
		stChartDate.stTopPartDate.fWeicha = stReportDataDetail.m_oExt.m_iOrderDiff*1.0/fPowVol;
		stChartDate.stTopPartDate.iBSNum = stReportDataDetail.m_vQtyPriceV.size();
		if(stChartDate.stTopPartDate.iBSNum < 0 || stChartDate.stTopPartDate.iBSNum > 5)
			stChartDate.stTopPartDate.iBSNum = 0;
		for (int i = 0; i < stChartDate.stTopPartDate.iBSNum; i++)
		{
			stQuotePrice.stBS[i].stB.fPrice = stChartDate.stTopPartDate.sVolPrice[i].fbuyQuote.fPrice = stReportDataDetail.m_vQtyPriceV[i].m_buyVol.m_uiPrice*1.0/fPowPrice;
			stQuotePrice.stBS[i].stB.fQty = stChartDate.stTopPartDate.sVolPrice[i].fbuyQuote.fQty = stReportDataDetail.m_vQtyPriceV[i].m_buyVol.m_uiQty*1.0/fPowVol;
			stQuotePrice.stBS[i].stS.fPrice = stChartDate.stTopPartDate.sVolPrice[i].fsellQuote.fPrice = stReportDataDetail.m_vQtyPriceV[i].m_sellVol.m_uiPrice*1.0/fPowPrice;
			stQuotePrice.stBS[i].stS.fQty = stChartDate.stTopPartDate.sVolPrice[i].fsellQuote.fQty = stReportDataDetail.m_vQtyPriceV[i].m_sellVol.m_uiQty*1.0/fPowVol;
		}

		stQuotePrice.fPrice = stChartDate.stTopPartDate.fNewPrice = stReportDataDetail.m_oReportPublicData.m_uiNewPrice*1.0/fPowPrice;
		if (stReportDataDetail.m_oReportPublicData.m_uiTotalTurnoverQty > 0)
		{
			stChartDate.stTopPartDate.fPinjunPrice = (stReportDataDetail.m_oReportPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice)/(stReportDataDetail.m_oReportPublicData.m_uiTotalTurnoverQty/fPowVol)/iUnit;

		}
		else
			stChartDate.stTopPartDate.fPinjunPrice = 0.00;
		stChartDate.stTopPartDate.fZhangDie = stReportDataDetail.m_oReportPublicData.m_iPriceRiseDown*1.0/fPowVol;;
		stChartDate.stTopPartDate.fOpenPrice = stReportDataDetail.m_oReportPublicData.m_uiOpenPrice*1.0/fPowPrice;;

		stChartDate.stTopPartDate.fFudu = stReportDataDetail.m_oReportPublicData.m_iPriceRiseDownRate*1.0/fPowVol;
		stChartDate.stTopPartDate.fMaxPrice = stReportDataDetail.m_oReportPublicData.m_uiMaxPrice*1.0/fPowPrice;
		stChartDate.stTopPartDate.fNowVol = stReportDataDetail.m_oReportPublicData.m_uiNewQty*1.0/fPowVol;
		stChartDate.stTopPartDate.fMinPrice = stReportDataDetail.m_oReportPublicData.m_uiMinPrice*1.0/fPowPrice;

		stChartDate.stTopPartDate.fTotalVol = stReportDataDetail.m_oReportPublicData.m_uiTotalTurnoverQty*1.0/fPowVol;
		stChartDate.stTopPartDate.fVolRatio = stReportDataDetail.m_oReportPublicData.m_iQtyRate*1.0/fPowVol;
		stChartDate.stTopPartDate.fDealMoney = stReportDataDetail.m_oReportPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice;

		if (stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares > 0)
		{
			stChartDate.stTopPartDate.fHuanshouRatio = stReportDataDetail.m_oReportPublicData.m_uiTotalTurnoverQty*1.0/stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares*100;

		}
		else
			stChartDate.stTopPartDate.fHuanshouRatio= 100;
		stQuotePrice.fYesClosePrice = stChartDate.stTopPartDate.fYesDPrice = stReportDataDetail.m_oReportPublicData.m_uiYesClosePrice*1.0/fPowPrice;
		stChartDate.stTopPartDate.fYesSettlPrice = stReportDataDetail.m_oReportPublicData.m_uiYesSetllPrice*1.0/fPowPrice;
		stChartDate.stTopPartDate.fOutPan = stReportDataDetail.m_oExt.m_uiPOrderSaleQty*1.0/fPowVol;
		stChartDate.stTopPartDate.fInPan = stReportDataDetail.m_oExt.m_uiPOrderBuyQty*1.0/fPowVol;

		if (m_pSpotQuoteDate&&stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase(m_pSpotQuoteDate->GetSymbol().szCode) == 0)
		{
			stQuotePrice.fPrice = stChartDate.stTopPartDate.fNewPrice;
			stQuotePrice.fYesClosePrice = stChartDate.stTopPartDate.fYesDPrice;
			stQuotePrice.lastTick = stReportDataDetail.m_oTDateTime.m_iUpdateDataTime;
			m_pSpotQuoteDate->SetQuotePrice(stQuotePrice);
			m_pSpotQuoteDate->NotifyRecvWnd(0,0);
		}
	} while (0);
	m_lockmapHisTTDate.Unlock();

	/////////////////
	GetMaxMinPrice(stChartDate.stTrendDate.fMaxPrice,stChartDate.stTrendDate.fMinPrice,stChartDate.stTrendDate.fYsPrice);
	GetMaxMinVol(stChartDate.stTrendDate.fMaxVol,stChartDate.stTrendDate.fMinVol,stChartDate.stTrendDate.fPPVol);

	float fMaxV,fMinV,fMarkV;
	GetMaxMinPrice_KMAP(fMaxV,fMinV,fMarkV);
	stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,0);
	GetMaxMinVol_KMAP(fMaxV,fMinV,fMarkV);
	stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,1);
	GetMaxMinOther_KMAP(fMaxV,fMinV,fMarkV);
	stChartDate.stKDate.SetMaxMinValue(fMaxV,fMinV,fMarkV,2);
	//////////////////

	STUSimpleSymbol_ZYQ stSimpleSymbol_ZYQ;
	stSimpleSymbol_ZYQ.m_usMarketType = stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket;
	strcpy(stSimpleSymbol_ZYQ.m_szSymbolCode,stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.GetBuffer(0));
	CDateManage_ZYQ::Instance()->ReqDealDetail_ZYQ(&stSimpleSymbol_ZYQ);

	RedrawWindow();
}

void CDlgTrendChart_D::TransDealDetailDate_ZYQ(STUTraceUnit_ZYQ_Client &stTraceUnit_ZYQ)
{
	m_lockmapHisTTDate.Lock();
	do
	{
		if (stTraceUnit_ZYQ.m_oSymbol.m_usMarketType != stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket ||
			stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase(stTraceUnit_ZYQ.m_oSymbol.ssSymbolCode.c_str())!=0)
			break;

		DealDetailsDate stDealDetails;
		stChartDate.stTrendDate.Clear();

		float fPowPrice = pow(10,(double)stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit);
		float fPowVol = pow(10,(double)2);
		int iNum = stTraceUnit_ZYQ.m_vTraceUnit.size();
		for (int i = 0; i < iNum; i++)
		{
			stDealDetails.iTradeDay = stTraceUnit_ZYQ.m_vTraceUnit[i].m_oTDateTime.m_iTradeDay;
			stDealDetails.uiDateTime = stTraceUnit_ZYQ.m_vTraceUnit[i].m_oTDateTime.m_iUpdateDataTime;//20150128141327;
			stDealDetails.fPrice = stTraceUnit_ZYQ.m_vTraceUnit[i].m_uiPrice*1.0/fPowPrice;
			stDealDetails.fQty = stTraceUnit_ZYQ.m_vTraceUnit[i].m_uiQty*1.0/fPowVol;
			//if (stTraceUnit_ZYQ.m_vTraceUnit[i].m_chTurnoverDirec != 0 && 
			//	stTraceUnit_ZYQ.m_vTraceUnit[i].m_chTurnoverDirec != 1)
			//{
			//	stDealDetails.cBSDirec = 0;
			//}
			//else
				stDealDetails.cBSDirec = stTraceUnit_ZYQ.m_vTraceUnit[i].m_chTurnoverDirec;
			
			stChartDate.stDealDetailsInfo.vecDealDetails.push_back(stDealDetails);
			//stChartDate.stDealDetailsInfo.vecDealDetails.insert(stChartDate.stDealDetailsInfo.vecDealDetails.begin(),stDealDetails);
		}

	} while (0);
	m_lockmapHisTTDate.Unlock();
	
	STUSimpleSymbol_ZYQ stSimpleSymbol_ZYQ;
	stSimpleSymbol_ZYQ.m_usMarketType = stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket;
	strcpy(stSimpleSymbol_ZYQ.m_szSymbolCode,stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.GetBuffer(0));
	CDateManage_ZYQ::Instance()->ReqTimeTrendDate_ZYQ(&stSimpleSymbol_ZYQ);

	RedrawWindow();
}
void CDlgTrendChart_D::TransTimeTrendDate_ZYQ(STUSTKMinUnit_ZYQ_Client &stTKMinUnit)
{
	m_lockmapHisTTDate.Lock();
	do
	{
		if (stTKMinUnit.m_oSymbol.m_usMarketType != stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket ||
			stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase(stTKMinUnit.m_oSymbol.ssSymbolCode.c_str())!=0)
			break;

		int iVecNum = stTKMinUnit.m_vMinUnit.size();
		int iTimeSlot,iAllTm;
		GetTradeTimeInfo(iTimeSlot,iAllTm);
		if (iTimeSlot <= 0 || iVecNum <= 0)break;
		stChartDate.stTrendDate.Clear();

		UINT iUnit = stChartDate.stSymbolTradeInfo.stSymbolInfo.iAgreeunit;
		if (iUnit <= 0)
		{
			iUnit = 1;
		}
		float fPowPrice = pow(10,(double)stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit);
		float fPowVol = pow(10,(double)2);

		UINT64 iBg = stTKMinUnit.m_vMinUnit[0].m_oTDateTime.m_iUpdateDataTime;
		UINT64 iEnd = stTKMinUnit.m_vMinUnit[iVecNum-1].m_oTDateTime.m_iUpdateDataTime;
		HisDateSpot stHisDateSpot;
		HisTTimeDate stHisTTimeDate;
		int iCurPushTm = 0;
		for (int i = 0; i < iVecNum; i++)
		{
			iCurPushTm = stTKMinUnit.m_vMinUnit[i].m_oTDateTime.m_iUpdateDataTime%1000000;
			if (!IsInTradeTime(iCurPushTm))continue;

			stHisTTimeDate.iTradeDay = stTKMinUnit.m_vMinUnit[i].m_oTDateTime.m_iTradeDay;
			stHisTTimeDate.uiDateTime = stTKMinUnit.m_vMinUnit[i].m_oTDateTime.m_iUpdateDataTime;
			stHisTTimeDate.fClosePrice = stTKMinUnit.m_vMinUnit[i].m_uiTurnoverPrice*1.0/fPowPrice;
			stHisTTimeDate.fTrunoverQty = stTKMinUnit.m_vMinUnit[i].m_uiTrunoverQty*1.0/fPowVol;      
			stHisTTimeDate.fOrderQty = stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares*1.0/fPowVol;
			stHisTTimeDate.fYesSettlPrice = stChartDate.stTopPartDate.fYesSettlPrice;
			if (stTKMinUnit.m_vMinUnit[i].m_uiTrunoverQty > 0)
			{
				stHisTTimeDate.fAveragePrice = (stTKMinUnit.m_vMinUnit[i].m_uiTrunoverFunds*1.0/fPowPrice)/(stTKMinUnit.m_vMinUnit[i].m_uiTrunoverQty/fPowVol)/iUnit;
			}
			else
			{
				stHisTTimeDate.fAveragePrice = stHisTTimeDate.fClosePrice;
			}

			stHisDateSpot.stHisTTimeDate = stHisTTimeDate;
			stChartDate.stTrendDate.mapHisTTDate[stHisTTimeDate.uiDateTime] = stHisDateSpot;
		}

		////////////////
		if (stChartDate.stTrendDate.mapHisTTDate.size() <= 0)break;

		TmapHisDateSpot::iterator itMap;
		INT64 iBeginTm = stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[0].inBeginTime;
		INT64 ilMapBgTm = stChartDate.stTrendDate.mapHisTTDate.begin()->first;
		INT64 ilMapEdTm = stChartDate.stTrendDate.mapHisTTDate.rbegin()->first;
		int iYYR = ilMapBgTm/1000000;
		int iMapBgTm = ilMapBgTm%1000000;
		int iMapEdTm = ilMapEdTm%1000000;
		iBeginTm = (iBeginTm/100)*100;
		iMapEdTm = (iMapEdTm/100)*100;
		iMapBgTm = (iMapBgTm/100)*100;
		
		UINT64 iCurFillTm = 0;
		float fBeforPrice = stChartDate.stTopPartDate.fYesDPrice;
		do 
		{
			while(!IsInTradeTime(iBeginTm))
			{
				iBeginTm = GetNextTime(iBeginTm,1,1);
			}
			if (iBeginTm >= iMapEdTm ||
				iBeginTm >= stChartDate.stSymbolTradeInfo.stTradeTime.stTradeTimeSlot[iTimeSlot-1].inEndTime)break;

			iCurFillTm = (INT64)iYYR*1000000 + iBeginTm;
			itMap = stChartDate.stTrendDate.mapHisTTDate.find(iCurFillTm);
			if (itMap != stChartDate.stTrendDate.mapHisTTDate.end())
			{
				fBeforPrice = itMap->second.stHisTTimeDate.fClosePrice;
			}
			else
			{
				stHisTTimeDate.iTradeDay = iYYR;
				stHisTTimeDate.uiDateTime = iCurFillTm;
				stHisTTimeDate.fClosePrice = fBeforPrice;
				stHisTTimeDate.fTrunoverQty = 0;      
				stHisTTimeDate.fOrderQty = 0;
				stHisTTimeDate.fYesSettlPrice = fBeforPrice;
				stHisTTimeDate.fAveragePrice = fBeforPrice;
				stHisDateSpot.stHisTTimeDate = stHisTTimeDate;
				stChartDate.stTrendDate.mapHisTTDate[stHisTTimeDate.uiDateTime] = stHisDateSpot;
			}
			iBeginTm = GetNextTime(iBeginTm,1,1);
		} while (1);
		//////////////////////
	} while (0);
	m_lockmapHisTTDate.Unlock();

	GetMaxMinPrice(stChartDate.stTrendDate.fMaxPrice,stChartDate.stTrendDate.fMinPrice,stChartDate.stTrendDate.fYsPrice);
	GetMaxMinVol(stChartDate.stTrendDate.fMaxVol,stChartDate.stTrendDate.fMinVol,stChartDate.stTrendDate.fPPVol);
	RedrawWindow();

	ReqKDate_ZYQ();
}
void CDlgTrendChart_D::ReqKDate_ZYQ(UINT uiReqCycle,UINT uiHasNum,UINT uiReqNum)
{
	STUSimpleSymbol_ZYQ stSimpleSymbol_ZYQ;
	stSimpleSymbol_ZYQ.m_usMarketType = stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket;
	strcpy(stSimpleSymbol_ZYQ.m_szSymbolCode,stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.GetBuffer(0));
	STUHisKIndexDirec stHisKIndexDirec;
	stHisKIndexDirec.m_ucHisType = uiReqCycle;
	stHisKIndexDirec.m_iHasNum = uiHasNum;
	stHisKIndexDirec.m_iReqNum = uiReqNum;
	CDateManage_ZYQ::Instance()->ReqKDate_ZYQ(&stSimpleSymbol_ZYQ,&stHisKIndexDirec);

	stChartDate.stKDate.stKDateHead.uiCycle = uiReqCycle;
}
void CDlgTrendChart_D::TransKDate_ZYQ(STUHisKUnit_ZYQ_Client &stHisKUnit_ZYQ)
{
	m_lockmapHisTTDate.Lock();
	do 
	{
		if (stHisKUnit_ZYQ.m_oSymbol.m_usMarketType != stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket ||
			stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase(stHisKUnit_ZYQ.m_oSymbol.ssSymbolCode.c_str())!=0)
			break;

		HisUnitK_ZYQ stHisUnitK;
		int iVecNum = stHisKUnit_ZYQ.m_vHisKUnit.size();
		if (iVecNum <= 0)break;

		UINT iUnit = stChartDate.stSymbolTradeInfo.stSymbolInfo.iAgreeunit;
		if (iUnit <= 0)
		{
			iUnit = 1;
		}
		float fPowPrice = pow(10,(double)stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit);
 		float fPowVol = pow(10,(double)2);
		//stChartDate.stKDate.mapHisUnitK_ZYQ.
		float fBeforClosePrice = 0.0;
		TmapHisUnitK_ZYQ::iterator itMap;
		INT64 uiBeforTm = 0;
		itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin();
		if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end())
		{
			uiBeforTm = itMap->first;
		}
		for (int i = 0; i < iVecNum; i++)
		{
			stHisUnitK.uiDate = stHisKUnit_ZYQ.m_vHisKUnit[i].m_oTDateTime.m_iUpdateDataTime;
			stHisUnitK.fOpenPrice = stHisKUnit_ZYQ.m_vHisKUnit[i].m_uiOpenPrice*1.0/fPowPrice;
			stHisUnitK.fClosePrice = stHisKUnit_ZYQ.m_vHisKUnit[i].m_uiClosePrice*1.0/fPowPrice;
			stHisUnitK.fMaxPrice = stHisKUnit_ZYQ.m_vHisKUnit[i].m_uiMaxPrice*1.0/fPowPrice;
			stHisUnitK.fMinPrice = stHisKUnit_ZYQ.m_vHisKUnit[i].m_uiMinPrice*1.0/fPowPrice;
			if (stHisKUnit_ZYQ.m_vHisKUnit[i].m_uiTrunoverQty > 0)
			{
				stHisUnitK.fJunPrice = (stHisKUnit_ZYQ.m_vHisKUnit[i].m_iTotalTurnoverFunds*1.0/fPowPrice)/(stHisKUnit_ZYQ.m_vHisKUnit[i].m_uiTrunoverQty/fPowVol)/iUnit;
			}
			else
				stHisUnitK.fJunPrice = stHisUnitK.fClosePrice;
			stHisUnitK.fBeforClosePrice = fBeforClosePrice;
			stHisUnitK.fTrunoverMoney = stHisKUnit_ZYQ.m_vHisKUnit[i].m_iTotalTurnoverFunds*1.0/fPowPrice;
			stHisUnitK.fTrunoverQty = stHisKUnit_ZYQ.m_vHisKUnit[i].m_uiTrunoverQty*1.0/fPowVol;
			if (stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares > 0)
			{
				stHisUnitK.fTurnoverRate = stHisUnitK.fTrunoverQty*1.0*100/stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares;
			}
			else
				stHisUnitK.fTurnoverRate = 100;

			stChartDate.stKDate.mapHisUnitK_ZYQ[stHisUnitK.uiDate] = stHisUnitK;

			fBeforClosePrice = stHisUnitK.fClosePrice;
		}

		//if (stChartDate.stKDate.uiCurBeginTm == 0 && iVecNum > 0)
		//{
		//	stChartDate.stKDate.uiCurBeginTm = stChartDate.stKDate.mapHisUnitK_ZYQ.begin()->first;
		//}
		if (uiBeforTm > 0)
		{
			itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.find(uiBeforTm);
			if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end())
			{
				itMap->second.fBeforClosePrice = fBeforClosePrice;
			}
		}

		CalcIndexDate(&(stChartDate.stKDate));
	} while (0);
	m_lockmapHisTTDate.Unlock();

	////int iKNum = stChartDate.stKDate.mapHisUnitK_ZYQ.size();
	//GetMaxMinPrice_KMAP(stChartDate.stKDate.fMaxPrice_K,stChartDate.stKDate.fMinPrice_K,stChartDate.stKDate.fYsPrice_K);
	//GetMaxMinVol_KMAP(stChartDate.stKDate.fMaxVol_K,stChartDate.stKDate.fMinVol_K,stChartDate.stKDate.fPPVol_K);
	//GetMaxMinOther_KMAP(stChartDate.stKDate.fMaxOther_K,stChartDate.stKDate.fMinOther_K,stChartDate.stKDate.fPPOther_K);
	RedrawWindow();
}
int CDlgTrendChart_D::IsValidSymbol(CString strSymbolCode)
{
	int iItem = -1;
	TmapQuoteItemLstDate::iterator itMapItem = m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.begin();
	TmapQuoteLstSubItemDate::iterator itMapSubItem;
	for (;itMapItem != m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.end(); itMapItem++)
	{
		itMapSubItem = itMapItem->second.mapQuoteLstSubItemDate.find(1);
		if (itMapSubItem != itMapItem->second.mapQuoteLstSubItemDate.end())
		{
			if (itMapSubItem->second.strSItemText.CompareNoCase(strSymbolCode) == 0)
			{
				iItem = itMapItem->second.uiItemID;
				break;
			}
		}
	}
	return iItem;
}
void CDlgTrendChart_D::TransPushReportDate_ZYQ(STURealDataPush_ZYQ_Client &stPushReportDate)
{
	m_lockmapHisTTDate.Lock();
	do 
	{
		int iItem = IsValidSymbol(stPushReportDate.m_oSSymobl.ssSymbolCode.c_str());
		//if (stPushReportDetailDate.m_oRealPublicData.m_uiNewQty == 0||stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice == 0)break;

		if(iItem <= 0)break;
		SymbolTradeInfo_ZYQ stSymbolTradeInfo_ZYQ;
		stSymbolTradeInfo_ZYQ.stSymbolInfo.cMarket = m_stQuoteLstDate.stTabInfo.uiCurSel;
		stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode = GetCurSelQuoteSubItem(iItem,1);
		if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stSymbolTradeInfo_ZYQ))break;

		UINT iUnit = stSymbolTradeInfo_ZYQ.stSymbolInfo.iAgreeunit;
		if (iUnit <= 0)
		{
			iUnit = 1;
		}
		float fPowPrice = pow(10,(double)stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		float fPowVol = pow(10,(double)2);
		double fYesPrice = atof(GetCurSelQuoteSubItem(iItem,10));

		////////////
		stcQuotePrice stQuotePrice;
		stQuotePrice.fPrice = stPushReportDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice;
		stQuotePrice.fYesClosePrice = fYesPrice*1.0;
		stQuotePrice.lastTick = stPushReportDate.m_oTDateTime.m_iUpdateDataTime;

		stQuotePrice.stBS[0].stB.fPrice = stPushReportDate.m_oQtyPriceV1.m_buyVol.m_uiPrice*1.0/fPowPrice;
		stQuotePrice.stBS[0].stB.fQty = stPushReportDate.m_oQtyPriceV1.m_buyVol.m_uiQty*1.0/fPowVol;
		stQuotePrice.stBS[0].stS.fPrice = stPushReportDate.m_oQtyPriceV1.m_sellVol.m_uiPrice*1.0/fPowPrice;
		stQuotePrice.stBS[0].stS.fQty = stPushReportDate.m_oQtyPriceV1.m_sellVol.m_uiQty*1.0/fPowVol;

		GetCurSelQuoteItem(iItem)->stQuotePrice = stQuotePrice;
		if (m_pSpotQuoteDate&&stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode.CompareNoCase(m_pSpotQuoteDate->GetSymbol().szCode) == 0)
		{
			m_pSpotQuoteDate->SetQuotePrice(stQuotePrice);
			m_pSpotQuoteDate->NotifyRecvWnd(0,0);
		}

		//1最新价2涨跌3均价4最高价5最低价6昨收价
		CString strTemp;
		int x = 0;//编号
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
		//strTemp.Format(_T("%d"),stQuoteItemLstDate.uiItemID);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = strTemp;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

		x++;//1商品代码
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = vecDate.m_oSSymobl.ssSymbolCode.c_str();
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

		x++;//2商品名称
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolName;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

		x++;//3开盘价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiOpenPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiOpenPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		SetQuoteItemText(iItem,x,strTemp,6);

		x++;//4最新价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiNewPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		SetQuoteItemText(iItem,x,strTemp,1);

		x++;//5现量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiNewQty*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewQty*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp);

		x++;//6涨跌
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_iPriceRiseDown,0)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iPriceRiseDown*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_iPriceRiseDown*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp,2);

		x++;//7幅度
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_iPriceRiseDownRate,0)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iPriceRiseDownRate*1.0/fPowVol,2) + _T("%");
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_iPriceRiseDownRate*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp,2);

		x++;//8最高价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiMaxPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiMaxPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		SetQuoteItemText(iItem,x,strTemp,4,fYesPrice);

		x++;//9最低价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiMinPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiMinPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		SetQuoteItemText(iItem,x,strTemp,5,fYesPrice);

		x++;//10昨收盘价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiYesClosePrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

		x++;//11买价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oQtyPriceV1.m_buyVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		SetQuoteItemText(iItem,x,strTemp,1);

		x++;//12买量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_buyVol.m_uiQty*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oQtyPriceV1.m_buyVol.m_uiQty*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp);

		x++;//13卖价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oQtyPriceV1.m_sellVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		SetQuoteItemText(iItem,x,strTemp,1);

		x++;//14卖量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_sellVol.m_uiQty*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oQtyPriceV1.m_sellVol.m_uiQty*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp);

		x++;//15成交量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiTotalTurnoverQty*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp);

		x++;//16成交金额
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iTotalTurnoverFunds*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice,2);
		SetQuoteItemText(iItem,x,strTemp);

		x++;//17均价
		if (stPushReportDate.m_oRealPublicData.m_uiTotalTurnoverQty > 0)
		{
			double dbJun = (stPushReportDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice)/(stPushReportDate.m_oRealPublicData.m_uiTotalTurnoverQty/fPowVol)/iUnit;
			//strTemp = CDateManage::DoubleToString(dbJun,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
			//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(fPowPrice*dbJun,uiYesPrice)];
			strTemp = CDateManage::DoubleToString(dbJun,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);

		}
		else
		{
			strTemp = (_T("--"));
		}
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = strTemp;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		SetQuoteItemText(iItem,x,strTemp,3,fYesPrice);

		x++;//18库存量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[2];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(stSymbolTradeInfo_ZYQ.stSymbolInfo.uiTotalShares*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		//strTemp = CDateManage::DoubleToString(stSymbolTradeInfo_ZYQ.stSymbolInfo.uiTotalShares*1.0/fPowVol,2);
		//SetQuoteItemText(iItem,x,strTemp,0);

		x++;//19量比
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iQtyRate*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiQtyRate*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp);

		x++;//20委比
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iOrderRate*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiOrderRate*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp);
	} while (0);
	m_lockmapHisTTDate.Unlock();

	RedrawWindow();
}
void CDlgTrendChart_D::TransPushReportDetailDate_ZYQ(STURealDataDetailPush_ZYQ_Client &stPushReportDetailDate)
{
	m_lockmapHisTTDate.Lock();
	do 
	{
		if (stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase(stPushReportDetailDate.m_oSSymobl.ssSymbolCode.c_str())!=0)break;
		if (stPushReportDetailDate.m_oRealPublicData.m_uiNewQty == 0||stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice == 0)break;

		//TURNOVER_DEIRC_B = 0,  // 买方成交
		//	TURNOVER_DEIRC_S = 1,  // 卖方成交
		//	TURNOVER_DEIRC_F = 2,  // 集合竞价或是其他成交
		//	TURNOVER_DEIRC_C = 3,  // 无成交
		BOOL bFlag = FALSE;
		if (stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice  == 0)break;
		if (stPushReportDetailDate.m_oRealPublicData.m_chTurnoverDirec == TURNOVER_DEIRC_F||
			stPushReportDetailDate.m_oRealPublicData.m_chTurnoverDirec == TURNOVER_DEIRC_C)
		{
			bFlag = TRUE;
		}

		UINT iUnit = stChartDate.stSymbolTradeInfo.stSymbolInfo.iAgreeunit;
		if (iUnit <= 0)
		{
			iUnit = 1;
		}
		float fPowPrice = pow(10,(double)stChartDate.stSymbolTradeInfo.stSymbolInfo.cPriceUnit);
		float fPowVol = pow(10,(double)2);

		stcQuotePrice stQuotePrice;
		//stChartDate.stTopPartDate.Clear();

		stChartDate.stTopPartDate.fWibi = stPushReportDetailDate.m_oRealPublicData.m_uiOrderRate*1.0/fPowVol;
		stChartDate.stTopPartDate.fWeicha = stPushReportDetailDate.m_oExt.m_iOrderDiff*1.0/fPowVol;
		stChartDate.stTopPartDate.iBSNum = stPushReportDetailDate.m_vQtyPriceV.size();
		if(stChartDate.stTopPartDate.iBSNum < 0 || stChartDate.stTopPartDate.iBSNum > 5)
			stChartDate.stTopPartDate.iBSNum = 0;
		for (int i = 0; i < stChartDate.stTopPartDate.iBSNum; i++)
		{
			stQuotePrice.stBS[i].stB.fPrice = stChartDate.stTopPartDate.sVolPrice[i].fbuyQuote.fPrice = stPushReportDetailDate.m_vQtyPriceV[i].m_buyVol.m_uiPrice*1.0/fPowPrice;
			stQuotePrice.stBS[i].stB.fQty = stChartDate.stTopPartDate.sVolPrice[i].fbuyQuote.fQty = stPushReportDetailDate.m_vQtyPriceV[i].m_buyVol.m_uiQty*1.0/fPowVol;
			stQuotePrice.stBS[i].stS.fPrice = stChartDate.stTopPartDate.sVolPrice[i].fsellQuote.fPrice = stPushReportDetailDate.m_vQtyPriceV[i].m_sellVol.m_uiPrice*1.0/fPowPrice;
			stQuotePrice.stBS[i].stS.fQty = stChartDate.stTopPartDate.sVolPrice[i].fsellQuote.fQty = stPushReportDetailDate.m_vQtyPriceV[i].m_sellVol.m_uiQty*1.0/fPowVol;
		}

		stChartDate.stTopPartDate.fNewPrice = stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice;
		if (stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty > 0)
		{
			stChartDate.stTopPartDate.fPinjunPrice = stPushReportDetailDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty/iUnit;

		}
		else
			stChartDate.stTopPartDate.fPinjunPrice = 0.00;
		stChartDate.stTopPartDate.fZhangDie = stPushReportDetailDate.m_oRealPublicData.m_iPriceRiseDown*1.0/fPowVol;;
		//stChartDate.stTopPartDate.fOpenPrice = stReportDataDetail.m_oReportPublicData.m_uiOpenPrice*1.0/fPowPrice;;

		stChartDate.stTopPartDate.fFudu = stPushReportDetailDate.m_oRealPublicData.m_iPriceRiseDownRate*1.0/fPowVol;
		stChartDate.stTopPartDate.fMaxPrice = getMaxDate(stChartDate.stTopPartDate.fMaxPrice,stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice);
		stChartDate.stTopPartDate.fNowVol = stPushReportDetailDate.m_oRealPublicData.m_uiNewQty*1.0/fPowVol;
		stChartDate.stTopPartDate.fMinPrice = getMinDate(stChartDate.stTopPartDate.fMinPrice,stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice);

		stChartDate.stTopPartDate.fTotalVol = stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/fPowVol;
		stChartDate.stTopPartDate.fVolRatio = stPushReportDetailDate.m_oRealPublicData.m_iPriceRiseDownRate*1.0/fPowVol;
		stChartDate.stTopPartDate.fDealMoney = stPushReportDetailDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice;

		if (stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares > 0)
		{
			stChartDate.stTopPartDate.fHuanshouRatio = stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares*100;

		}
		else
			stChartDate.stTopPartDate.fHuanshouRatio= 100;
		//stChartDate.stTopPartDate.fYesDPrice = stPushReportDetailDate.m_oRealPublicData.m_uiYesClosePrice*1.0/fPowPrice;
		//stChartDate.stTopPartDate.fYesSettlPrice = stReportDataDetail.m_oReportPublicData.m_uiYesSetllPrice*1.0/fPowPrice;
		stChartDate.stTopPartDate.fOutPan = stPushReportDetailDate.m_oExt.m_uiPOrderSaleQty*1.0/fPowVol;
		stChartDate.stTopPartDate.fInPan = stPushReportDetailDate.m_oExt.m_uiPOrderBuyQty*1.0/fPowVol;

		////////////
		if (m_pSpotQuoteDate&&stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase(m_pSpotQuoteDate->GetSymbol().szCode) == 0)
		{
			stQuotePrice.fPrice = stChartDate.stTopPartDate.fNewPrice;
			stQuotePrice.fYesClosePrice = stChartDate.stTopPartDate.fYesDPrice;
			stQuotePrice.lastTick = stPushReportDetailDate.m_oTDateTime.m_iUpdateDataTime;
			m_pSpotQuoteDate->SetQuotePrice(stQuotePrice);
			m_pSpotQuoteDate->NotifyRecvWnd(0,0);
		}
		////////
		if (bFlag)break;
		///////////////////////
		HisDateSpot stHisDateSpot;
		HisTTimeDate stHisTTimeDate;
		stHisTTimeDate.iTradeDay = stPushReportDetailDate.m_oTDateTime.m_iTradeDay;
		stHisTTimeDate.uiDateTime = stPushReportDetailDate.m_oTDateTime.m_iUpdateDataTime;
		stHisTTimeDate.fClosePrice = stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice;
		stHisTTimeDate.fTrunoverQty = stPushReportDetailDate.m_oRealPublicData.m_uiNewQty*1.0/fPowVol;    
		stHisTTimeDate.fOrderQty = stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares*1.0/fPowVol;
		//stHisTTimeDate.fYesSettlPrice = stChartDate.stTopPartDate.fYesSettlPrice;
		if (stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty > 0)
		{
			stHisTTimeDate.fAveragePrice = (stPushReportDetailDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice)/(stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty/fPowVol)/iUnit;
		}
		else
		{
			stHisTTimeDate.fAveragePrice = stHisTTimeDate.fClosePrice;
		}
    	stHisDateSpot.stHisTTimeDate = stHisTTimeDate;
		INT64 uiCurTm = (stPushReportDetailDate.m_oTDateTime.m_iUpdateDataTime/100)*100;
		//TmapHisDateSpot::iterator itMap = stChartDate.stTrendDate.mapHisTTDate.find(uiCurTm);
		//if (itMap != stChartDate.stTrendDate.mapHisTTDate.end())
		//{
		//	;
		//}
		stChartDate.stTrendDate.mapHisTTDate[uiCurTm] = stHisDateSpot;

		//////////////////
		HisUnitK_ZYQ stHisUnitK;
		float fBeforClosePrice = 0.0;
		TmapHisUnitK_ZYQ::reverse_iterator itMap;
		INT64 uiBeforTm = 0;
		itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.rbegin();
		if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.rend())
		{
			uiBeforTm = itMap->first;
			stHisUnitK = itMap->second;
		}

		if (uiCurTm > uiBeforTm && uiBeforTm > 0)
		{
			stHisUnitK.fBeforClosePrice = itMap->second.fClosePrice;


			stHisUnitK.uiDate = stPushReportDetailDate.m_oTDateTime.m_iUpdateDataTime;
			stHisUnitK.fClosePrice = stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice;
			stHisUnitK.fMaxPrice = stHisUnitK.fClosePrice;
			stHisUnitK.fMinPrice = stHisUnitK.fClosePrice;
			stHisUnitK.fJunPrice = stHisTTimeDate.fAveragePrice;
			stHisUnitK.fTrunoverQty = stPushReportDetailDate.m_oRealPublicData.m_uiNewQty*1.0/fPowVol;
			stHisUnitK.fTrunoverMoney = stHisUnitK.fTrunoverQty*stHisUnitK.fClosePrice*iUnit;

			stHisUnitK.fBeforTrunoverMoney = stPushReportDetailDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice;
			stHisUnitK.fBeforTrunoverQty = stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/fPowVol;

			if (stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares > 0)
			{
				stHisUnitK.fTurnoverRate = stHisUnitK.fTrunoverQty*1.0*100/stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares;
			}
			else
				stHisUnitK.fTurnoverRate = 100;
			stHisUnitK.fOpenPrice = stHisUnitK.fClosePrice;

		}
		else if(uiCurTm == uiBeforTm)
		{
			if (stHisUnitK.fBeforTrunoverMoney <= 0.00000000001)
			{
				stHisUnitK.fBeforTrunoverMoney = stPushReportDetailDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice;
			}
			if (stHisUnitK.fBeforTrunoverQty <= 0.00000000001)
			{
				stHisUnitK.fBeforTrunoverQty = stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/fPowVol;
			}

			stHisUnitK.uiDate = stPushReportDetailDate.m_oTDateTime.m_iUpdateDataTime;
			//stHisUnitK.fOpenPrice = stHisKUnit_ZYQ.m_vHisKUnit[i].m_uiOpenPrice*1.0/fPowPrice;
			stHisUnitK.fClosePrice = stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice;
			stHisUnitK.fMaxPrice = getMaxDate(stChartDate.stTopPartDate.fMaxPrice,stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice);
			stHisUnitK.fMinPrice = getMinDate(stChartDate.stTopPartDate.fMinPrice,stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice);
			stHisUnitK.fJunPrice = stHisTTimeDate.fAveragePrice;
			stHisUnitK.fTrunoverQty += (stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/fPowVol - stHisUnitK.fBeforTrunoverQty);
			stHisUnitK.fTrunoverMoney += (stPushReportDetailDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice - stHisUnitK.fBeforTrunoverMoney);

			stHisUnitK.fBeforTrunoverMoney = stPushReportDetailDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice;
			stHisUnitK.fBeforTrunoverQty = stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/fPowVol;

			if (stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares > 0)
			{
				stHisUnitK.fTurnoverRate = stHisUnitK.fTrunoverQty*1.0*100/stChartDate.stSymbolTradeInfo.stSymbolInfo.uiTotalShares;
			}
			else
				stHisUnitK.fTurnoverRate = 100;
		}

		//stHisUnitK.fTrunoverMoney = stPushReportDetailDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowVol;
		//stHisUnitK.fTrunoverQty = stPushReportDetailDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/fPowVol;
// 		//////////////
		INT64 iCurBg = 0;
		if (stChartDate.stKDate.mapHisUnitK_ZYQ.size() > stChartDate.stKDate.iDisplayNum)
		{
			TmapHisUnitK_ZYQ::reverse_iterator rItMap = stChartDate.stKDate.mapHisUnitK_ZYQ.rbegin();
			int iDisplayNum = stChartDate.stKDate.iDisplayNum;
			while((iDisplayNum--) > 1)
			{
				if (rItMap != stChartDate.stKDate.mapHisUnitK_ZYQ.rend())
				{
					rItMap++;
				}	
			}

			if (stChartDate.stKDate.uiCurBeginTm == 0 || stChartDate.stKDate.uiCurBeginTm >= rItMap->second.uiDate)
			{	
				iCurBg = rItMap->second.uiDate;
			}
		}
// 		/////////////////
		stChartDate.stKDate.mapHisUnitK_ZYQ[uiCurTm] = stHisUnitK;
		if (iCurBg > 0)
		{
			TmapHisUnitK_ZYQ::iterator itBg = stChartDate.stKDate.mapHisUnitK_ZYQ.find(iCurBg);
			itBg++;
			if (itBg != stChartDate.stKDate.mapHisUnitK_ZYQ.end())
			{
				stChartDate.stKDate.uiCurBeginTm = itBg->second.uiDate;
			}
		}

		CalcIndexDate(&(stChartDate.stKDate));
		/////////////////////
		DealDetailsDate stDealDetails;
		stDealDetails.iTradeDay = stPushReportDetailDate.m_oTDateTime.m_iTradeDay;
		stDealDetails.uiDateTime = stPushReportDetailDate.m_oTDateTime.m_iUpdateDataTime;//20150128141327;
		stDealDetails.fPrice = stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice;
		stDealDetails.fQty = stPushReportDetailDate.m_oRealPublicData.m_uiNewQty*1.0/fPowVol;

		stDealDetails.cBSDirec = stPushReportDetailDate.m_oRealPublicData.m_chTurnoverDirec;
		stChartDate.stDealDetailsInfo.vecDealDetails.push_back(stDealDetails);
	} while (0);
	m_lockmapHisTTDate.Unlock();
	GetMaxMinPrice(stChartDate.stTrendDate.fMaxPrice,stChartDate.stTrendDate.fMinPrice,stChartDate.stTrendDate.fYsPrice);
	GetMaxMinVol(stChartDate.stTrendDate.fMaxVol,stChartDate.stTrendDate.fMinVol,stChartDate.stTrendDate.fPPVol);

	RedrawWindow();
}
void CDlgTrendChart_D::TransOpenMarket_ZYQ(STUOpenMarket_ZYQ_Client &stOpenMarketDate)
{
	//if (m_iType != 1)return;
	m_lockmapHisTTDate.Lock();
	do 
	{
		int iItem = IsValidSymbol(stOpenMarketDate.m_oSSymobl.ssSymbolCode.c_str());
		//if (stPushReportDetailDate.m_oRealPublicData.m_uiNewQty == 0||stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice == 0)break;

		if(iItem <= 0)break;
		SymbolTradeInfo_ZYQ stSymbolTradeInfo_ZYQ;
		stSymbolTradeInfo_ZYQ.stSymbolInfo.cMarket = m_stQuoteLstDate.stTabInfo.uiCurSel;
		stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode = GetCurSelQuoteSubItem(iItem,1);
		if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stSymbolTradeInfo_ZYQ))break;

		UINT iUnit = stSymbolTradeInfo_ZYQ.stSymbolInfo.iAgreeunit;
		if (iUnit <= 0)
		{
			iUnit = 1;
		}
		float fPowPrice = pow(10,(double)stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		float fPowVol = pow(10,(double)2);
		double fYesPrice = atof(GetCurSelQuoteSubItem(iItem,10));

		//1最新价2涨跌3均价4最高价5最低价6昨收价
		CString strTemp;
		int x = 0;//编号
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
		//strTemp.Format(_T("%d"),stQuoteItemLstDate.uiItemID);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = strTemp;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

		x++;//1商品代码
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = vecDate.m_oSSymobl.ssSymbolCode.c_str();
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

		x++;//2商品名称
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolName;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

		x++;//3开盘价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiOpenPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiOpenPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//4最新价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiNewPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//5现量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiNewQty*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//6涨跌
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_iPriceRiseDown,0)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iPriceRiseDown*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//7幅度
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_iPriceRiseDownRate,0)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iPriceRiseDownRate*1.0/fPowVol,2) + _T("%");
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//8最高价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiMaxPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiMaxPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//9最低价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiMinPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiMinPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//10昨收盘价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiYesClosePrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = CDateManage::DoubleToString(stOpenMarketDate.m_oOpenInfo.m_uiYesClosePrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);;
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//11买价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//12买量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_buyVol.m_uiQty*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//13卖价
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice,uiYesPrice)];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//14卖量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_sellVol.m_uiQty*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//15成交量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiTotalTurnoverQty*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//16成交金额
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iTotalTurnoverFunds*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//17均价
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//18库存量
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[2];
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(stSymbolTradeInfo_ZYQ.stSymbolInfo.uiTotalShares*1.0/fPowVol,2);
		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
		//strTemp = CDateManage::DoubleToString(stSymbolTradeInfo_ZYQ.stSymbolInfo.uiTotalShares*1.0/fPowVol,2);
		strTemp = CDateManage::DoubleToString(stOpenMarketDate.m_oOpenInfo.m_iTotalIssueQty*1.0/fPowVol,2);
		SetQuoteItemText(iItem,x,strTemp);

		x++;//19量比
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);

		x++;//20委比
		strTemp = (_T("--"));
		SetQuoteItemText(iItem,x,strTemp,7);
	}while(0);
	m_lockmapHisTTDate.Unlock();

	RedrawWindow();
}
void CDlgTrendChart_D::TransCloseMarket_ZYQ(STUCloseMarket_ZYQ_Client &stCloseMarketDate)
{
	//m_lockmapHisTTDate.Lock();
	//do 
	//{
	//	int iItem = IsValidSymbol(stCloseMarketDate.m_oSSymobl.ssSymbolCode.c_str());
	//	//if (stPushReportDetailDate.m_oRealPublicData.m_uiNewQty == 0||stPushReportDetailDate.m_oRealPublicData.m_uiNewPrice == 0)break;

	//	if(iItem <= 0)break;
	//	SymbolTradeInfo_ZYQ stSymbolTradeInfo_ZYQ;
	//	stSymbolTradeInfo_ZYQ.stSymbolInfo.cMarket = m_stQuoteLstDate.stTabInfo.uiCurSel;
	//	stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode = GetCurSelQuoteSubItem(iItem,1);
	//	if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stSymbolTradeInfo_ZYQ))break;

	//	UINT iUnit = stSymbolTradeInfo_ZYQ.stSymbolInfo.iAgreeunit;
	//	if (iUnit <= 0)
	//	{
	//		iUnit = 1;
	//	}
	//	float fPowPrice = pow(10,(double)stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	float fPowVol = pow(10,(double)2);
	//	double fYesPrice = atof(GetCurSelQuoteSubItem(iItem,10));

	//	//1最新价2涨跌3均价4最高价5最低价6昨收价
	//	CString strTemp;
	//	int x = 0;//编号
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
	//	//strTemp.Format(_T("%d"),stQuoteItemLstDate.uiItemID);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = strTemp;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

	//	x++;//1商品代码
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = vecDate.m_oSSymobl.ssSymbolCode.c_str();
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

	//	x++;//2商品名称
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolName;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

	//	x++;//3开盘价
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiOpenPrice,uiYesPrice)];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiOpenPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stCloseMarketDate.m_oCloseInfo.m_uiOpenPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	SetQuoteItemText(iItem,x,strTemp,6);

	//	x++;//4最新价
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiNewPrice,uiYesPrice)];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	//strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	//SetQuoteItemText(iItem,x,strTemp,1);

	//	x++;//5现量
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiNewQty*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewQty*1.0/fPowVol,2);
	//	SetQuoteItemText(iItem,x,strTemp);

	//	x++;//6涨跌
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_iPriceRiseDown,0)];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iPriceRiseDown*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_iPriceRiseDown*1.0/fPowVol,2);
	//	SetQuoteItemText(iItem,x,strTemp,2);

	//	x++;//7幅度
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_iPriceRiseDownRate,0)];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iPriceRiseDownRate*1.0/fPowVol,2) + _T("%");
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_iPriceRiseDownRate*1.0/fPowVol,2);
	//	SetQuoteItemText(iItem,x,strTemp,2);

	//	x++;//8最高价
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiMaxPrice,uiYesPrice)];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiMaxPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	SetQuoteItemText(iItem,x,strTemp,4,fYesPrice);

	//	x++;//9最低价
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oReportPublicData.m_uiMinPrice,uiYesPrice)];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiMinPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiNewPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	SetQuoteItemText(iItem,x,strTemp,5,fYesPrice);

	//	x++;//10昨收盘价
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiYesClosePrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;

	//	x++;//11买价
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice,uiYesPrice)];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_buyVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oQtyPriceV1.m_buyVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	SetQuoteItemText(iItem,x,strTemp,1);

	//	x++;//12买量
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_buyVol.m_uiQty*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oQtyPriceV1.m_buyVol.m_uiQty*1.0/fPowVol,2);
	//	SetQuoteItemText(iItem,x,strTemp);

	//	x++;//13卖价
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice,uiYesPrice)];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_sellVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oQtyPriceV1.m_sellVol.m_uiPrice*1.0/fPowPrice,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//	SetQuoteItemText(iItem,x,strTemp,1);

	//	x++;//14卖量
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oQtyPriceV1.m_sellVol.m_uiQty*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oQtyPriceV1.m_sellVol.m_uiQty*1.0/fPowVol,2);
	//	SetQuoteItemText(iItem,x,strTemp);

	//	x++;//15成交量
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_uiTotalTurnoverQty*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiTotalTurnoverQty*1.0/fPowVol,2);
	//	SetQuoteItemText(iItem,x,strTemp);

	//	x++;//16成交金额
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iTotalTurnoverFunds*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice,2);
	//	SetQuoteItemText(iItem,x,strTemp);

	//	x++;//17均价
	//	if (stPushReportDate.m_oRealPublicData.m_uiTotalTurnoverQty > 0)
	//	{
	//		double dbJun = (stPushReportDate.m_oRealPublicData.m_iTotalTurnoverFunds*1.0/fPowPrice)/(stPushReportDate.m_oRealPublicData.m_uiTotalTurnoverQty/fPowVol)/iUnit;
	//		//strTemp = CDateManage::DoubleToString(dbJun,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);
	//		//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[GetUDColor(fPowPrice*dbJun,uiYesPrice)];
	//		strTemp = CDateManage::DoubleToString(dbJun,stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit);

	//	}
	//	else
	//	{
	//		strTemp = (_T("--"));
	//	}
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = strTemp;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	SetQuoteItemText(iItem,x,strTemp,3,fYesPrice);

	//	x++;//18库存量
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrUD[2];
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(stSymbolTradeInfo_ZYQ.stSymbolInfo.uiTotalShares*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	//strTemp = CDateManage::DoubleToString(stSymbolTradeInfo_ZYQ.stSymbolInfo.uiTotalShares*1.0/fPowVol,2);
	//	//SetQuoteItemText(iItem,x,strTemp,0);

	//	x++;//19量比
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrNormal;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iQtyRate*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiQtyRate*1.0/fPowVol,2);
	//	SetQuoteItemText(iItem,x,strTemp);

	//	x++;//20委比
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].clrSItemText = dwclrIndex;
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].strSItemText = CDateManage::DoubleToString(vecDate.m_oReportPublicData.m_iOrderRate*1.0/fPowVol,2);
	//	//stQuoteItemLstDate.mapQuoteLstSubItemDate[x].iSTextAlignmentType = 2;
	//	strTemp = CDateManage::DoubleToString(stPushReportDate.m_oRealPublicData.m_uiOrderRate*1.0/fPowVol,2);
	//	SetQuoteItemText(iItem,x,strTemp);
	//} while (0);
	//m_lockmapHisTTDate.Unlock();

	//RedrawWindow();
}
void CDlgTrendChart_D::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DWORD dwPos = GetMessagePos();
	CPoint point1( LOWORD(dwPos), HIWORD(dwPos) );
	///////////////////
	if (m_iType == 3&&CPublic::Instance()->GetDrawDll()->m_pSetGraphicProperty)
	{
		(*CPublic::Instance()->GetDrawDll()->m_pSetGraphicProperty)(this,point1);
	}

	if (m_iType == 1)return;
	CMenu menu;  
	CMenu popup;
	popup.CreatePopupMenu();//创建弹出菜单  
	menu.CreatePopupMenu();//创建弹出菜单  
	popup.AppendMenu(MF_STRING,ID_NORMALORDER, _T("现货下单"));//添加子菜单(弹出菜单MF_POPUP类型对应着子菜单的句柄) 

	CMenu SubMenu,SubPopMenu1,SubPopMenu2,SubPopMenu3,SubPopMenu4,SubPopMenu5,SubPopMenu6,
		SubPopMenu7,SubPopMenu8,SubPopMenu9,SubPopMenu10;
	if (m_iType == 3)
	{
		popup.InsertMenu(2,MF_BYPOSITION|MF_POPUP, (UINT)menu.m_hMenu,_T("分析周期"));//插入子菜单  
		int iSubId = ID_TECHANALY_1MINLINE;
		menu.AppendMenu(MF_STRING,iSubId++,_T("1分钟线")); 
		menu.AppendMenu(MF_STRING,iSubId++,_T("5分钟线")); 
		menu.AppendMenu(MF_STRING,iSubId++,_T("10分钟线")); 
		menu.AppendMenu(MF_STRING,iSubId++,_T("15分钟线")); 
		menu.AppendMenu(MF_STRING,iSubId++,_T("30分钟线")); 
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING,iSubId++,_T("1小时线")); 
		menu.AppendMenu(MF_STRING,iSubId++,_T("2小时线")); 
		menu.AppendMenu(MF_STRING,iSubId++,_T("4小时线")); 
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING,iSubId++,_T("日线")); 
		menu.AppendMenu(MF_STRING,iSubId++,_T("周线")); 
		menu.AppendMenu(MF_STRING,iSubId++,_T("月线")); 

		SubMenu.CreatePopupMenu();
		SubPopMenu1.CreatePopupMenu();
		SubPopMenu2.CreatePopupMenu();
		SubPopMenu3.CreatePopupMenu();
		SubPopMenu4.CreatePopupMenu();
		SubPopMenu5.CreatePopupMenu();
		SubPopMenu6.CreatePopupMenu();
		SubPopMenu7.CreatePopupMenu();
		SubPopMenu8.CreatePopupMenu();
		SubPopMenu9.CreatePopupMenu();
		SubPopMenu10.CreatePopupMenu();

		popup.InsertMenu(3,MF_BYPOSITION|MF_POPUP, (UINT)SubMenu.m_hMenu,_T("画线工具"));//插入子菜单  
		
		SubMenu.InsertMenu(1,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu1.m_hMenu,_T("简单线性"));//插入子菜单 
		SubPopMenu1.AppendMenu(MF_STRING,ID_DRAWLINE_BEELINE_Z,_T("创建直线")); 
		SubPopMenu1.AppendMenu(MF_STRING,ID_DRAWLINE_LIMITLINE_Z,_T("创建线段")); 
		SubPopMenu1.AppendMenu(MF_STRING,ID_DRAWLINE_RADIALLINE_Z,_T("创建射线")); 
		SubPopMenu1.AppendMenu(MF_STRING,ID_DRAWLINE_LEVELLINE_Z,_T("水平线")); 
		SubPopMenu1.AppendMenu(MF_STRING,ID_DRAWLINE_APLOMTLINE_Z,_T("垂直线")); 
		SubPopMenu1.AppendMenu(MF_STRING,ID_DRAWLINE_PARALLELLINE_Z,_T("平行线")); 

		SubMenu.InsertMenu(2,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu2.m_hMenu,_T("百分比线"));//插入子菜单 
		SubPopMenu2.AppendMenu(MF_STRING,ID_DRAWLINE_GOLDENSECTION_Z,_T("黄金分割线")); 
		SubPopMenu2.AppendMenu(MF_STRING,ID_DRAWLINE_PENCENT_Z,_T("百分比线")); 

		SubMenu.InsertMenu(3,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu3.m_hMenu,_T("周期线"));//插入子菜单 
		SubPopMenu3.AppendMenu(MF_STRING,ID_DRAWLINE_AVGCYCLE_Z,_T("平均周期线"));
		SubPopMenu3.AppendMenu(MF_STRING,ID_DRAWLINE_SYMMTRYLINE_Z,_T("对称线"));
		SubPopMenu3.AppendMenu(MF_STRING,ID_DRAWLINE_FABONACCI_Z,_T("费波纳契周期线"));
		SubPopMenu3.AppendMenu(MF_STRING,ID_DRAWLINE_TIMERULER_Z,_T("时间尺"));

		SubMenu.InsertMenu(4,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu4.m_hMenu,_T("江恩-速阻"));//插入子菜单 
		SubPopMenu4.AppendMenu(MF_STRING,ID_DRAWLINE_GENNANGLE_Z,_T("江恩角度"));
		SubPopMenu4.AppendMenu(MF_STRING,ID_DRAWLINE_GENNBOX_Z,_T("江恩箱"));
		SubPopMenu4.AppendMenu(MF_STRING,ID_DRAWLINE_SPEEDLINE_Z,_T("速阻线"));

		SubMenu.InsertMenu(5,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu5.m_hMenu,_T("安德鲁音叉线"));//插入子菜单 
		SubPopMenu5.AppendMenu(MF_STRING,ID_DRAWLINE_ANDREW_Z,_T("安德鲁音叉线"));
		SubPopMenu5.AppendMenu(MF_STRING,ID_DRAWLINE_GRIDLINE_Z,_T("栅格线"));

		SubMenu.InsertMenu(6,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu6.m_hMenu,_T("矩形"));//插入子菜单 
		SubPopMenu6.AppendMenu(MF_STRING,ID_DRAWLINE_RECT_Z,_T("矩形"));
		SubPopMenu6.AppendMenu(MF_STRING,ID_DRAWLINE_TRIANGLE_Z,_T("三角行"));
		SubPopMenu6.AppendMenu(MF_STRING,ID_DRAWLINE_BOX_Z,_T("箱体线"));

		//SubMenu.InsertMenu(7,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu7.m_hMenu,_T("回归线"));//插入子菜单 
		//SubPopMenu7.AppendMenu(MF_STRING,ID_DRAWLINE_REGRESSLINE_Z,_T("线性回归"));
		//SubPopMenu7.AppendMenu(MF_STRING,ID_DRAWLINE_REGRESSSTRAP_Z,_T("回归带"));
		//SubPopMenu7.AppendMenu(MF_STRING,ID_DRAWLINE_CHANNEL_Z,_T("回归通道"));

		SubMenu.InsertMenu(8,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu8.m_hMenu,_T("圆"));//插入子菜单 
		SubPopMenu8.AppendMenu(MF_STRING,ID_DRAWLINE_CIRCLE_Z,_T("圆"));
		SubPopMenu8.AppendMenu(MF_STRING,ID_DRAWLINE_ELLIPSE_Z,_T("椭圆"));
		SubPopMenu8.AppendMenu(MF_STRING,ID_DRAWLINE_CIRARC_Z,_T("对数螺旋圆"));
		SubPopMenu8.AppendMenu(MF_STRING,ID_DRAWLINE_SINE_Z,_T("正玄线"));

		SubMenu.InsertMenu(9,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu9.m_hMenu,_T("图标"));//插入子菜单 
		SubPopMenu9.AppendMenu(MF_STRING,ID_DRAWLINE_UPARROW_Z,_T("上箭头图标"));
		SubPopMenu9.AppendMenu(MF_STRING,ID_DRAWLINE_DOWNARROW_Z,_T("下箭头图标"));
		SubPopMenu9.AppendMenu(MF_STRING,ID_DRAWLINE_CHARS_Z,_T("文字图标"));
		SubPopMenu9.AppendMenu(MF_STRING,ID_DRAWLINE_TEXT_Z,_T("文本录入"));

		SubMenu.InsertMenu(10,MF_BYPOSITION|MF_POPUP, (UINT)SubPopMenu10.m_hMenu,_T("恢复操作"));//插入子菜单 
		SubPopMenu10.AppendMenu(MF_STRING,ID_DRAWLINE_UNDO_Z,_T("撤销"));
		SubPopMenu10.AppendMenu(MF_STRING,ID_DRAWLINE_REDO_Z,_T("恢复"));
	}
	//CMenu *pMn = popup.GetSubMenu(1);
	
	popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, GetParent() );
	DestroyMenu(menu.m_hMenu);
	DestroyMenu(popup.m_hMenu);
	if (m_iType == 3)
	{
		DestroyMenu(SubPopMenu1.m_hMenu);
		DestroyMenu(SubPopMenu2.m_hMenu);
		DestroyMenu(SubPopMenu3.m_hMenu);
		DestroyMenu(SubPopMenu4.m_hMenu);
		DestroyMenu(SubPopMenu5.m_hMenu);
		DestroyMenu(SubPopMenu6.m_hMenu);
		DestroyMenu(SubPopMenu7.m_hMenu);
		DestroyMenu(SubPopMenu8.m_hMenu);
		DestroyMenu(SubPopMenu9.m_hMenu);
		DestroyMenu(SubPopMenu10.m_hMenu);
	}
	//////////////////////////////
 	//CMenu menu;
 	//VERIFY( menu.LoadMenu( IDR_ZYQ ) );
 	//CMenu* popup = menu.GetSubMenu(0);
 	//ASSERT( popup != NULL );
 	//DWORD dwPos = GetMessagePos();
 	//CPoint point1( LOWORD(dwPos), HIWORD(dwPos) );
 	//popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, GetParent() );

	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CDlgTrendChart_D::InValid(float x)
{
	BOOL ret = memcmp(&x,INVALIDF,sizeof(float));
	return (ret==0);
}
//函数功能：计算MACD指标数据
void CDlgTrendChart_D::CalcIndexDate(KDate *pstKDate,CString strKey)
{
	//////////////////////////////////////////////////////////////////////////
	//计算数据  0x7fffffff作为无效数据
	SHistoryUnit *pKUnit = NULL;
	CString strTemp = _T("");
	do 
	{
		int		i,nc;
		int m_nCurves,m_nParamNum,iValueNum;
		//char cKey[20] = "MA";
		iValueNum = pstKDate->mapHisUnitK_ZYQ.size();
		if(iValueNum < 0)break;

		pKUnit = new SHistoryUnit[iValueNum];
		TmapHisUnitK_ZYQ::iterator itMap;
		int iIndex = 0;
		for (itMap = pstKDate->mapHisUnitK_ZYQ.begin(); itMap != pstKDate->mapHisUnitK_ZYQ.end();itMap++,iIndex++)
		{
			pKUnit[iIndex].m_uiDate = (itMap->second.uiDate%10000000000)/100;
			pKUnit[iIndex].m_fOpenPrice = itMap->second.fOpenPrice;
			pKUnit[iIndex].m_fMaxPrice = itMap->second.fMaxPrice;
			pKUnit[iIndex].m_fMinPrice = itMap->second.fMinPrice;
			pKUnit[iIndex].m_fClosePrice = itMap->second.fClosePrice;
			pKUnit[iIndex].m_uiVolume = itMap->second.fTrunoverQty;
			pKUnit[iIndex].m_uiSum = itMap->second.fTrunoverMoney;
			itMap->second.mapIndexDate.clear();
		}
		//TVecIndexKey::iterator
		int iKeyNum = pstKDate->vecIndexDisplay.size();
		if(iKeyNum != 3) return;
		for (int iKey = 0;iKey < iKeyNum;iKey++)
		{
			strKey = pstKDate->vecIndexDisplay[iKey];
			pstKDate->MapIndexDateHead[strKey].strTitle = strKey;
			if ( m_thCalc.Calc((short)F_ZB,strKey,pKUnit,iValueNum) )
			{
				//DestroyObj();//清除老数据

				//if (m_nCurves < m_thCalc.GetOutVarSize() ||
				//	m_nParamNum < m_thCalc.GetRegParSize())
				//{
				//	ResetParaBuffers(m_thCalc.GetRegParSize(), m_thCalc.GetOutVarSize());
				//}
				//else
				{
					pstKDate->MapIndexDateHead[strKey].uiCurves = m_thCalc.GetOutVarSize();
					pstKDate->MapIndexDateHead[strKey].uiParamNum = m_thCalc.GetRegParSize();

					if (pstKDate->MapIndexDateHead[strKey].uiCurves > 5)break;
				}

				//InitDataBuffer(m_nValueNum);//初始化数据区

				CString title;
				///////////////////////////////////////
				//计算比率，防止溢出
				int nDiv;
				nDiv=1;
				float fV = 0.00,fMax = 0.00;
				fMax  = 0;
				for ( nc =0;nc<pstKDate->MapIndexDateHead[strKey].uiCurves;++nc )
				{
					for (i = 0; i < iValueNum; i++)
					{
						if ( !InValid(m_thCalc[nc][i]) )
						{
							fV = m_thCalc[nc][i];
							if(fMax < fV)
								fMax = fV;
						}
					}
				}
				if(fMax >1000000)
					nDiv = 10000;
				////////////////////////////////////////
				for ( nc =0;nc<pstKDate->MapIndexDateHead[strKey].uiCurves;++nc )
				{
					itMap = pstKDate->mapHisUnitK_ZYQ.begin();
					for (i = 0; i < iValueNum && itMap != pstKDate->mapHisUnitK_ZYQ.end(); (i++,itMap++))
					{
						if ( InValid(m_thCalc[nc][i]) )
						{
							//m_ppCurveValue[nc][i] = 0x7fffffff;
							itMap->second.mapIndexDate[strKey].fEqualLine[nc] = 0x7fffffff;
						}
						else
						{
							//m_ppCurveValue[nc][i] = m_thCalc[nc][i];
							//m_ppCurveValue[nc][i] /= nDiv;
							fV = m_thCalc[nc][i];
							itMap->second.mapIndexDate[strKey].fEqualLine[nc] = m_thCalc[nc][i];
							itMap->second.mapIndexDate[strKey].fEqualLine[nc] /= nDiv;
						}
					}
					//m_pLineType[nc] = GetLineType(m_thCalc.GetOutVarProperty(nc));
					pstKDate->MapIndexDateHead[strKey].uiEqualLineType[nc] = GetLineType(m_thCalc.GetOutVarProperty(nc));
					//if ( nc>0 )		title += ",";
					//title += m_thCalc.GetOutVarWord(nc);
					//if (nc>0)stKDate.MapIndexDateHead[strKey].strEqlLineTitel += ",";
					//stKDate.MapIndexDateHead[strKey].strEqlLineTitel += m_thCalc.GetOutVarWord(nc);
					pstKDate->MapIndexDateHead[strKey].strEqlLineTitel[nc] = m_thCalc.GetOutVarWord(nc);
				}

				pstKDate->MapIndexDateHead[strKey].strTitle += _T("[");
				for ( i=0;i<m_thCalc.GetRegParSize();++i )
				{
					//m_pParamValue[i] = m_thCalc.GetRegParFloat(i);
					pstKDate->MapIndexDateHead[strKey].iParam[i] = m_thCalc.GetRegParFloat(i);
					if (i == m_thCalc.GetRegParSize()-1)
					{
						strTemp.Format(_T("%d"),pstKDate->MapIndexDateHead[strKey].iParam[i]);
					}
					else
					    strTemp.Format(_T("%d,"),pstKDate->MapIndexDateHead[strKey].iParam[i]);
					pstKDate->MapIndexDateHead[strKey].strTitle += strTemp;
				}
				pstKDate->MapIndexDateHead[strKey].strTitle += _T("]");
				//title.MakeUpper();
				////UpdateIndexParamInfo(title);
			}
		}
	} while (0);
	if (pKUnit)
	{
		delete  []pKUnit;
		pKUnit = NULL;
	}
}
UINT CDlgTrendChart_D::GetLineType(int nGSType)
{
	switch(nGSType)
	{
	case VOLSTICK:
		return 4;
	case CIRCLEDOT:	
		return 2;
	case COLORSTICK:
		return 1;
	default:
		return 0;
	}
}
LRESULT CDlgTrendChart_D::OnNotifySendByTrade(WPARAM wPar, LPARAM lPar)
{
	char *pstr = (char *)lPar;

	SymbolTradeInfo_ZYQ stSymbolTradeInfo_ZYQ;
	stSymbolTradeInfo_ZYQ.stSymbolInfo.cMarket = 0;
	stSymbolTradeInfo_ZYQ.stSymbolInfo.strSymbolCode = pstr;
	if (CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stSymbolTradeInfo_ZYQ))
	{
		m_pSpotQuoteDate->ClearSpotQuoteDate();
		SCodeInfo1 stCode;
		strcpy(stCode.szCode,pstr);
		stCode.cPriceUnit = stSymbolTradeInfo_ZYQ.stSymbolInfo.cPriceUnit;
		m_pSpotQuoteDate->SetSymbol(stCode);

		if (m_iType == 1)
		{
			int iItem = IsValidSymbol(stCode.szCode);
			if(iItem > 0)
			{
				m_pSpotQuoteDate->SetQuotePrice(GetCurSelQuoteItem(iItem)->stQuotePrice);
				m_pSpotQuoteDate->NotifyRecvWnd(0,0);
			}
		}
		else if(m_iType == 3 || m_iType == 2)
		{
			if (stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase(stCode.szCode) == 0)
			{
				stcQuotePrice stQuotePrice;
				stQuotePrice.fPrice = stChartDate.stTopPartDate.fNewPrice;
				stQuotePrice.fYesClosePrice = stChartDate.stTopPartDate.fYesDPrice;
				stQuotePrice.lastTick;
				for (int i = 0; i < 5; i++)
				{
					stQuotePrice.stBS[i].stB.fPrice = stChartDate.stTopPartDate.sVolPrice[i].fbuyQuote.fPrice;
					stQuotePrice.stBS[i].stB.fQty = stChartDate.stTopPartDate.sVolPrice[i].fbuyQuote.fQty;
					stQuotePrice.stBS[i].stS.fPrice = stChartDate.stTopPartDate.sVolPrice[i].fsellQuote.fPrice;
					stQuotePrice.stBS[i].stS.fQty = stChartDate.stTopPartDate.sVolPrice[i].fsellQuote.fPrice;
				}
				m_pSpotQuoteDate->SetQuotePrice(stQuotePrice);
				m_pSpotQuoteDate->NotifyRecvWnd(0,0);
			}
		}
	}
	//delete pstr;
	return 1;
}
LRESULT CDlgTrendChart_D::OnKeyGhostMsg(WPARAM wPar, LPARAM lPar)
{
	switch(wPar)
	{
	case KEYGHOST_STOCK:
		{
			do 
			{
				if (stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode.CompareNoCase((char*)lPar) == 0)break;

				CString strSymbolCode;
				strSymbolCode = (char*)lPar;
				if (strSymbolCode.IsEmpty()) break;//return m_bTips;

				stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket = 0;
				stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode = strSymbolCode;
				if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stChartDate.stSymbolTradeInfo))
				{
					stChartDate.stSymbolTradeInfo.Clear();
					//return m_bTips;
					break;
				}

				stChartDate.stTopPartDate.Clear();
				stChartDate.stDealDetailsInfo.Clear();
				stChartDate.stTrendDate.Clear();
				stChartDate.stKDate.Clear();

				STUSimpleSymbol_ZYQ stSimpleSymbol_ZYQ;
				stSimpleSymbol_ZYQ.m_usMarketType = m_stQuoteLstDate.stTabInfo.uiCurSel;
				//memcpy(stSimpleSymbol_ZYQ.m_szSymbolCode)
				strcpy(stSimpleSymbol_ZYQ.m_szSymbolCode,strSymbolCode.GetBuffer(0));
				CDateManage_ZYQ::Instance()->ReqReportDateDetail_ZYQ(&stSimpleSymbol_ZYQ);

				m_bTips = FALSE;
				m_iType = 2;
				break;
				/////////////////

			} while (0);
		}
		break;
	case KEYGHOST_FORMULA:
		{
			do 
			{
				if (m_iType != 3) break;
				MainIndex stMainIndex = HitTestMianIndex((char *)lPar,&(stChartDate.stKDate.vecMainIndex));
				/*if (stMainIndex.uiIndexID > 0)*/stChartDate.stKDate.uiCurSelIndex = stMainIndex.uiIndexID;
				stChartDate.stKDate.vecIndexDisplay[2] = (char *)lPar;
				CalcIndexDate(&(stChartDate.stKDate));
				RedrawWindow();
				break;
			} while (0);	
		}
		break;
	default:
		break;
	}
	return 0;
}

void CDlgTrendChart_D::OnUpdateUserCmd(CCmdUI *pCmdUI)
{
	switch(pCmdUI->m_nID)
	{
	case ID_TECHANALY_1MINLINE://1分钟线
	case ID_TECHANALY_5MINLINE: //5分钟线
	case ID_TECHANALY_10MINLINE: //10分钟线
	case ID_TECHANALY_15MINLINE: //15分钟线
	case ID_TECHANALY_30MINLINE: //30分钟线
	case ID_TECHANALY_60MINLINE: //60分钟线
	case ID_TECHANALY_DAYLINE: //日线
	case ID_TECHANALY_WEEKLINE: //周线
	case ID_TECHANALY_MONLINE: //月线

		{
			pCmdUI->Enable(FALSE);
		}
		break;	
	default:
		{
			if((pCmdUI->m_nID >=ID_DRAWLINE_BEGINTAG_Z)&&(pCmdUI->m_nID <=ID_DRAWLINE_ENDTAG_Z))
			{
				pCmdUI->Enable(FALSE);
			}
		}
		break;
	}
}

void CDlgTrendChart_D::OnComRange(UINT Iid)
{
	switch(Iid)
	{
	case ID_TECHANALY_1MINLINE://1分钟线
	case ID_TECHANALY_5MINLINE: //5分钟线
	case ID_TECHANALY_10MINLINE: //10分钟线
	case ID_TECHANALY_15MINLINE: //15分钟线
	case ID_TECHANALY_30MINLINE: //30分钟线
	case ID_TECHANALY_60MINLINE: //60分钟线
	case ID_TECHANALY_DAYLINE: //日线
	case ID_TECHANALY_WEEKLINE: //周线
	case ID_TECHANALY_MONLINE: //月线
		{
			do 
			{
				if (m_iType != 3)break;
				int nCircle[] = {MINUTE1_HISK,MINUTES5_HISK,MINUTES10_HISK,MINUTES15_HISK,
					MINUTES30_HISK,MINUTES60_HISK,MINUTES62_HISK,MINUTES64_HISK,DAY_HISK,WEEK_HISK,MONTH_HISK};
				if (stChartDate.stKDate.stKDateHead.uiCycle == nCircle[Iid-ID_TECHANALY_1MINLINE])break;

				stChartDate.stKDate.Clear();
				ReqKDate_ZYQ(nCircle[Iid-ID_TECHANALY_1MINLINE]);
			} while (0);
		}
		break;	
	default:
		{
			if((Iid >=ID_DRAWLINE_BEGINTAG_Z)&&(Iid <=ID_DRAWLINE_ENDTAG_Z))
			{
				if(Iid == ID_DRAWLINE_REDO_Z)
				{
					if(CPublic::Instance()->GetDrawDll()->m_pNextStep)
						(*CPublic::Instance()->GetDrawDll()->m_pNextStep)();
				}
				else if(Iid == ID_DRAWLINE_UNDO_Z)
				{
					if(CPublic::Instance()->GetDrawDll()->m_pPreStep)
						(*CPublic::Instance()->GetDrawDll()->m_pPreStep)();
				}
				else
				{
					if(CPublic::Instance()->GetDrawDll()->m_pCreateGraph)
						(*CPublic::Instance()->GetDrawDll()->m_pCreateGraph)(Iid -ID_DRAWLINE_BEGINTAG_Z);
				}
			}
		}
		break;
	}
}

void CDlgTrendChart_D::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);

	//QuickKeyResponse(nRepCnt,nFlags);
}
BOOL CDlgTrendChart_D::QuickKey_ChangeWnd(UINT nChar)
{
	BOOL bRet = FALSE;
	if (nChar == VK_F5)
	{
		//ChangeDisplayMode();
		m_lockmapHisTTDate.Lock();
		if (m_iType == 3)
		{
			m_bTips = FALSE;
			m_iType = 2;
		}
		else if (m_iType == 2)
		{
			m_bTips = FALSE;
			m_iType = 3;
		}
		else if(m_iType == 1||m_iType == 4)
		{
			do 
			{
				CString strSymbolCode;
				strSymbolCode = GetCurSelQuoteSubItem(m_stQuoteLstDate.stQuoteItemLst.uiCurSel,1);
				if (strSymbolCode.IsEmpty()) break;//return m_bTips;

				stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket = m_stQuoteLstDate.stTabInfo.uiCurSel;
				stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode = strSymbolCode;
				if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stChartDate.stSymbolTradeInfo))
				{
					stChartDate.stSymbolTradeInfo.Clear();
					//return m_bTips;
					break;
				}

				stChartDate.stTopPartDate.Clear();
				stChartDate.stDealDetailsInfo.Clear();
				stChartDate.stTrendDate.Clear();
				stChartDate.stKDate.Clear();

				STUSimpleSymbol_ZYQ stSimpleSymbol_ZYQ;
				stSimpleSymbol_ZYQ.m_usMarketType = m_stQuoteLstDate.stTabInfo.uiCurSel;
				//memcpy(stSimpleSymbol_ZYQ.m_szSymbolCode)
				strcpy(stSimpleSymbol_ZYQ.m_szSymbolCode,strSymbolCode.GetBuffer(0));
				CDateManage_ZYQ::Instance()->ReqReportDateDetail_ZYQ(&stSimpleSymbol_ZYQ);

				m_bTips = FALSE;
				m_iType = 2;

				USES_CONVERSION;
				CPublic::Instance()->GetDrawDll()->m_pSetCurSymbol(T2W(stSimpleSymbol_ZYQ.m_szSymbolCode,strSymbolCode));
			} while (0);
		}
		m_lockmapHisTTDate.Unlock();
		RedrawWindow();

		bRet = TRUE;
	}
	else if (nChar == VK_F2)
	{
		m_lockmapHisTTDate.Lock();
		if (m_iType != 1)
		{
			m_bTips = FALSE;
			m_iType = 1;

			STUReportDataIndex_ZYQ stReportDataIndex_ZYQ;
			stReportDataIndex_ZYQ.m_usMarketType = m_stQuoteLstDate.stTabInfo.uiCurSel;  // 市场类型
			stReportDataIndex_ZYQ.m_ucOrderBy = m_stQuoteLstDate.stHeadLstDate.uiCurSort;                 // 排序类型
			stReportDataIndex_ZYQ.m_ucSeqType = m_stQuoteLstDate.stHeadLstDate.bSort;                 // 排序(正序,逆序)        
			stReportDataIndex_ZYQ.m_usBeginPos = m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin;                // 开始位置 ///1开始
			stReportDataIndex_ZYQ.m_usReqNum = 50;                  // 请求个数
			stReportDataIndex_ZYQ.m_usReqDesSymoblNum = 0;         // 指定请求个数; 如果是0,那么表示请求当前市场类型下面所有商品
			CDateManage_ZYQ::Instance()->ReqReportDate_ZYQ(&stReportDataIndex_ZYQ);
		}
		m_lockmapHisTTDate.Unlock();
		RedrawWindow();

		bRet = TRUE;
	}
	else if (nChar == VK_F10)
	{
		CString strSymbolCode = _T("");
		m_lockmapHisTTDate.Lock();
		if (m_iType == 1)
		{
			pQuoteItemLstDate pQID =  GetCurSelQuoteItem(m_stQuoteLstDate.stQuoteItemLst.uiCurSel);
			if (pQID)
			{
				strSymbolCode = GetCurSelQuoteSubItem(pQID,1);
			}
		}
		else if (m_iType == 3 || m_iType == 2)
		{
			strSymbolCode = stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode;
		}
	    m_bTips = FALSE;
		m_iType = 4;
		m_lockmapHisTTDate.Unlock();
		CString strURLM;
		strURLM = CPublic::Instance()->GetURL(nChar);
		if (!strURLM.IsEmpty()&&!strSymbolCode.IsEmpty())
		{
			CString strURL;
			strURL.Format(strURLM,strSymbolCode);
			m_dlgHtmlShow.Navigate(strURL);
		}
		m_dlgHtmlShow.ShowWindow(SW_SHOW);

		bRet = TRUE;
	}
	else if (nChar == VK_F8)
	{
		CString strSymbolCode = _T("");
		if (m_iType == 1)
		{
			pQuoteItemLstDate pQID =  GetCurSelQuoteItem(m_stQuoteLstDate.stQuoteItemLst.uiCurSel);
			if (pQID)
			{
				strSymbolCode = GetCurSelQuoteSubItem(pQID,1);
			}
		}
		else if (m_iType == 3 || m_iType == 2)
		{
			strSymbolCode = stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode;
		}
		CString strURLM;
		strURLM = CPublic::Instance()->GetURL(nChar);
		if (!strURLM.IsEmpty()&&!strSymbolCode.IsEmpty())
		{
			CString strURL;
			strURL.Format(strURLM,strSymbolCode);
			TCHAR *pchar = new TCHAR[strURL.GetLength()*sizeof(TCHAR) + 2];
			memcpy(pchar,strURL.GetBuffer(0),strURL.GetLength()*sizeof(TCHAR)+1);
			GetParent()->PostMessage(WM_SHOWURL,0,(LPARAM)pchar);
		}

		bRet = TRUE;
	}
	//else
	//{
	//	CString strTmp;
	//	strTmp.Format(_T("KeyDown:%d"),nChar);
	//	AfxMessageBox(strTmp);
	//}
	return bRet;
}
BOOL CDlgTrendChart_D::QuickKey_Filter(UINT nChar)
{
	if (nChar == VK_ESCAPE)
	{
		if (m_iType == 1) return TRUE;
		m_lockmapHisTTDate.Lock();
		if (m_iType == 3)
		{
			m_bTips = FALSE;
			m_iType = 2;
		}
		else if (m_iType == 2 || m_iType == 4)
		{
			m_bTips = FALSE;
			m_iType = 1;

			STUReportDataIndex_ZYQ stReportDataIndex_ZYQ;
			stReportDataIndex_ZYQ.m_usMarketType = m_stQuoteLstDate.stTabInfo.uiCurSel;  // 市场类型
			stReportDataIndex_ZYQ.m_ucOrderBy = m_stQuoteLstDate.stHeadLstDate.uiCurSort;                 // 排序类型
			stReportDataIndex_ZYQ.m_ucSeqType = m_stQuoteLstDate.stHeadLstDate.bSort;                 // 排序(正序,逆序)        
			stReportDataIndex_ZYQ.m_usBeginPos = m_stQuoteLstDate.stQuoteItemLst.uiItemIDBegin;                // 开始位置 ///1开始
			stReportDataIndex_ZYQ.m_usReqNum = 50;                  // 请求个数
			stReportDataIndex_ZYQ.m_usReqDesSymoblNum = 0;         // 指定请求个数; 如果是0,那么表示请求当前市场类型下面所有商品
			CDateManage_ZYQ::Instance()->ReqReportDate_ZYQ(&stReportDataIndex_ZYQ);
		}
		m_lockmapHisTTDate.Unlock();
		RedrawWindow();

		return TRUE;
	}
	if (nChar == VK_RETURN)
	{
		return TRUE;
	}
	if (m_iType == 3)
	{
		if(CPublic::Instance()->GetDrawDll()->m_pKeyDown)
			if((*CPublic::Instance()->GetDrawDll()->m_pKeyDown)(nChar))RedrawWindow();
	}
	
	return FALSE;
}

BOOL CDlgTrendChart_D::QuickKey_TrendChart(UINT nChar)
{
	BOOL bRet = TRUE;
	switch(nChar)
	{
	case VK_DOWN:
		{//缩小数据
			if (m_iType != 3)return FALSE;
			m_lockmapHisTTDate.Lock();
			if (stChartDate.stKDate.fKEveryWithAll > 3)
			{
				stChartDate.stKDate.fKEveryWithAll -= 2;
			}
			m_lockmapHisTTDate.Unlock();
			RedrawWindow();
		}
		break;
	case VK_UP:
		{//放大数据
			if (m_iType != 3)return FALSE;
			m_lockmapHisTTDate.Lock();
			if (stChartDate.stKDate.fKEveryWithAll < 30)
			{
				stChartDate.stKDate.fKEveryWithAll += 2;
			}
			m_lockmapHisTTDate.Unlock();
			RedrawWindow();
		}
		break;
	case VK_LEFT:
		{
			if (m_iType != 3)return FALSE;
			//左移光标
			//////////////////////////////
			INT64 iBeginTm = GetFrontTime(stChartDate.stKDate.uiCurBeginTm);
			stChartDate.stKDate.uiCurBeginTm = iBeginTm;
			TmapHisUnitK_ZYQ::iterator itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin();
			if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end() && iBeginTm == itMap->second.uiDate)
			{
				ReqKDate_ZYQ(stChartDate.stKDate.stKDateHead.uiCycle,stChartDate.stKDate.mapHisUnitK_ZYQ.size(),200);
			}
			RedrawWindow();
		}
		break;
	case VK_RIGHT:
		{
			if (m_iType != 3)return FALSE;
			//右移光标
			INT64 iBeginTm = GetNextTime(stChartDate.stKDate.uiCurBeginTm);
			stChartDate.stKDate.uiCurBeginTm = iBeginTm;
			TmapHisUnitK_ZYQ::iterator itMap = stChartDate.stKDate.mapHisUnitK_ZYQ.begin();
			if (itMap != stChartDate.stKDate.mapHisUnitK_ZYQ.end() && iBeginTm == itMap->second.uiDate)
			{
				ReqKDate_ZYQ(stChartDate.stKDate.stKDateHead.uiCycle,stChartDate.stKDate.mapHisUnitK_ZYQ.size(),200);
			}
			RedrawWindow();
		}
		break;
	case VK_NEXT:
		{
			ChangeSymbol();
		}
		break;
	case VK_PRIOR:
		{
			ChangeSymbol(1);
		}
		break;
	default:
		{
			bRet = FALSE;
		}
		break;
	}

	return bRet;
}
BOOL CDlgTrendChart_D::QuickKeyResponse(WPARAM wPar,LPARAM lPar)
{
	BOOL bRet = TRUE;
	do 
	{
		if (QuickKey_Filter(wPar))break;
		if (QuickKey_ChangeWnd(wPar))break;
		if (QuickKey_TrendChart(wPar))break;
		bRet = FALSE;
	} while (0);
	if (m_iType != 4)
	{
		m_dlgHtmlShow.Navigate(_T("about:blank"), NULL, NULL);
		m_dlgHtmlShow.ShowWindow(SW_HIDE);
	}

	return bRet;
}
pQuoteItemLstDate CDlgTrendChart_D::GetFrontQuoteItem()
{
	int iItem = IsValidSymbol(stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode);
	int iNum = m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.size();
	pQuoteItemLstDate pQLD = NULL;
	do 
	{
		if (iItem <= 0 || iNum <= 0 || iItem == 1) break;
		if (iItem > 1)iItem--;

		m_stQuoteLstDate.stQuoteItemLst.uiCurSel = iItem;
		pQLD = GetCurSelQuoteItem(iItem);
	} while (0);

	return pQLD;
}
pQuoteItemLstDate CDlgTrendChart_D::GetNextQuoteItem()
{
	int iItem = IsValidSymbol(stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode);
	int iNum = m_stQuoteLstDate.stQuoteItemLst.mapQuoteItemLstDate.size();
	pQuoteItemLstDate pQLD = NULL;
	do 
	{
		if (iItem <= 0 || iNum <= 0 || iItem == iNum) break;
		if (iItem < iNum)iItem++;

		m_stQuoteLstDate.stQuoteItemLst.uiCurSel = iItem;
		pQLD = GetCurSelQuoteItem(iItem);
	} while (0);

	return pQLD;
}
void CDlgTrendChart_D::ChangeSymbol(int iDir)
{
	pQuoteItemLstDate pQLD = NULL;
	if (iDir == 0) pQLD = GetNextQuoteItem();
	else pQLD = GetFrontQuoteItem();
	if (!pQLD)return;
	/////////////
	BOOL bResh = FALSE;
	m_lockmapHisTTDate.Lock();
	do 
	{
		CString strSymbolCode;
		strSymbolCode = GetCurSelQuoteSubItem(pQLD,1);
		if (strSymbolCode.IsEmpty()) break;

		stChartDate.stSymbolTradeInfo.stSymbolInfo.cMarket = m_stQuoteLstDate.stTabInfo.uiCurSel;
		stChartDate.stSymbolTradeInfo.stSymbolInfo.strSymbolCode = strSymbolCode;
		if (!CDateManage_ZYQ::Instance()->GetSymbolTradeInfo_ZYQ(stChartDate.stSymbolTradeInfo))break;
		//{
		//	stChartDate.stSymbolTradeInfo.Clear();
		//	//return m_bTips;
		//	break;
		//}
		//if (m_iType == 3)
		//{
		//	bResh = TRUE;
		//	m_bTips = FALSE;
		//	ReqKDate_ZYQ();
		//	break;
		//}
		stChartDate.stTopPartDate.Clear();
		stChartDate.stDealDetailsInfo.Clear();
		stChartDate.stTrendDate.Clear();
		stChartDate.stKDate.Clear();

		STUSimpleSymbol_ZYQ stSimpleSymbol_ZYQ;
		stSimpleSymbol_ZYQ.m_usMarketType = m_stQuoteLstDate.stTabInfo.uiCurSel;
		//memcpy(stSimpleSymbol_ZYQ.m_szSymbolCode)
		strcpy(stSimpleSymbol_ZYQ.m_szSymbolCode,strSymbolCode.GetBuffer(0));
		CDateManage_ZYQ::Instance()->ReqReportDateDetail_ZYQ(&stSimpleSymbol_ZYQ);

		m_bTips = FALSE;
		//m_iType = 2;
		bResh = TRUE;

		USES_CONVERSION;
		CPublic::Instance()->GetDrawDll()->m_pSetCurSymbol(T2W(stSimpleSymbol_ZYQ.m_szSymbolCode,strSymbolCode));
	} while (0);
	m_lockmapHisTTDate.Unlock();
	//if (bResh)RedrawWindow();
}

void CDlgTrendChart_D::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ReArrayWnd();
}

LRESULT CDlgTrendChart_D::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect,rcBottomHide,rcTopHide;  
	GetWindowRect(&rect);  

	if (point.y >= rect.bottom-8)  
		return HTBOTTOM;  
	else  
	{  
		return CWnd::OnNcHitTest(point);  
	}  
	return 0;  

	//return CDialog::OnNcHitTest(point);
}


void CDlgTrendChart_D::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHitTest == HTBOTTOM)  
	{
		//SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, 0);  
		//SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));  
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);  
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
		::SendMessage(GetParent()->GetSafeHwnd(),WM_WNDSIZECHANGE,0,0);
		Invalidate();
	}
	else
		CDialog::OnNcLButtonDown(nHitTest, point);
}
