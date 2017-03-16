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
	//右键按下
	void MouseRBtnUp(CPoint pt);
	//鼠标右件按下
	virtual BOOL MouseRBDown(CPoint point);
	//测试点击区域
	virtual int HitTest(CPoint point);
	//鼠标按下
	virtual BOOL MouseDown(CPoint point);
	//鼠标双击抬头
	int MouseDbClitTitle(CPoint pt);
	//设置水平坐标
	virtual void SetHoriPoint(int *pHoriPoint,int nHoriPoint);
	//设置当前位置
	virtual void SetCurrentStation(int iStation);
	//得到最大参数
	virtual int GetMaxParam();
	//设置是否画出均线
	void SetMAMode(BOOL bDraw,CString strIndex);
	//计算位置
	virtual void CalcStation();
	//设置画图区域
	virtual void MoveTo(CRect rRect);
	//传递数据
	virtual BOOL UpdateData(LPVOID pNewData);

 	CTechAnalysisKLine();
	virtual ~CTechAnalysisKLine();

	//鼠标移动
	BOOL MouseMove(CPoint point);
	// 鼠标移出
	void MouseMoveOut();
	//////////////////////////////////////////////////////////////////////////
	//画出图形
	virtual BOOL Draw(CDC* pDC);
	//设置历史视图股票名称
	virtual void SetTitleInfo(CString szTitle,int nDataType);
	//设置显示属性

	//重置数据
	virtual void ResetData();
private:

	//画出选择区间
	void DrawSelectRect();
	//函数功能:画出各类坐标线
	void DrawSpecialAxisStyle(CRect rRect,
		float* pStation,
		int nDiv,  
		COLORREF crLineColor, 
		COLORREF crNormalTextColor);	
	//画出带颜色的K线
	void DrawColorKLine(int nStation,COLORREF crKLine,int nMask = 0);
	//查找当前日子在坐标中的位置
	int FindStationFromDate(int nDate);
	//////////////////////////////////////////////////////////////////////////
	//画K线指标显示值
	virtual void DrawTitleText();
	//画最大最小值
	void DrawMaxAndMinSign();
	//画当前价格线
	void DrawCurrentPrice();

	//////////////////////////////////////////////////////////////////////////
protected:
	//计算区域内的最大最小值
	virtual void CalcMinMaxPrice();
 	//计算均线的值
	virtual void CalculateValue();
private:
	//////////////////////////////////////////////////////////////////////////
	BOOL		m_bDrawMA;//画出均线
	CString		m_szStockName;//证券名称

	
	CTechAnalysisSelf *	m_pMAIndex;//均线技术指标
	int				  m_nMaxPriceStation;//最高价位置
	int				  m_nMinPriceStation;//最低价位置


	float			  m_fLastPrice;		//最新价格


	//计算未来价位使用
	int	  m_nFristClickStation;//第一点击位置
	int	  m_nSecondClickStation;//第二点击位置
	BOOL  m_bRBtnDown;//右键是否按下
	CPoint m_ptRBtnBegin,m_ptRBtnEnd;//鼠标右键按键

	BOOL  m_bRBFirstClick;//测底第一次点击

	struct SClickStation
	{
		int nDate;
		int nPrice;
	}  m_SelectRect[2];//选择的画线区间

	BOOL m_bSelectPointError;//选点错误标志
	const SCodeInfo* m_pCodeInfo;

	int m_nCircleType;//周期
};

#endif // !defined(AFX_TECHANALYSISKLINE_H__614B4D0A_91FF_4688_A3EC_1AAB7F17F7D0__INCLUDED_)
