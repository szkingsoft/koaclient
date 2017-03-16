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
	//删除所有画线
	void RemoveAllDrawLine();
	//////////////////////////////////////////////////////////////////////////
	//设置画线类型
	void SetDrawLineType(int nLineType);
	//////////////////////////////////////////////////////////////////////////
	//删除当前画线
	BOOL DeleteCurrentDrawLine();
	//////////////////////////////////////////////////////////////////////////
	//以下函数仅供K线使用接口
	//设置显示属性
	void SetShowStyle(DWORD dwStyle);
	//设置证券抬头信息
	void SetTitleInfo(CString szTitle,int nDataType);
	//////////////////////////////////////////////////////////////////////////
	//以下为公共接口
	CTechCreator();
	~CTechCreator();
	//////////////////////////////////////////////////////////////////////////
	//鼠标右键点下
	BOOL MouseRBDown(CPoint point);
	//////////////////////////////////////////////////////////////////////////
	//判断鼠标点击点的指标，设置为选择状态
	BOOL MouseDown(CPoint point);
	//鼠标移动
	BOOL MouseMove(CPoint point);
	//鼠标抬起，主要用于划线
	BOOL MouseUp(CPoint point);
	//更新数据
	virtual BOOL UpdateData(LPVOID pNewData);
	//画技术指标图形
	virtual BOOL Draw(CDC* pDC);
 	//设置画图区域
	virtual void MoveTo(CRect rRect);
	//测试当前位置
	int HitTest(CPoint point);
	//设置当前位置点
	void SetCurrentStation(int nStation);	
	//设置技术指标类型
	void SetTechType(CString nType);
	void UpdateTechNameTitle();
 	//得到指标类型
	CString GetTechType(){return m_szTechType;};
  	//设置水平标尺
	void SetHoriPoint(int* pHoriPoint,int nHoriPoint);
	//通过位置得到价格
	float GetPriceFromStation(int nStation);
	//通过价格得到位置
	int GetStationFromPrice(float nValue);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// add by david gong at 2014-08-13
	int GetDateFromStation(int nXStation);
	int GetStationFromDate(int nDate);
	void GetDrawRect(CRect& rect);
	double GetUnitLength();
	BOOL GetHighLowFromTime(const int & time,float &dHigh,float &dLow);
	void GetCloseFromXSgmt( const int &uBeginX,const int &uEndX, CArray< CPoint,CPoint&> &arPtClose );//获得指定坐标点内的所有收盘价
	BOOL GetHighLowFromXSgmt( const UINT uBeginX, const UINT uEndX, CPoint &ptHigh, CPoint &ptLow );
	//////////////////////////////////////////////////////////////////////////
 	//打开技术指标参数修改配置对话框
 	//得到最大参数点
	int GetMaxParam();	
	
	//清空所有数据
	void ResetData();

	//鼠标双击抬头
	int MouseDbClickTitle(CPoint pt);

	//画出当前图元框架
	void DrawTotalRect(CDC* pDC);

	//设置最大参数值
	void SetMaxParamer(int nMaxPar);
private:
	CTechAnalysisBasePic* m_pTechAnalysis;  //技术指标指针
	CString       m_szTechType;      //技术指标类型
};
#endif /* _INC_HISCURVEPIC_409F3A80028F_INCLUDED */
