// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_BASHISEPIC_409F013A0251_INCLUDED
#define _INC_BASHISEPIC_409F013A0251_INCLUDED

class CBasePic 
{
public:	
	enum AXISMODE{
		DOUBLESIDEDOUBLECOLOR,//˫��˫ɫ
		DOUBLESIDESINGLECOLOR,//˫�ߵ�ɫ
		SINGLESIDEDOUBLECOLOR,//����˫ɫ 
		SINGLESIDESINGLECOLOR,//���ߵ�ɫ
		KLINEMODE,
		HISTORYAXISMODE		  //��ʷ��������
	};//���껭ͼģʽ


	enum LINETYPE	//������
	{
		LINKLINE,	//������
		BARLINE,	//��״��
		CIRCLELINE, //Ȧ״��
		KLINE,		//K��
		VOLUME,		//�ɽ���
		AMERICANLINE//������
	};

	CBasePic();
	virtual ~CBasePic();
	//////////////////////////////////////////////////////////////////////////
 	//��������: ��������
	virtual BOOL UpdateData(LPVOID pNewData) = 0;
	//��������: ���Ե��Ƿ��������� 
	virtual int HitTest(CPoint point); 	
	//�������ܣ���ͼͼ���ϲ���ı���Ϣ
	CFont* SetFont(CFont* pFont);
	//�������ܣ����û�ͼ����
	virtual void MoveTo(CRect rRect);

	//����ƶ�
	virtual BOOL MouseMove(CPoint point);
	// ����Ƴ�
	virtual void MouseMoveOut();	
	//����������
	virtual BOOL MouseLBtnUp(CPoint point);
	//����������
	virtual BOOL MouseLBtnDown(CPoint point);

	//////////////////////////////////////////////////////////////////////////
 	//��������: ���ú������
	virtual void SetHoriPoint(int *pHoriPoint,int nHoriPoint);
	//��������:���õ�ǰ���λ��
	virtual void SetCurrentStation(int iStation);
	//ͨ��λ�õõ�ָ��ֵ
	virtual float GetPriceFromStation(int nStation);
	//ͨ��ָ��ֵ�õ�λ��
	int GetStationFromPrice(float fValue);
	//��������: �õ���ֱ�����߸���
	int GetVertAxisCellNum(int iHeight);
	//��������: ����ָ����ʾֵ��С����λ��
	inline void SetDecimal(int iDecimal){m_nDecimal = iDecimal;};
	//////////////////////////////////////////////////////////////////////////
 	//������������
	virtual void DrawTotalRect(CDC* pDC);

	//ʵʱ��������
	virtual void DrawRealAxisStyle(CRect rRect,
							int nDiv, 
							AXISMODE nMode,
							COLORREF crLineColor, 
							COLORREF crNormalTextColor,
							COLORREF crRiseTextColor,
							COLORREF crDownTextColor,
							long* pXSign = NULL,
							long  nSignNum = 0,
							long  nHotSign = 0);

  	//��������: ������״����Ϣ
 	virtual void DrawBarLine(CRect rRect, 
					int nLineWidth, 
					int *pValues, 
					int lValueNum,
					COLORREF crRiseTextColor,
					COLORREF crDownTextColor,
					float fBaseValue = 0.0f);

   	//��������:��������������
 	virtual void DrawAxisStyle(CRect rRect,
					 float* pStation,
					 int nDiv,  
					 AXISMODE nMode,
					 COLORREF crLineColor, 
					 COLORREF crNormalTextColor); 	


	//���ٷֱ�����
	virtual void DrawPrecentAxisStyle(CRect rRect,
							float* pStation,
							int nDiv,  
							AXISMODE nMode,
							COLORREF crLineColor, 
							COLORREF crNormalTextColor,
							float fBasePrice); 

 	//��������: ��������������,����ʵʱ�����ߡ����ߡ���������������
 	virtual void DrawLinkLine(CRect rRect,
					  int nLineWidth, 
					  COLORREF clrLineColor,
					  int* pValues, 
					  int lValueNum,
					  BOOL bSelctState = FALSE);

	//�������ܣ�����ͼ��
	virtual BOOL Draw(CDC* pDC) = 0; 
protected:
 	//�������ܣ��Ƚϵõ������Сֵ
 	void GetMaxMinPrice(float &fMinPrice,float& fMaxPrice,float *pData, int nDataNum,BOOL bFlash = TRUE);
protected:
	int m_nDecimal;           //ָ��ֽ��ʾ���ݵ�С����λ��

	float m_fMaxPrice;       //���۸�
	float m_fMinPrice;	     //��С�۸�
 
	int* m_pHoriPoint;		 //ˮƽ������λ��
	int m_nHoriPoint;		 //ˮƽ��������

	CPoint m_CurrentPoint;   //��ǰλ�õ�
	int m_nCurrentStation;   //��ǰ����λ��
	
	int m_nVAxisCount;       //��ֱ��������
	int m_nValueNum;         //��ǰ��Ч��ֵ����

	CRect m_rectDraw;        //��Ч��ͼ����
	CRect m_TotalRect;       //������
	int   m_nKLineWidth;     //K�߿��
	int	  m_nKLineSperator;  //K�߼��
	
	CDC* m_pDC;              //��ͼ�豸ָ��
	CFont * m_pFont;         //�������� 
private:
	BOOL m_bActivate;		 //�Ƿ�Ϊ��ǰ����״̬
	int m_nVertCellHeight;   //��Ԫ��߶�

protected:
	///////////////////////////////////////////////////////////
	float** m_ppCurveValue;	    //ָ������ָ���ָ��
	int** m_ppCurveStation;     //���ݵ�λ��
	int	  m_nCurves;	        //������

	int*  m_pLineType;	        //ָ�껭�����ͣ���K�ߡ��ɽ�����	
	int*  m_pParamValue;	    //ָ�����ָ��	
	COLORREF* m_pColorValue;    //ָ���е�ÿ���ߵ���ɫ����	
	int	  m_nParamNum;	        //ָ���������	

	void InitDataBuffer(int nDataSize);
	//�����������
	void DestroyObj();

	//////////////////////////////////////////////////////////////////////////
	//
	float m_fVertZoomRaw;
};

#endif /* _INC_BASEPIC_409F013A0251_INCLUDED */
