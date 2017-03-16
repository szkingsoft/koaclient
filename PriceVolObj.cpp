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
//���仭ͼ���估���ݳ�ʼ��
//////////////////////////////////////////////////////////////////////////
//�������ܣ����·���������
//������������
//�������أ�
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
	//���ݼ���ָ�����ͣ�ȷ���ֳɼ�������
	CRect ItemRect;
	ItemRect = m_DrawRect;
	if(!(m_nShowMode&AUTOFITLEFTSIZE))
	{//�Զ���Ӧ��߾�
		ItemRect.left += VERTBANKSIZE;
	}
	if(!(m_nShowMode&NORIGHTRAWRULER))
	{
		ItemRect.right -= VERTBANKSIZE;
	}
	//////////////////////////////////////////////////////////////////////////
	//�õ��߶�
//	float fHeight;
	if(!(m_nShowMode&NOTIMERULER))
	{
		ItemRect.bottom = m_DrawRect.bottom- HORIBANKSIZE;//�²�����
		//����ʱ��̶�����
		m_TimeRect = ItemRect;//�õ����
		m_TimeRect.top =m_DrawRect.bottom- HORIBANKSIZE;
		m_TimeRect.bottom = m_DrawRect.bottom;
	}
	if(!(m_nShowMode&NOTITLEINFO))
	{
		ItemRect.top += HORIBANKSIZE;//�ϲ�����
		//����̧ͷ����
		m_TitleRect = ItemRect;
		m_TitleRect.top = m_DrawRect.top;
		m_TitleRect.bottom = m_TitleRect.top + HORIBANKSIZE;
	}
	else
	{
		ItemRect.top += 10;//�����ϲ�����
	}
//	fHeight = (float);

	//�ɽ��۸�����
	m_PriceRect = ItemRect;//�õ�ϵͳ��ȸ߶�
//	m_PriceRect.bottom = ItemRect.top + fHeight;
	m_PricePic.MoveTo(m_PriceRect);//�۸�����

	
	//ʵʱ����ָ��ͼԪ����
	m_TechRect = ItemRect;
	m_TechRect.top = m_VolumnRect.bottom;
	m_TechRect.bottom = m_DrawRect.bottom - HORIBANKSIZE;
	
	//������Ϣ����ͼ����
	m_TipInfoRect.left =m_DrawRect.left;
	m_TipInfoRect.right = m_DrawRect.left+VERTBANKSIZE-2;
	m_TipInfoRect.top = m_DrawRect.top+HORIBANKSIZE;
	m_TipInfoRect.bottom = m_DrawRect.top+18*4+10/*120+HORIBANKSIZE*/;

	m_InfoTable.MoveTo(m_TipInfoRect);

}

//////////////////////////////////////////////////////////////////////////
//���ù�Ʊ��Ϣ����ͼ���͵�
//////////////////////////////////////////////////////////////////////////
//�������ܣ�����֤ȯ��Ϣ
//����������pCodeInfo֤ȯ��Ϣ
//�������أ���
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

//�������ܣ����ݽ���ʱ��������ͼԪ�������ڴ�
//������������
//�������أ���
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
		nHoriPoints = pStockInfo->nTotalTradeTime;//�õ��ܽ���ʱ��
	}
	if(nHoriPoints<0)return;
	m_PricePic.SetTradeTime(nHoriPoints/60);
	nHoriPoints += 2;
	int nDecimal = CPublic::Instance()->GetDecimal(m_pCodeInfo);
	int nPow = (int)pow(10,(double)CPublic::Instance()->GetDecimal(m_pCodeInfo));
	m_PricePic.SetDecimal(nDecimal,nPow);			//�ɽ���
	if(m_nHoriPoints != nHoriPoints) //�ܽ���ʱ����ͬ�����������·����ڴ�
	{
		m_nHoriPoints = nHoriPoints;
		m_PricePic.SetHoriPoint(m_nHoriPoints);	   //�ɽ���
 		//���仺����
		if (m_pPriceValues)	delete []m_pPriceValues;
		m_pPriceValues  = new long[m_nHoriPoints];
	}
	for(int i =0;i<m_nHoriPoints;i++)
	{
		m_pPriceValues[i]= 0x7fffffff;
	}
//	ZeroMemory(m_pPriceValues,m_nHoriPoints*sizeof(long));
}	

//�������ܣ����û�ͼģʽ
//����������0Ϊ���ɽ�����1Ϊ����������
//			2Ϊ����ͼ    3ΪADLָ��
//�������أ�
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

//�������ܣ����û�ͼģʽ��
//����������TRUEΪ��ʱ����ͼ�� FALSEΪ���ͬ��
//�������أ�
void CPriceVolObj::SetDisplayMode(DWORD nShowMode)
{
	m_nShowMode = nShowMode;
	m_PricePic.SetDisplayMode(nShowMode);
}

//////////////////////////////////////////////////////////////////////////
//���󡢽��ܴ�������
//////////////////////////////////////////////////////////////////////////
//��������: �Ƿ��������codeInfo��������
//��������: codeInfo��Ҫ�������Ʒ
//��������: void
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

//��������: �����һ�ν��뵱ǰ��ͼʱ����Ʒ����������
//��������: ��
//��������: void
void CPriceVolObj::RequestData()
{
	if(m_pCodeInfo == NULL)
		return;
	// ���������Ϣ����
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
	// ���������Ϣ����
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

//�������ܣ������������
//����������pData ָ�����ݻ�������ָ��
//          dwDataSize ��������
//�������أ�����ڱ������ڷ���TRUE,����Ϊfalse 
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
				if((nCurrentDate <(nLastDate-1)))continue;//����һ��
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
	case PUSH_REALDATA:        //ʵʱ����
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
						if(oLastTrace.uiTime >= trace.uiTime)//�ж�ʱ�����
						{		
							nPostion = GetHoriPoint(trace.uiTime);
							if((m_nHoriPoints <= nPostion)||(nPostion<0))continue;
							m_pPriceValues[nPostion]= pRecvQuote->uiPrice;//����Ϊ��ǰ�۸�
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
									break;//Сʱ������ʱ����
							}
							m_listTraceInfo.erase(it1,it2);
							memmove(m_pPriceValues,m_pPriceValues+60,(m_nValueNum-60)*sizeof(long));//�ƶ�����
							m_nValueNum-=60;//�����ƶ�һ��Сʱ
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
					{//��ǰ�ƶ�һλ����
						memmove(m_pPriceValues,m_pPriceValues+1,(m_nValueNum-1)*sizeof(long));//�ƶ�����
						m_pPriceValues[m_nValueNum-1]= pRecvQuote->uiPrice;			
						m_listTraceInfo.pop_front();//ɾ����ǰ���
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
	{//���δ��������
		if(m_pPriceValues[nCount] == 0)
		{
			if(nCount > 0)
			{
				m_pPriceValues[nCount] = m_pPriceValues[nCount-1];
			}
			//m_pVolumeValues[nCount] = 0;//û�гɽ���
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//�ַ����ݸ�������ͼԪ��������ʾ��Ϣ��
//////////////////////////////////////////////////////////////////////////
//��������: ����obj�е���ͼ��CPricePic��CVolPic�е�����
//��������: ��
//��������: void
void CPriceVolObj::UpdatePicData()
{	 
	CMulQuantitySequence seq;	

	seq.m_nSize =m_nValueNum;//��ǰ����
	seq.m_nStart = 0;
	seq.m_nSeqs = 4;
//	seq.m_lReference = m_fPrevClose;
	//�ɽ���ͼԪ
	long *pSeqs[4];
	memset(pSeqs,0,sizeof(pSeqs));
	pSeqs[0] = m_pPriceValues;       //�ɽ���	
    seq.m_ppValues = pSeqs;
	m_PricePic.UpdateData(&seq);
	//������Ϣ���
	if((m_pPriceValues)&&(m_nCurrentStation >=0)&&(m_nCurrentStation<m_nValueNum))
	{
		UpdateInfoTip();
	}
}

//�������ܣ�����ͼԪ��ʾ��Ϣ������
//������������
//�������أ���
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
		nTime = GetHoriPointTime(m_nCurrentStation);//������Ϣʱ��
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
	seq.m_lReference = (long)&fPrevPrice;//���ռ�
	seq.m_nSeqs = 1;
	seq.m_nStart = 0;
	long* pTemp = (long*)&realMin;
	seq.m_ppValues = (long**)&pTemp;
	m_InfoTable.UpdateData(&seq,0);
}

//////////////////////////////////////////////////////////////////////////
//������̡���ꡢ�û��ӿ�
//////////////////////////////////////////////////////////////////////////
//�������ܣ���Ӧ�����¼�
//����������nChar ���̰���
//�������أ��ޡ�
BOOL CPriceVolObj::OnChar(int nChar)
{
	// �������ƶ�ʱ���޶˴���mousemove�¼��Ĵ���
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
			m_nCurrentStation = m_nValueNum-1;//���һ��
			break;
		default:
			bResult = FALSE;
			break;
	}
	if(bResult)
	{//������Ϣ���͸�ͼԪ�Ļ���λ��
		m_bMouseMove = TRUE;
		m_PricePic.SetCurrentStation(m_nCurrentStation);
 		//������Ϣ��
		if((m_pPriceValues)&&(m_nCurrentStation >=0)&&(m_nCurrentStation<m_nValueNum))
		{
			UpdateInfoTip();
		}
	}
	return bResult;
}

//�������ܣ���Ӧ�û�ID��Ϣ
//����������iSel - ѡ�в˵���
//�������أ���
BOOL CPriceVolObj::OnUserCmd(UINT iSel)
{
	return TRUE;
} 

//��������:���������λ��
//�������������λ��
//�������أ�����ڱ������ڷ���TRUE,����Ϊfalse 
BOOL CPriceVolObj::HitTest(CPoint point)
{
	return m_DrawRect.PtInRect(point);
}

BOOL CPriceVolObj::MouseLBtnDbClick(CPoint point)
{
	OnChar(VK_RETURN);
	return TRUE;
}

//�������ܣ���Ӧ��갴����Ϣ
//�����������������
//�������أ�
BOOL CPriceVolObj::MouseLBtnDown(CPoint point)
{
	if(!m_bLeftButtonDown&&m_TipInfoRect.PtInRect(point))
	{//����϶���Ϣ���
		m_bLeftButtonDown = TRUE;
		m_DrapPoint = point;
	}
	else
	{
		m_bDrawCross = TRUE;
	}
	return FALSE;
}

//�������ܣ���Ӧ����Ҽ������Ϣ
//����������lpWnd - ������ָ�룻point - �������
//�������أ�TRUE/FALSE
BOOL CPriceVolObj::MouseRBtnDown(LPVOID lpWnd,CPoint point)
{
	UINT uiMenuItemID; //ѡ�в˵���IDֵ
	CWnd* pViewWnd = (CWnd*)lpWnd;
	if(m_PricePic.HitTest(point)) //���ڷ�ʱͼ������
	{
//		uiMenuItemID = PopupRBtnMenu(pViewWnd,point,,0);
		CPoint scrnPoint;
		CMenu popMenu;

		if(NULL == pViewWnd) return FALSE; //��ָֹ���
		scrnPoint = point;

		popMenu.LoadMenu(IDR_RIGHTMENU); //װ�ز˵���Դ
		
		CMenu* pSubMenu;
		pSubMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_PRICEVOLOBJ);
		CLanguageInterface::Instance()->InitRightMenu(pSubMenu,POPSUBMENU_INDEX_PRICEVOLOBJ);
		//pSubMenu->CheckMenuItem(ID_TOOL_ADDSTOCK,MF_CHECKED | MF_BYCOMMAND);

		uiMenuItemID = pSubMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN
			,scrnPoint.x,scrnPoint.y,AfxGetMainWnd()); //�����˵�
		popMenu.DestroyMenu(); //���ٲ˵�
	}
	return TRUE;
}

//�������ܣ���Ӧ��굯����Ϣ
//�����������������
//�������أ�
BOOL CPriceVolObj::MouseLBtnUp(CPoint point)
{
	if(m_bLeftButtonDown)
	{//�Ѿ����ƽ���϶�״̬
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

//�������ܣ�����ƶ���Ϣ
//�����������������
//�������أ�
BOOL CPriceVolObj::MouseMove(CPoint point)
{
	// ����������϶�ʱ������������ƶ�����ʱ�޶˴���mousemove��Ϣ�Ĵ���
	if( (point.x == m_DrawRect.left) 
		&& (point.y == m_DrawRect.top))
	{
		return FALSE;
	}
	
	if(m_bLeftButtonDown)
	{//����϶���Ϣ���
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
	//�жϵ�ǰ�ƶ�λ��
	return TransStation(point);
}

//////////////////////////////////////////////////////////////////////////
//��ͼ
//////////////////////////////////////////////////////////////////////////
//�������ܣ���ʱ��̶�
//����������rRectʱ������
//�������أ���
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
	ItemRect.bottom = ItemRect.top + textSize.cy+2;//�õ�����
	
	pDC->FillSolidRect(ItemRect,RGB(255,255,175));//�������
	pDC->DrawText(strTip,ItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//��������
	pDC->SelectObject(OldFont);
	pDC->SetBkMode(nMode);
}

//�������ܣ���ָ���豸��ͼ
//����������pDC��ͼ�豸
//�������أ�
BOOL CPriceVolObj::Draw(CDC* pDC)
{
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);

	if(!(m_nShowMode&NOTIMERULER))
		DrawTimeRect(pDC);//����ʱ������
	if(!(m_nShowMode&NOTITLEINFO))
		DrawTitleInfo(pDC);//��̧ͷ��Ϣ

	m_PricePic.Draw(pDC);			//�ɽ�������
	//����Ϣ������
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
	//�������۸����������ǩ
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
	//��������Ӧλ�ü۸�
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
	{//����λ��
		ItemRect.right = m_DrawRect.right;
		ItemRect.left = ItemRect.right - textSize.cx-2;
	}
	pDC->Rectangle(ItemRect);
	pDC->FillSolidRect(ItemRect,RGB(0,0,255));
	pDC->DrawText(szText,ItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);			

	//////////////////////////////////////////////////////////////////////////
	//�����²�ʱ���ǩ
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
	{//����λ��
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

//����̧ͷ��Ϣ
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
		if(nWidth<=0)	nWidth = 4;//��С���
		
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
	else if( m_nShowMode & TITLEPRICEDAY) // ��ʾ��������
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
	//���͸���ͼԪ����֪ͨ��Ϣ���
	if(m_nCurrentStation != iStation)
	{
		m_bMouseMove = TRUE;
		m_PricePic.SetCurrentStation(iStation);
		m_PricePic.SetCurrentPrice(m_nCurrentPrice);
		m_nCurrentStation = iStation;//��ǰλ��
		
		//////////////////////////////////////////////////////////////////////////
		//�ָ�����
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
	if(nSpendTime<0)nSpendTime += 1440;//��ֹ��ҹ��Խ��
	if(nSpendTime>=1440)nSpendTime -= 1440;//Ԥ��
	return nSpendTime;
}