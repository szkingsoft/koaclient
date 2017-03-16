// TechAnalysisKLine.cpp: implementation of the CTechAnalysisKLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "TechAnalysisKLine.h"
#include "techanalysisself.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const int KLINEW[] = {1,2,3,5,7,10,15,20,28,38};//�̶�K�߿��
const int HCELLW[] = {1,2,4,5,6,8,9,10,18,24,36,48};//K�߼�����

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTechAnalysisKLine::CTechAnalysisKLine()
{
	m_bDrawMA = FALSE;

	m_dwShowStyle = 0;

	m_pMAIndex = NULL;
	m_bRBtnDown = FALSE;

	m_nMaxPriceStation = 0;
	m_nMinPriceStation = 0;



	m_bSelectPointError = FALSE;
	m_ptRBtnBegin = CPoint(0,0);
	m_ptRBtnEnd = CPoint(0,0);

	memset(m_SelectRect,0,sizeof(m_SelectRect));
	
	m_nFristClickStation = -1;
	m_nSecondClickStation = -1;
	m_fVertZoomRaw = 1;
}

CTechAnalysisKLine::~CTechAnalysisKLine()
{
	if(m_pMAIndex)
	{
		delete m_pMAIndex;
		m_pMAIndex = NULL;
	}
}

//�������ܣ�����K�߼���ָ���е�����ͼ��
//����������pDC
//�������أ���
BOOL CTechAnalysisKLine::Draw(CDC *pDC)
{
	m_pDC = pDC;
	if(!pDC)return FALSE;
	if(	m_pHoriPoint == NULL)return FALSE;

    if( m_nDataBegin < 0)
		return FALSE;
	//////////////////////////////////////////////////////////////////////////

	if(m_bRBtnDown&&(m_ptRBtnEnd.x != 0)&&(m_ptRBtnEnd.y != 0)
		&&(m_ptRBtnBegin.x != 0)&&(m_ptRBtnBegin.y != 0))
	{//�����Ҽ�����ʱ��ѡ���
		CPen* NewOldPen = (CPen*)m_pDC->SelectStockObject(NULL_PEN);
		CBrush  selectBrush(CInterfaceManager::Instance()->GetSysColor(ID_GRIDROWSEL_COLOR));
		CBrush* oldBrush = (CBrush*)m_pDC->SelectObject(&selectBrush);
		m_pDC->Rectangle(m_ptRBtnBegin.x,m_ptRBtnBegin.y,m_ptRBtnEnd.x,m_ptRBtnEnd.y);
		m_pDC->SelectObject(oldBrush);
		m_pDC->SelectObject(NewOldPen);		
	}
	//////////////////////////////////////////////////////////////////////////
	COLORREF clrLine;
	clrLine = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	COLORREF clrEquire;
	clrEquire = RGB(255,50,50);//CInterfaceManager::Instance()->GetSysColor(ID_STATCOLOR);
	
	CFont * pFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	SetFont(pFont);
	float* pStation= new float[m_nVAxisCount];
	for(int i= 0;i<m_nVAxisCount;i++)
	{
		if (m_nVAxisCount==1)
		{
			pStation[i]= m_fMinPrice + ((m_fMaxPrice-m_fMinPrice)*m_fVertZoomRaw)/2;
		}
		else
		{
			pStation[i]= m_fMinPrice + ((m_fMaxPrice-m_fMinPrice)*m_fVertZoomRaw)/(m_nVAxisCount-1)*i;
		}
		//pStation[i]= m_fMinPrice + ((m_fMaxPrice-m_fMinPrice)*m_fVertZoomRaw)/(m_nVAxisCount-1)*i;
	}


	DrawAxisStyle(m_AxisRect,
	pStation,
	m_nVAxisCount,
	KLINEMODE,
	clrLine,
	clrEquire);	//��������

	delete []pStation;
	//////////////////////////////////////////////////////////////////////////
	//����K�߼���ʾ��Ϣ
	int nCount;	

	{
		//////////////////////////////////////////////////////////////////////////
		//��������K��
		for(nCount= m_nDataBegin;nCount<m_nValueNum ;nCount++)
			DrawKLine(nCount);
	}
	//////////////////////////////////////////////////////////////////////////
	DrawTitleText();//�������ı�
	//////////////////////////////////////////////////////////////////////////
	if((m_pMAIndex))
		m_pMAIndex->Draw(pDC);//������
	//////////////////////////////////////////////////////////////////////////
	//������߼ۣ���ͼ�
	DrawMaxAndMinSign();
	//////////////////////////////////////////////////////////////////////////
	//�������¼۸�
	DrawCurrentPrice();
	//////////////////////////////////////////////////////////////////////////
	//�������໭ͼ����
	for(nCount= 0;nCount<m_ArrDrawLine.GetSize();nCount++)
	{
		CDrawLineTool& draw = m_ArrDrawLine[nCount];
		draw.Draw(pDC);
	}
	return TRUE;
}

void CTechAnalysisKLine::DrawCurrentPrice()
{
	int nStation;
	nStation = GetStationFromPrice(m_fLastPrice);

	int nOldMode = m_pDC->SetBkMode(TRANSPARENT);
	COLORREF LineColor = CInterfaceManager::Instance()->GetSysColor(ID_INDEXLINE7_COLOR);
	CPen newPen(PS_SOLID,1,RGB(255,255,255/*200,200,200*/));
	CPen* OldPen = m_pDC->SelectObject(&newPen);
	m_pDC->MoveTo(m_rectDraw.left,nStation);
	m_pDC->LineTo(m_rectDraw.right,nStation);
	CString strCurPrice;
	CString szFormat;
	szFormat.Format(_T("%%.%df"),m_nDecimal);
	strCurPrice.Format(szFormat,m_fLastPrice);
	CRect textRect;
	//m_pDC->DrawTextA(strCurPrice,textRect,DT_CALCRECT);
	//CRect rectRight;
	//rectRight.left = m_rectDraw.right+5;
	//rectRight.top = nStation-textRect.Height()/2-1;
	//rectRight.bottom = rectRight.top+textRect.Height()/*+1*/;
	//rectRight.right = rectRight.left+textRect.Width()+1;

	COLORREF crText = CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR);
	COLORREF crLine = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	COLORREF crFill = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_pDC->SetTextColor(crText);
	CFont * textFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	CFont * OldFont = m_pDC->SelectObject(textFont);

	///////
	CSize textSize = m_pDC->GetTextExtent(strCurPrice);
	CRect rectRight;
	rectRight.left = m_rectDraw.right+5;
	rectRight.top = nStation-textSize.cy/2-1;
	rectRight.bottom = rectRight.top+textSize.cy/*+1*/;
	rectRight.right = rectRight.left+textSize.cx+5;
	//////////

	CBrush FillBrush(crFill);
	CPen  BordLine(PS_SOLID,1,crLine);
	CBrush* OldBrush = m_pDC->SelectObject(&FillBrush);
	m_pDC->Rectangle(rectRight);
	m_pDC->FillSolidRect(rectRight,RGB(0,0,255));
	m_pDC->DrawTextA(strCurPrice,rectRight,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_pDC->SelectObject(OldFont);
	m_pDC->SelectObject(OldBrush);
	m_pDC->SelectObject(OldPen);
	m_pDC->SetBkMode(nOldMode);
}

//�������ܣ���K��ָ����ʾֵ
//������������
//�������أ���
void CTechAnalysisKLine::DrawTitleText()
{
	CFont* pTextFont = CInterfaceManager::Instance()->GetSysFont(ID_HISTORYPICTITLEFONT);
	CFont* OldFont = m_pDC->SelectObject(pTextFont);

	int nMode = m_pDC->SetBkMode(TRANSPARENT);
	COLORREF TextColor = CInterfaceManager::Instance()->GetSysColor(ID_FORMTITLE_COLOR);
	m_pDC->SetTextColor(TextColor); 
	m_pDC->DrawText(m_szStockName,m_TitleRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	m_pDC->SelectObject(OldFont);

	m_pDC->SetBkMode(nMode);
	// ���ò�����������
	m_TitleParamRect = m_TitleRect;
	m_TitleParamRect.left += m_pDC->GetTextExtent(m_szStockName).cx;
}

void CTechAnalysisKLine::CalculateValue()
{
}

//�������ܣ����������ڵ�ָ�������Сֵ
//������������
//�������أ���
void CTechAnalysisKLine::CalcMinMaxPrice()
{
	float fPow;
	fPow = pow((double)10,(int)m_nDecimal);
	//////////////////////////////////////////////////////////////////////////
	//���������ͼ�
	int nCount;
	int nMaxPrice,nMinPrice;
	nMaxPrice = 0x80000000;//��Сֵ
	nMinPrice = 0x7fffffff;//���ֵ 

	int nValue1,nValue2;
	for(nCount= m_nDataBegin;nCount<m_nValueNum;nCount++)
	{
		nValue1 = m_pKUnit[nCount].m_fMaxPrice*fPow;
		nValue2 = m_pKUnit[nCount].m_fMinPrice*fPow;
		if(nMaxPrice < nValue1)
		{
			nMaxPrice = nValue1;
			m_nMaxPriceStation = nCount;
		}
		if(nMinPrice > nValue2)
		{
			nMinPrice = nValue2;
			m_nMinPriceStation = nCount;
		}
	}

	//m_nVAxisCount = 5;

	float fBiz;
	fBiz= (nMaxPrice-nMinPrice)/20;
	m_fMaxPrice = nMaxPrice+fBiz;//m_fMaxPrice>?m_fMaxPrice:nMaxPrice;
	m_fMaxPrice /= fPow;
	m_fMinPrice = nMinPrice-fBiz;//m_fMinPrice<?m_fMinPrice:nMinPrice;
	m_fMinPrice /= fPow;
}

//�������ܣ�������ʷ��ͼ��Ʊ����
//����������szTitle��Ʊ����
//�������أ���
void CTechAnalysisKLine::SetTitleInfo(CString szTitle,int nDataType)
{
	m_szStockName = szTitle;
	if(m_pMAIndex)
		m_pMAIndex->SetTitleBeginStation(m_szStockName.GetLength());
	m_nCircleType = nDataType;
}


void CTechAnalysisKLine::ResetData()
{
	CTechAnalysisBasePic::ResetData();
	if(m_pMAIndex)m_pMAIndex->ResetData();
}

BOOL CTechAnalysisKLine::UpdateData(LPVOID pNewData)
{	
	//////////////////////////////////////////////////////////////////////////	
	CTechAnalysisBasePic::UpdateData(pNewData);
	m_nDecimal = ((CMulQuantitySequence*)pNewData)->m_lMax;
	m_fLastPrice = m_pKUnit[m_nValueNum-1].m_fClosePrice;

	float fMaxValue,fMinValue;
	if(m_pMAIndex)
	{
		m_pMAIndex->UpdateData(pNewData);//�������ݸ�����

		m_pMAIndex->GetMaxMinValue(fMaxValue,fMinValue);
		if(m_fMaxPrice< fMaxValue)
		{
			if(fMaxValue != 0)
				m_fMaxPrice = fMaxValue;
		}
		if(m_fMinPrice < 0.001f || m_fMinPrice > fMinValue)
		{
			if(fMinValue != 0)
				m_fMinPrice = fMinValue;
		}
		m_pMAIndex->SetMaxMinValue(m_fMaxPrice,m_fMinPrice);

		m_pMAIndex->CalcStation();
	}

	
	m_nFristClickStation = m_nDataBegin;
	m_nSecondClickStation = m_nDataBegin+m_nValueNum-1;
//	m_pArrPower = NULL;
//	m_nPowerNum = 0;
	return TRUE;
}

void CTechAnalysisKLine::MoveTo(CRect rRect)
{
	CBasePic::MoveTo(rRect);
	m_TitleRect = rRect;
	m_TitleRect.bottom = m_TitleRect.top+ TITLESIZE;

	m_AxisRect = rRect;//��������
	m_AxisRect.top = m_TitleRect.bottom;
	m_rectDraw = m_AxisRect;//��ͼ����
	m_rectDraw.left += 3;
	m_rectDraw.right -= 3;
	//////////////////////////////////////////////////////////////////////////
	if(m_pMAIndex)//m_pMAIndex->MoveTo(rRect);
	{//���þ�������j
		m_pMAIndex->SetShowStyle(NOAXISRECT);//���ò���Ҫ����
		m_pMAIndex->SetRect(m_TitleRect,0);
		m_pMAIndex->SetRect(m_AxisRect,1);
		m_pMAIndex->SetRect(m_TotalRect,3);
		m_pMAIndex->SetRect(m_rectDraw,2);
	}
	//////
	int iHight = m_rectDraw.Height();
	if(iHight>150)
	{
		m_nVAxisCount = 5;
	}
	else if(iHight>50)
	{
		m_nVAxisCount = 3;
	}
	else if(iHight>30)
	{
		m_nVAxisCount = 2;
	}
	else
	{
		m_nVAxisCount = 1;
	}
	////////
}

//����ƶ�
BOOL CTechAnalysisKLine::MouseMove(CPoint point)
{
	if(!m_rectDraw.PtInRect(point))
	{
		MouseMoveOut();//����Ƴ�
	}
	//////////////////////////////////////////////////////////////////////////
	m_ptNowStation = point;
	if(m_bRBtnDown&&m_rectDraw.PtInRect(point))
	{//�Ƿ��Ҽ���ť�Ѿ�����
		m_ptRBtnEnd = point;
		return TRUE;
	}
	BOOL bRet;
	bRet = CTechAnalysisBasePic::MouseMove(point);
	if(m_pMAIndex)
		return	m_pMAIndex->MouseMove(point);

	
	return bRet;
}

// ����Ƴ�
void CTechAnalysisKLine::MouseMoveOut()
{
	memset(m_SelectRect,0,sizeof(m_SelectRect));
}


void CTechAnalysisKLine::CalcStation()
{
}

void CTechAnalysisKLine::SetMAMode(BOOL bDraw,CString strIndex)
{
	if(m_pMAIndex)
	{
		delete m_pMAIndex;
		m_pMAIndex = NULL;
	}
	m_pMAIndex = new CTechAnalysisSelf((char*)(LPCTSTR)strIndex);
}

int CTechAnalysisKLine::GetMaxParam()
{
	int nMaxParam;
	nMaxParam = 0;
	if(m_pMAIndex)
		nMaxParam =  m_pMAIndex->GetMaxParam();
	return nMaxParam;
}

void CTechAnalysisKLine::SetCurrentStation(int iStation)
{
	CBasePic::SetCurrentStation(iStation);
	if(m_pMAIndex)m_pMAIndex->SetCurrentStation(iStation);
}

void CTechAnalysisKLine::SetHoriPoint(int *pHoriPoint, int nHoriPoint)
{
	CBasePic::SetHoriPoint(pHoriPoint,nHoriPoint);
	if(m_pMAIndex)m_pMAIndex->SetHoriPoint(pHoriPoint,nHoriPoint);
}

BOOL CTechAnalysisKLine::MouseDown(CPoint point)
{
	if(!CTechAnalysisBasePic::MouseDown(point)
		&&m_pMAIndex)
		return m_pMAIndex->MouseDown(point);
	
	return TRUE; 
}

int CTechAnalysisKLine::MouseDbClitTitle(CPoint pt)
{
	if( m_TitleRect.PtInRect(pt))
	{
		// K��ֻ�ܽ��в������л�
		if(m_pMAIndex)
		{
			m_TitleParamRect.right = m_TitleParamRect.left + m_pMAIndex->GetTitleParamRect().Width();
			if( m_TitleParamRect.PtInRect(pt))
				return 2;		
		}
	}

	return 0;
}

void CTechAnalysisKLine::DrawMaxAndMinSign()
{
	if(!m_pHoriPoint||!m_pKUnit)return;

	if(m_nMaxPriceStation < 0 || m_nMaxPriceStation < m_nDataBegin)
		return;

	CRect ItemRect;
	float fPrice;
	CString szItem;
	CSize TextSize;
	int nStation;

	
	int nOldBkMode = m_pDC->SetBkMode(TRANSPARENT);
	
	CFont* pTextFont = CInterfaceManager::Instance()->GetSysFont(ID_NUMBER_FONT);
	CFont * pOldFont = m_pDC->SelectObject(pTextFont);

	COLORREF crLine = CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR);
	CPen LinePen(PS_SOLID,1,crLine);
	CPen* pOldPen = m_pDC->SelectObject(&LinePen);
	//////////////////////////////////////////////////////////////////////////
	//��߼�
	fPrice = m_pKUnit[m_nMaxPriceStation].m_fMaxPrice;
	CString szFormat;
	szFormat.Format(_T("%%.%df"),m_nDecimal);
	szItem.Format(szFormat,fPrice);

	nStation = GetStationFromPrice(m_pKUnit[m_nMaxPriceStation].m_fMaxPrice);
	CPoint basePoint;
	basePoint.y = nStation-1;
	basePoint.x = (long)(m_pHoriPoint[(m_nMaxPriceStation- m_nDataBegin)] + m_nKLineWidth/2);
	CPoint endPoint;
	endPoint.x = basePoint.x+10;
	endPoint.y = basePoint.y;
	
	CPoint UpPoint,DownPoint;
	UpPoint.x = basePoint.x +5;
	UpPoint.y = basePoint.y -2;
	DownPoint.x = basePoint.x +5;
	DownPoint.y = basePoint.y +2;
	
	CRect szRect;
	TextSize = m_pDC->GetTextExtent(szItem);
	szRect.left = endPoint.x+1;
	szRect.top = endPoint.y;
	szRect.bottom = szRect.top + TextSize.cy;
	szRect.right = szRect.left + TextSize.cx;
	if(szRect.top > m_rectDraw.bottom)
	{//��Խ�±�
		endPoint.y = basePoint.y-4;
		szRect.bottom = endPoint.y-1;
		szRect.top =szRect.bottom -TextSize.cy;
	}
	if(szRect.right > m_rectDraw.right)
	{//��Խ�ұ߽�
		endPoint.x = basePoint.x -10;
		UpPoint.x = basePoint.x -5;
		DownPoint.x = basePoint.x -5;
		
		szRect.right = endPoint.x-1;
		szRect.left = szRect.right - TextSize.cx;
	}
	m_pDC->MoveTo(basePoint);
	m_pDC->LineTo(endPoint);
	m_pDC->MoveTo(basePoint);
	m_pDC->LineTo(UpPoint);
	m_pDC->MoveTo(basePoint);
	m_pDC->LineTo(DownPoint);
	
	TextSize = m_pDC->GetTextExtent(szItem);

	COLORREF crText = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	m_pDC->SetTextColor(crText);
	m_pDC->DrawText(szItem,szRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE| DT_NOPREFIX | DT_NOCLIP | DT_END_ELLIPSIS);	
	//////////////////////////////////////////////////////////////////////////
	//��ͼ�
	fPrice = m_pKUnit[m_nMinPriceStation].m_fMinPrice;
	szItem.Format(szFormat,fPrice);

	basePoint.y=GetStationFromPrice(m_pKUnit[m_nMinPriceStation].m_fMinPrice);
	basePoint.x = (long)(m_pHoriPoint[(m_nMinPriceStation-m_nDataBegin)] + m_nKLineWidth/2);
	endPoint.x = basePoint.x+10;
	endPoint.y = basePoint.y;
	UpPoint.x = basePoint.x +5;
	UpPoint.y = basePoint.y -2;
	DownPoint.x = basePoint.x +5;
	DownPoint.y = basePoint.y +2;
	
	TextSize = m_pDC->GetTextExtent(szItem);
	szRect.left = endPoint.x+1;
	szRect.bottom = endPoint.y-1;
	szRect.top =szRect.bottom -TextSize.cy;
	szRect.right = szRect.left + TextSize.cx;
	if(szRect.bottom > m_rectDraw.bottom)
	{//��Խ�±�
		endPoint.y = basePoint.y;
		szRect.bottom = endPoint.y-1;
		szRect.top =szRect.bottom -TextSize.cy;
	}
	if(szRect.right > m_rectDraw.right)
	{//��Խ�ұ߽�
		endPoint.x = basePoint.x -10;
		UpPoint.x = basePoint.x -5;
		DownPoint.x = basePoint.x -5;
		szRect.right = endPoint.x-1;
		szRect.left = szRect.right - TextSize.cx;
	}
	
	m_pDC->MoveTo(basePoint);
	m_pDC->LineTo(endPoint);
	m_pDC->MoveTo(basePoint);
	m_pDC->LineTo(UpPoint);
	m_pDC->MoveTo(basePoint);
	m_pDC->LineTo(DownPoint);
	
	
	crText = CInterfaceManager::Instance()->GetSysColor(ID_DOWN_COLOR);
	m_pDC->SetTextColor(crText);
	m_pDC->DrawText(szItem,szRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE| DT_NOPREFIX | DT_NOCLIP | DT_END_ELLIPSIS);

	m_pDC->SelectObject(pOldPen);
	m_pDC->SelectObject(pOldFont);
	m_pDC->SetBkMode(nOldBkMode);
}

int CTechAnalysisKLine::HitTest(CPoint point)
{
	m_CurrentPoint = point;
	if(m_rectDraw.PtInRect(point))
		return 1;
	return 0;
}

BOOL CTechAnalysisKLine::MouseRBDown(CPoint point)
{
	if(!CTechAnalysisBasePic::MouseRBDown(point)&&m_pMAIndex)
		return m_pMAIndex->MouseRBDown(point);
	if(this->m_rectDraw.PtInRect(point))
	{
		m_bRBtnDown = TRUE;
		return TRUE; 
	}
	return FALSE;
}

void CTechAnalysisKLine::MouseRBtnUp(CPoint pt)
{
	m_bRBtnDown = FALSE;
}

void CTechAnalysisKLine::DrawColorKLine(int nStation,COLORREF crKLine,int nMask)
{
	ASSERT( nStation >= 0);
	if( NULL == m_pHoriPoint)return;	
	if(NULL == m_pKUnit)return;
	if( m_nDataBegin < 0)return;

	int nHoriStation;
	nHoriStation = nStation;

	const SHistoryUnit* pKUnit = m_pKUnit+nStation+m_nDataBegin;//����	
	CPen LinePen(PS_SOLID,1,crKLine);
	CPen* oldPen = m_pDC->SelectObject(&LinePen);	
	int nOpenStation,nCloseStation;//��������
	nOpenStation = GetStationFromPrice(pKUnit->m_fOpenPrice);//���̼�λ��
	nCloseStation = GetStationFromPrice(pKUnit->m_fClosePrice);//���̼�λ��
	if(nOpenStation > nCloseStation)//����
	{//����
		CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
		m_pDC->Rectangle(m_pHoriPoint[nHoriStation],nOpenStation,
			m_pHoriPoint[nHoriStation]+m_nKLineWidth,nCloseStation);		
		m_pDC->SelectObject(OldBrush);
	}
	else if(nOpenStation < nCloseStation)//�µ�
	{
		CRect FillRect = CRect(m_pHoriPoint[nHoriStation],
			nOpenStation,
			m_pHoriPoint[nHoriStation]+m_nKLineWidth,
			nCloseStation);
		m_pDC->FillSolidRect(FillRect,crKLine);
	}
	else//���
	{
		m_pDC->MoveTo(m_pHoriPoint[nHoriStation],nOpenStation);
		m_pDC->LineTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth,nCloseStation);
	}
	int nMiddle;//�����м�����
	nMiddle = m_pHoriPoint[nHoriStation] + m_nKLineWidth/2;
	
	int nHighStation,nLowStation;//�����ͼ�
	nHighStation = GetStationFromPrice(pKUnit->m_fMaxPrice);
	nLowStation = GetStationFromPrice(pKUnit->m_fMinPrice);
	if(nOpenStation< nCloseStation)
	{//����λ��
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

//��������:��������������
void  CTechAnalysisKLine::DrawSpecialAxisStyle(CRect rRect,
						   float* pStation,
						   int nDiv,  
						   COLORREF crLineColor, 
						   COLORREF crNormalTextColor)
{
	if(m_fMinPrice == m_fMaxPrice)return;//�۸����
	if(m_pHoriPoint == NULL)return;
//	int nFristStation;
	int nCount;
	if( nDiv <=1 )return;	
	float fRise;
	fRise = (float)((m_fMaxPrice - m_fMinPrice)*m_fVertZoomRaw);
	fRise /= (nDiv-1);//ÿ��ļ۸�
	//////////////////////////////////////////////////////////////////////////
	//���
	CPen CordLine(PS_SOLID,1,crLineColor);
	CPen *OldPen;
	CFont *OldFont = m_pDC->SelectObject(m_pFont);
	int iYStation;
	//////////////////////////////////////////////////////////////////////////
	//�۸�ָ���	
	CPen DotLine(PS_DOT,1,crLineColor);
	OldPen = m_pDC->SelectObject(&DotLine);//��������
	CString szPrice;
	double fPrice;
	CRect ItemRect;//��Ԫ������
	CSize textsize;
	int nOldMode = m_pDC->SetBkMode(TRANSPARENT);

	for(nCount =0;nCount<nDiv;nCount++)
	{
		iYStation = rRect.bottom - rRect.Height()*(pStation[nCount]-m_fMinPrice)/((m_fMaxPrice-m_fMinPrice)*m_fVertZoomRaw);//nCount/(nDiv-1);
		m_pDC->MoveTo(rRect.left,iYStation);
// 		if((nFristStation > 0)&&(nFristStation <=m_nHoriPoint))
// 		{
// 			m_pDC->LineTo(m_pHoriPoint[ nFristStation]+m_nKLineWidth/2,iYStation);
// 		}
// 		else
		{
			m_pDC->LineTo(rRect.right,iYStation);
		}
		fPrice = pStation[nCount];
		szPrice = CPublic::Instance()->GetFormateValue(fPrice,m_nDecimal);
		textsize = m_pDC->GetTextExtent(szPrice);
		//////////////////////////////////////////////////////////////////////////
		//modify by david gong at 2013/09/15
// 		ItemRect.right = rRect.left-2;//�������
// 		ItemRect.left = rRect.left -2 -textsize.cx;
		ItemRect.left = rRect.right+2;//�������
		ItemRect.right = rRect.right+2+textsize.cx;
		//////////////////////////////////////////////////////////////////////////
		ItemRect.top = iYStation - textsize.cy/2;
		ItemRect.bottom = iYStation+ textsize.cy/2+1;
		m_pDC->SetTextColor(crNormalTextColor);
		m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);//�������

	}
	m_pDC->SelectObject(OldFont);
	m_pDC->SelectObject(OldPen);
	m_pDC->SetBkMode(nOldMode);
}

int CTechAnalysisKLine::FindStationFromDate(int nDate)
{
	nDate &=0x7fffffff;
	int nCount;
	int nStation;
	nStation = -1;
	if(m_pKUnit[0].m_uiDate >nDate)return -1;
	int nNewDate;
	for(nCount=0;nCount<m_nValueNum;nCount++)
	{
		nNewDate = m_pKUnit[nCount].m_uiDate&0x7fffffff;
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
	nStation -= m_nDataBegin;
	return nStation;
}

void CTechAnalysisKLine::DrawSelectRect()
{
	CRect rSelRect;
	rSelRect.SetRectEmpty();
	rSelRect.left =  FindStationFromDate(m_SelectRect[0].nDate);
	rSelRect.right  = FindStationFromDate(m_SelectRect[1].nDate);
	if((rSelRect.left >= (m_nValueNum-m_nDataBegin))||(rSelRect.right <0))return;
	if(rSelRect.left <0)rSelRect.left = 0;
	if(rSelRect.right >= (m_nValueNum-m_nDataBegin))rSelRect.right = m_nHoriPoint-1;

	rSelRect.left = m_pHoriPoint[rSelRect.left]+m_nKLineWidth/2;
	rSelRect.right = m_pHoriPoint[rSelRect.right]+m_nKLineWidth/2;

	float fPow;
	fPow = (float)pow(10,(double)m_nDecimal);
	
	rSelRect.top = GetStationFromPrice((float)m_SelectRect[0].nPrice/fPow);
	rSelRect.bottom = GetStationFromPrice((float)m_SelectRect[1].nPrice/fPow);
	if(!rSelRect.IsRectNull()&&(rSelRect.left != 0)
		&&(rSelRect.right != 0)
		&&(rSelRect.top !=0)&&(rSelRect.bottom != 0))
	{
		CPen* NewOldPen = (CPen*)m_pDC->SelectStockObject(NULL_PEN);
		CBrush  selectBrush(CInterfaceManager::Instance()->GetSysColor(ID_GRIDROWSEL_COLOR));
		CBrush* oldBrush = (CBrush*)m_pDC->SelectObject(&selectBrush);
		m_pDC->Rectangle(rSelRect);
		m_pDC->SelectObject(oldBrush);
		m_pDC->SelectObject(NewOldPen);	
	}
}


void CTechAnalysisKLine::ZoomVertAxis(bool bZoomIn)
{
	if(bZoomIn)
	{
		m_fVertZoomRaw *= 1.1;
	}
	else
	{
		if(m_fVertZoomRaw >1)
		{
			m_fVertZoomRaw *=10;
			m_fVertZoomRaw /=11;
		}
	}
	m_pMAIndex->SetVertZoomRaw(m_fVertZoomRaw);
}