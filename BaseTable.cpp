// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "BaseTable.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBaseTable::CBaseTable()
{
	//m_crBkColor = GetSysColor(COLOR_WINDOW);
	//m_crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	//m_crTextBkColor = GetSysColor(COLOR_WINDOW);
	//m_crSelectColor = GetSysColor(COLOR_HIGHLIGHT);
	m_nCurrentSel = 0;//��ǰѡ��
	m_nCurrentBeginLine = 0;//��ʼ����
	m_nRowHeight = 18;//Ĭ�ϸ߶�
	m_nSortColIndex = -1;
	m_bDescSort = TRUE; 
	m_pFont = NULL;
	m_pImageList = NULL;
	m_dwStyle = 0;
	m_dwExStyle = 0;
	m_bActivate = TRUE;
	//m_crLineColor = GetSysColor(COLOR_WINDOWTEXT);
	m_nLineWidth = 1;

	m_nFixedColumn = 0;
	m_nHoriBeiginLine = 0;
	m_ArrdwItemOrder.SetSize(0,20);

	m_bDrawColumnLine = TRUE;

	m_bSeledRowFillSolidRect = TRUE;

	//����������ɫ������

	ZeroMemory(&m_DrawRect,sizeof(CRect));
}

CBaseTable::~CBaseTable()
{

}
//�������ܣ�ɾ����Ԫ
//����������nItem ��Ԫ����
//�������أ��ɹ�ΪTRUE,����ΪFALSE
BOOL CBaseTable::DeleteItem(int nItem)
{
	if(nItem >= m_ArrItemInfo.GetSize())
		return FALSE;//�޴���
	m_ArrItemInfo.RemoveAt(nItem);
	return TRUE;
}

//�������ܣ�����ͼ��
//����������pDC ��ͼ�豸ָ��
//�������أ���
//�����쳣����
void CBaseTable::Draw(CDC* pDC)
{
	SetColorsAndFonts();
    if( m_nCurrentBeginLine < 0)
		m_nCurrentBeginLine = 0;

	//////////////////////////////////////////////////////////////////////////
	//����
	//��䱳������
	pDC->FillSolidRect(m_DrawRect,m_crBkColor);//��䱳��ɫ
	//////////////////////////////////////////////////////////////////////////
	CFont* oldFont = NULL;
	if(m_pFont)
		oldFont = pDC->SelectObject(m_pFont);
	int iOldDrawMode;
	iOldDrawMode = pDC->SetBkMode(TRANSPARENT);

	CPen DvidePen(PS_SOLID,m_nLineWidth,m_crLineColor);
	CPen * oldPen = pDC->SelectObject(&DvidePen);
	//////////////////////////////////////////////////////////////////////////
	int nCount,nSubCount;//������
	CRect ItemRect;//��Ԫ������
	
	int nBeginHeight;
	nBeginHeight = 0;//��ʼ�߶�
	int nLeftSize;//���ߴ�
	pDC->SetTextColor(m_crTextColor);//����������ɫ
	int nDrawFormat;
	int iStation;//��ǰ��Ԫ��
	if(!(m_dwStyle&LVS_NOCOLUMNHEADER))
	{//��ͷ��Ϣ	
		ItemRect.top = m_DrawRect.top;
		ItemRect.bottom = m_DrawRect.top+ m_nRowHeight;//ͷ��λ��
		nBeginHeight = m_nRowHeight;//�߶�
		nLeftSize = 0;

		for(nCount = 0;nCount<m_ArrColumn.GetSize();nCount++)
		{
			if(nCount >= m_nFixedColumn)
				iStation = nCount+m_nHoriBeiginLine;//�����Ҳ�����
			else
				iStation = nCount;
			if(iStation >= m_ArrColumn.GetSize())break;
			iStation = m_ArrdwItemOrder[iStation];//�任��Ӧλ��
			SColumnHead& column = m_ArrColumn[iStation];
			ItemRect.left = m_DrawRect.left+nLeftSize;
			ItemRect.right = ItemRect.left+ column.dwColumnWide;
			ItemRect.left +=2;
			ItemRect.right -= 2;
			if(ItemRect.left >m_DrawRect.right)break;//����
			nDrawFormat = DT_SINGLELINE|DT_VCENTER;
			if(column.dwFormat == LVCFMT_LEFT)
				nDrawFormat |= DT_LEFT;
			else if(column.dwFormat == LVCFMT_CENTER)
				nDrawFormat |= DT_CENTER;
			else
				nDrawFormat |= DT_RIGHT;
			pDC->DrawText(column.szColumnText,ItemRect,nDrawFormat);
			if(m_pImageList&&(m_nSortColIndex == iStation))
			{//��ͼ�����������
				CPoint BasePoint(ItemRect.right-4,ItemRect.top+5);
				m_pImageList->Draw(pDC,m_bDescSort,BasePoint,ILD_NORMAL);
			}
			if(m_bDrawColumnLine)
			{
				ItemRect.left -=2;
				ItemRect.right += 2;
				pDC->MoveTo(ItemRect.right,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.bottom-1);
				pDC->LineTo(ItemRect.left,ItemRect.bottom-1);
				pDC->LineTo(ItemRect.left,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.top);
			}
			nLeftSize  += column.dwColumnWide;//���
		}
	}
	CPoint ImagePoint;
	CFont* pOldSubFont;//�ӵ�Ԫ�ڲ�����
	for(nCount = 0;nCount<(m_ArrItemInfo.GetSize()-m_nCurrentBeginLine);nCount++)
	{//���ڲ���Ԫ��
		ItemRect.top = m_DrawRect.top + nBeginHeight+(nCount/2)*m_nRowHeight;//�߶�
		ItemRect.bottom = ItemRect.top+ m_nRowHeight;//�߶ȡ�
		if(ItemRect.bottom> m_DrawRect.bottom)
			break;//������ǰ����ʾ����
		nLeftSize = 0;	
		if((m_dwExStyle&LVS_EX_FULLROWSELECT)&&(nCount == m_nCurrentSel))
		{//��ǰѡ��
			ItemRect.left = m_DrawRect.left;
			ItemRect.right = m_DrawRect.right;	
			ItemRect.top ++;
	
			if( !m_bSeledRowFillSolidRect)
			{
				// �Ե�ǰѡ�����������ʾ
				CPen pen(PS_SOLID,1,m_crSelectColor);
				CPen* pOldPen = pDC->SelectObject(&pen);
				pDC->MoveTo(ItemRect.left,ItemRect.bottom);
				pDC->LineTo(ItemRect.right,ItemRect.bottom);
				
				pDC->MoveTo(ItemRect.left,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.top);
				
				pDC->SelectObject(pOldPen);
			}
			else
			{
				pDC->FillSolidRect(ItemRect,m_crSelectColor);
			}

		}
		CLineInfo & LineInfo = m_ArrItemInfo[m_nCurrentBeginLine+nCount];
		for(nSubCount= 0;nSubCount< LineInfo.GetSize();nSubCount++)
		{
			if(nSubCount >= m_nFixedColumn)
				iStation = nSubCount+m_nHoriBeiginLine;//�����Ҳ�����
			else
				iStation = nSubCount;

			if(iStation >= m_ArrColumn.GetSize())break;
			iStation = m_ArrdwItemOrder[iStation];//�任��Ӧλ��			
			SSubItem& subItem = LineInfo[iStation];
			SColumnHead& column = m_ArrColumn[iStation];
			ItemRect.left = m_DrawRect.left+nLeftSize;
			ItemRect.right = ItemRect.left+ column.dwColumnWide;
			ItemRect.left +=0;
			ItemRect.right -= 5;
			if(ItemRect.left >m_DrawRect.right)break;//����
			//////////////////////////////////////////////////////////////////////////
			if(subItem.crTextBkColor != -1)//��������ɫ
			{
				CRect ClientRect;
				ClientRect = ItemRect;
				ClientRect.right --;
				ClientRect.left ++;
				ClientRect.top++;
				ClientRect.bottom--;
				pDC->FillSolidRect(ClientRect,subItem.crTextBkColor);//���ñ���ɫ
			}
			if(subItem.crTextColor != -1)
				pDC->SetTextColor(subItem.crTextColor);//��������ɫ
			else
				pDC->SetTextColor(m_crTextColor);//ƽ������ɫ
			//////////////////////////////////////////////////////////////////////////
			nDrawFormat = DT_SINGLELINE|DT_VCENTER;
			if(subItem.dwExStyle == -1)
			{//���ñ�׼��ҵ����
				if(column.dwFormat == LVCFMT_LEFT)
					nDrawFormat |= DT_LEFT;
				else if(column.dwFormat== LVCFMT_CENTER)
					nDrawFormat |= DT_CENTER;
				else
					nDrawFormat |= DT_RIGHT;
			}
			else
			{//���ø��Ի�����
				if(subItem.dwExStyle& LVCFMT_CENTER)
					nDrawFormat |= DT_CENTER;
				else if(subItem.dwExStyle&LVCFMT_RIGHT)
					nDrawFormat |= DT_RIGHT;
				else nDrawFormat |= DT_LEFT;
			}

			CRect TextRect;
			TextRect = ItemRect;
			//////////////////////////////////////////////////////////////////////////
			if((m_pImageList)&&(subItem.dwItemImage != -1))
			{//��ǩ
				ImagePoint = CPoint(ItemRect.left+2, ItemRect.top+5);
				m_pImageList->Draw(pDC,subItem.dwItemImage,ImagePoint,ILD_NORMAL);//�����
			}
			TextRect.left +=5;
			if(subItem.pFont)
			{//�и��Ի�����
				pOldSubFont = pDC->SelectObject(subItem.pFont);//ѡ����Ի�����
				pDC->DrawText(subItem.szItemText,TextRect,nDrawFormat);//������Ϣ
				pDC->SelectObject(pOldSubFont);//�ָ�����
			}
			else
			{//����Ĭ������
				pDC->DrawText(subItem.szItemText,TextRect,nDrawFormat);//������Ϣ				
			}

// 			if((m_pImageList)&&(subItem.dwItemImage != -1))
// 			{//��ǩ
// 				ImagePoint = CPoint(ItemRect.right+2, ItemRect.top+5);
// 				m_pImageList->Draw(pDC,subItem.dwItemImage,ImagePoint,ILD_NORMAL);//�����
// 			}
			ItemRect.left -=2;
			ItemRect.right += 2;

			if(subItem.dwStyle&SSubItem::TOP_LINE)
			{//��Ԫ���ϲ໭��
				pDC->MoveTo(ItemRect.left,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.top); 
			}
			if(subItem.dwStyle&SSubItem::LEFT_LINE)
			{//��Ԫ����໭��
				pDC->MoveTo(ItemRect.left,ItemRect.top);
				pDC->LineTo(ItemRect.left,ItemRect.bottom);
			}
			if(subItem.dwStyle&SSubItem::RIGHT_LINE)
			{//��Ԫ���Ҳ໭��
				pDC->MoveTo(ItemRect.right,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.bottom);
			}
			if(subItem.dwStyle&SSubItem::BOTTOM_LINE)
			{//��Ԫ���²໭��
				pDC->MoveTo(ItemRect.left,ItemRect.bottom);
				pDC->LineTo(ItemRect.right,ItemRect.bottom);
			}
			if(m_dwExStyle&LVS_EX_GRIDLINES)
			{//�ָ���
				pDC->MoveTo(ItemRect.right,ItemRect.top);
				pDC->LineTo(ItemRect.right,ItemRect.bottom-1);
				pDC->LineTo(ItemRect.left,ItemRect.bottom-1);
			}
			nLeftSize  += column.dwColumnWide;//���
		}
	}
	if(oldFont)
	{
		pDC->SelectObject(oldFont);
	}

	pDC->SelectObject(oldPen);

	pDC->SetBkMode(iOldDrawMode);
}

//�������ܣ�ȷ��ָ���пɼ�
//����������nItem Ҫ��ʾ��������
//�������أ��ɹ�ΪTRUE,����ΪFALSE
//�����쳣����
BOOL CBaseTable::EnsureRowVisible(int nItem)
{
	if((nItem < 0)||(nItem >=  m_ArrItemInfo.GetSize()))
		return FALSE;//�޴���

	int nDisplayableLineCount = GetDisplayableLineNum();
	if(nItem  < m_nCurrentBeginLine)
	{//Ҫ��ʾ����������
		while( nItem < m_nCurrentBeginLine)
		{
			m_nCurrentBeginLine --;//= nDisplayableLineCount;//���Ϸ���
		}
	}
	else if(nItem >= m_nCurrentBeginLine + nDisplayableLineCount)
	{//Ҫ��ʾ����������

		while(nItem >= m_nCurrentBeginLine+nDisplayableLineCount)
		{
			m_nCurrentBeginLine ++;//= nDisplayableLineCount;//���·���һ��
		}
	}
	return TRUE;
}

void CBaseTable::SetSortParam(int nSortColIndex,BOOL bDescOrder)
{
	m_nSortColIndex = nSortColIndex;
	m_bDescSort = bDescOrder;
}

//�������ܣ��õ���ǰ�ĵ�Ԫ����
//������������
//��������:��Ԫ����
//�����쳣:��
int CBaseTable::GetItemCount()
{
	return m_ArrItemInfo.GetSize();
}

//�������ܣ��õ���Ԫ������
//��������:nItem ��
//         nSubItem ��
//         lpszText ���ص�����
//��������:�ɹ�ΪTRUE,����ΪFALSE
CString CBaseTable::GetItemText(int nItem, int nSubItem)
{
	if(m_ArrItemInfo.GetSize()<= nItem)return _T("");
	CLineInfo & line = m_ArrItemInfo[nItem];
	SSubItem& subItem = line[nSubItem];
	return subItem.szItemText;
}

//��������:�õ������λ��
//����������pt �������
//�������أ��ɹ���ǰѡ���У����򷵻� -1
//�����쳣����
int CBaseTable::HitTest(CPoint pt)
{
	if(!m_DrawRect.PtInRect(pt))return -1;
	int iLine;
	iLine = (pt.y -m_DrawRect.top)/m_nRowHeight;
	if(m_dwStyle!=LVS_NOCOLUMNHEADER)
	{
		iLine--;//��ȥͷ
	}
	if(iLine >= m_ArrItemInfo.GetSize())return -1;

	return iLine;
}

//�������ܣ�����һ�У����ڳ�ʼ���б�
//����������nCol ��ǰҪ�����λ��
//����������lpszColumnHead���б�ͷ������
//����������nFormat    �����ֵ���ʾλ��
//����������nWidth     �еĿ��
//�������أ��ɹ����ص�ǰ�е�λ�ã�����Ϊ-1

int CBaseTable::InsertColumn(int nCol, LPCTSTR lpszColumnHead, int nFormat, int nWidth)
{
	SColumnHead columnhead;
	columnhead.szColumnText = lpszColumnHead;
	columnhead.dwFormat = nFormat;
	columnhead.dwItemImage = -1;
	columnhead.dwColumnWide = nWidth;
	m_ArrColumn.InsertAt(nCol,columnhead);	
	m_ArrdwItemOrder.Add(nCol);
	return (int)0;
}

//�������ܣ��޸ĵ�Ԫ������
//��������: nItem   ��Ԫ��
//����������nSubItem ��Ԫ��
//����������lpszText ��Ԫ����
//          nImage   ��Ԫ����ʾͼ��
//          crText   ��Ԫ������ɫ
//�������أ��ɹ�ΪTRUE,����ΪFALSE
//�����쳣����
BOOL CBaseTable::InsertItem(int nItem, int nSubItem, LPCTSTR lpszText, int nImage,
							COLORREF crText,COLORREF crTextBkColor,DWORD dwStyle )
{
	SSubItem subItem;
	subItem.szItemText = lpszText;
	subItem.crTextBkColor = crTextBkColor;
	subItem.crTextColor = crText;
	subItem.dwItemImage = nImage;
	subItem.dwStyle = dwStyle;
	if(nItem >= m_ArrItemInfo.GetSize())
	{//����
		m_ArrItemInfo.SetSize(nItem+1);
	}
	CLineInfo& Line = m_ArrItemInfo[nItem];//��
	if(nSubItem >= m_ArrColumn.GetSize())
	{//����������
		return FALSE;
	}
	if(Line.GetSize() < m_ArrColumn.GetSize())
	{
		Line.SetSize(m_ArrColumn.GetSize());
	}
	Line.SetAt(nSubItem,subItem);//��	
	return TRUE;
}

CFont* CBaseTable::SetFont(CFont* pFont)
{
	//System
	CFont* oldFont = NULL;
	oldFont = m_pFont;
	m_pFont = pFont;
	return oldFont;
}

//�������ܣ����ñ����ʾ��ͼ���б�
//��������: ���õ�ͼ���б��ָ��
//�������أ�ǰһ��ͼ���б��ָ��
//�����쳣��GetLastError�õ����Ĵ���
CImageList* CBaseTable::SetImageList(CImageList* pImageList)
{
	CImageList* oldImageList;
	oldImageList = m_pImageList;
	m_pImageList = pImageList;
	return oldImageList;
}


//�������ܣ���������
//����������pData ��ǰ��������е�����
//����������dwDataNum���ݳ���
//��������:0Ϊʧ�ܣ�����Ϊ�ɹ�
int CBaseTable::SortItem(LPVOID pData, DWORD dwDataNum)
{
	return (int)0;
}
//�������ܣ����û�ͼ����
//����������rRect ��ͼ����
//��������:��
//�����쳣:��
void CBaseTable::MoveTo(CRect rRect)
{
	m_DrawRect = rRect;
	m_nCurrentShowLines = m_DrawRect.Height()/m_nRowHeight;//��ǰ����ʾ����	
};

void CBaseTable::GetRect(CRect& rrectTable)
{
	rrectTable = m_DrawRect;
}

BOOL CBaseTable::InsertItem(int nItem,int nSubItem,SSubItem subItem)
{
	if(nItem >= m_ArrItemInfo.GetSize())
	{//����
		m_ArrItemInfo.SetSize(nItem+1);
	}
	CLineInfo& Line = m_ArrItemInfo[nItem];//��
	if(nSubItem >= m_ArrColumn.GetSize())
	{//����������
		return FALSE;
	}
	if(Line.GetSize() < m_ArrColumn.GetSize())
	{
		Line.SetSize(m_ArrColumn.GetSize());
	}
	Line.SetAt(nSubItem,subItem);//��	
	return TRUE;
}

BOOL CBaseTable::OnKeyDownEvent(UINT nChar)
{
	switch(nChar)
	{
	case VK_LEFT:
		{
			if(m_nHoriBeiginLine >0)
				m_nHoriBeiginLine --;
			else
				m_nHoriBeiginLine = 0;
		}
		break;
	case VK_RIGHT:
		{
			if(m_nHoriBeiginLine < m_ArrColumn.GetSize()-m_nFixedColumn-1)
				m_nHoriBeiginLine++;
			else
				m_nHoriBeiginLine = m_ArrColumn.GetSize()-m_nFixedColumn-1;
		}
		break;
	case VK_UP:
		{
			m_nCurrentSel --;
			if(m_nCurrentSel < 0)
			{
				int nLine = m_nCurrentSel+m_nCurrentBeginLine;
				if(nLine<0)nLine = 0;//m_ArrItemInfo.GetSize()-1;
				EnsureRowVisible(nLine);
				m_nCurrentSel = nLine-m_nCurrentBeginLine;
			}
		}
		break;
	case VK_DOWN:
		{
			m_nCurrentSel ++;
			
			int nCurPageRowCount = GetDisplayableLineNum();
			int nNewBeginLine = m_nCurrentSel+m_nCurrentBeginLine;
			if( m_nCurrentBeginLine >= GetLastPageBeginLine(m_ArrItemInfo.GetSize()))
			{
				nCurPageRowCount = m_ArrItemInfo.GetSize() - (GetLastPageBeginLine(m_ArrItemInfo.GetSize()));
				nNewBeginLine = 0;
			}
			
			if(m_nCurrentSel > nCurPageRowCount-1)
			{
				EnsureRowVisible(nNewBeginLine);
				m_nCurrentSel = nNewBeginLine-m_nCurrentBeginLine;
				if(m_nCurrentSel>=nCurPageRowCount)
				{
					m_nCurrentSel = nCurPageRowCount-1;
				}
			}			
		}
		break;
	case VK_NEXT:
		{
			int nDisplayableLineCount = GetDisplayableLineNum(); 
			m_nCurrentBeginLine += nDisplayableLineCount;
			int nTotalItemCount = m_ArrItemInfo.GetSize();
			if(m_nCurrentBeginLine > nTotalItemCount)
				m_nCurrentBeginLine = 0;
			if(m_nCurrentBeginLine > nTotalItemCount - nDisplayableLineCount)
			{
				m_nCurrentBeginLine = nTotalItemCount-nDisplayableLineCount;//GetLastPageBeginLine(nTotalItemCount);
			}
		}
		break;
	case VK_PRIOR:
		{
			m_nCurrentBeginLine -= GetDisplayableLineNum();
			if(m_nCurrentBeginLine<0)
			{
				m_nCurrentBeginLine = 0;//GetLastPageBeginLine(m_ArrItemInfo.GetSize());
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}

int CBaseTable::GetDisplayableLineNum()
{
	if(m_dwStyle == LVS_NOCOLUMNHEADER)
		return m_nCurrentShowLines;
	return m_nCurrentShowLines-1;
}

int CBaseTable::GetLastPageBeginLine( int nTotalRowCount)
{
	// ��֤��һҳ��ʾ��0��ʼ	
	int nDisplayableLineCount = GetDisplayableLineNum();
	
	int nLastPageCount = nTotalRowCount % nDisplayableLineCount;
	if( nLastPageCount == 0)
		nLastPageCount = nDisplayableLineCount;
	
	return (nTotalRowCount - nLastPageCount);			
}

int CBaseTable::GetCurPageRowCount(int nCurBeginIndex,int nTotalRowCount)
{
	ASSERT(nCurBeginIndex <= nTotalRowCount);

	if( nTotalRowCount <= 0)
		return 0;
	
	int nDistance = nTotalRowCount - nCurBeginIndex;
	int nDisplayableLineCount = GetDisplayableLineNum();

	if( nDistance < GetDisplayableLineNum())
	{
		return nDistance;
	}
	else
	{
		return nDisplayableLineCount;
	}
}

BOOL CBaseTable::DeleteAll()
{
	m_ArrItemInfo.RemoveAll();
	return TRUE;
}

//�������ܣ��õ�����ܿ��
//������������
//�������أ�����ܿ��
int CBaseTable::GetTotalWidth()
{
	int iRec;
	int nTotalWidth;

	nTotalWidth = 0;
	for(iRec = 0; iRec < m_ArrColumn.GetSize(); iRec++)
	{
		nTotalWidth += m_ArrColumn[iRec].dwColumnWide;
	}
	return nTotalWidth;
}

//�������ܣ��õ����ǰ���
//������������
//�������أ���ǰ���
int CBaseTable::GetCurWidth()
{
/*	int iRec;
	int nCurWidth;

	nCurWidth = 0;
	bool bOver = false;
	for(iRec = 0; iRec < m_ArrColumn.GetSize() && !bOver; iRec++)
	{
		nCurWidth += m_ArrColumn[iRec].dwColumnWide;
		if(nCurWidth > m_DrawRect.Width())
			bOver = true;
	}
	if(bOver) --iRec;*/
	
	return m_DrawRect.Width();
}

int CBaseTable::GetColumnWidth(int nCol)
{
	if(nCol >= m_ArrColumn.GetSize())return 0;
	return m_ArrColumn[nCol].dwColumnWide;
}

int CBaseTable::GetCurrentHorBeignLine()
{
	return m_nHoriBeiginLine;
}

int CBaseTable::GetFixedColumn()
{
	return m_nFixedColumn;
}

int CBaseTable::GetColumnNum()
{
	return m_ArrColumn.GetSize();
}

void CBaseTable::SetHoriBeginLine(int nBegin)
{
	if(nBegin> 0)
	{
		//////////////////////////////////////////////////////////////////////////
		int nCount;
		for(nCount= m_nFixedColumn;nCount<m_ArrColumn.GetSize();nCount++)
		{
			nBegin -= GetColumnWidth(nCount);
			if(nBegin<0)break;
		}
		m_nHoriBeiginLine = nCount-1;
	}
	else
		m_nHoriBeiginLine = 0;
	//////////////////////////////////////////////////////////////////////////
	if(m_nHoriBeiginLine<0)
		m_nHoriBeiginLine = 0;
	if(m_nHoriBeiginLine >m_ArrColumn.GetSize() - m_nFixedColumn)
		m_nHoriBeiginLine = m_ArrColumn.GetSize() -m_nFixedColumn-1;
}

// ��ָ֤��������ʾ�����Ŀ�ʼ������
int CBaseTable::EnsureColVisible(int nColIndex)
{
	// ��ȡָ���е�ʵ������˳��
	int nColOrder;
	for( nColOrder=0; nColOrder<m_ArrColumn.GetSize(); nColOrder++)
	{
		if( m_ArrdwItemOrder[nColOrder] == nColIndex)
			break;
	}	

	if( (nColOrder <= 0) 
		|| ( nColOrder > m_ArrColumn.GetSize()))
	{
		return 0;
	}

	// ��ȥ�̶���֮��Ŀ��
	int nActiveWidth = GetCurWidth();
	int i;
	for(i = 0; i<m_nFixedColumn; i++)
	{
		nActiveWidth -= GetColumnWidth(i);
	}
	
	int nBeforeColWidthSum = 0;
	for(i= nColOrder; i>=m_nFixedColumn; i--)
	{
		nBeforeColWidthSum += GetColumnWidth(i);
		if(nBeforeColWidthSum >= nActiveWidth )
			break;
	}
	m_nHoriBeiginLine = i - m_nFixedColumn + 1;
    
	// �����µ���ʼ�е�����λ��
	nBeforeColWidthSum = 0;
	for( i = 0; i<m_nHoriBeiginLine + m_nFixedColumn; i++)
	{
		nBeforeColWidthSum += GetColumnWidth(i);
	}

	return nBeforeColWidthSum;
}

//�������ܣ������Ҽ��˵�
//����������pWnd - ����ָ�룻point - �������
//          uiPopMenuID - �˵�ID��iSel - �˵����
//�������أ�ѡ�в˵�IDֵ
UINT CBaseTable::PopupRBtnMenu(CWnd* pWnd,CPoint point,UINT uiPopMenuID,int iSel)
{
	CPoint scrnPoint;
	CMenu popMenu;
	UINT uiItemID;

	if(NULL == pWnd) return -1; //��ָֹ���

	scrnPoint = point;
	pWnd->ClientToScreen(&scrnPoint);
	popMenu.LoadMenu(uiPopMenuID); //װ�ز˵���Դ
	CLanguageInterface::Instance()->InitRightMenu(&popMenu,iSel);
	CMenu* subMenu;
	subMenu = popMenu.GetSubMenu(iSel);
	
	//�����˵�
	uiItemID = subMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN
		,scrnPoint.x,scrnPoint.y,AfxGetMainWnd()); //�����˵�
	popMenu.DestroyMenu(); //���ٲ˵�
	return uiItemID;
}

//������ɫ������
void CBaseTable::SetColorsAndFonts()
{
	m_crBkColor = RGB(0,0,0);//CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_crSelectColor = CInterfaceManager::Instance()->GetSysColor(ID_GRIDROWSEL_COLOR);
	m_crTextColor = RGB(255,255,255);//CInterfaceManager::Instance()->GetSysColor(ID_GRIDFIXED_COLOR);
	m_crTextBkColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_crLineColor = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	m_pFont = CInterfaceManager::Instance()->GetSysFont(ID_TABLE_FONT);
}

void CBaseTable::SetSortDirect(int nSortColumn, BOOL bDirect)
{
	m_nSortColIndex = nSortColumn;
	m_bDescSort = bDirect;
}

void CBaseTable::SetDrawColumnLine(BOOL bDraw)
{
	m_bDrawColumnLine = bDraw;
}

void CBaseTable::SetSeledRowFillSolidRect(BOOL bFill,COLORREF crFillColor)
{
	m_bSeledRowFillSolidRect = bFill;
	if( crFillColor != -1)
	{
		m_crSelectColor = crFillColor;
	}
}