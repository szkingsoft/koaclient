// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_HISCURVEPIC_409F3A80028F_INCLUDED
#define _INC_HISCURVEPIC_409F3A80028F_INCLUDED

#include "techanalysisbasepic.h"
//Function:
class CTechCreator
{
public:
	void ZoomVertAxis(bool bZoomIn);
	void MouseRBtnUp(CPoint pt);
	//////////////////////////////////////////////////////////////////////////
	//ɾ�����л���
	void RemoveAllDrawLine();
	//////////////////////////////////////////////////////////////////////////
	//���û�������
	void SetDrawLineType(int nLineType);
	//////////////////////////////////////////////////////////////////////////
	//ɾ����ǰ����
	BOOL DeleteCurrentDrawLine();
	//////////////////////////////////////////////////////////////////////////
	//���º�������K��ʹ�ýӿ�
	//������ʾ����
	void SetShowStyle(DWORD dwStyle);
	//����֤ȯ̧ͷ��Ϣ
	void SetTitleInfo(CString szTitle,int nDataType);
	//////////////////////////////////////////////////////////////////////////
	//����Ϊ�����ӿ�
	CTechCreator();
	~CTechCreator();
	//////////////////////////////////////////////////////////////////////////
	//����Ҽ�����
	BOOL MouseRBDown(CPoint point);
	//////////////////////////////////////////////////////////////////////////
	//�ж���������ָ�꣬����Ϊѡ��״̬
	BOOL MouseDown(CPoint point);
	//����ƶ�
	BOOL MouseMove(CPoint point);
	//���̧����Ҫ���ڻ���
	BOOL MouseUp(CPoint point);
	//��������
	virtual BOOL UpdateData(LPVOID pNewData);
	//������ָ��ͼ��
	virtual BOOL Draw(CDC* pDC);
 	//���û�ͼ����
	virtual void MoveTo(CRect rRect);
	//���Ե�ǰλ��
	int HitTest(CPoint point);
	//���õ�ǰλ�õ�
	void SetCurrentStation(int nStation);	
	//���ü���ָ������
	void SetTechType(CString nType);
	void UpdateTechNameTitle();
 	//�õ�ָ������
	CString GetTechType(){return m_szTechType;};
  	//����ˮƽ���
	void SetHoriPoint(int* pHoriPoint,int nHoriPoint);
	//ͨ��λ�õõ��۸�
	float GetPriceFromStation(int nStation);
	//ͨ���۸�õ�λ��
	int GetStationFromPrice(float nValue);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// add by david gong at 2014-08-13
	int GetDateFromStation(int nXStation);
	int GetStationFromDate(int nDate);
	void GetDrawRect(CRect& rect);
	double GetUnitLength();
	BOOL GetHighLowFromTime(const int & time,float &dHigh,float &dLow);
	void GetCloseFromXSgmt( const int &uBeginX,const int &uEndX, CArray< CPoint,CPoint&> &arPtClose );//���ָ��������ڵ��������̼�
	BOOL GetHighLowFromXSgmt( const UINT uBeginX, const UINT uEndX, CPoint &ptHigh, CPoint &ptLow );
	//////////////////////////////////////////////////////////////////////////
 	//�򿪼���ָ������޸����öԻ���
 	//�õ���������
	int GetMaxParam();	
	
	//�����������
	void ResetData();

	//���˫��̧ͷ
	int MouseDbClickTitle(CPoint pt);

	//������ǰͼԪ���
	void DrawTotalRect(CDC* pDC);

	//����������ֵ
	void SetMaxParamer(int nMaxPar);
private:
	CTechAnalysisBasePic* m_pTechAnalysis;  //����ָ��ָ��
	CString       m_szTechType;      //����ָ������
};
#endif /* _INC_HISCURVEPIC_409F3A80028F_INCLUDED */
