// DistractStatTable.cpp: implementation of the CDistractStatTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "DistractStatTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistractStatTable::CDistractStatTable()
{
	m_pCodeInfo = NULL;
	m_nOpenDate = 0;
	m_nCloseDate = 0;
	m_nTotalNum = 0;
}

CDistractStatTable::~CDistractStatTable()
{

}

BOOL CDistractStatTable::Init()
{
	SetColorsAndFonts();
	
	InsertColumn(0,_T(""),LVCFMT_RIGHT,70);
	InsertColumn(1,_T(""),LVCFMT_RIGHT,90);
	InsertColumn(2,_T(""),LVCFMT_RIGHT,80);
	InsertColumn(3,_T(""),LVCFMT_RIGHT,70);
	InsertColumn(4,_T(""),LVCFMT_RIGHT,90);
	SetStyle(LVS_NOCOLUMNHEADER);
	SetExtendStyle(LVS_EX_GRIDLINES);

	int nCol, nRow;
	int nCount;
	for(nCount = 0;nCount<14;nCount++)
	{
		nCol = nCount/2;
		nRow = nCount%2;
		if(nRow>0)
			nRow = 3;
		InsertItem(nCol,nRow,CLanguageInterface::Instance()->GetStr(20110+nCount));
	}	
	return TRUE;
}

BOOL CDistractStatTable::UpdateData(LPVOID lpData,int nDataLen)
{
	if (lpData == NULL ) return FALSE;
	CMulQuantitySequence* pSeq = (CMulQuantitySequence*)lpData;
	SHistoryUnit * pKunit = (SHistoryUnit*) pSeq->m_ppValues;
	int nKUnitNum = pSeq->m_nSize;
	int nCount;
	if(0 == nKUnitNum)return FALSE;
	//////////////////////////////////////////////////////////////////////////
	//ͳ������
	struct SDistractStat
	{
		int nOpenDate;//��ʼʱ��
		float fOpenPrice;//���̼�
		int nCloseDate;//����ʱ��
		float fClosePrice;//���̼�
		int nMaxDate; //��߼�����
		float fMaxPrice;//��߼�
		int nMinDate;//��ͼ�����
		float fMinPrice;//��ͼ�
		float fAverPrice;//����
		float fRise;//�ǵ�
		float nRange;//�ǵ�����
		float fSwing;//���
		int nDownDay;//�µ�����
		int nRiseDay;//��������	
		int nEquireDay;//ƽ������
	} StatInfo;//
	memset(&StatInfo,0,sizeof(StatInfo));
	//���̼�
	StatInfo.fOpenPrice = pKunit[0].m_fOpenPrice;
	StatInfo.nOpenDate = pKunit[0].m_uiDate&0x7fffffff;
	//���̼�
	StatInfo.nCloseDate = pKunit[nKUnitNum-1].m_uiDate&0x7fffffff;
	StatInfo.fClosePrice = pKunit[nKUnitNum-1].m_fClosePrice;//

	StatInfo.fMaxPrice = 0;
	StatInfo.fMinPrice = 10000;

	for(nCount= 0;nCount<nKUnitNum;nCount++)
	{
		if(StatInfo.fMaxPrice <(int)pKunit[nCount].m_fMaxPrice)
		{//��߼�b
			StatInfo.nMaxDate = pKunit[nCount].m_uiDate&0x7fffffff;
			StatInfo.fMaxPrice = pKunit[nCount].m_fMaxPrice;
		}
		if(StatInfo.fMinPrice > (int)pKunit[nCount].m_fMinPrice)
		{//��ͼ�
			StatInfo.nMinDate = pKunit[nCount].m_uiDate&0x7fffffff;
			StatInfo.fMinPrice = pKunit[nCount].m_fMinPrice;
		}
		if(pKunit[nCount].m_fClosePrice > pKunit[nCount].m_fOpenPrice)
		{
			StatInfo.nRiseDay ++;//����
		}
		else if(pKunit[nCount].m_fClosePrice < pKunit[nCount].m_fOpenPrice)
		{
			StatInfo.nDownDay++;//�µ�
		}
		else
			StatInfo.nEquireDay++;//ƽ��
			
	}
	StatInfo.fRise = StatInfo.fClosePrice - StatInfo.fOpenPrice;//�ǵ�
	if( StatInfo.fOpenPrice > 0)
	{
	   StatInfo.nRange = StatInfo.fRise*100/StatInfo.fOpenPrice;//����
	   StatInfo.fSwing = (StatInfo.fMaxPrice - StatInfo.fMinPrice)*100/StatInfo.fOpenPrice;//���
	}
	//////////////////////////////////////////////////////////////////////////
	//���뵥Ԫ
	CString szText;
	float fPrice;
	fPrice = StatInfo.fOpenPrice;
	szText.Format(m_szFormat,fPrice);
	InsertItem(0,1,szText);
	szText.Format(_T("%d"),StatInfo.nOpenDate);
	InsertItem(0,2,szText);//����

	fPrice = StatInfo.fRise;
	szText.Format(m_szFormat,fPrice);
	InsertItem(0,4,szText);//�ǵ�

	fPrice = StatInfo.fClosePrice;
	szText.Format(m_szFormat,fPrice);
	InsertItem(1,1,szText);
	szText.Format(_T("%d"),StatInfo.nCloseDate);
	InsertItem(1,2,szText);//����

	szText.Format(_T("%.2f%%"),StatInfo.nRange);
	InsertItem(1,4,szText);//����

	fPrice = StatInfo.fMaxPrice;
	szText.Format(m_szFormat,fPrice);
	InsertItem(2,1,szText);
	szText.Format(_T("%d"),StatInfo.nMaxDate);
	InsertItem(2,2,szText); //���


	fPrice = StatInfo.fMinPrice;
	szText.Format(m_szFormat,fPrice);
	InsertItem(3,1,szText);
	szText.Format(_T("%d"),StatInfo.nMinDate);
	InsertItem(3,2,szText); //���
	
	szText.Format(_T("%.2f%%"),StatInfo.fSwing);
	InsertItem(3,4,szText);//���

	szText.Format(_T("%d"),StatInfo.nRiseDay);
	InsertItem(4,4,szText);
	



	szText.Format(_T("%d"),StatInfo.nDownDay);
	InsertItem(5,4,szText);


	szText.Format(_T("%d"),StatInfo.nEquireDay);
	InsertItem(6,4,szText);

	return TRUE;
}

void CDistractStatTable::SetColorsAndFonts()
{
	m_crBkColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_crTextColor = CInterfaceManager::Instance()->GetSysColor(ID_GRIDFIXED_COLOR);
	m_crTextBkColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_crLineColor = CInterfaceManager::Instance()->GetSysColor(ID_GRIDLINE_COLOR);
	m_pFont = CInterfaceManager::Instance()->GetSysFont(ID_TABLE_FONT);
}

void CDistractStatTable::Draw(CDC *pDC)
{
	CBaseTable::Draw(pDC);
	//����߿�
	COLORREF cr;
	cr = CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR);
	CPen myPen(PS_SOLID,1,cr);
	CPen* oldPen;
	oldPen = pDC->SelectObject(&myPen);
	CBrush* oldbrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(m_DrawRect);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldbrush);	
}

void CDistractStatTable::SetStockInfo(const SCodeInfo *pCodeInfo)
{
	m_pCodeInfo = pCodeInfo;
	m_szFormat.Format(_T("%%.%df"),CPublic::Instance()->GetDecimal(m_pCodeInfo));
}

void CDistractStatTable::MoveTo(CRect rRect)
{
	CBaseTable::MoveTo(rRect);
}
