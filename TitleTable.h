// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_TITLETABLE_409F394502FC_INCLUDED
#define _INC_TITLETABLE_409F394502FC_INCLUDED

#include "BaseTable.h"

class CTitleTable : public CBaseTable
{
public:
	CTitleTable();

	virtual ~CTitleTable();

	//设置证券信息
	void SetStockInfo(const SCode* pStockInfo);

	//处理标题数据

	//函数功能：用于初始化列和设置属性，所有基层类必须重载此函
	//数
	//函数参数：无
	//函数返回:成功为TRUE,错误为FALSE
	//函数异常:无
	virtual BOOL Init();

	//函数功能：传递数据，所有继承类必须重载此接口来设置数据
	//函数参数：lpData 多态数据流
	//函数返回:成功为TRUE,错误为FALSE
	//函数异常:无
	virtual BOOL UpdateData(LPVOID lpData);
	//绘图
	void Draw(CDC* pDC);

private:
	SStockInfo m_StockInfo; //证券信息
};

#endif /* _INC_TITLETABLE_409F394502FC_INCLUDED */
