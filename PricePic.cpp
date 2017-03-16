// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "PricePic.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPricePic::CPricePic()
{
	m_iDeimal = 3;
	m_pPricePoints = NULL;
	m_nShowMode = 0;
	m_lMinPrice = 0;
	m_lMaxPrice = 100;
	m_nTradeTime = 4;
}

CPricePic::~CPricePic()
{
	ClearData();
}

// ����������ڴ�����
void CPricePic::ClearData()
{
	if(m_pPricePoints)
	{
		delete []m_pPricePoints;
		m_pPricePoints = NULL;
	}
}

//�������ܣ���������
void CPricePic::MoveTo(CRect rRect,BOOL bAuto)
{
	if((m_nShowMode&AUTOFITLEFTSIZE)&&bAuto)
	{
		m_OrignRect = rRect;
	}
	else
	{
		CDynBasePic::MoveTo(rRect);
		m_rectDraw = rRect;
		//////////////////////////////////////////////////////////////////////////
		//���¼�������λ��
		if((m_lHoriPoint >0))
		{
			SetHoriPoint(m_lHoriPoint);
			RegetStation();
		}

	}
}

// Ϊ�˿̶���ʾ���������������Сֵ
void CPricePic::JustMaxMinValueForDispaly(long &lMaxValue,long &lMinValue,
										  long lRefMidValue,int nVAxisCount,
										  long lUpLimitValue,long lDownLimitValue)
{
//	GetFormatMaxMin(lMaxValue,lMinValue,lRefMidValue,nVAxisCount);//�õ���Ӧλ��
	long lMin;
	lMin = (long)((double)lMaxValue*0.999);
	if(lMinValue>lMin)lMinValue = lMin;

}


//�������ܣ���������
BOOL CPricePic::UpdateData(LPVOID pNewData)
{
	CMulQuantitySequence *pData = (CMulQuantitySequence *)pNewData;
	m_nValueNum = pData->m_nSize;
	m_lReference = pData->m_lReference;//���ռ�
	m_pPrice = pData->m_ppValues[0];//�ɽ���

	long lUpLimitPrice = pData->m_lMax; //��ͣ��
	long lDownLimitPrice = pData->m_lMin;
	//////////////////////////////////////////////////////////////////////////
	//�õ�������
	GetMaxMinPrice(m_lMinPrice,m_lMaxPrice,m_pPrice,m_nValueNum);//�õ�������


	m_nVAxisCount = GetVertAxisCellNum(m_rectDraw.Height());
	if(m_nVAxisCount== 0)
		return FALSE;

	JustMaxMinValueForDispaly(m_lMaxPrice,m_lMinPrice,m_lReference,
		                  m_nVAxisCount,lUpLimitPrice,lDownLimitPrice);
	
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��ǰ������
	ClearData();
	if (m_nValueNum <= 0)
		return FALSE;//û�����ݷ���
	m_pPricePoints = new long[m_nValueNum];
	RegetStation();
	return TRUE;	
}

//�������ܣ���ͼ
BOOL CPricePic::Draw(CDC* pDC)
{
	m_pDC = pDC;
	//////////////////////////////////////////////////////////////////////////
	COLORREF clrRise;
	clrRise = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	COLORREF clrEquire;
	clrEquire = RGB(255,0,0);//CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR);
	COLORREF clrDown;
	clrDown =  CInterfaceManager::Instance()->GetSysColor(ID_DOWN_COLOR);
	COLORREF clrLine;
	clrLine = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	//////////////////////////////////////////////////////////////////////////
	//��������
	CFont * pFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	SetFont(pFont);
	//////////////////////////////////////////////////////////////////////////
	if(m_nShowMode&AUTOFITLEFTSIZE)
	{
		CFont* pOldFont = pDC->SelectObject(pFont);
		CSize textSize = pDC->GetTextExtent(_T("9999.9999 "));
		CRect rRect;
		rRect = m_OrignRect;
		rRect.left += textSize.cx;
		pDC->SelectObject(pFont); 
		MoveTo(rRect,FALSE);
	}
	//////////////////////////////////////////////////////////////////////////
	if(m_nTradeTime<=0)return FALSE;
	long* pStation  = new long[m_nTradeTime];
	for(int i = 0;i<m_nTradeTime;i++)
		pStation[i] = 60*i;
// 	AXISMODE ShowMode;
// 	if(m_nShowMode&NORIGHTRAWRULER)
// 		ShowMode = SINGLESIDEDOUBLECOLOR;//ʵʱ����ģʽ
// 	else
// 		ShowMode = DOUBLESIDEDOUBLECOLOR;//���ͬ��ģʽ

//	ShowMode = SINGLESIDESINGLECOLOR;
	DrawAxisStyle(m_rectDraw,
					m_lMinPrice,
					m_lMaxPrice,
					m_nVAxisCount,
					clrLine,
					clrEquire,
					pStation,
					m_nTradeTime,
					m_nTradeTime*10);

	//���ɽ���
	COLORREF clrLineColor;
	clrLineColor = CInterfaceManager::Instance()->GetSysColor(ID_INDEXLINE1_COLOR);
	DrawLinkLine(m_rectDraw,
				1,
				clrLineColor,
				m_lMaxPrice,
				m_lMinPrice,
				m_pPricePoints,
				m_nValueNum);//�����ɽ�����

	//////////////////////////////////////////////////////////////////////////
	DrawTitleText();
	DrawCurrentStationLine();//����ǰλ��	

	delete []pStation;
	return TRUE;	
}

//�������ܣ��õ���ʽ���������ͼ�
void CPricePic::GetFormatMaxMin(long& lMaxPrice, long& lMinPrice, long lRefPrice,int nVAxisCount)
{
	long lMaxDiff,lMinDiff;
	lMaxDiff = abs(lMaxPrice - lRefPrice);//��߼ۺ������̲��
	lMinDiff = abs(lMinPrice - lRefPrice);//��ͼۺ������̲��

	if(0 == nVAxisCount%2)
		nVAxisCount ++;
	int iItemCount;
	iItemCount = nVAxisCount/2;
	if(0 == iItemCount) 
		iItemCount = 1;
	long lDiffent;
	if(lMaxDiff> lMinDiff)
		lDiffent = (lMaxDiff/iItemCount+1)*iItemCount;
	else
		lDiffent = (lMinDiff/iItemCount+1)*iItemCount;

	if(m_iDeimal == 3)
	{
		if(lDiffent < nVAxisCount/2) 
			lDiffent= nVAxisCount/2;
	}
	else
	{
		if(lDiffent/10 <nVAxisCount/2)
			lDiffent =nVAxisCount/2*10;
	}

	// Ϊ�˱���ָ�������������С����ʾͼ�ο��ţ�������С���䷶ΧΪ 1%
	if( lDiffent < (lRefPrice * 0.01))
		lDiffent = lRefPrice * 0.01;
	
	lMaxPrice = lRefPrice + lDiffent;
	lMinPrice = lRefPrice - lDiffent;
}

//�������ܣ�ͨ���۸�õ�λ��
int CPricePic::GetStationFromPrice(long lPrice)
{
	if(m_lMaxPrice == m_lMinPrice)
	{
		return m_rectDraw.bottom;
	}
	long lStation;
	double dbinfo;
	dbinfo = (lPrice-m_lMinPrice);
	dbinfo *= m_rectDraw.Height();
	dbinfo /= (m_lMaxPrice - m_lMinPrice);
	lStation =(long)dbinfo;
	lStation = m_rectDraw.bottom - lStation;
	if(lStation >= m_rectDraw.bottom)
		lStation =m_rectDraw.bottom-1;
	if(lStation <= m_rectDraw.top)
		lStation = m_rectDraw.top+1;
	return lStation;
}

//�������ܣ����¼���λ��
void CPricePic::RegetStation()
{
	m_nVAxisCount = GetVertAxisCellNum(m_rectDraw.Height());
	if(0 == m_nVAxisCount%2)
	{
		m_nVAxisCount--;
	}
	//ͨ���۸��������λ��ֵ
	long lCount;
	for(lCount = 0;lCount<m_nValueNum;lCount++)
	{
		if(m_pPrice[lCount] != 0x7fffffff)
		{
			m_pPricePoints[lCount] = GetStationFromPrice(m_pPrice[lCount]);//�۸�
		}
		else
		{
			if(lCount == 0)
				m_pPricePoints[lCount] = m_rectDraw.top;
			else
				m_pPricePoints[lCount]= m_pPricePoints[lCount-1];
		}
	}
}