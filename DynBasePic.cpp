// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "DynBasePic.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDynBasePic::CDynBasePic()
{
	m_iDeimal = 0;
	m_nPow = 1;
	m_pDC = NULL;	
	m_lHoriPoint = 242;
	m_pHoriPointStation = NULL;
	m_pFont = NULL;
	m_lMaxPrice = 1000;
	m_lMinPrice = 0;
	m_nVertCellHeight = 50;
	m_nVAxisCount = 0;
	m_nCurrentStation = -1;
	m_nValueNum = 0;
	ZeroMemory(&m_CurrentPoint,sizeof(CPoint));
	ZeroMemory(&m_TotalRect,sizeof(CRect));
	ZeroMemory(&m_rectDraw,sizeof(CRect));

}

CDynBasePic::~CDynBasePic()
{
	if(m_pHoriPointStation)	
		delete []m_pHoriPointStation;
}

//////////////////////////////////////////////////////////
//函数名称:DrawAxisStyle
//函数功能:画出各类坐标线
//函数参数:rRect 画图区间
//	       lMinValue  最小价格
//         lMaxValue  最大价格
//         pAxisPrice 坐标价格数列
//		   nDecimal   小数点位数
//		   nMode	  画图模式
//		   crLineColor
//		   crNormalTextColor 普通文字色
//		   crRiseTextColor   上涨文字颜色
//		   crDownTextColor   下跌文字颜色
void CDynBasePic::DrawAxisStyle(CRect rRect,
							 long lMinValue, 
							 long lMaxValue,
							 int nDiv, 
							 COLORREF crLineColor, 
							 COLORREF crNormalTextColor,
							 long* pXSign,
							 long nSignNum,
							 long  nHotSign)
{	
	if(m_pHoriPointStation == NULL)
		return;
	if(nDiv<=1)return;//坐标错误
	int nDecimal = m_iDeimal;
	float fRise = (float)(lMaxValue - lMinValue);
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
	if(lMaxValue == lMinValue)
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
	szFormat.Format(_T("%%.%df"),nDecimal);
	CRect ItemRect;//单元格区域
	CSize textsize;

	float fPrice = 0.0f;
	szPrice.Format(szFormat,(float(lMaxValue + lMinValue)/2)/m_nPow);
	int nCount;
	float fHeight;
	fHeight = (float)rRect.Height()/(nDiv-1);
	for(nCount =0;nCount<nDiv;nCount++)
	{
		iYStation = rRect.bottom - nCount*fHeight;
		if((nCount!= 0)&&(nCount!= nDiv-1))
		{
			m_pDC->MoveTo(rRect.left,iYStation);
			m_pDC->LineTo(rRect.right,iYStation);
		}
		fPrice = (lMinValue +nCount*fRise);
		szPrice.Format(szFormat,fPrice/m_nPow);
		textsize = m_pDC->GetTextExtent(szPrice);
		ItemRect.right = rRect.left-2;//左侧文字
		ItemRect.left = rRect.left -2 -textsize.cx;
		if(nCount == 0)
		{
			ItemRect.top = iYStation - textsize.cy;
			ItemRect.bottom = iYStation;
		}
		else if(nCount ==(nDiv-1))
		{
			ItemRect.top = iYStation;
			ItemRect.bottom = iYStation+ textsize.cy;
		}
		else
		{
			ItemRect.top = iYStation - textsize.cy/2;
			ItemRect.bottom = iYStation+ textsize.cy/2+1;
		}
		m_pDC->SetTextColor(crNormalTextColor);
		m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);//左侧文字
	}
	m_pDC->SelectObject(OldPen);
	m_pDC->SetBkMode(nOldMode);
	m_pDC->SelectObject(OldFont);
	m_pDC->SelectObject(OldBrush);
	
	int iXStation;
	for(nCount= 0;nCount<nSignNum;nCount++)
	{
		iXStation = m_pHoriPointStation[pXSign[nCount]];//当前位置
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

//////////////////////////////////////////////////////////
//函数名称：DrawLinkLine
//函数功能: 画出各类连接线,用于实时走势线、均线、其他各类接线
//函数参数:  rRect 画图区间
//　	     nLineWidth 线宽度
//           clrLineColor 线的颜色
//　　　　　 lMaxValue 最高值
//　　　　   lMinValue 最低值
//　　　　　 pValues   传递的数列
//           lValueNum 数列的数量
//函数返回: 无
//函数异常: 无
void CDynBasePic::DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, 
							long lMaxValue, long lMinVlaue, long* pValues, long lValueNum)
{
	if(pValues == NULL)return;
	if(m_pHoriPointStation == NULL)return;
	if(m_lMaxPrice == m_lMinPrice)return;
	if(lValueNum >m_lHoriPoint)lValueNum = m_lHoriPoint;//保证数据有效性
	if(IsBadReadPtr(pValues,lValueNum*sizeof(long)))return;//内存区错误
	CPen LinePen(PS_SOLID,nLineWidth,clrLineColor);
	CPen *OldPen = m_pDC->SelectObject(&LinePen);
	long lCount;
	for(lCount = 1;lCount < lValueNum;lCount++)
	{
		if(pValues[lCount]== 0x7fffffff || pValues[lCount-1]== 0x7fffffff )
			continue;	
		m_pDC->MoveTo(m_pHoriPointStation[lCount-1],pValues[lCount-1]);
		m_pDC->LineTo(m_pHoriPointStation[lCount],pValues[lCount]);

	}
	m_pDC->SelectObject(OldPen);
}

//////////////////////////////////////////////////////////
//函数名称：DrawVolumnBar
//函数功能: 画出各类条状图，用于成交量的画图
//函数参数: rRect 画图区间
//　	 nLineWidth 线宽度
//　　　　　 lMaxValue 最高值
//　　　　   lMinValue 最低值
//　　　　　 pValues   传递的数列
//           lValueNum 数列的数量
//			 clrDownColor 上涨颜色
//           clrRiseColor 下跌颜色
//           nRefPos    条状宽度
//函数返回: 无
//函数异常: 无
void CDynBasePic::DrawVolumnBar(CRect rRect, int nLineWidth, long lMaxValue, long lMinValue,
							 long* pValues, long* pPrice, long lValueNum, COLORREF clrRiseColor,
							 COLORREF clrDownColor,int iRefPos)
{
	if(pValues == NULL)return;
	if(m_pHoriPointStation == NULL)return;
	if(m_lMaxPrice == m_lMinPrice)return;
	
	if(IsBadReadPtr(pValues,lValueNum*sizeof(long)))return;//内存区错误
	CPen RisePen(PS_SOLID,1,clrRiseColor);
	CPen DownPen(PS_SOLID,1,clrDownColor);
	CBrush DownBrush(clrDownColor);
	long lCount;
	CPen *OldPen = NULL;
	for(lCount=0;lCount<lValueNum;lCount++)
	{
		if(pPrice[lCount])
			OldPen = m_pDC->SelectObject(&RisePen);
		else
			OldPen = m_pDC->SelectObject(&DownPen);
		if(pValues[lCount] < m_rectDraw.bottom-2)
		{//分时走势使用
		 	if(m_pHoriPointStation[lCount] >= m_rectDraw.right)
		 		m_pHoriPointStation[lCount] = m_rectDraw.right - 1;
			m_pDC->MoveTo(m_pHoriPointStation[lCount],m_rectDraw.bottom-2);
			m_pDC->LineTo(m_pHoriPointStation[lCount],pValues[lCount]);
		}
		m_pDC->SelectObject(OldPen);
	}
}

//////////////////////////////////////////////////////////
//////////////////
//函数名称: SetHoriPoint
//函数功能: 设置横坐标点
//函数参数: iHoriPoint 横坐标点数
//函数返回: 无
//函数异常: 无
void CDynBasePic::SetHoriPoint(int iHoriPoint)
{
	if(iHoriPoint<0)return;
	m_lHoriPoint = iHoriPoint;
	ASSERT(m_lHoriPoint > 1);

	if(m_pHoriPointStation)
	{
		delete []m_pHoriPointStation;
		m_pHoriPointStation = NULL;
	}
	m_pHoriPointStation = new long[m_lHoriPoint];	

	float fPointWidth = GetHoriPointWidth(); 
	long lCount;//计数器
	//////////////////////////////////////////////////////////////////////////
	//得到各点的位置
	for(lCount = 0;lCount< m_lHoriPoint;lCount++)
	{
		m_pHoriPointStation[lCount] = m_rectDraw.left +(int)(fPointWidth*lCount);
	}	
}

// 获取制定位置的x坐标
int CDynBasePic::GetHoriPointXByStation(int iStation)
{
	if( iStation >= m_lHoriPoint)
		return 0;
	
	return m_pHoriPointStation[iStation];
}

float CDynBasePic::GetHoriPointWidth()
{
	float fPointWide = (float)m_rectDraw.Width();
	ASSERT( m_lHoriPoint != 1);
	fPointWide /= (m_lHoriPoint-1);//计算单格宽度
	return fPointWide;
}

//////////////////////////////////////////////////////////
//函数名称: HitTest
//函数功能: 测试点是否在区域内 
//函数参数: point 点区域
//函数返回：成功为 TRUE,失败为FALSE
//函数异常: 无
BOOL CDynBasePic::HitTest(CPoint point)
{
	m_CurrentPoint = point;
	return m_rectDraw.PtInRect(point);	
}


//函数功能：画图图形上侧的文本信息
void CDynBasePic::DrawTitleText()
{

}


CFont* CDynBasePic::SetFont(CFont *pFont)
{
	CFont* CurFont;
	CurFont = m_pFont;
	m_pFont = pFont;
	return CurFont;
}

void CDynBasePic::MoveTo(CRect rRect)
{
	m_TotalRect = rRect;//总区域
}

void CDynBasePic::GetMaxMinPrice(long &lMinPrice, long &lMaxPrice, long *pData, long nDataNum,BOOL bFlash)
{
	if(nDataNum == 0)return;//无数据直接返回
	
	if(bFlash)//刷新最高最低价
	{
		lMaxPrice = 0x80000000;
		lMinPrice = 0x7fffffff;
 	}
 	long nCount;
 	for(nCount= 0;nCount<nDataNum;nCount++)
 	{//循环比较
		if(pData[nCount] == 0x7fffffff)
			continue;
 		if(pData[nCount]>lMaxPrice)
 			lMaxPrice = pData[nCount];
 		if(pData[nCount]<lMinPrice)
			lMinPrice = pData[nCount];
 	} 

	if( lMinPrice == 0x7fffffff)
	{
		lMinPrice = 0;
	}
	if( lMaxPrice == 0x80000000)
	{
		lMaxPrice = lMinPrice;
	}
}

int CDynBasePic::GetVertAxisCellNum(int iHeight)
{
	if( m_nVertCellHeight <= 0)
		return 2;
	
	int nVertNum;
	nVertNum = iHeight/m_nVertCellHeight;
	nVertNum++;
	if(nVertNum <= 1)nVertNum = 2;
	return nVertNum;
}

void CDynBasePic::DrawBarLine(CRect rRect, int nLineWidth, long lMaxValue, long lMinVlaue, 
						   long *pValues, long lValueNum,COLORREF crRiseTextColor,
						   COLORREF crDownTextColor,int nBaseValue)
{
	if(pValues == NULL)return;
	if(m_pHoriPointStation == NULL)return;
	if(m_lMaxPrice == m_lMinPrice)return;
	if( lMaxValue == lMinVlaue) return;
	if( pValues == NULL)return;
	
	if(lValueNum >m_lHoriPoint)lValueNum = m_lHoriPoint;//保证数据有效性
	if(IsBadReadPtr(pValues,lValueNum*sizeof(long)))return;//内存区错误
	CPen RiseLinePen(PS_SOLID,nLineWidth,crRiseTextColor);
	CPen DownLinePen(PS_SOLID,nLineWidth,crDownTextColor);
	long lZeroStation;//零坐标位置
	
	//得到基准轴位置
	lZeroStation = m_rectDraw.bottom - (nBaseValue - lMinVlaue)*m_rectDraw.Height()/(lMaxValue-lMinVlaue);
	
	CPen *OldPen;
	long lCount;
	//////////////////////////////////////////////////////////////////////////
	//画出线
	for(lCount = 0;lCount < lValueNum;lCount++)
	{
		if(pValues[lCount]== 0x7fffffff)continue;
		if(pValues[lCount] < lZeroStation)
			OldPen = m_pDC->SelectObject(&RiseLinePen);
		else
			OldPen = m_pDC->SelectObject(&DownLinePen);
		m_pDC->MoveTo(m_pHoriPointStation[lCount],lZeroStation);
		m_pDC->LineTo(m_pHoriPointStation[lCount],pValues[lCount]);
		m_pDC->SelectObject(OldPen);
	}
}

int CDynBasePic::GetCurrentStation(CPoint point)
{
	if(m_rectDraw.PtInRect(point))
		return (point.x - m_rectDraw.left)*m_lHoriPoint/m_rectDraw.Width();
	else
		return -1;
}

void CDynBasePic::SetCurrentStation(int iStation)
{
	m_nCurrentStation = iStation;
}

void CDynBasePic::DrawCurrentStationLine()
{
	if( m_pHoriPointStation == NULL)
		return;
	
	if((m_nCurrentStation >=0)&&(m_nCurrentStation < m_nValueNum))
	{
		if(m_pHoriPointStation[m_nCurrentStation] >= m_rectDraw.right)
			m_pHoriPointStation[m_nCurrentStation] = m_rectDraw.right - 1;
		int iOldMode = m_pDC->SetROP2(R2_NOT);//选择异或反思画图
		m_pDC->MoveTo(m_pHoriPointStation[m_nCurrentStation],m_rectDraw.top);
		m_pDC->LineTo(m_pHoriPointStation[m_nCurrentStation],m_rectDraw.bottom);	


		int nPostion=GetStationFromPrice(m_lCurrentPrice);
		m_pDC->MoveTo(m_rectDraw.left,nPostion);
		m_pDC->LineTo(m_rectDraw.right,nPostion);



		m_pDC->SetROP2(iOldMode);
	}
}
 
void CDynBasePic::DrawTotalRect(COLORREF clrLine)
{
	CPen CordLine(PS_SOLID,1,clrLine);
	CPen *OldPen = m_pDC->SelectObject(&CordLine);
	CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
	m_pDC->Rectangle(m_TotalRect);
	m_pDC->SelectObject(OldPen);
	m_pDC->SelectObject(OldBrush);
}

long CDynBasePic::GetPriceFromStation(int nStation)
{
	if((nStation >m_rectDraw.bottom)||(nStation<m_rectDraw.top))return -1;
	if( m_rectDraw.Height() > 0)
	{
		return m_lMinPrice +(m_rectDraw.bottom - nStation)*(m_lMaxPrice- m_lMinPrice)/m_rectDraw.Height();
	}
	else
		return -1;
}


void CDynBasePic::SetCurrentPrice(long lPrice)
{
	m_lCurrentPrice = lPrice;
}