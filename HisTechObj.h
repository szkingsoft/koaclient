// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_HISTECHOBJ_409F3AB302ED_INCLUDED
#define _INC_HISTECHOBJ_409F3AB302ED_INCLUDED

#include "BaseDrawObj.h"
#include "techcreator.h"
#include "InfoTable.h"
#include "distractstattable.h"

#define	TP_FIRST	1    //定义历史数据请求类型第一次
#define	TP_NEXT		2    //定义历史数据请求类型除第一次之外的类型
#define	TP_END		3    //已经请求完所有历史数据

//Function:技术分析对象
//Author:  gongxiaofeng
class CHisTechObj: public CBaseDrawObj
{
public:
	void SetDrawLineCallBcakFun();
	void SetCurSymbol(CString strSymbolCode);
public:	
	enum REQUESTDATATYPE
	{
		NORMALREQUEST,//正常请求
		LEFTMOVEREQUEST,//左移请求
		CTRLLEFTMOVEREQUEST,//Ctrl+左移请求
		FIRSTGETDATA,
		ZOOMINREQUEST
	};
	
	enum DISTACTSTATSTATE
	{
		BEGINSTAT,//开始统计
		FIRSTCLICK,//第一次点击
		SECONDCLICK,//第二次点击
		ENDSTAT//结束统计
	};//区域统计状态
 	//////////////////////////////////////////////////////////////////////////

	CHisTechObj();
	~CHisTechObj(); 
	/////////////////////////////////////////////////////////////////////////
	//请求地雷索引数据
	//计算子区域大小
	virtual void RecalSubObjSize(int iRightW = 50);
	//设置商品信息
	void SetStockInfo(const SCodeInfo* pCodeInfo);
	//设置证券代码和周期信息到子图
	void SetStockNameToPic();
	//////////////////////////////////////////////////////////////////////////
    //请求数据
	void RequestData();
	void RequestData_ZYQ();
	/////////////////////////////////////////////////////////////////////////
	//传递数据
	virtual BOOL UpdateData(LPVOID pData,  int nDataSize);
	/////////////////////////////////////////////////////////////////////////
	//画图
	virtual BOOL Draw(CDC* pDC);
	/////////////////////////////////////////////////////////////////////////	
	//函数功能：响应菜单消息
	BOOL OnUserCmd(UINT iSel);
	BOOL OnUpdateUserCmd(CCmdUI *pCmdUI);
	
	// 初始化K线周期类型
	void InitCircleType();
	
	// 当前K线周期类型是否为初始状态
	BOOL IsInitCircleType();
	int GetCurKLinePriodID(){return m_nCurKLinePriodID;}

public://消息响应
	BOOL TreatDayDataArrived(const char* pHisotyData);//处理日线到达消息
	BOOL TreatDayDataArrived_ZYQ(const char* pHisotyData);//处理日线到达消息
	void SetFormulaCommand(char* pCommand);//设置指标命令
	void ResetTechData();//重置指标状态
	void DestoryData();//删除数据
	//////////////////////////////////////////////////////////////////////////
	//函数功能：响应键盘事件 
	virtual BOOL OnChar(int nChar);	 
	//鼠标响应
	virtual BOOL HitTest(CPoint point);
	BOOL MouseLBtnUp(CPoint point);
	BOOL MouseLBtnDown(CPoint point);
	BOOL MouseLBtnDbClick(CPoint point);
	BOOL MouseMove(CPoint point);
	BOOL MouseRBtnDown(LPVOID lpWnd,CPoint point);
	BOOL MouseRBtnUp(CPoint point);

	CTechCreator* GetPic(int nPic) {return m_pHisCurvePic+nPic;};
private:
	//////////////////////////////////////////////////////////////////////////
	//Ctrl 左移
	void CtrlRightMove();
	//Ctrl 右移
	void CtrlLeftMove();
	//右移
	void RightMove();
	//左移
	void LeftMove();
 	//调整K线宽度
	void AdjustKUnitWidth();
	//设置水平坐标点
	void SetHoriPoint();
	//视图放大
	void ZoomOutData();
	//视图缩小
	void ZoomInData();

	//////////////////////////////////////////////////////////////////////////	
	//传递当前位置给子图形
	void TransCurStationToPic();	
	//传输数据到子图
	void TransKDataToPic();
	//更新历史视图中子图形所需的数据
	//////////////////////////////////////////////////////////////////////////
	//画出十字光标
	void DrawCrossCursor(CDC* pDC); 	
	//画出时间区间
	void DrawTimeRect(CDC* pDC);	
	// 根据最新的数据更新k线的对应值
	void UpdateKLineMinPriceByNowData(float &rfOldValue,float fNewValue);

	void ReAssignPicNum(int nNewPicNum);
	void CalcDistractInfo();
	void DrawDistractLine(CDC* pDC);

	BOOL MakeDayData(StuRecvQuoteInfo* pQuote);
	const SHistoryUnit* GetCurrentKUnit();
	void DeleteIndex();
	
    // 如果当前K线周期是大于日的，则需要将当天服务K线合并到历史K线中去
	BOOL CombineTodayKLine(unsigned char usKLinePriod,const SHistoryUnit*  pTodayKUnit,
					   SHistoryUnit* pPreHisKUnit);

	// 调整K线数据为正常显示所需
	void AdjustKLineData2Valid(SHistoryUnit& rNowKUnit,const SHistoryUnit* pAllKUnitList,int nNowKUnitIndex);
	
    // 保存指标状态参数
	BOOL SaveIndexStatusParam();
	// 装载指标状态参数
	BOOL LoadIndexStatusParam();
	
	// 获取当前十字光标所在位置的价格
	BOOL GetCurCrossCursorPosValue(CString &rstrFormateValue,float &rfValue);

	//从服务器接受的数据或设置数据变量////////////////////////////////////////
	const SCodeInfo*	m_pCodeInfo;			 //商品信息
	int					m_nCircleInfo;			 //周期信息	
	SHistoryUnit*		m_pRecvKUnit;			 //已从服务器请求过来的历史数据指针
	SHistoryUnit*		m_pDoneKunit;			 //处理过的K线用于合并K线和处理除权使用
	SHistoryUnit*		m_pTranslateKUnit;		 //传输用K线
	BOOL				m_bTranslateIsPoint;		 //传输是否为指针
	int					m_nValueNum;			     //已从服务器请求过来的历史数据个数	

	short				m_sRequestStatus;		 //请求数据状态，分第一次和第一次以后请求	
	short				m_sLastSize;			     //最近一次请求K线数据个数
	REQUESTDATATYPE     m_nLeftMoveRequestStatus;	//左移状态
	
	int m_nDrawLineState;			//画线状态
	
	//历史视图状态改变变量
	////////////////////////////////////////////////////
	int		   m_nDataBegin;			 //数据开始位
	int		   m_nDataEnd;				 //数据结束位
	int		   m_nDataOffset;			 //数据偏移量
	int		   m_nKUnitWidth;			 //当前K线宽度
	float	   m_fCurrentWidth;			 //当前K线分割宽度（m_nKUnitWidth + k线之间的间隙）	
	int	       m_nRecsOfUnit;			 //一根K线上包含的K线数量
	int        m_nCanShowMaxKUnits;      //当前能显示的最大K线数量

	//历史数据显示变量////////////////////////////////////////////////////////
	CRect	   m_TimeRect;			      //时间区域
	int	       m_nTotalPicNum;		      //图形数量
 	CRect	   m_TipInfoRect;			  //信息框区域
	CRect	   m_KLineRect;			      //K线区域
	BOOL	   m_bMouseMove;			  //鼠标移动	
	BOOL	   m_bLeftButtonDown;	      //鼠标左键按下	
	CPoint	   m_DrapPoint;				  //拖动点
	int		   m_nVertStation;			  //当前鼠标垂直位置
	int		   m_nHoriStation;			  //当前鼠标水平位置
	int		   m_nNowShowDate;			  //当前鼠标表示日期 add by gongxiaofeng at 2004-7-23
	int*       m_pHoriPoint;			  //水平坐标点的位置	
	int        m_nHoriPoint;              //水平坐标点数
	int        m_nMaxParameret;	          //技术指标参数中的最大值
	int		   m_nParameret;			  //当前偏移位置值
	
	//子对象//////////////////////////////////////////////////////////////////
	CTechCreator*     m_pHisCurvePic;     //历史曲线
	CInfoTable        m_InfoTable;		  //对应日期的K线数据显示表格

 	DWORD								m_dwShowStyle;//显示属性
	int									m_nCurrentPicNum;//图形数量
//	BOOL								m_bRequestedData;//请求数据状态
	//区域统计用
	CDistractStatTable* m_pDistractStatTable;//统计表格
	DISTACTSTATSTATE					m_nDistractStatState;//区域统计状态
	int									m_nFirstClickState;//第一次点击位置
	int									m_nSecondClickState;//第二次点击位置		
	int									m_nCurrentTechIndex;
private://INI文件读写盘
	CString		m_strIndexParamPath;  //指数参数文件路径
	int			m_nCurKLinePriodID;//当前K线周期类型对应的资源ID
	BOOL		m_bDrawCross;//是否画出十字线
	BOOL		m_bMouseInRect;//鼠标是否在当前点内
	int			m_nDrawState;	//画线状态
	BOOL		m_bChipDistribPic;//移动筹码显示状态

	int			m_nMemoryStation;//当前记忆位置

private://叠加K线用
	void CtrlDownZoom();
	void CtrlUpZoom();
	int			m_nRequestSize;//请求大小
	int			m_nMemorySize;//记忆中的请求大小
	BOOL		m_bAddDataEnd;//请求数据是否结束
private:
	int FindStationFromDate(int nDate);
	void DrawSelectArea(CDC* pDC);//画出选择区间

	CRect m_rectSelect;
	BOOL m_bDrawSelect;
	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 20140806
	CString	m_strDefaultIndex;//默认指标

	BOOL m_bMousePress;
	CPoint m_ptCurPress;
};

#endif /* _INC_HISTECHOBJ_409F3AB302ED_INCLUDED */
