// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_BASETABLE_409F38C900AB_INCLUDED
#define _INC_BASETABLE_409F38C900AB_INCLUDED

//Function:ͼ�������
#pragma pack(1)

struct SSubItem
{
	enum SUBITEMLINE
	{//����
		TOP_LINE = 0x1000,
		BOTTOM_LINE= 0x2000,
		LEFT_LINE = 0x4000,
		RIGHT_LINE= 0x8000
	};
	SSubItem()
	{
		dwStyle = 0;
		crTextColor = -1;
		crTextBkColor = -1;
		dwItemImage = -1;
		pFont = NULL;
		dwExStyle = -1;
	}
	CString szItemText;//��Ԫ����Ϣ
	DWORD   dwItemImage;//��Ԫ��ͼ��(ͼ���б���ţ�-1��ʾ��ͼ��)
	COLORREF crTextColor;//��Ԫ������ɫ(-1��ʾΪ��ϵͳĬ����ɫ)
	COLORREF crTextBkColor;//��Ԫ�񱳾�ɫ(-1��ʾΪϵͳĬ����ɫ)
	DWORD   dwStyle;//����
	DWORD	dwExStyle;//��չ����
	CFont*	pFont;//��������
};
typedef CArray<SSubItem,SSubItem&> CLineInfo;
typedef CArray<CLineInfo,CLineInfo&> CItemArray;

struct SColumnHead
{
	CString  szColumnText;//ͷ������Ϣ
	DWORD dwItemImage;//��Ԫ��ͼ��
	DWORD dwFormat;//��ʾ��ʽ
	DWORD dwColumnWide;//���	
};
typedef CArray<SColumnHead,SColumnHead&> CColumnArray;
#pragma pack()

class CBaseTable 
{
public:	

	void SetHoriBeginLine(int nBegin);
	// ��ָ֤��������ʾ����,�����µ���ʼλ��
	int EnsureColVisible(int nColIndex);
	int GetColumnNum();
	int GetFixedColumn();//�õ��̶�������
	int GetCurrentHorBeignLine();//�õ���ǰˮƽ��ʼ��
	int GetColumnWidth(int nCol);//�õ�ָ���п��
	BOOL DeleteAll();//�����������
	int GetItemHeight(){return (int)m_nRowHeight;};//�õ�һ�и߶�
	int	GetCurrentSelLine(){return m_nCurrentSel;};//�õ���ǰѡ����
	int	GetCurrentBeginLine(){return m_nCurrentBeginLine;};//�õ���ǰ��ʼ��
	virtual void SetCurrentSelLine(int nSel){m_nCurrentSel = nSel;};//���õ�ǰѡ����
	int GetDisplayableLineNum();//�õ�����ʾ������
	virtual BOOL OnKeyDownEvent(UINT nChar);//������Ӧ	
	
	CBaseTable();
	virtual ~CBaseTable();

	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��������µ�Ԫ��(Cell)����
	//��������: nItem �к�
	//          nSubItem �к�
	//          subItem ��Ԫ��(Cell)����
	BOOL InsertItem(int nItem,int nSubItem,SSubItem subItem);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�ɾ��һ��
	//����������nItem �к�
	//�������أ��ɹ�ΪTRUE,����ΪFALSE
	virtual BOOL DeleteItem(int nItem);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�����ͼ��
	//����������pDC ��ͼ�豸ָ��
	//�������أ���
	//�����쳣����
	virtual void Draw(CDC* pDC);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�ȷ��ָ���пɼ�
	//����������nItem Ҫ��ʾ��������
	//�������أ��ɹ�ΪTRUE,����ΪFALSE
	//�����쳣����
	virtual BOOL EnsureRowVisible(int nItem);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��õ���ǰ�ı������
	//������������
	//��������: �������
	//�����쳣: ��
	virtual int GetItemCount();
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��õ���Ԫ������
	//��������:nItem �к�
	//         nSubItem �к�
	//         lpszText ���ص�����
	//��������:�ɹ�ΪTRUE,����ΪFALSE
	virtual CString GetItemText(int nItem, int nSubItem);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��õ���ǰѡ�����к�
	//������������
	//�������أ���ǰѡ���к�
	//�����쳣����
	virtual int GetSelItem(){return m_nCurrentSel;};
	//////////////////////////////////////////////////////////////////////////
	//��������:�õ������λ��(��)
	//����������pt �������
	//�������أ��ɹ��򷵻ص�ǰѡ���кţ����򷵻� -1
	//�����쳣����
	virtual int HitTest(CPoint pt);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����ڳ�ʼ���к��������ԣ����л�����������ش˺�
	//��
	//������������
	//��������:�ɹ�ΪTRUE,����ΪFALSE
	//�����쳣:��
	virtual BOOL Init() = 0;
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�����һ�У����ڳ�ʼ���б�
	//����������nCol ��ǰҪ�����λ��
	//����������lpszColumnHead���б�ͷ������
	//����������nFormat    �����ֵ���ʾλ��
	//����������nWidth     �еĿ��
	//�������أ��ɹ����ص�ǰ�е�λ�ã�����Ϊ-1
	virtual int InsertColumn(int nCol, LPCTSTR lpszColumnHead, int nFormat = LVCFMT_LEFT, int nWidth = -1);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��޸ĵ�Ԫ������
	//��������: nItem   ��Ԫ��
	//����������nSubItem ��Ԫ��
	//����������lpszText ��Ԫ����
	//          nImage   ��Ԫ����ʾͼ��
	//          crText   ��Ԫ������ɫ
	//�������أ��ɹ�ΪTRUE,����ΪFALSE
	//�����쳣����
	virtual BOOL InsertItem(int nItem, int nSubItem, LPCTSTR lpszText, int nImage = -1,
		                    COLORREF crText = -1,COLORREF crTextBkColor = -1,DWORD dwStyle = 0);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����ñ�񱳾�ɫ
	//��������: cr ���õı���ɫ
	//��������:��
	//�����쳣:��
	virtual void SetBkColor(COLORREF cr){m_crBkColor = cr;};
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����û�ͼ����
	//����������rRect ��ͼ����
	//��������:��
	//�����쳣:��
	virtual void MoveTo(CRect rRect);	

	void GetRect(CRect& rrectTable);
	
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�������ʾ����
	//����������pFont ������
	//�������أ�������(��ǰ����)
	virtual CFont* SetFont(CFont* pFont);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����ñ����ʾ��ͼ���б�
	//��������: ���õ�ͼ���б��ָ��
	//�������أ�ǰһ��ͼ���б��ָ��
	//�����쳣��GetLastError�õ����Ĵ���
	virtual CImageList* SetImageList(CImageList* pImageList);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�����ÿ��Ԫ��ĸ߶�(�и�)
	//����������nHeight ��Ԫ��߶�(�и�)
	//��������:��
	//�����쳣:��
	virtual void SetItemHeight(int nHeight){m_nRowHeight = (float)nHeight;};
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�������ʾ���
	//������������ʾ���
	//��������:��
	//�����쳣����
	virtual void SetStyle(DWORD dwStyle){m_dwStyle |= dwStyle;};
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�������ʾ��չ���
	//������������ʾ���
	//��������:��
	//�����쳣����
	virtual void SetExtendStyle(DWORD dwExStyle){m_dwExStyle |= dwExStyle;};
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�����ͨ������ɫ
	//����������cr ������ɫ
	//�������أ���
	virtual void SetTextColor(COLORREF cr){m_crTextColor = cr;};
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ���������
	//����������pData ��ǰ��������е�����
	//����������dwDataNum���ݳ���
	//��������:0Ϊʧ�ܣ�����Ϊ�ɹ�
	virtual int SortItem(LPVOID pData, DWORD dwDataNum);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��������ݣ����м̳���������ش˽ӿ�����������
	//����������lpData ��̬������
	//��������:�ɹ�ΪTRUE,����ΪFALSE
	//�����쳣:��
	virtual BOOL UpdateData(LPVOID lpData,int nDataLen) = 0;
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����������߲���
	//����������crLine ����ɫ
	//          nWidth �߿��
	virtual void SetLineInfo(COLORREF crLine,int nWidth = 1){m_crLineColor = crLine;m_nLineWidth = nWidth;};
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����ù̶���ʾ������
	//��������: nFixedLine �̶�������
	void SetFixedColumn(int nFixedLine)
	{if((nFixedLine>=0)&&(nFixedLine<m_ArrColumn.GetSize()))
		m_nFixedColumn = nFixedLine;};
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��õ�����ܿ��
	int GetTotalWidth();
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��õ����ǰ���
	int GetCurWidth();
	//�������ܣ������Ҽ��˵�
	virtual UINT PopupRBtnMenu(CWnd* pWnd,CPoint point,UINT uiPopMenuID,int iSel);
	//������ɫ������
	virtual void SetColorsAndFonts();

protected:

	void SetSortParam(int nSortColIndex,BOOL bDescOrder);	

	// ��ȡ���һҳ���е�λ������ 
	int GetLastPageBeginLine( int nTotalRowCount);
	
	int GetCurPageRowCount(int nCurBeginIndex,int nTotalRowCount);

	void SetDrawColumnLine(BOOL bDraw);

	void SetSortDirect(int nSortColumn,BOOL bDirect);

	void SetSeledRowFillSolidRect(BOOL bFill,COLORREF crFillColor = -1);

protected:
	COLORREF m_crBkColor;//ϵͳ����ɫ

	COLORREF m_crSelectColor;//ѡ��ɫ
	
	COLORREF m_crTextColor;//����ɫ

	CItemArray m_ArrItemInfo;//������Ϣ
	
	CColumnArray  m_ArrColumn;//Column��Ϣ

	COLORREF m_crTextBkColor;//���ֱ���ɫ

	CFont* m_pFont; //��������

	DWORD m_dwStyle; //��ʾ��� ��ȫ����CListCtrl����

	DWORD m_dwExStyle;//��չ��ʾ���� ��ȫ����CListCtrl��չ����

	CRect m_DrawRect;	//��ͼ����

	int m_nCurrentSel;//��ǰѡ����

	float m_nRowHeight; //��Ԫ��߶�(�и�)

	int m_nCurrentBeginLine;	//��ǰ��ʼ��

	int m_nCurrentShowLines;	//��ǰ��Ļ����ʾ��

	CImageList* m_pImageList;	//��Ԫ��ʹ��ͼ���б�

	COLORREF m_crLineColor;	//��������ɫ

	int      m_nLineWidth;	//�����߿��

	CDWordArray	m_ArrdwItemOrder;//��Ԫλ��	
	
	BOOL m_bDescSort;    //�Ƿ�Ӵ�С������
	int m_nSortColIndex; //��ǰ������
protected:
	int		m_nFixedColumn;//���̶���
	int		m_nHoriBeiginLine;//ˮƽ��ʼλ��
private:
	BOOL m_bActivate;	//�Ƿ�Ϊ��ǰ����״̬
	BOOL m_bDrawColumnLine;//������̧ͷ��
	BOOL m_bSeledRowFillSolidRect; // ѡ�����Ƿ�ʹ��ʵ�ľ�����������ʾ
};

#endif /* _INC_BASETABLE_409F38C900AB_INCLUDED */
