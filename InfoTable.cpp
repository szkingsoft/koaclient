// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "InfoTable.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CInfoTable::CInfoTable()
{
	//m_nInfoMode = MINUTEDATAINFO;//����������ģʽ
	m_nCircleInfo = MINUTE1_HISK;
	m_fNewPrice = 0.00f;
}

CInfoTable::~CInfoTable()
{
	// TODO: Add your specialized code here.
}

//�������ܣ����ڳ�ʼ���к��������ԣ����л�����������ش˺�
//��
//������������
//��������:�ɹ�ΪTRUE,����ΪFALSE
//�����쳣:��
BOOL CInfoTable::Init()
{
	//��ʼ������
	m_ArrColumn.RemoveAll();//���
	InsertColumn(0,_T(""),LVCFMT_LEFT,m_DrawRect.Width());
	SetStyle(LVS_NOCOLUMNHEADER);
	m_nSystemStyle = CInterfaceManager::Instance()->GetSysStyle();
	return TRUE;
}

void CInfoTable::SetMode(int nCircleInfo)
{
	m_nCircleInfo = nCircleInfo;
}

BOOL CInfoTable::Init(int nCircleInfo)
{
	BOOL bInit;
	bInit = Init();
	m_nCircleInfo = nCircleInfo;
//	m_nInfoMode = nInfoMode;//����������ģʽ
	return bInit;
}

//�������ܣ��������ݣ����м̳���������ش˽ӿ�����������
//����������lpData ��̬������
//��������:�ɹ�ΪTRUE,����ΪFALSE
//�����쳣:��
BOOL CInfoTable::UpdateData(LPVOID lpData,int nDataLen)
{
	CMulQuantitySequence * pSeq = (CMulQuantitySequence*)lpData;

	if(m_nCircleInfo < DAY_HISK)
	{
		SDynamicMinsUnit* pRealMinute =(SDynamicMinsUnit* )pSeq->m_ppValues[0];
		float* pRef = (float*)pSeq->m_lReference;
		UpdateMinuteInfo(pRealMinute,*pRef);
	}
	else
	{
		SHistoryUnit* pKUnit = (SHistoryUnit*)pSeq->m_ppValues[0];
		float* pRef = (float*)pSeq->m_lReference;
		float* pStation= (float*)pSeq->m_lMin;
		UpdateDayInfo(pKUnit,*pRef,*pStation, m_nCircleInfo);
	}
	return TRUE;
}

//���������������ݵ���ʾ  add by witming
BOOL CInfoTable::UpdateDataFromHis(LPVOID lpData,int nDataLen)
{
//���ݿͷ����� ͳһ�޸ĳ�ÿ�յ���ʾ��ʽ delete by witming 20151103
	CMulQuantitySequence * pSeq = (CMulQuantitySequence*)lpData;
	SHistoryUnit* pKUnit = (SHistoryUnit*)pSeq->m_ppValues[0];
	float* pRef = (float*)pSeq->m_lReference;
	float* pStation= (float*)pSeq->m_lMin;
	UpdateDayInfo(pKUnit,*pRef,*pStation, m_nCircleInfo);

	return TRUE;
}

void CInfoTable::Draw(CDC *pDC)
{
	SetColorsAndFonts();
//	InsertColumn(0,"",LVCFMT_LEFT,m_DrawRect.Width());
	//////////////////////////////////////////////////////////////////////////

	CBaseTable::Draw(pDC);
	//����߿�
	COLORREF cr;
	cr = RGB(175,50,50);//CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR);
	CPen myPen(PS_SOLID,1,cr);
	CPen* oldPen = pDC->SelectObject(&myPen);
	CBrush* oldbrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(m_DrawRect);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldbrush);
}

void CInfoTable::UpdateMinuteInfo(SDynamicMinsUnit *pRealMinute,float fLastClose)
{
	//////////////////////////////////////////////////////////////////////////
	//���ó�ʼ��ֵ
	COLORREF crRise,crDown,crEquire,crText;
	crRise = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	crDown =  CInterfaceManager::Instance()->GetSysColor(ID_DOWN_COLOR);
	crEquire = CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR);
	crText = CInterfaceManager::Instance()->GetSysColor(ID_STATCOLOR);

	SSubItem subItem;
	CString szItem;
	subItem.dwExStyle = LVCFMT_RIGHT;
	//subItem.dwStyle = SSubItem::BOTTOM_LINE;
	InsertItem(0,0,CLanguageInterface::Instance()->GetStr(20011),-1,crText,-1);
	//szItem.Format(_T("%d"),pRealMinute->m_uiTime);
	szItem = CDateManage::Instance()->FormatTime(pRealMinute->m_uiTime,0);
	subItem.szItemText = szItem;
	subItem.crTextColor = crEquire;
	InsertItem(1,0,subItem);
	//////////////////////////////////////////////////////////////////////////
	InsertItem(2,0,CLanguageInterface::Instance()->GetStr(20012),-1,crText,-1);

	float fPrice;
	fPrice = (float)(pRealMinute->m_fNewPrice);
	szItem.Format(m_szFormat,fPrice);
	if(fPrice == 0.000f)
	{
		subItem.szItemText ="null";
	}
	else
	{
		subItem.szItemText = szItem;
	}

	
	if(pRealMinute->m_fNewPrice > fLastClose)
		subItem.crTextColor = crRise;
	else if(pRealMinute->m_fNewPrice < fLastClose)
		subItem.crTextColor = crDown;
	else
		subItem.crTextColor = crEquire;

	InsertItem(3,0,subItem);

	InsertItem(4,0,CLanguageInterface::Instance()->GetStr(20013),-1,crText,-1);

	fPrice = (float)(m_fNewPrice);
	szItem.Format(m_szFormat,fPrice);
	CString strTemp;
	strTemp.Format(m_szFormat,0.00000000);
	subItem.szItemText = szItem;
	if (szItem.CompareNoCase(strTemp) == 0)
	{
		subItem.szItemText = _T("null");
		subItem.crTextColor = crEquire;
	}
	else
	{
		if(m_fNewPrice> fLastClose)
			subItem.crTextColor = crRise;
		else if(m_fNewPrice < fLastClose)
			subItem.crTextColor = crDown;
		else
			subItem.crTextColor = crEquire;
	}

	

	InsertItem(5,0,subItem);
}

void CInfoTable::UpdateDayInfo(SHistoryUnit *pKUnit,float fLastClose,float fCrossCursorPosValue, int nCircleInfo)
{
	//////////////////////////////////////////////////////////////////////////
	//���ó�ʼ��ֵ
	if(IsBadReadPtr(pKUnit,sizeof(SHistoryUnit)))return;
	
	COLORREF crRise,crDown,crEquire,crText;
	crRise = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	crDown =  CInterfaceManager::Instance()->GetSysColor(ID_DOWN_COLOR);
	crEquire = RGB(255,255,255);//CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR);
	crText = CInterfaceManager::Instance()->GetSysColor(ID_STATCOLOR);

	SSubItem subItem;
	CString szItem;
	float fPrice;
	int nInserRowIndex = 0;
	subItem.dwExStyle = LVCFMT_RIGHT;
	InsertItem(nInserRowIndex++,0,CLanguageInterface::Instance()->GetStr(20011),-1,crText,-1);

	int nDate;
	nDate = pKUnit->m_uiDate&0x7fffffff;
	
	subItem.szItemText = CDateManage::Instance()->FormatString(nDate, nCircleInfo);

// 	if(nDate/10000000>1)
// 	{//����
// 		//subItem.szItemText.Format(_T("%08d"),nDate);//����
// 		subItem.szItemText = CDateManage::Instance()->FormatString(nDate,DAY_HISK);
// 	}
// 	else
// 	{
// 		//subItem.szItemText.Format(_T("%04d-%04d"),nDate/10000,nDate%10000);//ʱ��
// 		subItem.szItemText = CDateManage::Instance()->FormatString(nDate,DAY_HISK-1);
// 	}
	subItem.crTextColor = crEquire;
	InsertItem(nInserRowIndex++,0,subItem);

	InsertItem(nInserRowIndex++,0,CLanguageInterface::Instance()->GetStr(20014),-1,crText,-1);
	fPrice = pKUnit->m_fOpenPrice;
	subItem.szItemText.Format(m_szFormat,fPrice);
	if( pKUnit->m_fOpenPrice > fLastClose)
		subItem.crTextColor = crRise;
	else if( pKUnit->m_fOpenPrice < fLastClose)
		subItem.crTextColor = crDown;
	else
		subItem.crTextColor = crEquire;

	InsertItem(nInserRowIndex++,0,subItem);

	InsertItem(nInserRowIndex++,0,CLanguageInterface::Instance()->GetStr(20015),-1,crText,-1);

	if(pKUnit->m_fMaxPrice > fLastClose)
		subItem.crTextColor = crRise;
	else if(pKUnit->m_fMaxPrice < fLastClose)
		subItem.crTextColor = crDown;
	else
		subItem.crTextColor = crEquire;
	fPrice = pKUnit->m_fMaxPrice;
	subItem.szItemText.Format(m_szFormat,fPrice);
	InsertItem(nInserRowIndex++,0,subItem);

	InsertItem(nInserRowIndex++,0,CLanguageInterface::Instance()->GetStr(20016),-1,crText,-1);

	if(pKUnit->m_fMinPrice > fLastClose)
		subItem.crTextColor = crRise;
	else if(pKUnit->m_fMinPrice <fLastClose)
		subItem.crTextColor = crDown;
	else
		subItem.crTextColor = crEquire;
	fPrice = pKUnit->m_fMinPrice;
	subItem.szItemText.Format(m_szFormat,fPrice);
	InsertItem(nInserRowIndex++,0,subItem);

	InsertItem(nInserRowIndex++,0,CLanguageInterface::Instance()->GetStr(20017),-1,crText,-1);

	if(pKUnit->m_fClosePrice > fLastClose)
		subItem.crTextColor = crRise;
	else if(pKUnit->m_fClosePrice <fLastClose)
		subItem.crTextColor = crDown;
	else
		subItem.crTextColor = crEquire;
	fPrice = pKUnit->m_fClosePrice;
	subItem.szItemText.Format(m_szFormat,fPrice);
	InsertItem(nInserRowIndex++,0,subItem);

	InsertItem(nInserRowIndex++,0,CLanguageInterface::Instance()->GetStr(20018),-1,crText);
	if(fLastClose != 0)
	{
		fPrice = pKUnit->m_fClosePrice - fLastClose;
		fPrice *= 100;
		fPrice /= fLastClose;
		subItem.szItemText.Format(_T("%.4f%%"),fPrice);
	}
	else
	{
		subItem.szItemText = _T("-");
	}

	if(pKUnit->m_fClosePrice > fLastClose)
		subItem.crTextColor = crRise;
	else if(pKUnit->m_fClosePrice < fLastClose)
		subItem.crTextColor = crDown;
	else
		subItem.crTextColor = crEquire;	
	InsertItem(nInserRowIndex++,0,subItem);



	//InsertItem(nInserRowIndex++,0,CLanguageInterface::Instance()->GetStr(20013),-1,crText,-1);

	//if(m_fNewPrice == 0.00f)
	//{
	//	fPrice = (float)(m_fNewPrice);
	//	szItem.Format(m_szFormat,fPrice);
	//	subItem.szItemText = szItem;

	//	if(m_fNewPrice> fLastClose)
	//		subItem.crTextColor = crRise;
	//	else if(m_fNewPrice < fLastClose)
	//		subItem.crTextColor = crDown;
	//	else
	//		subItem.crTextColor = crEquire;

	//	InsertItem(nInserRowIndex++,0,subItem);
	//}
}

void CInfoTable::SetStockInfo(const SCodeInfo* pCodeInfo)
{
	memcpy(&m_oCodeInfo,pCodeInfo,sizeof(m_oCodeInfo));
	m_szFormat.Format(_T("%%.%df"),CPublic::Instance()->GetDecimal(&m_oCodeInfo));
}