// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_DYNBASEPIC_409F013A0251_INCLUDED
#define _INC_DYNBASEPIC_409F013A0251_INCLUDED

const int AUTOFITLEFTSIZE = 0x0040;//自动适应左边距
class CDynBasePic 
{
public:
	enum AXISMODE
	{
		DOUBLESIDEDOUBLECOLOR,//双边双色
		DOUBLESIDESINGLECOLOR,//双边单色
		SINGLESIDEDOUBLECOLOR,//单边双色 
		SINGLESIDESINGLECOLOR,//单边单色
		HISTORYAXISMODE		  //历史数据坐标
	};//坐标画图模式
	CDynBasePic();
	virtual ~CDynBasePic();
	
	//函数功能:设置当前光标位置
	void SetCurrentStation(int iStation);
	

	//函数功能:得到当前光标位置
	virtual int GetCurrentStation(CPoint point);

	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 2013/09/17
	//增加十字线价格功能
	void SetCurrentPrice(long lPrice);

	//////////////////////////////////////////////////////////////////////////
	
	// 获取制定位置的x坐标
	int GetHoriPointXByStation(int iStation);

	// 获取单元列的宽度
	float GetHoriPointWidth();

	//函数功能:重新计算各点位置
	virtual void RegetStation(){};

	//函数功能: 画出条状线信息
	void DrawBarLine(CRect rRect, 
					int nLineWidth, 
					long lMaxValue,
					long lMinVlaue, 
					long *pValues, 
					long lValueNum,
					COLORREF crRiseTextColor,
					COLORREF crDownTextColor,
					int nBaseValue = 0);

	//函数功能: 得到垂直坐标线格数
	int GetVertAxisCellNum(int iHeight);

	//函数功能:画出各类坐标线
	void DrawAxisStyle(CRect rRect,
					 long lMinValue, 
					 long lMaxValue,
					 int nDiv, 
					 COLORREF crLineColor, 
					 COLORREF crNormalTextColor,
					 long* pXSign = NULL,
					 long  nSignNum = 0,
					 long  nHotSign = 0);

//函数功能: 设置价格小数点位数
	inline void SetDecimal(int iDecimal,int nPow){m_iDeimal = iDecimal;m_nPow = nPow;};

	//函数功能: 画出各类连接线,用于实时走势线、均线、其他各类连接线
	void DrawLinkLine(CRect rRect, int nLineWidth, COLORREF clrLineColor, long lMaxValue, long lMinVlaue, long* pValues, long lValueNum);

	//函数功能: 画出各类条状图，用于成交量的画图
	void DrawVolumnBar(CRect rRect, int nLineWidth, long lMaxValue, long lMinValue, long* pValues, long* pPrice,
		                long lValueNum, COLORREF clrRiseColor, COLORREF clrDownColor, int iRefPos = 0);

	//函数功能: 设置横坐标点
	virtual void SetHoriPoint(int iHoriPoint);

	//函数功能: 更新数据
	virtual BOOL UpdateData(LPVOID pNewData) = 0;

	//函数功能: 测试点是否在区域内 
	virtual BOOL HitTest(CPoint point);

	//函数功能：画图
	virtual BOOL Draw(CDC* pDC) = 0;

	//函数功能：画图图形上侧的文本信息
	virtual void DrawTitleText();

	//函数功能：设置当前字体
	CFont* SetFont(CFont* pFont);

	//函数功能：设置画图区域
	virtual void MoveTo(CRect rRect);

	//函数功能：设置单元格高度
	virtual void SetItemHeight(int nHeight){m_nVertCellHeight =nHeight; };
	
	//函数功能：从位置得到价格
	long GetPriceFromStation(int nStation);		
	
	//函数功能：画出整个区域
	void DrawTotalRect(COLORREF clrLine);

	virtual int GetStationFromPrice(long lPrice) =0;
protected:
	//函数功能：画当前光标线
	virtual void DrawCurrentStationLine();
	//////////////////////////////////////////////////////////////////////////
	//函数功能：比较得到最大最小值
	void GetMaxMinPrice(long &lMinPrice,long& lMaxPrice,long *pData, long nDataNum,BOOL bFlash = TRUE);
protected:
	int	   m_iDeimal;			//小数点位数
	int    m_nPow;				//除以的比例
	long   m_lMaxPrice;			//最大价格
	long   m_lMinPrice;			//最小价格 
	
	long*  m_pHoriPointStation; //水平坐标点的位置
	long   m_lHoriPoint;		//水平坐标点数 
	long   m_nVertCellHeight;	//单元格高度
	int    m_nVAxisCount;       //垂直坐标数量
	CPoint m_CurrentPoint;		//当前位置点
	long   m_nCurrentStation;   //当前光标点位置
	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 2013/09/17
	//增加了十字线的价格
	long  m_lCurrentPrice;//当前价格
	//////////////////////////////////////////////////////////////////////////
	int    m_nValueNum;		    //当前有效的值数量	
protected:	
	CRect  m_TotalRect;			//总区域
	CRect  m_rectDraw;		    //有效画图区域	 
	CDC*   m_pDC;			    //画图设备指针 
	CFont* m_pFont;			    //文字字体
};

#endif /* _INC_DYNBASEPIC_409F013A0251_INCLUDED */
