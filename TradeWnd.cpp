
#include "stdafx.h"

#include "TradeWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

CTradeWnd::CTradeWnd()
{
}

CTradeWnd::~CTradeWnd()
{
}

BEGIN_MESSAGE_MAP(CTradeWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, &CTradeWnd::OnChangeActiveTab)
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(WM_TRADEDLGINFO,OnClosTradeDlg)
	ON_MESSAGE(WM_ASKSTOCKINFO,OnStockInfoMsg)
	ON_MESSAGE(WM_ASKSTOCKNAME,OnStockNameMsg)
	ON_WM_CLOSE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

int CTradeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadDllInfo();
	//3
	//CPublic::Instance()->RegistWnd(VIEWINFO_TRADE,this);
	return 0;
}

void CTradeWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	CRect clientrect(0,0,cx,cy);
	//CPublic::Instance()->GetTradeDll()->m_pMoveDlg(clientrect);
}

LRESULT CTradeWnd::OnDataMessage(WPARAM wParam,LPARAM lParam)
{
	SNormaHead *pDataHead = (SNormaHead*)wParam;
	if(pDataHead->m_usType == PUSH_REALDATA)
	{
		StuRecvQuoteInfo* pRecvQuote1= (StuRecvQuoteInfo*)(pDataHead+1);

		//int nRecord = 0;
		//int nHqstruLen = 0;
		//nHqstruLen = sizeof(StuRecvQuoteInfo);
		//nRecord = (lParam-sizeof(SNormaHead))/nHqstruLen;
		int uiDataLen = sizeof(StuRecvQuoteInfo);		
		for(int i=0;i<pDataHead->m_usSize;i++)
		{
			StuRecvQuoteInfo* pRecvQuote = (StuRecvQuoteInfo*)(pRecvQuote1+i);

			//unsigned int uiDataLen = lParam -sizeof(SNormaHead);

			CPublic::Instance()->GetTradeDll()->m_pUpdateQuote((const char*)pRecvQuote,uiDataLen,0);//提交给交易端
		}
	}
	if(pDataHead->m_usType == REPORTDATA)
	{
		SNowData* pNow = (SNowData*)(pDataHead+1);
		StuRecvQuoteInfo* pQuote = new StuRecvQuoteInfo[pDataHead->m_usSize];
		for(int i = 0;i<pDataHead->m_usSize;i++)
		{
			memcpy(&pQuote[i].sCode,&pNow[i].sCode,sizeof(SCodeInfo));
			pQuote[i].lastTick = pNow[i].timeUpdate;
			pQuote[i].uiPrice = pNow[i].uiNewPrice;
		}
		unsigned int uiDataLen = pDataHead->m_usSize*sizeof(StuRecvQuoteInfo);
		CPublic::Instance()->GetTradeDll()->m_pUpdateQuote((const char*)pQuote,uiDataLen,0);//提交给交易端	
		if(pQuote)delete []pQuote;
	}
	return 1;
}

void CTradeWnd::OnClose()
{
	CPublic::Instance()->GetTradeDll()->m_pDestoryDlg();
	CPublic::Instance()->UnRegistWnd(VIEWINFO_TRADE,this);
	CDockablePane::OnClose();
}

LRESULT CTradeWnd::OnClosTradeDlg(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->PostMessage(WM_TRADEDLGINFO);
	return 0;
}
LRESULT CTradeWnd::OnStockNameMsg(WPARAM wParam,LPARAM lParam)
{
	return 1;
}

LRESULT CTradeWnd::OnStockInfoMsg(WPARAM wParam,LPARAM lParam)
{
	return 0;
}

void CTradeWnd::OnUserCmd(UINT uiCmd)
{
	if(!CPublic::Instance()->GetTradeDll()->m_pMenuCmd(uiCmd,0))
	{
		MessageBox(CLanguageInterface::Instance()->GetStr(20067),CLanguageInterface::Instance()->GetStr(20068),MB_OK|MB_ICONWARNING);
	}
}

void CTradeWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}

void CTradeWnd::LoadDllInfo()
{
	//m_hClientDlg = CPublic::Instance()->GetTradeDll()->m_pInitDlog(GetSafeHwnd());//对话框
	CRect clientrect;
	GetClientRect(clientrect);
	//CPublic::Instance()->GetTradeDll()->m_pMoveDlg(clientrect);
}