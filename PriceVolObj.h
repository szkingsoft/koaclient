// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PRICEVOLOBJ_409F013A0330_INCLUDED
#define _INC_PRICEVOLOBJ_409F013A0330_INCLUDED

#include "BaseDrawObj.h"
#include "PricePic.h"
#include "InfoTable.h"

class CSinglockGuard
{
public:
	CSinglockGuard(CCriticalSection* pSinglock)
	{
		if(NULL == pSinglock)return;
		m_pSinglock = pSinglock;
//		m_pSinglock->Lock();
	}
	~CSinglockGuard()
	{
		if(NULL == m_pSinglock)return;
//		m_pSinglock->Unlock();
	}
private:
	CCriticalSection* m_pSinglock;
};

static CArray<SCodeInfo,SCodeInfo&> m_CodeInfoArray;
class CPriceVolObj: public CBaseDrawObj
{
public:
	BOOL  TransStation(CPoint point);

	CPriceVolObj();
	virtual ~CPriceVolObj();
 	//////////////////////////////////////////////////////////////////////////
  	//函数功能：重新分配子区域
	virtual void RecalSubObjSize(int iRightW = 50);
	//函数功能：设置画图模式，nShowMode走势图类型
	void SetDisplayMode(DWORD nShowMode);
	//函数功能：设置画图模式
	BOOL SetRealTechType(int nTechType);
	int GetRealTechType();
 	//函数功能：在指定设备画图
 	virtual BOOL Draw(CDC* pDC);    
	//////////////////////////////////////////////////////////////////////////
	//函数功能：仅请求只有当前股票的走势数据
	void RequestData();
	void RequestData_SP();
	//函数功能：传输证券信息
 	void SetStockInfo(const SCodeInfo* pCodeInfo);
	//函数功能：处理更新数据
 	virtual BOOL UpdateData(LPVOID pData,int dwDataSize = 0);			
	//////////////////////////////////////////////////////////////////////////
	//函数功能：响应键盘事件
 	virtual BOOL OnChar(int nChar);
	//函数功能：响应用户ID消息
	BOOL OnUserCmd(UINT iSel);
	//函数功能：鼠标移动消息
	//函数功能：测试鼠标点击位置
	virtual BOOL HitTest(CPoint point);
	
	BOOL MouseLBtnDbClick(CPoint point);
	//函数功能：响应鼠标移动消息
	BOOL MouseMove(CPoint point);
	//函数功能：响应鼠标左键弹起消息
	BOOL MouseLBtnUp(CPoint point);  
	//函数功能：响应鼠标左键按键消息
	BOOL MouseLBtnDown(CPoint point);
	//函数功能：响应鼠标右键点击消息
	BOOL MouseRBtnDown(LPVOID lpWnd,CPoint point);

private:
	//画提示信息
	void DrawTip(CDC *pDC,CPoint point,CString strTip);
	
	//画出抬头信息
	void DrawTitleInfo(CDC* pDC);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：根据交易时间分配各个图元的数据内存
	void SetHoriPoint();
	//是否请求过SCodeInfo
	BOOL IsExistCodeInfo(SCodeInfo codeInfo);	
 	//函数功能：画时间刻度
	void DrawTimeRect(CDC* pDC); 	

	void DrawCrossCursor(CDC *pDC);
	//////////////////////////////////////////////////////////////////////////
	// 针对特殊情况，调整分钟数据至有效
	void AdjustMinsData2Valide();
 	//函数功能：更新成交价、成交量图元数据
	void UpdatePicData();	
	//函数功能：更新图元显示信息框数据
	void UpdateInfoTip();
	
	
	int  GetHoriPointTime(int nPos);

	int  GetHoriPoint(int nTime);

private:
	//////////////////////////////////////////////////////////////////////////
	CRect m_PriceRect;           //成交价区域
	CRect m_VolumnRect;			 //成交量区域
	CRect m_TechRect;			 //指标区域
	CRect m_TimeRect;			 //时间坐标区域
	CRect m_TipInfoRect;		 //信息条区域
	CRect m_TitleRect;			 //抬头区域
	BOOL  m_bMouseMove;			 //鼠标移动
	BOOL  m_bLeftButtonDown;	 //鼠标左键按下
	CPoint m_DrapPoint;		 	 //拖动点	
	//////////////////////////////////////////////////////////////////////////
	long* m_pPriceValues;        //成交价值
	//////////////////////////////////////////////////////////////////////////
 	int   m_nHoriPoints;		 //总交易时间
	int   m_nValueNum;			 //有效数据数量
 	int   m_nRealTechType;		 //当前技术指标画图类型 
 	int   m_nShowMode;			 //显示模式
	int   m_nCurrentStation;	 //当前鼠标位置
	int   m_nCurrentPrice;//当前鼠标价格位置
private:
	//////////////////////////////////////////////////////////////////////////
	CPricePic    m_PricePic;	       //成交价图元	
	CInfoTable   m_InfoTable;          //信息表格
	const SCodeInfo*  m_pCodeInfo;   //证券信息
//	float m_fPrevClose;        //当前商品的昨收价
	int m_iDate;                       //交易日期
	BOOL		 m_bDrawCross;		   //画出光标

	 CCriticalSection		m_lockData;
	list<STraceUnit>	m_listTraceInfo;//成交信息
};

#endif /* _INC_PRICEVOLOBJ_409F013A0330_INCLUDED */
