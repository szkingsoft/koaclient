// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "PriceVolObj.h"
#include "StockView.h"
#include "resource.h"
#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPriceVolObj::CPriceVolObj()
{
	m_pPriceValues = NULL;

	m_nHoriPoints = 0;
	m_nValueNum = 0;
	m_nRealTechType = PRICEVOLPIC;


	m_bMouseMove = FALSE;
	m_nCurrentStation = -1;
	m_bLeftButtonDown = FALSE;
	m_nCurrentStation = 0;
	
	m_pCodeInfo = NULL;
	//m_fPrevClose = 0.0f;
	m_iDate = CPublic::Instance()->GetDate();	
	m_CodeInfoArray.RemoveAll();
	m_bDrawCross = FALSE;
}

CPriceVolObj::~CPriceVolObj()
{
	if(m_pPriceValues)
	{
		delete []m_pPriceValues;	
		m_pPriceValues = NULL;
	}

	m_CodeInfoArray.RemoveAll();
}
//////////////////////////////////////////////////////////////////////////
//分配画图区间及数据初始化
//////////////////////////////////////////////////////////////////////////
//函数功能：重新分配子区域
//函数参数：无
//函数返回：
void CPriceVolObj::RecalSubObjSize(int iRightW)
{
	if(!CPublic::Instance()->IsMinuteDataMode())
	{
		m_nShowMode = NORIGHTRAWRULER|NOTIMERULER;
	}
	else
	{
		m_nShowMode =NORIGHTRAWRULER;
	}
	//根据技术指标类型，确定分成几个区域
	CRect ItemRect;
	ItemRect = m_DrawRect;
	if(!(m_nShowMode&AUTOFITLEFTSIZE))
	{//自动适应左边距
		ItemRect.left += VERTBANKSIZE;
	}
	if(!(m_nShowMode&NORIGHTRAWRULER))
	{
		ItemRect.right -= VERTBANKSIZE;
	}
	//////////////////////////////////////////////////////////////////////////
	//得到高度
//	float fHeight;
	if(!(m_nShowMode&NOTIMERULER))
	{
		ItemRect.bottom = m_DrawRect.bottom- HORIBANKSIZE;//下侧区域
		//设置时间刻度区域
		m_TimeRect = ItemRect;//得到宽度
		m_TimeRect.top =m_DrawRect.bottom- HORIBANKSIZE;
		m_TimeRect.bottom = m_DrawRect.bottom;
	}
	if(!(m_nShowMode&NOTITLEINFO))
	{
		ItemRect.top += HORIBANKSIZE;//上侧区域
		//设置抬头区域
		m_TitleRect = ItemRect;
		m_TitleRect.top = m_DrawRect.top;
		m_TitleRect.bottom = m_TitleRect.top + HORIBANKSIZE;
	}
	else
	{
		ItemRect.top += 10;//保留上侧区域
	}
//	fHeight = (float);

	//成交价格区域
	m_PriceRect = ItemRect;//得到系统宽度高度
//	m_PriceRect.bottom = ItemRect.top + fHeight;
	m_PricePic.MoveTo(m_PriceRect);//价格区域

	
	//实时技术指标图元区域
	m_TechRect = ItemRect;
	m_TechRect.top = m_VolumnRect.bottom;
	m_TechRect.bottom = m_DrawRect.bottom - HORIBANKSIZE;
	
	//设置信息条画图区域
	m_TipInfoRect.left =m_DrawRect.left;
	m_TipInfoRect.right = m_DrawRect.left+VERTBANKSIZE-2;
	m_TipInfoRect.top = m_DrawRect.top+HORIBANKSIZE;
	m_TipInfoRect.bottom = m_DrawRect.top+18*4+10/*120+HORIBANKSIZE*/;

	m_InfoTable.MoveTo(m_TipInfoRect);

}

//////////////////////////////////////////////////////////////////////////
//设置股票信息、画图类型等
//////////////////////////////////////////////////////////////////////////
//函数功能：设置证券信息
//函数参数：pCodeInfo证券信息
//函数返回：无
void CPriceVolObj::SetStockInfo(const SCodeInfo* pCodeInfo)
{
	if(NULL == pCodeInfo)
		return;
	if(pCodeInfo->cPriceUnit<0)return;
	m_pCodeInfo = pCodeInfo;
	m_InfoTable.SetStockInfo(pCodeInfo);
	SetHoriPoint();
	RecalSubObjSize();
}

//函数功能：根据交易时间分配各个图元的数据内存
//函数参数：无
//函数返回：无
void CPriceVolObj::SetHoriPoint()
{

	m_InfoTable.Init();  

	const StuSymbolData* pStockInfo = CPublic::Instance()->GetStockInfo(m_pCodeInfo);
	
	int nHoriPoints;
	if(NULL == pStockInfo)
	{
		nHoriPoints = 60;
	}
	else
	{
		nHoriPoints = pStockInfo->nTotalTradeTime;//得到总交易时间
	}
	if(nHoriPoints<0)return;
	m_PricePic.SetTradeTime(nHoriPoints/60);
	nHoriPoints += 2;
	int nDecimal = CPublic::Instance()->GetDecimal(m_pCodeInfo);
	int nPow = (int)pow(10,(double)CPublic::Instance()->GetDecimal(m_pCodeInfo));
	m_PricePic.SetDecimal(nDecimal,nPow);			//成交价
	if(m_nHoriPoints != nHoriPoints) //总交易时间相同，不用再重新分配内存
	{
		m_nHoriPoints = nHoriPoints;
		m_PricePic.SetHoriPoint(m_nHoriPoints);	   //成交价
 		//分配缓冲区
		if (m_pPriceValues)	delete []m_pPriceValues;
		m_pPriceValues  = new long[m_nHoriPoints];
	}
	for(int i =0;i<m_nHoriPoints;i++)
	{
		m_pPriceValues[i]= 0x7fffffff;
	}
//	ZeroMemory(m_pPriceValues,m_nHoriPoints*sizeof(long));
}	

//函数功能：设置画图模式
//函数参数：0为画成交量，1为画买卖力道
//			2为量比图    3为ADL指标
//函数返回：
BOOL CPriceVolObj::SetRealTechType(int nTechType)
{
	if(nTechType < PRICEVOLPIC )
		nTechType = PRICEVOLPIC;
	if((nTechType>=PRICEVOLPIC))
	{
		m_nRealTechType = nTechType;
		RecalSubObjSize();
		return TRUE;
	}
	return FALSE;
}

int CPriceVolObj::GetRealTechType()
{
	return m_nRealTechType;
}

//函数功能：设置画图模式，
//函数参数：TRUE为分时走势图， FALSE为多股同列
//函数返回：
void CPriceVolObj::SetDisplayMode(DWORD nShowMode)
{
	m_nShowMode = nShowMode;
	m_PricePic.SetDisplayMode(nShowMode);
}

//////////////////////////////////////////////////////////////////////////
//请求、接受处理数据
//////////////////////////////////////////////////////////////////////////
//函数功能: 是否已请求过codeInfo走势数据
//函数参数: codeInfo需要请求的商品
//函数返回: void
BOOL CPriceVolObj::IsExistCodeInfo(SCodeInfo codeInfo)
{
	int iCodeNum;
	for(iCodeNum = 0; iCodeNum < m_CodeInfoArray.GetSize(); iCodeNum++)
	{
		const SCodeInfo& code =  m_CodeInfoArray[iCodeNum];
		if(!memcmp(&codeInfo,&code,sizeof(SCodeInfo)))
			return TRUE;
	}
	return FALSE;
}

//函数功能: 请求第一次进入当前视图时的商品的走势数据
//函数参数: 无
//函数返回: void
void CPriceVolObj::RequestData()
{
	if(m_pCodeInfo == NULL)
		return;
	// 请求当天的信息地雷
	int iBufferSize;
	iBufferSize = sizeof(SCodeInfo)+sizeof(SNormaHead); 
	char * pBuffer = new char[iBufferSize];
	SNormaHead* pRrequest =(SNormaHead*)pBuffer ;
	if(CPublic::Instance()->IsMinuteDataMode())
	{
		pRrequest->m_usType = STKMINUTEDATA; 
	}
	else
	{
		pRrequest->m_usType = STKTRACEDATA; 
	}
//	pRrequest->m_usType = STKTRACEDATA; 
	pRrequest->m_usIndex = CPublic::Instance()->AddRequestIndex();
	pRrequest->m_usSize = 1;

	SCodeInfo *pCode = (SCodeInfo *)(pRrequest+1);
	memcpy(pCode,m_pCodeInfo,sizeof(SCodeInfo));
	CPublic::Instance()->SendRequestData(pBuffer,iBufferSize);
	delete []pBuffer;
}
void CPriceVolObj::RequestData_SP()
{
	if(m_pCodeInfo == NULL)
		return;
	// 请求当天的信息地雷
	int iBufferSize;
	iBufferSize = sizeof(SCodeInfo)+sizeof(SNormaHead); 
	char * pBuffer = new char[iBufferSize];
	SNormaHead* pRrequest =(SNormaHead*)pBuffer ;
	if(CPublic::Instance()->IsMinuteDataMode())
	{
		pRrequest->m_usType = STKMINUTEDATA_ZYQ; 
	}
	else
	{
		pRrequest->m_usType = STKTRACEDATA_ZYQ; 
	}
	pRrequest->m_usType = STKTRACEDATA; 
	pRrequest->m_usIndex = CPublic::Instance()->AddRequestIndex();
	pRrequest->m_usSize = 1;

	SCodeInfo *pCode = (SCodeInfo *)(pRrequest+1);
	memcpy(pCode,m_pCodeInfo,sizeof(SCodeInfo));
	CPublic::Instance()->SendRequestData(pBuffer,iBufferSize);
	delete []pBuffer;
}

//函数功能：处理更新数据
//函数参数：pData 指向数据缓冲区的指针
//          dwDataSize 数据数量
//函数返回：如果在本区域内返回TRUE,否则为false 
BOOL CPriceVolObj::UpdateData(LPVOID pData,int dwDataSize)
{
	CSinglockGuard singlock(&m_lockData);
	SNormaHead* pDealData = (SNormaHead*)pData;
	float fPow;
	fPow = (float)pow((double)10,m_pCodeInfo->cPriceUnit);
	switch(pDealData->m_usType)
	{
	case STKMINUTEDATA:   
	case STKTRACEDATA:   
		{
			if(pDealData->m_usIndex != CPublic::Instance()->GetRequestIndex())
				return FALSE;

			m_listTraceInfo.clear();
			STraceUnit* pPriceUnit = (STraceUnit*)(pDealData+1);

			int nPostion=0;
			int nLastTime,nLastDate;
			nLastTime = pPriceUnit[pDealData->m_usSize-1].uiTime;
			nLastDate = pPriceUnit[pDealData->m_usSize-1].uiDate;
			int nCurrentDate,nCurrentTime;
			const StuSymbolData* pStockInfo = CPublic::Instance()->GetStockInfo(m_pCodeInfo);
			////////////////
			//pDealData->m_usSize = pStockInfo->nTotalTradeTime;
			//pPriceUnit = new STraceUnit[pDealData->m_usSize];
			//CTime tmCur = CTime::GetCurrentTime();
			//CTime tmEnd(tmCur.GetYear(),tmCur.GetMonth(),tmCur.GetDay(),5,2,0);
			//CTimeSpan tmSpan(60);
			//for (int x = pDealData->m_usSize -1; x >= 0;x--)
			//{
			//	tmEnd -= tmSpan;
			//	pPriceUnit[x].uiTime = atoi(tmEnd.Format(_T("%H%M")));
			//	pPriceUnit[x].uiDate = atoi(tmEnd.Format(_T("%Y%m%d")));
			//	pPriceUnit[x].uiPrice = 2000 + rand()%10;
			//}
			//nLastTime = pPriceUnit[pDealData->m_usSize-1].uiTime;
			//nLastDate = pPriceUnit[pDealData->m_usSize-1].uiDate;
			////////////////////////////////
			int iStockBeginTime = (NULL == pStockInfo)?0:pStockInfo->nBeginTime;
			iStockBeginTime = (iStockBeginTime/60)*100 + iStockBeginTime%60;
			//////////////////////
			for(int i = 0;i< pDealData->m_usSize;i++)
			{	
				nCurrentTime = pPriceUnit[i].uiTime;
				nCurrentDate = pPriceUnit[i].uiDate;
				if((nCurrentDate <(nLastDate-1)))continue;//超过一天
				if((nCurrentDate == (nLastDate-1)))
				{
					if(nCurrentTime<=nLastTime)continue;
					if((nLastTime<iStockBeginTime/*pStockInfo->nBeginTime*/)&&(nCurrentTime<iStockBeginTime/*pStockInfo->nBeginTime*/))
					{
						continue;
					}
				}
	
				if((nCurrentDate == nLastDate)&&(nCurrentTime<iStockBeginTime/*pStockInfo->nBeginTime*/)&&(nLastTime > iStockBeginTime/*pStockInfo->nBeginTime*/))continue;

				m_listTraceInfo.push_back(pPriceUnit[i]);
				if(CPublic::Instance()->IsMinuteDataMode())
				{
					//nPostion = GetHoriPoint(pPriceUnit[i].uiTime);
					//if((m_nHoriPoints <= nPostion)||(nPostion<0))continue;
					//m_pPriceValues[nPostion] = pPriceUnit[i].uiPrice;
					//////////////////	
					int iLastPos = GetHoriPoint(nLastTime);
					nPostion = GetHoriPoint(pPriceUnit[i].uiTime);
					if (iLastPos >= nPostion)
					{
						nPostion = GetHoriPoint(pPriceUnit[i].uiTime);
						if((m_nHoriPoints <= nPostion)||(nPostion<0))continue;
					    m_pPriceValues[nPostion] = pPriceUnit[i].uiPrice;
					}
					//////////////////////
				}
				else
				{
					m_pPriceValues[i] = pPriceUnit[i].uiPrice;
				}
			}
			if(CPublic::Instance()->IsMinuteDataMode())
			{
				if(nPostion<m_nHoriPoints)
					m_nValueNum = nPostion;	
				else
					m_nValueNum = m_nHoriPoints-1;
			}
			else
			{
				m_nValueNum = pDealData->m_usSize;
			}					
		}
		break;
	case PUSH_REALDATA:        //实时数据
		{
			//return FALSE;
			STraceUnit trace;
			float fNewPrice;
			StuRecvQuoteInfo* pRecvQuote1 = (StuRecvQuoteInfo*)(pDealData+1);	

			//int nRecord = 0;
			//int nHqstruLen = 0;
			//nHqstruLen = sizeof(StuRecvQuoteInfo);
			//nRecord = (dwDataSize-sizeof(SNormaHead))/nHqstruLen;
			for(int i=0;i<pDealData->m_usSize;i++)
			{
				StuRecvQuoteInfo* pRecvQuote = (StuRecvQuoteInfo*)(pRecvQuote1+i);

				CTime quotetime(pRecvQuote->lastTick);
				trace.uiDate = quotetime.GetYear()*10000+quotetime.GetMonth()*100+quotetime.GetDay();
				trace.uiTime = quotetime.GetHour()*10000+quotetime.GetMinute()*100+quotetime.GetSecond();
				trace.uiPrice = pRecvQuote->uiPrice;			

				fNewPrice = (float)(pRecvQuote->uiPrice/pow(10,(double)m_pCodeInfo->cPriceUnit));
				m_InfoTable.UpdateNewPrice(fNewPrice);
				if(CPublic::Instance()->IsMinuteDataMode())
				{
					STraceUnit oLastTrace;
					int nPostion;
					trace.uiTime/=100;
			
					if(m_listTraceInfo.size()>0)
					{
						oLastTrace = m_listTraceInfo.back();
						if(oLastTrace.uiTime >= trace.uiTime)//判断时间相等
						{		
							nPostion = GetHoriPoint(trace.uiTime);
							if((m_nHoriPoints <= nPostion)||(nPostion<0))continue;
							m_pPriceValues[nPostion]= pRecvQuote->uiPrice;//设置为当前价格
							m_listTraceInfo.pop_back();
							m_listTraceInfo.push_back(trace);
							break;
						}
					}

					if(m_nValueNum < m_nHoriPoints)
					{
						m_listTraceInfo.push_back(trace);
						nPostion = GetHoriPoint(trace.uiTime);
						if((m_nHoriPoints <= nPostion)||(nPostion<0))continue;
						m_pPriceValues[nPostion]= pRecvQuote->uiPrice;
						//m_nValueNum++;	
						m_nValueNum = nPostion;
					}
					else
					{
						if(m_nValueNum > 0)
						{
							list<STraceUnit>::iterator it1,it2;
							it1 = m_listTraceInfo.begin();
							for(it2 =m_listTraceInfo.begin();it2 != m_listTraceInfo.end();it2++ )
							{
								if(it1->uiTime/100 != it2->uiTime/100 )
									break;//小时数不等时结束
							}
							m_listTraceInfo.erase(it1,it2);
							memmove(m_pPriceValues,m_pPriceValues+60,(m_nValueNum-60)*sizeof(long));//移动数据
							m_nValueNum-=60;//向左移动一个小时
						}

						m_listTraceInfo.push_back(trace);
						nPostion = GetHoriPoint(trace.uiTime);	
						if((m_nHoriPoints <= nPostion)||(nPostion<0))continue;
						m_pPriceValues[nPostion]= pRecvQuote->uiPrice;
					}

				}
				else
				{
					if(m_nValueNum >= m_nHoriPoints)
					{//往前移动一位数据
						memmove(m_pPriceValues,m_pPriceValues+1,(m_nValueNum-1)*sizeof(long));//移动数据
						m_pPriceValues[m_nValueNum-1]= pRecvQuote->uiPrice;			
						m_listTraceInfo.pop_front();//删除最前侧的
					}
					else
					{
						m_pPriceValues[m_nValueNum]= pRecvQuote->uiPrice;
						m_nValueNum++;
					}
					m_listTraceInfo.push_back(trace);
				}
			}
		}
		break;
	default:
		{
		}
		break;
	}
	AdjustMinsData2Valide();
	UpdatePicData();
	return TRUE;
}

void CPriceVolObj::AdjustMinsData2Valide()
{
	for(int nCount = 0;nCount < m_nValueNum; nCount++)
	{//填充未处理数据
		if(m_pPriceValues[nCount] == 0)
		{
			if(nCount > 0)
			{
				m_pPriceValues[nCount] = m_pPriceValues[nCount-1];
			}
			//m_pVolumeValues[nCount] = 0;//没有成交价
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//分发数据给各个子图元及数据显示信息框
//////////////////////////////////////////////////////////////////////////
//函数功能: 更新obj中的子图形CPricePic、CVolPic中的数据
//函数参数: 无
//函数返回: void
void CPriceVolObj::UpdatePicData()
{	 
	CMulQuantitySequence seq;	

	seq.m_nSize =m_nValueNum;//当前数量
	seq.m_nStart = 0;
	seq.m_nSeqs = 4;
//	seq.m_lReference = m_fPrevClose;
	//成交价图元
	long *pSeqs[4];
	memset(pSeqs,0,sizeof(pSeqs));
	pSeqs[0] = m_pPriceValues;       //成交价	
    seq.m_ppValues = pSeqs;
	m_PricePic.UpdateData(&seq);
	//更新信息表格
	if((m_pPriceValues)&&(m_nCurrentStation >=0)&&(m_nCurrentStation<m_nValueNum))
	{
		UpdateInfoTip();
	}
}

//函数功能：更新图元显示信息框数据
//函数参数：无
//函数返回：无
void CPriceVolObj::UpdateInfoTip()
{
	if(m_nValueNum == 0)
		return;
	if(m_nCurrentStation>=m_nValueNum)
		return;
	
	SDynamicMinsUnit realMin;

	if(CPublic::Instance()->IsMinuteDataMode())
	{
		int nTime;
		CSinglockGuard lock(&m_lockData);
		nTime = GetHoriPointTime(m_nCurrentStation);//中午休息时间
		realMin.m_uiTime = nTime/60*100+nTime%60;
	
		if(m_pPriceValues[m_nCurrentStation]== 0x7fffffff)
		{
			realMin.m_fNewPrice = 0.000f;
		}
		else
		{
			realMin.m_fNewPrice = (float)m_pPriceValues[m_nCurrentStation];
			realMin.m_fNewPrice /= (double)pow(10,(double)m_pCodeInfo->cPriceUnit);
		}
	}
	else
	{
		CSinglockGuard lock(&m_lockData);
		list<STraceUnit>::iterator it;
		it = m_listTraceInfo.begin();
		for(int i=0;i<m_nCurrentStation;i++)
			it  ++;
		realMin.m_uiTime = it->uiTime;
		realMin.m_fNewPrice = (float)m_pPriceValues[m_nCurrentStation];
		realMin.m_fNewPrice /= (double)pow(10,(double)m_pCodeInfo->cPriceUnit);
	}



	int nStation;
	nStation = (m_nCurrentStation== 0)?m_nCurrentStation:(m_nCurrentStation-1);
	
	float fPrevPrice;
	fPrevPrice = m_pPriceValues[nStation];
	fPrevPrice /= (double)pow(10,(double)m_pCodeInfo->cPriceUnit);
	CMulQuantitySequence seq;
	seq.m_lReference = (long)&fPrevPrice;//昨收价
	seq.m_nSeqs = 1;
	seq.m_nStart = 0;
	long* pTemp = (long*)&realMin;
	seq.m_ppValues = (long**)&pTemp;
	m_InfoTable.UpdateData(&seq,0);
}

//////////////////////////////////////////////////////////////////////////
//处理键盘、鼠标、用户接口
//////////////////////////////////////////////////////////////////////////
//函数功能：响应键盘事件
//函数参数：nChar 键盘按键
//函数返回：无　
BOOL CPriceVolObj::OnChar(int nChar)
{
	// 针对鼠标移动时会无端触发mousemove事件的处理
// 	CPoint station(m_DrawRect.left,m_DrawRect.top);
// 	CPublic::Instance()->GetStockView()->ClientToScreen(&station);
// 	SetCursorPos(station.x,station.y);

	BOOL bResult;
	bResult = TRUE;
	switch(nChar)
	{
	case VK_ESCAPE:
		   {  
			   if(m_bDrawCross)
			   {
				   m_bDrawCross = FALSE;
				   return TransStation(CPoint(0,0));
			   }
			   else
				   bResult = FALSE;
			   //				m_bMouseMove = FALSE;				
			}
			break;
		case VK_LEFT:
			{
				m_nCurrentStation--;
				if(m_nCurrentStation<0)
					m_nCurrentStation = m_nValueNum-1;
				m_bDrawCross = TRUE;
			}
			break;
		case VK_RIGHT:
			{

				m_nCurrentStation++;
				if(m_nCurrentStation > m_nValueNum)
					m_nCurrentStation = 0;
				m_bDrawCross = TRUE;
			}
			break;
		case VK_HOME:
			m_nCurrentStation = 0;
			break;
		case VK_END:
			m_nCurrentStation = m_nValueNum-1;//最后一笔
			break;
		default:
			bResult = FALSE;
			break;
	}
	if(bResult)
	{//更新信息表格和各图元的画线位置
		m_bMouseMove = TRUE;
		m_PricePic.SetCurrentStation(m_nCurrentStation);
 		//更新信息条
		if((m_pPriceValues)&&(m_nCurrentStation >=0)&&(m_nCurrentStation<m_nValueNum))
		{
			UpdateInfoTip();
		}
	}
	return bResult;
}

//函数功能：响应用户ID消息
//函数参数：iSel - 选中菜单项
//函数返回：无
BOOL CPriceVolObj::OnUserCmd(UINT iSel)
{
	return TRUE;
} 

//函数功能:测试鼠标点击位置
//函数参数：点击位置
//函数返回：如果在本区域内返回TRUE,否则为false 
BOOL CPriceVolObj::HitTest(CPoint point)
{
	return m_DrawRect.PtInRect(point);
}

BOOL CPriceVolObj::MouseLBtnDbClick(CPoint point)
{
	OnChar(VK_RETURN);
	return TRUE;
}

//函数功能：响应鼠标按键消息
//函数参数：鼠标点击点
//函数返回：
BOOL CPriceVolObj::MouseLBtnDown(CPoint point)
{
	if(!m_bLeftButtonDown&&m_TipInfoRect.PtInRect(point))
	{//点击拖动信息表格
		m_bLeftButtonDown = TRUE;
		m_DrapPoint = point;
	}
	else
	{
		m_bDrawCross = TRUE;
	}
	return FALSE;
}

//函数功能：响应鼠标右键点击消息
//函数参数：lpWnd - 父窗口指针；point - 鼠标点击点
//函数返回：TRUE/FALSE
BOOL CPriceVolObj::MouseRBtnDown(LPVOID lpWnd,CPoint point)
{
	UINT uiMenuItemID; //选中菜单项ID值
	CWnd* pViewWnd = (CWnd*)lpWnd;
	if(m_PricePic.HitTest(point)) //点在分时图区域内
	{
//		uiMenuItemID = PopupRBtnMenu(pViewWnd,point,,0);
		CPoint scrnPoint;
		CMenu popMenu;

		if(NULL == pViewWnd) return FALSE; //防止指针空
		scrnPoint = point;

		popMenu.LoadMenu(IDR_RIGHTMENU); //装载菜单资源
		
		CMenu* pSubMenu;
		pSubMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_PRICEVOLOBJ);
		CLanguageInterface::Instance()->InitRightMenu(pSubMenu,POPSUBMENU_INDEX_PRICEVOLOBJ);
		//pSubMenu->CheckMenuItem(ID_TOOL_ADDSTOCK,MF_CHECKED | MF_BYCOMMAND);

		uiMenuItemID = pSubMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN
			,scrnPoint.x,scrnPoint.y,AfxGetMainWnd()); //弹出菜单
		popMenu.DestroyMenu(); //销毁菜单
	}
	return TRUE;
}

//函数功能：响应鼠标弹起消息
//函数参数：鼠标点击点
//函数返回：
BOOL CPriceVolObj::MouseLBtnUp(CPoint point)
{
	if(m_bLeftButtonDown)
	{//已经点击平且拖动状态
		m_bLeftButtonDown = FALSE;
		int iXPat =point.x-m_DrapPoint.x;
		int iYPat =point.y - m_DrapPoint.y;
		m_TipInfoRect.left += iXPat;
		m_TipInfoRect.right += iXPat;
		m_TipInfoRect.top += iYPat;
		m_TipInfoRect.bottom += iYPat;
		m_InfoTable.MoveTo(m_TipInfoRect);
		return TRUE;
	}
	return FALSE;
}

//函数功能：鼠标移动消息
//函数参数：鼠标点击点
//函数返回：
BOOL CPriceVolObj::MouseMove(CPoint point)
{
	// 鼠标在最左上端时不做处理，针对移动键盘时无端触发mousemove消息的处理
	if( (point.x == m_DrawRect.left) 
		&& (point.y == m_DrawRect.top))
	{
		return FALSE;
	}
	
	if(m_bLeftButtonDown)
	{//点击拖动信息表格
		int iXPat =point.x-m_DrapPoint.x;
		int iYPat =point.y - m_DrapPoint.y;
		m_TipInfoRect.left += iXPat;
		m_TipInfoRect.right += iXPat;
		m_TipInfoRect.top += iYPat;
		m_TipInfoRect.bottom += iYPat;
		m_InfoTable.MoveTo(m_TipInfoRect);
		m_DrapPoint = point;
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	//判断当前移动位置
	return TransStation(point);
}

//////////////////////////////////////////////////////////////////////////
//画图
//////////////////////////////////////////////////////////////////////////
//函数功能：画时间刻度
//函数参数：rRect时间区域
//函数返回：无
void CPriceVolObj::DrawTimeRect(CDC* pDC)
{
	int y = m_TimeRect.top;
	int x;
	char szBuf[40];
	if (m_nHoriPoints <= 2)
		return;	
	
	int nMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF timeAxeColor = CInterfaceManager::Instance()->GetSysColor(ID_COORDTEXT_COLOR);
	pDC->SetTextColor(timeAxeColor);
	CFont* TimeFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	CFont *pOldFont = pDC->SelectObject(TimeFont);
	int nMinute;

	for (int i = 0;i<=m_nHoriPoints; i+= 120)
	{
		x = m_TimeRect.left + m_TimeRect.Width()*i/(m_nHoriPoints-2);
		CSinglockGuard lock(&m_lockData);
		nMinute = GetHoriPointTime(i);
		sprintf(szBuf, "%02d:%02d", nMinute /60, nMinute %60);
		x -= pDC->GetTextExtent(CString(szBuf), strlen(szBuf)).cx / 2;
		pDC->TextOut(x, y, CString(szBuf), strlen(szBuf));
	}
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nMode);
}

void CPriceVolObj::DrawTip(CDC *pDC,CPoint point,CString strTip)
{
	CFont* pTextFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	CFont* OldFont = pDC->SelectObject(pTextFont);
	COLORREF crText = CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR);
	pDC->SetTextColor(crText);
	int nMode = pDC->SetBkMode(OPAQUE);
	
	CSize textSize = pDC->GetTextExtent(strTip);
	CRect ItemRect;
	ItemRect.left = point.x-1;
	ItemRect.top = point.y;
	ItemRect.right = ItemRect.left +textSize.cx+1;
	ItemRect.bottom = ItemRect.top + textSize.cy+2;//得到区域
	
	pDC->FillSolidRect(ItemRect,RGB(255,255,175));//画出外框
	pDC->DrawText(strTip,ItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//画出文字
	pDC->SelectObject(OldFont);
	pDC->SetBkMode(nMode);
}

//函数功能：在指定设备画图
//函数参数：pDC画图设备
//函数返回：
BOOL CPriceVolObj::Draw(CDC* pDC)
{
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);

	if(!(m_nShowMode&NOTIMERULER))
		DrawTimeRect(pDC);//画出时间区间
	if(!(m_nShowMode&NOTITLEINFO))
		DrawTitleInfo(pDC);//画抬头信息

	m_PricePic.Draw(pDC);			//成交价区域
	//画信息条区域
 	if(m_bDrawCross)
	{
		m_InfoTable.Draw(pDC);
		DrawCrossCursor(pDC);
	}
	pDC->SetBkMode(nOldBkMode);
	return TRUE;	
}

void CPriceVolObj::DrawCrossCursor(CDC *pDC)
{
	if(!m_pCodeInfo||(m_pCodeInfo->cPriceUnit<0))return;
	//////////////////////////////////////////////////////////////////////////
	//画出左侧价格或者其他标签
	CRect ItemRect;
	CSize textSize;
	COLORREF crText = CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR);
	COLORREF crLine = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	COLORREF crFill = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	CBrush FillBrush(crFill);
	CPen  BordLine(PS_SOLID,1,crLine);
	CBrush* OldBrush = pDC->SelectObject(&FillBrush);
	CPen* OldPen = pDC->SelectObject(&BordLine);
	int nOldMode =pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);
	CFont * textFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	CFont * OldFont = pDC->SelectObject(textFont);

	//////////////////////////////////////////////////////////////////////////
	//检索鼠标对应位置价格
	CString szText;
	float fValue;
	CString strFormat;
	strFormat.Format(_T("%%.%df"),m_pCodeInfo->cPriceUnit);
	float fPrice;
	fPrice = m_nCurrentPrice;
	fPrice /= pow((long)10,(double)m_pCodeInfo->cPriceUnit);
	szText.Format(strFormat,fPrice);
	textSize = pDC->GetTextExtent(szText);
	int nPos = m_PricePic.GetStationFromPrice(m_nCurrentPrice);
	ItemRect.top = nPos-textSize.cy/2;
	ItemRect.bottom =ItemRect.top +textSize.cy;
	//ItemRect.right = m_DrawRect.left + VERTBANKSIZE-2;
	ItemRect.right = m_PriceRect.left-2; 
	ItemRect.left = ItemRect.right  - textSize.cx-6;
	if(ItemRect.right>m_DrawRect.right)
	{//纠正位置
		ItemRect.right = m_DrawRect.right;
		ItemRect.left = ItemRect.right - textSize.cx-2;
	}
	pDC->Rectangle(ItemRect);
	pDC->FillSolidRect(ItemRect,RGB(0,0,255));
	pDC->DrawText(szText,ItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);			

	//////////////////////////////////////////////////////////////////////////
	//画出下侧时间标签
	int nDate;
	nDate=GetHoriPointTime(m_nCurrentStation);
	szText.Format(_T("%02d:%02d"),nDate/60,nDate%60);
	ItemRect = m_TimeRect;
	int x;
	x = m_TimeRect.left + m_TimeRect.Width()*m_nCurrentStation/(m_nHoriPoints-2);
	ItemRect.left = x;
	textSize = pDC->GetTextExtent(szText);
	ItemRect.right = ItemRect.left+ textSize.cx+2;
	if(ItemRect.right>m_TimeRect.right)
	{//纠正位置
		ItemRect.right = m_TimeRect.right;
		ItemRect.left = ItemRect.right - textSize.cx-2;
	}
	pDC->Rectangle(ItemRect);
	pDC->FillSolidRect(ItemRect,RGB(0,0,255));
	pDC->DrawText(szText,ItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(OldFont);
	pDC->SelectObject(OldBrush);
	pDC->SelectObject(OldPen);
	pDC->SetBkMode(nOldMode);
}

//画出抬头信息
void CPriceVolObj::DrawTitleInfo(CDC *pDC)
{
	if(NULL == m_pCodeInfo)
		return;

	if( m_nValueNum < 1)
	{
		return;
	}

	CFont* pTextFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	CFont * pOldFont = pDC->SelectObject(pTextFont);
	COLORREF crTitle = CInterfaceManager::Instance()->GetSysColor(ID_INFOLETTER_COLOR);
	COLORREF crRise  = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	COLORREF crDown  = CInterfaceManager::Instance()->GetSysColor(ID_DOWN_COLOR);
	COLORREF crEqual  = CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR);
	
	pDC->SetTextColor(crTitle);

	CString szText;
	CSize TextSize;
	int nWidth;	
	CString strStockName;
	strStockName = m_pCodeInfo->szCode;

	const StuSymbolData* pSymbol;
	pSymbol = CPublic::Instance()->GetStockInfo(m_pCodeInfo);
	if(NULL != pSymbol)
	{
		strStockName += pSymbol->szName;
	}

	CRect ItemRect = m_TitleRect;

	long lPow;
	CString szFormat;
	lPow = CPublic::Instance()->GetDecimal(m_pCodeInfo);
	szFormat.Format(_T("%%.%df"),CPublic::Instance()->GetDecimal(m_pCodeInfo));


	if(m_nShowMode&TITLEINDEXLAST)
	{
		szText.Format(_T("beijingprice:%.5f"),m_pPriceValues[m_nValueNum-1]);
		TextSize = pDC->GetTextExtent(szText);
		nWidth = (m_TitleRect.Width()/2 -TextSize.cx)/4;
		nWidth += 4;

		szText = strStockName;		
		TextSize = pDC->GetTextExtent(szText); 

		ItemRect.right = ItemRect.left+ TextSize.cx;

		pDC->DrawText(szText,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		
		szText = _T("   volu:");
		TextSize = pDC->GetTextExtent(szText);
		ItemRect.left = ItemRect.right+nWidth;
		ItemRect.right = ItemRect.left+ TextSize.cx;
		pDC->DrawText(szText,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		float fPrice;
		fPrice = (float)m_pPriceValues[m_nValueNum-1];
// 		if(fPrice >m_pCodeInfo->m_dbLastClose)
// 			pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR));
// 		else
// 			pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_DOWN_COLOR));
		fPrice /= lPow;
		szText.Format(szFormat,fPrice);
		TextSize = pDC->GetTextExtent(szText); 
		ItemRect.left = ItemRect.right;
		ItemRect.right = ItemRect.left+ TextSize.cx;
		pDC->DrawText(szText,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	}
	else if(m_nShowMode&TILELASTPRICE)
	{
		szText.Format(_T("beijing1pric:%.5f"),m_pPriceValues[m_nValueNum-1]);
		TextSize = pDC->GetTextExtent(szText);
		nWidth = (m_TitleRect.Width() -TextSize.cx)/4;
		if(nWidth<=0)	nWidth = 4;//最小间距
		
		pDC->SetTextColor(crTitle);
		szText = strStockName;    	
		TextSize = pDC->GetTextExtent(szText); 		
		ItemRect.right = ItemRect.left+ TextSize.cx;		
		pDC->DrawText(szText,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		
		float fPrice;
		fPrice = (float)m_pPriceValues[m_nValueNum-1];
		szText = _T("volu:");
		pDC->SetTextColor(crTitle);
		TextSize = pDC->GetTextExtent(szText); 
		ItemRect.left = ItemRect.right+nWidth;
		ItemRect.right = ItemRect.left+ TextSize.cx;
		if(ItemRect.right >m_TitleRect.right)return;
		pDC->DrawText(szText,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);		
	}
	else if( m_nShowMode & TITLEPRICEDAY) // 显示走势日期
	{
		szText.Format(_T("%d"),m_iDate);
		TextSize = pDC->GetTextExtent(szText);
		ItemRect.left = ItemRect.right - (TextSize.cx+1);
		pDC->DrawText(szText,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		if(CPublic::Instance()->IsMinuteDataMode())
		{
			szText = CLanguageInterface::Instance()->GetStr(20001);
		}
		else
		{
			szText = CLanguageInterface::Instance()->GetStr(20003);
		}
		TextSize = pDC->GetTextExtent(szText);
		ItemRect.right = ItemRect.left+ TextSize.cx;
		pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_GRIDFIXED_COLOR));
		pDC->DrawText(szText,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
		szText = strStockName;
		TextSize = pDC->GetTextExtent(szText); 
		ItemRect.left = ItemRect.right+5;
		ItemRect.right = ItemRect.left+ TextSize.cx;
		pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR));
		pDC->DrawText(szText,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	}
	pDC->SelectObject(pOldFont);
}

BOOL CPriceVolObj::TransStation(CPoint point)
{
	int iStation;
	iStation = -1;
	long lPrice = 0;
	if(m_PriceRect.PtInRect(point))
	{
		iStation = m_PricePic.GetCurrentStation(point);
	}
	m_nCurrentPrice = m_PricePic.GetPriceFromStation(point.y);


	//////////////////////////////////////////////////////////////////////////
	//传送给各图元并且通知信息表格
	if(m_nCurrentStation != iStation)
	{
		m_bMouseMove = TRUE;
		m_PricePic.SetCurrentStation(iStation);
		m_PricePic.SetCurrentPrice(m_nCurrentPrice);
		m_nCurrentStation = iStation;//当前位置
		
		//////////////////////////////////////////////////////////////////////////
		//恢复数据
		if((m_pPriceValues)&&(m_nCurrentStation >=0)&&(m_nCurrentStation<m_nValueNum))
		{
			UpdateInfoTip();
		}
		return TRUE;
	}
	return FALSE;
}

int  CPriceVolObj::GetHoriPointTime(int nPos)
{
	const StuSymbolData* pStockInfo = CPublic::Instance()->GetStockInfo(m_pCodeInfo);
	int nFirstTime;
	if(NULL == pStockInfo)
	{
		nFirstTime = 0;
	}
	else
	{
		nFirstTime = pStockInfo->nBeginTime;
	}
	
	int nRet;
	nRet = nFirstTime+nPos;
	if(nRet>=1440)nRet = nRet-1440;
	return nRet;
}

int  CPriceVolObj::GetHoriPoint(int nTime)
{
	const StuSymbolData* pStockInfo = CPublic::Instance()->GetStockInfo(m_pCodeInfo);
	int nFirstTime;
	if(NULL == pStockInfo)
	{
		nFirstTime = 0;
	}
	else
	{
		nFirstTime = pStockInfo->nBeginTime;
	}
	if(nFirstTime>1440||nFirstTime<0)
	{
		nFirstTime = 0;
	}
//	if(nTime<nFirstTime)return -1;

	int nSpend;
	nSpend = nTime%100;
	nTime = nTime/100*60+nSpend;
	int nSpendTime;
	nSpendTime = nTime-nFirstTime;
	if(nSpendTime<0)nSpendTime += 1440;//防止过夜后越界
	if(nSpendTime>=1440)nSpendTime -= 1440;//预防
	return nSpendTime;
}