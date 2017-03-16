#include "StdAfx.h"
#include "PriceInfoObj.h"
#include "resource.h"
#include "IniFile.h"

#define TABLE_HIGHKEEPSIDE 5
#define TABLE_WIDTHKEEPSIDE 2

CPriceInfoObj::CPriceInfoObj(void)
{
	m_nDisplayRow = 0;
	m_nBeginLine = 0;
	m_nTotalLine = 0;
	m_nCurrentSel = 0;
	m_ImageList.Create(IDB_ARROWBITMAP,15,15,RGB(255,255,255));
}


CPriceInfoObj::~CPriceInfoObj(void)
{
	m_ImageList.DeleteImageList();
}

void CPriceInfoObj::CalculateItemRect()
{
	m_iLastY = m_TotalRect.Height();
	if(m_vecInfoItem.size() <=0)return;

	int nWidth,nHeight;
	CBaseInfoItem::GetItemWidthHeight(nWidth,nHeight);

	int nTotalWidth,nTotalHeight;
	nTotalWidth = m_TotalRect.Width();
	int nCol,nRow;
	nCol = nTotalWidth/(nWidth+TABLE_WIDTHKEEPSIDE);//行

	int iItmeCount = m_vecInfoItem.size();
	m_nTotalLine = iItmeCount/nCol;
	if(0 != m_vecInfoItem.size()%nCol)m_nTotalLine +=1;//当前总行数
	nRow = m_nTotalLine;

	int nHeightSide,nWidthSide;
	nWidthSide = (nTotalWidth - nCol*((nWidth+TABLE_WIDTHKEEPSIDE))+TABLE_WIDTHKEEPSIDE)/2;
	nHeightSide = 20;

	int iItem = 0;
	CRect singleRect(0,0,0,0);
	int iLastY;
	for( int i = 0;i<nRow;i++)
	{
		for(int j= 0;j<nCol;j++)
		{
			if(iItem>=iItmeCount)break;
			singleRect.left = m_TotalRect.left +j*(nWidth+TABLE_WIDTHKEEPSIDE)+nWidthSide;
			singleRect.right = singleRect.left+nWidth;
			singleRect.top = m_TotalRect.top+ i*(nHeight+TABLE_HIGHKEEPSIDE)+nHeightSide;
			singleRect.bottom = singleRect.top+nHeight;
			iLastY = singleRect.bottom;

			m_vecInfoItem[iItem].MoveTo(singleRect);
			iItem++;
		}
	}
	
	m_iLastY = iLastY + nHeightSide;
	if (m_nTotalLine > 1 && IsWindow(m_hNotifyWnd))
	{
		::SendMessage(m_hNotifyWnd,WM_UPDATECTRL,m_iLastY,0);
	}
	//MoveTo(CRect(m_TotalRect.left,m_TotalRect.top,m_TotalRect.right,m_iLastY));
}
//////////////////////////////////////////////////////////
//计算子区域大小
void CPriceInfoObj::RecalSubObjSize(int iRightW)
{
	CalculateItemRect();
	return;
	if(m_vecInfoItem.size() ==0)return;//无数据
	int nWidth,nHeight;
	CBaseInfoItem::GetItemWidthHeight(nWidth,nHeight);
	int nTotalWidth,nTotalHeight;
	nTotalHeight = m_TotalRect.Height();
	nTotalWidth = m_TotalRect.Width();
	if(nTotalWidth <nWidth||nTotalHeight < nHeight)return;//空间太小，无法放置
	int nCol,nRow;
	nCol = nTotalWidth/(nWidth+TABLE_WIDTHKEEPSIDE);
	nRow = nTotalHeight/(nHeight+TABLE_HIGHKEEPSIDE);
	if(nCol==0||nRow==0)return;

	m_nDisplayRow = nRow;//当前可显示行数
	m_nDisplayCol = nCol;//可显示列数
	m_nTotalLine = m_vecInfoItem.size()/nCol;
	if(0 != m_vecInfoItem.size()%nCol)m_nTotalLine +=1;//当前总行数

	int nItemCount = m_nBeginLine*nCol;
	CRect singleRect;
	int i,j;

	int nHeightSide,nWidthSide;
	nWidthSide = (m_TotalRect.Width()-nCol*(nWidth+TABLE_HIGHKEEPSIDE))/(nCol*2);
	nHeightSide = (m_TotalRect.Height()-nRow*(nHeight+TABLE_HIGHKEEPSIDE))/(nRow*2);
	for( i = 0;i<nRow;i++)
	{
		for(j= 0;j<nCol;j++)
		{
			if(nItemCount>=m_vecInfoItem.size())break;
			singleRect.left = m_TotalRect.left +j*nWidth+(j+1)*TABLE_WIDTHKEEPSIDE+nWidthSide;
			singleRect.right = singleRect.left+nWidth;
			singleRect.top = m_TotalRect.top+ i*nHeight+(i+1)*TABLE_HIGHKEEPSIDE+nHeightSide;
			singleRect.bottom = singleRect.top+nHeight;
			if(!m_TotalRect.PtInRect(singleRect.BottomRight()))break;

			m_vecInfoItem[nItemCount].MoveTo(singleRect);
			nItemCount++;
		}
	}
	for(i = 0;i<m_nBeginLine*nCol;i++)
	{
		if(m_vecInfoItem.size()<=i)break;
		m_vecInfoItem[i].MoveTo(CRect(0,0,0,0));
	}
	for(i = nItemCount;i<m_vecInfoItem.size();i++)
	{
		if(m_vecInfoItem.size()<=i)break;
		m_vecInfoItem[i].MoveTo(CRect(0,0,0,0));
	}
}

//////////////////////////////////////////////////////////
//设置画图区域
void CPriceInfoObj::MoveTo(CRect rRect)
{
	m_TotalRect = rRect;
	RecalSubObjSize();
}

//////////////////////////////////////////////////////////
//传递数据
BOOL CPriceInfoObj::UpdateData(LPVOID pData, int nDataSize)
{
	SNormaHead *pDataHead = (SNormaHead*)pData;
	int iCount;

	vector<CBaseInfoItem>::iterator it;
	SNowData* pNowData;
	const StuSymbolData* pCodeInfo;
	if(pDataHead->m_usType == REPORTDATA)
	{	
		SNowData* pNow = (SNowData*)(pDataHead+1);
		int nNum;
		nNum = 0;
		for(iCount =0;iCount<pDataHead->m_usSize;iCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[iCount].sCode);
			if(NULL == pCodeInfo)continue;
			nNum++;
		}
		Init(nNum);
		int nPostion = 0;
		
		vector<int> vecSort;
		vecSort.resize(nNum);
		for(int i=0;i < vecSort.size();i++)
			vecSort[i] = -1;

		float fBuy,fSell;
		int nTradeProductNum = 0;//可交易产品数量
		//此处统计可交易产品位置
		for(iCount =0;iCount<pDataHead->m_usSize;iCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[iCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '1')
			{
				nPostion = 0;
				CPublic::Instance()->GetTradeDll()->m_pGetCustomUnit(pNow[iCount].sCode.szCode,fBuy,fSell,nPostion);
				if(nPostion == 0)
				{
					//vecSort[nTradeProductNum]= nTradeProductNum;
					CPublic::Instance()->SetStockTradeStatus(&pNow[iCount].sCode);
				}
				else
				{
					vecSort[nTradeProductNum] = nPostion-1;
			//	}
					nTradeProductNum++;
				}
			}
		}
		nTradeProductNum = 0;
		//此处统计非交易产品位置
		for(iCount =0;iCount<pDataHead->m_usSize;iCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[iCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '0')
			{
				//vecSort[nTradeProductNum]= nTradeProductNum;
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
		int nStation =0;
		for(iCount =0;iCount<pDataHead->m_usSize;iCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[iCount].sCode);
			if(NULL == pCodeInfo)
				continue;
			if(pCodeInfo->cTrade == '1')
			{
				if(nStation<vecSort.size())
				{
					nPostion = vecSort[nStation];
				}
				else
				{
					nPostion = nStation;
				}
				m_vecInfoItem[nPostion].SetStockInfo(&pNow[iCount].sCode,pCodeInfo->szName);
				m_vecInfoItem[nPostion].SetQuote(pNow+iCount);	
				nStation++;
			}
		}
		for(iCount =0;iCount<pDataHead->m_usSize;iCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[iCount].sCode);
			if(NULL == pCodeInfo)
				continue;
			if(pCodeInfo->cTrade == '0')
			{
				if(nStation<vecSort.size())
				{
					nPostion = vecSort[nStation];
				}
				else
				{
					nPostion = nStation;
				}
				m_vecInfoItem[nPostion].SetStockInfo(&pNow[iCount].sCode,pCodeInfo->szName);
				m_vecInfoItem[nPostion].SetQuote(pNow+iCount);	
				nStation++;
			}
		}
		//初始化表格
		RecalSubObjSize();
	}
	else if(pDataHead->m_usType == PUSH_REALDATA)
	{
		StuRecvQuoteInfo* pRecvQuote= (StuRecvQuoteInfo*)(pDataHead+1);
		//查找行列数
		SNowData nowData;
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
						break;
					}
				}		
			//}
		}
	}

	return TRUE;
}

//测试点击位置
int CPriceInfoObj::HitTest(CPoint point)
{
	int nCount;
	for(nCount = 0;nCount<m_vecInfoItem.size();nCount++)
	{
		if(m_vecInfoItem[nCount].HitTest(point))
		{
			return nCount;
		}
	}
	return -1;
//	return m_TotalRect.PtInRect(point);
}
int	CPriceInfoObj::HitRect(CPoint point,int& nBuySellSign)
{
	int nCount;
	for(nCount = 0;nCount<m_vecInfoItem.size();nCount++)
	{
		nBuySellSign = m_vecInfoItem[nCount].HistRect(point);
		if(0 != nBuySellSign)
		{
			return nCount;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////
//画图
BOOL CPriceInfoObj::Draw(CDC* pDC)
{	
	COLORREF crBKColor;
	crBKColor = CInterfaceManager::Instance()->GetSysColor(ID_PRICEBOARD_BACKGROUNDCOLOR);//ID_PRICEBOARD_BK);
	pDC->FillSolidRect(m_TotalRect,crBKColor);

	int nWidth,nHeight;
	CBaseInfoItem::GetItemWidthHeight(nWidth,nHeight);
	int nTotalWidth,nTotalHeight;
	nTotalHeight = m_TotalRect.Height();
	nTotalWidth = m_TotalRect.Width();
	if(nTotalWidth <nWidth||nTotalHeight < nHeight) return FALSE;//空间太小，无法放置


	vector<CBaseInfoItem>::iterator  it;
	for(it = m_vecInfoItem.begin();it != m_vecInfoItem.end();it++)
	{
		it->Draw(pDC);
	}
	return TRUE;
}

	
BOOL CPriceInfoObj::OnChar(int nChar)
{
	BOOL bRet = FALSE;
	switch( nChar) 
	{
	case VK_DOWN:
		{	
			int nCurSel = GetCurrentSelLine();
			nCurSel += m_nDisplayCol;	
			if(nCurSel >(m_vecInfoItem.size()-1))return FALSE;		
			if(nCurSel >=((m_nBeginLine+m_nDisplayRow)*m_nDisplayCol))
			{
//				nCurSel %=(m_nDisplayCol*m_nDisplayRow);
				m_nBeginLine += m_nDisplayRow;
				if((m_nBeginLine+m_nDisplayRow) >= m_nTotalLine) m_nBeginLine = m_nTotalLine-m_nDisplayRow;
				RecalSubObjSize();
			}
			SetCurrentSelLine(nCurSel);
		}
		break;
	case VK_UP:
		{
			int nCurSel = GetCurrentSelLine();
			nCurSel-= m_nDisplayCol;
			if(nCurSel <=0)return FALSE;
			if(nCurSel < ((m_nBeginLine)*m_nDisplayCol))
			{
				m_nBeginLine -= m_nDisplayRow;
				if(m_nBeginLine<=0)m_nBeginLine =0;
				RecalSubObjSize();
			}
			SetCurrentSelLine(nCurSel);					
		}
		break;
	case VK_LEFT:
		{
			int nCurSel = GetCurrentSelLine();
			if(nCurSel <=0)return FALSE;
			nCurSel--;
			SetCurrentSelLine(nCurSel);
		}
		break;
	case VK_RIGHT:
		{
			int nCurSel = GetCurrentSelLine();
			if(nCurSel >=(m_vecInfoItem.size()-1))return FALSE;
			nCurSel ++;		
			SetCurrentSelLine(nCurSel);
		}
		break;
	case VK_NEXT:
		{
			if((m_nBeginLine+m_nDisplayRow) >= m_nTotalLine)return FALSE;
			m_nBeginLine += m_nDisplayRow;
			if((m_nBeginLine+m_nDisplayRow) >= m_nTotalLine) m_nBeginLine = m_nTotalLine-m_nDisplayRow;
			RecalSubObjSize();
		}
		break;
	case VK_PRIOR:
		{
			if(m_nBeginLine<=0)return FALSE;
			m_nBeginLine -= m_nDisplayRow;
			if(m_nBeginLine<=0)m_nBeginLine =0;
			RecalSubObjSize();
		}
		break;
	case VK_HOME:
		{
			if(m_nBeginLine<=0)return FALSE;
			m_nBeginLine =0;
		}
		break;
	case VK_END:
		{
			if((m_nBeginLine+m_nDisplayRow) >= m_nTotalLine)return FALSE;
			m_nBeginLine = m_nTotalLine-m_nDisplayRow;
		}
		break;
	case VK_RETURN:
		{
// 			SCodeInfo* pCode;
// 			pCode = m_vecInfoItem[m_nCurrentSel].GetStockInfo();		
		}
		break;
	default:
		break;
	}	
	return bRet;
}

BOOL  CPriceInfoObj::Init(int nNum)
{
	m_vecInfoItem.resize(nNum);
	for(int nCount= 0;nCount<nNum;nCount++)
	{
		m_vecInfoItem[nCount].SetImageList(&m_ImageList);
	}
	RecalSubObjSize();
	LoadIndexStatusParam();
	return TRUE;
}
BOOL CPriceInfoObj::OnLButtonDblClk(CWnd* pWnd,UINT uiFlag,CPoint point)
{
	return FALSE;
}
int CPriceInfoObj::GetCurrentSelLine()
{
	return m_nCurrentSel;
}

SCodeInfo* CPriceInfoObj::GetCurSelCodeInfo()
{
	if(m_nCurrentSel<0||m_nCurrentSel>m_vecInfoItem.size())return NULL;
	return m_vecInfoItem[m_nCurrentSel].GetStockInfo();
}

void CPriceInfoObj::ShowHideOtherQuote(bool bShow)
{

}



// 鼠标右键按下响应
BOOL CPriceInfoObj::OnRButtonDown(UINT nFlags,CPoint point)
{
	int nCurSel = HitTest(point);//得到选择点
	if(nCurSel == -1)return FALSE;
	SetCurrentSelLine(nCurSel);//设置当前选择行

	return TRUE;
}

// 鼠标左键按下响应
BOOL CPriceInfoObj::OnLButtonDown(UINT nFlags,CPoint point)
{
	int nCurSel = HitTest(point);//得到选择点
	if(nCurSel == -1)return FALSE;
	SetCurrentSelLine(nCurSel);//设置当前选择行
	SCodeInfo* pCurSelCode;
	pCurSelCode = GetCurSelCodeInfo();//do some thing
	if(NULL == pCurSelCode)return FALSE;
	CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
	return TRUE;

}

// 鼠标左键双击按下响应
BOOL CPriceInfoObj::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int nBuySellSign;
	int nCurSel = HitRect(point,nBuySellSign);//HitTest(point);//得到选择点
	if(nCurSel == -1)return FALSE;
	SetCurrentSelLine(nCurSel);//设置当前选择行
	SCodeInfo* pCurSelCode;
	pCurSelCode = GetCurSelCodeInfo();//do some thing
	if(NULL == pCurSelCode || pCurSelCode->szCode[0] == 0)return FALSE;
	CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
	const StuSymbolData* pSymbolInfo = CPublic::Instance()->GetStockInfo(pCurSelCode);
	float fPrice =0.0f;

	if(pSymbolInfo->cTrade == SYMBOLTYPE_TRADEPRODUCT&&CPublic::Instance()->GetTradeDll()->m_pGetUserStatus)
	{	
		nBuySellSign =(nBuySellSign==1)?1:0;
		CPublic::Instance()->CallQuickTrade(pCurSelCode,nBuySellSign);
	}
	return TRUE;
}

void CPriceInfoObj::SetCurrentSelLine(int nSel)
{
	m_nCurrentSel = nSel;
	int nCount;
	for(nCount = 0;nCount<m_vecInfoItem.size();nCount++)
	{
		if(nCount == nSel)
		{
			m_vecInfoItem[nCount].SetSelectStatus(TRUE);
		}
		else
		{
			m_vecInfoItem[nCount].SetSelectStatus(FALSE);
		}
	}
}
void CPriceInfoObj::LoadIndexStatusParam()
{
	//加载配置文件
	CString strParamPath;
	strParamPath = CPublic::Instance()->GetSysPath();
	strParamPath += _T("koa.ini");
	CIniFile  iniFile;
	iniFile.Create( strParamPath);
}