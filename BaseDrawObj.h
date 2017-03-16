// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_BASEDRAWOBJ_409F013A0320_INCLUDED
#define _INC_BASEDRAWOBJ_409F013A0320_INCLUDED

const long	VERTBANKSIZE = 90;
const long  VERTBANKSIZE_HISOBJ = 130;
const long  HORIBANKSIZE = 18;
class CBaseDrawObj
{
public:
	int m_iType;
	void SetType(int iType = 0){m_iType = iType;}
	int GetType(){return m_iType;}
public:
	void ActivateObj(BOOL bActivate = TRUE);
	//////////////////////////////////////////////////////////
	//计算子区域大小
	virtual void RecalSubObjSize(int iRightW = 50) = 0;

	//////////////////////////////////////////////////////////
	//设置画图区域
	virtual void MoveTo(CRect rRect);

	//////////////////////////////////////////////////////////
	//传递数据
	virtual BOOL UpdateData(LPVOID pData, int nDataSize) = 0;

	//测试点击位置
	virtual BOOL HitTest(CPoint point) = 0;

	//////////////////////////////////////////////////////////
	//画图
	virtual BOOL Draw(CDC* pDC) = 0;

	CBaseDrawObj();

	virtual ~CBaseDrawObj();

	//函数功能：得到画图对象的名称
	//函数参数：返回的对象名称
	//函数返回：成功为TRUE,错误为FALSE
	//函数异常：无
	BOOL GetObjName(LPCTSTR lpszName){lpszName = m_szObjName;return TRUE;};

	//函数功能：设置对象名称
	//函数参数: 对象名称
	//函数返回:无
	//函数异常：无
	void SetObjName(LPCTSTR lpszName){m_szObjName = lpszName;};

	//函数功能：响应键盘事件
	//函数参数：nChar 键盘按键
	//函数返回：无　
	virtual BOOL OnChar(int nChar) = 0;

	//函数功能：弹出右键菜单
	//函数参数：pWnd - 窗口指针；point - 鼠标点击点
	//          uiPopMenuID - 菜单ID；iSel - 菜单序号
	//函数返回：选中菜单ID值
	virtual UINT PopupRBtnMenu(CWnd* pWnd,CPoint point,UINT uiPopMenuID,int iSel);

	// 响应系统菜单、工具栏的命令
	virtual BOOL OnUserCmd(int nSel){return FALSE;}

	//　设置系统菜单、工具栏具体项的显示状态
	virtual BOOL OnUpdateUserCmd(CCmdUI *pCmdUI){return FALSE;}


protected:
	//画图区间
	CRect m_DrawRect;

private:
	//对象名称
	CString m_szObjName;
protected:
	//是否为当前激活状态
	BOOL m_bActivate;
};

#endif /* _INC_BASEDRAWOBJ_409F013A0320_INCLUDED */
