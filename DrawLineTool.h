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
//画线工具类
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
	int  CheckIsLine(CPoint point);//检查点是否在线上 0为没有选中，1为选中第一个点，2为选中第二个点，3为第三个点，4为选中区域，可以平移

	void SetColor(COLORREF cr){m_crLine = cr;};//设置画线颜色
	void Draw(CDC* pDC);	   //画出图形
	void MouseLBUp(CPoint point);//鼠标弹起
	void MouseLBDown(CPoint point);//鼠标按下

private:
	void getBorderPoint(CPoint Begin,CPoint End,CPoint* BordPoint);
	void DrawGannLine(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	void DrawGoldLine(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	void DrawParallel(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	void DrawRectangle(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	void DrawLine(CDC *pDC,CPoint beginPoint,CPoint endPoint);
	
	LINETYPE m_nState;//画图状态
	struct SKeyPoint
	{
		int nDay;//日期
		float fPrice;//价格
	};
	SKeyPoint	m_BeginPoint;
	SKeyPoint	m_EndPoint;
	SKeyPoint	m_ThirdPoint;//第三点
	COLORREF m_crLine;//线颜色
	BOOL	m_bSelected;//是否为选中状态
	CRect	m_DrawRect;//画图区间
	CTechAnalysisBasePic* m_pHistoryObj;//父窗口

	BOOL   m_bMoveState;//移动状态
	CSize  m_MoveSize;//移动大小
};

#endif // !defined(AFX_DRAWLINETOOL_H__DF8FFCE6_40AB_4B5A_830E_001913C6DC26__INCLUDED_)
