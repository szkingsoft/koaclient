// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PRICEPIC_409F013A02A5_INCLUDED
#define _INC_PRICEPIC_409F013A02A5_INCLUDED

#include "DynBasePic.h"

const int NOTITLEINFO     = 0X0001;//����̧ͷ
const int NORIGHTRAWRULER = 0x0002;//�����Ҳ������
const int NOTIMERULER     = 0x0004;//�����²�ʱ���
const int TILELASTPRICE   = 0x0008;//̧ͷ��ʾ���һ��
const int TITLEINDEXLAST  = 0x0010;//ָ��̧ͷ
const int TITLELARGEFONT  = 0x0020;//̧ͷ������
const int TITLEPRICEDAY   = 0x0080;//��������


class CPricePic: public CDynBasePic
{
public:	
	CPricePic();
	virtual ~CPricePic();
	//////////////////////////////////////////////////////////////////////////
	//���û�ͼģʽ
	void SetDisplayMode(int nShowMode){m_nShowMode = nShowMode;};
	//////////////////////////////////////////////////////////////////////////
	//���¼���λ��
	virtual void RegetStation();
	//////////////////////////////////////////////////////////////////////////
	//���û�ͼ����
	virtual void MoveTo(CRect rRect,BOOL bAuto = TRUE);
	//////////////////////////////////////////////////////////////////////////
	//��������
	virtual BOOL UpdateData(LPVOID pNewData);
	//////////////////////////////////////////////////////////////////////////
	//��ͼ
	virtual BOOL Draw(CDC* pDC);
	//////////////////////////////////////////////////////////////////////////
	//�õ���ǰ�۸��λ��
	virtual int GetStationFromPrice(long lPrice);

	void SetTradeTime(int nTradeTime){m_nTradeTime = nTradeTime;};
private:


	// Ϊ�˿̶���ʾ���������������Сֵ
	void JustMaxMinValueForDispaly(long &lMaxValue,long &lMinValue,
		                           long lRefMidValue,int nVAxisCount,
								   long lUpLimitValue,long lDownLimitValue);

	//////////////////////////////////////////////////////////////////////////
	//��ʽ��������ֵ
	void GetFormatMaxMin(long& lMaxPrice, long& lMinPrice, long lRefPos,int nVAxisCount);

	// ����������ڴ�����
	void ClearData();

	//���ý���ʱ��

	
private:	
	int m_nShowMode;//��ͼģʽ

	long m_lReference;//���ռ�

	long* m_pPricePoints;//�۸�ֱλ��ָ��

	long * m_pPrice;//�۸�ָ��

	CRect m_OrignRect;//ԭʼ����

	int	  m_nTradeTime;
};

#endif /* _INC_PRICEPIC_409F013A02A5_INCLUDED */
