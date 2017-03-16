#include "stdafx.h"
#include "BaseHisPic.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBaseHisPic::CBaseHisPic()
{
	m_iDeimal = 0;
	m_pDC = NULL;	
	m_pHoriPointStation = NULL;
	m_nHoriPoint = 0;
	m_pFont = NULL;
	m_nVAxisCount = 0;

	m_nVertCellHeight = 20;
	m_nCurrentStation = -1;
	m_nValueNum = 0;

	m_nKLineWidth = 0;	
	m_nKLineSperator = 0;

	m_nMaxPrice = 1000;
	m_nMinPrice = 0;
	
	ZeroMemory(&m_CurrentPoint,sizeof(CPoint));   //当前位置点
	m_nVAxisCount = 0;       //垂直坐标数量
	ZeroMemory(&m_rectDraw,sizeof(CRect));        //有效画图区域
	ZeroMemory(&m_TotalRect,sizeof(CRect));       //总区域
	
	m_bActivate = FALSE;		 //是否为当前激活状态
	m_fVertZoomRaw = 1;
}

CBaseHisPic::~CBaseHisPic()
{
}

//函数功能:画出各类坐标线
//函数参数:rRect 画图区间
//	       lMinValue 最小价格
//         lMaxValue 最大价格
//         pAxisPrice 坐标价格数列
//		   nDecimal   小数点位数
//	 nMode	  画图模式
//	 crLineColor
//   crNormalTextColor 普通文字色
//   crRiseTextColor   上涨文字颜色
//   crDownTextColor   下跌文字颜色
void CBaseHisPic::DrawAxisStyle(CRect rRect,
							 int lMinValue, 
							 int lMaxValue,
							 float* pStation,
							 int nDiv, 
							 int nDecimal, 
							 AXISMODE nMode,
							 COLORREF crLineColor, 
							 COLORREF crNormalTextColor)
{	
	if(lMaxValue == lMinValue)return;//价格错误
	if(m_pHoriPointStation == NULL)return;
	if( nDiv <=1 )
		return;
	
	float fRise = (float)((lMaxValue - lMinValue)*m_fVertZoomRaw);
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
		iYStation = rRect.bottom - rRect.Height()*(pStation[nCount]-lMinValue)/((lMaxValue-lMinValue)*m_fVertZoomRaw);//nCount/(nDiv-1);
		if((nMode == KLINEMODE)||((nCount!= 0)&&(nCount!= nDiv-1)))
		{
			m_pDC->MoveTo(rRect.left,iYStation);
			m_pDC->LineTo(rRect.right,iYStation);
		}
		fPrice = pStation[nCount];// (lMinValue +nCount*fRise);
		fPrice /= pow(10,(double)nDecimal);
		szPrice = CPublic::Instance()->GetFormateValue(fPrice,m_iDeimal);
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
//　　　　　 lMaxValue 最高值
//　　　　   lMinValue 最低值
//　　　　　 pValues   传递的数列
//           lValueNum 数列的数量
//函数返回: 无
//函数异常: 无
void CBaseHisPic::DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, 
							   int lMaxValue, int lMinVlaue, int* pValues, int lValueNum,
							   BOOL bSelctState)
{
	if(pValues == NULL)return;
	if(m_pHoriPointStation == NULL)return;
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//保证数据有效性
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//内存区错误
	if((m_nMinPrice == m_nMaxPrice)&&(m_nMaxPrice == 0))return;	
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
		m_pDC->MoveTo(m_pHoriPointStation[lCount-1]+m_nKLineWidth/2,pValues[lCount-1]);
		m_pDC->LineTo(m_pHoriPointStation[lCount]+m_nKLineWidth/2,pValues[lCount]);

		if((bSelctState)&&(m_pHoriPointStation[lCount]-nLastStation >50))
		{//画出选择焦点
			nLastStation = m_pHoriPointStation[lCount];
			FillRect.top = pValues[lCount]-3;
			FillRect.bottom = pValues[lCount]+3;
			FillRect.left = m_pHoriPointStation[lCount]-3;
			FillRect.right = m_pHoriPointStation[lCount]+3;
			m_pDC->Rectangle(FillRect);
		}
	}
	m_pDC->SelectObject(oldBrush);
	m_pDC->SelectObject(OldPen);
}

//函数功能: 画出条状线信息
//函数参数: rRect 画图区域
//          nLineWidth 线宽度
//          lMaxValue 最大值
//          lMinValue 最小值
//          pValues   各点的指针
//          lValueNum  点的数量
//          crRiseTextColor     上涨颜色
//          crDownTextColor     下跌颜色
//          nBaseValue   基值
//函数返回: 无
void CBaseHisPic::DrawBarLine(CRect rRect, int nLineWidth,
							  int lMaxValue, int lMinVlaue, 
							  int *pValues, int lValueNum,
							  COLORREF crRiseTextColor,COLORREF crDownTextColor,int nBaseValue)
{
	if(pValues == NULL)return;
	if(m_pHoriPointStation == NULL)return;
	if((m_nMinPrice == m_nMaxPrice)&&(m_nMaxPrice == 0))return;
 
	if( 0 == ( lMaxValue - lMinVlaue))
		return;	
	
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//保证数据有效性
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//内存区错误
	CPen RiseLinePen(PS_SOLID,nLineWidth,crRiseTextColor);
	CPen DownLinePen(PS_SOLID,nLineWidth,crDownTextColor);
	int lZeroStation;//零坐标位置
	
	//得到基准轴位置
	lZeroStation = m_rectDraw.bottom - 
		           (nBaseValue - lMinVlaue)*m_rectDraw.Height()/(lMaxValue-lMinVlaue);
	
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
		m_pDC->MoveTo(m_pHoriPointStation[lCount]+m_nKLineWidth/2,lZeroStation);
		m_pDC->LineTo(m_pHoriPointStation[lCount]+m_nKLineWidth/2,pValues[lCount]);
		m_pDC->SelectObject(OldPen);
	}
}

//函数功能：画出整个区域
//函数参数：clrLine区域线颜色
//函数返回：无
void CBaseHisPic::DrawTotalRect(CDC* pDC)
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
void CBaseHisPic::MoveTo(CRect rRect)
{
	m_TotalRect = rRect;//总区域
}


//鼠标移动
BOOL CBaseHisPic::MouseMove(CPoint point)
{
	return FALSE;
}

// 鼠标移出
void  CBaseHisPic::MouseMoveOut()
{
	return;
}

//鼠标左键弹起
BOOL  CBaseHisPic::MouseLBtnUp(CPoint point)
{
	return FALSE;
}

//鼠标左键按下
BOOL CBaseHisPic::MouseLBtnDown(CPoint point)
{
	return FALSE;
}

//函数功能: 测试点是否在区域内 
//函数参数: point 点区域
//函数返回：成功为 TRUE,失败为FALSE
//函数异常: 无
int CBaseHisPic::HitTest(CPoint point)
{
	m_CurrentPoint = point;

	return m_rectDraw.PtInRect(point);
}

//函数功能: 设置横坐标点
//函数参数: iHoriPoint 横坐标点数
//函数返回: 无
//函数异常: 无
void CBaseHisPic::SetHoriPoint(int *pHoriPoint,int nHoriPoint)
{
	m_nHoriPoint = nHoriPoint;
	m_pHoriPointStation = pHoriPoint;
}

//函数功能：设置当前字体
//函数参数：pFont
//函数返回：无
CFont* CBaseHisPic::SetFont(CFont *pFont)
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
void CBaseHisPic::GetMaxMinPrice(long &lMinPrice, long &lMaxPrice,
								 int *pData, int nDataNum,BOOL bFlash)
{
	if(nDataNum == 0)return;//无数据直接返回
	if( pData == NULL)
		return;
	
	if(bFlash)//刷新最高最低价
	{
		lMaxPrice = 0x80000000;
		lMinPrice = 0x7fffffff;
 	}
 	int nCount;
 	for(nCount= 0;nCount<nDataNum;nCount++)
 	{//循环比较
		if(pData[nCount] == 0x7fffffff)
			continue;
 		if(pData[nCount]>lMaxPrice)
 			lMaxPrice = pData[nCount];
 		if(pData[nCount]<lMinPrice)
			lMinPrice = pData[nCount];
 	} 
}

//函数功能: 得到垂直坐标线格数
//函数参数: 画图区间高度
//函数返回: 垂直线个数
int CBaseHisPic::GetVertAxisCellNum(int iHeight)
{
	return iHeight/m_nVertCellHeight;
}

//函数功能: 设置当前光标位置
//函数参数：iStation当前光标位置
//函数返回：无
void CBaseHisPic::SetCurrentStation(int iStation)
{
	m_nCurrentStation = iStation;
}

//函数功能：通过位置得到指标值
//函数参数：nStation当前光标位置
//函数返回：指标值
long CBaseHisPic::GetPriceFromStation(int nStation)
{
	if((nStation >m_rectDraw.bottom)||(nStation<m_rectDraw.top))return -1;
	if (m_rectDraw.Height() > 0)
	{
		return m_nMinPrice + (m_nMaxPrice- m_nMinPrice)*m_fVertZoomRaw*(((float)(m_rectDraw.bottom - nStation))/m_rectDraw.Height());
	}
	else
		return -1;
}

//函数功能：通过指标值得到位置
//函数参数：nValue指标值
//函数返回：在画图区域中的位置
int CBaseHisPic::GetStationFromPrice(int nValue)
{
	if(m_nMaxPrice == m_nMinPrice)return m_rectDraw.top;
	int nStation;
	nStation = (int)(( ((double)(nValue - m_nMinPrice))/(m_nMaxPrice- m_nMinPrice)*m_fVertZoomRaw)*m_rectDraw.Height());
	nStation = m_rectDraw.bottom - nStation;
	if(nStation>m_rectDraw.bottom)nStation = m_rectDraw.bottom;
	if(nStation<m_rectDraw.top)nStation = m_rectDraw.top;
	return nStation;
}
