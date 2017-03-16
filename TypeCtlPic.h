// TypeCtlPic.h: interface for the CTypeCtlPic class.
#if !defined(AFX_TYPECTLPIC_H__ED4F4C79_F077_4E7D_AE39_50A83A45E868__INCLUDED_)
#define AFX_TYPECTLPIC_H__ED4F4C79_F077_4E7D_AE39_50A83A45E868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////
//������    ���Ϳ���������
//////////////////////////////////////////
struct STypeCtlUnit //���Ϳ�������Ԫ
{
	CString m_szUnitText;   //��Ԫ�ı�
	CRect   m_rtUnitRect;   //��Ԫ����
	//int     m_iMarginWidth; //��Ԫ�߾�
	bool    m_bFocus;       //�Ƿ񱻼���

public:
	STypeCtlUnit()
	{
		m_szUnitText.Empty();
		m_rtUnitRect.SetRectEmpty();
		//m_iMarginWidth = 0;
		m_bFocus = false;
	}
};
typedef CArray<STypeCtlUnit,STypeCtlUnit&> CTypeControl;

class CTypeCtlPic
{
public:
	CTypeCtlPic();
	virtual ~CTypeCtlPic();

	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����û�ͼ����
	//����������rRect ��ͼ����
	//��������: ��
	//�����쳣: ��
	virtual void MoveTo(CRect rRect);
	//////////////////////////////////////////////////////////////////////////
	//��������: �õ������λ��
	//����������pt �������
	//�������أ��ɹ��򷵻ص�ǰѡ�е�Ԫ��ţ����򷵻� -1
	//�����쳣����
	virtual int HitTest(CPoint pt);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����õ�Ԫ������(ѡ��)/������(��ѡ��)
	//����������iUnit ��Ԫ�ţ�bFocus �Ƿ񱻼���
	//�������أ���
	virtual void SetUnitFocus(int iUnit, bool bFocus = false);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����ص�������������еĵ�Ԫ
	//����������iSel��Ӧ�ļ��Ԫ
	//�������أ���
	void OnSelectFocus(int iSel);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����뵥Ԫ
	//����������lpszText ��Ԫ�ı���bFocus �Ƿ񱻼���
	//��������: �ɹ�ΪTRUE,����ΪFALSE
	virtual BOOL InsertUnit(LPCTSTR lpszText,bool bFocus = false);
	//�������ܣ�����ͼ��
	//����������pDC ��ͼ�豸ָ��
	//�������أ���
	virtual BOOL Draw(CDC* pDC);
	//�������ܣ����㵥Ԫ���δ�С
	//����������pDC ��ͼ�豸ָ�룻iUnit ��Ԫ���
	//�������أ���Ԫ���
	virtual int ComputeUnitRect(CDC* pDC,int iUnit);
	//�������ܣ����Ƶ�Ԫͼ��
	//����������pDC ��ͼ�豸ָ�룻iUnit ��Ԫ���
	//�������أ���
	virtual void DrawUnit(CDC* pDC,int iUnit);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����Ϳ�������ʼ��
	//������������
	//��������: �ɹ�ΪTRUE,����ΪFALSE
	virtual BOOL Init(){return TRUE;};
	//�������ܣ���Ӧ�����
	//����������point ����
	//�������أ�TURE �ػ���FALSE ���ػ�
	virtual BOOL MouseDown(CPoint point);
	//�������ܣ���ȡ��ǰѡ��ҳǩ���
	//������������
	//�������أ���ǰѡ��ҳǩ���
	virtual int GetSelTabID();
	//�������ܣ����õ�ǰѡ��ҳǩ���
	//����������iID ҳǩ���
	//�������أ���
	virtual void SetSelTabID(int iID);
	//�������ܣ���ȡ���Ϳ��������Ͻ�����
	//������������
	//�������أ����Ͻ�����
	virtual CPoint GetTopLeftPt();
	//�������ܣ���ȡ���Ϳ��������Ͻ�����
	//����������pDC �豸DC
	//�������أ����Ͻ�����
	virtual CPoint GetRightTopPt(CDC* pDC);
	//�������ܣ�������ɫ������
	virtual void SetColorsAndFonts();
	//�������ܣ����õ�ǰѡ��ҳǩ�ı�
	virtual void SetSelTabText(LPCTSTR lpszText);
	//�������ܣ���ȡѡ��ҳǩ�ı�
	virtual CString GetSelTabText(CPoint point);
	//�������ܣ����ѡ��ҳǩ
	virtual void CleanSelTab();

	const CRect& GetRect(){return m_rectDraw;}
	
	void ClearAllTab();
	CTypeControl m_ArrCtlUnits; //���Ϳ�������Ԫ����
private:
	CRect        m_rectDraw;    //��Ч��ͼ����
	int          m_iMarginWidth;//��Ԫ�߾�
	int          m_iCurFocus;   //��ǰ�����Ԫ

	CFont*    m_pftText;          //�ı�����
	COLORREF m_crLineColor;     //�߿�����ɫ
	COLORREF m_crTextColor;     //�ı�ɫ(δѡ��)
	COLORREF m_crTextSelColor;  //�ı�ɫ(ѡ��)
	COLORREF m_crBgColor;       //���屳��ɫ
	COLORREF m_crUnitSelBgColor;//��Ԫ����ɫ(ѡ��)
	COLORREF m_crUnitBgColor;   //��Ԫ����ɫ(δѡ��)
private:
	CSize    m_sizeArror;       //��ͷ��С
};

#endif // !defined(AFX_TYPECTLPIC_H__ED4F4C79_F077_4E7D_AE39_50A83A45E868__INCLUDED_)
