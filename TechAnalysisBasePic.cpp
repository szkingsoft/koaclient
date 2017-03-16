// TechAnalysisBasePic.cpp: implementation of the CTechAnalysisBasePic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "TechAnalysisBasePic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//��ʼ��ָ����������
//////////////////////////////////////////////////////////////////////

CTechAnalysisBasePic::CTechAnalysisBasePic()
{
	m_pKUnit = NULL;

	m_nKLineWidth = 0;

	m_nTitleBeginStation = 0;//̧ͷ��ʼλ��
	m_nDataBegin = 0; //ˮƽ��ʼλ�õ�

	m_nCurrentSelCurves = -1;//û���κ�ѡ���ָ��

	ZeroMemory(&m_TitleNameRect,sizeof(CRect));
	ZeroMemory(&m_TitleParamRect,sizeof(CRect));
	ZeroMemory(&m_TitleRect,sizeof(CRect));
	ZeroMemory(&m_AxisRect,sizeof(CRect));

	m_dwShowStyle = 0;	
	m_nParamNum = 0;	        //ָ���������	



	ZeroMemory(&m_SelectDrawLineOrginPoint,sizeof(CPoint));//ѡ���ԭʼ��
	m_nSelectedState = NOSELECT; //ѡ�л��ߵ�״̬
	m_nSelectedDrawLine = -1;
	
	m_nDrawState = NULLDRAW;

	m_nAxisMode = PRICEMODE;//��ͨ�۸�����
	m_nVAxisCount = 5;
	m_nRecsOfUnit = 1;
	m_nMaxParamDay = 0;

}

CTechAnalysisBasePic::~CTechAnalysisBasePic()
{
}

//�������ܣ���������ָ��
//����������szTitle  ָ����ʾ��ʽ
//          nParam   ָ���������
//          nCurvers ָ����ʾ����
//�������أ���
void CTechAnalysisBasePic::Create(CString strParamTitle,int nParam,int nCurvers)
{
    UpdateIndexParamInfo(strParamTitle);
	
	ResetParaBuffers(nParam,nCurvers);
}

// ���¼���ָ���������
void CTechAnalysisBasePic::UpdateIndexParamInfo(CString strParamTitle)
{
	m_strIndexParamTitle = strParamTitle;
}


//�������ܣ���������
//����������pNewData����ָ��
//�������أ���
BOOL CTechAnalysisBasePic::UpdateData(LPVOID pNewData)
{
	m_lockUpdateData.Lock();
	CMulQuantitySequence * pSeq = (CMulQuantitySequence*)pNewData;
	m_nValueNum = pSeq->m_nSize;//������
	m_nKLineWidth = pSeq->m_lReference;//K�߿���
	m_pKUnit = (SHistoryUnit*)pSeq->m_ppValues[0];//����
	if(NULL == m_pKUnit)return FALSE;//k�ߴ���
	m_nRecsOfUnit= pSeq->m_lMin;//ͬһλ��K������
	
	m_nDecimal = pSeq->m_lMax;//CPublic::Instance()->GetTechIndexValueDecimal(m_strIndexType);

	m_nDataBegin = pSeq->m_nStart;//������ʼλ��
	ASSERT(m_nDataBegin >= 0);
 	CalculateValue();//����ָ������
	CalcMinMaxPrice();//���������ͼ�
	CalcStation();//���¼���λ��
	m_nDecimal = CPublic::Instance()->GetTechIndexValueDecimal(m_strIndexType);

	if (m_nCurrentStation < 0 || m_nCurrentStation >= m_nValueNum)
	{
		m_nCurrentStation = m_nValueNum - m_nDataBegin - 1;
	}
	m_lockUpdateData.Unlock();
	return TRUE;
}

//�������ܣ����㼼��ָ����ÿ��ֵ��ͼ�������е�������ֵ
//������������
//�������أ���
void CTechAnalysisBasePic::CalcStation()
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
				{//����ָ������ϵͳ����
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

//�������ܣ��õ�ָ���е������Сֵ
//������������
//�������أ���
void CTechAnalysisBasePic::CalcMinMaxPrice()
{
	if(NULL == m_ppCurveValue)return;//���ݴ���
	int nCount;
	for(nCount = 0;nCount<m_nCurves;nCount++)
	{
		GetMaxMinPrice(m_fMinPrice,m_fMaxPrice,m_ppCurveValue[nCount]+m_nDataBegin,m_nValueNum-m_nDataBegin,!nCount);
	}	
	//////////////////////////////////////////////////////////////////////////
	
	char* pGridInfo;
	pGridInfo = m_thCalc.GetFormulaGridInfo((char*)(LPCTSTR)m_strIndexType);
	if(pGridInfo&&strlen(pGridInfo))
	{//�й̶�����
		//���ҵ�����
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

//�������ܣ��õ�����ָ��Ĳ��������ֵ
//������������
//�������أ�����ָ��Ĳ��������ֵ
int CTechAnalysisBasePic::GetMaxParam()
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

//�������ܣ��򿪼���ָ��ע�ͶԻ���
//������������
//�������أ���
void CTechAnalysisBasePic::OpenTechNoteDlg()
{
	
}

//�������ܣ���ȡ����ָ��Ĳ���
//����������nAnalysisָ������
//�������أ���
void CTechAnalysisBasePic::ReadTechCurveParam(CString strIndexType)
{

}

//////////////////////////////////////////////////////////////////////
//�ı�ָ�껭ͼ����ָ������
//////////////////////////////////////////////////////////////////////

//�������ܣ��ƶ�����ָ��ͼ������
//����������rRect����ָ������
//�������أ���
void CTechAnalysisBasePic::MoveTo(CRect rRect)
{
	CBasePic::MoveTo(rRect);
	m_TitleRect = rRect;
	m_TitleRect.bottom = m_TitleRect.top+ TITLESIZE;
	m_AxisRect = rRect;//��������
	m_AxisRect.top = m_TitleRect.bottom;
	m_rectDraw = m_AxisRect;//��ͼ����
	m_rectDraw.left += 3;
	m_rectDraw.right -= 3;
 	//���¼���λ��
	CalcStation(); 

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
}

//�������ܣ�����ָ������
//����������
//�������أ�
void CTechAnalysisBasePic::SetTechType(CString strType)
{
	m_strIndexType = strType;
	UpdateTechNameTitle();
}

void CTechAnalysisBasePic::UpdateTechNameTitle()
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

//�������ܣ�ƽ��������
//����������fValue
//�������أ���
int CTechAnalysisBasePic::TrimDouble(double fValue)
{
	return fValue > 0 ? (long)(fValue + 0.5) : (long)(fValue - 0.5);	
}

//�������ܣ�����Ϣ������ʾ��Ϣ
//����������point��ǰ����
//�������أ���

void CTechAnalysisBasePic::DrawTip(CDC *pDC,CPoint point,CString strTip)
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
	ItemRect.bottom = ItemRect.top + textSize.cy+2;//�õ�����
	
	pDC->FillSolidRect(ItemRect,CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR));//�������
	pDC->DrawText(strTip,ItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//��������
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nMode);
}

CString CTechAnalysisBasePic::GetParamName(int nIndex)
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
//������ָ��ͼ�Ρ�ָ��ֵ��ʾ
//////////////////////////////////////////////////////////////////////

//�������ܣ���ָ����ʾֵ
//������������
//�������أ���
void CTechAnalysisBasePic::DrawTitleText()
{
	CRect ItemRect;
	ItemRect = m_TitleRect;

	CFont* pTextFont;
	pTextFont = CInterfaceManager::Instance()->GetSysFont(ID_HISTORYPICTITLEFONT);
	CFont* pOldFont = m_pDC->SelectObject(pTextFont);

	CSize TextSize = m_pDC->GetTextExtent(_T("1"));
	ItemRect.left = m_rectDraw.left + TextSize.cx*m_nTitleBeginStation;//���Ͽ�ʼλ��
	ItemRect.right = m_rectDraw.right;
	int nMode = m_pDC->SetBkMode(TRANSPARENT);
	COLORREF TextColor = CInterfaceManager::Instance()->GetSysColor(ID_FORMTITLE_COLOR);
	m_pDC->SetTextColor(TextColor); 

	m_TitleNameRect = m_TitleRect;
	m_TitleNameRect.right = m_TitleNameRect.left+ m_pDC->GetTextExtent(m_strIndexNameTitle).cx;
	CString szTemp; 
	
	m_pDC->DrawText(m_strIndexNameTitle,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	//////////////////////////////////////////////////////////////////////////
	//������ǰ������

	m_TitleParamRect = m_TitleRect;
	m_TitleParamRect.left = m_TitleNameRect.right +3;
	//m_TitleParamRect.right = m_TitleParamRect.left;  // ��ʼ�������������� 

	if(m_ppCurveValue && m_pColorValue && m_nCurrentStation > -1 && m_nDataBegin > -1  )
	{
		float fPrice;
		CString szFormat;
		szFormat.Format(_T("%%.%df"),m_nDecimal);
		CString szItem = m_strIndexNameTitle;
		for(int nCount = 0;nCount<m_nCurves;nCount++)
		{//����������

			if(m_nValueNum <= m_nCurrentStation+ m_nDataBegin)
				continue;

			// �ɽ���ָ���һά�����ǳɽ���ֵ��Ҫ��ʾ�Ĳ���������ֵ���ӵڶ�ά��ʼ
			if(( m_strIndexType.Compare(_T("VOL")) == 0)&&(nCount == m_nCurves -1))
				break;

			TextSize = m_pDC->GetTextExtent(szItem);
			ItemRect.left += (TextSize.cx+3);
			//m_TitleParamRect.right += 10;		
			
			m_pDC->SetTextColor(m_pColorValue[nCount]);//������ɫ

			int nCurIndex = nCount;
			// VOL�ĵ�һά�����ǳɽ���ֵ��������ʾ��ǰ����ֵ�ӵڶ�ά��ʼ��ƫ��1
			if( m_strIndexType.Compare(_T("VOL")) == 0)
			{
				nCurIndex = nCount+1;
			}	
			szItem = GetParamName(nCurIndex);			
			
			if(m_ppCurveValue[nCurIndex][m_nCurrentStation+m_nDataBegin] == 0x7fffffff)
				continue;
			fPrice = m_ppCurveValue[nCurIndex][m_nCurrentStation+m_nDataBegin];//�õ���ǰ�۸�
			szTemp.Format(szFormat,fPrice);
			szItem += "=";
			szItem += szTemp;
			ItemRect.right = ItemRect.left +m_pDC->GetTextExtent(szItem).cx;
			if(ItemRect.right >m_rectDraw.right)break;//��ֹԽ��
			m_pDC->DrawText(szItem,ItemRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);//��������
			
			//m_TitleParamRect.right += m_pDC->GetTextExtent(szItem).cx;   // ���²�����������
		}
	}
	m_pDC->SelectObject(pOldFont);
	m_pDC->SetBkMode(nMode);

	//////////////////////////////////////////////////////////////////////////
	//������ʾ��Ϣ
	if(m_TitleNameRect.PtInRect(m_ptNowStation))
	{
		if(( 0 != m_strIndexType.CompareNoCase(_T("MA")))&&( 0 != m_strIndexType.CompareNoCase(_T("BOLL"))))
		{
			CString szTipInfo;
			szTipInfo = CLanguageInterface::Instance()->GetStr(20030);
			DrawTip(m_pDC,m_ptNowStation,szTipInfo);
		}
		
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

//�������ܣ�������ָ��ͼ��
//����������pDC
//�������أ�BOOL
BOOL CTechAnalysisBasePic::Draw(CDC *pDC)
{
	if(!m_pHoriPoint||!m_pKUnit)return FALSE;
	m_pDC = pDC;
	COLORREF clrRise;
	clrRise = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	COLORREF clrEquire;
	clrEquire = RGB(255,50,50);//CInterfaceManager::Instance()->GetSysColor(ID_STATCOLOR);
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
		{//�й̶�����
			//���ҵ�����
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
			//�õ���������
			m_nVAxisCount = nGridNum+2;
			pStation = new float[m_nVAxisCount];
           
			//////////////////////////////////////////////////////////////////////////
			//���ù̶�����
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
			{
				if(m_nVAxisCount == 1)
				{
					pStation[i]= m_fMinPrice + ((m_fMaxPrice-m_fMinPrice)*m_fVertZoomRaw)/2;
				}
				else
				    pStation[i]= m_fMinPrice + ((m_fMaxPrice-m_fMinPrice)*m_fVertZoomRaw)/(m_nVAxisCount-1)*i;
			}
		}
		if(m_nAxisMode == PRICEMODE)
		{
			DrawAxisStyle(m_AxisRect,
				pStation,
				m_nVAxisCount,
				HISTORYAXISMODE,
				clrLine,
				clrEquire);	//��������
		}
		else if(m_nAxisMode == PRECENTMODE)
		{
			DrawPrecentAxisStyle(m_AxisRect,
				pStation,
				m_nVAxisCount,
				HISTORYAXISMODE,
				clrLine,
				clrEquire,
				m_pKUnit[m_nDataBegin].m_fClosePrice);	//��������
		}
		delete []pStation;
	}
	if(m_ppCurveValue)
	{
		int nCount;
		int nLineColorIndex = 0;
		m_strIndexType;
		for(nCount = 0;nCount<m_nCurves;nCount++)
		{
			switch(m_pLineType[nCount]) 
			{
			case LINKLINE://����
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
			case BARLINE://��״��
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
							clrDown);//�ɽ���
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
	DrawTitleText();//����̧ͷ

	if((0 == m_strIndexType.CollateNoCase(_T("SAR"))))
	{//�����BOLL�߻���SAR,���ӻ�������
		int nSubCount;
		for(nSubCount=0;nSubCount<m_nValueNum-m_nDataBegin;nSubCount++)
		{
			DrawAmercianLine(nSubCount);
		}	
	}

	//////////////////////////////////////////////////////////////////////////
	//���߲���
	int nCount;
	for(nCount= 0;nCount<m_ArrDrawLine.GetSize();nCount++)
	{
		CDrawLineTool& draw = m_ArrDrawLine[nCount];
		draw.Draw(pDC);
	}
	return TRUE;
}

void CTechAnalysisBasePic::SetShowStyle(DWORD dwStyle)
{
	m_dwShowStyle = dwStyle;
}


//��������: ��������������,����ʵʱ�����ߡ����ߡ������������
//��������:  rRect ��ͼ����
//��	     nLineWidth �߿���
//           clrLineColor �ߵ���ɫ
//���������� pValues   ���ݵ�����
//           lValueNum ���е�����
//��������: ��
//�����쳣: ��
void CTechAnalysisBasePic::DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, 
							int* pValues, int lValueNum,BOOL bSelctState)
{
	if(pValues == NULL)return;
	if(m_pHoriPoint == NULL)return;
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//��֤������Ч��
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//�ڴ�������
	if((m_fMinPrice == m_fMaxPrice)&&(m_fMaxPrice == 0))return;	
	CPen LinePen(PS_SOLID,nLineWidth,clrLineColor);
	CPen *OldPen = m_pDC->SelectObject(&LinePen);
	int nCount;
	CBrush FillBrush(clrLineColor);
	CBrush* oldBrush = m_pDC->SelectObject(&FillBrush);
	
	// ԭ��û�г�ʼ��
	int nLastStation = 0;


	int nHoriStation;
	
	CRect FillRect;
	for(nCount = 1;nCount < lValueNum;nCount++)
	{
		if(pValues[nCount]== 0x7fffffff || pValues[nCount-1]== 0x7fffffff )
			continue;	
		nHoriStation = nCount;//����
		if(nHoriStation<=0)nHoriStation = 1;

		m_pDC->MoveTo(m_pHoriPoint[nHoriStation-1]+m_nKLineWidth/2,pValues[nCount-1]);
		m_pDC->LineTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth/2,pValues[nCount]);
		
		if((bSelctState)&&(m_pHoriPoint[nHoriStation]-nLastStation >50))
		{//����ѡ�񽹵�
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

//��������: ������״����Ϣ
//��������: rRect ��ͼ����
//          nLineWidth �߿���
//          pValues   �����ָ��
//          lValueNum  �������
//          crRiseTextColor     ������ɫ
//          crDownTextColor     �µ���ɫ
//          nBaseValue   ��ֵ
//��������: ��
void CTechAnalysisBasePic::DrawBarLine(CRect rRect, int nLineWidth,
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
	int nCount;
	//////////////////////////////////////////////////////////////////////////
	//������
	int nHoriStation;
	for(nCount = 0;nCount < lValueNum;nCount++)
	{
		if(pValues[nCount]== 0x7fffffff)continue;
		nHoriStation = nCount;//������λ��

		if(pValues[nCount] < lZeroStation)
			OldPen = m_pDC->SelectObject(&RiseLinePen);
		else
			OldPen = m_pDC->SelectObject(&DownLinePen);
		m_pDC->MoveTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth/2,lZeroStation);
		m_pDC->LineTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth/2,pValues[nCount]);
		m_pDC->SelectObject(OldPen);
	}
}

void CTechAnalysisBasePic::DrawVolumnBar(CRect rRect, int nLineWidth,
										 int *pValues,int lValueNum, 
										 COLORREF clrRiseColor, COLORREF clrDownColor)
{
	if(pValues == NULL)return;
	if(m_pHoriPoint == NULL)return;
	if(lValueNum >m_nHoriPoint)lValueNum = m_nHoriPoint;//��֤������Ч��
	//if((m_fMinPrice == m_fMaxPrice)&&(m_fMaxPrice == 0))return;
	if( m_pKUnit == NULL)return;
	if( m_nDataBegin < 0)return;
	
	if(IsBadReadPtr(pValues,lValueNum*sizeof(int)))return;//�ڴ�������
	
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
				oldBrush = (CBrush*)m_pDC->SelectStockObject(NULL_BRUSH);//���÷����״̬
			}
			else
			{
				OldPen = m_pDC->SelectObject(&DownPen);
				oldBrush = m_pDC->SelectObject(&DownBrush);//�������״̬
			}
			 RectangleRect = CRect(m_pHoriPoint[nHoriStation],
									nVolBarTopValue,
									m_pHoriPoint[nHoriStation]+m_nKLineWidth,
									nVolBarBottomValue);//��ͼ����

			m_pDC->Rectangle(RectangleRect);//��������
			
			m_pDC->SelectObject(oldBrush);			
			m_pDC->SelectObject(OldPen);
		}
	}
}


//��������:��������K��
//��������:��pDC ��ͼ�豸ָ��
//			 nStation ��ǰK��λ��
//�������أ� ��
void CTechAnalysisBasePic::DrawKLine(int nStation)
{
	ASSERT( nStation >= 0);

	if( NULL == m_pHoriPoint)return;

	if(m_pKUnit == NULL)return;
	if( m_nDataBegin < 0)return;



	int nHoriStation;
	nHoriStation = nStation-m_nDataBegin;

	COLORREF clrLine;
	const SHistoryUnit* pKUnit = m_pKUnit+nStation;//����

	ASSERT( pKUnit);

	if(pKUnit->m_fOpenPrice <= pKUnit->m_fClosePrice)//����
		clrLine = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	else//�µ�
		clrLine =  CInterfaceManager::Instance()->GetSysColor(ID_KLINEDOWN_COLOR);
	CPen LinePen(PS_SOLID,1,clrLine);
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
		m_pDC->FillSolidRect(FillRect,clrLine);
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

void CTechAnalysisBasePic::DrawAmercianLine(int nStation)
{
	ASSERT( nStation >= 0);
	
	if( NULL == m_pHoriPoint)return;
	
	if(m_pKUnit == NULL)return;
	if( m_nDataBegin < 0)return;
	
	COLORREF clrLine;
	const SHistoryUnit* pKUnit = m_pKUnit+nStation+m_nDataBegin;//����
	
	ASSERT( pKUnit);

	int nHoriStation;
	
	nHoriStation = nStation;
	
	if(pKUnit->m_fOpenPrice <= pKUnit->m_fClosePrice)//����
		clrLine = CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR);
	else//�µ�
		clrLine =  CInterfaceManager::Instance()->GetSysColor(ID_KLINEDOWN_COLOR);
	CPen LinePen(PS_SOLID,1,clrLine);
	CPen* oldPen = m_pDC->SelectObject(&LinePen);
	
	int nOpenStation,nCloseStation;//��������
	nOpenStation = GetStationFromPrice(pKUnit->m_fOpenPrice);//���̼�λ��
	nCloseStation = GetStationFromPrice(pKUnit->m_fClosePrice);//���̼�λ��
	
	int nHighStation,nLowStation;//�����ͼ�
	nHighStation = GetStationFromPrice(pKUnit->m_fMaxPrice);
	nLowStation = GetStationFromPrice(pKUnit->m_fMinPrice);
	
	int nMiddle;//�����м�����
	nMiddle = m_pHoriPoint[nHoriStation] + m_nKLineWidth/2;
	m_pDC->MoveTo(nMiddle,nLowStation);
	m_pDC->LineTo(nMiddle,nHighStation);//�м�����
	
	m_pDC->MoveTo(nMiddle,nOpenStation);
	m_pDC->LineTo(m_pHoriPoint[nHoriStation],nOpenStation);//���̼�
	
	m_pDC->MoveTo(nMiddle,nCloseStation);
	m_pDC->LineTo(m_pHoriPoint[nHoriStation]+m_nKLineWidth,nCloseStation);//���̼�
	
	
	m_pDC->SelectObject(oldPen);
}

void CTechAnalysisBasePic::GetMaxMinValue(float &nMaxValue, float &nMinValue)
{
	nMaxValue = m_fMaxPrice;
	nMinValue = m_fMinPrice;
}

void CTechAnalysisBasePic::SetMaxMinValue(float nMaxValue, float nMinValue)
{
	m_fMinPrice = nMinValue;
	m_fMaxPrice = nMaxValue;
}

void CTechAnalysisBasePic::SetTitleBeginStation(int nStation)
{
	m_nTitleBeginStation = nStation;
}

int CTechAnalysisBasePic::MouseDbClitTitle(CPoint pt)
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

const CRect& CTechAnalysisBasePic::GetTitleParamRect()
{
	return m_TitleParamRect;
}

void CTechAnalysisBasePic::SetRect(CRect rRect, int nRectType)
{//���ھ���
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
		CalcStation();//���¼���λ��
		break;
	case 3:
		m_TotalRect = rRect;
	default:
		break;
	}
}

void CTechAnalysisBasePic::ResetData()
{
	m_pKUnit = NULL;
	m_pHoriPoint = NULL;
	m_nValueNum = 0;
	m_nDataBegin = 0;
	m_fMaxPrice = 0.0000001f;
	m_fMinPrice = 10000000.0f;
}

int CTechAnalysisBasePic::GetParamNum()
{
	return m_nParamNum;
}

BOOL CTechAnalysisBasePic::ResetParaBuffers(int nParam, int nCurvers)
{
	//ɾ������
	if (m_pLineType != NULL)
	{
		delete []m_pLineType;
		m_pLineType = NULL;
	}
	
	//ɾ������
	if (m_pParamValue != NULL)
	{
		delete []m_pParamValue;
		m_pParamValue = NULL;
	}
	
	//ɾ����ɫ
	if (m_pColorValue != NULL)
	{
		delete []m_pColorValue;
		m_pColorValue = NULL;
	}

	m_nCurves = nCurvers;
	m_nParamNum = nParam;//��������
		
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

void CTechAnalysisBasePic::GetDrawRect(CRect& rRect)
{
	rRect = m_rectDraw;
}

BOOL CTechAnalysisBasePic::GetDateFromPoint(const CPoint& point,int& nDay,float& fPrice)
{
// 	int nStation;
// 	int nDiffent;
// 	nDiffent = 0;
// 
// 	nStation = (point.x - m_rectDraw.left)*(m_nValueNum-m_nDataBegin)/m_rectDraw.Width();
// 	if(nStation>=0)
// 	{
// 		if(nStation+m_nDataBegin+nDiffent<m_nValueNum)
// 			nDay= m_pKUnit[nStation+m_nDataBegin+nDiffent].m_uiDate;
// 		else
// 			nDay= m_pKUnit[m_nValueNum-1].m_uiDate + (m_nValueNum-nStation-m_nDataBegin-nDiffent);
// 	}
// 	else
// 	{
// 		nDiffent = -nStation;
// 	}
// 	fPrice = m_fMinPrice +(m_rectDraw.bottom - point.y)*((m_fMaxPrice- m_fMinPrice)/m_rectDraw.Height());

	nDay = GetDateFromStation(point.x);
	fPrice = GetPriceFromStation(point.y);
	return TRUE;
}

BOOL CTechAnalysisBasePic::GetPointFromDate(int nDay,float fPrice,CPoint& point)
{
	point.y = GetStationFromPrice(fPrice);//
// 	//////////////////////////////////////////////////////////////////////////
// 	//����ʱ��
// 	for(int j=m_nDataBegin;j<m_nValueNum;j++)
// 	{
// 		if(nDay <= m_pKUnit[j].m_uiDate)
// 		{
// 			point.x = m_pHoriPoint[j-m_nDataBegin]+m_nKLineWidth/2;
// 			break;
// 		}
// 	}
// 	if(nDay > m_pKUnit[m_nValueNum-1].m_uiDate)
// 	{
// 		point.x = m_pHoriPoint[m_nValueNum-m_nDataBegin-1]+m_nKLineWidth/2 +(nDay- m_pKUnit[m_nValueNum-1].m_uiDate)*(m_pHoriPoint[1]-m_pHoriPoint[0]);
// 	}
	point.x = GetStationFromDate(nDay);
	return TRUE;
}
BOOL CTechAnalysisBasePic::MouseDown(CPoint point)
{
	if(!m_rectDraw.PtInRect(point))
	{
		m_nCurrentSelCurves = -1;
		return FALSE;
	}
	return FALSE;
}
BOOL CTechAnalysisBasePic::MouseUp(CPoint point)
{
	if(!m_rectDraw.PtInRect(point))return FALSE;

	//�����Ƿ�Ϊ����״̬

	return FALSE;
}
BOOL CTechAnalysisBasePic::MouseMove(CPoint point)
{
	//////////////////////////////////////////////////////////////////////////
	//�õ���ǰλ��
	m_ptNowStation = point;
	if(m_TitleRect.PtInRect(point))
		return TRUE;
	return FALSE;
}
void CTechAnalysisBasePic::SetDrawLineType(int nLineType)
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

BOOL CTechAnalysisBasePic::DeleteCurrentDrawLine()
{
	//////////////////////////////////////////////////////////////////////////
	//�����Ƿ�Ϊ����״̬
	if(m_nSelectedDrawLine == -1)return FALSE;
	if(m_ArrDrawLine.GetSize() == 0)return FALSE;
	m_ArrDrawLine.RemoveAt(m_nSelectedDrawLine);//ɾ����ǰ����
	m_nSelectedDrawLine = -1;
	return TRUE;
}
BOOL CTechAnalysisBasePic::MouseRBDown(CPoint point)
{
	//�Ƚϵ�ǰ��
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
		{//�ҵ�
			m_nCurrentSelCurves = nCount;
			return TRUE;
		}
	}
	return FALSE;
}

void CTechAnalysisBasePic::RemoveAllDrawLine()
{
	m_ArrDrawLine.RemoveAll();
	m_nSelectedDrawLine = -1;
}


//////////////////////////////////////////////////////////////////////////
int CTechAnalysisBasePic::GetDateFromStation(int nXStation)
{
	int nStation;
	int nDiffent;
	nDiffent = 0;
	int nDate;
	nStation = (nXStation- m_rectDraw.left)*(m_nValueNum-m_nDataBegin)/m_rectDraw.Width();
	if(nStation>=0)
	{
		if(nStation+m_nDataBegin+nDiffent<m_nValueNum)
			nDate= m_pKUnit[nStation+m_nDataBegin+nDiffent].m_uiDate;
		else
			nDate= m_pKUnit[m_nValueNum-1].m_uiDate + (m_nValueNum-nStation-m_nDataBegin-nDiffent);
		return nDate;
	}
	nDiffent = -nStation;
	return -1;
}

int CTechAnalysisBasePic::GetStationFromDate(int nDate)
{
	int nXstation = 0;
	if(nDate<m_pKUnit[m_nDataBegin].m_uiDate)return -1;
	if(nDate > m_pKUnit[m_nValueNum-1].m_uiDate)return -1;
// 	{
// 		nXstation = m_pHoriPoint[m_nValueNum-m_nDataBegin-1]+m_nKLineWidth/2 +(nDate- m_pKUnit[m_nValueNum-1].m_uiDate)*(m_pHoriPoint[1]-m_pHoriPoint[0]);
// 	}
	//////////////////////////////////////////////////////////////////////////
	//����ʱ��
	for(int j=m_nDataBegin;j<m_nValueNum;j++)
	{
		if(nDate <= m_pKUnit[j].m_uiDate)
		{
			nXstation = m_pHoriPoint[j-m_nDataBegin]+m_nKLineWidth/2;
			break;
		}
	}

	return nXstation;
}
double CTechAnalysisBasePic::GetKlineWidth()
{
	double dbWidth;
	dbWidth = m_rectDraw.Width();
	dbWidth /= m_nHoriPoint;
	return dbWidth;
}

BOOL CTechAnalysisBasePic::GetHighLowFromTime(const int & time,float &dHigh,float &dLow)
{
	if(time <m_pKUnit[m_nDataBegin].m_uiDate)return FALSE;//С�ڿ�ʼʱ��
	for(int j=m_nDataBegin;j<m_nValueNum;j++)
	{
		if(time <= m_pKUnit[j].m_uiDate)
		{
			dHigh = m_pKUnit[j].m_fMaxPrice;
			dLow = m_pKUnit[j].m_fMinPrice;
			return TRUE;
		}
	}
	return FALSE;
}

void CTechAnalysisBasePic::GetCloseFromXSgmt( const int &uBeginX,const int &uEndX, CArray< CPoint,CPoint&> &arPtClose )
{
	arPtClose.RemoveAll();
	if(uEndX <m_pKUnit[m_nDataBegin].m_uiDate)return ;//С�ڿ�ʼʱ��
	CPoint ptClose;
	for(int j=m_nDataBegin;j<m_nValueNum;j++)
	{
		if((uBeginX<= m_pKUnit[j].m_uiDate)&&(uEndX>= m_pKUnit[j].m_uiDate))
		{
			GetPointFromDate(m_pKUnit[j].m_uiDate,m_pKUnit[j].m_fClosePrice,ptClose);
			arPtClose.Add(ptClose);
		}
		if(uEndX<m_pKUnit[j].m_uiDate)
		{
			break;
		}
	}
}
BOOL CTechAnalysisBasePic::GetHighLowFromXSgmt( const UINT uBeginX, const UINT uEndX, CPoint &ptHigh, CPoint &ptLow )
{
	if(uEndX <m_pKUnit[m_nDataBegin].m_uiDate)return FALSE;//С�ڿ�ʼʱ��
	BOOL bFrist= TRUE;
	int nHighDate,nLowdate;
	float fHighPrice,fLowPrice;
	for(int j=m_nDataBegin;j<m_nValueNum;j++)
	{
		if(uBeginX>m_pKUnit[j].m_uiDate)continue;
		if(uEndX<m_pKUnit[j].m_uiDate)break;
		if(bFrist)
		{
			nHighDate = nLowdate = m_pKUnit[m_nDataBegin].m_uiDate;
			fHighPrice = m_pKUnit[m_nDataBegin].m_fMaxPrice;
			fLowPrice = m_pKUnit[m_nDataBegin].m_fMinPrice;
			bFrist = FALSE;
			continue;
		}
		if(fHighPrice<m_pKUnit[j].m_fMaxPrice)
		{
			fHighPrice = m_pKUnit[j].m_fMaxPrice;
			nHighDate = m_pKUnit[j].m_uiDate;
		}
		if(fLowPrice >m_pKUnit[j].m_fMinPrice)
		{
			fLowPrice = m_pKUnit[j].m_fMinPrice;
			nLowdate = m_pKUnit[j].m_uiDate;
		}
	}
	GetPointFromDate(nHighDate,fHighPrice,ptHigh);
	GetPointFromDate(nLowdate,fLowPrice,ptLow);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////