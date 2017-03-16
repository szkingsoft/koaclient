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

//函数功能: 更新数据
//函数参数: pData 传递的数据
//函数返回: 成功为 TRUE,失败为FALSE
//函数异常: 无
BOOL CTechCreator::UpdateData(LPVOID pNewData)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->UpdateData(pNewData);
	return TRUE;	
}

//函数功能：画出图形
//函数参数: pDC 画图设备指针
//函数返回：成功为 TRUE,失败为FALSE
//函数异常: 无
BOOL CTechCreator::Draw(CDC* pDC)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->Draw(pDC);
	return TRUE;	
}

//函数功能：设置证券抬头信息
//函数参数：szTitle 证券信息
//函数返回：无
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

//函数功能：设置技术指标类型
//函数参数：ANALYSISTPICTYPE nType技术指标类型
//函数返回：无
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

//函数功能：测试当前位置
//函数参数：point 
//函数返回：BOOL
int CTechCreator::HitTest(CPoint point)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->HitTest(point);
	return 0;
}

//函数功能：设置画图区域
//函数参数：rRect技术指标画图区域
//函数返回：无
void CTechCreator::MoveTo(CRect rRect)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->MoveTo(rRect);
}

//函数功能：设置指标横坐标点及当前屏幕内横坐标点个数
//函数参数：pHoriPoint 指标横坐标点
//函数返回：nHoriPoint当前屏幕内横坐标点个个数
void CTechCreator::SetHoriPoint(int *pHoriPoint,int nHoriPoint)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->SetHoriPoint(pHoriPoint,nHoriPoint);
}

//函数功能：设置当前位置点
//函数参数：nStation当前光标点
//函数返回：无
void CTechCreator::SetCurrentStation(int nStation)
{
	if(m_pTechAnalysis)
		m_pTechAnalysis->SetCurrentStation(nStation);
}

//函数功能：通过当前光标点得到指标值
//函数参数：nStation当前光标点
//函数返回：指标值
float CTechCreator::GetPriceFromStation(int nStation)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetPriceFromStation(nStation);
	return -1;
}

//函数功能：通过指标值得到指标位置
//函数参数：nValue
//函数返回：指标位置
int CTechCreator::GetStationFromPrice(float nValue)
{
	if(m_pTechAnalysis)
		return m_pTechAnalysis->GetStationFromPrice(nValue);
	return 0;
}

//函数功能：得到指标最大参数值
//函数参数：无
//函数返回：指标最大参数值
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
//设置最大参数值
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