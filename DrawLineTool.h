// DrawLineTool.h: interface for the CDrawLineTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWLINETOOL_H__DF8FFCE6_40AB_4B5A_830E_001913C6DC26__INCLUDED_)
#define AFX_DRAWLINETOOL_H__DF8FFCE6_40AB_4B5A_830E_001913C6DC26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum LINETYPE
{
	LINE,
	RECTANGLE,
	PARALLEL,
	GOLDLINE,
	GANNLINE,
};
enum SELECTEDSTAT
{
	NOSELECT,
	SELECTEDFIRSTPOINT,
	SELECTEDENDPOINT,
	SELECTTHIRDPOINT,
	SELECTRECTANGLE,
};
//���߹�����
class CTechAnalysisBasePic;

class CDrawLineTool  
{
public:	
	void MoveEnd(CSize offset);
	void GetDrawRect(CRect& rRect);
	void SetParent(LPVOID pParent);
	CDrawLineTool();
	~CDrawLineTool();

	void SetThirdPoint(CPoint point);
	void SetSelectState(BOOL bSelected);
	void SetOffset(CSize offset);
	void SetDrawMode(int nState);
	int  CheckIsLine(CPoint point);//�����Ƿ������� 0Ϊû��ѡ�У�1Ϊѡ�е�һ���㣬2Ϊѡ�еڶ����㣬3Ϊ�������㣬4Ϊѡ�����򣬿���ƽ��

	void SetColor(COLORREF cr){m_crLine = cr;};//���û�����ɫ
	void Draw(CDC* pDC);	   //����ͼ��
	void MouseLBUp(CPoint point);//��굯��
	void MouseLBDown(CPoint point);//��갴��

private:
	void getBorderPoint(CPoint Begin,CPoint End,CPoint* BordPoint);
	void DrawGannLine(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	void DrawGoldLine(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	void DrawParallel(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	void DrawRectangle(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	void DrawLine(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	
	LINETYPE m_nState;//��ͼ״̬
	struct SKeyPoint
	{
		int nDay;//����
		float fPrice;//�۸�
	};
	SKeyPoint	m_BeginPoint;
	SKeyPoint	m_EndPoint;
	SKeyPoint	m_ThirdPoint;//������
	COLORREF m_crLine;//����ɫ
	BOOL	m_bSelected;//�Ƿ�Ϊѡ��״̬
	CRect	m_DrawRect;//��ͼ����
	CTechAnalysisBasePic* m_pHistoryObj;//������

	BOOL   m_bMoveState;//�ƶ�״̬
	CSize  m_MoveSize;//�ƶ���С
};

#endif // !defined(AFX_DRAWLINETOOL_H__DF8FFCE6_40AB_4B5A_830E_001913C6DC26__INCLUDED_)
