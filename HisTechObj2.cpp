// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "koaclient.h"
#include "HisTechObj.h"
#include "timevalue.h"
#include "IniFile.h"
#include "techanalysiskline.h"
#include "mainfrm.h"


CHisTechObj::CHisTechObj()
{
	m_pRecvKUnit = NULL;
	m_pCodeInfo = NULL;
	m_pDoneKunit = NULL;
	m_pTranslateKUnit = NULL;
	m_bTranslateIsPoint = FALSE;
	
	m_pHisCurvePic = NULL;
	m_pHoriPoint = NULL;

	m_bChipDistribPic = FALSE;

	m_nCurrentPicNum = 2;
	//��ȡӦ�ó���·��
	m_strIndexParamPath = CPublic::Instance()->GetSysPath();
	m_strIndexParamPath += _T("koa.ini");
	
	LoadIndexStatusParam();
	
	m_pHoriPoint = NULL;//ˮƽ��
	m_nHoriPoint = 0;
	m_nMaxParameret = 0;
	m_nParameret =0;

	m_nLeftMoveRequestStatus = NORMALREQUEST;
	
	m_dwShowStyle = 0;
	m_pHisCurvePic[0].SetShowStyle(m_dwShowStyle);//��ʾ��Ϣ����
	
	m_pDistractStatTable = NULL;

	m_nDistractStatState = ENDSTAT;//��ͳ��״̬
//	m_bRequestedData = FALSE;
	m_nCurrentTechIndex = -1;

	InitCircleType();
	
	m_bDrawCross = FALSE;
//	m_bTranslateIsPoint = TRUE;
	m_sRequestStatus = 0;		 //��������״̬���ֵ�һ�κ͵�һ���Ժ�����	
	m_sLastSize = 0;			     //���һ������K�����ݸ���
	
	m_nValueNum = 0;			     //�Ѵӷ����������������ʷ���ݸ���	

	//��ʷ��ͼ״̬�ı����
	////////////////////////////////////////////////////
	m_nDataBegin = 0;			 //���ݿ�ʼλ
	m_nDataEnd = 0;				 //���ݽ���λ
	m_nDataOffset = 0;			 //����ƫ����
	m_nKUnitWidth = 0;			 //��ǰK�߿��
	m_nRecsOfUnit = 1;			 //һ��K���ϰ�����K������

	//��ʷ������ʾ����////////////////////////////////////////////////////////
	ZeroMemory(&m_TimeRect,sizeof(CRect));	//ʱ������
	m_nTotalPicNum = 2;		      //ͼ������
	ZeroMemory(&m_TipInfoRect,sizeof(CRect));  //��Ϣ������
	ZeroMemory(&m_KLineRect,sizeof(CRect));			      //K������
	m_bMouseMove = FALSE;			  //����ƶ�	
	m_bLeftButtonDown = FALSE;	      //����������	
	ZeroMemory(&m_DrapPoint,sizeof(CPoint));				  //�϶���
	m_nVertStation = -1;
	m_nHoriStation = -1;			  //��ǰ���ˮƽλ��
	m_nNowShowDate = -1;//�õ�����

	m_nFirstClickState = -1;//��һ�ε��λ��
	m_nSecondClickState = -1;//�ڶ��ε��λ��

	memset(m_rectSelect,0,sizeof(m_rectSelect));
	m_bDrawSelect = FALSE;
	m_nDrawState = NULLDRAW;
}

CHisTechObj::~CHisTechObj()
{
	DestoryData();
	
	//////////////////////////////////////////////////////////////////////////
	//ɾ��ͼ��
	if(m_pHisCurvePic)delete []m_pHisCurvePic;
	if(m_pDistractStatTable)delete m_pDistractStatTable;
}

//�������ܣ��������
//������������
//�������أ���
void CHisTechObj::DestoryData()
{
	//ɾ������
	if(m_pRecvKUnit)
	{
		delete []m_pRecvKUnit;
		m_pRecvKUnit = NULL;
	}
	if(m_pDoneKunit)
	{
		delete []m_pDoneKunit;
		m_pDoneKunit = NULL;
	}
	if(m_pHoriPoint)
	{
		delete []m_pHoriPoint;
		m_pHoriPoint = NULL;
	}	
	if(m_bTranslateIsPoint&&m_pTranslateKUnit)
	{
		delete []m_pTranslateKUnit;
		m_pTranslateKUnit = NULL;
		m_bTranslateIsPoint = FALSE;
	}
	ResetTechData();
}

///////////////////////////////////////////////////////////////////////////////
//�������ܣ������������С
//������������
//�������أ�void
void CHisTechObj::RecalSubObjSize()
{ 
	if(m_DrawRect.IsRectEmpty())return;
	CRect KLineRect,VolumneRect,TechRect;
	
	int nTechIndexPicHeight = (m_DrawRect.Height()- 2*HORIBANKSIZE);
	nTechIndexPicHeight /= (m_nCurrentPicNum+2);
	
	KLineRect = m_DrawRect;//�ɽ�������
	//////////////////////////////////////////////////
	//modify by david gong at 2013/09/05
	//�޸����������Ҳ�
//	KLineRect.left += VERTBANKSIZE_HISOBJ;//���������������	
	KLineRect.right -= VERTBANKSIZE_HISOBJ;//���������������
	/////////////////////////////////////////////////
	KLineRect.bottom = m_DrawRect.bottom - (m_nCurrentPicNum-1) * nTechIndexPicHeight - HORIBANKSIZE;
	m_pHisCurvePic[0].MoveTo(KLineRect);

	m_KLineRect = KLineRect;

	if(m_nCurrentPicNum >= 2)
	{	
		TechRect= KLineRect;//������ͼ����
		for(int i = 0;i<m_nCurrentPicNum -1;i++)
		{
			TechRect.top = KLineRect.bottom-1+i*nTechIndexPicHeight;
			TechRect.bottom = KLineRect.bottom + (i+1)*nTechIndexPicHeight;
			m_pHisCurvePic[1+i].MoveTo(TechRect);
		}
	}
	m_TimeRect = KLineRect;					 //ʱ������
	m_TimeRect.top = m_DrawRect.bottom - HORIBANKSIZE;
	m_TimeRect.bottom = m_DrawRect.bottom;
	m_TimeRect.left += 3;
	m_TimeRect.right -= 3;

	//////////////////////////////////////////////////////////////////////////
	//modify by david gong at 2012/09/15
	//�ƶ�Ĭ�ϵ��Ҳ�
	m_TipInfoRect.left =m_DrawRect.left+10;
	m_TipInfoRect.right = m_DrawRect.left+VERTBANKSIZE_HISOBJ;
// 	m_TipInfoRect.right =m_DrawRect.right;
// 	m_TipInfoRect.left = m_DrawRect.right - VERTBANKSIZE_HISOBJ;
	//////////////////////////////////////////////////////////////////////////
	m_TipInfoRect.top = m_DrawRect.top+16;

	// ��Ϊȥ������ʾ������������Ҫ����Ϣ���ĸ߶ȵ�С
	m_TipInfoRect.bottom = m_DrawRect.top+16*16+4+16;

	//������Ϣ����ͼ����
	m_InfoTable.MoveTo(m_TipInfoRect);
	m_InfoTable.Init(CInfoTable::DAYDATAINFO);//����Ϊ��������״̬
	 //����K�߿��
	if(m_nHoriPoint)
	{
		SetHoriPoint();//���ø���λ��
	}

	TransKDataToPic();//���´�������
}

//�������ܣ�������Ʒ��Ϣ
//����������pCodeInfo ��Ʒ��Ϣ
//�������أ�void��
void CHisTechObj::SetStockInfo(const SCodeInfo* pCodeInfo)
{
 	m_bDrawCross = FALSE;
	m_nMemoryStation = -1;//add at 11-03


	for(int i = 0;i<m_nCurrentPicNum;i++)
	{
		m_pHisCurvePic[i].RemoveAllDrawLine();
		m_pHisCurvePic[i].SetCurrentStation(-1);
	}
	//////////////////////////////////////////////////////////////////////////
	//��һ���������ݣ����ԭ����
	DestoryData();//�������
	if(pCodeInfo)
	{
		//���ù�Ʊ��Ϣ
		m_InfoTable.SetStockInfo(pCodeInfo);
		m_pCodeInfo = pCodeInfo;//����֤ȯ����
	}
	//////////////////////////////////////////////////////////////////////////
    //�ӷ������յ������ݸ�������ǰ��Ļ��ʾ�����ݸ�����Ϊ0
    //��������������Ϊ��һ��
	m_sRequestStatus = TP_FIRST;
	m_nValueNum = 0;
	//K�߿����ΪĬ�Ͽ����ʾ
	m_nKUnitWidth = 2;
	m_nRecsOfUnit = 1;	
	//�����궯�����
	m_nVertStation = -1;
	m_nHoriStation = -1;	
	m_bMouseMove = FALSE;
	m_bLeftButtonDown = FALSE;
	m_nNowShowDate = -1;

	//////////////////////////////////////////////////////////////////////////
	//�������ݿ�ʼ����λ��
	//����������  
	int nCount;
	int nMax;
	int nMaxParam;
	nMaxParam = 0;
	for(nCount = 0;nCount< m_nCurrentPicNum;nCount++)
	{
		nMax = m_pHisCurvePic[nCount].GetMaxParam();
		if(nMaxParam < nMax)
			nMaxParam = nMax;
	}
	m_nMaxParameret = nMaxParam;//�õ�������
	
	m_nParameret = nMaxParam;//��ǰ������
	
	//�õ�����������
	m_sLastSize = 100+ m_nMaxParameret;
	m_nLeftMoveRequestStatus = NORMALREQUEST ;
	
	//////////////////////////////////////////////////////////////////////////
	
	if(m_nDataEnd-11<m_nDataBegin)
	{
		m_nDataBegin = m_nDataEnd = 0;
	}
//	m_nDataBegin = 0;
//	m_nDataEnd = 0;
	m_nHoriPoint = 0;

	//////////////////////////////////////////////////////////////////////////
	//�ر�����ͳ��
	if(m_pDistractStatTable)
	{
		delete m_pDistractStatTable;
		m_pDistractStatTable = NULL;
	}
	m_nDistractStatState = ENDSTAT;


	memset(m_rectSelect,0,sizeof(m_rectSelect));
	m_bDrawSelect = FALSE;
	//////////////////////////////////////////////////////////////////////////
}

//�������ܣ�������ʷ����������Ϣ�������ݸ�����ָ����ͼ��
//������������
//�������أ���
void CHisTechObj::SetStockNameToPic()
{
	if(!m_pCodeInfo) return;//������Ч
	//�������ڸ���ͼ

	const int nArrInfo[]={1,5,10,15,30,60,62,64,101,102,103};

	CString szTitle;
	szTitle = m_pCodeInfo->szCode;
	const StuSymbolData* pSymbol;
	pSymbol = CPublic::Instance()->GetStockInfo(m_pCodeInfo);
	if(NULL != pSymbol)
	{
		szTitle += pSymbol->szName;
	}


	szTitle +="(";
	for(int nCount = 0;nCount<11;nCount++)
	{
		if(m_nCircleInfo == nArrInfo[nCount])
		{
			szTitle += CLanguageInterface::Instance()->GetStr(20078+nCount);
			break;
		}
	}
	szTitle += ")";
	m_pHisCurvePic[0].SetTitleInfo(szTitle,m_nCircleInfo);
}

///////////////////////////////////////////////////////////////////////////////
//����������󡢽�������
///////////////////////////////////////////////////////////////////////////////
//�������ܣ���������
//������������
//�������أ���
void CHisTechObj::RequestData()
{
	if((NULL ==  m_pCodeInfo))	return; 
//	if(m_bRequestedData)return;
//	m_bRequestedData = TRUE;
//	TRACE("��������\r\n");
	//��������
	int nBufferSize;
	nBufferSize = sizeof(SNormaHead)+sizeof(SHisotyDataReq)+sizeof(SCodeInfo);
	char* pBuffer = new char[nBufferSize];
	ZeroMemory(pBuffer,nBufferSize);
	SNormaHead* pReq  = (SNormaHead*)pBuffer;
	pReq->m_usType = HISKDATA;//��������
	pReq->m_usIndex = CPublic::Instance()->AddRequestIndex();
	pReq->m_usSize = 1;
	SHisotyDataReq* pHistoyReq;
	pHistoyReq = (SHisotyDataReq*)(pReq+1);
	pHistoyReq->nDataType = m_nCircleInfo;
	pHistoyReq->nHasNum = m_nValueNum;
	pHistoyReq->nAskNum = m_sLastSize;
	SCodeInfo* pCode = (SCodeInfo*)(pHistoyReq+1);
	memcpy(pCode,m_pCodeInfo,sizeof(SCodeInfo));
	CPublic::Instance()->SendRequestData(pBuffer,nBufferSize); 
	//////////////////////////////////////////////////////////////////////////			
	delete []pBuffer; 
}

//�������ܣ�������ʷ����
//����������pData ����ָ��
//�������أ���
BOOL CHisTechObj::UpdateData(LPVOID pData, int nDataSize)
{
	if( m_pCodeInfo == NULL)
		return FALSE;	
	if( NULL == pData)
	{
		if(m_pRecvKUnit)
		{
			delete []m_pRecvKUnit;
			m_pRecvKUnit = NULL;
		}
		TransKDataToPic();
		return TRUE;
	}
	SNormaHead* pDealData = (SNormaHead*)pData;
	switch(pDealData->m_usType)
	{
	case HISKDATA:
		{	
//			TRACE("�յ�����\r\n");	
//			m_bRequestedData = FALSE;
			TreatDayDataArrived((char*)pData);
		}
		break;
	case PUSH_REALDATA:
		{//ʵʱ����
			StuRecvQuoteInfo* pRevData =(StuRecvQuoteInfo*)(pDealData+1);
			for(int i=0;i<pDealData->m_usSize;i++)
			{
				StuRecvQuoteInfo* pQuote = (StuRecvQuoteInfo*)(pRevData+i);
				MakeDayData(pQuote);	
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break; 
	}
	TransKDataToPic();
	return TRUE;
}

// �����ǰK�������Ǵ����յģ�����Ҫ���������K�ߺϲ�����ʷK����ȥ
BOOL CHisTechObj::CombineTodayKLine(unsigned char usKLinePriod,const SHistoryUnit*  pTodayKUnit,
					   SHistoryUnit* pPreHisKUnit)
{
    BOOL bNeedCombined = FALSE;
	switch( usKLinePriod) 
    {
    case WEEK_HISK:  // ����
		bNeedCombined = CPublic::Instance()->IsSameWeek(pPreHisKUnit->m_uiDate,pTodayKUnit->m_uiDate);
    	break;
	case MONTH_HISK: // ����
		bNeedCombined = CPublic::Instance()->IsSameMonth(pPreHisKUnit->m_uiDate,pTodayKUnit->m_uiDate);
    	break;
     default:
        break;
    }

	if( bNeedCombined )
	{
		if (pPreHisKUnit->m_fMaxPrice < pTodayKUnit->m_fMaxPrice)
		{
			pPreHisKUnit->m_fMaxPrice = pTodayKUnit->m_fMaxPrice;
		}
		if (pPreHisKUnit->m_fMinPrice> pTodayKUnit->m_fMinPrice && 
			pTodayKUnit->m_fMinPrice != 0 )
		{
			pPreHisKUnit->m_fMinPrice = pTodayKUnit->m_fMinPrice;
		}
		pPreHisKUnit->m_fClosePrice = pTodayKUnit->m_fClosePrice;
		pPreHisKUnit->m_uiVolume += pTodayKUnit->m_uiVolume;
		pPreHisKUnit->m_uiSum += pTodayKUnit->m_uiSum ;
		pPreHisKUnit->m_uiDate = pTodayKUnit->m_uiDate;
	}
	return bNeedCombined;
}

// ����K������Ϊ������ʾ����
void CHisTechObj::AdjustKLineData2Valid(SHistoryUnit& rNowKUnit,
										const SHistoryUnit* pAllKUnitList,int nNowKUnitIndex)
{
	// ���ڿ��̼�Ϊ0�ĵ�Ԫ��ȡ֮ǰ�����һ����Ч���̼�Ϊ���̼�
	if( rNowKUnit.m_fOpenPrice <= 0.001f)
	{
		int i = nNowKUnitIndex - 1;
		while ( i >= 0)
		{
			if( pAllKUnitList[i].m_fClosePrice > 0.001f)
			{
				rNowKUnit.m_fOpenPrice = pAllKUnitList[i].m_fClosePrice;
				break;
			}
			i--;
		}
		
		if( rNowKUnit.m_fClosePrice <= 0.001f)
		{
			rNowKUnit.m_fClosePrice = rNowKUnit.m_fOpenPrice;			
		}
		if( rNowKUnit.m_fMaxPrice <= 0.001f)
		{
			rNowKUnit.m_fMaxPrice = rNowKUnit.m_fOpenPrice;			
		}
		if( rNowKUnit.m_fMinPrice <= 0.001f || rNowKUnit.m_fMinPrice == 0xFFFFFFFF)
		{
			rNowKUnit.m_fMinPrice = rNowKUnit.m_fOpenPrice;			
		}
	}
	else 
	{
		if( rNowKUnit.m_fMinPrice <= 0.001f) // ��֤��Сֵ��Ϊ��,���ử��һ������
		{
			rNowKUnit.m_fMinPrice = min(rNowKUnit.m_fOpenPrice,rNowKUnit.m_fClosePrice);
		}
		if( rNowKUnit.m_fMaxPrice <= 0.001f) // ��֤���ֵ��Ϊ��,���ử��һ������
		{
			rNowKUnit.m_fMaxPrice = max(rNowKUnit.m_fOpenPrice,rNowKUnit.m_fClosePrice);
		}
		if( rNowKUnit.m_fMinPrice > rNowKUnit.m_fMaxPrice) // ��֤��СֵС�����ֵ
		{
			float fTemp = rNowKUnit.m_fMinPrice;
			rNowKUnit.m_fMinPrice = rNowKUnit.m_fMaxPrice;
			rNowKUnit.m_fMaxPrice = fTemp;
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
//�ı���ʷ��ͼ״̬(K�߿�ȡ��Ŵ���С�����ơ����Ƶ�)
///////////////////////////////////////////////////////////////////////////////
//�������ܣ�����K�߿��
//����������nDataNum ��������
//�������أ���
void CHisTechObj::AdjustKUnitWidth()
{ 
	if(m_nDataEnd > m_nDataBegin)
	{
		m_fCurrentWidth = (float)m_TimeRect.Width();
		m_fCurrentWidth /=(m_nDataEnd - m_nDataBegin);//��ȥ���Ҳౣ�����,K��֮�����Ч���
	}
	else
	{
		m_fCurrentWidth = 10;
	}
	m_nRecsOfUnit = (m_nDataEnd-m_nDataBegin)>m_TimeRect.Width()?(m_nDataEnd-m_nDataBegin)/m_TimeRect.Width()+1:1;
	//����õ�K����Ч��϶
	int iSize = (int)(m_fCurrentWidth);
	if(iSize >66)
	{
		m_nKUnitWidth = 61;
		m_fCurrentWidth = 65;
	}
	else if(iSize>20)
		m_nKUnitWidth = (int)(m_fCurrentWidth/1.3);
	else if(iSize>17)
		m_nKUnitWidth = 15;
	else if(iSize >14)
		m_nKUnitWidth = 13;
	else if(iSize >12)
		m_nKUnitWidth = 11;
	else if(iSize > 10)
		m_nKUnitWidth = 9;
	else if(iSize >8)
		m_nKUnitWidth = 7;
	else if (iSize >5)
		m_nKUnitWidth = 5;
	else if(iSize >2)
		m_nKUnitWidth = 3;
	else m_nKUnitWidth = 1;
}

//�������ܣ�����ָ�������㼰��ǰ��Ļ�ں���������,�����ݸ���������ָ��
//������������
//�������أ���
void CHisTechObj::SetHoriPoint()
{
	if(m_nHoriPoint<=0)return;
	if(m_pHoriPoint)
	{
		delete []m_pHoriPoint;
		m_pHoriPoint = NULL;
	}

	m_pHoriPoint = new int[m_nHoriPoint];
	int nCount;//������
	//////////////////////////////////////////////////////////////////////////
	//�õ������λ��
	for(nCount = 0;nCount< m_nHoriPoint;nCount++)
		m_pHoriPoint[nCount] = m_TimeRect.left +(int)(nCount*m_fCurrentWidth);
	//////////////////////////////////////////////////////////////////////////
	//����λ�ø���ͼ��
	for(nCount = 0;nCount<m_nCurrentPicNum;nCount++)
		m_pHisCurvePic[nCount].SetHoriPoint(m_pHoriPoint,m_nHoriPoint);
}

//�������ܣ��Ŵ�K������
//������������
//�������أ�void
void CHisTechObj::ZoomOutData()
{
	if(m_pRecvKUnit == NULL)return;//�����û�з�������
	if(m_nValueNum < m_nDataBegin)return;//λ�ô���

	if(m_nVertStation <0)
	{
		m_nVertStation = 0;
	}
	
	int nDisplayNum;//�õ���ǰ��Ļ������ʾ����
	nDisplayNum = m_nDataEnd -m_nDataBegin;
	if(nDisplayNum <11)return;

	nDisplayNum =nDisplayNum*10/11;
	//////////////////////////////////////////////////////////////////////////
	//�޸Ĳ���  11-03
	m_nDataBegin = m_nDataEnd -nDisplayNum;
//	m_nDataEnd = m_nDataBegin+ nDisplayNum;
	//////////////////////////////////////////////////////////////////////////
	if(m_nDataEnd > m_nValueNum)
	{
		m_nDataEnd = m_nValueNum;	
		m_nDataBegin = m_nDataEnd -nDisplayNum;
	}
	TransKDataToPic();

}

//�������ܣ���СK������`
//������������
//�������أ�void
void CHisTechObj::ZoomInData()
{
	if((m_pRecvKUnit == NULL))return;//�����û�з�������
	if(m_nRecsOfUnit>2)return;//����K�ߴ���2���ܾ�����
	if(m_nVertStation <0)
	{
		m_nVertStation = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	//��ǰλ��
	int nDisplayDataNum;
	nDisplayDataNum = m_nDataEnd -m_nDataBegin;
	nDisplayDataNum =nDisplayDataNum*11/10;	
	m_nDataBegin = m_nDataEnd - nDisplayDataNum;
	if(m_nDataBegin<0)
	{
		if(TP_END == m_sRequestStatus)
		{
			m_nDataBegin = 0;
			m_nDataEnd = m_nDataBegin+nDisplayDataNum;
			if(m_nDataEnd >m_nValueNum)
				m_nDataEnd =m_nValueNum;
		}
		else
		{
			m_sLastSize = -m_nDataBegin;
			m_nLeftMoveRequestStatus =NORMALREQUEST;
			m_nDataBegin = 0;
			RequestData();
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	TransKDataToPic();//�������ݵ���ͼ
}

///////////////////////////////////////////////////////////////////////////////
//�������ݸ���ʷ��ͼ�еĸ�����ͼ��
///////////////////////////////////////////////////////////////////////////////
//�������ܣ����ݵ�ǰλ�ø���ͼ��
//������������
//����������void
void CHisTechObj::TransCurStationToPic()
{
	if(m_nVertStation<0)return;

	int nCount;
	for(nCount = 0;nCount<m_nCurrentPicNum;nCount++)
		m_pHisCurvePic[nCount].SetCurrentStation(m_nVertStation);
	if(m_nVertStation != -1)
	{
		if(m_nValueNum <= m_nDataBegin +m_nVertStation)return;

		int nStation;

		nStation = m_nDataBegin+m_nVertStation;
		if(nStation >= m_nDataEnd)
			nStation = m_nDataEnd - 1;//���һ������
		long* pTemp = (long*)(m_pTranslateKUnit + nStation);
		CMulQuantitySequence seq;
		seq.m_ppValues = (long**)&pTemp;
		if(nStation>0)
			seq.m_lReference = (long)&m_pTranslateKUnit[nStation-1].m_fClosePrice;
		else
			seq.m_lReference = (long)&m_pTranslateKUnit[nStation].m_fOpenPrice;

		//��ǰ���λ�õļ۸�
		CString strTemp;
		float fTemp;
		GetCurCrossCursorPosValue(strTemp,fTemp);
		seq.m_lMin = (long)&fTemp;
		
		m_InfoTable.UpdateData(&seq,0);
	}
}

//�������ܣ��������ݵ���ͼ
//����������pData ����ָ��
//����������nDataNum ��������
void CHisTechObj::TransKDataToPic()
{
	if (m_pRecvKUnit == NULL) return;
	if(m_nDataBegin<0)return;

	SetStockNameToPic();//����������Ϣ
	//////////////////////////////////////////////////////////////////////////
	AdjustKUnitWidth();//����K�߿��
	//////////////////////////////////////////////////////////////////////////
	//����������
	if(m_bTranslateIsPoint&&m_pTranslateKUnit)
	{
		delete []m_pTranslateKUnit;
		m_pTranslateKUnit = NULL;
	}
	m_pTranslateKUnit = m_pRecvKUnit;
	m_bTranslateIsPoint = FALSE;
	CMulQuantitySequence seq;
	

	
	seq.m_nSize =m_nDataEnd;//��������
	seq.m_ppValues =(long**) &m_pTranslateKUnit;//����ָ��
	
	seq.m_lReference = m_nKUnitWidth;//K�߿��

	seq.m_lMax = CPublic::Instance()->GetDecimal(m_pCodeInfo);
	seq.m_nStart = m_nDataBegin;//��ʼλ��

	seq.m_lMin = m_nRecsOfUnit;//������
	//////////////////////////////////////////////////////////////////////////
	//add by gongxiaofeng at 2004-7-22

	int nDisplayNum;
	nDisplayNum = m_TimeRect.Width()*m_nRecsOfUnit;
	if(nDisplayNum < seq.m_nSize -seq.m_nStart)
	{
		seq.m_nSize = nDisplayNum + seq.m_nStart;
		m_nDataEnd = m_nDataBegin + seq.m_nSize;
	}

	//////////////////////////////////////////////////////////////////////////
 	m_nHoriPoint = seq.m_nSize - seq.m_nStart ;//������
	if (m_nVertStation	>= m_nHoriPoint - 1)
	{//�Ŵ�����У�ԭ������λ�ÿ��ܻ����Ŀǰ�ܹ���ʾ�����K����Ŀ�����Դ˴����о���
		m_nVertStation = m_nHoriPoint - 1;
	}
	m_nDataOffset = seq.m_nStart;//ƫ��λ��
	SetHoriPoint();//����λ��
	//////////////////////////////////////////////////////////////////////////
	//�������ݸ���ͼԪ
	int nCount;
	for(nCount = 0;nCount<m_nCurrentPicNum;nCount++)
		m_pHisCurvePic[nCount].UpdateData(&seq);
	//////////////////////////////////////////////////////////////////////////
	TransCurStationToPic();//���ݵ�ǰλ�ø���ͼ
	//////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
//����ӿ�ID�����̡������Ϣ
//�������ܣ���Ӧ�˵���Ϣ
//����������nSel - ѡ�в˵���
//�������أ���
BOOL CHisTechObj::OnUserCmd(UINT iSel)
{
	switch(iSel)
	{
	case ID_FUNCTION_MACD:
	case ID_FUNCTION_RSI:
	case ID_FUNCTION_BOLL:
	case ID_FUNCTION_KDJ:
	case ID_FUNCTION_DMI:
	case ID_FUNCTION_WR:
		{
			char szInfo[10][6]={"MACD","RSI","BOLL","KDJ","DMI","WR"};			
			SetFormulaCommand(szInfo[iSel- ID_FUNCTION_MACD]);
		}
		break;
	case ID_TECHANALY_TECHTYPE:  //ѡ��ָ��
		{
			if((m_pHisCurvePic == NULL) ||  m_nCurrentPicNum <= 1 ||
				m_nCurrentTechIndex <= 0 )
				return TRUE;
			CString strIndex;
			
			strIndex = m_pHisCurvePic[m_nCurrentTechIndex].GetTechType(); 
			if (CFormulaFace::OnOpenFormulaParasSet(strIndex) == IDOK &&
				strIndex.GetLength() > 0)
			{
				if(m_nCurrentTechIndex == 0)break;
				m_pHisCurvePic[m_nCurrentTechIndex].SetTechType(strIndex);
				RecalSubObjSize();
				TransCurStationToPic();
				SaveIndexStatusParam();
			}
		}
		break;	
	case ID_TECHANALY_TOTAL: //����ͳ��
		{
			if( ENDSTAT == m_nDistractStatState)
			{
				m_nDistractStatState = BEGINSTAT;

			}
			else
			{
				m_nDistractStatState = ENDSTAT;
				if(m_pDistractStatTable)
				{
					delete m_pDistractStatTable;
					m_pDistractStatTable = NULL;
				}

				AfxGetMainWnd()->RedrawWindow();
			}
		}
		break;			
	case ID_TECHANALY_1MINLINE: //1������
	case ID_TECHANALY_5MINLINE: //5������
	case ID_TECHANALY_10MINLINE: //10������
	case ID_TECHANALY_15MINLINE: //15������
	case ID_TECHANALY_30MINLINE: //30������
	case ID_TECHANALY_60MINLINE: //60������
	case ID_TECHANALY_2HOURS:	//2Сʱ��
	case ID_TECHANALY_4HOURS:	//4Сʱ��
	case ID_TECHANALY_DAYLINE: //����
	case ID_TECHANALY_WEEKLINE: //����
	case ID_TECHANALY_MONLINE: //����
		{
			m_nCurKLinePriodID = iSel;			
			int nCircle[] = {MINUTE1_HISK,MINUTES5_HISK,MINUTES10_HISK,MINUTES15_HISK,
				MINUTES30_HISK,MINUTES60_HISK,MINUTES62_HISK,MINUTES64_HISK,DAY_HISK,WEEK_HISK,MONTH_HISK};
			int nSelType;
			nSelType = iSel - ID_TECHANALY_1MINLINE;
			nSelType = nCircle[nSelType];
			if(nSelType == m_nCircleInfo)return FALSE;//������ֱͬ�ӷ���
			m_nCircleInfo = nSelType;
			m_nDataBegin = m_nDataEnd = 0;
			SetStockInfo(NULL);
			RequestData();
		}
		break;
	case ID_ANALYSIT_PICCOMMBIN_ONEPIC: //һͼ
	case ID_ANALYSIT_PICCOMMBIN_TWOPIC: //��ͼ
	case ID_ANALYSIT_PICCOMMBIN_THREEPIC: //��ͼ
	case ID_ANALYSIT_PICCOMMBIN_FOURPIC:
	case ID_ANALYSIT_PICCOMMBIN_SIXPIC:
	case ID_ANALYSIT_PICCOMMBIN_NINEPIC:
		{
			int nPicNum[] = {1,2,3,4,6,9};
			iSel -= ID_ANALYSIT_PICCOMMBIN_ONEPIC;
			if(m_nCurrentPicNum != nPicNum[iSel])
			{
				ReAssignPicNum(nPicNum[iSel]);
				RecalSubObjSize();//���·�������				
				SaveIndexStatusParam();
			}

		}
		break;
	case ID_ANALYSIT_PICCOMMBIN_DEFAULT:
		{
			if(m_strDefaultIndex.IsEmpty())return FALSE;//û��Ĭ��ָ��
			//////////////////////////////////////////////////////////////////////////
			int nIndexNum;
			CString strIndexName;
			strIndexName = m_strDefaultIndex;
			nIndexNum = 0;
			int iPos;
			CString strLeft;
			CStringArray ArrIndex;
			while(-1 != (iPos = strIndexName.Find(",")))
			{
				nIndexNum ++;
				strLeft = strIndexName.Left(iPos);
				ArrIndex.Add(strLeft);
				iPos++;				
				strIndexName = strIndexName.Mid(iPos);
			}
			//////////////////////////////////////////////////////////////////////////

			if(m_nCurrentPicNum != nIndexNum)
			{
				ReAssignPicNum(nIndexNum);
			}
			for(int iRec = 0; iRec < ArrIndex.GetSize(); iRec++)
			{
				m_pHisCurvePic[iRec].SetTechType(ArrIndex[iRec]);
			}
			RecalSubObjSize();//���·�������
			SaveIndexStatusParam();
		}
		break;
	case ID_INDEX_PARAM: //ָ�����
		{
			CString strTech = m_pHisCurvePic[m_nCurrentTechIndex].GetTechType();

			if (strTech.CompareNoCase(_T("KLINEMA")) == 0) strTech = _T("MA");
			char szIndex[100];
			strcpy(szIndex,(char*)(LPCTSTR)strTech);
			if (CFormulaFace::OnOpenSingleFormulaParasSet(CString(szIndex)))
			{
				if (strTech.CollateNoCase(_T("MA")))
					m_pHisCurvePic[m_nCurrentTechIndex].SetTechType(strTech);
				else
					m_pHisCurvePic[m_nCurrentTechIndex].UpdateTechNameTitle();
				RecalSubObjSize();
				TransCurStationToPic();	
			}
		}
		break;
	case ID_INDEX_INFO://ָ��ע��
		{
			if((m_pHisCurvePic == NULL) || (m_nCurrentTechIndex < 0)
				|| (m_nCurrentTechIndex > m_nCurrentPicNum - 1))
				return FALSE;
			CString strTech;
			
			strTech = m_pHisCurvePic[m_nCurrentTechIndex].GetTechType(); 
			if (strTech.CompareNoCase(_T("KLINEMA")) == 0) strTech = _T("MA");
			
			CFormulaFace::OnOpenFormulaDesc((char*)(LPCTSTR)strTech);
		}
		break;
	case ID_INDEX_DELETE://ɾ��ָ��
		{
			DeleteIndex();
		}
		break;
	case ID_INDEX_MODIFYFUNCTION: //����ָ�깫ʽ
		{
			if((m_pHisCurvePic == NULL) || (m_nCurrentTechIndex > m_nCurrentPicNum - 1))
				return FALSE;
			CString strTech = m_pHisCurvePic[m_nCurrentTechIndex].GetTechType();
			SDefineReturn paras;
			
			memset(&paras, 0, sizeof(paras));
			if (strTech.CompareNoCase(_T("KLINEMA")) == 0) strTech = _T("MA");
			
			if (CFormulaFace::OnOpenDefineTechDlg(paras, (char*)(LPCTSTR)strTech) == IDOK)
			{
				m_pHisCurvePic[m_nCurrentTechIndex].UpdateTechNameTitle();
				RecalSubObjSize();
				TransCurStationToPic();
			}
		}
		break;
	case ID_DRAWLINE_BEELINE:
	case ID_DRAWLINE_RECTANGLE:
	case ID_DRAWLINE_GOLDLINE:
	case ID_DRAWLINE_PARALLEL:
	case ID_DRAWLINE_GANLINE:
	case ID_DRAWLINE_STOP:	
		{
			m_nDrawLineState = iSel -ID_DRAWLINE_BEELINE+1;
//			m_nDrawState = BEGINDRAW;
			int nCount;
			for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
			{
				m_pHisCurvePic[nCount].SetDrawLineType(m_nDrawLineState);
			}
			if(iSel == ID_DRAWLINE_STOP)
			{
				m_nDrawState = SELECTSTATE;
			}
			else
			{
				m_nDrawState = BEGINDRAW;
//				m_nDrawState = ENDDRAW;
			}
		}
		break;
	case ID_DRAWLINE_DELETE:
		{
			int nCount;
			for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
			{
				if(m_pHisCurvePic[nCount].DeleteCurrentDrawLine())
				{
					AfxGetMainWnd()->RedrawWindow();
					break;
				}
			}
			m_nDrawLineState = -1;
		}
		break;
	case ID_TECHANALY_ZOOMOUT:
		{
			ZoomOutData();
		}
		break;
    case ID_TECHANALY_ZOOMIN:
		{
			ZoomInData();			
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CHisTechObj::OnUpdateUserCmd(CCmdUI *pCmdUI)
{
	switch(pCmdUI->m_nID)
	{
	case ID_TECHANALY_1MINLINE://1������
	case ID_TECHANALY_5MINLINE: //5������
	case ID_TECHANALY_10MINLINE: //10������
	case ID_TECHANALY_15MINLINE: //15������
	case ID_TECHANALY_30MINLINE: //30������
	case ID_TECHANALY_60MINLINE: //60������
	case ID_TECHANALY_DAYLINE: //����
	case ID_TECHANALY_WEEKLINE: //����
	case ID_TECHANALY_MONLINE: //����

		{
			pCmdUI->Enable(pCmdUI->m_nID != m_nCurKLinePriodID);
		}
		break;		
	default:
		if((pCmdUI->m_nID >ID_DRAWLINE_TAG)&&(pCmdUI->m_nID<= ID_DRAWLINE_DELETE))
		{
			pCmdUI->Enable(TRUE);
			if(pCmdUI->m_nID== ID_DRAWLINE_STOP)
			{
				if(m_nDrawState == SELECTSTATE)
					pCmdUI->SetCheck(TRUE);
				else
					pCmdUI->SetCheck(FALSE);
			}
			else
			{
				if((pCmdUI->m_nID == m_nDrawLineState + ID_DRAWLINE_BEELINE-1)&&(m_nDrawState!= SELECTSTATE))
				{
					pCmdUI->SetCheck(TRUE);
				}
				else
				{
					pCmdUI->SetCheck(FALSE);
				}
			}
		}
		else
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}

void CHisTechObj::InitCircleType()
{
	m_nCircleInfo = MINUTE1_HISK;//��������
    m_nCurKLinePriodID = ID_TECHANALY_1MINLINE;
}

// ��ǰK�����������Ƿ�Ϊ��ʼ״̬
BOOL CHisTechObj::IsInitCircleType()
{
	return (m_nCircleInfo == DAY_HISK);
}

//�������ܣ���Ӧ�����¼�
//����������nChar ���̰���
//�������أ��ޡ�
BOOL CHisTechObj::OnChar(int nChar)
{	
	switch(nChar)
	{
	case VK_DOWN:  
		{
			if(GetKeyState(VK_CONTROL) & 0x8000)
				CtrlDownZoom();
			else
				ZoomInData();       //��С����
		}
		break;
	case VK_UP:
		{
			if(GetKeyState(VK_CONTROL) & 0x8000)
				CtrlUpZoom();
			else
				ZoomOutData();      //�Ŵ�����
		}
		break;
	case VK_LEFT:
		{//���ƹ��	
			// �������ƶ�ʱ���޶˴���mousemove�¼��Ĵ���
			CPoint station(m_DrawRect.left,m_DrawRect.top);
			CPublic::Instance()->GetView(/*VIEWINFO_STOCK*/DLG_TRENDCHART)->ClientToScreen(&station);
			SetCursorPos(station.x,station.y);
			if(GetKeyState(VK_CONTROL) & 0x8000)
				CtrlLeftMove();
			else
				LeftMove();
			m_bDrawCross = TRUE;
		}
		break;
	case VK_RIGHT:
		{//���ƹ��
			// �������ƶ�ʱ���޶˴���mousemove�¼��Ĵ���
			CPoint station(m_DrawRect.left,m_DrawRect.top);
			CPublic::Instance()->GetView(/*VIEWINFO_STOCK*/DLG_TRENDCHART)->ClientToScreen(&station);
			SetCursorPos(station.x,station.y);

			if(GetKeyState(VK_CONTROL) & 0x8000)
				CtrlRightMove();
			else
				RightMove();
			m_bDrawCross = TRUE;
		}
		break;
	case VK_HOME:
		{
			m_nVertStation = 0;
			TransCurStationToPic();
		}
		break;
	case VK_END:
		{
			m_nVertStation = (m_nDataEnd - m_nDataBegin )-1;
			TransCurStationToPic();
		}
		break;
	case VK_RETURN:
		{
			MouseLBtnDbClick(CPoint(0,0));
		}
		break;
	case VK_F8:  // �л�K������
		{
			if( m_nCurKLinePriodID == ID_TECHANALY_MONLINE)
			{
				m_nCurKLinePriodID = ID_TECHANALY_1MINLINE;
			}
			else
			{
				m_nCurKLinePriodID++;
			}			
			OnUserCmd(m_nCurKLinePriodID);
		}
		break;
	case VK_ESCAPE:
		{
			if(m_nDrawState != NULLDRAW)
				m_nDrawState = NULLDRAW;

			if(m_nDistractStatState != ENDSTAT)
				m_nDistractStatState = ENDSTAT;
			else if(m_bDrawCross)
				m_bDrawCross = FALSE;
			else return FALSE;
		}
		break;
	case VK_DELETE:
		{
			OnUserCmd(ID_DRAWLINE_DELETE);
		}
		break;
	default:
		break;
	}
	return TRUE;
}

//���Ե��λ��
BOOL CHisTechObj::HitTest(CPoint point)
{
	return m_DrawRect.PtInRect(point);
}

BOOL CHisTechObj::MouseLBtnDown(CPoint point)
{
	if(m_pHoriPoint == NULL)return FALSE;
	//////////////////////////////////////////////////////////////////////////
	if(!m_bDrawSelect&&(m_nDrawState == SELECTSTATE))	m_bDrawCross = !m_bDrawCross;
	int nCount;
	//////////////////////////////////////////////////////////////////////////

	if((BEGINSTAT== m_nDistractStatState )||(SECONDCLICK == m_nDistractStatState))
	{//ͳ��״̬����һ�ε��
		m_nFirstClickState = m_nVertStation;//��һ��λ��
		m_nDistractStatState = FIRSTCLICK;
		return TRUE;
	}
	if(FIRSTCLICK == m_nDistractStatState)
	{//ͳ��״̬���ڶ��ε��
		m_nSecondClickState = m_nVertStation+1;//�ڶ���λ��
		m_nDistractStatState = SECONDCLICK;
		//////////////////////////////////////////////////////////////////////////
		//�������Ҵ������ݸ����
		if(m_nFirstClickState>m_nSecondClickState)
		{//����λ��
			int nTemp;
			nTemp = m_nSecondClickState;
			m_nSecondClickState = m_nFirstClickState;
			m_nFirstClickState = nTemp;
		}
		CalcDistractInfo();
		return TRUE;
	}
	if(!m_bLeftButtonDown&&m_TipInfoRect.PtInRect(point))
	{//����϶���Ϣ���
		m_bLeftButtonDown = TRUE;
		m_DrapPoint = point;
		return TRUE;
	}
	if((m_nDrawState !=BEGINDRAW )&&(m_KLineRect.PtInRect(point)))
	{
		m_bDrawSelect = TRUE;
		m_rectSelect.left = m_rectSelect.right = point.x;
		m_rectSelect.top = m_rectSelect.bottom = point.y;
	}
	for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
	{
		if(m_pHisCurvePic[nCount].MouseDown(point))
		{
			m_nCurrentTechIndex = nCount;
			break;
		}
	}
	if(m_nCurrentTechIndex != -1)
	{//�ҵ��˱�־��
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	//������

	for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
	{
		if((m_pHisCurvePic)
		&&(m_pHisCurvePic[nCount].HitTest(point)))
		{
			m_nCurrentTechIndex = nCount;
			return TRUE;//�ҵ����ʵ�
		}
	}

	return FALSE;
}

BOOL CHisTechObj::MouseLBtnDbClick(CPoint point)
{
//	if(m_bRequestedData||(m_pRecvKUnit == NULL))return FALSE;//�����û�з�������
	//////////////////////////////////////////////////////////////////////////
	//�����Ƿ���̧ͷ
	int nStation;
	for(int i = 0;i<m_nCurrentPicNum;i++)
	{
		nStation = m_pHisCurvePic[i].MouseDbClickTitle(point);
		if(nStation == 1)
		{
			m_nCurrentTechIndex = i;
			OnUserCmd(ID_TECHANALY_TECHTYPE);
			return TRUE;
		}
		else if(nStation == 2)
		{
			m_nCurrentTechIndex = i;
			OnUserCmd(ID_INDEX_PARAM);//�򿪲������öԻ���
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CHisTechObj::MouseLBtnUp(CPoint point)
{
	int nCount;
	for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
	{
		m_pHisCurvePic[nCount].MouseUp(point);
	}
	m_nDrawState = SELECTSTATE;

	if(m_bLeftButtonDown)
	{//�Ѿ������Ϣ�����϶�״̬
		m_bLeftButtonDown = FALSE;
		int iXPat = point.x-m_DrapPoint.x;
		int iYPat = point.y - m_DrapPoint.y;
		m_TipInfoRect.left += iXPat;
		m_TipInfoRect.right += iXPat;
		m_TipInfoRect.top += iYPat;
		m_TipInfoRect.bottom += iYPat;
		m_InfoTable.MoveTo(m_TipInfoRect);
		return TRUE;
	}
	if(m_KLineRect.PtInRect(point)&&m_bDrawSelect)
	{
		m_rectSelect.right = point.x;
		m_rectSelect.bottom = point.y;
		m_bDrawSelect = FALSE;
		float fKLineUnit;
		fKLineUnit = (float)m_rectSelect.Width();
		fKLineUnit /=m_fCurrentWidth;
		if(fKLineUnit <11)return FALSE;
		int nCount;	
		for(nCount = 0;nCount<m_nHoriPoint;nCount++)
		{
			if(m_pHoriPoint[nCount]>m_rectSelect.right)
			{
				m_nDataEnd = m_nDataBegin +nCount;
				break;
			}
		}
		for(nCount = 0;nCount<m_nHoriPoint;nCount++)
		{
			if(m_pHoriPoint[nCount]>m_rectSelect.left)
			{
				m_nDataBegin += nCount;
				break;
			}
		}
		TransKDataToPic();
		return TRUE;
	}
// 	if(m_bDrawCross)
// 	{
// //		m_bDrawCross = FALSE;
// 		return TRUE;
// 	}
	return FALSE;
}

BOOL CHisTechObj::MouseMove(CPoint point)
{
	// ����������϶�ʱ������������ƶ�����ʱ�޶˴���mousemove��Ϣ�Ĵ���
	m_bMouseInRect = m_DrawRect.PtInRect(point);
	if(!m_bMouseInRect)
	{
		memset(m_rectSelect,0,sizeof(m_rectSelect));
	}

	if( (point.x == m_DrawRect.left) 
		&& (point.y == m_DrawRect.top))
	{
		return FALSE;
	}
	int nCount;

	for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
	{
		if(m_pHisCurvePic[nCount].MouseMove(point))
		{
			return TRUE;
		}
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
	static int nMouseStation = 0;
	int nCurMouseStation = 0;
	for(nCount = 0;nCount<m_nCurrentPicNum;nCount++)
	{
		nCurMouseStation = m_pHisCurvePic[nCount].MouseDbClickTitle(point);
		if(nCurMouseStation>0)
		{
			HCURSOR cur;
			cur = LoadCursor(NULL,MAKEINTRESOURCE(32649));
			SetCursor(cur);
			break;
		}
	}
	if(nCount >= m_nCurrentPicNum)
	{
		nMouseStation = 0;
		
		HCURSOR cur;
		cur = LoadCursor(NULL,IDC_ARROW);
		SetCursor(cur);
	}

	//////////////////////////////////////////////////////////////////////////
	//�жϵõ���ǰ�����
	if(NULL == m_pHoriPoint)return FALSE;

	for(nCount = 0;nCount<m_nCurrentPicNum;nCount++)
	{
		if(m_pHisCurvePic[nCount].HitTest(point))
		{
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if(m_KLineRect.PtInRect(point)&&m_bDrawSelect)
	{
		m_rectSelect.right = point.x;
		m_rectSelect.bottom = point.y;
	}
	if(nCount>=m_nTotalPicNum)return FALSE;//�����κ���ͼ����

	for(nCount= 0;nCount<m_nHoriPoint;nCount++)
	{
		if(point.x >= m_pHoriPoint[nCount] &&
			point.x <= (m_pHoriPoint[nCount]+m_fCurrentWidth))
		{
			m_nVertStation = nCount;
			break;
		}
	}
	if (nCount >= m_nHoriPoint)	return FALSE; //�����κ�K�߷�Χ�ڣ�ֱ�ӷ���
	
	if(m_nVertStation != -1)
	{
		m_nHoriStation = point.y;

		TransCurStationToPic();//���ݵ�ǰλ�ø���ͼ

		//////////////////////////////////////////////////////////////////////////
		// add by david at 2004-7-23
		int nStation;
		nStation = m_nDataBegin +m_nVertStation*m_nRecsOfUnit;
		if(nStation >= m_nValueNum)nStation = m_nValueNum-1;//���һ������
		m_nNowShowDate = m_pRecvKUnit[nStation].m_uiDate&0x7fffffff;//�õ�����
		//////////////////////////////////////////////////////////////////////////
		
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	// add by david at 2004-7-23	
	m_nNowShowDate = -1;

	return FALSE;
}

//�������ܣ���Ӧ����Ҽ������Ϣ
//����������lpWnd - ������ָ�룻point - �������
//�������أ�TRUE/FALSE
BOOL CHisTechObj::MouseRBtnDown(LPVOID lpWnd,CPoint point)
{
	CView* pViewWnd = (CView*)lpWnd;
	//////////////////////////////////////////////////////////////////////////
	//�жϵ����
	BOOL bClickIndex;
	bClickIndex = FALSE;
	m_nCurrentTechIndex = -1;
	int nCount; 
	CPoint clientPoint;
	clientPoint = point;
	ScreenToClient(pViewWnd->GetSafeHwnd(),&clientPoint);
	for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
	{
		if(m_pHisCurvePic)
		{
			if( m_pHisCurvePic[nCount].MouseRBDown(clientPoint))
			{
				m_nCurrentTechIndex = nCount;
				bClickIndex = TRUE;
				break;
			}
			else if( m_pHisCurvePic[nCount].HitTest(clientPoint))
			{
				m_nCurrentTechIndex = nCount;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	CMenu popMenu;

	popMenu.LoadMenu(IDR_RIGHTMENU); //װ�ز˵���Դ


	CMenu* pSubMenu;
	if(!bClickIndex)
	{	

		pSubMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_HISTECHOBJ);	
		CLanguageInterface::Instance()->InitRightMenu(pSubMenu,POPSUBMENU_INDEX_HISTECHOBJ);

	}
	else
	{		
		pSubMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_TECHINDEX);
		CLanguageInterface::Instance()->InitRightMenu(pSubMenu,POPSUBMENU_INDEX_TECHINDEX);
	}


	pSubMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN
		,point.x,point.y,AfxGetMainWnd()); //�����˵�
	popMenu.DestroyMenu(); //���ٲ˵�

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//��ͼ
//////////////////////////////////////////////////////////////////////////////////
//�������ܣ�����ʮ�ֹ��
//����������pDC
//�������أ�void
void CHisTechObj::DrawCrossCursor(CDC *pDC)
{
	if((m_nVertStation>=0)&&(m_nVertStation < m_nHoriPoint)
		&&(m_nHoriStation >m_DrawRect.top)&&(m_nHoriStation<m_TimeRect.top))
	{//��Ч������
		//////////////////////////////////////////////////////////////////////////
		//��ʮ����
		int nVertStation;
		nVertStation = m_TimeRect.left+ (int)(m_nVertStation*m_fCurrentWidth);
		nVertStation += m_nKUnitWidth/2;//λ��K���м�
		{
			int iOldMode = pDC->SetROP2(R2_NOT);//ѡ�����˼��ͼ
			pDC->MoveTo(nVertStation,m_DrawRect.top+16);
			pDC->LineTo(nVertStation,m_TimeRect.top);
			pDC->MoveTo(m_TimeRect.left,m_nHoriStation);
			pDC->LineTo(m_TimeRect.right,m_nHoriStation);
			pDC->SetROP2(iOldMode);
		}
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
		if( GetCurCrossCursorPosValue(szText,fValue))
		{
			textSize = pDC->GetTextExtent(szText);
			ItemRect.top = m_nHoriStation;
			ItemRect.bottom =ItemRect.top +textSize.cy;
			//ItemRect.right = m_DrawRect.left + VERTBANKSIZE-2;
			ItemRect.left = m_KLineRect.right+2; 
			ItemRect.right = ItemRect.left  + textSize.cx+6;
			if(ItemRect.right>m_DrawRect.right)
			{//����λ��
				ItemRect.right = m_DrawRect.right;
				ItemRect.left = ItemRect.right - textSize.cx-2;
			}
			pDC->Rectangle(ItemRect);
			pDC->FillSolidRect(ItemRect,RGB(0,0,255));
			pDC->DrawText(szText,ItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);			
		}
				
		//////////////////////////////////////////////////////////////////////////
		//�����²�ʱ���ǩ
		int nDate;
		int nStation;
		nStation = m_nDataBegin +m_nVertStation;

		if(nStation >= m_nValueNum)nStation = m_nValueNum-1;//���һ������
		nDate = /*m_pRecvKUnit*/m_pTranslateKUnit[nStation].m_uiDate&0x7fffffff;
		if(m_nCircleInfo >= DAY_HISK)
			szText.Format(_T("%08d"),nDate);
		else
			szText.Format(_T("%d"),nDate);
		ItemRect = m_TimeRect;
		ItemRect.left = nVertStation;
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
}

// ��ȡ��ǰʮ�ֹ������λ�õļ۸�
BOOL CHisTechObj::GetCurCrossCursorPosValue(CString &rstrFormateVaule,float &rfValue)
{
	float fPrice = -1;
	rstrFormateVaule = "-";
	rfValue = 0;
	int nCount;
	for(nCount = 0;nCount<m_nCurrentPicNum;nCount++)
	{
		fPrice = m_pHisCurvePic[nCount].GetPriceFromStation(m_nHoriStation);
		if(fPrice != -1)
		{//�ҵ���Ӧλ��
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////		
	if(fPrice != -1)
	{
		rfValue = fPrice;
		CString szName;
		szName = m_pHisCurvePic[nCount].GetTechType();
		if((-1 != szName.Find(_T("KLINE")))||(szName ==_T("MA"))||(szName ==_T("BOLL")))
			rstrFormateVaule = CPublic::Instance()->GetFormateValue(rfValue,CPublic::Instance()->GetDecimal(m_pCodeInfo));
		else
			rstrFormateVaule = CPublic::Instance()->GetFormateValue(rfValue,CPublic::Instance()->GetTechIndexValueDecimal(szName));	

		return TRUE;
	}

	return FALSE;
}

//�������ܣ���ʱ������
//����������pDC
//�������أ�void
void CHisTechObj::DrawTimeRect(CDC *pDC)
{
	if(0 ==m_nHoriPoint)return;
	if(m_nDataBegin<0)return;

	int nMaxDataCount;

	nMaxDataCount = m_nDataBegin + (m_nHoriPoint - 1 );

	if((m_nValueNum < nMaxDataCount)) return;//����Խ��
	
// 	if(IsBadReadPtr(m_pRecvKUnit,m_nHoriPoint*m_nRecsOfUnit*sizeof(SHistoryKUnit))return;
	CRect rRect = m_TimeRect;
	int nCount;
	CString szItem;//������Ϣ
	CRect	ItemRect;//��Ԫ����
	COLORREF crText;
	crText  = CInterfaceManager::Instance()->GetSysColor(ID_COORDTEXT_COLOR);
	CFont* TextFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	CFont* OldFont = pDC->SelectObject(TextFont);
	pDC->SetTextColor(crText);
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF crLine;
	crLine = CInterfaceManager::Instance()->GetSysColor(ID_COORDTEXT_COLOR);
	CPen PenLine(PS_SOLID,1,crLine);
	CPen *OldPen = pDC->SelectObject(&PenLine);
	//////////////////////////////////////////////////////////////////////////
	int nDate1,nDate2;
	CRect TextRect;
	CSize TextSize;//���ֳߴ�
	TextRect = CRect(0,0,0,0);
	//////////////////////////////////////////////////////////////////////////
	for(nCount = 0;nCount<m_nHoriPoint;nCount++)
	{
		ItemRect = m_TimeRect;//�õ�ʱ������
		ItemRect.top = m_TimeRect.top +6;
		ItemRect.left = m_TimeRect.left + (int)(nCount*m_fCurrentWidth)+m_nKUnitWidth/2;
		if(ItemRect.left <TextRect.right )continue;
		//////////////////////////////////////////////////////////////////////////
		//������־��
		int nStation;
		nStation = m_nDataBegin +nCount;

		if(nStation >= m_nValueNum)break;
		
		nDate1 = m_pRecvKUnit[nStation].m_uiDate&0x7fffffff;//�õ���һ���������� 
		if(nCount)
		{
			nStation = m_nDataBegin +(nCount-1);
			
			nDate2 = m_pRecvKUnit[nStation].m_uiDate&0x7ffffff;//�õ��ڶ�����������
		}
		//////////////////////////////////////////////////////////////////////////
		//�����²�����
		if(m_nCircleInfo < DAY_HISK)
		{//����������
			if((0 == nCount)
				||(nDate1 != nDate2))
			{//��һ�λ���������Ϣ
				pDC->MoveTo(ItemRect.left,rRect.top);
				pDC->LineTo(ItemRect.left,rRect.top+4);
				//////////////////////////////////////////////////////////////////////////				
				szItem.Format(_T("%d"),nDate1);//������Ϣ
				pDC->DrawText(szItem,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);//��������
				TextSize = pDC->GetTextExtent(szItem);
				TextRect = ItemRect;
				TextRect.right = TextRect.left + TextSize.cx+2;
			}			
		}
		else
		{
			if(0 == nCount)
			{//��һ�λ�������Ϣ
				pDC->MoveTo(ItemRect.left,rRect.top);
				pDC->LineTo(ItemRect.left,rRect.top+4);
				//////////////////////////////////////////////////////////////////////////
				szItem.Format(_T("%d"),nDate1/10000);//��
				pDC->DrawText(szItem,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);//��������
				TextSize = pDC->GetTextExtent(szItem);
				TextRect = ItemRect;
				TextRect.right = TextRect.left + TextSize.cx+2;
				continue;
			}
			if((m_nCircleInfo == DAY_HISK)
				||(m_nCircleInfo == WEEK_HISK))
			{//���±�־��
				if((nDate1%10000)/100 != (nDate2%10000)/100)
				{
					pDC->MoveTo(ItemRect.left,rRect.top);
					pDC->LineTo(ItemRect.left,rRect.top+4);
					//////////////////////////////////////////////////////////////////////////
					szItem.Format(_T("%02d"),(nDate1%10000)/100);//����Ϣ
					pDC->DrawText(szItem,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);//��������
					TextSize = pDC->GetTextExtent(szItem);
					TextRect = ItemRect;
					TextRect.right = TextRect.left + TextSize.cx+2;
				}				
			}
			else if(m_nCircleInfo == MONTH_HISK)
			{//�������־��
				if(nDate1/10000 != nDate2/10000)
				{//��������Ϣ
					pDC->MoveTo(ItemRect.left,rRect.top);
					pDC->LineTo(ItemRect.left,rRect.top+4);
					//////////////////////////////////////////////////////////////////////////
					szItem.Format(_T("%d"),nDate1/10000);//������Ϣ
					pDC->DrawText(szItem,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);//��������					
					TextSize = pDC->GetTextExtent(szItem);
					TextRect = ItemRect;
					TextRect.right = TextRect.left + TextSize.cx+2;
				}
			}
		} 
	}
	pDC->SetBkMode(nOldMode);
	pDC->SelectObject(OldPen);
	pDC->SelectObject(OldFont);
}

//�������ܣ�����ͼ������ͼ��
//����������pDC
//�������أ�void
BOOL CHisTechObj::Draw(CDC* pDC)
{
	//////////////////////////////////////////////////////////////////////////
	int nCount;	
	for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
		m_pHisCurvePic[nCount].DrawTotalRect(pDC);
	//////////////////////////////////////////////////////////////////////////
	//��������ͼ��
	if(!m_pRecvKUnit||IsBadReadPtr(m_pRecvKUnit,m_nValueNum*sizeof(SHistoryUnit)))
		return FALSE;
	for(nCount= 0;nCount<m_nCurrentPicNum;nCount++)
		m_pHisCurvePic[nCount].Draw(pDC);

	//////////////////////////////////////////////////////////////////////////
	//����ʱ������
	DrawTimeRect(pDC);
	//////////////////////////////////////////////////////////////////////////
	//��ʮ�ֹ��
//	if(m_bDrawCross&&m_bMouseInRect)
	//modify by gongxiaofeng at 2006-2-23
	if(m_bDrawCross&&m_bMouseInRect)
	{
		DrawCrossCursor(pDC);	
		//////////////////////////////////////////////////////////////////////////
		//д����Ϣ��ǩ
		if((0 != m_nValueNum)&&(-1 != m_nVertStation )&&(m_nNowShowDate != -1))
			m_InfoTable.Draw(pDC);
	}
	if(m_pDistractStatTable&&(SECONDCLICK == m_nDistractStatState))
		m_pDistractStatTable->Draw(pDC);//����ͳ�Ʊ��
	if((FIRSTCLICK == m_nDistractStatState)||(SECONDCLICK == m_nDistractStatState))
	{//����������
		DrawDistractLine(pDC);
	}

	//////////////////////////////////////////////////////////////////////////
	//����ѡ������
	DrawSelectArea(pDC);
	return TRUE;
}

//���������������


//����
void CHisTechObj::LeftMove()
{
//	if(m_bRequestedData||(m_pRecvKUnit == NULL))return;//�����û�з�������

	if(!m_bDrawCross||(m_nVertStation < 0))
	{
		m_nVertStation = m_nHoriPoint - 1;
	}
	else if((m_nVertStation == 0))
	{//��������
		if((m_nMaxParameret >= m_nDataBegin))
		{//�ڴ�������
			m_sLastSize = 1;
			m_nLeftMoveRequestStatus = LEFTMOVEREQUEST;	
			RequestData();
			return;
		}
		m_nDataBegin --;
		m_nDataEnd --;
		TransKDataToPic();
	}
	else
	{
		m_nVertStation--;	
	}

	int nStation = m_nVertStation;
	if(nStation >= m_nValueNum)nStation = m_nValueNum-1;//���һ������
	if(nStation >= m_nValueNum)nStation = m_nValueNum-1;//���һ������

	if( m_pHisCurvePic[0].GetPriceFromStation(m_nHoriStation) == -1)
	{
		int nValue = (int)m_pTranslateKUnit[nStation].m_fClosePrice;//�õ����̼�
		m_nHoriStation = m_pHisCurvePic[0].GetStationFromValue(nValue);//�õ���ǰλ��
	}
	
	TransCurStationToPic();//���ݵ�ǰλ�ø���ͼ		
	//////////////////////////////////////////////////////////////////////////
	// add by david at 2004-7-23
	m_nNowShowDate = m_pTranslateKUnit[nStation].m_uiDate&0x7fffffff;//�õ�����
	//////////////////////////////////////////////////////////////////////////	
}

//����
void CHisTechObj::RightMove()
{
	if((m_pRecvKUnit == NULL))return;//�����û�з�������

	int nDataStation;
	if(!m_bDrawCross||(m_nVertStation <0))
	{
		m_nVertStation = 0;
	}
	else if(m_nVertStation >= m_nHoriPoint-1) //�Ѿ����˱���ĻK�����ұ�
	{
		//���㵱ǰ����λ��
		nDataStation = m_nDataBegin +m_nVertStation;
		
		if (nDataStation >= (m_nValueNum - 1))
			return; //K�����Ѿ���ʾ�����һ��
		
		//������λ��
		if(m_nRecsOfUnit <= 1)
		{
			m_nVertStation++;
		}
		else
		{	
			m_nVertStation++;
		}
		if((m_nVertStation >= m_nHoriPoint-1 )
			&&!((m_nDataBegin == 0)&&(m_nDataEnd == m_nValueNum-1)))//�Ѿ���ʾ����Ļ�����ұߣ���Ļ�����ƶ�һ��K��λ��
		{
			m_nDataBegin++;

			m_nDataEnd= m_nDataBegin+ m_nHoriPoint*m_nRecsOfUnit;

			if(m_nDataEnd>m_nValueNum)
				m_nDataEnd = m_nValueNum;
			
			m_nNowShowDate= -1;
			m_nVertStation = m_nHoriPoint-1;//���һ��

			TransKDataToPic();
		}		
		//û�дﵽ���ѹ��ϵ������ʾ��һ������		
	}
	else//û�е����ұ�
	{
		m_nVertStation++;
	}
	
	int nValue;
	
	//���㵱ǰ����λ��
	nDataStation = m_nVertStation;

	if(nDataStation >= m_nValueNum) nDataStation = m_nValueNum-1;//���һ������
	
	if( m_pHisCurvePic[0].GetPriceFromStation(m_nHoriStation) == -1)
	{
		nValue = (int)m_pTranslateKUnit[nDataStation].m_fClosePrice;//�õ����̼�
		m_nHoriStation = m_pHisCurvePic[0].GetStationFromValue(nValue);//�õ���ǰλ��
	}
	
	TransCurStationToPic();//���ݵ�ǰλ�ø���ͼ		

	m_nNowShowDate = m_pTranslateKUnit[nDataStation].m_uiDate&0x7fffffff;//�õ�����
	//////////////////////////////////////////////////////////////////////////				
}

void CHisTechObj::CtrlLeftMove()
{
//	if(m_bRequestedData||(m_pRecvKUnit == NULL))return;//�����û�з�������
	//////////////////////////////////////////////////////////////////////////
	//���¼������ݵ�����

	int nDisplayNum;
	nDisplayNum = m_nDataEnd-m_nDataBegin;
	if((m_nDataBegin <= 0))
	{
		m_sLastSize =nDisplayNum;
		m_nLeftMoveRequestStatus = CTRLLEFTMOVEREQUEST;		
		RequestData();
		return;
	}
	/////////////////////////////////////////////////////////////////////////
	m_nDataBegin -= nDisplayNum;
	if(m_nDataBegin<0)
	{
		m_nDataBegin = 0;
	}
	m_nDataEnd = m_nDataBegin + nDisplayNum;
	
	if(m_nDataEnd>= m_nValueNum)
		m_nDataEnd = m_nValueNum;
	
	m_nNowShowDate= -1;
	m_nVertStation = -1;
	TransKDataToPic();//
}

void CHisTechObj::CtrlRightMove()
{
	if((m_pRecvKUnit == NULL))return;//�����û�з�������
	if(m_nDataEnd >= m_nValueNum) return;//���ݵ���ֱ�ӷ���	

	int nDisplayNum;
	nDisplayNum = (m_nDataEnd-m_nDataBegin);
	m_nDataBegin += nDisplayNum;	//���������	
	m_nDataEnd = m_nDataBegin + nDisplayNum;	

	if(m_nDataEnd > m_nValueNum)
	{//����Խ��
		m_nDataEnd = m_nValueNum;
		m_nDataBegin = m_nDataEnd - nDisplayNum;
		if(m_nDataBegin<0)
			m_nDataBegin = 0;
	}
	m_nNowShowDate= -1;
	m_nVertStation = -1;
	TransKDataToPic();//	
}

void CHisTechObj::CalcDistractInfo()
{
	if(m_pDistractStatTable)
	{
		delete m_pDistractStatTable;
		m_pDistractStatTable = NULL;
	}
	m_pDistractStatTable = new CDistractStatTable;
	m_pDistractStatTable->Init();//��ʼ�����
	CRect rRect;
	rRect.bottom = m_TimeRect.top- 20;
	rRect.top =  rRect.bottom - 7*m_pDistractStatTable->GetItemHeight();
	rRect.left = m_TimeRect.left+ (m_TimeRect.Width()-80*5)/2;
	rRect.right = m_TimeRect.left+ (m_TimeRect.Width()+80*5)/2;
	m_pDistractStatTable->MoveTo(rRect);//��������
	
	m_pDistractStatTable->SetStockInfo(m_pCodeInfo);
	int nStatIdx;

	nStatIdx = m_nFirstClickState;

	CMulQuantitySequence seq;
	memset(&seq, 0, sizeof(seq));
	
	seq.m_nSize = m_nSecondClickState - m_nFirstClickState;
//	seq.m_ppValues = (long**)(m_pRecvKUnit + nStatIdx);
	seq.m_ppValues = (long**)(m_pTranslateKUnit+m_nDataBegin+nStatIdx);
	
	m_pDistractStatTable->UpdateData(&seq,0);//��������
	
}
void CHisTechObj::DrawSelectArea(CDC* pDC)
{
	if(!m_bDrawSelect)return;
	CRect SelectRect;
	SelectRect =m_rectSelect;
	SelectRect.NormalizeRect();
	int nOldMode = pDC->SetROP2(R2_NOT);
	pDC->MoveTo(SelectRect.left,SelectRect.top);
	pDC->LineTo(SelectRect.left,SelectRect.bottom);
	pDC->LineTo(SelectRect.right,SelectRect.bottom);
	pDC->LineTo(SelectRect.right,SelectRect.top);
	pDC->LineTo(SelectRect.left,SelectRect.top);
	pDC->SetROP2(nOldMode);
}

void CHisTechObj::DrawDistractLine(CDC *pDC)
{
	int nOldMode = pDC->SetROP2(R2_NOT);
   
	int nBeginXPos = 0,nEndXPos = 0;

	int nBeginStation = m_nFirstClickState;
	if( nBeginStation < m_nHoriPoint)
	{
		nBeginXPos = m_pHoriPoint[nBeginStation]+m_nKUnitWidth/2;
		pDC->MoveTo(nBeginXPos,m_KLineRect.top+16);
		pDC->LineTo(nBeginXPos,m_KLineRect.bottom);
	}
	else
	{
		nBeginXPos = m_KLineRect.left; 
	}
	
	if(m_nDistractStatState == SECONDCLICK)
	{
	    int nEndStation = (m_nSecondClickState-1);
		if( nEndStation < m_nHoriPoint)
		{
			nEndXPos = m_pHoriPoint[nEndStation] +m_nKUnitWidth/2;
			pDC->MoveTo(nEndXPos,m_KLineRect.top+16);
			pDC->LineTo(nEndXPos,m_KLineRect.bottom);
		}
		else
		{
			if( nEndStation > nBeginStation )
			{
				nEndXPos = m_KLineRect.right;
			}
			else
			{
				nEndXPos = m_KLineRect.left;
			}
		}
	}
	else
	{
		nEndXPos = m_pHoriPoint[m_nVertStation];
	}

	if( nBeginXPos != nEndXPos)
	{
		int nYPos= m_KLineRect.top+m_KLineRect.Height()/2;
		pDC->MoveTo(nBeginXPos,nYPos);
		pDC->LineTo(nEndXPos,nYPos);
	}
	pDC->SetROP2(nOldMode);
}

void CHisTechObj::ReAssignPicNum(int nNewPicNum)
{
	if( nNewPicNum > m_nCurrentPicNum)
	{
		vector<CString> vecIndexName;
		int i;
		for( i=0; i<m_nCurrentPicNum; i++)
		{
			vecIndexName.push_back(m_pHisCurvePic[i].GetTechType());			
		}

		delete []m_pHisCurvePic;
		m_pHisCurvePic = new CTechCreator[nNewPicNum];
		vector<CString>::iterator iterName;
		i = 0;
		for(iterName = vecIndexName.begin();
			iterName != vecIndexName.end();
			iterName++)
		{
			m_pHisCurvePic[i].SetTechType(*iterName);
			i++;
		}
				
		const char *pszIndex;		
		for(i = 0; i < nNewPicNum - m_nCurrentPicNum; i++)
		{
			pszIndex = CFormulaFace::FormuCode(i);
			if (pszIndex != NULL)
				m_pHisCurvePic[m_nCurrentPicNum+i].SetTechType(CString(pszIndex));
		}
	}
	m_nCurrentPicNum = nNewPicNum;
}

void CHisTechObj::DeleteIndex()
{
	if((m_nCurrentPicNum<2)||(m_nCurrentTechIndex == 0))return;//ֻ��һ��ͼ�Σ�����ѡ������ͼ
	CTechCreator* pTemp = new CTechCreator[m_nCurrentPicNum-1];
	for(int i = 0;i<m_nCurrentPicNum-1;i++)
	{
		if(i<m_nCurrentTechIndex)
			pTemp[i].SetTechType(m_pHisCurvePic[i].GetTechType());
		else
			pTemp[i].SetTechType(m_pHisCurvePic[i+1].GetTechType());			
	}
	delete []m_pHisCurvePic;
	m_pHisCurvePic = pTemp;
	m_nCurrentPicNum --;
	RecalSubObjSize();
}

// �������µ����ݸ���k�ߵĶ�Ӧֵ
void CHisTechObj::UpdateKLineMinPriceByNowData( float &rfOldValue,float fNewValue)
{
	// ����ʵʱ����Ϊ0��ֵ
	if( fNewValue == 0.000f)
		return;
	rfOldValue = fNewValue;
}

BOOL CHisTechObj::MakeDayData(StuRecvQuoteInfo* pQuote)
{
	if(m_nValueNum <1)return FALSE;	
	float fNewPrice;
	fNewPrice = (float)(pQuote->uiPrice/pow(10,(double)m_pCodeInfo->cPriceUnit));

	CTime quotetime(pQuote->lastTick);
	if(m_nCircleInfo<= 60)
	{//�����ߡ�����
		CTimeValue timevalue;
		timevalue.SetCurrentDate(CPublic::Instance()->GetDate());
		int nMinute;
		nMinute = quotetime.GetHour()*100+quotetime.GetMinute();
		int nDay;
		nDay= quotetime.GetMonth()*100+quotetime.GetDay();
		// add by david gong at 20140126
		int nLastTime;
		nLastTime = m_pRecvKUnit[m_nValueNum-1].m_uiDate;
		BOOL bCondication;
		bCondication= FALSE;
		int nLastMinute;
		nLastMinute = nLastTime%10000;
		if((nMinute == 0)&&(nLastMinute ==nMinute)) bCondication= TRUE;
		if((nMinute<(nLastMinute+m_nCircleInfo))&&(nMinute!= 0))bCondication = TRUE;
		if(!bCondication)
			//////////////////////////////////////////////////////////////////////////
		{//������ǰ����
			SHistoryUnit * pTemp = new SHistoryUnit[m_nValueNum+1];
			memcpy(pTemp,m_pRecvKUnit,m_nValueNum*sizeof(SHistoryUnit));
			if(m_pRecvKUnit)delete []m_pRecvKUnit;
			m_pRecvKUnit = pTemp;
			m_nValueNum++;//������
			/////////////////////////////////////////////////////////////////////////
			if((m_nDataEnd-m_nDataBegin)>=m_sLastSize)
				m_nDataBegin++;
			m_nDataEnd++;
			int nCurrentDate;
			////////////////////////////////////////////////////////////////////////// 	
			if(m_nCircleInfo != 1)
			{
				int nTransTime;
				nTransTime = nMinute/100*60+nMinute%100;
				nTransTime = (nTransTime/m_nCircleInfo)*m_nCircleInfo;
				nTransTime = nTransTime/60*100+nTransTime%60;
				nCurrentDate =nDay*10000+ nTransTime;	
			}
			else
			{
				nCurrentDate =nDay*10000+ nMinute;		
			}

			m_pRecvKUnit[m_nValueNum-1].m_uiDate = nCurrentDate;
			m_pRecvKUnit[m_nValueNum-1].m_fMaxPrice = fNewPrice;
			UpdateKLineMinPriceByNowData(m_pRecvKUnit[m_nValueNum-1].m_fMinPrice,fNewPrice);
			m_pRecvKUnit[m_nValueNum-1].m_fClosePrice = fNewPrice;
			m_pRecvKUnit[m_nValueNum-1].m_fOpenPrice = fNewPrice;

			int nPrevSum;
			nPrevSum = 0;
			int nCount;
			nCount = m_nValueNum-2;
			int nDate;
			nDate = CPublic::Instance()->GetDate()%10000;
			while((nCount>=0)&&(m_pRecvKUnit[nCount].m_uiDate/10000 == nDate))
			{
				nPrevSum += m_pRecvKUnit[nCount].m_uiSum;
				nCount--;
			}
		}
		else
		{//�ڵ�ǰʱ��������
			if(m_pRecvKUnit[m_nValueNum-1].m_fMaxPrice < fNewPrice)
			{
				m_pRecvKUnit[m_nValueNum-1].m_fMaxPrice = fNewPrice;
			}
			if(m_pRecvKUnit[m_nValueNum-1].m_fMinPrice> fNewPrice)
			{
				UpdateKLineMinPriceByNowData(m_pRecvKUnit[m_nValueNum-1].m_fMinPrice,fNewPrice);
			}
			m_pRecvKUnit[m_nValueNum-1].m_fClosePrice = fNewPrice;
		}
		m_InfoTable.UpdateNewPrice(fNewPrice);//�¼۸�
	}
	else
	{//���ߡ�����
		int nDate;
		nDate = CPublic::Instance()->GetDate();
		if(m_nValueNum<1)return FALSE;
		if(m_pRecvKUnit[m_nValueNum-1].m_fMaxPrice< fNewPrice)
		{
			m_pRecvKUnit[m_nValueNum-1].m_fMaxPrice = fNewPrice;
		}
		if(m_pRecvKUnit[m_nValueNum-1].m_fMinPrice> fNewPrice)
		{
			UpdateKLineMinPriceByNowData(m_pRecvKUnit[m_nValueNum-1].m_fMinPrice,fNewPrice);
		}
		m_pRecvKUnit[m_nValueNum-1].m_fClosePrice = fNewPrice;
	}
	return TRUE;
}
void CHisTechObj::ResetTechData()
{
	for(int i = 0; i < m_nCurrentPicNum; i++)
	{
		m_pHisCurvePic[i].ResetData();
	}
}

void CHisTechObj::SetFormulaCommand(char *pCommand)
{
	if(m_nCurrentPicNum<1)return;
// 	if((0 == strcmp(pCommand,("BOLL")))||(0 == strcmp(pCommand,("MA"))))
// 	{//����ͨ����MA����ʾ����������
// 		m_pHisCurvePic[0].SetTechType(CString(pCommand));
// 	}
// 	else
	{
		//////////////////////////////////////////////////////////////////////////
		//modify by david gong at 2013/09/18
		//�޸�ָ����ʾ��Ϣ
		if(m_nCurrentTechIndex<m_nCurrentPicNum&&m_nCurrentTechIndex>=0)
			m_pHisCurvePic[m_nCurrentTechIndex].SetTechType(CString(pCommand));
		//////////////////////////////////////////////////////////////////////////
	}
	RecalSubObjSize();
    SaveIndexStatusParam();
}

// ����ָ��״̬����
BOOL CHisTechObj::SaveIndexStatusParam()
{
	//ָ�����
	CIniFile  iniFile;
	iniFile.Create( m_strIndexParamPath);
	iniFile.SetVarInt( _T("INDEX"), _T("NUM"), m_nCurrentPicNum);
	CString strIndex;
	for(int i = 0; i < m_nCurrentPicNum; i++)
	{
		strIndex += m_pHisCurvePic[i].GetTechType();
		strIndex += _T(",");
	}
	iniFile.SetVarStr( _T("INDEX"), _T("INDEX"), strIndex);
	return TRUE;
}

// װ��ָ��״̬����
BOOL CHisTechObj::LoadIndexStatusParam()
{
	//���������ļ�
	CIniFile  iniFile;
	iniFile.Create( m_strIndexParamPath);
	int nIndexCount;
	if( !iniFile.GetVarInt( _T("INDEX"), _T("NUM"), nIndexCount ))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10003),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if(m_pHisCurvePic)
	{
		delete []m_pHisCurvePic;
		m_pHisCurvePic = NULL;
	}	
	
	m_pHisCurvePic = new CTechCreator[nIndexCount];
	m_nCurrentPicNum = nIndexCount;

	CString strIndexName;
	if( !iniFile.GetVarStr( _T("INDEX"), _T("INDEX"), strIndexName))
	{
		MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10003),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	int iPos;
	for(int iRec = 0; iRec < m_nCurrentPicNum; iRec++)
	{
		iPos = strIndexName.Find(_T(","),0);
		m_pHisCurvePic[iRec].SetTechType(strIndexName.Left(iPos));
		iPos++;
		strIndexName = strIndexName.Mid(iPos);
	}
	//////////////////////////////////////////////////////////////////////////
	// add by david gong at 20140806
	//��ȡĬ��ָ������
	m_strDefaultIndex = _T("MA,MACD,KDJ,");
	if( !iniFile.GetVarStr( _T("INDEX"), _T("DEFAULTINDEX"), strIndexName))
	{
		//MessageBox(NULL,CLanguageInterface::Instance()->GetStr(10003),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	m_strDefaultIndex = strIndexName;

	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}


BOOL CHisTechObj::TreatDayDataArrived(const char *pHisotyData)
{
	SNormaHead * pDayHead = (SNormaHead*)(pHisotyData);
	SHisKUnit* pDayData = (SHisKUnit*)(pDayHead+1);
	if((pDayHead->m_usSize<= 0))
	{
		TransKDataToPic();//�������ݵ���ͼ
		return FALSE;
	}

	SHistoryUnit* pTemp;		  //��ʱ����
	pTemp = new SHistoryUnit[m_nValueNum + pDayHead->m_usSize];


	int nStation;
	nStation =0;
	float fPow;
	fPow = (float)pow(10,(double)m_pCodeInfo->cPriceUnit);
	int nValidateNum;
	nValidateNum = 0;
	for(int nCount = 0;nCount< pDayHead->m_usSize;nCount++)
	{
		if(m_nCircleInfo >= DAY_HISK)
		{
			pTemp[nCount].m_uiDate = pDayData[nCount].uiDate;
		}
		else
		{
			pTemp[nCount].m_uiDate =pDayData[nCount].uiDate%10000*10000+ pDayData[nCount].uiTime;
		}
		pTemp[nCount].m_uiSum = pDayData[nCount].uiTime;
		pTemp[nCount].m_uiVolume = 0;
		pTemp[nCount].m_fClosePrice = (float)pDayData[nCount].uiClosePrice/fPow;
		pTemp[nCount].m_fMaxPrice = (float)pDayData[nCount].uiMaxPrice/fPow;
		pTemp[nCount].m_fMinPrice = (float)pDayData[nCount].uiMinPrice/fPow;
		pTemp[nCount].m_fOpenPrice = (float)pDayData[nCount].uiOpenPrice/fPow;
		nValidateNum++;
	}
	
	if(m_pRecvKUnit)
	{
		// �����ǰK�������Ǵ����յģ�����Ҫ���������K�ߺϲ�����ʷK����ȥ
		if( m_nValueNum == 1 && CombineTodayKLine(m_nCircleInfo,m_pRecvKUnit,&pTemp[pDayHead->m_usSize-1]))
		{
			m_nValueNum =0;				
		}
		memcpy(pTemp+nValidateNum,m_pRecvKUnit,m_nValueNum*sizeof(SHistoryUnit));
		delete []m_pRecvKUnit; //ɾ��������
		m_pRecvKUnit = NULL;
		ResetTechData();
	}
	m_pRecvKUnit = pTemp;
	m_nValueNum += nValidateNum;

	if((m_sLastSize > pDayHead->m_usSize))
	{//���õ�������С�ڵõ�������
		m_sRequestStatus = TP_END;//�����������
	}
	//////////////////////////////////////////////////////////////////////////
	//����ʼ����ʾ
	if(LEFTMOVEREQUEST == m_nLeftMoveRequestStatus)
	{//���ƹ��
		m_nDataBegin = m_sLastSize -1;//���ݿ�ʼλ��
		m_nDataEnd = m_nDataBegin+(m_nHoriPoint)*m_nRecsOfUnit;
		m_nLeftMoveRequestStatus = NORMALREQUEST;

		m_nVertStation = 0;
		m_nNowShowDate = -1;
	}
	else if(CTRLLEFTMOVEREQUEST == m_nLeftMoveRequestStatus)
	{//Ctrl����
		m_nDataBegin = 0;
		m_nDataEnd = m_nDataBegin+(m_nHoriPoint)*m_nRecsOfUnit;

		m_nLeftMoveRequestStatus = NORMALREQUEST;
		m_nNowShowDate = -1;
	}
	else if(NORMALREQUEST == m_nLeftMoveRequestStatus)
	{//����ѹ��
		m_nDataEnd = m_nValueNum;
		m_nMemoryStation += pDayHead->m_usSize;	

		if(TP_END == m_sRequestStatus)
		{
			m_nDataBegin = m_nDataEnd - m_sLastSize;
			if(m_nDataBegin<0)m_nDataBegin = 0;
			m_nMemoryStation -= m_nDataBegin;
			m_nVertStation = m_nMemoryStation/m_nRecsOfUnit;
		}
		else
		{
			m_nDataBegin = 0;
			m_nParameret = m_nMaxParameret;
		}
		m_nVertStation = m_nMemoryStation/m_nRecsOfUnit - m_nParameret;
		m_nMemoryStation = 0;
	}
	else if(FIRSTGETDATA == m_nLeftMoveRequestStatus)
	{
		if((m_sRequestStatus != TP_END))
		{
			if(m_nValueNum>m_sLastSize)
				m_nDataBegin = m_nDataEnd;
		}
		else
		{
			m_nParameret += m_nValueNum - m_sLastSize;
			if(m_nParameret<0)m_nParameret =0;
		}

		m_nDataEnd = m_nValueNum;	
		m_nLeftMoveRequestStatus = NORMALREQUEST;
		m_nVertStation = -1;
	}
	return TRUE;
}



BOOL CHisTechObj::MouseRBtnUp(CPoint point)
{
	m_pHisCurvePic[0].MouseRBtnUp(point);
	return TRUE;
}

void CHisTechObj::CtrlUpZoom()
{
	m_pHisCurvePic[0].ZoomVertAxis(false);
}

void CHisTechObj::CtrlDownZoom()
{
	m_pHisCurvePic[0].ZoomVertAxis(true);
}



int CHisTechObj::FindStationFromDate(int nDate)
{
	nDate &=0x7fffffff;
	int nCount;
	int nStation;
	nStation = -1;
	if(m_pRecvKUnit[0].m_uiDate >nDate)return -1;
	int nNewDate;
	for(nCount=0;nCount<m_nValueNum;nCount++)
	{
		nNewDate = m_pRecvKUnit[nCount].m_uiDate&0x7fffffff;
		if(nDate == nNewDate)
		{
			nStation= nCount;
			break;
		}
		else if(nDate < nNewDate)
		{
			nStation =nCount-1;//��ǰλ��
			break;
		}
	}
	return nStation;
}