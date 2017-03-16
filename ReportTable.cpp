///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReportTable.h"
#include "resource.h"
#include <algorithm>
#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Construction/Destruction /////////////////////////////////////////

CReportTable::CReportTable()
{
	m_pImageList  = new CImageList;
	m_pImageList->Create(IDB_ARROWBITMAP1,10,10,RGB(255,255,255));
	
	m_nRealDataUpdateBeginIndex = 0;	
	m_nCurBeginIndex = 0;	
	m_nCurRequestBeginIndex  = 0;
	m_nCurRequestEndIndex   = 0;
	m_nItemStyle = 0;
	SetExtendStyle(LVS_EX_FULLROWSELECT);
	m_nRowHeight = 24;
	SetDrawColumnLine (TRUE);
	m_nItemStyle = SSubItem::BOTTOM_LINE|SSubItem::RIGHT_LINE|SSubItem::LEFT_LINE;

//	m_nTradeTimeCount = 1;
}

CReportTable::~CReportTable()
{
	if(m_pImageList)
	{
		m_pImageList->DeleteImageList();
		delete m_pImageList;
	}
	SaveIndexStatusParam();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Overrides ///////////////////////////////////////////////////////////////////
//函数功能：用于初始化列和设置属性，所有基层类必须重载此函数
//函数参数：无
//函数返回: 成功为TRUE,错误为FALSE
//函数异常: 无
BOOL CReportTable::Init()
{  
	m_ArrColumn.RemoveAll();

	SetExtendStyle(LVS_EX_FULLROWSELECT);
	
	int nCodeColWidth = 57;
	
	m_vecReportCol.clear();

	TReportColInfo tColList[] =
	{
		{PRICE_NORMAL,     {CLanguageInterface::Instance()->GetStr(20020),-1,LVCFMT_LEFT,100}},
		{PRICE_NORMAL,     {CLanguageInterface::Instance()->GetStr(20021),-1,LVCFMT_LEFT,140}},
		{PRICE_NORMAL,     {CLanguageInterface::Instance()->GetStr(20022),-1,LVCFMT_RIGHT,80}},
		{PRICE_NORMAL,     {CLanguageInterface::Instance()->GetStr(20023),-1,LVCFMT_RIGHT,80}},
		{PRICE_NORMAL,     {CLanguageInterface::Instance()->GetStr(20024),-1,LVCFMT_RIGHT,80}},
		{PRICE_NORMAL,     {CLanguageInterface::Instance()->GetStr(20025),-1,LVCFMT_RIGHT,80}},
		{PRICE_NORMAL,	   {CLanguageInterface::Instance()->GetStr(20026),-1,LVCFMT_RIGHT,80}}
	};

	int nColCount = 7;
	int nColOrderIndexList[]={0,1,2,3,4,5,6};
	
	//////////////////////////////////////////////////////////////////////////
	//获取应用程序路径
	m_strParamPath = CPublic::Instance()->GetSysPath();
	m_strParamPath += _T("koa.ini");
	LoadIndexStatusParam();
	vector<int>::iterator it;
	//////////////////////////////////////////////////////////////////////////
	for( int i=0; i<nColCount; i++)
	{
		it = find(m_vecShow.begin(),m_vecShow.end(),i);
		if(it == m_vecShow.end())
		{
			tColList[i].tColHeadInfo.dwColumnWide = 0;
		}
		InsertColumn(i,tColList[i].tColHeadInfo.szColumnText,
			       	 tColList[i].tColHeadInfo.dwFormat,
					 tColList[i].tColHeadInfo.dwColumnWide);
		
		m_ArrdwItemOrder[i] = nColOrderIndexList[i];

		m_vecReportCol.push_back(tColList[i]);
	}	
	//////////////////////////////////////////////////////////////////////////
	SetFixedColumn(1);//设置固定列数	
	return TRUE;
}

//函数功能：传递数据，所有继承类必须重载此接口来设置数据
//函数参数：lpData 多态数据流
//函数返回: 成功为TRUE,错误为FALSE
//函数异常: 无
BOOL CReportTable::UpdateData(LPVOID lpData,int nDataLen)
{
	SNormaHead *pDataHead = (SNormaHead*)lpData;

	if(pDataHead->m_usType == REPORTDATA)
	{	
		SNowData* pNow = (SNowData*)(pDataHead+1);
		int nCount;
		int nNum;
		nNum = 0;
		const StuSymbolData* pCodeInfo;
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			nNum++;
		}
		int nPostion = 0;

		vector<int> vecSort;
		vecSort.resize(nNum);
		for(int i=0;i < vecSort.size();i++)
			vecSort[i] = -1;

		float fBuy,fSell;
		int nTradeProductNum = 0;//可交易产品数量
		//此处统计可交易产品位置
		//for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		//{	
		//	pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
		//	if(NULL == pCodeInfo)continue;
		//	if(pCodeInfo->cTrade == '1')
		//	{
		//		nPostion = 0;
		//		CPublic::Instance()->GetTradeDll()->m_pGetCustomUnit(pNow[nCount].sCode.szCode,fBuy,fSell,nPostion);
		//		if(nPostion == 0)
		//		{
		//			vecSort[nTradeProductNum]= nTradeProductNum;
		//		}
		//		else
		//		{
		//			vecSort[nTradeProductNum] = nPostion-1;
		//		}

		//		nTradeProductNum++;
		//	}
		//}
		
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

		m_vecNowData.resize(nNum);	
		int nStation =0;
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '1')
			{
				nPostion = vecSort[nStation];
			
				m_vecNowData[nPostion].oNowData = pNow[nCount];
				m_vecNowData[nPostion].nUpDown = -1;
				m_vecNowData[nPostion].ssSymbolName = pCodeInfo->szName;
				m_vecNowData[nPostion].cIsTradeSymbol = pCodeInfo->cTrade;
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

				m_vecNowData[nPostion].oNowData = pNow[nCount];
				m_vecNowData[nPostion].nUpDown = -1;
				m_vecNowData[nPostion].ssSymbolName = pCodeInfo->szName;
				m_vecNowData[nPostion].cIsTradeSymbol = pCodeInfo->cTrade;
				nStation++;
			}
		}
		m_nTotalLine = nNum;
		m_vecShowData = m_vecNowData;
		TransData2Table();
		SetCurrentSelLine(0);
	}
	else if(pDataHead->m_usType == PUSH_REALDATA)
	{
		if((m_vectCodeRowInfo.empty())||m_vecShowData.empty())return FALSE;
		StuRecvQuoteInfo* pRecvQuote= (StuRecvQuoteInfo*)(pDataHead+1);
//		TRACE("pointer: %x\r\n",pRecvQuote);
		int nStation;		
		//查找行列数
		for(int iCount =0;iCount<pDataHead->m_usSize;iCount++)
		{
//			int nRecord = 0;
//			int nHqstruLen = 0;
//			nHqstruLen = sizeof(StuRecvQuoteInfo);
//			nRecord = (nDataLen-sizeof(SNormaHead))/nHqstruLen;
//			for(int i=0;i<nRecord;i++)
//			{
				StuRecvQuoteInfo* pQuote = (StuRecvQuoteInfo*)(pRecvQuote+iCount);
//				TRACE("datapointer:%x\r\n",pQuote);
				if(!FindStation(&pQuote->sCode,nStation))continue;
				UpdateRealData(pQuote,m_vecShowData[nStation]);
				//if (m_vecShowData[nStation].nUpDown == -1)
				//	continue;
				if((nStation>=m_nCurrentBeginLine)&&(nStation < (m_nCurrentBeginLine+m_nCurrentShowLines)))
				{
					UpdateNowData(&m_vecShowData[nStation],m_vectCodeRowInfo[nStation-m_nCurrentBeginLine],m_vecShowData[nStation].nUpDown);
					UpdateReportItem(m_nCurrentBeginLine,nStation,m_vectCodeRowInfo[nStation-m_nCurrentBeginLine]);
				}
//			}
		}
	}	
	return TRUE;
}
void CReportTable::TransData2Table()
{
	TCodeRowInfo codeinfo;
	codeinfo.vecColValue.resize(6);//设置长度
	m_vectCodeRowInfo.clear();
	int nDispalayNum;
	nDispalayNum = ((m_nCurBeginIndex+m_nCurrentShowLines)>m_nTotalLine)?m_nTotalLine:(m_nTotalLine-m_nCurBeginIndex);
	for(int nCount=0;nCount<nDispalayNum;nCount++)
	{
		UpdateNowData(&m_vecShowData[m_nCurBeginIndex+nCount],codeinfo,m_vecShowData[m_nCurBeginIndex+nCount].nUpDown);
		m_vectCodeRowInfo.push_back(codeinfo);
	}
	UpdateReport(m_vectCodeRowInfo);
}

bool CReportTable::FindStation(SCodeInfo* pCode,int& nStation)
{
	for(nStation = 0;nStation < m_vecShowData.size();nStation++)
	{
		if( 0 == strcmp(m_vecShowData[nStation].oNowData.sCode.szCode,pCode->szCode))
			return true;
	}
	return false;
}

BOOL CReportTable::OnUserCmd(unsigned int nSel)
{
	return FALSE;
}

BOOL CReportTable::OnChar(UINT nChar)
{
	BOOL bRet = FALSE;
	switch( nChar) 
	{
	case VK_DOWN:
		{	
// 			int nCurSel = GetCurrentSelLine();
// 			nCurSel++;		
// 			m_nCurrentSel = nCurSel;
		}
		break;
	case VK_UP:
		{
// 			int nCurSel = GetCurrentSelLine();
// 			nCurSel--;
// 			m_nCurrentSel = nCurSel;						
		}
		break;
	case VK_NEXT:
		{
			if((m_nCurBeginIndex+(m_nCurrentShowLines-1)) > m_nTotalLine)return FALSE;
			m_nCurBeginIndex += (m_nCurrentShowLines-1);
			if((m_nCurBeginIndex) > m_nTotalLine) m_nCurBeginIndex = 0;
		}
		break;
	case VK_PRIOR:
		{
			if(m_nCurBeginIndex<=0)return FALSE;
			m_nCurBeginIndex -= (m_nCurrentShowLines-1);
			if(m_nCurBeginIndex<=0)m_nCurBeginIndex =0;
		}
		break;
	case VK_HOME:
		{
			if(m_nCurBeginIndex<=0)return FALSE;
			m_nCurBeginIndex =0;
		}
		break;
	case VK_END:
		{
			if((m_nCurBeginIndex+(m_nCurrentShowLines-1)) >= m_nTotalLine)return FALSE;
			m_nCurBeginIndex = m_nTotalLine-(m_nCurrentShowLines-1);
		}
		break;
	default:
		break;
	}	
	if( !bRet)
	{
		bRet = CBaseTable::OnKeyDownEvent(nChar);
	}	
	return bRet;
}

void CReportTable::Draw(CDC *pDC)
{
	CBaseTable::Draw(pDC);
	{
		CPen LinePen(PS_SOLID,1,m_crLineColor);
		CPen* oldPen = pDC->SelectObject(&LinePen);
		pDC->MoveTo(m_DrawRect.left,m_DrawRect.top+m_nRowHeight);
		pDC->LineTo(m_DrawRect.right,m_DrawRect.top+m_nRowHeight);
		pDC->SelectObject(oldPen);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////  Implementation //////////////////////////////////////////////////
SCodeInfo* CReportTable::GetCurSelCodeInfo()
{
	if(m_vectCodeRowInfo.size() <= 0) //没有证券
		return NULL;
	
	int iClickLine = GetCurrentSelLine();//得到当前选择行
	if(-1 == iClickLine) 
		return NULL;//无效
	
	if( iClickLine >= m_ArrItemInfo.GetSize())
		return NULL;
    if((m_nCurrentBeginLine+iClickLine)>= m_vecCurrentCode.size())return NULL;
	return &m_vecCurrentCode[m_nCurrentBeginLine+iClickLine];
}

//函数功能：响应鼠标右键点击消息
BOOL CReportTable::OnRButtonDown(CWnd *pParentWnd,UINT nFlags,CPoint point)
{
	return TRUE;
}

void CReportTable::SetCurrentSelLine(int nSel)
{
	CBaseTable::SetCurrentSelLine(nSel);
	SCodeInfo* pCurSelCode;
	pCurSelCode = GetCurSelCodeInfo();//do some thing
	if(NULL == pCurSelCode)return ;	
	//CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
	CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
}

// 鼠标左键按下响应按钮
BOOL CReportTable::OnLButtonDown(CWnd *pParentWnd,UINT nFlags,CPoint point)
{
	int nCurSelCodIndex = IsClickColumn(point);
	if(nCurSelCodIndex != -1)
	{
		return TRUE;
	}	
	int nCurSel = HitTest(point);//得到选择点
	if(nCurSel != -1)
	{
		SetCurrentSelLine(nCurSel);//设置当前选择行
		return TRUE;
	}	
	
	return FALSE;
}

BOOL CReportTable::OnLButtonDblClk(CWnd *pParentWnd,UINT nFlags, CPoint point) 
{
	// 如果是双击表头，则返回
    int nCurSel = IsClickColumn(point);
	if(nCurSel != -1)
		return FALSE;		
	
    // 如果鼠标没有在表格区域内，也返回	
	int nCol=-1;
	nCurSel = HitReport(point,nCol);//得到选择点
	if(nCurSel == -1)return FALSE;
	if(nCol== -1)return FALSE;//没有选择任何列
	nCol= (nCol == 2)?1:2;
	SetCurrentSelLine(nCurSel);//设置当前选择行
	SCodeInfo* pCurSelCode;
	pCurSelCode = GetCurSelCodeInfo();//do some thing
// 	if(NULL == pCurSelCode || pCurSelCode->szCode[0] == 0 )return FALSE;	
// 	CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
	const StuSymbolData* pSymbolInfo = CPublic::Instance()->GetStockInfo(pCurSelCode);
	if(pSymbolInfo->cTrade == SYMBOLTYPE_TRADEPRODUCT&&CPublic::Instance()->GetTradeDll()->m_pGetUserStatus)
	{
		nCol =(nCol==1)?1:0;
		CPublic::Instance()->CallQuickTrade(pCurSelCode,nCol);
	}	
	return TRUE;
}

int	CReportTable::HitReport(CPoint pt,int& nCol)
{
	if(!m_DrawRect.PtInRect(pt))return -1;
	int iLine;
	iLine = (pt.y -m_DrawRect.top)/m_nRowHeight;
	if(m_dwStyle!=LVS_NOCOLUMNHEADER)
	{
		iLine--;//减去头
	}
	if(iLine >= m_ArrItemInfo.GetSize())return -1;

	int nCurrentPoint;
	nCurrentPoint = pt.x - m_DrawRect.left;
	nCol = -1;
	for(int i = 0;i<m_ArrColumn.GetSize();i++)
	{
		if(nCurrentPoint<= m_ArrColumn[i].dwColumnWide)
		{
			nCol= i;
			break;
		}
		nCurrentPoint -=m_ArrColumn[i].dwColumnWide;
	}
	return iLine;
}

unsigned char CReportTable::GetColSortDefine(int nColIndex)
{
	int i = 0;
	vector<TReportColInfo>::iterator iterCol;
	for( iterCol = m_vecReportCol.begin();
	iterCol != m_vecReportCol.end();
	iterCol++)
	{
		if( i == nColIndex)
		  return iterCol->ucColDefine;

		i++;
	}
	return 0;
}

void CReportTable::UpdateReport(const vector<TCodeRowInfo> &vecCodeRowInfo)
{
	int i = 0;
	int nDisplayIndex = 0;
	nDisplayIndex = m_nCurBeginIndex;
	DeleteAll();
	m_vecCurrentCode.clear();
	vector<TCodeRowInfo>::const_iterator iterCurRow;
	for( iterCurRow = vecCodeRowInfo.begin();
		iterCurRow != vecCodeRowInfo.end();
		iterCurRow++)
	{
		UpdateReportItem(nDisplayIndex,i,*iterCurRow);	
		i++;
		nDisplayIndex++;
		m_vecCurrentCode.push_back(iterCurRow->tCodeInfo);
	}
}

void CReportTable::UpdateReportItem(int nDisplayIndex,int nIndex,const TCodeRowInfo &tCurRow)
{
	InsertItem(nIndex,0,CString(tCurRow.tCodeInfo.szCode),tCurRow.nImage,-1,-1,m_nItemStyle);//名称
	InsertItem(nIndex,1,CString(tCurRow.ssSymbolName.c_str()),-1,-1,-1,m_nItemStyle);//名称
	int j = 2;
	vector<TReportItemInfo>::const_iterator iterCurRowItem;
	for( iterCurRowItem = tCurRow.vecColValue.begin();
		iterCurRowItem != tCurRow.vecColValue.end();
		iterCurRowItem++)
	{
		InsertItem(nIndex,j,iterCurRowItem->strFormatValue,
			-1,iterCurRowItem->crTextColor,iterCurRowItem->crBackColor,m_nItemStyle);
		j++;
	}
// 	const StuSymbolData* pSymbol = CPublic::Instance()->GetStockInfo(&tCurRow.tCodeInfo);
// 	COLORREF crBK;
// 	if(pSymbol->cTrade =='0')
// 	{
// 		crBK = CInterfaceManager::Instance()->GetSysColor(ID_TRADESYMBOL_COLOR);
// 	}
// 	else if(pSymbol->cTrade == '1')
// 	{
// 		crBK = CInterfaceManager::Instance()->GetSysColor(ID_UNTRADESYMBOL_COLOR);
// 	}
// 	SetItemLineBk(nIndex,crBK);
}

//是否点中了头,如果是，返回当前点中列，否则返回-1
int CReportTable::IsClickColumn(CPoint point)
{
	if(m_dwExStyle == LVS_NOCOLUMNHEADER)
		return -1;

	if((point.y < m_DrawRect.top)||(point.y >m_DrawRect.top +m_nRowHeight))return -1;
	int iStation;
	int nCount;
	int nWidth;
	nWidth = point.x;
	for(nCount = 0;nCount<m_ArrColumn.GetSize();nCount++)
	{//判断位置
		if(nCount >= m_nFixedColumn)
			iStation = nCount+m_nHoriBeiginLine;//加上右侧增量
		else
			iStation = nCount;
		if(iStation >= m_ArrColumn.GetSize())break;
		iStation = m_ArrdwItemOrder[iStation];//变换对应位置
		SColumnHead& column = m_ArrColumn[iStation];
		if(nWidth > column.dwColumnWide)
		{
			nWidth -= column.dwColumnWide;
		}
		else
		{
			break;//找到对应
		}
	}

	s_nSortColIndex = iStation;

	return iStation;
}

//更新最新数据
void CReportTable::UpdateNowData(const SUpdateData *pNow,TCodeRowInfo &rtRowInfo,int nUpDown)
{
	strcpy(rtRowInfo.tCodeInfo.szCode,pNow->oNowData.sCode.szCode);//名称
	rtRowInfo.ssSymbolName = pNow->ssSymbolName;//商品名字

	rtRowInfo.nImage = nUpDown;
	const StuSymbolData* pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow->oNowData.sCode);	
	if(NULL == pCodeInfo)return;
	COLORREF crText;
//	COLORREF crBack;

	if(1 == nUpDown)
	{
		crText = CInterfaceManager::Instance()->GetSysColor(ID_DOWN_COLOR);
	}
	else if(0 == nUpDown)
	{
		crText = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	}
	else
	{
		crText = CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR);
	}

// 	if(pNow->cIsTradeSymbol == 1)
// 	{
// 		crBack = CInterfaceManager::Instance()->GetSysColor(ID_UNTRADESYMBOL_COLOR);
// 	}
// 	else
// 	{
// 		crBack = CInterfaceManager::Instance()->GetSysColor(ID_TRADESYMBOL_COLOR);
// 	}

	CString strPriceDecFormat;

	strPriceDecFormat.Format(_T("%%.%df"),CPublic::Instance()->GetDecimal(&pCodeInfo->sCode));

	float fBuyUnit,fSellUnit;
	int nSort;
	nSort = -1;
	CPublic::Instance()->GetCustomUnit(pNow->oNowData.sCode.szCode,fBuyUnit,fSellUnit,nSort);


	float fPow;
	fPow = pow(10,(double)pCodeInfo->sCode.cPriceUnit);

	vector<TReportItemInfo>::iterator it = rtRowInfo.vecColValue.begin();

	it->fValue =  (float)pNow->oNowData.uiNewPrice/fPow+fSellUnit;//卖出
	it->strFormatValue.Format(strPriceDecFormat,it->fValue);
	it->crTextColor = crText;
//	it->crBackColor = crBack;
	it++;


	it->fValue = (float)pNow->oNowData.uiNewPrice/fPow +fBuyUnit;//买入
	it->strFormatValue.Format(strPriceDecFormat,it->fValue);
	it->crTextColor = crText;
//	it->crBackColor = crBack;
	it++;

	it->fValue =  (float)pNow->oNowData.uiMaxPrice/fPow;//最高
	it->strFormatValue.Format(strPriceDecFormat,it->fValue);
	it->crTextColor = crText;
//	it->crBackColor = crBack;
	it++;

	it->fValue =  (float)pNow->oNowData.uiMinPrice/fPow;//最低
	it->strFormatValue.Format(strPriceDecFormat,it->fValue);
	it->crTextColor = crText;
//	it->crBackColor = crBack;
	it++;

	CTime  mytime(pNow->oNowData.timeUpdate);
	CString strTmp;

	strTmp = mytime.Format(_T("%H:%M:%S"));
	if (strTmp == "00:00:00")
		strTmp = "--:--:--";
	it->fValue  = 0.00f;
	it->strFormatValue = strTmp;
	it->crTextColor = crText;
//	it->crBackColor = crBack;
}

//更新实时数据
void CReportTable::UpdateRealData(const StuRecvQuoteInfo* pReal,SUpdateData &oUpdateData)
{

	//转换
	if(pReal->uiPrice > oUpdateData.oNowData.uiNewPrice)
		oUpdateData.nUpDown = 0;
	else if(pReal->uiPrice < oUpdateData.oNowData.uiNewPrice)
		oUpdateData.nUpDown = 1;
	else
		oUpdateData.nUpDown = -1;
	oUpdateData.oNowData.uiNewPrice = pReal->uiPrice;//最新价
	if(pReal->uiPrice >oUpdateData.oNowData.uiMaxPrice)//最高价
		oUpdateData.oNowData.uiMaxPrice = pReal->uiPrice;
	if(pReal->uiPrice <oUpdateData.oNowData.uiMinPrice)//最低价
		oUpdateData.oNowData.uiMinPrice = pReal->uiPrice;

	oUpdateData.oNowData.timeUpdate = pReal->lastTick;

}

int	CReportTable::GetTotalCount()
{
	return (int)m_vectCodeRowInfo.size();
}

void CReportTable::ShowHideOtherQuote(BOOL bShow)
{
	if(bShow)
	{
		m_vecShowData = m_vecNowData;

	}
	else
	{
		m_vecShowData.clear();
		int nCount;
		for(nCount =0;nCount<m_vecNowData.size();nCount++)
		{	
			if(m_vecNowData[nCount].cIsTradeSymbol == '0')continue;
			m_vecShowData.push_back(m_vecNowData[nCount]);
		}

	}
	m_nTotalLine = m_vecShowData.size();
	TransData2Table();
}

void CReportTable::ShowHighLowPrice(BOOL bShow)
{
	if(m_ArrColumn.GetSize()<6)return;
	if(bShow)
	{
		m_ArrColumn[4].dwColumnWide = 80;
		m_ArrColumn[5].dwColumnWide = 80;
		m_vecShow.push_back(4);
		m_vecShow.push_back(5);
	}
	else
	{
		m_ArrColumn[4].dwColumnWide = 0;
		m_ArrColumn[5].dwColumnWide = 0;
		vector<int>::iterator it;
		it = find(m_vecShow.begin(),m_vecShow.end(),4);
		if(it != m_vecShow.end())
			m_vecShow.erase(it);
		it = find(m_vecShow.begin(),m_vecShow.end(),5);
		if(it != m_vecShow.end())
			m_vecShow.erase(it);
	}

}

void CReportTable::ShowTime(BOOL bShow)
{
	if(m_ArrColumn.GetSize()<6)return;
	if(bShow)
	{
		m_ArrColumn[6].dwColumnWide = 50;
		m_vecShow.push_back(6);
	}
	else
	{
		m_ArrColumn[6].dwColumnWide = 0;
		vector<int>::iterator it;
		it = find(m_vecShow.begin(),m_vecShow.end(),6);
		if(it != m_vecShow.end())
			m_vecShow.erase(it);
	}
}
void CReportTable::ResetTableColor()
{
	TransData2Table();
}

void CReportTable::LoadIndexStatusParam()
{
	//加载配置文件
	CIniFile  iniFile;
	iniFile.Create( m_strParamPath);

	CString strShow;
	if( !iniFile.GetVarStr( _T("REPORT"), _T("SHOW"), strShow))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10003),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return ;
	}
	int iPos;
	CString strLeft;
	do
	{
		iPos = strShow.Find(_T(","));
		if(iPos != -1)
		{
			strLeft=strShow.Left(iPos);
			strShow = strShow.Mid(iPos+1);
			m_vecShow.push_back(atoi((char*)(LPCTSTR)strLeft));
		}
	}while(-1 != iPos);

}

void CReportTable::SaveIndexStatusParam()
{
	if(m_strParamPath.IsEmpty())return;
	//加载配置文件
	CIniFile  iniFile;
	iniFile.Create( m_strParamPath);

	CString strIndexName;
	vector<int>::iterator it;
	CString strValue;
	for(it = m_vecShow.begin();it != m_vecShow.end();it++)
	{
		strValue.Format(_T("%d"),*it);
		strIndexName +=strValue;
		strIndexName +=",";
	}
	if( !iniFile.SetVarStr( _T("REPORT"), _T("SHOW"), strIndexName))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10003),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return ;
	}
}