// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_TITLETABLE_409F394502FC_INCLUDED
#define _INC_TITLETABLE_409F394502FC_INCLUDED

#include "BaseTable.h"

class CTitleTable : public CBaseTable
{
public:
	CTitleTable();

	virtual ~CTitleTable();

	//����֤ȯ��Ϣ
	void SetStockInfo(const SCode* pStockInfo);

	//�����������

	//�������ܣ����ڳ�ʼ���к��������ԣ����л�����������ش˺�
	//��
	//������������
	//��������:�ɹ�ΪTRUE,����ΪFALSE
	//�����쳣:��
	virtual BOOL Init();

	//�������ܣ��������ݣ����м̳���������ش˽ӿ�����������
	//����������lpData ��̬������
	//��������:�ɹ�ΪTRUE,����ΪFALSE
	//�����쳣:��
	virtual BOOL UpdateData(LPVOID lpData);
	//��ͼ
	void Draw(CDC* pDC);

private:
	SStockInfo m_StockInfo; //֤ȯ��Ϣ
};

#endif /* _INC_TITLETABLE_409F394502FC_INCLUDED */
