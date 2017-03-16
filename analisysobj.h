// AnalisysObj.h: interface for the CAnalisysObj class.
#if !defined(AFX_ANALISYSOBJ_H__2DF68C4A_7D7E_4A86_88C5_50BC0A5B5D71__INCLUDED_)
#define AFX_ANALISYSOBJ_H__2DF68C4A_7D7E_4A86_88C5_50BC0A5B5D71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "koaclient.h"
#include "BaseDrawObj.h"
#include "pricevolobj.h"
#include "histechobj.h"
#include "TypeCtlPic.h"

class CAnalisysObj : public CBaseDrawObj
{
public:
	void SetType(int iType);
	void SetDrawLineCallBcakFun();
public:
	BOOL MouseRBtnUp(CPoint point);
	void SetFormulaCommand(char* pCommand);
	CAnalisysObj();
	virtual ~CAnalisysObj();

	//函数功能：设置证券信息
	//函数参数：pCodeInfo 证券信息
	//函数返回：无
	void SetStockInfo(const SCodeInfo *pCodeInfo);
	/////////////////////////////////////////////////////////
	//函数功能：初始化环境
	//函数参数：无
	//函数返回：无
	void Initialization();
	//////////////////////////////////////////////////////////

	//函数功能：计算子区域大小
	//函数参数：无
	//函数返回：无
	virtual void RecalSubObjSize(int iRightW=50);
	//////////////////////////////////////////////////////////

	//函数功能：传递行情数据
	//函数参数：pData 数据缓冲区指针；nDataSize 数据数量
	//函数返回：TRUE/FALSE
	virtual BOOL UpdateData(LPVOID pData, int nDataSize);
	//////////////////////////////////////////////////////////

	//函数功能：测试点击位置
	//函数参数：point 鼠标点击点
	//函数返回：TRUE - 点中；FALSE - 未点中
	virtual BOOL HitTest(CPoint point);
	//////////////////////////////////////////////////////////

	//函数功能：键盘响应函数
	//函数参数：nChar 按键ID
	//函数返回：TRUE/FALSE
	virtual BOOL OnChar(int nChar);
	//////////////////////////////////////////////////////////
	//函数参数：鼠标点击点
	//函数返回：TRUE/FALSE
	virtual int/*BOOL*/ MouseLBtnDown(CPoint point);
	//函数参数：鼠标点击点
	virtual BOOL MouseMove(CPoint point);
	//函数参数：鼠标点击点
	virtual BOOL MouseLBtnUp(CPoint point);
	//函数功能：鼠标左键双击
	virtual BOOL MouseLBtnDbClick(CPoint point);
	//函数功能：响应鼠标右键点击消息
	virtual BOOL MouseRBtnDown(LPVOID lpWnd,CPoint point);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：画图
	virtual BOOL Draw(CDC* pDC);
	//////////////////////////////////////////////////////////
	//函数功能：设置画图模式
	void SetAnalisysMode(int nDrawMode,BOOL bRequstData = TRUE);
	//函数功能：设置画图模式
	int GetAnalisysMode() { return m_nDrawMode;}
	
	int GetCurKLinePriodID(){return m_HisTechObj.GetCurKLinePriodID();}

	//函数功能：判断画图模式是否在数组中
	BOOL IsExistDrawMode(int nDrawMode);
	//函数功能：设置画图模式并设置类型控制栏焦点
	void SetModeAndFocus(int nSel,BOOL bRequestData = TRUE);
	//函数功能：返回前一个画图模式
	int GetPrevDrawMode();
	//函数功能：设置画图模式
	void SetDrawMode(int nDrawMode);
	//函数功能：重新初始化表格
	void ReInitTable();

	// 响应系统菜单、工具栏的命令
	BOOL OnUserCmd(int nSel);	
	BOOL OnUpdateUserCmd(CCmdUI *pCmdUI);
private:
	const SCodeInfo* m_pCodeInfo;//个股信息结构
	int           m_nDrawMode;    //画图模式
	CUIntArray    m_ArrDrawMode;  //画图模式数组
	CUIntArray    m_ArrDrawMode2; //画图模式数组2

	CPriceVolObj  m_PriceVolObj;  //个股视图对象
	CHisTechObj	  m_HisTechObj;	  //技术分析对象
	CTypeCtlPic	  m_TypeCtlPic;  //类型控制栏
	int			  m_nSel;//当前选择
};

#endif // !defined(AFX_ANALISYSOBJ_H__2DF68C4A_7D7E_4A86_88C5_50BC0A5B5D71__INCLUDED_)