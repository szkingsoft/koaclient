// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_INFOTABLE_409F39320241_INCLUDED
#define _INC_INFOTABLE_409F39320241_INCLUDED

#include "BaseTable.h"

//Function:对象显示值表格
class CInfoTable : public CBaseTable
{
public:
	CInfoTable();
	virtual ~CInfoTable();

// 	enum INFOMODE
// 	{
// 		MINUTEDATAINFO,//分钟数据信息
// 		DAYDATAINFO//日线数据信息
// 	};
	//设置数据传输模式
//	void SetMode(INFOMODE nMode){m_nInfoMode = nMode;};
	void SetMode(int nCircleInfo);
	virtual void Draw(CDC* pDC);

	//函数功能：用于初始化列和设置属性，所有基层类必须重载此函数
	virtual BOOL Init();

	//BOOL Init(INFOMODE nInfoMode);
	BOOL Init(int nCircleInfo);

	//函数功能：传递数据，所有继承类必须重载此接口来设置数据
	 virtual BOOL UpdateData(LPVOID lpData,int nDataLen);
	 
	
	//设置证券信息
	void SetStockInfo(const SCodeInfo* pCodeInfo);
	//设置最新价格
	void UpdateNewPrice(float fNewPrice){m_fNewPrice=fNewPrice;};
private:
	//更新日线数据
	void UpdateDayInfo(SHistoryUnit* pKUnit,float fLastClose,float fCrossCursorPosValue, int nCircleInfo);
	//更新分钟线数据
	void UpdateMinuteInfo(SDynamicMinsUnit* pRealMinute,float fLastClose);
//	INFOMODE	m_nInfoMode; //表格接受数据模式
	int m_nCircleInfo;
	int			m_nSystemStyle;

	CString m_szFormat;      //价格格式化字串
	SCodeInfo  m_oCodeInfo; //证券信息
	float		   m_fNewPrice; //最新价格

public:
	BOOL UpdateDataFromHis(LPVOID lpData,int nDataLen);    //技术分析来的数据的显示  add by witming
};

#endif /* _INC_INFOTABLE_409F39320241_INCLUDED */
