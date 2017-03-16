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
		DOUBLESIDEDOUBLECOLOR,//双边双色
		DOUBLESIDESINGLECOLOR,//双边单色
		SINGLESIDEDOUBLECOLOR,//单边双色 
		SINGLESIDESINGLECOLOR,//单边单色
		KLINEMODE,
		HISTORYAXISMODE		  //历史数据坐标
	};//坐标画图模式


	enum LINETYPE	//线类型
	{
		LINKLINE,	//连接线
		BARLINE,	//条状线
		CIRCLELINE, //圈状线
		KLINE,		//K线
		VOLUME,		//成交量
		AMERICANLINE//美国线
	};

	CBasePic();
	virtual ~CBasePic();
	//////////////////////////////////////////////////////////////////////////
 	//函数功能: 更新数据
	virtual BOOL UpdateData(LPVOID pNewData) = 0;
	//函数功能: 测试点是否在区域内 
	virtual int HitTest(CPoint point); 	
	//函数功能：画图图形上侧的文本信息
	CFont* SetFont(CFont* pFont);
	//函数功能：设置画图区域
	virtual void MoveTo(CRect rRect);

	//鼠标移动
	virtual BOOL MouseMove(CPoint point);
	// 鼠标移出
	virtual void MouseMoveOut();	
	//鼠标左键弹起
	virtual BOOL MouseLBtnUp(CPoint point);
	//鼠标左键按下
	virtual BOOL MouseLBtnDown(CPoint point);

	//////////////////////////////////////////////////////////////////////////
 	//函数功能: 设置横坐标点
	virtual void SetHoriPoint(int *pHoriPoint,int nHoriPoint);
	//函数功能:设置当前光标位置
	virtual void SetCurrentStation(int iStation);
	//通过位置得到指标值
	virtual float GetPriceFromStation(int nStation);
	//通过指标值得到位置
	int GetStationFromPrice(float fValue);
	//函数功能: 得到垂直坐标线格数
	int GetVertAxisCellNum(int iHeight);
	//函数功能: 设置指标显示值的小数点位数
	inline void SetDecimal(int iDecimal){m_nDecimal = iDecimal;};
	//////////////////////////////////////////////////////////////////////////
 	//画出整个区域
	virtual void DrawTotalRect(CDC* pDC);

	//实时走势坐标
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

  	//函数功能: 画出条状线信息
 	virtual void DrawBarLine(CRect rRect, 
					int nLineWidth, 
					int *pValues, 
					int lValueNum,
					COLORREF crRiseTextColor,
					COLORREF crDownTextColor,
					float fBaseValue = 0.0f);

   	//函数功能:画出各类坐标线
 	virtual void DrawAxisStyle(CRect rRect,
					 float* pStation,
					 int nDiv,  
					 AXISMODE nMode,
					 COLORREF crLineColor, 
					 COLORREF crNormalTextColor); 	


	//画百分比坐标
	virtual void DrawPrecentAxisStyle(CRect rRect,
							float* pStation,
							int nDiv,  
							AXISMODE nMode,
							COLORREF crLineColor, 
							COLORREF crNormalTextColor,
							float fBasePrice); 

 	//函数功能: 画出各类连接线,用于实时走势线、均线、其他各类连接线
 	virtual void DrawLinkLine(CRect rRect,
					  int nLineWidth, 
					  COLORREF clrLineColor,
					  int* pValues, 
					  int lValueNum,
					  BOOL bSelctState = FALSE);

	//函数功能：画出图形
	virtual BOOL Draw(CDC* pDC) = 0; 
protected:
 	//函数功能：比较得到最大最小值
 	void GetMaxMinPrice(float &fMinPrice,float& fMaxPrice,float *pData, int nDataNum,BOOL bFlash = TRUE);
protected:
	int m_nDecimal;           //指标纸显示数据的小数点位数

	float m_fMaxPrice;       //最大价格
	float m_fMinPrice;	     //最小价格
 
	int* m_pHoriPoint;		 //水平坐标点的位置
	int m_nHoriPoint;		 //水平坐标点个数

	CPoint m_CurrentPoint;   //当前位置点
	int m_nCurrentStation;   //当前光标点位置
	
	int m_nVAxisCount;       //垂直坐标数量
	int m_nValueNum;         //当前有效的值数量

	CRect m_rectDraw;        //有效画图区域
	CRect m_TotalRect;       //总区域
	int   m_nKLineWidth;     //K线宽度
	int	  m_nKLineSperator;  //K线间隔
	
	CDC* m_pDC;              //画图设备指针
	CFont * m_pFont;         //文字字体 
private:
	BOOL m_bActivate;		 //是否为当前激活状态
	int m_nVertCellHeight;   //单元格高度

protected:
	///////////////////////////////////////////////////////////
	float** m_ppCurveValue;	    //指标数据指针的指针
	int** m_ppCurveStation;     //数据的位置
	int	  m_nCurves;	        //数据量

	int*  m_pLineType;	        //指标画线类型，如K线、成交量等	
	int*  m_pParamValue;	    //指标参数指针	
	COLORREF* m_pColorValue;    //指标中的每条线的颜色配置	
	int	  m_nParamNum;	        //指标参数数量	

	void InitDataBuffer(int nDataSize);
	//清空所有数据
	void DestroyObj();

	//////////////////////////////////////////////////////////////////////////
	//
	float m_fVertZoomRaw;
};

#endif /* _INC_BASEPIC_409F013A0251_INCLUDED */
