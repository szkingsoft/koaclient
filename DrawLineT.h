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

const int TITLESIZE = 16;//抬头大小区间
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
	//画线计算
	//删除所有画线
	void RemoveAllDrawLine();
	//鼠标移动
	virtual BOOL MouseMove(CPoint point);
	//删除当前选中画线
	BOOL DeleteCurrentDrawLine();
	//设置画线类型
	void SetDrawLineType(int nLineType);
	//鼠标左键按下
	virtual BOOL MouseUp(CPoint point);

	//从位置得到日期
	BOOL GetDateFromPoint(const CPoint& point,int& nDay,float& fPrice);
	//从日期得到位置
	BOOL GetPointFromDate(int nDay,float fPrice,CPoint& point);

	//////////////////////////////////////////////////////////////////////////
	//得到画图区间
	void GetDrawRect(CRect& rRect);
	//重新设置参数数据缓冲区
	BOOL ResetParaBuffers(int nParam, int nCurvers);
	//得到参数数量
	int  GetParamNum();
	//重置数据
	virtual void ResetData();
	//得到抬头参数区域
	const CRect& GetTitleParamRect();
	//设置画图区域
	void SetRect(CRect rRect, int nRectType);
	//设置抬头开始位置
	void SetTitleBeginStation(int nStation);

	//设置最大最小值，主要用于均线等叠加指标[
	void SetMaxMinValue(float nMaxValue,float nMinValue);
	//得到最大最小值
	void GetMaxMinValue(float& nMaxValue,float& nMinValue);



	//////////////////////////////////////////////////////////////////////////
	//以下函数仅供K线使用接口
	
	//设置除权信息
	virtual void SetPoweInfo(LPVOID pData,int nPowerNum){};
	//设置显示属性
	virtual void SetShowStyle(DWORD dwStyle);
	//设置信息地雷索引信息
	virtual void SetMineIndexInfo(LPVOID lpData){};
	//设置证券抬头信息
	virtual void SetTitleInfo(CString szTitle,int nDataType){};
	//////////////////////////////////////////////////////////////////////////
	//以下为公共接口
	//////////////////////////////////////////////////////////////////////////
 	//更新数据
	virtual BOOL UpdateData(LPVOID pNewData);
	//移动位置
	virtual void MoveTo(CRect rRect);
	//画出图形
	virtual BOOL Draw(CDC* pDC);
 	//打开技术指标参数设置对话框
	//打开“用法注释”对话框
	void OpenTechNoteDlg();	
 	//得到最大参数
	virtual int GetMaxParam(); 	
	// 更新技术指标类型
	void SetTechType(CString strType);
	//更新及时指标抬头	
	void UpdateTechNameTitle();	
	//测试当前点击点
	virtual BOOL MouseDown(CPoint point);
	//鼠标按下右键
	virtual BOOL MouseRBDown(CPoint point);
	//鼠标双击抬头
	virtual	int MouseDbClitTitle(CPoint pt);
	//计算技术指标中每个值在图形区域中的纵坐标值
	virtual void CalcStation(); 
	//////////////////////////////////////////////////////////////////////////
	//设置最大参数值
	void SetMaxParamer(int nMaxPar){m_nMaxParamDay = nMaxPar;};
protected:

	//////////////////////////////////////////////////////////////////////////
	//画出美国线
	virtual void DrawAmercianLine(int nStation);

	//画单个K线
	virtual void DrawKLine(int nStation);

	//画提示信息
	virtual void DrawTip(CDC *pDC,CPoint point,CString strTip);
	//画成交量柱状条
	virtual void DrawVolumnBar(CRect rRect,
								int nLineWidth,
								int *pValues, 
								int lValueNum,
								COLORREF clrRiseColor,
								COLORREF clrDownColor);

	//函数功能: 画出条状线信息
	virtual void DrawBarLine(CRect rRect, 
		int nLineWidth, 
		int *pValues, 
		int lValueNum,
		COLORREF crRiseTextColor,
		COLORREF crDownTextColor,
		float fBaseValue = 0.0f);
	//函数功能: 画出各类连接线,用于实时走势线、均线、其他各类连接线
	virtual void DrawLinkLine(CRect rRect,
		int nLineWidth, 
		COLORREF clrLineColor,
		int* pValues, 
		int lValueNum,
		BOOL bSelctState = FALSE);
	//////////////////////////////////////////////////////////////////////////
	//创建技术分析参数
	void Create(CString strParamTitle,int nParam,int nCurvers);
	// 更新技术指标参数内容
	void UpdateIndexParamInfo(CString strParamTitle);	
	//计算技术指标值
	virtual void CalculateValue(){};
 	//计算指标中的最大最小值
	virtual void CalcMinMaxPrice();
 	//画指标显示数据
	virtual	void DrawTitleText();
  	//读取技术参数参数
	void ReadTechCurveParam(CString strIndexType);
 	//平整浮点数
	int TrimDouble(double fValue);
	//得到参数名称
	CString GetParamName(int nIndex);
protected:
 	CRect	m_TitleNameRect;//抬头名字区域
	CRect   m_TitleParamRect;//抬头参数区域

	CRect   m_TitleRect;//抬头文本区域
	CRect		m_AxisRect;		//坐标区域
	int	m_nTitleBeginStation;   //指标值显示的开始位置
	
	DWORD		m_dwShowStyle;//显示类型

	CFormulaFace m_thCalc;		//指标公式调用接口
	
	const SHistoryUnit*	m_pKUnit;		//K线数据	
	int			m_nDataBegin;	//水平开始点

	int			m_nCurrentSelCurves;//当前选择的指标线	

	int			m_nMaxParamDay;		//最大参数日期，显示点
	enum AXISMODE
	{
		PRICEMODE,//普通价格坐标
		PRECENTMODE,//百分比坐标
		LOGARITHMMODE,//对数坐标
	};
	AXISMODE			m_nAxisMode;//坐标方式
	//////////////////////////////////////////////////////////////////////////
	//画线
	CArray<CDrawLineTool,CDrawLineTool&>m_ArrDrawLine;//画线
	int		m_nDrawState;	//画线状态
	int		m_nSelectedDrawLine;//当前选中的画线
	CPoint	m_SelectDrawLineOrginPoint;//选择的原始点
	int		m_nSelectedState;	//选中画线的状态
	int     m_nLineType;		//画线的类型

private:
	CString     m_strIndexType;        //当前指标类型
	CString		m_strIndexNameTitle;     //指标名称标题
	CString		m_strIndexParamTitle;	 //指标参数标题

protected:
	CPoint		  m_ptNowStation;//当前鼠标点
	//////////////////////////////////////////////////////////////////////////
	// add by david at 06-03-22
	int		   m_nRecsOfUnit;//每个点的K线数量

};

#endif // !defined(AFX_TECHANALYSISBASEPIC_H__E9142FE4_6A95_49E6_AB6B_C0AE31ED6EEB__INCLUDED_)
