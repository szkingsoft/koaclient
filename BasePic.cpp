// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "BasePic.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBasePic::CBasePic()
{
	m_nDecimal = 0;
	m_pDC = NULL;	
	m_nHoriPoint = 0;
	m_pFont = NULL;
	m_nVAxisCount = 0;


	m_pHoriPoint = NULL;	//水平各点位置

	m_nVertCellHeight = 20;
	m_nCurrentStation = -1;
	m_nValueNum = 0;

	m_nKLineWidth = 0;	
	m_nKLineSperator = 0;

	m_fMaxPrice = 0.0f;
	m_fMinPrice = 0.0f;
	
	ZeroMemory(&m_CurrentPoint,sizeof(CPoint));   //当前位置点
	m_nVAxisCount = 0;       //垂直坐标数量
	ZeroMemory(&m_rectDraw,sizeof(CRect));        //有效画图区域
	ZeroMemory(&m_TotalRect,sizeof(CRect));       //总区域
	
	m_bActivate = FALSE;		 //是否为当前激活状态

	m_ppCurveValue = NULL;//数据区
	m_nCurves = 0;
	m_pParamValue = NULL;//参数	
	m_ppCurveStation = NULL;//数据位置值	
	m_nVAxisCount = 0;
	m_pColorValue = NULL;//颜色配置值
	m_pLineType = NULL;
	m_fVertZoomRaw = 1;
}

CBasePic::~CBasePic()
{
	//////////////////////////////////////////////////////////////////////////
	DestroyObj();//清除数据
	//线类型
	if(m_pLineType)
	{
		delete []m_pLineType;
		m_pLineType = NULL;
	}
	if(m_pParamValue)
	{
		delete []m_pParamValue;//删除指标参数
		m_pParamValue = NULL;
	}
	if(m_pColorValue)
	{
		delete []m_pColorValue;//删除指标颜色
		m_pColorValue = NULL; 
	}
}

//函数功能:画出各类坐标线
//函数参数:rRect 画图区间
//         pAxisPrice 坐标价格数列
//		   nDecimal   小数点位数
//	 nMode	  画图模式
//	 crLineColor
//   crNormalTextColor 普通文字色
//   crRiseTextColor   上涨文字颜色
//   crDownTextColor   下跌文字颜色
void CBasePic::DrawAxisStyle(CRect rRect,
							 float* pStation,
							 int nDiv, 
							 AXISMODE nMode,
							 COLORREF crLineColor, 
							 COLORREF crNormalTextColor)
{	
	if(m_fMinPrice == m_fMaxPrice)return;//价格错误
	if(m_pHoriPoint == NULL)return;
	if( nDiv < 1 )
		return;
	
	float fRise = (float)((m_fMaxPrice - m_fMinPrice)*m_fVertZoomRaw);
	fRise /= (nDiv-1);//每格的价格
	//////////////////////////////////////////////////////////////////////////
	//外框
	CPen CordLine(PS_SOLID,1,crLineColor);
	CPen *OldPen = m_pDC->SelectObject(&CordLine);
	CFont *OldFont = m_pDC->SelectObject(m_pFont);
	CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
	if(nMode != KLINEMODE)
		m_pDC->Rectangle(rRect);
	int iYStation;
	m_pDC->SelectObject(OldPen);
	//////////////////////////////////////////////////////////////////////////
	//价格分割线	
	CPen DotLine(PS_DOT,1,crLineColor);
	CPen SolidLine(PS_SOLID,1,crLineColor);

	OldPen = m_pDC->SelectObject(&DotLine);//技术分析
	int nCount;
	CString szPrice;
	double fPrice;
	CRect ItemRect;//单元格区域
	CSize textsize;
	int nOldMode = m_pDC->SetBkMode(TRANSPARENT);
	for(nCount =0;nCount<nDiv;nCount++)
	{
		iYStation = rRect.bottom - rRect.Height()*(pStation[nCount]-m_fMinPrice)/((m_fMaxPrice-m_fMinPrice)*m_fVertZoomRaw);//nCount/(nDiv-1);
		if((nMode == KLINEMODE)||((nCount!= 0)&&(nCount!= nDiv-1))||nDiv==1)
		{
			m_pDC->MoveTo(rRect.left,iYStation);
			m_pDC->LineTo(rRect.right,iYStation);
		}
		fPrice = pStation[nCount];
		szPrice = CPublic::Instance()->GetFormateValue(fPrice,m_nDecimal);
		textsize = m_pDC->GetTextExtent(szPrice);
		//////////////////////////////////////////////////////////////////////////
		//modify by david gong at 2013/09/15
		//显示文字在右侧
// 		ItemRect.right = rRect.left-2;//左侧文字
// 		ItemRect.left = rRect.left -2 -textsize.cx;
		ItemRect.left = rRect.right+2;//左侧文字
		ItemRect.right = rRect.right +2 +textsize.cx;
		//////////////////////////////////////////////////////////////////////////
		ItemRect.top = iYStation - textsize.cy/2;
		ItemRect.bottom = iYStation+ textsize.cy/2+1;
		m_pDC->SetTextColor(crNormalTextColor);
		m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);//左侧文字
	}
 	m_pDC->SelectObject(OldFont);
	m_pDC->SelectObject(OldBrush);
	m_pDC->SelectObject(OldPen);
	m_pDC->SetBkMode(nOldMode);
}

//画百分比坐标
void CBasePic::DrawPrecentAxisStyle(CRect rRect,
									   float* pStation,
									   int nDiv, 
									   AXISMODE nMode,
									   COLORREF crLineColor, 
									   COLORREF crNormalTextColor,
									   float fBasePrice)
{
	if(m_fMaxPrice == m_fMinPrice)return;//价格错误
	if(m_pHoriPoint == NULL)return;
	if( nDiv < 1 )
		return;
	
	float fRise = m_fMaxPrice - m_fMinPrice;
	fRise /= (nDiv-1);//每格的价格
	//////////////////////////////////////////////////////////////////////////
	//外框
	CPen CordLine(PS_SOLID,1,crLineColor);
	CPen *OldPen = m_pDC->SelectObject(&CordLine);
	CFont *OldFont = m_pDC->SelectObject(m_pFont);
	CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
	if(nMode != KLINEMODE)
		m_pDC->Rectangle(rRect);
	int iYStation;
	m_pDC->SelectObject(OldPen);
	//////////////////////////////////////////////////////////////////////////
	//价格分割线	
	CPen DotLine(PS_DOT,1,crLineColor);
	CPen SolidLine(PS_SOLID,1,crLineColor);
	
	OldPen = m_pDC->SelectObject(&DotLine);//技术分析
	int nCount;
	CString szPrice;
	double fPrice;
	CRect ItemRect;//单元格区域
	CSize textsize;
	int nOldMode = m_pDC->SetBkMode(TRANSPARENT);
	for(nCount =0;nCount<nDiv;nCount++)
	{
		iYStation = rRect.bottom - rRect.Height()*(pStation[nCount]-m_fMinPrice)/(m_fMaxPrice-m_fMinPrice);//nCount/(nDiv-1);
		if((nMode == KLINEMODE)||((nCount!= 0)&&(nCount!= nDiv-1)))
		{
			m_pDC->MoveTo(rRect.left,iYStation);
			m_pDC->LineTo(rRect.right,iYStation);
		}
		fPrice = pStation[nCount]-fBasePrice;// (lMinValue +nCount*fRise);
		fPrice = fPrice*100/fBasePrice;
		szPrice.Format(_T("%.2f%%"),fPrice);
		textsize = m_pDC->GetTextExtent(szPrice);
		ItemRect.right = rRect.left-2;//左侧文字
		ItemRect.left = rRect.left -2 -textsize.cx;
		ItemRect.top = iYStation - textsize.cy/2;
		ItemRect.bottom = iYStation+ textsize.cy/2+1;
		m_pDC->SetTextColor(crNormalTextColor);
		m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);//左侧文字
	}
	m_pDC->SelectObject(OldFont);
	m_pDC->SelectObject(OldBrush);
	m_pDC->SelectObject(OldPen);
	m_pDC->SetBkMode(nOldMode);
}
//函数功能: 画出各类连接线,用于实时走势线、均线、其他各类接线
//函数参数:  rRect 画图区间
//　	     nLineWidth 线宽度
//           clrLineColor 线的颜色
//　　　　　 pValues   传递的数列
//           lValueNum 数列的数量
//函数返回: 无
//函数异常: 无
void CBasePic::DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, 
							   int* pValues, int lValueNum,BOOL bSelctState)
{
	if(pValues == NULL)return;
	if(m_pHoriPoint == NULL)return;
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//保证数据有效性
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//内存区错误
	if((m_fMinPrice == m_fMaxPrice)&&(m_fMaxPrice == 0))return;	
	CPen LinePen(PS_SOLID,nLineWidth,clrLineColor);
	CPen *OldPen = m_pDC->SelectObject(&LinePen);
	int lCount;
	CBrush FillBrush(clrLineColor);
	CBrush* oldBrush = m_pDC->SelectObject(&FillBrush);
	
	// 原来没有初始化
	int nLastStation = 0;
	
	CRect FillRect;
	for(lCount = 1;lCount < lValueNum;lCount++)
	{
		if(pValues[lCount]== 0x7fffffff || pValues[lCount-1]== 0x7fffffff )
			continue;	
		m_pDC->MoveTo(m_pHoriPoint[lCount-1]+m_nKLineWidth/2,pValues[lCount-1]);
		m_pDC->LineTo(m_pHoriPoint[lCount]+m_nKLineWidth/2,pValues[lCount]);

		if((bSelctState)&&(m_pHoriPoint[lCount]-nLastStation >50))
		{//画出选择焦点
			nLastStation = m_pHoriPoint[lCount];
			FillRect.top = pValues[lCount]-3;
			FillRect.bottom = pValues[lCount]+3;
			FillRect.left = m_pHoriPoint[lCount]-3;
			FillRect.right = m_pHoriPoint[lCount]+3;
			m_pDC->Rectangle(FillRect);
		}
	}
	m_pDC->SelectObject(oldBrush);
	m_pDC->SelectObject(OldPen);
}

//函数功能: 画出条状线信息
//函数参数: rRect 画图区域
//          nLineWidth 线宽度
//          pValues   各点的指针
//          lValueNum  点的数量
//          crRiseTextColor     上涨颜色
//          crDownTextColor     下跌颜色
//          nBaseValue   基值
//函数返回: 无
void CBasePic::DrawBarLine(CRect rRect, int nLineWidth,
							  int *pValues, int lValueNum,
							  COLORREF crRiseTextColor,COLORREF crDownTextColor,float fBaseValue)
{
	if(pValues == NULL)return;
	if(m_pHoriPoint == NULL)return;
	if( m_fMaxPrice ==  m_fMinPrice)
		return;	
	
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//保证数据有效性
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//内存区错误
	CPen RiseLinePen(PS_SOLID,nLineWidth,crRiseTextColor);
	CPen DownLinePen(PS_SOLID,nLineWidth,crDownTextColor);
	int lZeroStation;//零坐标位置
	
	//得到基准轴位置
	lZeroStation = m_rectDraw.bottom - 
		           (fBaseValue - m_fMinPrice)*m_rectDraw.Height()/(m_fMaxPrice-m_fMinPrice);
	
	CPen *OldPen;
	int lCount;
	//////////////////////////////////////////////////////////////////////////
	//画出线
	for(lCount = 0;lCount < lValueNum;lCount++)
	{
		if(pValues[lCount]== 0x7fffffff)continue;
		if(pValues[lCount] < lZeroStation)
			OldPen = m_pDC->SelectObject(&RiseLinePen);
		else
			OldPen = m_pDC->SelectObject(&DownLinePen);
		m_pDC->MoveTo(m_pHoriPoint[lCount]+m_nKLineWidth/2,lZeroStation);
		m_pDC->LineTo(m_pHoriPoint[lCount]+m_nKLineWidth/2,pValues[lCount]);
		m_pDC->SelectObject(OldPen);
	}
}

//函数功能：画出整个区域
//函数参数：clrLine区域线颜色
//函数返回：无
void CBasePic::DrawTotalRect(CDC* pDC)
{
	COLORREF clrLine = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	CPen CordLine(PS_SOLID,1,clrLine);
	CPen *OldPen = pDC->SelectObject(&CordLine);
	CBrush* OldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(m_TotalRect);
	pDC->SelectObject(OldPen);
	pDC->SelectObject(OldBrush);
}

//函数功能：设置画图区域
//函数参数：rRect画图区域
//函数返回：无
void CBasePic::MoveTo(CRect rRect)
{
	m_TotalRect = rRect;//总区域
}


//鼠标移动
BOOL CBasePic::MouseMove(CPoint point)
{
	return FALSE;
}

// 鼠标移出
void  CBasePic::MouseMoveOut()
{
	return;
}

//鼠标左键弹起
BOOL  CBasePic::MouseLBtnUp(CPoint point)
{
	return FALSE;
}

//鼠标左键按下
BOOL CBasePic::MouseLBtnDown(CPoint point)
{
	return FALSE;
}

//画实时走势坐标
void CBasePic::DrawRealAxisStyle(CRect rRect,
							 int nDiv, 
							 AXISMODE nMode,
							 COLORREF crLineColor, 
							 COLORREF crNormalTextColor,
							 COLORREF crRiseTextColor,
							 COLORREF crDownTextColor,
							 long* pXSign,
							 long nSignNum,
							 long  nHotSign)
{	
	if(m_pHoriPoint == NULL)
		return;
	if(nDiv<=1)return;//坐标错误
	float fRise = (float)(m_fMaxPrice - m_fMinPrice);
	fRise /= (nDiv-1);//每格的价格
	//////////////////////////////////////////////////////////////////////////
	//外框
	CPen CordLine(PS_SOLID,1,crLineColor);
	CPen *OldPen = m_pDC->SelectObject(&CordLine);
	CFont *OldFont = m_pDC->SelectObject(m_pFont);
	CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
	m_pDC->Rectangle(rRect);
	m_pDC->SelectObject(OldPen);
	int iYStation;
	if((nMode == DOUBLESIDEDOUBLECOLOR)||(nMode == SINGLESIDEDOUBLECOLOR))
	{//画出中轴线
		if(0 == nDiv%2)
		{
			m_pDC->SelectObject(OldFont);
			m_pDC->SelectObject(OldBrush);
			return;//如果是价格走势必须是数量为单数，否则错误
		}
		CPen MiddleLine(PS_SOLID,2,crLineColor);
		OldPen = m_pDC->SelectObject(&MiddleLine);
		iYStation = (rRect.top+rRect.bottom)/2;
		m_pDC->MoveTo(rRect.left,iYStation);
		m_pDC->LineTo(rRect.right,iYStation);
		m_pDC->SelectObject(OldPen);
	}

	if(m_fMaxPrice == m_fMinPrice)
	{
		m_pDC->SelectObject(OldFont);
		m_pDC->SelectObject(OldBrush);
		return;//价格错误
	}

	//////////////////////////////////////////////////////////////////////////
	//价格分割线	
	CPen DotLine(PS_DOT,1,crLineColor);
	CPen SolidLine(PS_SOLID,1,crLineColor);
	OldPen = m_pDC->SelectObject(&SolidLine);
	int nOldMode = m_pDC->SetBkMode(TRANSPARENT);
	CFont *pProportionFont = CInterfaceManager::Instance()->GetSysFont(ID_PROPORTION_FONT);

	CString szPrice;
	CString szFormat;
	szFormat.Format(_T("%%.%df"),m_nDecimal);
	CRect ItemRect;//单元格区域
	CSize textsize;

	float fPrice = 0.0f;
	szPrice.Format(szFormat,(m_fMaxPrice + m_fMinPrice)/2);
	float fMidPrice = atof((char*)(LPCTSTR)szPrice);
	int nCount;
	for(nCount =0;nCount<nDiv;nCount++)
	{
		if(((nMode ==SINGLESIDESINGLECOLOR))
			&&((nCount == 0)||(nCount == nDiv-1)))
			continue;
		if((nMode== HISTORYAXISMODE)&&(nCount==0))
			continue;
		iYStation = rRect.bottom - rRect.Height()*nCount/(nDiv-1);
		if((nCount!= 0)&&(nCount!= nDiv-1))
		{
			m_pDC->MoveTo(rRect.left,iYStation);
			m_pDC->LineTo(rRect.right,iYStation);
		}

		if( nCount == nDiv/2)
		{
			fPrice = fMidPrice;
		}
		else
		{
			fPrice = (m_fMinPrice +nCount*fRise);
		}
		szPrice.Format(szFormat,fPrice);

		textsize = m_pDC->GetTextExtent(szPrice);
		ItemRect.right = rRect.left-2;//左侧文字
		ItemRect.left = rRect.left -2 -textsize.cx;
		ItemRect.top = iYStation - textsize.cy/2;
		ItemRect.bottom = iYStation+ textsize.cy/2+1;
		if((nMode == DOUBLESIDEDOUBLECOLOR)||(nMode == SINGLESIDEDOUBLECOLOR))
		{
			if(nCount> nDiv/2)
				m_pDC->SetTextColor(crRiseTextColor);
			else if(nCount <nDiv/2)
				m_pDC->SetTextColor(crDownTextColor);
			else
				m_pDC->SetTextColor(crNormalTextColor);
		}
		else
		{
			m_pDC->SetTextColor(crNormalTextColor);
		}	
		m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);//左侧文字

		if(nMode == DOUBLESIDEDOUBLECOLOR)
		{//画出右侧比例
			if( fMidPrice != 0)
			{
				szPrice.Format(_T("%.2f%%"),(fabs(fPrice-fMidPrice)*100)/fMidPrice);
				CFont *pTempFont = m_pDC->SelectObject(pProportionFont);			
				textsize = m_pDC->GetTextExtent(szPrice);

				ItemRect.left = rRect.right+2;
				ItemRect.right = ItemRect.left + textsize.cx;

				m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);	
				m_pDC->SelectObject(pTempFont);
			}
		}
	}
	m_pDC->SelectObject(OldPen);
	m_pDC->SetBkMode(nOldMode);
	m_pDC->SelectObject(OldFont);
	m_pDC->SelectObject(OldBrush);

	int iXStation;
	for(nCount= 0;nCount<nSignNum;nCount++)
	{
		iXStation = m_pHoriPoint[pXSign[nCount]];//当前位置
		if(pXSign[nCount] == nHotSign)
		{//特别标志位
			OldPen = m_pDC->SelectObject(&CordLine);
			m_pDC->MoveTo(iXStation,rRect.top);
			m_pDC->LineTo(iXStation,rRect.bottom);
			m_pDC->SelectObject(OldPen);
		}
		else
		{
			if(nCount%2)
			{
				OldPen = m_pDC->SelectObject(&DotLine);
				m_pDC->MoveTo(iXStation,rRect.top);
				m_pDC->LineTo(iXStation,rRect.bottom);
				m_pDC->SelectObject(OldPen);
			}
			else
			{
				OldPen = m_pDC->SelectObject(&SolidLine);
				m_pDC->MoveTo(iXStation,rRect.top);
				m_pDC->LineTo(iXStation,rRect.bottom);
				m_pDC->SelectObject(OldPen);

			}
		}
	}
}

//函数功能: 测试点是否在区域内 
//函数参数: point 点区域
//函数返回：成功为 TRUE,失败为FALSE
//函数异常: 无
int CBasePic::HitTest(CPoint point)
{
	m_CurrentPoint = point;

	return m_rectDraw.PtInRect(point);
}

//函数功能: 设置横坐标点
//函数参数: iHoriPoint 横坐标点数
//函数返回: 无
//函数异常: 无
void CBasePic::SetHoriPoint(int *pHoriPoint,int nHoriPoint)
{
	m_nHoriPoint = nHoriPoint;
	m_pHoriPoint = pHoriPoint;
}

//函数功能：设置当前字体
//函数参数：pFont
//函数返回：无
CFont* CBasePic::SetFont(CFont *pFont)
{
	CFont* CurFont;
	CurFont = m_pFont;
	m_pFont = pFont;
	return CurFont;
}

//函数功能：比较得到最大最小值
//函数参数: lMinPrice 返回的最小值
//          lMaxPrice 返回的最大值
//          pData    数据数列
//          lDataNum 数据数量
//函数返回：无
void CBasePic::GetMaxMinPrice(float &fMinPrice, float &fMaxPrice,
								 float *pData, int nDataNum,BOOL bFlash)
{
	if(nDataNum == 0)return;//无数据直接返回
	if( pData == NULL)
		return;
	int nMaxPrice,nMinPrice;

	float fPow;
	fPow = pow(10,(double)m_nDecimal);

	if(bFlash)//刷新最高最低价
	{
		nMaxPrice = 0x80000000;
		nMinPrice = 0x7fffffff;
 	}
	else
	{
		if(fMaxPrice > 0.00000001f  )
			nMaxPrice = fMaxPrice*fPow;
		else
			nMaxPrice =0x80000000;

		if(fMinPrice <1000000)
			nMinPrice = fMinPrice*fPow;	
		else
			nMinPrice = 0x7fffffff;
	}
 	int nCount;
	int nValue;
 	for(nCount= 0;nCount<nDataNum;nCount++)
 	{//循环比较
		if(pData[nCount] == 0x7fffffff)
			continue;
		if(pData[nCount] == 0x80000000)
			continue;
// 		if((pData[nCount]>=0.000001f)&&(pData[nCount]>))
// 			continue;
		nValue = (int)(pData[nCount]*fPow);
		if(nValue == 0x7fffffff)
			continue;
		if(nValue == 0x80000000)
			continue;

 		if(nValue>nMaxPrice)
 			nMaxPrice = nValue;
 		if(nValue<nMinPrice)
			nMinPrice = nValue;
 	} 

	if(nMaxPrice != 0x80000000)
	{
		fMaxPrice = nMaxPrice;
		fMaxPrice/=fPow;
	}

	if(nMinPrice != 0x7fffffff)
	{
		fMinPrice = nMinPrice;
		fMinPrice/=fPow;
	}
}

//函数功能: 得到垂直坐标线格数
//函数参数: 画图区间高度
//函数返回: 垂直线个数
int CBasePic::GetVertAxisCellNum(int iHeight)
{
	if( m_nVertCellHeight <= 0)
		return 2;

	int nVertNum;
	nVertNum = iHeight/m_nVertCellHeight;
	nVertNum++;
	if(nVertNum <= 1)nVertNum = 2;
	return nVertNum;
}

//函数功能: 设置当前光标位置
//函数参数：iStation当前光标位置
//函数返回：无
void CBasePic::SetCurrentStation(int iStation)
{
	m_nCurrentStation = iStation;
}

//函数功能：通过位置得到指标值
//函数参数：nStation当前光标位置
//函数返回：指标值
float CBasePic::GetPriceFromStation(int nStation)
{
	if((nStation >m_rectDraw.bottom)||(nStation<m_rectDraw.top))return -1;
	if (m_rectDraw.Height() > 0)
	{
		return m_fMinPrice + (m_fMaxPrice- m_fMinPrice)*m_fVertZoomRaw*(((float)(m_rectDraw.bottom - nStation))/m_rectDraw.Height());
	}
	else
		return -1;
}

//函数功能：通过指标值得到位置
//函数参数：nValue指标值
//函数返回：在画图区域中的位置
int CBasePic::GetStationFromPrice(float fValue)
{
	if(m_fMaxPrice == m_fMinPrice)return m_rectDraw.top;
	int nStation;
	nStation = (int)(( ((double)(fValue - m_fMinPrice))/((m_fMaxPrice- m_fMinPrice)*m_fVertZoomRaw))*m_rectDraw.Height());
	nStation = m_rectDraw.bottom - nStation;
	if(nStation>m_rectDraw.bottom)nStation = m_rectDraw.bottom;
	if(nStation<m_rectDraw.top)nStation = m_rectDraw.top;
	return nStation;
}

//函数功能: 初始化数据缓冲区
//函数参数: nParam 指标参数数量
//          nBufferSize 缓冲区数量
//          nDataSize   数据长度
//函数返回：无
void CBasePic::InitDataBuffer(int nDataSize)
{
	if(0 == m_nCurves)return;
	DestroyObj();//清除老数据
	//////////////////////////////////////////////////////////////////////////
	//申请新数据
	m_ppCurveValue = new float*[m_nCurves];
	ZeroMemory(m_ppCurveValue,m_nCurves*sizeof(float*));
	int nCount;
	if(nDataSize)
	{
		for(nCount= 0;nCount<m_nCurves;nCount++)
			m_ppCurveValue[nCount] = new float[nDataSize];

	}
	//////////////////////////////////////////////////////////////////////////
	//数据位置
	m_ppCurveStation = new int*[m_nCurves];
	ZeroMemory(m_ppCurveStation,m_nCurves*sizeof(int*));
	if(nDataSize)
	{
		for(nCount= 0;nCount<m_nCurves;nCount++)
			m_ppCurveStation[nCount] = new int[nDataSize];
	}
}

//函数功能：清除数据
//函数参数：无
//函数返回：无
void CBasePic::DestroyObj()
{
	//////////////////////////////////////////////////////////////////////////
	//删除数据
	if(m_ppCurveValue)
	{
		int nCount;
		for(nCount=0;nCount<m_nCurves;nCount++)
		{//删除各类数据
			if(m_ppCurveValue[nCount])
			{
				delete []m_ppCurveValue[nCount];
				m_ppCurveValue[nCount] = NULL;
			}
		}
		delete[]m_ppCurveValue;
		m_ppCurveValue = NULL;
	}
	//删除数据位置
	if(m_ppCurveStation)
	{
		int nCount;
		for(nCount=0;nCount<m_nCurves;nCount++)
		{//删除各类数据
			if(m_ppCurveStation[nCount])
			{
				delete []m_ppCurveStation[nCount];
				m_ppCurveStation[nCount] = NULL;
			}
		}
		delete[]m_ppCurveStation;
		m_ppCurveStation = NULL;
	} 
}










