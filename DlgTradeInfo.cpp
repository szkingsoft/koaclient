// DlgTradeInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgTradeInfo.h"


// CDlgTradeInfo 对话框

IMPLEMENT_DYNAMIC(CDlgTradeInfo, CDialog)

CDlgTradeInfo::CDlgTradeInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTradeInfo::IDD, pParent)
{
	m_iIndex = -1;
	m_pQuote = NULL;
}

CDlgTradeInfo::~CDlgTradeInfo()
{
	if (m_pQuote)
	{
		delete []m_pQuote;
		m_pQuote = NULL;
	}
}

void CDlgTradeInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTradeInfo, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgTradeInfo 消息处理程序


void CDlgTradeInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcCLt;
	GetClientRect(&rcCLt);
	SetTradeWndPos(rcCLt);
}


BOOL CDlgTradeInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTradeInfo::InitCtrl()
{
	InitTradeWnd();
	if (m_iIndex == 0)
	{
		CPublic::Instance()->RegistWnd(8,this);
	}
	else if (m_iIndex == 1)
	{
		CPublic::Instance()->RegistWnd(9,this,2);
	}
}

void CDlgTradeInfo::InitTradeWnd()
{
	if (IsWindow( CPublic::Instance()->GetTradeDll()->m_pInitWnd(GetSafeHwnd(),m_iIndex,CPublic::Instance()->GetClientType())))
	{
		CRect clientrect;
		GetClientRect(clientrect);
		SetTradeWndPos(clientrect);
	}
	else
	{
		CString strTemp;
		strTemp.Format(_T("Index:%d Type:%d"),m_iIndex,CPublic::Instance()->GetClientType());
		AfxMessageBox(strTemp);
	}
}

void CDlgTradeInfo::SetTradeWndPos(CRect &rcWnd)
{
	CPublic::Instance()->GetTradeDll()->m_pMoveWnd(rcWnd,m_iIndex);
}
void CDlgTradeInfo::AnalyMetalsQuete(WPARAM wParam,LPARAM lParm)
{
	CString strTem;
	DWORD iTmBg = GetTickCount();
	int iRet = 1;
	SNormaHead *pDataHead = (SNormaHead*)wParam;
	if(pDataHead->m_usType == PUSH_REALDATA)
	{ 
		iRet = PUSH_REALDATA;
		StuRecvQuoteInfo* pRecvQuote1= (StuRecvQuoteInfo*)(pDataHead+1);

		int uiDataLen = sizeof(StuRecvQuoteInfo);		
		for(int i=0;i<pDataHead->m_usSize;i++)
		{
			StuRecvQuoteInfo* pRecvQuote = (StuRecvQuoteInfo*)(pRecvQuote1+i);

			//unsigned int uiDataLen = lParam -sizeof(SNormaHead);

			CPublic::Instance()->GetTradeDll()->m_pUpdateQuote((const char*)pRecvQuote,uiDataLen,m_iIndex);//提交给交易端
		}
	}
	if(pDataHead->m_usType == REPORTDATA)
	{
		iRet = REPORTDATA;
		//////
		mapQSA mapGt;
		mapQSA::iterator mapIt;
		GetQuoteSymbolAttrib(mapGt);
		int x = 0;
		/////////
		SNowData* pNow = (SNowData*)(pDataHead+1);
		StuRecvQuoteInfo* pQuote = new StuRecvQuoteInfo[pDataHead->m_usSize];
		for(int i = 0;i<pDataHead->m_usSize;i++)
		{
			mapIt = mapGt.find(CString(pNow[i].sCode.szCode));
			if (mapIt != mapGt.end())
			{
				if (mapIt->second.strMarketStat != '3')
				{
					memcpy(&pQuote[x].sCode,&pNow[i].sCode,sizeof(SCodeInfo));
					pQuote[x].lastTick = pNow[i].timeUpdate;
					pQuote[x].uiPrice = pNow[i].uiNewPrice;
					x++;
				}
			}
			else
			{
				memcpy(&pQuote[x].sCode,&pNow[i].sCode,sizeof(SCodeInfo));
				pQuote[x].lastTick = pNow[i].timeUpdate;
				pQuote[x].uiPrice = pNow[i].uiNewPrice;
				x++;
			}
		}
		if(x > 0)
		{
			unsigned int uiDataLen = x/*pDataHead->m_usSize*/*sizeof(StuRecvQuoteInfo);
			CPublic::Instance()->GetTradeDll()->m_pUpdateQuote((const char*)pQuote,uiDataLen,m_iIndex);//提交给交易端	
			if(pQuote)delete []pQuote;
		}
	}
	//DWORD iTmEd = GetTickCount();
	//strTem.Format(_T("ID:%d 消息类型:%x 豪秒数:%d \r\n"),m_iIndex,iRet,(iTmEd-iTmBg));
	//TRACE(strTem);
}
void CDlgTradeInfo::GetQuoteSymbolAttrib(mapQSA &mapOut)
{
	QueteSymbolAttribute stcQSA[500];
	int iMun = CPublic::Instance()->GetTradeDll()->m_pGetQuoteSymbolAttrib(stcQSA);
	for (int i = 0; i < iMun; i++)
	{
		mapOut[stcQSA[i].strCode] = stcQSA[i];
	}
}
void CDlgTradeInfo::AnalySpotQuete(WPARAM wPar,LPARAM lParm)
{
	CString strTem;
	DWORD iTmBg = GetTickCount();
	int iRet = 1;
	if (wPar == REPORTDATA_ZYQ)
	{
		iRet = PUSH_REALDATA;
		////////////
		QUOTEINFO_M stcQuote;
		CDateManage::Instance()->GetRealQuoteInfo(stcQuote);
		if (stcQuote.iNum > 0)
		{
			CPublic::Instance()->GetTradeDll()->m_pUpdateQuote((const char*)stcQuote.pstcQuote,stcQuote.iNum,m_iIndex);//提交给交易端	
		}
	}
	if (wPar == PUSH_REALDATA_ZYQ)
	{
    	iRet = REPORTDATA;
		////////////
		QUOTEINFO_M stcQuote;
		CDateManage::Instance()->GetRealQuoteInfo(stcQuote);
		if (stcQuote.iNum > 0)
		{
			CPublic::Instance()->GetTradeDll()->m_pUpdateQuote((const char*)stcQuote.pstcQuote,stcQuote.iNum,m_iIndex);//提交给交易端	
		}
	}
	//DWORD iTmEd = GetTickCount();
	//strTem.Format(_T("ID:%d 消息类型:%x 豪秒数:%d \r\n"),m_iIndex,iRet,(iTmEd-iTmBg));
	//TRACE(strTem);
}
LRESULT CDlgTradeInfo::OnDataMessage(WPARAM wParam,LPARAM lParm)
{
	int iRet = 1;
	if (m_iIndex == 0)
	{
		AnalyMetalsQuete(wParam,lParm);
	}
	else if (m_iIndex == 1)
	{
		AnalySpotQuete(wParam,lParm);
	}
	return iRet;
}

BOOL CDlgTradeInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgTradeInfo::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox(_T("22221111"));
	CDialog::OnOK();
}


BOOL CDlgTradeInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClt;
	GetClientRect(&rcClt);
	pDC->FillSolidRect(rcClt.left,rcClt.top,rcClt.Width(),rcClt.Height(),RGB(31,34,41));
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


void CDlgTradeInfo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}
