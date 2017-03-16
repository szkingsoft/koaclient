// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_DYNBASEPIC_409F013A0251_INCLUDED
#define _INC_DYNBASEPIC_409F013A0251_INCLUDED

const int AUTOFITLEFTSIZE = 0x0040;//�Զ���Ӧ��߾�
class CDynBasePic 
{
public:
	enum AXISMODE
	{
		DOUBLESIDEDOUBLECOLOR,//˫��˫ɫ
		DOUBLESIDESINGLECOLOR,//˫�ߵ�ɫ
		SINGLESIDEDOUBLECOLOR,//����˫ɫ 
		SINGLESIDESINGLECOLOR,//���ߵ�ɫ
		HISTORYAXISMODE		  //��ʷ��������
	};//���껭ͼģʽ
	CDynBasePic();
	virtual ~CDynBasePic();
	
	//��������:���õ�ǰ���λ��
	void SetCurrentStation(int iStation);
	

	//��������:�õ���ǰ���λ��
	virtual int GetCurrentStation(CPoint point);

	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 2013/09/17
	//����ʮ���߼۸���
	void SetCurrentPrice(long lPrice);

	//////////////////////////////////////////////////////////////////////////
	
	// ��ȡ�ƶ�λ�õ�x����
	int GetHoriPointXByStation(int iStation);

	// ��ȡ��Ԫ�еĿ��
	float GetHoriPointWidth();

	//��������:���¼������λ��
	virtual void RegetStation(){};

	//��������: ������״����Ϣ
	void DrawBarLine(CRect rRect, 
					int nLineWidth, 
					long lMaxValue,
					long lMinVlaue, 
					long *pValues, 
					long lValueNum,
					COLORREF crRiseTextColor,
					COLORREF crDownTextColor,
					int nBaseValue = 0);

	//��������: �õ���ֱ�����߸���
	int GetVertAxisCellNum(int iHeight);

	//��������:��������������
	void DrawAxisStyle(CRect rRect,
					 long lMinValue, 
					 long lMaxValue,
					 int nDiv, 
					 COLORREF crLineColor, 
					 COLORREF crNormalTextColor,
					 long* pXSign = NULL,
					 long  nSignNum = 0,
					 long  nHotSign = 0);

//��������: ���ü۸�С����λ��
	inline void SetDecimal(int iDecimal,int nPow){m_iDeimal = iDecimal;m_nPow = nPow;};

	//��������: ��������������,����ʵʱ�����ߡ����ߡ���������������
	void DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, long lMaxValue, long lMinVlaue, long* pValues, long lValueNum);

	//��������: ����������״ͼ�����ڳɽ����Ļ�ͼ
	void DrawVolumnBar(CRect rRect, int nLineWidth, long lMaxValue, long lMinValue, long* pValues, long* pPrice,
		                long lValueNum, COLORREF clrRiseColor, COLORREF clrDownColor, int iRefPos = 0);

	//��������: ���ú������
	virtual void SetHoriPoint(int iHoriPoint);

	//��������: ��������
	virtual BOOL UpdateData(LPVOID pNewData) = 0;

	//��������: ���Ե��Ƿ��������� 
	virtual BOOL HitTest(CPoint point);

	//�������ܣ���ͼ
	virtual BOOL Draw(CDC* pDC) = 0;

	//�������ܣ���ͼͼ���ϲ���ı���Ϣ
	virtual void DrawTitleText();

	//�������ܣ����õ�ǰ����
	CFont* SetFont(CFont* pFont);

	//�������ܣ����û�ͼ����
	virtual void MoveTo(CRect rRect);

	//�������ܣ����õ�Ԫ��߶�
	virtual void SetItemHeight(int nHeight){m_nVertCellHeight =nHeight; };
	
	//�������ܣ���λ�õõ��۸�
	long GetPriceFromStation(int nStation);		
	
	//�������ܣ�������������
	void DrawTotalRect(COLORREF clrLine);

	virtual int GetStationFromPrice(long lPrice) =0;
protected:
	//�������ܣ�����ǰ�����
	virtual void DrawCurrentStationLine();
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��Ƚϵõ������Сֵ
	void GetMaxMinPrice(long &lMinPrice,long& lMaxPrice,long *pData, long nDataNum,BOOL bFlash = TRUE);
protected:
	int	   m_iDeimal;			//С����λ��
	int    m_nPow;				//���Եı���
	long   m_lMaxPrice;			//���۸�
	long   m_lMinPrice;			//��С�۸� 
	
	long*  m_pHoriPointStation; //ˮƽ������λ��
	long   m_lHoriPoint;		//ˮƽ������� 
	long   m_nVertCellHeight;	//��Ԫ��߶�
	int    m_nVAxisCount;       //��ֱ��������
	CPoint m_CurrentPoint;		//��ǰλ�õ�
	long   m_nCurrentStation;   //��ǰ����λ��
	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 2013/09/17
	//������ʮ���ߵļ۸�
	long  m_lCurrentPrice;//��ǰ�۸�
	//////////////////////////////////////////////////////////////////////////
	int    m_nValueNum;		    //��ǰ��Ч��ֵ����	
protected:	
	CRect  m_TotalRect;			//������
	CRect  m_rectDraw;		    //��Ч��ͼ����	 
	CDC*   m_pDC;			    //��ͼ�豸ָ�� 
	CFont* m_pFont;			    //��������
};

#endif /* _INC_DYNBASEPIC_409F013A0251_INCLUDED */
