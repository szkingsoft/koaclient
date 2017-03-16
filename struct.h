
/////////////////////////////////////////////////////////////////////////
//实现功能：定义了程序中所需要使用的各种结构体
//
//所有价格要除于1000
//所有成交金额都以万元为单位
/////////////////////////////////////////////////////////////////////////
#ifndef INC_STRUCT_H
#define INC_STRUCT_H
#include "quotestruct.h"
///////////////////////////////////////////////////////////////
#pragma pack(1)
struct SHistoryUnit
{
	unsigned int		m_uiDate;
	float	m_fOpenPrice;
	float	m_fMaxPrice;
	float	m_fMinPrice;
	float	m_fClosePrice;
	unsigned	int		m_uiVolume;
	unsigned	int		m_uiSum;//成交金额
};//本地内存历史走势


//当日证券成交数据，用于画价格线和成交量线
struct	SDynamicMinsUnit//以分钟为单位生成
{
	unsigned int		m_uiTime;//距离第一次开盘时间
	float	m_fNewPrice;
};
#pragma pack()
#endif