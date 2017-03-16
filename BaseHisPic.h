#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CBASHISEPIC_409F013A0251_INCLUDED
#define _INC_CBASHISEPIC_409F013A0251_INCLUDED

class CBaseHisPic 
{
public:	
	enum AXISMODE{
		KLINEMODE,
		HISTORYAXISMODE		  //��ʷ��������
	};//���껭ͼģʽ
	CBaseHisPic();
	virtual ~CBaseHisPic();
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
	virtual long GetPriceFromStation(int nStation);
	//ͨ��ָ��ֵ�õ�λ��
	int GetStationFromPrice(int nValue);
	//��������: �õ���ֱ�����߸���
	int GetVertAxisCellNum(int iHeight);
	//��������: ����ָ����ʾֵ��С����λ��
	inline void SetDecimal(int iDecimal){m_iDeimal = iDecimal;};
	//////////////////////////////////////////////////////////////////////////
 	//������������
	void DrawTotalRect(CDC* pDC);
  	//��������: ������״����Ϣ
 	void DrawBarLine(CRect rRect, int nLineWidth, int lMaxValue, int lMinVlaue, int *pValues, int lValueNum,COLORREF crRiseTextColor,COLORREF crDownTextColor,int nBaseValue = 0);
   	//��������:��������������
 	void DrawAxisStyle(CRect rRect,
					 int lMinValue, 
					 int lMaxValue,
					 float* pStation,
					 int nDiv, 
					 int nDecimal, 
					 AXISMODE nMode,
					 COLORREF crLineColor, 
					 COLORREF crNormalTextColor); 	
 	//��������: ��������������,����ʵʱ�����ߡ����ߡ���������������
 	void DrawLinkLine(CRect rRect,
					  int nLineWidth, 
					  COLORREF clrLineColor,
					  int lMaxValue, 
					  int lMinVlaue, 
					  int* pValues, 
					  int lValueNum,
					  BOOL bSelctState = FALSE
					  );
  	//��������: ����������״ͼ�����ڳɽ����Ļ�ͼ
	//�������ܣ�����ͼ��
	virtual BOOL Draw(CDC* pDC) = 0; 
protected:
 	//�������ܣ��Ƚϵõ������Сֵ
 	void GetMaxMinPrice(long &lMinPrice,long& lMaxPrice,int *pData, int nDataNum,BOOL bFlash = TRUE);
protected:
	int m_iDeimal;           //ָ��ֽ��ʾ���ݵ�С����λ��

	long m_nMaxPrice;         //���۸�
	long m_nMinPrice;	     //��С�۸�
 
	int* m_pHoriPointStation;//ˮƽ������λ��
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
	float m_fVertZoomRaw;	//����ѹ������

};

#endif /* _INC_CBASEPIC_409F013A0251_INCLUDED */
