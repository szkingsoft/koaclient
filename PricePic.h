// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PRICEPIC_409F013A02A5_INCLUDED
#define _INC_PRICEPIC_409F013A02A5_INCLUDED

#include "DynBasePic.h"

const int NOTITLEINFO     = 0X0001;//不画抬头
const int NORIGHTRAWRULER = 0x0002;//不画右侧比例尺
const int NOTIMERULER     = 0x0004;//不画下侧时间尺
const int TILELASTPRICE   = 0x0008;//抬头显示最后一笔
const int TITLEINDEXLAST  = 0x0010;//指数抬头
const int TITLELARGEFONT  = 0x0020;//抬头大字体
const int TITLEPRICEDAY   = 0x0080;//走势日期


class CPricePic: public CDynBasePic
{
public:	
	CPricePic();
	virtual ~CPricePic();
	//////////////////////////////////////////////////////////////////////////
	//设置画图模式
	void SetDisplayMode(int nShowMode){m_nShowMode = nShowMode;};
	//////////////////////////////////////////////////////////////////////////
	//重新计算位置
	virtual void RegetStation();
	//////////////////////////////////////////////////////////////////////////
	//设置画图区域
	virtual void MoveTo(CRect rRect,BOOL bAuto = TRUE);
	//////////////////////////////////////////////////////////////////////////
	//更新数据
	virtual BOOL UpdateData(LPVOID pNewData);
	//////////////////////////////////////////////////////////////////////////
	//画图
	virtual BOOL Draw(CDC* pDC);
	//////////////////////////////////////////////////////////////////////////
	//得到当前价格的位置
	virtual int GetStationFromPrice(long lPrice);

	void SetTradeTime(int nTradeTime){m_nTradeTime = nTradeTime;};
private:


	// 为了刻度显示正常，调整最大最小值
	void JustMaxMinValueForDispaly(long &lMaxValue,long &lMinValue,
		                           long lRefMidValue,int nVAxisCount,
								   long lUpLimitValue,long lDownLimitValue);

	//////////////////////////////////////////////////////////////////////////
	//格式化最高最低值
	void GetFormatMaxMin(long& lMaxPrice, long& lMinPrice, long lRefPos,int nVAxisCount);

	// 清除创建的内存数据
	void ClearData();

	//设置交易时间

	
private:	
	int m_nShowMode;//画图模式

	long m_lReference;//昨收价

	long* m_pPricePoints;//价格垂直位置指针

	long * m_pPrice;//价格指针

	CRect m_OrignRect;//原始区域

	int	  m_nTradeTime;
};

#endif /* _INC_PRICEPIC_409F013A02A5_INCLUDED */
