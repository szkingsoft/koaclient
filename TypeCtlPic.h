// TypeCtlPic.h: interface for the CTypeCtlPic class.
#if !defined(AFX_TYPECTLPIC_H__ED4F4C79_F077_4E7D_AE39_50A83A45E868__INCLUDED_)
#define AFX_TYPECTLPIC_H__ED4F4C79_F077_4E7D_AE39_50A83A45E868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////
//类名：    类型控制栏基类
//////////////////////////////////////////
struct STypeCtlUnit //类型控制栏单元
{
	CString m_szUnitText;   //单元文本
	CRect   m_rtUnitRect;   //单元矩形
	//int     m_iMarginWidth; //单元边距
	bool    m_bFocus;       //是否被激活

public:
	STypeCtlUnit()
	{
		m_szUnitText.Empty();
		m_rtUnitRect.SetRectEmpty();
		//m_iMarginWidth = 0;
		m_bFocus = false;
	}
};
typedef CArray<STypeCtlUnit,STypeCtlUnit&> CTypeControl;

class CTypeCtlPic
{
public:
	CTypeCtlPic();
	virtual ~CTypeCtlPic();

	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置画图区间
	//函数参数：rRect 画图区间
	//函数返回: 无
	//函数异常: 无
	virtual void MoveTo(CRect rRect);
	//////////////////////////////////////////////////////////////////////////
	//函数功能: 得到鼠标点击位置
	//函数参数：pt 鼠标点击点
	//函数返回：成功则返回当前选中单元序号，否则返回 -1
	//函数异常：无
	virtual int HitTest(CPoint pt);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置单元被激活(选中)/不激活(不选中)
	//函数参数：iUnit 单元号；bFocus 是否被激活
	//函数返回：无
	virtual void SetUnitFocus(int iUnit, bool bFocus = false);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：被回调，激活控制栏中的单元
	//函数参数：iSel对应的激活单元
	//函数返回：无
	void OnSelectFocus(int iSel);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：插入单元
	//函数参数：lpszText 单元文本；bFocus 是否被激活
	//函数返回: 成功为TRUE,错误为FALSE
	virtual BOOL InsertUnit(LPCTSTR lpszText,bool bFocus = false);
	//函数功能：画出图形
	//函数参数：pDC 画图设备指针
	//函数返回：无
	virtual BOOL Draw(CDC* pDC);
	//函数功能：计算单元矩形大小
	//函数参数：pDC 画图设备指针；iUnit 单元序号
	//函数返回：单元宽度
	virtual int ComputeUnitRect(CDC* pDC,int iUnit);
	//函数功能：绘制单元图形
	//函数参数：pDC 画图设备指针；iUnit 单元序号
	//函数返回：无
	virtual void DrawUnit(CDC* pDC,int iUnit);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：类型控制栏初始化
	//函数参数：无
	//函数返回: 成功为TRUE,错误为FALSE
	virtual BOOL Init(){return TRUE;};
	//函数功能：响应鼠标点击
	//函数参数：point 鼠标点
	//函数返回：TURE 重画；FALSE 不重画
	virtual BOOL MouseDown(CPoint point);
	//函数功能：获取当前选中页签序号
	//函数参数：无
	//函数返回：当前选中页签序号
	virtual int GetSelTabID();
	//函数功能：设置当前选中页签序号
	//函数参数：iID 页签序号
	//函数返回：无
	virtual void SetSelTabID(int iID);
	//函数功能：获取类型控制栏左上角坐标
	//函数参数：无
	//函数返回：左上角坐标
	virtual CPoint GetTopLeftPt();
	//函数功能：获取类型控制栏右上角坐标
	//函数参数：pDC 设备DC
	//函数返回：右上角坐标
	virtual CPoint GetRightTopPt(CDC* pDC);
	//函数功能：设置颜色和字体
	virtual void SetColorsAndFonts();
	//函数功能：设置当前选中页签文本
	virtual void SetSelTabText(LPCTSTR lpszText);
	//函数功能：获取选中页签文本
	virtual CString GetSelTabText(CPoint point);
	//函数功能：清除选中页签
	virtual void CleanSelTab();

	const CRect& GetRect(){return m_rectDraw;}
	
	void ClearAllTab();
	CTypeControl m_ArrCtlUnits; //类型控制栏单元数组
private:
	CRect        m_rectDraw;    //有效画图区域
	int          m_iMarginWidth;//单元边距
	int          m_iCurFocus;   //当前被激活单元

	CFont*    m_pftText;          //文本字体
	COLORREF m_crLineColor;     //边框线颜色
	COLORREF m_crTextColor;     //文本色(未选中)
	COLORREF m_crTextSelColor;  //文本色(选中)
	COLORREF m_crBgColor;       //整体背景色
	COLORREF m_crUnitSelBgColor;//单元背景色(选中)
	COLORREF m_crUnitBgColor;   //单元背景色(未选中)
private:
	CSize    m_sizeArror;       //箭头大小
};

#endif // !defined(AFX_TYPECTLPIC_H__ED4F4C79_F077_4E7D_AE39_50A83A45E868__INCLUDED_)
