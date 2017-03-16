// TechAnalysisBasePic.cpp: implementation of the CDrawLineT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "DrawLineT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//初始化指标所用数据
//////////////////////////////////////////////////////////////////////

CDrawLineT::CDrawLineT()
{
	m_pKUnit = NULL;

	m_nKLineWidth = 0;

	m_nTitleBeginStation = 0;//抬头开始位置
	m_nDataBegin = 0; //水平开始位置点

	m_nCurrentSelCurves = -1;//没有任何选择的指标

	ZeroMemory(&m_TitleNameRect,sizeof(CRect));
	ZeroMemory(&m_TitleParamRect,sizeof(CRect));
	ZeroMemory(&m_TitleRect,sizeof(CRect));
	ZeroMemory(&m_AxisRect,sizeof(CRect));

	m_dwShowStyle = 0;	
	m_nParamNum = 0;	        //指标参数数量	



	ZeroMemory(&m_SelectDrawLineOrginPoint,sizeof(CPoint));//选择的原始点
	m_nSelectedState = NOSELECT; //选中画线的状态
	m_nSelectedDrawLine = -1;
	
	m_nDrawState = NULLDRAW;

	m_nAxisMode = PRICEMODE;//普通价格坐标
	m_nVAxisCount = 5;
	m_nRecsOfUnit = 1;
	m_nMaxParamDay = 0;

}

CDrawLineT::~CDrawLineT()
{
}

//函数功能：创建技术指标
//函数参数：szTitle  指标显示格式
//          nParam   指标参数个数
//          nCurvers 指标显示条数
//函数返回：无
void CDrawLineT::Create(CString strParamTitle,int nParam,int nCurvers)
{
    UpdateIndexParamInfo(strParamTitle);
	
	ResetParaBuffers(nParam,nCurvers);
}

// 更新技术指标参数内容
void CDrawLineT::UpdateIndexParamInfo(CString strParamTitle)
{
	m_strIndexParamTitle = strParamTitle;
}


//函数功能：更新数据
//函数参数：pNewData数据指针
//函数返回：无
BOOL CDrawLineT::UpdateData(LPVOID pNewData)
{
	CMulQuantitySequence * pSeq = (CMulQuantitySequence*)pNewData;
	m_nValueNum = pSeq->m_nSize;//数据量
	m_nKLineWidth = pSeq->m_lReference;//K线宽度
	m_pKUnit = (SHistoryUnit*)pSeq->m_ppValues[0];//数据
	if(NULL == m_pKUnit)return FALSE;//k线错误
	m_nRecsOfUnit= pSeq->m_lMin;//同一位置K线数量
	
	m_nDecimal = pSeq->m_lMax;//CPublic::Instance()->GetTechIndexValueDecimal(m_strIndexType);

	m_nDataBegin = pSeq->m_nStart;//数据起始位置
	ASSERT(m_nDataBegin >= 0);
 	CalculateValue();//计算指标数据
	CalcMinMaxPrice();//检索最高最低价
	CalcStation();//重新计算位置
	m_nDecimal = CPublic::Instance()->GetTechIndexValueDecimal(m_strIndexType);

	if (m_nCurrentStation < 0 || m_nCurrentStation >= m_nValueNum)
	{
		m_nCurrentStation = m_nValueNum - m_nDataBegin - 1;
	}
	
	return TRUE;
}

//函数功能：计算技术指标中每个值在图形区域中的纵坐标值
//函数参数：无
//函数返回：无
void CDrawLineT::CalcStation()
{
	if(NULL ==m_ppCurveStation)return;
	int nCount,nSubCount;
	for(nCount= 0;nCount<m_nCurves;nCount++)
	{
		for(nSubCount = 0;nSubCount<m_nValueNum;nSubCount++)
		{
			if(m_ppCurveValue[nCount][nSubCount] == 0x7fffffff)
				m_ppCurveStation[nCount][nSubCount] = 0x7fffffff;
			else
			{			
				if(0 == m_strIndexType.CompareNoCase(_T("WR")))
				{//威廉指标坐标系统倒置
					m_ppCurveStation[nCount][nSubCount] = m_rectDraw.top+ (int)((__int64)(m_ppCurveValue[nCount][nSubCount] - m_fMinPrice)*m_rectDraw.Height()/(m_fMaxPrice- m_fMinPrice));					
				}
				else
				{
					m_ppCurveStation[nCount][nSubCount] = GetStationFromPrice(m_ppCurveValue[nCount][nSubCount]);
				}
			}
		}	
	}
}

//函数功能：得到指标中的最大最小值
//函数参数：无
//函数返回：无
void CDrawLineT::CalcMinMaxPrice()
{
	if(NULL == m_ppCurveValue)return;//数据错误
	int nCount;
	for(nCount = 0;nCount<m_nCurves;nCount++)
	{
		GetMaxMinPrice(m_fMinPrice,m_fMaxPrice,m_ppCurveValue[nCount]+m_nDataBegin,m_nValueNum-m_nDataBegin,!nCount);
	}	
	//////////////////////////////////////////////////////////////////////////
	
	char* pGridInfo;
	pGridInfo = m_thCalc.GetFormulaGridInfo((char*)(LPCTSTR)m_strIndexType);
	if(pGridInfo&&strlen(pGridInfo))
	{//有固定坐标
		//先找到数量
		int nGridNum;
		nGridNum = 0;
		char* pStart;
		pStart = pGridInfo;

		float fMinPrice;
		fMinPrice = atof(pStart);
		if(fMinPrice<m_fMinPrice)
			m_fMinPrice = fMinPrice;
		while (1) 
		{	
			nGridNum++;
			pStart = strstr(pStart,",");
			if((NULL == pStart)||(0== strlen(pStart)))
			{
				float fMaxPrice;
				fMaxPrice = atof(pGridInfo);
				if(fMaxPrice>m_fMaxPrice)
					m_fMaxPrice = fMaxPrice;
				break;
			}
			pStart +=1;
			pGridInfo= pStart;
		}

	}

	if((0 == m_strIndexType.CompareNoCase(_T("BOLL")))//
		||(0 == m_strIndexType.CompareNoCase(_T("SAR"))))
	{
		int nMaxPrice,nMinPrice;
		int nCount;
		nMaxPrice = 0x80000000;
		nMinPrice = 0x7fffffff;
		for(nCount=  m_nDataBegin;nCount<m_nValueNum;nCount++)
		{
			if(nMaxPrice < (int)(m_pKUnit[nCount].m_fMaxPrice*1000))
				nMaxPrice = m_pKUnit[nCount].m_fMaxPrice*1000;
			if(nMinPrice > (int)(m_pKUnit[nCount].m_fMinPrice*1000))
				nMinPrice = m_pKUnit[nCount].m_fMinPrice*1000;
		}
		if(nMaxPrice >m_fMaxPrice*1000)
		{
			m_fMaxPrice = nMaxPrice;
			m_fMaxPrice/=1000;
		}
		if(nMinPrice <m_fMinPrice*1000)
		{
			m_fMinPrice = nMinPrice;
			m_fMinPrice /=1000;
		}
	}

}

//函数功能：得到技术指标的参数的最大值
//函数参数：无
//函数返回：技术指标的参数的最大值
int CDrawLineT::GetMaxParam()
{ 
	if(!m_nParamNum||!m_pParamValue)return 0;
	int nCount;
	int nMaxParam;
	nMaxParam = 0;
	for(nCount= 0;nCount<m_nParamNum;nCount++)
	{
		if(nMaxParam< m_pParamValue[nCount])
			nMaxParam = m_pParamValue[nCount];
	}
	return nMaxParam;
}

//函数功能：打开技术指标注释对话框
//函数参数：无
//函数返回：无
void CDrawLineT::OpenTechNoteDlg()
{
	
}

//函数功能：读取技术指标的参数
//函数参数：nAnalysis指标类型
//函数返回：无
void CDrawLineT::ReadTechCurveParam(CString strIndexType)
{

}

//////////////////////////////////////////////////////////////////////
//改变指标画图区域、指标类型
//////////////////////////////////////////////////////////////////////

//函数功能：移动技术指标图形区域
//函数参数：rRect技术指标区域
//函数返回：无
void CDrawLineT::MoveTo(CRect rRect)
{
	CBasePic::MoveTo(rRect);
	m_TitleRect = rRect;
	m_TitleRect.bottom = m_TitleRect.top+ TITLESIZE;
	m_AxisRect = rRect;//坐标区域
	m_AxisRect.top = m_TitleRect.bottom;
	m_rectDraw = m_AxisRect;//画图区域
	m_rectDraw.left += 3;
	m_rectDraw.right -= 3;
 	//重新计算位置
	CalcStation(); 
}

//函数功能：设置指标类型
//函数参数：
//函数返回：
void CDrawLineT::SetTechType(CString strType)
{
	m_strIndexType = strType;
	UpdateTechNameTitle();
}

void CDrawLineT::UpdateTechNameTitle()
{
	m_strIndexNameTitle = m_strIndexType;
	if( m_nParamNum > 0)
	{
		CString strTemp;
		m_strIndexNameTitle += "(";
		for( int i=0; i<m_nParamNum; i++)
		{
			strTemp.Format(_T("%d"),m_pParamValue[i]);
			m_strIndexNameTitle += strTemp;
			if( i<m_nParamNum-1)
				m_strIndexNameTitle += ",";
		}
		m_strIndexNameTitle += ")";
	}
}

//函数功能：平整浮点数
//函数参数：fValue
//函数返回：无
int CDrawLineT::TrimDouble(double fValue)
{
	return fValue > 0 ? (long)(fValue + 0.5) : (long)(fValue - 0.5);	
}

//函数功能：画信息地雷提示信息
//函数参数：point当前鼠标点
//函数返回：无

void CDrawLineT::DrawTip(CDC *pDC,CPoint point,CString strTip)
{
	CFont* pTextFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	CFont * pOldFont = pDC->SelectObject(pTextFont);
	COLORREF crText = CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR);
	pDC->SetTextColor(crText);
	int nMode = pDC->SetBkMode(TRANSPARENT);
	
	CSize textSize = m_pDC->GetTextExtent(strTip);
	CRect ItemRect;
	ItemRect.left = point.x-1;
	ItemRect.top = point.y;
	ItemRect.right = ItemRect.left +textSize.cx+1;
	ItemRect.bottom = ItemRect.top + textSize.cy+2;//得到区域
	
	pDC->FillSolidRect(ItemRect,CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR));//画出外框
	pDC->DrawText(strTip,ItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//画出文字
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nMode);
}

CString CDrawLineT::GetParamName(int nIndex)
{
	CString strParamName;
	
	int i = 0;
	
	int nStartPos = 0;
	int nEndPos = 0;
	BOOL bContinue = TRUE;
	while( bContinue )
	{
		nEndPos = m_strIndexParamTitle.Find(_T(","),nStartPos);
		if( nEndPos == -1)
		{
			nEndPos = m_strIndexParamTitle.GetLength();
			bContinue = FALSE;
		}
		strParamName = m_strIndexParamTitle.Mid(nStartPos,nEndPos - nStartPos);
		if( i == nIndex)
			break;
		i++;	
		nStartPos = nEndPos + 1;	
	}

	return strParamName;
}


//////////////////////////////////////////////////////////////////////
//画技术指标图形、指标值显示
//////////////////////////////////////////////////////////////////////

//函数功能：画指标显示值
//函数参数：无
//函数返回：无
void CDrawLineT::DrawTitleText()
{
	CRect ItemRect;
	ItemRect = m_TitleRect;

	CFont* pTextFont;
	pTextFont = CInterfaceManager::Instance()->GetSysFont(ID_HISTORYPICTITLEFONT);
	CFont* pOldFont = m_pDC->SelectObject(pTextFont);

	CSize TextSize = m_pDC->GetTextExtent(_T("1"));
	ItemRect.left = m_rectDraw.left + TextSize.cx*m_nTitleBeginStation;//加上开始位置
	ItemRect.right = m_rectDraw.right;
	int nMode = m_pDC->SetBkMode(TRANSPARENT);
	COLORREF TextColor = CInterfaceManager::Instance()->GetSysColor(ID_FORMTITLE_COLOR);
	m_pDC->SetTextColor(TextColor); 

	m_TitleNameRect = m_TitleRect;
	m_TitleNameRect.right = m_TitleNameRect.left+ m_pDC->GetTextExtent(m_strIndexNameTitle).cx;
	CString szTemp; 
	
	m_pDC->DrawText(m_strIndexNameTitle,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	//////////////////////////////////////////////////////////////////////////
	//画出当前个数据

	m_TitleParamRect = m_TitleRect;
	m_TitleParamRect.left = m_TitleNameRect.right +3;
	//m_TitleParamRect.right = m_TitleParamRect.left;  // 初始化参数标题区域 

	if(m_ppCurveValue && m_pColorValue && m_nCurrentStation > -1 && m_nDataBegin > -1  )
	{
		float fPrice;
		CString szFormat;
		szFormat.Format(_T("%%.%df"),m_nDecimal);
		CString szItem = m_strIndexNameTitle;
		for(int nCount = 0;nCount<m_nCurves;nCount++)
		{//画出各数字

			if(m_nValueNum <= m_nCurrentStation+ m_nDataBegin)
				continue;

			// 成交量指标第一维参数是成交量值，要显示的参数“均线值”从第二维开始
			if(( m_strIndexType.Compare(_T("VOL")) == 0)&&(nCount == m_nCurves -1))
				break;

			TextSize = m_pDC->GetTextExtent(szItem);
			ItemRect.left += (TextSize.cx+3);
			//m_TitleParamRect.right += 10;		
			
			m_pDC->SetTextColor(m_pColorValue[nCount]);//设置颜色

			int nCurIndex = nCount;
			// VOL的第一维数据是成交量值，所以显示当前均线值从第二维开始，偏移1
			if( m_strIndexType.Compare(_T("VOL")) == 0)
			{
				nCurIndex = nCount+1;
			}	
			szItem = GetParamName(nCurIndex);			
			
			if(m_ppCurveValue[nCurIndex][m_nCurrentStation+m_nDataBegin] == 0x7fffffff)
				continue;
			fPrice = m_ppCurveValue[nCurIndex][m_nCurrentStation+m_nDataBegin];//得到当前价格
			szTemp.Format(szFormat,fPrice);
			szItem += "=";
			szItem += szTemp;
			ItemRect.right = ItemRect.left +m_pDC->GetTextExtent(szItem).cx;
			if(ItemRect.right >m_rectDraw.right)break;//防止越界
			m_pDC->DrawText(szItem,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);//画出文字
			
			//m_TitleParamRect.right += m_pDC->GetTextExtent(szItem).cx;   // 更新参数标题区域
		}
	}
	m_pDC->SelectObject(pOldFont);
	m_pDC->SetBkMode(nMode);

	//////////////////////////////////////////////////////////////////////////
	//画出提示信息
	if(m_TitleNameRect.PtInRect(m_ptNowStation))
	{
		CString szTipInfo;
		szTipInfo = CLanguageInterface::Instance()->GetStr(20030);
		DrawTip(m_pDC,m_ptNowStation,szTipInfo);
	}
	else
	{
		CRect ParamRect;
		ParamRect = m_TitleParamRect;
		ParamRect.right = ItemRect.right;
		if(ParamRect.PtInRect(m_ptNowStation))
		{
			CString szTipInfo;
			szTipInfo = CLanguageInterface::Instance()->GetStr(20031);
			DrawTip(m_pDC,m_ptNowStation,szTipInfo);
		}
	}
}

//函数功能：画技术指标图形
//函数参数：pDC
//函数返回：BOOL
BOOL CDrawLineT::Draw(CDC *pDC)
{
	if(!m_pHoriPoint||!m_pKUnit)return FALSE;
	m_pDC = pDC;
	COLORREF clrRise;
	clrRise = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	COLORREF clrEquire;
	clrEquire = CInterfaceManager::Instance()->GetSysColor(ID_STATCOLOR);
	COLORREF clrDown;
	clrDown =  CInterfaceManager::Instance()->GetSysColor(ID_KLINEDOWN_COLOR);//ID_DOWN_COLOR);
	COLORREF clrLine;
	clrLine = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	CFont* pFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	SetFont(pFont);
	//////////////////////////////////////////////////////////////////////////
	if(!(m_dwShowStyle&NOAXISRECT))
	{
		float* pStation;	
		char* pGridInfo;
		pGridInfo = m_thCalc.GetFormulaGridInfo((char*)(LPCTSTR)m_strIndexType);
		if(pGridInfo&&strlen(pGridInfo))
		{//有固定坐标
			//先找到数量
			int nGridNum;
			nGridNum = 0;
			char* pStart;
			pStart = pGridInfo;
			while (1) 
			{	
				nGridNum++;
				pStart = strstr(pStart,",");
				if((NULL == pStart)||(0== strlen(pStart)))break;
				pStart +=1;

			}
			//////////////////////////////////////////////////////////////////////////
			//得到坐标数量
			m_nVAxisCount = nGridNum+2;
			pStation = new float[m_nVAxisCount];
           
			//////////////////////////////////////////////////////////////////////////
			//设置固定坐标
			nGridNum=0;
			pStart = pGridInfo;
			while(1)
			{	
				nGridNum++;
				pStation[nGridNum] = atof(pStart);
				pStart = strstr(pStart,",");
				if((NULL == pStart)||(0== strlen(pStart)))break;
				pStart +=1;
			}
			pStation[0]= m_fMinPrice;

			pStation[m_nVAxisCount-1]= m_fMaxPrice; 
		}
		else
		{	
			pStation = new float[m_nVAxisCount];
			for(int i= 0;i<m_nVAxisCount;i++)
				pStation[i]= m_fMinPrice + ((m_fMaxPrice-m_fMinPrice)*m_fVertZoomRaw)/(m_nVAxisCount-1)*i;
		}
		if(m_nAxisMode == PRICEMODE)
		{
			DrawAxisStyle(m_AxisRect,
				pStation,
				m_nVAxisCount,
				HISTORYAXISMODE,
				clrLine,
				clrEquire);	//画坐标线
		}
		else if(m_nAxisMode == PRECENTMODE)
		{
			DrawPrecentAxisStyle(m_AxisRect,
				pStation,
				m_nVAxisCount,
				HISTORYAXISMODE,
				clrLine,
				clrEquire,
				m_pKUnit[m_nDataBegin].m_fClosePrice);	//画坐标线
		}
		delete []pStation;
	}
	if(m_ppCurveValue)
	{
		int nCount;
		int nLineColorIndex = 0;
		for(nCount = 0;nCount<m_nCurves;nCount++)
		{
			switch(m_pLineType[nCount]) 
			{
			case LINKLINE://连线
				{
					DrawLinkLine(m_rectDraw,
								1,
								m_pColorValue[nLineColorIndex],
								m_ppCurveStation[nCount]+m_nDataBegin,
								m_nValueNum-m_nDataBegin,
								nCount == m_nCurrentSelCurves);
					nLineColorIndex++;
				}
				break;
			case BARLINE://条状线
				{
					DrawBarLine(m_rectDraw,
								1,
								m_ppCurveStation[nCount]+m_nDataBegin,
								m_nValueNum-m_nDataBegin,
								clrRise,
								clrDown);			
				}
				break;
			case VOLUME:
				{
					DrawVolumnBar(m_rectDraw,
							1,
							m_ppCurveStation[nCount]+m_nDataBegin,
							m_nValueNum- m_nDataBegin,
							clrRise,
							clrDown);//成交量
				}
				break;
			case AMERICANLINE:
				{
					int nSubCount;
					for(nSubCount=m_nDataBegin;nSubCount<m_nValueNum;nSubCount++)
					{
						DrawAmercianLine(nSubCount);
					}
				}
				break;
			case KLINE:
				{
					int nSubCount;
					for(nSubCount=m_nDataBegin;nSubCount<m_nValueNum;nSubCount++)
					{
						DrawKLine(nSubCount);
					}
				}
				break;
			default:
				break;
			}
		}
	}
	DrawTitleText();//画出抬头

	if((0 == m_strIndexType.CollateNoCase(_T("SAR"))))
	{//如果是BOLL线或者SAR,增加画美国线
		int nSubCount;
		for(nSubCount=0;nSubCount<m_nValueNum-m_nDataBegin;nSubCount++)
		{
			DrawAmercianLine(nSubCount);
		}	
	}

	//////////////////////////////////////////////////////////////////////////
	//画线部分
	int nCount;
	for(nCount= 0;nCount<m_ArrDrawLine.GetSize();nCount++)
	{
		CDrawLineTool& draw = m_ArrDrawLine[nCount];
		draw.Draw(pDC);
	}
	return TRUE;
}

void CDrawLineT::SetShowStyle(DWORD dwStyle)
{
	m_dwShowStyle = dwStyle;
}


//函数功能: 画出各类连接线,用于实时走势线、均线、其他各类接线
//函数参数:  rRect 画图区间
//　	     nLineWidth 线宽度
//           clrLineColor 线的颜色
//　　　　　 pValues   传递的数列
//           lValueNum 数列的数量
//函数返回: 无
//函数异常: 无
void CDrawLineT::DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, 
							int* pValues, int lValueNum,BOOL bSelctState)
{
	if(pValues == NULL)return;
	if(m_pHoriPoint == NULL)return;
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//保证数据有效性
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//内存区错误
	if((m_fMinPrice == m_fMaxPrice)&&(m_fMaxPrice == 0))return;	
	CPen LinePen(PS_SOLID,nLineWidth,clrLineColor);
	CPen *OldPen = m_pDC->SelectObject(&LinePen);
	int nCount;
	CBrush FillBrush(clrLineColor);
	CBrush* oldBrush = m_pDC->SelectObject(&FillBrush);
	
	// 原来没有初始化
	int nLastStation = 0;


	int nHoriStation;
	
	CRect FillRect;
	for(nCount = 1;nCount < lValueNum;nCount++)
	{
		if(pValues[nCount]== 0x7fffffff || pValues[nCount-1]== 0x7fffffff )
			continue;	
		nHoriStation = nCount;//数量
		if(nHoriStation<=0)nHoriStation = 1;

		m_pDC->MoveTo(m_pHoriPoint[nHoriStation-1]+m_nKLineWidth/2,pValues[nCount-1]);
		m_pDC->LineTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth/2,pValues[nCount]);
		
		if((bSelctState)&&(m_pHoriPoint[nHoriStation]-nLastStation >50))
		{//画出选择焦点
			nLastStation = m_pHoriPoint[nHoriStation];
			FillRect.top = pValues[nCount]-3;
			FillRect.bottom = pValues[nCount]+3;
			FillRect.left = m_pHoriPoint[nHoriStation]-3;
			FillRect.right = m_pHoriPoint[nHoriStation]+3;
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
void CDrawLineT::DrawBarLine(CRect rRect, int nLineWidth,
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
	int nCount;
	//////////////////////////////////////////////////////////////////////////
	//画出线
	int nHoriStation;
	for(nCount = 0;nCount < lValueNum;nCount++)
	{
		if(pValues[nCount]== 0x7fffffff)continue;
		nHoriStation = nCount;//横坐标位置

		if(pValues[nCount] < lZeroStation)
			OldPen = m_pDC->SelectObject(&RiseLinePen);
		else
			OldPen = m_pDC->SelectObject(&DownLinePen);
		m_pDC->MoveTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth/2,lZeroStation);
		m_pDC->LineTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth/2,pValues[nCount]);
		m_pDC->SelectObject(OldPen);
	}
}

void CDrawLineT::DrawVolumnBar(CRect rRect, int nLineWidth,
										 int *pValues,int lValueNum, 
										 COLORREF clrRiseColor, COLORREF clrDownColor)
{
	if(pValues == NULL)return;
	if(m_pHoriPoint == NULL)return;
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//保证数据有效性
	if((m_fMinPrice == m_fMaxPrice)&&(m_fMaxPrice == 0))return;
	if( m_pKUnit == NULL)return;
	if( m_nDataBegin < 0)return;
	
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//内存区错误
	
	CPen RisePen(PS_SOLID,nLineWidth,clrRiseColor);
	CPen DownPen(PS_SOLID,nLineWidth,clrDownColor);
	CBrush DownBrush(clrDownColor);
				
	int nVolBarBottomValue = m_rectDraw.bottom-2; 
	int nHoriStation;
	int nVolBarTopValue;
	
	CPen *OldPen;
	CBrush *oldBrush;
	int nCount;
	CRect RectangleRect;
	for(nCount=0;nCount<lValueNum;nCount++)
	{
		nVolBarTopValue = pValues[nCount];

		nHoriStation = nCount;
		
	    if(nVolBarTopValue > nVolBarBottomValue)
		{
			nVolBarTopValue = nVolBarBottomValue-1;
		}
		if( nVolBarTopValue != nVolBarBottomValue)
		{
			if(m_pKUnit[nCount+m_nDataBegin].m_fOpenPrice <= m_pKUnit[nCount+m_nDataBegin].m_fClosePrice)
			{	
				OldPen = m_pDC->SelectObject(&RisePen);
				oldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);//设置非填充状态
			}
			else
			{
				OldPen = m_pDC->SelectObject(&DownPen);
				oldBrush = m_pDC->SelectObject(&DownBrush);//设置填充状态
			}
			 RectangleRect = CRect(m_pHoriPoint[nHoriStation],
									nVolBarTopValue,
									m_pHoriPoint[nHoriStation]+m_nKLineWidth,
									nVolBarBottomValue);//画图区域

			m_pDC->Rectangle(RectangleRect);//画出矩形
			
			m_pDC->SelectObject(oldBrush);			
			m_pDC->SelectObject(OldPen);
		}
	}
}


//函数功能:　画单个K线
//函数参数:　pDC 画图设备指针
//			 nStation 当前K线位置
//函数返回： 无
void CDrawLineT::DrawKLine(int nStation)
{
	ASSERT( nStation >= 0);

	if( NULL == m_pHoriPoint)return;

	if(m_pKUnit == NULL)return;
	if( m_nDataBegin < 0)return;



	int nHoriStation;
	nHoriStation = nStation-m_nDataBegin;

	COLORREF clrLine;
	const SHistoryUnit* pKUnit = m_pKUnit+nStation;//数据

	ASSERT( pKUnit);

	if(pKUnit->m_fOpenPrice <= pKUnit->m_fClosePrice)//上涨
		clrLine = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	else//下跌
		clrLine =  CInterfaceManager::Instance()->GetSysColor(ID_KLINEDOWN_COLOR);
	CPen LinePen(PS_SOLID,1,clrLine);
	CPen* oldPen = m_pDC->SelectObject(&LinePen);

	int nOpenStation,nCloseStation;//矩形区域
	nOpenStation = GetStationFromPrice(pKUnit->m_fOpenPrice);//开盘价位置
	nCloseStation = GetStationFromPrice(pKUnit->m_fClosePrice);//收盘价位置
	if(nOpenStation > nCloseStation)//上涨
	{//空心
		CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
		m_pDC->Rectangle(m_pHoriPoint[nHoriStation],nOpenStation,
			m_pHoriPoint[nHoriStation]+m_nKLineWidth,nCloseStation);		
		m_pDC->SelectObject(OldBrush);
	}
	else if(nOpenStation < nCloseStation)//下跌
	{
		CRect FillRect = CRect(m_pHoriPoint[nHoriStation],
			nOpenStation,
			m_pHoriPoint[nHoriStation]+m_nKLineWidth,
			nCloseStation);
		m_pDC->FillSolidRect(FillRect,clrLine);
	}
	else//相等
	{
		m_pDC->MoveTo(m_pHoriPoint[nHoriStation],nOpenStation);
		m_pDC->LineTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth,nCloseStation);
	}
	int nMiddle;//矩形中间区域
	nMiddle = m_pHoriPoint[nHoriStation] + m_nKLineWidth/2;

	int nHighStation,nLowStation;//最高最低价
	nHighStation = GetStationFromPrice(pKUnit->m_fMaxPrice);
	nLowStation = GetStationFromPrice(pKUnit->m_fMinPrice);
	if(nOpenStation< nCloseStation)
	{//交换位置
		int nTemp;
		nTemp = nOpenStation;
		nOpenStation = nCloseStation;
		nCloseStation = nTemp;
	}
	m_pDC->MoveTo(nMiddle,nHighStation);
	m_pDC->LineTo(nMiddle,nCloseStation);

	m_pDC->MoveTo(nMiddle,nLowStation);
	m_pDC->LineTo(nMiddle,nOpenStation-1);

	m_pDC->SelectObject(oldPen);
}

void CDrawLineT::DrawAmercianLine(int nStation)
{
	ASSERT( nStation >= 0);
	
	if( NULL == m_pHoriPoint)return;
	
	if(m_pKUnit == NULL)return;
	if( m_nDataBegin < 0)return;
	
	COLORREF clrLine;
	const SHistoryUnit* pKUnit = m_pKUnit+nStation+m_nDataBegin;//数据
	
	ASSERT( pKUnit);

	int nHoriStation;
	
	nHoriStation = nStation;
	
	if(pKUnit->m_fOpenPrice <= pKUnit->m_fClosePrice)//上涨
		clrLine = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	else//下跌
		clrLine =  CInterfaceManager::Instance()->GetSysColor(ID_KLINEDOWN_COLOR);
	CPen LinePen(PS_SOLID,1,clrLine);
	CPen* oldPen = m_pDC->SelectObject(&LinePen);
	
	int nOpenStation,nCloseStation;//矩形区域
	nOpenStation = GetStationFromPrice(pKUnit->m_fOpenPrice);//开盘价位置
	nCloseStation = GetStationFromPrice(pKUnit->m_fClosePrice);//收盘价位置
	
	int nHighStation,nLowStation;//最高最低价
	nHighStation = GetStationFromPrice(pKUnit->m_fMaxPrice);
	nLowStation = GetStationFromPrice(pKUnit->m_fMinPrice);
	
	int nMiddle;//矩形中间区域
	nMiddle = m_pHoriPoint[nHoriStation] + m_nKLineWidth/2;
	m_pDC->MoveTo(nMiddle,nLowStation);
	m_pDC->LineTo(nMiddle,nHighStation);//中间竖线
	
	m_pDC->MoveTo(nMiddle,nOpenStation);
	m_pDC->LineTo(m_pHoriPoint[nHoriStation],nOpenStation);//开盘价
	
	m_pDC->MoveTo(nMiddle,nCloseStation);
	m_pDC->LineTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth,nCloseStation);//收盘价
	
	
	m_pDC->SelectObject(oldPen);
}

void CDrawLineT::GetMaxMinValue(float &nMaxValue, float &nMinValue)
{
	nMaxValue = m_fMaxPrice;
	nMinValue = m_fMinPrice;
}

void CDrawLineT::SetMaxMinValue(float nMaxValue, float nMinValue)
{
	m_fMinPrice = nMinValue;
	m_fMaxPrice = nMaxValue;
}

void CDrawLineT::SetTitleBeginStation(int nStation)
{
	m_nTitleBeginStation = nStation;
}

int CDrawLineT::MouseDbClitTitle(CPoint pt)
{
	if(m_TitleRect.PtInRect(pt))
	{
		if(m_TitleNameRect.PtInRect(pt))
			return 1;
		else if( m_TitleParamRect.PtInRect(pt))
			return 2;
	}
	return 0;
}

const CRect& CDrawLineT::GetTitleParamRect()
{
	return m_TitleParamRect;
}

void CDrawLineT::SetRect(CRect rRect, int nRectType)
{//用于均线
	switch(nRectType)
	{
	case 0:
		m_TitleRect = rRect;
		break;
	case 1:
		m_AxisRect = rRect;
		break;
	case 2:
		m_rectDraw = rRect;
		CalcStation();//重新计算位置
		break;
	case 3:
		m_TotalRect = rRect;
	default:
		break;
	}
}

void CDrawLineT::ResetData()
{
	m_pKUnit = NULL;
	m_pHoriPoint = NULL;
	m_nValueNum = 0;
	m_nDataBegin = 0;
	m_fMaxPrice = 0.0000001f;
	m_fMinPrice = 10000000.0f;
}

int CDrawLineT::GetParamNum()
{
	return m_nParamNum;
}

BOOL CDrawLineT::ResetParaBuffers(int nParam, int nCurvers)
{
	//删除线型
	if (m_pLineType != NULL)
	{
		delete []m_pLineType;
		m_pLineType = NULL;
	}
	
	//删除参数
	if (m_pParamValue != NULL)
	{
		delete []m_pParamValue;
		m_pParamValue = NULL;
	}
	
	//删除颜色
	if (m_pColorValue != NULL)
	{
		delete []m_pColorValue;
		m_pColorValue = NULL;
	}

	m_nCurves = nCurvers;
	m_nParamNum = nParam;//参数数量
		
	m_pLineType = new int[m_nCurves];
	ZeroMemory(m_pLineType,m_nCurves * sizeof(int));
	m_pParamValue = new int[m_nParamNum];
	ZeroMemory(m_pParamValue,m_nParamNum * sizeof(int));
	m_pColorValue  = new COLORREF[m_nCurves];

	if (m_pLineType == NULL || m_pParamValue == NULL  ||
		m_pColorValue == NULL )
		return FALSE;

	for(int i = 0;i<m_nCurves;i++)
	{
		m_pColorValue[i]= CInterfaceManager::Instance()->GetSysColor(ID_INDEXLINE1_COLOR+i);
	}

	return TRUE;
}

void CDrawLineT::GetDrawRect(CRect& rRect)
{
	rRect = m_rectDraw;
}

BOOL CDrawLineT::GetDateFromPoint(const CPoint& point,int& nDay,float& fPrice)
{
	int nStation;
	int nDiffent;
	nDiffent = 0;

	nStation = (point.x - m_rectDraw.left)*(m_nValueNum-m_nDataBegin)/m_rectDraw.Width();
	if(nStation>=0)
	{
		if(nStation+m_nDataBegin+nDiffent<m_nValueNum)
			nDay= m_pKUnit[nStation+m_nDataBegin+nDiffent].m_uiDate;
		else
			nDay= m_pKUnit[m_nValueNum-1].m_uiDate + (m_nValueNum-nStation-m_nDataBegin-nDiffent);
	}
	else
	{
		nDiffent = -nStation;
	}
	fPrice = m_fMinPrice +(m_rectDraw.bottom - point.y)*((m_fMaxPrice- m_fMinPrice)/m_rectDraw.Height());

	return TRUE;
}

BOOL CDrawLineT::GetPointFromDate(int nDay,float fPrice,CPoint& point)
{
	point.y = GetStationFromPrice(fPrice);//
	//////////////////////////////////////////////////////////////////////////
	//查找时间
	for(int j=m_nDataBegin;j<m_nValueNum;j++)
	{
		if(nDay <= m_pKUnit[j].m_uiDate)
		{
			point.x = m_pHoriPoint[j-m_nDataBegin]+m_nKLineWidth/2;
			break;
		}
	}
	if(nDay > m_pKUnit[m_nValueNum-1].m_uiDate)
	{
		point.x = m_pHoriPoint[m_nValueNum-m_nDataBegin-1]+m_nKLineWidth/2 +(nDay- m_pKUnit[m_nValueNum-1].m_uiDate)*(m_pHoriPoint[1]-m_pHoriPoint[0]);
	}
	return TRUE;
}

BOOL CDrawLineT::MouseDown(CPoint point)
{
	if(!m_rectDraw.PtInRect(point))
	{
		m_nCurrentSelCurves = -1;
		m_nSelectedDrawLine = -1;
		for(int nCount = 0;nCount<m_ArrDrawLine.GetSize();nCount++)
		{
			m_ArrDrawLine[nCount].CheckIsLine(point);
		}
		m_nDrawState = SELECTSTATE;
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//检索是否为画线状态

	int nCount;
	if(m_nDrawState == SELECTSTATE)
	{
		BOOL bFind;
		int nSel;
		for(nCount= 0;nCount<m_ArrDrawLine.GetSize();nCount++)
		{
			CDrawLineTool& draw = m_ArrDrawLine[nCount];
			if(nSel = draw.CheckIsLine(point))
			{
				bFind = TRUE;
				m_nSelectedState = nSel; 
				m_nSelectedDrawLine = nCount;
				//////////////////////////////////////////////////////////////////////////
				//锁定移动区域				
				CRect cliprect;
				cliprect = m_rectDraw;
				CPublic::Instance()->GetView(VIEWINFO_STOCK)->ClientToScreen(cliprect);
				::ClipCursor(cliprect);
			}
		}
		if(bFind)
		{
			m_nDrawState = MOVEDRAW;
			m_SelectDrawLineOrginPoint = point;
			return TRUE;
		}
		else
			m_nSelectedDrawLine = -1;
	}
	//////////////////////////////////////////////////////////////////////////
	if(m_nDrawState == BEGINDRAW)
	{//画线开始响应
		CRect cliprect;
		cliprect = m_rectDraw;
		CPublic::Instance()->GetView(VIEWINFO_STOCK)->ClientToScreen(cliprect);
		::ClipCursor(cliprect);
		
		m_ArrDrawLine.SetSize(m_ArrDrawLine.GetSize()+1);
		for(nCount = 0;nCount<m_ArrDrawLine.GetSize();nCount++)
		{
			CDrawLineTool& draw = m_ArrDrawLine[nCount];
			draw.SetSelectState(FALSE);
		}
		CDrawLineTool& draw = m_ArrDrawLine[m_ArrDrawLine.GetSize()-1];
		draw.SetDrawMode(m_nLineType-1);
		draw.SetParent(this);
		COLORREF crDrawLine= CInterfaceManager::Instance()->GetSysColor(ID_DRAWLINE_COLOR);
		draw.SetColor(crDrawLine);
		draw.MouseLBDown(point);//
		m_nDrawState = LEFTBTNDOWN;
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	//检索当前画线
	//比较当前点
	int nDiffent;
	m_nCurrentSelCurves = -1;
	if(NULL == m_ppCurveStation)return FALSE;
	if(m_nCurrentStation <0 || m_nDataBegin < 0)return FALSE;
	for(nCount = 0;nCount<m_nCurves;nCount++)
	{
		nDiffent = m_ppCurveStation[nCount][m_nCurrentStation+m_nDataBegin] - point.y;
		if(10>abs(nDiffent))
		{//找到
			m_nCurrentSelCurves = nCount;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDrawLineT::MouseUp(CPoint point)
{
	if(!m_rectDraw.PtInRect(point))return FALSE;

	if((m_nSelectedDrawLine!= -1)&&(m_nDrawState == MOVEDRAW))
	{
		CDrawLineTool& draw = m_ArrDrawLine[m_nSelectedDrawLine];
		if(m_nSelectedState == SELECTEDFIRSTPOINT)
		{
			draw.MouseLBDown(point);
		}
		else if(m_nSelectedState == SELECTEDENDPOINT)
		{
			draw.MouseLBUp(point);
		}
		else if(m_nSelectedState ==SELECTTHIRDPOINT)
		{
			draw.SetThirdPoint(point);
		}
		else if(m_nSelectedState == SELECTRECTANGLE)
		{	
			CSize move;
			move = point- m_SelectDrawLineOrginPoint;
			draw.MoveEnd(move);
			m_SelectDrawLineOrginPoint = point;
		}
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		m_nDrawState = SELECTSTATE;
		::ClipCursor(NULL);
		return TRUE;
	}
	
	if(m_nDrawState == LEFTBTNDOWN)
	{
		m_nSelectedDrawLine = m_ArrDrawLine.GetSize()-1;
		CDrawLineTool& draw = m_ArrDrawLine[m_nSelectedDrawLine];
		::ClipCursor(NULL);
		draw.MouseLBUp(point);//
		m_nDrawState = SELECTSTATE;
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	//检索是否为画线状态

	return FALSE;
}

void CDrawLineT::SetDrawLineType(int nLineType)
{
	m_nLineType = nLineType;
	if(nLineType)
	{
		m_nDrawState = BEGINDRAW;
	}
	else
	{
		m_nDrawState = SELECTSTATE;
	}
}

BOOL CDrawLineT::DeleteCurrentDrawLine()
{
	//////////////////////////////////////////////////////////////////////////
	//检索是否为画线状态
	if(m_nSelectedDrawLine == -1)return FALSE;
	if(m_ArrDrawLine.GetSize() == 0)return FALSE;
	m_ArrDrawLine.RemoveAt(m_nSelectedDrawLine);//删除当前划线
	m_nSelectedDrawLine = -1;
	return TRUE;
}

BOOL CDrawLineT::MouseMove(CPoint point)
{
	if((m_nSelectedDrawLine!= -1)&&(m_nDrawState == MOVEDRAW)&&m_ArrDrawLine.GetSize())
	{
		CDrawLineTool& draw = m_ArrDrawLine[m_nSelectedDrawLine];
		if(m_nSelectedState == SELECTEDFIRSTPOINT)
		{
			draw.MouseLBDown(point);
		}
		else if(m_nSelectedState == SELECTEDENDPOINT)
		{
			draw.MouseLBUp(point);
		}
		else if(m_nSelectedState ==SELECTTHIRDPOINT)
		{
			draw.SetThirdPoint(point);
		}
		else if(m_nSelectedState == SELECTRECTANGLE)
		{	
			CSize move;
			move = point- m_SelectDrawLineOrginPoint;
			if(move.cx&&move.cy)
			{
				draw.SetOffset(move);
				m_SelectDrawLineOrginPoint = point;
			}
			else
				return FALSE;
		}
		//////////////////////////////////////////////////////////////////////////
		switch(m_nSelectedState)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			{
				CSize moveSize;
				moveSize = point - m_SelectDrawLineOrginPoint;
				if(abs(moveSize.cx) > abs(moveSize.cy))
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));
				else
					SetCursor(LoadCursor(NULL,IDC_SIZENS));
				m_SelectDrawLineOrginPoint = point;
			}
			break;
		case 4:
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));	
			break;
		default:
			break;
		}		
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	if(m_nDrawState == LEFTBTNDOWN)
	{
		CDrawLineTool& draw = m_ArrDrawLine[m_ArrDrawLine.GetSize()-1];
		draw.MouseLBUp(point);//
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	//得到当前位置
	m_ptNowStation = point;
	if(m_TitleRect.PtInRect(point))
		return TRUE;
	return FALSE;
}

BOOL CDrawLineT::MouseRBDown(CPoint point)
{
	//比较当前点
	int nDiffent;
	m_nCurrentSelCurves = -1;
	if(!m_rectDraw.PtInRect(point))return FALSE;
	if(NULL == m_ppCurveStation)return FALSE;
	if(m_nCurrentStation <0 || m_nDataBegin < 0)return FALSE;
	int nCount;
	int nStation;
	nStation =0;
	for(nCount=0;nCount<m_nHoriPoint;nCount++)
	{
		if(point.x <= m_pHoriPoint[nCount])
		{
			nStation = nCount;
			break;
		}
	}
	
	for(nCount = 0;nCount<m_nCurves;nCount++)
	{
		nDiffent = m_ppCurveStation[nCount][nStation+m_nDataBegin] - point.y;
		if((5>nDiffent)&&(-5<nDiffent))
		{//找到
			m_nCurrentSelCurves = nCount;
			return TRUE;
		}
	}
	return FALSE;
}

void CDrawLineT::RemoveAllDrawLine()
{
	m_ArrDrawLine.RemoveAll();
	m_nSelectedDrawLine = -1;
}
