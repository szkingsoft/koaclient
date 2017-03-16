// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_INFOTABLE_409F39320241_INCLUDED
#define _INC_INFOTABLE_409F39320241_INCLUDED

#include "BaseTable.h"

//Function:������ʾֵ���
class CInfoTable : public CBaseTable
{
public:
	CInfoTable();
	virtual ~CInfoTable();

// 	enum INFOMODE
// 	{
// 		MINUTEDATAINFO,//����������Ϣ
// 		DAYDATAINFO//����������Ϣ
// 	};
	//�������ݴ���ģʽ
//	void SetMode(INFOMODE nMode){m_nInfoMode = nMode;};
	void SetMode(int nCircleInfo);
	virtual void Draw(CDC* pDC);

	//�������ܣ����ڳ�ʼ���к��������ԣ����л�����������ش˺���
	virtual BOOL Init();

	//BOOL Init(INFOMODE nInfoMode);
	BOOL Init(int nCircleInfo);

	//�������ܣ��������ݣ����м̳���������ش˽ӿ�����������
	 virtual BOOL UpdateData(LPVOID lpData,int nDataLen);
	 
	
	//����֤ȯ��Ϣ
	void SetStockInfo(const SCodeInfo* pCodeInfo);
	//�������¼۸�
	void UpdateNewPrice(float fNewPrice){m_fNewPrice=fNewPrice;};
private:
	//������������
	void UpdateDayInfo(SHistoryUnit* pKUnit,float fLastClose,float fCrossCursorPosValue, int nCircleInfo);
	//���·���������
	void UpdateMinuteInfo(SDynamicMinsUnit* pRealMinute,float fLastClose);
//	INFOMODE	m_nInfoMode; //����������ģʽ
	int m_nCircleInfo;
	int			m_nSystemStyle;

	CString m_szFormat;      //�۸��ʽ���ִ�
	SCodeInfo  m_oCodeInfo; //֤ȯ��Ϣ
	float		   m_fNewPrice; //���¼۸�

public:
	BOOL UpdateDataFromHis(LPVOID lpData,int nDataLen);    //���������������ݵ���ʾ  add by witming
};

#endif /* _INC_INFOTABLE_409F39320241_INCLUDED */
