// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_BASEDRAWOBJ_409F013A0320_INCLUDED
#define _INC_BASEDRAWOBJ_409F013A0320_INCLUDED

const long	VERTBANKSIZE = 90;
const long  VERTBANKSIZE_HISOBJ = 130;
const long  HORIBANKSIZE = 18;
class CBaseDrawObj
{
public:
	int m_iType;
	void SetType(int iType = 0){m_iType = iType;}
	int GetType(){return m_iType;}
public:
	void ActivateObj(BOOL bActivate = TRUE);
	//////////////////////////////////////////////////////////
	//�����������С
	virtual void RecalSubObjSize(int iRightW = 50) = 0;

	//////////////////////////////////////////////////////////
	//���û�ͼ����
	virtual void MoveTo(CRect rRect);

	//////////////////////////////////////////////////////////
	//��������
	virtual BOOL UpdateData(LPVOID pData, int nDataSize) = 0;

	//���Ե��λ��
	virtual BOOL HitTest(CPoint point) = 0;

	//////////////////////////////////////////////////////////
	//��ͼ
	virtual BOOL Draw(CDC* pDC) = 0;

	CBaseDrawObj();

	virtual ~CBaseDrawObj();

	//�������ܣ��õ���ͼ���������
	//�������������صĶ�������
	//�������أ��ɹ�ΪTRUE,����ΪFALSE
	//�����쳣����
	BOOL GetObjName(LPCTSTR lpszName){lpszName = m_szObjName;return TRUE;};

	//�������ܣ����ö�������
	//��������: ��������
	//��������:��
	//�����쳣����
	void SetObjName(LPCTSTR lpszName){m_szObjName = lpszName;};

	//�������ܣ���Ӧ�����¼�
	//����������nChar ���̰���
	//�������أ��ޡ�
	virtual BOOL OnChar(int nChar) = 0;

	//�������ܣ������Ҽ��˵�
	//����������pWnd - ����ָ�룻point - �������
	//          uiPopMenuID - �˵�ID��iSel - �˵����
	//�������أ�ѡ�в˵�IDֵ
	virtual UINT PopupRBtnMenu(CWnd* pWnd,CPoint point,UINT uiPopMenuID,int iSel);

	// ��Ӧϵͳ�˵���������������
	virtual BOOL OnUserCmd(int nSel){return FALSE;}

	//������ϵͳ�˵������������������ʾ״̬
	virtual BOOL OnUpdateUserCmd(CCmdUI *pCmdUI){return FALSE;}


protected:
	//��ͼ����
	CRect m_DrawRect;

private:
	//��������
	CString m_szObjName;
protected:
	//�Ƿ�Ϊ��ǰ����״̬
	BOOL m_bActivate;
};

#endif /* _INC_BASEDRAWOBJ_409F013A0320_INCLUDED */
