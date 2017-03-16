// TechAnalysisBasePic.h: interface for the CDrawLineT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(H_DRAWLINE_T)
#define H_DRAWLINE_T

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasePic.h"
#include "drawlinetool.h"

const int TITLESIZE = 16;//̧ͷ��С����
const int NOAXISRECT = 0x0004;

enum DRAWLINESTAT
{
	NULLDRAW = 0,
	BEGINDRAW,
	LEFTBTNDOWN,
	ENDDRAW,
	SELECTSTATE,
	MOVEDRAW,
	ENDMOVE,
};

class CDrawLineT   : public CBasePic 
{
public:
	virtual void ZoomVertAxis(bool bZoomIn){};
	CDrawLineT();
	virtual ~CDrawLineT();

	//////////////////////////////////////////////////////////////////////////
	//���߼���
	//ɾ�����л���
	void RemoveAllDrawLine();
	//����ƶ�
	virtual BOOL MouseMove(CPoint point);
	//ɾ����ǰѡ�л���
	BOOL DeleteCurrentDrawLine();
	//���û�������
	void SetDrawLineType(int nLineType);
	//����������
	virtual BOOL MouseUp(CPoint point);

	//��λ�õõ�����
	BOOL GetDateFromPoint(const CPoint& point,int& nDay,float& fPrice);
	//�����ڵõ�λ��
	BOOL GetPointFromDate(int nDay,float fPrice,CPoint& point);

	//////////////////////////////////////////////////////////////////////////
	//�õ���ͼ����
	void GetDrawRect(CRect& rRect);
	//�������ò������ݻ�����
	BOOL ResetParaBuffers(int nParam, int nCurvers);
	//�õ���������
	int  GetParamNum();
	//��������
	virtual void ResetData();
	//�õ�̧ͷ��������
	const CRect& GetTitleParamRect();
	//���û�ͼ����
	void SetRect(CRect rRect, int nRectType);
	//����̧ͷ��ʼλ��
	void SetTitleBeginStation(int nStation);

	//���������Сֵ����Ҫ���ھ��ߵȵ���ָ��[
	void SetMaxMinValue(float nMaxValue,float nMinValue);
	//�õ������Сֵ
	void GetMaxMinValue(float& nMaxValue,float& nMinValue);



	//////////////////////////////////////////////////////////////////////////
	//���º�������K��ʹ�ýӿ�
	
	//���ó�Ȩ��Ϣ
	virtual void SetPoweInfo(LPVOID pData,int nPowerNum){};
	//������ʾ����
	virtual void SetShowStyle(DWORD dwStyle);
	//������Ϣ����������Ϣ
	virtual void SetMineIndexInfo(LPVOID lpData){};
	//����֤ȯ̧ͷ��Ϣ
	virtual void SetTitleInfo(CString szTitle,int nDataType){};
	//////////////////////////////////////////////////////////////////////////
	//����Ϊ�����ӿ�
	//////////////////////////////////////////////////////////////////////////
 	//��������
	virtual BOOL UpdateData(LPVOID pNewData);
	//�ƶ�λ��
	virtual void MoveTo(CRect rRect);
	//����ͼ��
	virtual BOOL Draw(CDC* pDC);
 	//�򿪼���ָ��������öԻ���
	//�򿪡��÷�ע�͡��Ի���
	void OpenTechNoteDlg();	
 	//�õ�������
	virtual int GetMaxParam(); 	
	// ���¼���ָ������
	void SetTechType(CString strType);
	//���¼�ʱָ��̧ͷ	
	void UpdateTechNameTitle();	
	//���Ե�ǰ�����
	virtual BOOL MouseDown(CPoint point);
	//��갴���Ҽ�
	virtual BOOL MouseRBDown(CPoint point);
	//���˫��̧ͷ
	virtual	int MouseDbClitTitle(CPoint pt);
	//���㼼��ָ����ÿ��ֵ��ͼ�������е�������ֵ
	virtual void CalcStation(); 
	//////////////////////////////////////////////////////////////////////////
	//����������ֵ
	void SetMaxParamer(int nMaxPar){m_nMaxParamDay = nMaxPar;};
protected:

	//////////////////////////////////////////////////////////////////////////
	//����������
	virtual void DrawAmercianLine(int nStation);

	//������K��
	virtual void DrawKLine(int nStation);

	//����ʾ��Ϣ
	virtual void DrawTip(CDC *pDC,CPoint point,CString strTip);
	//���ɽ�����״��
	virtual void DrawVolumnBar(CRect rRect,
								int nLineWidth,
								int *pValues, 
								int lValueNum,
								COLORREF clrRiseColor,
								COLORREF clrDownColor);

	//��������: ������״����Ϣ
	virtual void DrawBarLine(CRect rRect, 
		int nLineWidth, 
		int *pValues, 
		int lValueNum,
		COLORREF crRiseTextColor,
		COLORREF crDownTextColor,
		float fBaseValue = 0.0f);
	//��������: ��������������,����ʵʱ�����ߡ����ߡ���������������
	virtual void DrawLinkLine(CRect rRect,
		int nLineWidth, 
		COLORREF clrLineColor,
		int* pValues, 
		int lValueNum,
		BOOL bSelctState = FALSE);
	//////////////////////////////////////////////////////////////////////////
	//����������������
	void Create(CString strParamTitle,int nParam,int nCurvers);
	// ���¼���ָ���������
	void UpdateIndexParamInfo(CString strParamTitle);	
	//���㼼��ָ��ֵ
	virtual void CalculateValue(){};
 	//����ָ���е������Сֵ
	virtual void CalcMinMaxPrice();
 	//��ָ����ʾ����
	virtual	void DrawTitleText();
  	//��ȡ������������
	void ReadTechCurveParam(CString strIndexType);
 	//ƽ��������
	int TrimDouble(double fValue);
	//�õ���������
	CString GetParamName(int nIndex);
protected:
 	CRect	m_TitleNameRect;//̧ͷ��������
	CRect   m_TitleParamRect;//̧ͷ��������

	CRect   m_TitleRect;//̧ͷ�ı�����
	CRect		m_AxisRect;		//��������
	int	m_nTitleBeginStation;   //ָ��ֵ��ʾ�Ŀ�ʼλ��
	
	DWORD		m_dwShowStyle;//��ʾ����

	CFormulaFace m_thCalc;		//ָ�깫ʽ���ýӿ�
	
	const SHistoryUnit*	m_pKUnit;		//K������	
	int			m_nDataBegin;	//ˮƽ��ʼ��

	int			m_nCurrentSelCurves;//��ǰѡ���ָ����	

	int			m_nMaxParamDay;		//���������ڣ���ʾ��
	enum AXISMODE
	{
		PRICEMODE,//��ͨ�۸�����
		PRECENTMODE,//�ٷֱ�����
		LOGARITHMMODE,//��������
	};
	AXISMODE			m_nAxisMode;//���귽ʽ
	//////////////////////////////////////////////////////////////////////////
	//����
	CArray<CDrawLineTool,CDrawLineTool&>m_ArrDrawLine;//����
	int		m_nDrawState;	//����״̬
	int		m_nSelectedDrawLine;//��ǰѡ�еĻ���
	CPoint	m_SelectDrawLineOrginPoint;//ѡ���ԭʼ��
	int		m_nSelectedState;	//ѡ�л��ߵ�״̬
	int     m_nLineType;		//���ߵ�����

private:
	CString     m_strIndexType;        //��ǰָ������
	CString		m_strIndexNameTitle;     //ָ�����Ʊ���
	CString		m_strIndexParamTitle;	 //ָ���������

protected:
	CPoint		  m_ptNowStation;//��ǰ����
	//////////////////////////////////////////////////////////////////////////
	// add by david at 06-03-22
	int		   m_nRecsOfUnit;//ÿ�����K������

};

#endif // !defined(AFX_TECHANALYSISBASEPIC_H__E9142FE4_6A95_49E6_AB6B_C0AE31ED6EEB__INCLUDED_)
