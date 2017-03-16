// TypeCtlPic.cpp: implementation of the CTypeCtlPic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "TypeCtlPic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTypeCtlPic::CTypeCtlPic()
{
	m_ArrCtlUnits.RemoveAll();//�������
	m_iMarginWidth = 4;       //��Ԫ�߾�̶�Ϊ4
	m_iCurFocus    = 9999;    //Ĭ�����ֵ

	//��������
	m_pftText = CInterfaceManager::Instance()->GetSysFont(ID_PRICEBOARD_PRICEBOARDFONT);
	//������ɫ������
	SetColorsAndFonts();
}

CTypeCtlPic::~CTypeCtlPic()
{
	m_ArrCtlUnits.RemoveAll();
}

//�������ܣ����û�ͼ����
//����������rRect ��ͼ����
//��������:��
//�����쳣:��
void CTypeCtlPic::MoveTo(CRect rRect)
{
	m_rectDraw = rRect;
}

//��������: �õ������λ��
//����������pt �������
//�������أ��ɹ��򷵻ص�ǰѡ�е�Ԫ��ţ����򷵻� -1
//�����쳣����
int CTypeCtlPic::HitTest(CPoint pt)
{
	if(!m_rectDraw.PtInRect(pt)) return -1;

	int iPos;
	bool bFinded = false;
	//ѭ������
	for(iPos = 0; iPos < m_ArrCtlUnits.GetSize() && !bFinded; iPos++)
	{
		//�ҵ�
		if(m_ArrCtlUnits.GetAt(iPos).m_rtUnitRect.PtInRect(pt))
			bFinded = true;
	}

	iPos--;
	if(bFinded) //�ҵ�
		return iPos;
	else        //û�ҵ�
		return -1;
}

//�������ܣ����õ�Ԫ������(ѡ��)/������(��ѡ��)
//����������iUnit ��Ԫ�ţ�bFocus �Ƿ񱻼���
//�������أ���
void CTypeCtlPic::SetUnitFocus(int iUnit, bool bFocus /* = false */)
{
	if(iUnit >= m_ArrCtlUnits.GetSize() || iUnit < 0) return; //��ֹ����Խ��
	STypeCtlUnit& sUnit = m_ArrCtlUnits[iUnit];
	sUnit.m_bFocus = bFocus;
}

//�������ܣ����뵥Ԫ
//����������lpszText ��Ԫ�ı���bFocus �Ƿ񱻼���
//��������: �ɹ�ΪTRUE,����ΪFALSE
BOOL CTypeCtlPic::InsertUnit(LPCTSTR lpszText,bool bFocus /* = false */)
{
	CString szUnitText;
	
	szUnitText = lpszText;
	if(szUnitText.IsEmpty()) return FALSE; //��Ԫ�ı������˳�
	//������Ԫ����
	STypeCtlUnit stuUnit;
	stuUnit.m_bFocus = bFocus;
	stuUnit.m_szUnitText = szUnitText;
	//���뵽��Ԫ����
	m_ArrCtlUnits.Add(stuUnit);
	//����ѡ��ҳǩ���
	if(bFocus)
	{
		m_iCurFocus = m_ArrCtlUnits.GetSize()-1;
	}
	return TRUE;
}


void CTypeCtlPic::ClearAllTab()
{
	m_ArrCtlUnits.RemoveAll();
}
//�������ܣ�����ͼ��
//����������pDC ��ͼ�豸ָ��
//�������أ���
BOOL CTypeCtlPic::Draw(CDC* pDC)
{
	//������Ч���˳�
	if(!CPublic::Instance()->IsRectValid(m_rectDraw))
		return FALSE;

	//����������ɫ������
	SetColorsAndFonts();

	//ˢ����ɫ
	//pDC->FillSolidRect(m_rectDraw,m_crBgColor);
	//�������Ϳ���������߿���
	CPen tabPen(PS_SOLID, 1, m_crLineColor);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&tabPen);
	pDC->MoveTo(m_rectDraw.left,m_rectDraw.top);
	pDC->LineTo(m_rectDraw.left,m_rectDraw.bottom);
	pDC->SelectObject(pOldPen);

	int iWidth = 0;     //���Ϳ��������
	bool bDraw = true;  //�Ƿ����
	//ѭ�������Ԫͼ��
	for(int i = 0; i < m_ArrCtlUnits.GetSize(); i++)
	{
		//���㵥Ԫ���δ�С
		iWidth += ComputeUnitRect(pDC,i);
		//�ж����Ϳ���������Ƿ�Խ��
		if(iWidth > m_rectDraw.Width()) bDraw = false;
		//���Ƶ�Ԫͼ��
		if(bDraw) DrawUnit(pDC,i);
	}
	return TRUE;
}

//�������ܣ����㵥Ԫ���δ�С
//����������pDC ��ͼ�豸ָ�룻iUnit ��Ԫ���
//�������أ���Ԫ���
int CTypeCtlPic::ComputeUnitRect(CDC* pDC,int iUnit)
{
	//����Խ���򷵻�0
	if(iUnit < 0 || iUnit >= m_ArrCtlUnits.GetSize()) return 0;

	//���㵥Ԫ�ı���С
	CFont* pOldFont = (CFont*) pDC->SelectObject(m_pftText);
	STypeCtlUnit& sUnit = m_ArrCtlUnits[iUnit];
	CRect& rc = sUnit.m_rtUnitRect;
	rc.SetRectEmpty();   //rc(0,0,0,0)
	pDC->DrawText(sUnit.m_szUnitText, &rc, DT_CALCRECT);
	m_sizeArror = pDC->GetTextExtent(_T("��"));//�����ͷ��С
	//ȷ����Ԫ���ε����Ͻ�����
	if(0 == iUnit) //�׵�Ԫ
	{
		CPoint pt(m_rectDraw.left,m_rectDraw.top);
		rc += pt;
	}
	else //iUnit >= 1
	{
		CPoint pt(m_ArrCtlUnits[iUnit-1].m_rtUnitRect.right
			,m_ArrCtlUnits[iUnit-1].m_rtUnitRect.top);
		rc += pt;
	}
	//�������ߵĵ�Ԫ�߾�(����չ2�����صĿ��)
	rc.right += (m_iMarginWidth*2);
	rc.bottom = m_rectDraw.bottom;
	pDC->SelectObject(pOldFont);

	//���ص�Ԫ���
	return rc.Width();
}

//�������ܣ����Ƶ�Ԫͼ��
//����������pDC ��ͼ�豸ָ�룻iUnit ��Ԫ���
//�������أ���
void CTypeCtlPic::DrawUnit(CDC* pDC,int iUnit)
{
	//����Խ�����˳�
	if(iUnit < 0 || iUnit >= m_ArrCtlUnits.GetSize()) return;

	STypeCtlUnit sUnit = m_ArrCtlUnits[iUnit];
	CRect rtUnit = sUnit.m_rtUnitRect;
	if(rtUnit.IsRectEmpty()) return; //��Ԫ���ο����˳�

	//�����豸����
	CPen tabPen(PS_SOLID, 1, m_crLineColor);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&tabPen);
	int iMode = pDC->SetBkMode(TRANSPARENT); //���ñ���ģʽΪ͸��
	
	//*************����***************
	CRgn rgn; //��Ԫ���������
    if(sUnit.m_bFocus) //��Ԫ��ѡ��/����
	{
		m_iCurFocus = iUnit; //��¼��ǰ�����Ԫ
		//��䵥Ԫ����ɫ
		if(0 == iUnit) //�׵�Ԫ
		{
			CPoint pts[5];
			pts[0] = CPoint(rtUnit.left, rtUnit.top);
			pts[1] = CPoint(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
			pts[2] = CPoint(rtUnit.left+m_iMarginWidth,rtUnit.bottom);
			pts[3] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
			pts[4] = CPoint(rtUnit.right+m_iMarginWidth, rtUnit.top);
			rgn.CreatePolygonRgn(pts, 5, WINDING); //�������������
		}
		else //iUnit >= 1
		{
			CPoint pts[4];
			pts[0] = CPoint(rtUnit.left-m_iMarginWidth, rtUnit.top);
			pts[1] = CPoint(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
			pts[2] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
			pts[3] = CPoint(rtUnit.right+m_iMarginWidth, rtUnit.top);
			rgn.CreatePolygonRgn(pts, 4, WINDING); //�������������
		}
		CBrush brush(m_crUnitSelBgColor);
		CPen newPen(PS_NULL, 1, CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR));
		CPen* oldPen = pDC->SelectObject(&newPen);
		pDC->FillRgn(&rgn, &brush);//��䵥Ԫ����
		pDC->SelectObject(oldPen);
		rgn.DeleteObject();

		//����Ԫ�߿���
		if(0 == iUnit) //�׵�Ԫ(������������Ԫ��ͬ)
		{
			pDC->MoveTo(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
			pDC->LineTo(rtUnit.left+m_iMarginWidth,rtUnit.bottom);
		}
		else //iUnit >= 1
		{
			pDC->MoveTo(rtUnit.left-m_iMarginWidth, rtUnit.top);
			pDC->LineTo(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
		}
		pDC->MoveTo(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
		pDC->LineTo(rtUnit.right,rtUnit.top+rtUnit.Height()/2);
	}
	else //δѡ��/����
	{
		//��䵥Ԫ����ɫ
		if(iUnit < m_iCurFocus) //δ���Ԫ�ڼ��Ԫ֮ǰ
		{
			if(iUnit+1 == m_iCurFocus) //���ڼ��Ԫ
			{
				CPoint pts[6];
				pts[0] = CPoint(rtUnit.left+m_iMarginWidth,rtUnit.top);
				pts[1] = CPoint(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
				pts[2] = CPoint(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
				pts[3] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
				pts[4] = CPoint(rtUnit.right,rtUnit.top+rtUnit.Height()/2);
				pts[5] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.top);
				rgn.CreatePolygonRgn(pts, 6, WINDING);
			}
			else //�����ڼ��Ԫ
			{
				CPoint pts[5];
				pts[0] = CPoint(rtUnit.left+m_iMarginWidth,rtUnit.top);
				pts[1] = CPoint(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
				pts[2] = CPoint(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
				pts[3] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
				pts[4] = CPoint(rtUnit.right+m_iMarginWidth, rtUnit.top);
				rgn.CreatePolygonRgn(pts, 5, WINDING);
			}
		}
		else //iUnit > m_iCurFocus δ���Ԫ�ڼ��Ԫ֮��
		{
			CPoint pts[5];
			pts[0] = CPoint(rtUnit.left+m_iMarginWidth,rtUnit.top);
			pts[1] = CPoint(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
			pts[2] = CPoint(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
			pts[3] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
			pts[4] = CPoint(rtUnit.right+m_iMarginWidth, rtUnit.top);
			rgn.CreatePolygonRgn(pts, 5, WINDING);
		}
		CBrush brush(m_crUnitBgColor);
		CPen newPen(PS_NULL, 1,  CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR));
		CPen* oldPen = pDC->SelectObject(&newPen);
		pDC->FillRgn(&rgn, &brush);//��䵥Ԫ����
		pDC->SelectObject(oldPen);
		rgn.DeleteObject();

		//����Ԫ�߿���
		pDC->MoveTo(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
		pDC->LineTo(rtUnit.left+m_iMarginWidth,rtUnit.top);
		
		pDC->MoveTo(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
		pDC->LineTo(rtUnit.left+m_iMarginWidth, rtUnit.bottom);

		pDC->MoveTo(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
		pDC->LineTo(rtUnit.right,rtUnit.top+rtUnit.Height()/2);

		if(iUnit < m_iCurFocus) //δ���Ԫ�ڼ��Ԫ֮ǰ
		{
			if(0 == iUnit) //�׵�Ԫ
			{
				pDC->MoveTo(rtUnit.left,rtUnit.top);
				pDC->LineTo(rtUnit.right-m_iMarginWidth,rtUnit.top);
			}
			else //iUnit >= 1
			{
				pDC->MoveTo(rtUnit.left-m_iMarginWidth,rtUnit.top);
				pDC->LineTo(rtUnit.right-m_iMarginWidth,rtUnit.top);
			}
		}
		else //iUnit > m_iCurFocus δ���Ԫ�ڼ��Ԫ֮��
		{
			pDC->MoveTo(rtUnit.left+m_iMarginWidth,rtUnit.top);
			pDC->LineTo(rtUnit.right+m_iMarginWidth,rtUnit.top);
		}
	}
	
	if(iUnit == m_ArrCtlUnits.GetSize()-1) //�������һ����Ԫ
	{
		pDC->MoveTo(rtUnit.right,rtUnit.top+rtUnit.Height()/2);
		pDC->LineTo(rtUnit.right+m_iMarginWidth, rtUnit.top);
		pDC->LineTo(m_rectDraw.right,m_rectDraw.top);
		
		pDC->MoveTo(rtUnit.right-m_iMarginWidth, rtUnit.top);
		pDC->LineTo(rtUnit.right+m_iMarginWidth, rtUnit.top);
	}

	//************����ı�**************
	CFont* pOldFont = (CFont*)pDC->SelectObject(m_pftText);
	if(sUnit.m_bFocus)
		pDC->SetTextColor(m_crTextSelColor); //��Ԫѡ��ɫ
	else
		pDC->SetTextColor(m_crTextColor); //δѡ��ɫ
	pDC->DrawText(sUnit.m_szUnitText, &rtUnit, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
	
	//�ָ��豸����
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SetBkMode(iMode);
}

//�������ܣ���Ӧ�����
//����������point ����
//�������أ�TURE �ػ���FALSE ���ػ�
BOOL CTypeCtlPic::MouseDown(CPoint point)
{
	int iSel;
	iSel = HitTest(point);
	if(-1 == iSel)
		return FALSE;
	OnSelectFocus(iSel);	
	return TRUE;
}

//�������ܣ����ص�������������еĵ�Ԫ
//����������iSel��Ӧ�ļ��Ԫ
//�������أ���
void CTypeCtlPic::OnSelectFocus(int iSel)
{
	//ûѡ�л�ѡ���������ѡ��
	if(iSel < 0)
	{
		CleanSelTab();
		return;
	}

	SetUnitFocus(m_iCurFocus,false); //ʹ��ѡ�е�Ԫδ����
	SetUnitFocus(iSel,true); //���ǰѡ�е�Ԫ
	m_iCurFocus = iSel; //���Ԫ��ֵ
}

//�������ܣ���ȡ��ǰѡ��ҳǩ���
//������������
//�������أ���ǰѡ��ҳǩ���
int CTypeCtlPic::GetSelTabID()
{
	return m_iCurFocus;
}

//�������ܣ���ȡѡ��ҳǩ�ı�
//����������point �������
//�������أ�_T("")�������˵����ǿգ�ҳǩ�ı�
CString CTypeCtlPic::GetSelTabText(CPoint point)
{
	STypeCtlUnit stuUnit;
	stuUnit = m_ArrCtlUnits[m_iCurFocus];
	CString szUnit;
	szUnit = stuUnit.m_szUnitText;
	if(szUnit.Find(_T("��"),0) < 0)
		return szUnit;
	if(point.x <= stuUnit.m_rtUnitRect.right-m_iMarginWidth-m_sizeArror.cx)
	{
		szUnit = szUnit.Left(szUnit.GetLength()-2);
		return szUnit;
	}
	else
	{
		return _T("");
	}
}

//�������ܣ����õ�ǰѡ��ҳǩ���
//����������iID ҳǩ���
//�������أ���
void CTypeCtlPic::SetSelTabID(int iID)
{
	m_iCurFocus = iID;

}

//�������ܣ����õ�ǰѡ��ҳǩ�ı�
//����������lpszText ҳǩ�ı�
//�������أ���
void CTypeCtlPic::SetSelTabText(LPCTSTR lpszText)
{
	if(m_iCurFocus < 0 || m_iCurFocus >= m_ArrCtlUnits.GetSize())
		return;
	STypeCtlUnit& stuUnit = m_ArrCtlUnits[m_iCurFocus];
	stuUnit.m_szUnitText = lpszText;
	//m_ArrCtlUnits[m_iCurFocus].m_szUnitText = lpszText;
}

//�������ܣ���ȡ���Ϳ��������Ͻ�����
//������������
//�������أ����Ͻ�����
CPoint CTypeCtlPic::GetTopLeftPt()
{
	return m_rectDraw.TopLeft();
}

//�������ܣ���ȡ���Ϳ��������Ͻ�����
//����������pDC �豸DC
//�������أ����Ͻ�����
CPoint CTypeCtlPic::GetRightTopPt(CDC* pDC)
{
	int iRec;
	int nCount;
	int nTotalWidth;

	nTotalWidth = 0;
	nCount = m_ArrCtlUnits.GetSize();
	for(iRec = 0; iRec < nCount; iRec++)
	{
		nTotalWidth += ComputeUnitRect(pDC,iRec);
	}
	nTotalWidth += m_iMarginWidth;
	
	CPoint pt;
	pt.x = m_rectDraw.left+nTotalWidth;
	pt.y = m_rectDraw.top;
	return pt;
}

//�������ܣ�������ɫ������
void CTypeCtlPic::SetColorsAndFonts()
{
	//��ȡϵͳ�趨�������ɫ
	//m_pFont = CPublic::Instance()->GetWantFont(ID_TABLE_FONT);
	m_crBgColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_crLineColor = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	m_crTextColor = CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR);
	m_crTextSelColor = CInterfaceManager::Instance()->GetSysColor(ID_HIGHLIGHT_COLOR);
	m_crUnitSelBgColor = CInterfaceManager::Instance()->GetSysColor(ID_GRIDROWSEL_COLOR);
	m_crUnitBgColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
}

//�������ܣ����ѡ��ҳǩ
void CTypeCtlPic::CleanSelTab()
{
	SetUnitFocus(m_iCurFocus,false);
	m_iCurFocus = 9999;
}