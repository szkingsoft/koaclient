// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "TechCreator.h"
#include "koaclient.h"

#include "techanalysiskline.h"
#include "TechAnalysisSelf.h"


#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTechCreator::CTechCreator()
{
	m_pTechAnalysis = NULL;
}

CTechCreator::~CTechCreator()
{
	if(m_pTechAnalysis)
		delete m_pTechAnalysis;
}

//��������: ��������
//��������: pData ���ݵ�����
//��������: �ɹ�Ϊ TRUE,ʧ��ΪFALSE
//�����쳣: ��
BOOL CTechCreator::UpdateData(LPVOID pNewData)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->UpdateData(pNewData);
	return TRUE;	
}

//�������ܣ�����ͼ��
//��������: pDC ��ͼ�豸ָ��
//�������أ��ɹ�Ϊ TRUE,ʧ��ΪFALSE
//�����쳣: ��
BOOL CTechCreator::Draw(CDC* pDC)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->Draw(pDC);
	return TRUE;	
}

//�������ܣ�����֤ȯ̧ͷ��Ϣ
//����������szTitle ֤ȯ��Ϣ
//�������أ���
void CTechCreator::SetTitleInfo(CString szTitle,int nDataType)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->SetTitleInfo(szTitle,nDataType);
}


void CTechCreator::SetShowStyle(DWORD dwStyle)
{
	if(m_pTechAnalysis&&((m_szTechType == "KLINE")||(m_szTechType == "KLINEMA" )))
		m_pTechAnalysis->SetShowStyle(dwStyle);	
}

//�������ܣ����ü���ָ������
//����������ANALYSISTPICTYPE nType����ָ������
//�������أ���
void CTechCreator::SetTechType(CString szType)
{
	m_szTechType = szType;
	if(m_pTechAnalysis)
	{
		delete m_pTechAnalysis; 
	}
	if(0 == szType.CompareNoCase(_T("KLINE")))
	{
		m_pTechAnalysis = new CTechAnalysisKLine;	
	}
	else if(( 0 == szType.CompareNoCase(_T("MA")))||( 0 == szType.CompareNoCase(_T("BOLL"))))
	{
		CTechAnalysisKLine* pKLine;
		pKLine = new CTechAnalysisKLine;
		pKLine->SetMAMode(TRUE,szType);
		m_pTechAnalysis = pKLine;
	}
	else
		m_pTechAnalysis = new CTechAnalysisSelf((char*)(LPCTSTR)szType);
	
	m_pTechAnalysis->SetTechType(szType);
}

void CTechCreator::UpdateTechNameTitle()
{
	if( m_pTechAnalysis)
		m_pTechAnalysis->UpdateTechNameTitle();
}

//�������ܣ����Ե�ǰλ��
//����������point 
//�������أ�BOOL
int CTechCreator::HitTest(CPoint point)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->HitTest(point);
	return 0;
}

//�������ܣ����û�ͼ����
//����������rRect����ָ�껭ͼ����
//�������أ���
void CTechCreator::MoveTo(CRect rRect)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->MoveTo(rRect);
}

//�������ܣ�����ָ�������㼰��ǰ��Ļ�ں���������
//����������pHoriPoint ָ��������
//�������أ�nHoriPoint��ǰ��Ļ�ں�����������
void CTechCreator::SetHoriPoint(int *pHoriPoint,int nHoriPoint)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->SetHoriPoint(pHoriPoint,nHoriPoint);
}

//�������ܣ����õ�ǰλ�õ�
//����������nStation��ǰ����
//�������أ���
void CTechCreator::SetCurrentStation(int nStation)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->SetCurrentStation(nStation);
}

//�������ܣ�ͨ����ǰ����õ�ָ��ֵ
//����������nStation��ǰ����
//�������أ�ָ��ֵ
float CTechCreator::GetPriceFromStation(int nStation)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetPriceFromStation(nStation);
	return -1;
}

//�������ܣ�ͨ��ָ��ֵ�õ�ָ��λ��
//����������nValue
//�������أ�ָ��λ��
int CTechCreator::GetStationFromPrice(float nValue)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetStationFromPrice(nValue);
	return 0;
}

//�������ܣ��õ�ָ��������ֵ
//������������
//�������أ�ָ��������ֵ
int CTechCreator::GetMaxParam()
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetMaxParam();
	return 0;
}


BOOL CTechCreator::MouseDown(CPoint point)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->MouseDown(point);
	return FALSE;
}

BOOL CTechCreator::MouseMove(CPoint point)
{
	if( m_pTechAnalysis)
		return m_pTechAnalysis->MouseMove(point);

	return FALSE;
}

void CTechCreator::DrawTotalRect(CDC *pDC)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->DrawTotalRect(pDC);
}

int CTechCreator::MouseDbClickTitle(CPoint pt)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->MouseDbClitTitle(pt);
	return 0;
}

void CTechCreator::ResetData()
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->ResetData();
}

BOOL CTechCreator::MouseUp(CPoint point)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->MouseUp(point);
	return FALSE;
}

BOOL CTechCreator::DeleteCurrentDrawLine()
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->DeleteCurrentDrawLine();
	return FALSE;
}

void CTechCreator::SetDrawLineType(int nLineType)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->SetDrawLineType(nLineType);
}

BOOL CTechCreator::MouseRBDown(CPoint point)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->MouseRBDown(point);
	return FALSE;
}

void CTechCreator::RemoveAllDrawLine()
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->RemoveAllDrawLine();
}


void CTechCreator::MouseRBtnUp(CPoint pt)
{
	if(m_pTechAnalysis&&((0 == m_szTechType.CompareNoCase(_T("KLINE")))||(0 == m_szTechType.CompareNoCase(_T("KLINEMA")))))
	{
		((CTechAnalysisKLine*)m_pTechAnalysis)->MouseRBtnUp(pt);
	}
}

void CTechCreator::ZoomVertAxis(bool bZoomIn)
{
	if(m_pTechAnalysis&&((0 == m_szTechType.CompareNoCase(_T("KLINE")))||(0 == m_szTechType.CompareNoCase(_T("KLINEMA")))))
	{
		((CTechAnalysisKLine*)m_pTechAnalysis)->ZoomVertAxis(bZoomIn);
	}
}
//����������ֵ
void CTechCreator::SetMaxParamer(int nMaxPar)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->SetMaxParamer(nMaxPar);
}
//////////////////////////////////////////////////////////////////////////
int CTechCreator::GetDateFromStation(int nXStation)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetDateFromStation(nXStation);
	return 0;
}

int CTechCreator::GetStationFromDate(int nDate)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetStationFromDate(nDate);
	return 0;
}
void CTechCreator::GetDrawRect(CRect& rect)
{
	m_pTechAnalysis->GetDrawRect(rect);
}

double CTechCreator::GetUnitLength()
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetKlineWidth();
	return 0.0f;
}

BOOL CTechCreator::GetHighLowFromTime(const int & time,float &dHigh,float &dLow)
{
	int nTime;
	nTime = GetDateFromStation(time);
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetHighLowFromTime(nTime,dHigh,dLow);
	return FALSE;
}

void CTechCreator::GetCloseFromXSgmt( const int &uBeginX,const int &uEndX, CArray< CPoint,CPoint&> &arPtClose )
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->GetCloseFromXSgmt(uBeginX,uEndX,arPtClose);
}

BOOL CTechCreator::GetHighLowFromXSgmt( const UINT uBeginX, const UINT uEndX, CPoint &ptHigh, CPoint &ptLow )
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetHighLowFromXSgmt(uBeginX,uEndX,ptHigh,ptLow);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////