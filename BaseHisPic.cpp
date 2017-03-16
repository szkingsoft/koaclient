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
	
	ZeroMemory(&m_CurrentPoint,sizeof(CPoint));   //��ǰλ�õ�
	m_nVAxisCount = 0;       //��ֱ��������
	ZeroMemory(&m_rectDraw,sizeof(CRect));        //��Ч��ͼ����
	ZeroMemory(&m_TotalRect,sizeof(CRect));       //������
	
	m_bActivate = FALSE;		 //�Ƿ�Ϊ��ǰ����״̬
	m_fVertZoomRaw = 1;
}

CBaseHisPic::~CBaseHisPic()
{
}

//��������:��������������
//��������:rRect ��ͼ����
//	       lMinValue ��С�۸�
//         lMaxValue ���۸�
//         pAxisPrice ����۸�����
//		   nDecimal   С����λ��
//	 nMode	  ��ͼģʽ
//	 crLineColor
//   crNormalTextColor ��ͨ����ɫ
//   crRiseTextColor   ����������ɫ
//   crDownTextColor   �µ�������ɫ
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
	if(lMaxValue == lMinValue)return;//�۸����
	if(m_pHoriPointStation == NULL)return;
	if( nDiv <=1 )
		return;
	
	float fRise = (float)((lMaxValue - lMinValue)*m_fVertZoomRaw);
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
//���������� lMaxValue ���ֵ
//��������   lMinValue ���ֵ
//���������� pValues   ���ݵ�����
//           lValueNum ���е�����
//��������: ��
//�����쳣: ��
void CBaseHisPic::DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, 
							   int lMaxValue, int lMinVlaue, int* pValues, int lValueNum,
							   BOOL bSelctState)
{
	if(pValues == NULL)return;
	if(m_pHoriPointStation == NULL)return;
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//��֤������Ч��
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//�ڴ�������
	if((m_nMinPrice == m_nMaxPrice)&&(m_nMaxPrice == 0))return;	
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
		m_pDC->MoveTo(m_pHoriPointStation[lCount-1]+m_nKLineWidth/2,pValues[lCount-1]);
		m_pDC->LineTo(m_pHoriPointStation[lCount]+m_nKLineWidth/2,pValues[lCount]);

		if((bSelctState)&&(m_pHoriPointStation[lCount]-nLastStation >50))
		{//����ѡ�񽹵�
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

//��������: ������״����Ϣ
//��������: rRect ��ͼ����
//          nLineWidth �߿��
//          lMaxValue ���ֵ
//          lMinValue ��Сֵ
//          pValues   �����ָ��
//          lValueNum  �������
//          crRiseTextColor     ������ɫ
//          crDownTextColor     �µ���ɫ
//          nBaseValue   ��ֵ
//��������: ��
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
	
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//��֤������Ч��
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//�ڴ�������
	CPen RiseLinePen(PS_SOLID,nLineWidth,crRiseTextColor);
	CPen DownLinePen(PS_SOLID,nLineWidth,crDownTextColor);
	int lZeroStation;//������λ��
	
	//�õ���׼��λ��
	lZeroStation = m_rectDraw.bottom - 
		           (nBaseValue - lMinVlaue)*m_rectDraw.Height()/(lMaxValue-lMinVlaue);
	
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
		m_pDC->MoveTo(m_pHoriPointStation[lCount]+m_nKLineWidth/2,lZeroStation);
		m_pDC->LineTo(m_pHoriPointStation[lCount]+m_nKLineWidth/2,pValues[lCount]);
		m_pDC->SelectObject(OldPen);
	}
}

//�������ܣ�������������
//����������clrLine��������ɫ
//�������أ���
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

//�������ܣ����û�ͼ����
//����������rRect��ͼ����
//�������أ���
void CBaseHisPic::MoveTo(CRect rRect)
{
	m_TotalRect = rRect;//������
}


//����ƶ�
BOOL CBaseHisPic::MouseMove(CPoint point)
{
	return FALSE;
}

// ����Ƴ�
void  CBaseHisPic::MouseMoveOut()
{
	return;
}

//����������
BOOL  CBaseHisPic::MouseLBtnUp(CPoint point)
{
	return FALSE;
}

//����������
BOOL CBaseHisPic::MouseLBtnDown(CPoint point)
{
	return FALSE;
}

//��������: ���Ե��Ƿ��������� 
//��������: point ������
//�������أ��ɹ�Ϊ TRUE,ʧ��ΪFALSE
//�����쳣: ��
int CBaseHisPic::HitTest(CPoint point)
{
	m_CurrentPoint = point;

	return m_rectDraw.PtInRect(point);
}

//��������: ���ú������
//��������: iHoriPoint ���������
//��������: ��
//�����쳣: ��
void CBaseHisPic::SetHoriPoint(int *pHoriPoint,int nHoriPoint)
{
	m_nHoriPoint = nHoriPoint;
	m_pHoriPointStation = pHoriPoint;
}

//�������ܣ����õ�ǰ����
//����������pFont
//�������أ���
CFont* CBaseHisPic::SetFont(CFont *pFont)
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
void CBaseHisPic::GetMaxMinPrice(long &lMinPrice, long &lMaxPrice,
								 int *pData, int nDataNum,BOOL bFlash)
{
	if(nDataNum == 0)return;//������ֱ�ӷ���
	if( pData == NULL)
		return;
	
	if(bFlash)//ˢ�������ͼ�
	{
		lMaxPrice = 0x80000000;
		lMinPrice = 0x7fffffff;
 	}
 	int nCount;
 	for(nCount= 0;nCount<nDataNum;nCount++)
 	{//ѭ���Ƚ�
		if(pData[nCount] == 0x7fffffff)
			continue;
 		if(pData[nCount]>lMaxPrice)
 			lMaxPrice = pData[nCount];
 		if(pData[nCount]<lMinPrice)
			lMinPrice = pData[nCount];
 	} 
}

//��������: �õ���ֱ�����߸���
//��������: ��ͼ����߶�
//��������: ��ֱ�߸���
int CBaseHisPic::GetVertAxisCellNum(int iHeight)
{
	return iHeight/m_nVertCellHeight;
}

//��������: ���õ�ǰ���λ��
//����������iStation��ǰ���λ��
//�������أ���
void CBaseHisPic::SetCurrentStation(int iStation)
{
	m_nCurrentStation = iStation;
}

//�������ܣ�ͨ��λ�õõ�ָ��ֵ
//����������nStation��ǰ���λ��
//�������أ�ָ��ֵ
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

//�������ܣ�ͨ��ָ��ֵ�õ�λ��
//����������nValueָ��ֵ
//�������أ��ڻ�ͼ�����е�λ��
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
