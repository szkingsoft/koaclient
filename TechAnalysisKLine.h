// TechAnalysisKLine.h: interface for the CTechAnalysisKLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TECHANALYSISKLINE_H__614B4D0A_91FF_4688_A3EC_1AAB7F17F7D0__INCLUDED_)
#define AFX_TECHANALYSISKLINE_H__614B4D0A_91FF_4688_A3EC_1AAB7F17F7D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TechAnalysisBasePic.h"

class CTechAnalysisSelf;
class CTechAnalysisKLine : public CTechAnalysisBasePic  
{
public:
	void ZoomVertAxis(bool bZoomIn);
	void SetStockInfo(const SCodeInfo* pStock){m_pCodeInfo = pStock;};
	//�Ҽ�����
	void MouseRBtnUp(CPoint pt);
	//����Ҽ�����
	virtual BOOL MouseRBDown(CPoint point);
	//���Ե������
	virtual int HitTest(CPoint point);
	//��갴��
	virtual BOOL MouseDown(CPoint point);
	//���˫��̧ͷ
	int MouseDbClitTitle(CPoint pt);
	//����ˮƽ����
	virtual void SetHoriPoint(int *pHoriPoint,int nHoriPoint);
	//���õ�ǰλ��
	virtual void SetCurrentStation(int iStation);
	//�õ�������
	virtual int GetMaxParam();
	//�����Ƿ񻭳�����
	void SetMAMode(BOOL bDraw,CString strIndex);
	//����λ��
	virtual void CalcStation();
	//���û�ͼ����
	virtual void MoveTo(CRect rRect);
	//��������
	virtual BOOL UpdateData(LPVOID pNewData);

 	CTechAnalysisKLine();
	virtual ~CTechAnalysisKLine();

	//����ƶ�
	BOOL MouseMove(CPoint point);
	// ����Ƴ�
	void MouseMoveOut();
	//////////////////////////////////////////////////////////////////////////
	//����ͼ��
	virtual BOOL Draw(CDC* pDC);
	//������ʷ��ͼ��Ʊ����
	virtual void SetTitleInfo(CString szTitle,int nDataType);
	//������ʾ����

	//��������
	virtual void ResetData();
private:

	//����ѡ������
	void DrawSelectRect();
	//��������:��������������
	void DrawSpecialAxisStyle(CRect rRect,
		float* pStation,
		int nDiv,  
		COLORREF crLineColor, 
		COLORREF crNormalTextColor);	
	//��������ɫ��K��
	void DrawColorKLine(int nStation,COLORREF crKLine,int nMask = 0);
	//���ҵ�ǰ�����������е�λ��
	int FindStationFromDate(int nDate);
	//////////////////////////////////////////////////////////////////////////
	//��K��ָ����ʾֵ
	virtual void DrawTitleText();
	//�������Сֵ
	void DrawMaxAndMinSign();
	//����ǰ�۸���
	void DrawCurrentPrice();

	//////////////////////////////////////////////////////////////////////////
protected:
	//���������ڵ������Сֵ
	virtual void CalcMinMaxPrice();
 	//������ߵ�ֵ
	virtual void CalculateValue();
private:
	//////////////////////////////////////////////////////////////////////////
	BOOL		m_bDrawMA;//��������
	CString		m_szStockName;//֤ȯ����

	
	CTechAnalysisSelf *	m_pMAIndex;//���߼���ָ��
	int				  m_nMaxPriceStation;//��߼�λ��
	int				  m_nMinPriceStation;//��ͼ�λ��


	float			  m_fLastPrice;		//���¼۸�


	//����δ����λʹ��
	int	  m_nFristClickStation;//��һ���λ��
	int	  m_nSecondClickStation;//�ڶ����λ��
	BOOL  m_bRBtnDown;//�Ҽ��Ƿ���
	CPoint m_ptRBtnBegin,m_ptRBtnEnd;//����Ҽ�����

	BOOL  m_bRBFirstClick;//��׵�һ�ε��

	struct SClickStation
	{
		int nDate;
		int nPrice;
	}  m_SelectRect[2];//ѡ��Ļ�������

	BOOL m_bSelectPointError;//ѡ������־
	const SCodeInfo* m_pCodeInfo;

	int m_nCircleType;//����
};

#endif // !defined(AFX_TECHANALYSISKLINE_H__614B4D0A_91FF_4688_A3EC_1AAB7F17F7D0__INCLUDED_)
