// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PRICEVOLOBJ_409F013A0330_INCLUDED
#define _INC_PRICEVOLOBJ_409F013A0330_INCLUDED

#include "BaseDrawObj.h"
#include "PricePic.h"
#include "InfoTable.h"

class CSinglockGuard
{
public:
	CSinglockGuard(CCriticalSection* pSinglock)
	{
		if(NULL == pSinglock)return;
		m_pSinglock = pSinglock;
//		m_pSinglock->Lock();
	}
	~CSinglockGuard()
	{
		if(NULL == m_pSinglock)return;
//		m_pSinglock->Unlock();
	}
private:
	CCriticalSection* m_pSinglock;
};

static CArray<SCodeInfo,SCodeInfo&> m_CodeInfoArray;
class CPriceVolObj: public CBaseDrawObj
{
public:
	BOOL  TransStation(CPoint point);

	CPriceVolObj();
	virtual ~CPriceVolObj();
 	//////////////////////////////////////////////////////////////////////////
  	//�������ܣ����·���������
	virtual void RecalSubObjSize(int iRightW = 50);
	//�������ܣ����û�ͼģʽ��nShowMode����ͼ����
	void SetDisplayMode(DWORD nShowMode);
	//�������ܣ����û�ͼģʽ
	BOOL SetRealTechType(int nTechType);
	int GetRealTechType();
 	//�������ܣ���ָ���豸��ͼ
 	virtual BOOL Draw(CDC* pDC);    
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ�������ֻ�е�ǰ��Ʊ����������
	void RequestData();
	void RequestData_SP();
	//�������ܣ�����֤ȯ��Ϣ
 	void SetStockInfo(const SCodeInfo* pCodeInfo);
	//�������ܣ������������
 	virtual BOOL UpdateData(LPVOID pData,int dwDataSize = 0);			
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ���Ӧ�����¼�
 	virtual BOOL OnChar(int nChar);
	//�������ܣ���Ӧ�û�ID��Ϣ
	BOOL OnUserCmd(UINT iSel);
	//�������ܣ�����ƶ���Ϣ
	//�������ܣ����������λ��
	virtual BOOL HitTest(CPoint point);
	
	BOOL MouseLBtnDbClick(CPoint point);
	//�������ܣ���Ӧ����ƶ���Ϣ
	BOOL MouseMove(CPoint point);
	//�������ܣ���Ӧ������������Ϣ
	BOOL MouseLBtnUp(CPoint point);  
	//�������ܣ���Ӧ������������Ϣ
	BOOL MouseLBtnDown(CPoint point);
	//�������ܣ���Ӧ����Ҽ������Ϣ
	BOOL MouseRBtnDown(LPVOID lpWnd,CPoint point);

private:
	//����ʾ��Ϣ
	void DrawTip(CDC *pDC,CPoint point,CString strTip);
	
	//����̧ͷ��Ϣ
	void DrawTitleInfo(CDC* pDC);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ����ݽ���ʱ��������ͼԪ�������ڴ�
	void SetHoriPoint();
	//�Ƿ������SCodeInfo
	BOOL IsExistCodeInfo(SCodeInfo codeInfo);	
 	//�������ܣ���ʱ��̶�
	void DrawTimeRect(CDC* pDC); 	

	void DrawCrossCursor(CDC *pDC);
	//////////////////////////////////////////////////////////////////////////
	// ����������������������������Ч
	void AdjustMinsData2Valide();
 	//�������ܣ����³ɽ��ۡ��ɽ���ͼԪ����
	void UpdatePicData();	
	//�������ܣ�����ͼԪ��ʾ��Ϣ������
	void UpdateInfoTip();
	
	
	int  GetHoriPointTime(int nPos);

	int  GetHoriPoint(int nTime);

private:
	//////////////////////////////////////////////////////////////////////////
	CRect m_PriceRect;           //�ɽ�������
	CRect m_VolumnRect;			 //�ɽ�������
	CRect m_TechRect;			 //ָ������
	CRect m_TimeRect;			 //ʱ����������
	CRect m_TipInfoRect;		 //��Ϣ������
	CRect m_TitleRect;			 //̧ͷ����
	BOOL  m_bMouseMove;			 //����ƶ�
	BOOL  m_bLeftButtonDown;	 //����������
	CPoint m_DrapPoint;		 	 //�϶���	
	//////////////////////////////////////////////////////////////////////////
	long* m_pPriceValues;        //�ɽ���ֵ
	//////////////////////////////////////////////////////////////////////////
 	int   m_nHoriPoints;		 //�ܽ���ʱ��
	int   m_nValueNum;			 //��Ч��������
 	int   m_nRealTechType;		 //��ǰ����ָ�껭ͼ���� 
 	int   m_nShowMode;			 //��ʾģʽ
	int   m_nCurrentStation;	 //��ǰ���λ��
	int   m_nCurrentPrice;//��ǰ���۸�λ��
private:
	//////////////////////////////////////////////////////////////////////////
	CPricePic    m_PricePic;	       //�ɽ���ͼԪ	
	CInfoTable   m_InfoTable;          //��Ϣ���
	const SCodeInfo*  m_pCodeInfo;   //֤ȯ��Ϣ
//	float m_fPrevClose;        //��ǰ��Ʒ�����ռ�
	int m_iDate;                       //��������
	BOOL		 m_bDrawCross;		   //�������

	 CCriticalSection		m_lockData;
	list<STraceUnit>	m_listTraceInfo;//�ɽ���Ϣ
};

#endif /* _INC_PRICEVOLOBJ_409F013A0330_INCLUDED */
