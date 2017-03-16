// DlgPriceInfoWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgPriceInfoWnd.h"
#include "PriceInfoObj.h"

// CDlgPriceInfoWnd 对话框

IMPLEMENT_DYNAMIC(CDlgPriceInfoWnd, CDialog)

CDlgPriceInfoWnd::CDlgPriceInfoWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPriceInfoWnd::IDD, pParent)
{
	m_mapQuoteCard.clear();
	m_iLastHight = 0;
	m_vecInfoItem.clear();
}

CDlgPriceInfoWnd::~CDlgPriceInfoWnd()
{
	ClearMap();
}

void CDlgPriceInfoWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriceInfoWnd, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_CLICKRC,OnClickRc)
	ON_MESSAGE(WM_DbCLICKRC,OnDbClickRc)
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
END_MESSAGE_MAP()


// CDlgPriceInfoWnd 消息处理程序


BOOL CDlgPriceInfoWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgPriceInfoWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ArrayQuoteCard();
}

void CDlgPriceInfoWnd::InitCtrl()
{
	return;
	CGraphBtn* pBtn = AddQuoteCard();
	pBtn->MoveWindow(10,10,300,120);
	RECTANGLEINFO stcFundDate[9];
	CString strTem;
	int i = 0;
	strTem.Format(_T("黄金1KG"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("买:"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("卖:"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("4998"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("5588"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("最高价:"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("8888"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("最低价:"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("3888"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;

	pBtn->SetQuoteCardDate(stcFundDate,9);
	//m_btnPriceInfo.MoveWindow(10,10,300,120);
	//RECTANGLEINFO stcFundDate[9];
	//CString strTem;
	//int i = 0;
	//strTem.Format(_T("黄金1KG"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;
	//i++;
	//strTem.Format(_T("买:"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;
	//i++;
	//strTem.Format(_T("卖:"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;
	//i++;
	//strTem.Format(_T("4998"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;
	//i++;
	//strTem.Format(_T("5588"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;
	//i++;
	//strTem.Format(_T("最高价:"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;
	//i++;
	//strTem.Format(_T("8888"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;
	//i++;
	//strTem.Format(_T("最低价:"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;
	//i++;
	//strTem.Format(_T("3888"),i+1);
	//stcFundDate[i].strText = strTem;
	//stcFundDate[i].iRectId = i+1;

	//m_btnPriceInfo.SetQuoteCardDate(stcFundDate,9);
}

BOOL CDlgPriceInfoWnd::UpdateData(char* pData,int nDataLen)
{
	SNormaHead *pDataHead = (SNormaHead*)pData;
	int nCount;

	vector<CBaseInfoItem>::iterator it;
	SNowData* pNowData;
	const StuSymbolData* pCodeInfo;
	if(pDataHead->m_usType == REPORTDATA)
	{	
		SNowData* pNow = (SNowData*)(pDataHead+1);
		int nNum;
		nNum = 0;
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			nNum++;
		}
		//Init(nNum);
		m_vecInfoItem.resize(nNum);

		int nPostion = 0;
		vector<int> vecSort;
		vecSort.resize(nNum);
		for(int i=0;i < vecSort.size();i++)
			vecSort[i] = -1;

		float fBuy,fSell;
		int nTradeProductNum = 0;//可交易产品数量
		//此处统计可交易产品位置
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '1')
			{
				nPostion = 0;
				CPublic::Instance()->GetTradeDll()->m_pGetCustomUnit(pNow[nCount].sCode.szCode,fBuy,fSell,nPostion);
				if(nPostion == 0)
				{
					//vecSort[nTradeProductNum]= nTradeProductNum;
					CPublic::Instance()->SetStockTradeStatus(&pNow[nCount].sCode);
				}
				else
				{
					vecSort[nTradeProductNum] = nPostion-1;

					nTradeProductNum++;
				}
			}
		}
		nTradeProductNum = 0;
		//此处统计非交易产品位置
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '0')
			{
				int i = 0;

				for(i = 0;i < vecSort.size(); i++)
				{
					if(vecSort[i] == nTradeProductNum)
					{
						nTradeProductNum++;
						i = -1;
					}
				}

				for(i = 0;i < vecSort.size(); i++)
				{
					if (vecSort[i] == -1)
					{
						vecSort[i] = nTradeProductNum;
						break;
					}
				}
				nTradeProductNum++;
			}
		}
		CDateManage::Instance()->ReArrayVector(vecSort);
		m_vecInfoItem.resize(nNum);	
		int nStation =0;
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '1')
			{
				nPostion = vecSort[nStation];
				m_vecInfoItem[nPostion].SetStockInfo(&pNow[nCount].sCode,pCodeInfo->szName);
				m_vecInfoItem[nPostion].SetQuote(pNow+nCount);	

				nStation++;
			}
		}
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '0')
			{
				nPostion = vecSort[nStation];

				m_vecInfoItem[nPostion].SetStockInfo(&pNow[nCount].sCode,pCodeInfo->szName);
				m_vecInfoItem[nPostion].SetQuote(pNow+nCount);	

				nStation++;
			}
		}
		//初始化表格
		//RecalSubObjSize();
		UpdateQuoteCardLst();
		ArrayQuoteCard();
	}
	else if(pDataHead->m_usType == PUSH_REALDATA)
	{
		StuRecvQuoteInfo* pRecvQuote= (StuRecvQuoteInfo*)(pDataHead+1);
		//查找行列数
		SNowData nowData;
		int iCount;
		for(iCount =0;iCount<pDataHead->m_usSize;iCount++)
		{
			//int nRecord = 0;
			//int nHqstruLen = 0;
			//nHqstruLen = sizeof(StuRecvQuoteInfo);
			//nRecord = (nDataSize-sizeof(SNormaHead))/nHqstruLen;
			//for(int i=0;i<nRecord;i++)
			//{
			StuRecvQuoteInfo* pQuote = (StuRecvQuoteInfo*)(pRecvQuote+iCount);

			for(it = m_vecInfoItem.begin();it != m_vecInfoItem.end();it++)
			{		
				pCodeInfo = CPublic::Instance()->GetStockInfo(it->GetStockInfo());
				if(NULL == pCodeInfo)continue;
				if( 0 == strcmp(pCodeInfo->sCode.szCode, pQuote->sCode.szCode))
				{
					pNowData = it->GetNowData();
					memcpy(&nowData,pNowData,sizeof(SNowData));

					nowData.uiNewPrice = pQuote->uiPrice;
					if(nowData.uiMaxPrice <nowData.uiNewPrice)
					{
						nowData.uiMaxPrice = nowData.uiNewPrice;
					}
					if(nowData.uiMinPrice >nowData.uiNewPrice)
					{
						nowData.uiMinPrice = nowData.uiNewPrice;
					}
					it->SetQuote(&nowData);
					UpdateQuoteCardLstDate(nowData,FALSE,pCodeInfo->szName);
					break;
				}
			}		
		}
	}

	return TRUE;
}

CGraphBtn* CDlgPriceInfoWnd::AddQuoteCard()
{
	CGraphBtn * pBtn = new CGraphBtn;
	static int iId = 3222;
	iId++;
	pBtn->Create(NULL,BS_NOTIFY|BS_OWNERDRAW,CRect(0,0,300,120),this,iId);
	pBtn->ShowWindow(SW_SHOW);
	return pBtn;
}

void CDlgPriceInfoWnd::ClearMap()
{
	std::map<CString,CGraphBtn*>::iterator itMap;
	for (itMap = m_mapQuoteCard.begin();itMap != m_mapQuoteCard.end();itMap++)
	{
		if (itMap->second != NULL&&IsWindow(itMap->second->GetSafeHwnd()))
		{
			itMap->second->DestroyWindow();
			delete itMap->second;
			itMap->second = NULL;
		}
	}
	m_mapQuoteCard.clear();
}

void CDlgPriceInfoWnd::UpdateQuoteCardLst()
{
	vector<CBaseInfoItem>::iterator vIt;
	int iNum = m_vecInfoItem.size();
	for (vIt = m_vecInfoItem.begin();vIt != m_vecInfoItem.end();vIt++)
	{
		SNowData* pstcNow = vIt->GetNowData();
		UpdateQuoteCardLstDate(*pstcNow,TRUE,vIt->GetSymbolName());
	}
}

void CDlgPriceInfoWnd::ArrayQuoteCard()
{
	std::map<CString,CGraphBtn*>::iterator itMap;
	int iOffH = 10;
	CRect rcClt;
	GetClientRect(&rcClt);
	int iVcNum = rcClt.Width()/305;
	if (iVcNum <= 0) iVcNum = 1;
	int i = 0;
	int iNUM = m_mapQuoteCard.size();
	//for (itMap = m_mapQuoteCard.begin();itMap != m_mapQuoteCard.end();itMap++)
	//{
	//	CGraphBtn* pBtn = itMap->second;
	//	pBtn->MoveWindow(10+(310)*(i%iVcNum),iOffH,300,120);
	//	if (i%iVcNum == (iVcNum-1))
	//	{
	//		iOffH+=130;
	//	}	
	//	//pBtn->Invalidate();
	//	i++;
	//}
	/////////////
	vector<CBaseInfoItem>::iterator vIt;
	int iNum = m_vecInfoItem.size();
	for (vIt = m_vecInfoItem.begin();vIt != m_vecInfoItem.end();vIt++)
	{
		SNowData* pstcNow = vIt->GetNowData();
		itMap = m_mapQuoteCard.find(pstcNow->sCode.szCode);
		if (itMap != m_mapQuoteCard.end())
		{
			CGraphBtn* pBtn = itMap->second;
			pBtn->MoveWindow(10+(310)*(i%iVcNum),iOffH,300,120);
			if (i%iVcNum == (iVcNum-1))
			{
				iOffH+=130;
			}	
			//pBtn->Invalidate();
			i++;
		}
	}
	/////////////////
	m_iLastHight = iOffH + (((i%iVcNum)==0/*&&iVcNum >1*/)?0/*-120*/:130);
	//if (iVcNum == 1)m_iLastHight += 120;

	//if (m_iLastHight < rcClt.Height())m_iLastHight = rcClt.Height();
	//CRect rcWnd;
	//GetWindowRect(&rcWnd);
	//GetParent()->ScreenToClient(&rcWnd);
	//MoveWindow(rcWnd.left,rcWnd.top,rcWnd.Width(),m_iLastHight);
	::SendMessage(GetParent()->GetSafeHwnd(),WM_UPDATECTRL,m_iLastHight,0);
}

void CDlgPriceInfoWnd::UpdateQuoteCardLstDate(const SNowData stcNowData,BOOL bAdd,CString strName)
{
	std::map<CString,CGraphBtn*>::iterator itMap;
	CString strCode = stcNowData.sCode.szCode;
	itMap = m_mapQuoteCard.find(strCode);
	CGraphBtn* pBtn = NULL;
	if (!bAdd)
	{
		if (itMap == m_mapQuoteCard.end())return;
		pBtn = itMap->second;
	}
	else
	{
		if (itMap == m_mapQuoteCard.end())
			pBtn = AddQuoteCard();
		else
			pBtn = itMap->second;
	}
	

	float fPow;
	fPow = pow(10,(double)stcNowData.sCode.cPriceUnit);

	RECTANGLEINFO stcFundDate[9];
	CString strTem;
	int i = 0;
	strTem.Format(_T("%s-"),stcNowData.sCode.szCode);
	strTem += strName;
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	m_mapQuoteCard[stcNowData.sCode.szCode] = pBtn;
	i++;
	strTem.Format(_T("卖:"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("买:"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	/////
	float fBuyPrice,fSellPrice;
	float fBuyUnit,fSellUnit;
	fBuyUnit = fSellUnit = 1;
	int nSort;
	CPublic::Instance()->GetCustomUnit(stcNowData.sCode.szCode,fBuyUnit,fSellUnit,nSort);
	fBuyPrice = (float)stcNowData.uiNewPrice/fPow+ fBuyUnit;
	fSellPrice = (float)stcNowData.uiNewPrice/fPow + fSellUnit;
	strTem = CPublic::Instance()->GetFormateValue(fSellPrice/*fBuyPrice*/,stcNowData.sCode.cPriceUnit);
	//////
	//strTem = CPublic::Instance()->GetFormateValue(stcNowData.uiNewPrice/fPow,stcNowData.sCode.cPriceUnit);
	DWORD crText;
	CString strDsPrice = pBtn->GetCurDislayPrice();
	CSkinAttribute_Fund *pSkin = (CSkinAttribute_Fund *)CDateManage::Instance()->GetSkinAttribute(4);
	if (strDsPrice.IsEmpty())
	{
		crText = pSkin->clrTextMark;//Color::MakeARGB(255,188,188,188);
	}
	else
	{
		if(strTem.CompareNoCase(strDsPrice) > 0)
			crText = pSkin->clrTextUp;//Color::MakeARGB(255,255,0,0);
		else if(strTem.CompareNoCase(strDsPrice) < 0)
			crText = pSkin->clrTextDown;//Color::MakeARGB(255,0,255,0);
		else
			crText = pSkin->clrTextMark;//Color::MakeARGB(255,188,188,188);
	}
	
	stcFundDate[i].clrText = crText;
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	//strTem.Format(_T("%d"),stcNowData.uiNewPrice);
	//strTem = CPublic::Instance()->GetFormateValue(stcNowData.uiNewPrice/fPow,stcNowData.sCode.cPriceUnit);
	strTem = CPublic::Instance()->GetFormateValue(fBuyPrice/*fSellPrice*/,stcNowData.sCode.cPriceUnit); 
	stcFundDate[i].clrText = crText;
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("最高价:"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	//strTem.Format(_T("%d"),stcNowData.uiMaxPrice);
	strTem = CPublic::Instance()->GetFormateValue(stcNowData.uiMaxPrice/fPow,stcNowData.sCode.cPriceUnit);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	strTem.Format(_T("最低价:"),i+1);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;
	i++;
	//strTem.Format(_T("%d"),stcNowData.uiMinPrice);
	strTem = CPublic::Instance()->GetFormateValue(stcNowData.uiMinPrice/fPow,stcNowData.sCode.cPriceUnit);
	stcFundDate[i].strText = strTem;
	stcFundDate[i].iRectId = i+1;

	pBtn->SetQuoteCardDate(stcFundDate,9);
	pBtn->Invalidate();
}

BOOL CDlgPriceInfoWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClt;
	GetClientRect(&rcClt);
	//rcClt.InflateRect(-2,-2,-2,-2);
	
	CDateManage::Instance()->DrawBkRect(pDC,rcClt);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CDlgPriceInfoWnd::OnDbClickRc(WPARAM wPar,LPARAM lPar)
{
	int iRet = 1;
	int iSubItem = wPar;
	if (iSubItem == 2||iSubItem == 3)
	{
		SCodeInfo* pCurSelCode = GetCurSelCodeInfo((CGraphBtn *)lPar);
		if (pCurSelCode == NULL)return iRet;

		CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
		const StuSymbolData* pSymbolInfo = CPublic::Instance()->GetStockInfo(pCurSelCode);

		if(pSymbolInfo->cTrade == SYMBOLTYPE_TRADEPRODUCT&&CPublic::Instance()->GetTradeDll()->m_pGetUserStatus)
		{	
			/*nBuySellSign =(nBuySellSign==1)?1:0;*/
			CPublic::Instance()->CallQuickTrade(pCurSelCode,(iSubItem-2));
		}
	}

	return iRet;
}

LRESULT CDlgPriceInfoWnd::OnClickRc(WPARAM wPar,LPARAM lPar)
{
	return 1;
	int iRet = 1;
	int iSubItem = wPar;
	if (iSubItem == 2||iSubItem == 3)
	{
		//CString strTem;
		//strTem.Format(_T("区域:%d"),iSubItem);
		//AfxMessageBox(strTem);

		SCodeInfo* pCurSelCode = GetCurSelCodeInfo((CGraphBtn *)lPar);
		if (pCurSelCode == NULL)return iRet;

		CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
		const StuSymbolData* pSymbolInfo = CPublic::Instance()->GetStockInfo(pCurSelCode);

		if(pSymbolInfo->cTrade == SYMBOLTYPE_TRADEPRODUCT&&CPublic::Instance()->GetTradeDll()->m_pGetUserStatus)
		{	
			/*nBuySellSign =(nBuySellSign==1)?1:0;*/
			CPublic::Instance()->CallQuickTrade(pCurSelCode,(iSubItem-2));
		}
	}

	return iRet;
}

SCodeInfo*  CDlgPriceInfoWnd::GetCurSelCodeInfo(CGraphBtn *pBtn)
{
	SCodeInfo* pCurSelCode = NULL;
	if (pBtn == NULL || !IsWindow(pBtn->GetSafeHwnd()))return pCurSelCode;

	std::map<CString,CGraphBtn*>::iterator itMap;
	for (itMap = m_mapQuoteCard.begin(); itMap != m_mapQuoteCard.end();itMap++)
	{
		if (pBtn == itMap->second)
		{
			break;
		}
	}
	if (itMap == m_mapQuoteCard.end())return pCurSelCode;

	CString strCode = itMap->first;
	CString strTem;
	std::vector<CBaseInfoItem>::iterator itVec;
	for (itVec = m_vecInfoItem.begin();itVec != m_vecInfoItem.end();itVec++)
	{
		SNowData* pstcNow = itVec->GetNowData();	
		strTem.Format(_T("%s"),pstcNow->sCode.szCode);
		if (strTem.CompareNoCase(strCode) == 0)
		{
			pCurSelCode = itVec->GetStockInfo();
			break;
		}
	}

	return pCurSelCode;
}

BOOL CDlgPriceInfoWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CDlgPriceInfoWnd::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	std::map<CString,CGraphBtn*>::iterator mapIt;
	for (mapIt = m_mapQuoteCard.begin();mapIt != m_mapQuoteCard.end(); mapIt++)
	{
		if (IsWindow(mapIt->second->GetSafeHwnd()))
		{
			mapIt->second->Invalidate();
		}
	}
	return 1;
}
