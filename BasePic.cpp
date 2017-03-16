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


	m_pHoriPoint = NULL;	//ˮƽ����λ��

	m_nVertCellHeight = 20;
	m_nCurrentStation = -1;
	m_nValueNum = 0;

	m_nKLineWidth = 0;	
	m_nKLineSperator = 0;

	m_fMaxPrice = 0.0f;
	m_fMinPrice = 0.0f;
	
	ZeroMemory(&m_CurrentPoint,sizeof(CPoint));   //��ǰλ�õ�
	m_nVAxisCount = 0;       //��ֱ��������
	ZeroMemory(&m_rectDraw,sizeof(CRect));        //��Ч��ͼ����
	ZeroMemory(&m_TotalRect,sizeof(CRect));       //������
	
	m_bActivate = FALSE;		 //�Ƿ�Ϊ��ǰ����״̬

	m_ppCurveValue = NULL;//������
	m_nCurves = 0;
	m_pParamValue = NULL;//����	
	m_ppCurveStation = NULL;//����λ��ֵ	
	m_nVAxisCount = 0;
	m_pColorValue = NULL;//��ɫ����ֵ
	m_pLineType = NULL;
	m_fVertZoomRaw = 1;
}

CBasePic::~CBasePic()
{
	//////////////////////////////////////////////////////////////////////////
	DestroyObj();//�������
	//������
	if(m_pLineType)
	{
		delete []m_pLineType;
		m_pLineType = NULL;
	}
	if(m_pParamValue)
	{
		delete []m_pParamValue;//ɾ��ָ�����
		m_pParamValue = NULL;
	}
	if(m_pColorValue)
	{
		delete []m_pColorValue;//ɾ��ָ����ɫ
		m_pColorValue = NULL; 
	}
}

//��������:��������������
//��������:rRect ��ͼ����
//         pAxisPrice ����۸�����
//		   nDecimal   С����λ��
//	 nMode	  ��ͼģʽ
//	 crLineColor
//   crNormalTextColor ��ͨ����ɫ
//   crRiseTextColor   ����������ɫ
//   crDownTextColor   �µ�������ɫ
void CBasePic::DrawAxisStyle(CRect rRect,
							 float* pStation,
							 int nDiv, 
							 AXISMODE nMode,
							 COLORREF crLineColor, 
							 COLORREF crNormalTextColor)
{	
	if(m_fMinPrice == m_fMaxPrice)return;//�۸����
	if(m_pHoriPoint == NULL)return;
	if( nDiv < 1 )
		return;
	
	float fRise = (float)((m_fMaxPrice - m_fMinPrice)*m_fVertZoomRaw);
	fRise /= (nDiv-1);//ÿ��ļ۸�
	//////////////////////////////////////////////////////////////////////////
	//���
	CPen CordLine(PS_SOLID,1,crLineColor);
	CPen *OldPen = m_pDC->SelectObject(&CordLine);
	CFont *OldFont = m_pDC->SelectObject(m_pFont);
	CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
	if(nMode != KLINEMODE)
		m_pDC->Rectangle(rRect);
	int iYStation;
	m_pDC->SelectObject(OldPen);
	//////////////////////////////////////////////////////////////////////////
	//�۸�ָ���	
	CPen DotLine(PS_DOT,1,crLineColor);
	CPen SolidLine(PS_SOLID,1,crLineColor);

	OldPen = m_pDC->SelectObject(&DotLine);//��������
	int nCount;
	CString szPrice;
	double fPrice;
	CRect ItemRect;//��Ԫ������
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
		//��ʾ�������Ҳ�
// 		ItemRect.right = rRect.left-2;//�������
// 		ItemRect.left = rRect.left -2 -textsize.cx;
		ItemRect.left = rRect.right+2;//�������
		ItemRect.right = rRect.right +2 +textsize.cx;
		//////////////////////////////////////////////////////////////////////////
		ItemRect.top = iYStation - textsize.cy/2;
		ItemRect.bottom = iYStation+ textsize.cy/2+1;
		m_pDC->SetTextColor(crNormalTextColor);
		m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);//�������
	}
 	m_pDC->SelectObject(OldFont);
	m_pDC->SelectObject(OldBrush);
	m_pDC->SelectObject(OldPen);
	m_pDC->SetBkMode(nOldMode);
}

//���ٷֱ�����
void CBasePic::DrawPrecentAxisStyle(CRect rRect,
									   float* pStation,
									   int nDiv, 
									   AXISMODE nMode,
									   COLORREF crLineColor, 
									   COLORREF crNormalTextColor,
									   float fBasePrice)
{
	if(m_fMaxPrice == m_fMinPrice)return;//�۸����
	if(m_pHoriPoint == NULL)return;
	if( nDiv < 1 )
		return;
	
	float fRise = m_fMaxPrice - m_fMinPrice;
	fRise /= (nDiv-1);//ÿ��ļ۸�
	//////////////////////////////////////////////////////////////////////////
	//���
	CPen CordLine(PS_SOLID,1,crLineColor);
	CPen *OldPen = m_pDC->SelectObject(&CordLine);
	CFont *OldFont = m_pDC->SelectObject(m_pFont);
	CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
	if(nMode != KLINEMODE)
		m_pDC->Rectangle(rRect);
	int iYStation;
	m_pDC->SelectObject(OldPen);
	//////////////////////////////////////////////////////////////////////////
	//�۸�ָ���	
	CPen DotLine(PS_DOT,1,crLineColor);
	CPen SolidLine(PS_SOLID,1,crLineColor);
	
	OldPen = m_pDC->SelectObject(&DotLine);//��������
	int nCount;
	CString szPrice;
	double fPrice;
	CRect ItemRect;//��Ԫ������
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
		ItemRect.right = rRect.left-2;//�������
		ItemRect.left = rRect.left -2 -textsize.cx;
		ItemRect.top = iYStation - textsize.cy/2;
		ItemRect.bottom = iYStation+ textsize.cy/2+1;
		m_pDC->SetTextColor(crNormalTextColor);
		m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);//�������
	}
	m_pDC->SelectObject(OldFont);
	m_pDC->SelectObject(OldBrush);
	m_pDC->SelectObject(OldPen);
	m_pDC->SetBkMode(nOldMode);
}
//��������: ��������������,����ʵʱ�����ߡ����ߡ������������
//��������:  rRect ��ͼ����
//��	     nLineWidth �߿��
//           clrLineColor �ߵ���ɫ
//���������� pValues   ���ݵ�����
//           lValueNum ���е�����
//��������: ��
//�����쳣: ��
void CBasePic::DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, 
							   int* pValues, int lValueNum,BOOL bSelctState)
{
	if(pValues == NULL)return;
	if(m_pHoriPoint == NULL)return;
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//��֤������Ч��
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//�ڴ�������
	if((m_fMinPrice == m_fMaxPrice)&&(m_fMaxPrice == 0))return;	
	CPen LinePen(PS_SOLID,nLineWidth,clrLineColor);
	CPen *OldPen = m_pDC->SelectObject(&LinePen);
	int lCount;
	CBrush FillBrush(clrLineColor);
	CBrush* oldBrush = m_pDC->SelectObject(&FillBrush);
	
	// ԭ��û�г�ʼ��
	int nLastStation = 0;
	
	CRect FillRect;
	for(lCount = 1;lCount < lValueNum;lCount++)
	{
		if(pValues[lCount]== 0x7fffffff || pValues[lCount-1]== 0x7fffffff )
			continue;	
		m_pDC->MoveTo(m_pHoriPoint[lCount-1]+m_nKLineWidth/2,pValues[lCount-1]);
		m_pDC->LineTo(m_pHoriPoint[lCount]+m_nKLineWidth/2,pValues[lCount]);

		if((bSelctState)&&(m_pHoriPoint[lCount]-nLastStation >50))
		{//����ѡ�񽹵�
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

//��������: ������״����Ϣ
//��������: rRect ��ͼ����
//          nLineWidth �߿��
//          pValues   �����ָ��
//          lValueNum  �������
//          crRiseTextColor     ������ɫ
//          crDownTextColor     �µ���ɫ
//          nBaseValue   ��ֵ
//��������: ��
void CBasePic::DrawBarLine(CRect rRect, int nLineWidth,
							  int *pValues, int lValueNum,
							  COLORREF crRiseTextColor,COLORREF crDownTextColor,float fBaseValue)
{
	if(pValues == NULL)return;
	if(m_pHoriPoint == NULL)return;
	if( m_fMaxPrice ==  m_fMinPrice)
		return;	
	
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//��֤������Ч��
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//�ڴ�������
	CPen RiseLinePen(PS_SOLID,nLineWidth,crRiseTextColor);
	CPen DownLinePen(PS_SOLID,nLineWidth,crDownTextColor);
	int lZeroStation;//������λ��
	
	//�õ���׼��λ��
	lZeroStation = m_rectDraw.bottom - 
		           (fBaseValue - m_fMinPrice)*m_rectDraw.Height()/(m_fMaxPrice-m_fMinPrice);
	
	CPen *OldPen;
	int lCount;
	//////////////////////////////////////////////////////////////////////////
	//������
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

//�������ܣ�������������
//����������clrLine��������ɫ
//�������أ���
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

//�������ܣ����û�ͼ����
//����������rRect��ͼ����
//�������أ���
void CBasePic::MoveTo(CRect rRect)
{
	m_TotalRect = rRect;//������
}


//����ƶ�
BOOL CBasePic::MouseMove(CPoint point)
{
	return FALSE;
}

// ����Ƴ�
void  CBasePic::MouseMoveOut()
{
	return;
}

//����������
BOOL  CBasePic::MouseLBtnUp(CPoint point)
{
	return FALSE;
}

//����������
BOOL CBasePic::MouseLBtnDown(CPoint point)
{
	return FALSE;
}

//��ʵʱ��������
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
	if(nDiv<=1)return;//�������
	float fRise = (float)(m_fMaxPrice - m_fMinPrice);
	fRise /= (nDiv-1);//ÿ��ļ۸�
	//////////////////////////////////////////////////////////////////////////
	//���
	CPen CordLine(PS_SOLID,1,crLineColor);
	CPen *OldPen = m_pDC->SelectObject(&CordLine);
	CFont *OldFont = m_pDC->SelectObject(m_pFont);
	CBrush* OldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);
	m_pDC->Rectangle(rRect);
	m_pDC->SelectObject(OldPen);
	int iYStation;
	if((nMode == DOUBLESIDEDOUBLECOLOR)||(nMode == SINGLESIDEDOUBLECOLOR))
	{//����������
		if(0 == nDiv%2)
		{
			m_pDC->SelectObject(OldFont);
			m_pDC->SelectObject(OldBrush);
			return;//����Ǽ۸����Ʊ���������Ϊ�������������
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
		return;//�۸����
	}

	//////////////////////////////////////////////////////////////////////////
	//�۸�ָ���	
	CPen DotLine(PS_DOT,1,crLineColor);
	CPen SolidLine(PS_SOLID,1,crLineColor);
	OldPen = m_pDC->SelectObject(&SolidLine);
	int nOldMode = m_pDC->SetBkMode(TRANSPARENT);
	CFont *pProportionFont = CInterfaceManager::Instance()->GetSysFont(ID_PROPORTION_FONT);

	CString szPrice;
	CString szFormat;
	szFormat.Format(_T("%%.%df"),m_nDecimal);
	CRect ItemRect;//��Ԫ������
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
		ItemRect.right = rRect.left-2;//�������
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
		m_pDC->DrawText(szPrice,ItemRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);//�������

		if(nMode == DOUBLESIDEDOUBLECOLOR)
		{//�����Ҳ����
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
		iXStation = m_pHoriPoint[pXSign[nCount]];//��ǰλ��
		if(pXSign[nCount] == nHotSign)
		{//�ر��־λ
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

//��������: ���Ե��Ƿ��������� 
//��������: point ������
//�������أ��ɹ�Ϊ TRUE,ʧ��ΪFALSE
//�����쳣: ��
int CBasePic::HitTest(CPoint point)
{
	m_CurrentPoint = point;

	return m_rectDraw.PtInRect(point);
}

//��������: ���ú������
//��������: iHoriPoint ���������
//��������: ��
//�����쳣: ��
void CBasePic::SetHoriPoint(int *pHoriPoint,int nHoriPoint)
{
	m_nHoriPoint = nHoriPoint;
	m_pHoriPoint = pHoriPoint;
}

//�������ܣ����õ�ǰ����
//����������pFont
//�������أ���
CFont* CBasePic::SetFont(CFont *pFont)
{
	CFont* CurFont;
	CurFont = m_pFont;
	m_pFont = pFont;
	return CurFont;
}

//�������ܣ��Ƚϵõ������Сֵ
//��������: lMinPrice ���ص���Сֵ
//          lMaxPrice ���ص����ֵ
//          pData    ��������
//          lDataNum ��������
//�������أ���
void CBasePic::GetMaxMinPrice(float &fMinPrice, float &fMaxPrice,
								 float *pData, int nDataNum,BOOL bFlash)
{
	if(nDataNum == 0)return;//������ֱ�ӷ���
	if( pData == NULL)
		return;
	int nMaxPrice,nMinPrice;

	float fPow;
	fPow = pow(10,(double)m_nDecimal);

	if(bFlash)//ˢ�������ͼ�
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
 	{//ѭ���Ƚ�
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

//��������: �õ���ֱ�����߸���
//��������: ��ͼ����߶�
//��������: ��ֱ�߸���
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

//��������: ���õ�ǰ���λ��
//����������iStation��ǰ���λ��
//�������أ���
void CBasePic::SetCurrentStation(int iStation)
{
	m_nCurrentStation = iStation;
}

//�������ܣ�ͨ��λ�õõ�ָ��ֵ
//����������nStation��ǰ���λ��
//�������أ�ָ��ֵ
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

//�������ܣ�ͨ��ָ��ֵ�õ�λ��
//����������nValueָ��ֵ
//�������أ��ڻ�ͼ�����е�λ��
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

//��������: ��ʼ�����ݻ�����
//��������: nParam ָ���������
//          nBufferSize ����������
//          nDataSize   ���ݳ���
//�������أ���
void CBasePic::InitDataBuffer(int nDataSize)
{
	if(0 == m_nCurves)return;
	DestroyObj();//���������
	//////////////////////////////////////////////////////////////////////////
	//����������
	m_ppCurveValue = new float*[m_nCurves];
	ZeroMemory(m_ppCurveValue,m_nCurves*sizeof(float*));
	int nCount;
	if(nDataSize)
	{
		for(nCount= 0;nCount<m_nCurves;nCount++)
			m_ppCurveValue[nCount] = new float[nDataSize];

	}
	//////////////////////////////////////////////////////////////////////////
	//����λ��
	m_ppCurveStation = new int*[m_nCurves];
	ZeroMemory(m_ppCurveStation,m_nCurves*sizeof(int*));
	if(nDataSize)
	{
		for(nCount= 0;nCount<m_nCurves;nCount++)
			m_ppCurveStation[nCount] = new int[nDataSize];
	}
}

//�������ܣ��������
//������������
//�������أ���
void CBasePic::DestroyObj()
{
	//////////////////////////////////////////////////////////////////////////
	//ɾ������
	if(m_ppCurveValue)
	{
		int nCount;
		for(nCount=0;nCount<m_nCurves;nCount++)
		{//ɾ����������
			if(m_ppCurveValue[nCount])
			{
				delete []m_ppCurveValue[nCount];
				m_ppCurveValue[nCount] = NULL;
			}
		}
		delete[]m_ppCurveValue;
		m_ppCurveValue = NULL;
	}
	//ɾ������λ��
	if(m_ppCurveStation)
	{
		int nCount;
		for(nCount=0;nCount<m_nCurves;nCount++)
		{//ɾ����������
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










