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
		HISTORYAXISMODE		  //历史数据坐标
	};//坐标画图模式
	CBaseHisPic();
	virtual ~CBaseHisPic();
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
	virtual long GetPriceFromStation(int nStation);
	//通过指标值得到位置
	int GetStationFromPrice(int nValue);
	//函数功能: 得到垂直坐标线格数
	int GetVertAxisCellNum(int iHeight);
	//函数功能: 设置指标显示值的小数点位数
	inline void SetDecimal(int iDecimal){m_iDeimal = iDecimal;};
	//////////////////////////////////////////////////////////////////////////
 	//画出整个区域
	void DrawTotalRect(CDC* pDC);
  	//函数功能: 画出条状线信息
 	void DrawBarLine(CRect rRect, int nLineWidth, int lMaxValue, int lMinVlaue, int *pValues, int lValueNum,COLORREF crRiseTextColor,COLORREF crDownTextColor,int nBaseValue = 0);
   	//函数功能:画出各类坐标线
 	void DrawAxisStyle(CRect rRect,
					 int lMinValue, 
					 int lMaxValue,
					 float* pStation,
					 int nDiv, 
					 int nDecimal, 
					 AXISMODE nMode,
					 COLORREF crLineColor, 
					 COLORREF crNormalTextColor); 	
 	//函数功能: 画出各类连接线,用于实时走势线、均线、其他各类连接线
 	void DrawLinkLine(CRect rRect,
					  int nLineWidth, 
					  COLORREF clrLineColor,
					  int lMaxValue, 
					  int lMinVlaue, 
					  int* pValues, 
					  int lValueNum,
					  BOOL bSelctState = FALSE
					  );
  	//函数功能: 画出各类条状图，用于成交量的画图
	//函数功能：画出图形
	virtual BOOL Draw(CDC* pDC) = 0; 
protected:
 	//函数功能：比较得到最大最小值
 	void GetMaxMinPrice(long &lMinPrice,long& lMaxPrice,int *pData, int nDataNum,BOOL bFlash = TRUE);
protected:
	int m_iDeimal;           //指标纸显示数据的小数点位数

	long m_nMaxPrice;         //最大价格
	long m_nMinPrice;	     //最小价格
 
	int* m_pHoriPointStation;//水平坐标点的位置
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
	float m_fVertZoomRaw;	//纵向压缩比率

};

#endif /* _INC_CBASEPIC_409F013A0251_INCLUDED */
