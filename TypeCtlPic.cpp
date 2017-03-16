// TypeCtlPic.cpp: implementation of the CTypeCtlPic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "TypeCtlPic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTypeCtlPic::CTypeCtlPic()
{
	m_ArrCtlUnits.RemoveAll();//数组清空
	m_iMarginWidth = 4;       //单元边距固定为4
	m_iCurFocus    = 9999;    //默认最大值

	//创建字体
	m_pftText = CInterfaceManager::Instance()->GetSysFont(ID_PRICEBOARD_PRICEBOARDFONT);
	//设置颜色和字体
	SetColorsAndFonts();
}

CTypeCtlPic::~CTypeCtlPic()
{
	m_ArrCtlUnits.RemoveAll();
}

//函数功能：设置画图区间
//函数参数：rRect 画图区间
//函数返回:无
//函数异常:无
void CTypeCtlPic::MoveTo(CRect rRect)
{
	m_rectDraw = rRect;
}

//函数功能: 得到鼠标点击位置
//函数参数：pt 鼠标点击点
//函数返回：成功则返回当前选中单元序号，否则返回 -1
//函数异常：无
int CTypeCtlPic::HitTest(CPoint pt)
{
	if(!m_rectDraw.PtInRect(pt)) return -1;

	int iPos;
	bool bFinded = false;
	//循环查找
	for(iPos = 0; iPos < m_ArrCtlUnits.GetSize() && !bFinded; iPos++)
	{
		//找到
		if(m_ArrCtlUnits.GetAt(iPos).m_rtUnitRect.PtInRect(pt))
			bFinded = true;
	}

	iPos--;
	if(bFinded) //找到
		return iPos;
	else        //没找到
		return -1;
}

//函数功能：设置单元被激活(选中)/不激活(不选中)
//函数参数：iUnit 单元号；bFocus 是否被激活
//函数返回：无
void CTypeCtlPic::SetUnitFocus(int iUnit, bool bFocus /* = false */)
{
	if(iUnit >= m_ArrCtlUnits.GetSize() || iUnit < 0) return; //防止数组越界
	STypeCtlUnit& sUnit = m_ArrCtlUnits[iUnit];
	sUnit.m_bFocus = bFocus;
}

//函数功能：插入单元
//函数参数：lpszText 单元文本；bFocus 是否被激活
//函数返回: 成功为TRUE,错误为FALSE
BOOL CTypeCtlPic::InsertUnit(LPCTSTR lpszText,bool bFocus /* = false */)
{
	CString szUnitText;
	
	szUnitText = lpszText;
	if(szUnitText.IsEmpty()) return FALSE; //单元文本空则退出
	//创建单元对象
	STypeCtlUnit stuUnit;
	stuUnit.m_bFocus = bFocus;
	stuUnit.m_szUnitText = szUnitText;
	//加入到单元数组
	m_ArrCtlUnits.Add(stuUnit);
	//设置选中页签序号
	if(bFocus)
	{
		m_iCurFocus = m_ArrCtlUnits.GetSize()-1;
	}
	return TRUE;
}


void CTypeCtlPic::ClearAllTab()
{
	m_ArrCtlUnits.RemoveAll();
}
//函数功能：画出图形
//函数参数：pDC 画图设备指针
//函数返回：无
BOOL CTypeCtlPic::Draw(CDC* pDC)
{
	//矩形无效则退出
	if(!CPublic::Instance()->IsRectValid(m_rectDraw))
		return FALSE;

	//首先设置颜色和字体
	SetColorsAndFonts();

	//刷背景色
	//pDC->FillSolidRect(m_rectDraw,m_crBgColor);
	//绘制类型控制栏整体边框线
	CPen tabPen(PS_SOLID, 1, m_crLineColor);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&tabPen);
	pDC->MoveTo(m_rectDraw.left,m_rectDraw.top);
	pDC->LineTo(m_rectDraw.left,m_rectDraw.bottom);
	pDC->SelectObject(pOldPen);

	int iWidth = 0;     //类型控制栏宽度
	bool bDraw = true;  //是否绘制
	//循环输出单元图形
	for(int i = 0; i < m_ArrCtlUnits.GetSize(); i++)
	{
		//计算单元矩形大小
		iWidth += ComputeUnitRect(pDC,i);
		//判断类型控制栏宽度是否越界
		if(iWidth > m_rectDraw.Width()) bDraw = false;
		//绘制单元图形
		if(bDraw) DrawUnit(pDC,i);
	}
	return TRUE;
}

//函数功能：计算单元矩形大小
//函数参数：pDC 画图设备指针；iUnit 单元序号
//函数返回：单元宽度
int CTypeCtlPic::ComputeUnitRect(CDC* pDC,int iUnit)
{
	//数组越界则返回0
	if(iUnit < 0 || iUnit >= m_ArrCtlUnits.GetSize()) return 0;

	//计算单元文本大小
	CFont* pOldFont = (CFont*) pDC->SelectObject(m_pftText);
	STypeCtlUnit& sUnit = m_ArrCtlUnits[iUnit];
	CRect& rc = sUnit.m_rtUnitRect;
	rc.SetRectEmpty();   //rc(0,0,0,0)
	pDC->DrawText(sUnit.m_szUnitText, &rc, DT_CALCRECT);
	m_sizeArror = pDC->GetTextExtent(_T("◢"));//计算箭头大小
	//确定单元矩形的左上角坐标
	if(0 == iUnit) //首单元
	{
		CPoint pt(m_rectDraw.left,m_rectDraw.top);
		rc += pt;
	}
	else //iUnit >= 1
	{
		CPoint pt(m_ArrCtlUnits[iUnit-1].m_rtUnitRect.right
			,m_ArrCtlUnits[iUnit-1].m_rtUnitRect.top);
		rc += pt;
	}
	//加上两边的单元边距(再扩展2个象素的宽度)
	rc.right += (m_iMarginWidth*2);
	rc.bottom = m_rectDraw.bottom;
	pDC->SelectObject(pOldFont);

	//返回单元宽度
	return rc.Width();
}

//函数功能：绘制单元图形
//函数参数：pDC 画图设备指针；iUnit 单元序号
//函数返回：无
void CTypeCtlPic::DrawUnit(CDC* pDC,int iUnit)
{
	//数组越界则退出
	if(iUnit < 0 || iUnit >= m_ArrCtlUnits.GetSize()) return;

	STypeCtlUnit sUnit = m_ArrCtlUnits[iUnit];
	CRect rtUnit = sUnit.m_rtUnitRect;
	if(rtUnit.IsRectEmpty()) return; //单元矩形空则退出

	//配置设备环境
	CPen tabPen(PS_SOLID, 1, m_crLineColor);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&tabPen);
	int iMode = pDC->SetBkMode(TRANSPARENT); //设置背景模式为透明
	
	//*************画线***************
	CRgn rgn; //单元多边形区域
    if(sUnit.m_bFocus) //单元被选中/激活
	{
		m_iCurFocus = iUnit; //记录当前被激活单元
		//填充单元背景色
		if(0 == iUnit) //首单元
		{
			CPoint pts[5];
			pts[0] = CPoint(rtUnit.left, rtUnit.top);
			pts[1] = CPoint(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
			pts[2] = CPoint(rtUnit.left+m_iMarginWidth,rtUnit.bottom);
			pts[3] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
			pts[4] = CPoint(rtUnit.right+m_iMarginWidth, rtUnit.top);
			rgn.CreatePolygonRgn(pts, 5, WINDING); //创建多边形区域
		}
		else //iUnit >= 1
		{
			CPoint pts[4];
			pts[0] = CPoint(rtUnit.left-m_iMarginWidth, rtUnit.top);
			pts[1] = CPoint(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
			pts[2] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
			pts[3] = CPoint(rtUnit.right+m_iMarginWidth, rtUnit.top);
			rgn.CreatePolygonRgn(pts, 4, WINDING); //创建多边形区域
		}
		CBrush brush(m_crUnitSelBgColor);
		CPen newPen(PS_NULL, 1, CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR));
		CPen* oldPen = pDC->SelectObject(&newPen);
		pDC->FillRgn(&rgn, &brush);//填充单元区域
		pDC->SelectObject(oldPen);
		rgn.DeleteObject();

		//画单元边框线
		if(0 == iUnit) //首单元(画法与其他单元不同)
		{
			pDC->MoveTo(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
			pDC->LineTo(rtUnit.left+m_iMarginWidth,rtUnit.bottom);
		}
		else //iUnit >= 1
		{
			pDC->MoveTo(rtUnit.left-m_iMarginWidth, rtUnit.top);
			pDC->LineTo(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
		}
		pDC->MoveTo(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
		pDC->LineTo(rtUnit.right,rtUnit.top+rtUnit.Height()/2);
	}
	else //未选中/激活
	{
		//填充单元背景色
		if(iUnit < m_iCurFocus) //未激活单元在激活单元之前
		{
			if(iUnit+1 == m_iCurFocus) //紧邻激活单元
			{
				CPoint pts[6];
				pts[0] = CPoint(rtUnit.left+m_iMarginWidth,rtUnit.top);
				pts[1] = CPoint(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
				pts[2] = CPoint(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
				pts[3] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
				pts[4] = CPoint(rtUnit.right,rtUnit.top+rtUnit.Height()/2);
				pts[5] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.top);
				rgn.CreatePolygonRgn(pts, 6, WINDING);
			}
			else //不紧邻激活单元
			{
				CPoint pts[5];
				pts[0] = CPoint(rtUnit.left+m_iMarginWidth,rtUnit.top);
				pts[1] = CPoint(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
				pts[2] = CPoint(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
				pts[3] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
				pts[4] = CPoint(rtUnit.right+m_iMarginWidth, rtUnit.top);
				rgn.CreatePolygonRgn(pts, 5, WINDING);
			}
		}
		else //iUnit > m_iCurFocus 未激活单元在激活单元之后
		{
			CPoint pts[5];
			pts[0] = CPoint(rtUnit.left+m_iMarginWidth,rtUnit.top);
			pts[1] = CPoint(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
			pts[2] = CPoint(rtUnit.left+m_iMarginWidth, rtUnit.bottom);
			pts[3] = CPoint(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
			pts[4] = CPoint(rtUnit.right+m_iMarginWidth, rtUnit.top);
			rgn.CreatePolygonRgn(pts, 5, WINDING);
		}
		CBrush brush(m_crUnitBgColor);
		CPen newPen(PS_NULL, 1,  CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR));
		CPen* oldPen = pDC->SelectObject(&newPen);
		pDC->FillRgn(&rgn, &brush);//填充单元区域
		pDC->SelectObject(oldPen);
		rgn.DeleteObject();

		//画单元边框线
		pDC->MoveTo(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
		pDC->LineTo(rtUnit.left+m_iMarginWidth,rtUnit.top);
		
		pDC->MoveTo(rtUnit.left,rtUnit.top+rtUnit.Height()/2);
		pDC->LineTo(rtUnit.left+m_iMarginWidth, rtUnit.bottom);

		pDC->MoveTo(rtUnit.right-m_iMarginWidth, rtUnit.bottom);
		pDC->LineTo(rtUnit.right,rtUnit.top+rtUnit.Height()/2);

		if(iUnit < m_iCurFocus) //未激活单元在激活单元之前
		{
			if(0 == iUnit) //首单元
			{
				pDC->MoveTo(rtUnit.left,rtUnit.top);
				pDC->LineTo(rtUnit.right-m_iMarginWidth,rtUnit.top);
			}
			else //iUnit >= 1
			{
				pDC->MoveTo(rtUnit.left-m_iMarginWidth,rtUnit.top);
				pDC->LineTo(rtUnit.right-m_iMarginWidth,rtUnit.top);
			}
		}
		else //iUnit > m_iCurFocus 未激活单元在激活单元之后
		{
			pDC->MoveTo(rtUnit.left+m_iMarginWidth,rtUnit.top);
			pDC->LineTo(rtUnit.right+m_iMarginWidth,rtUnit.top);
		}
	}
	
	if(iUnit == m_ArrCtlUnits.GetSize()-1) //绘制最后一个单元
	{
		pDC->MoveTo(rtUnit.right,rtUnit.top+rtUnit.Height()/2);
		pDC->LineTo(rtUnit.right+m_iMarginWidth, rtUnit.top);
		pDC->LineTo(m_rectDraw.right,m_rectDraw.top);
		
		pDC->MoveTo(rtUnit.right-m_iMarginWidth, rtUnit.top);
		pDC->LineTo(rtUnit.right+m_iMarginWidth, rtUnit.top);
	}

	//************输出文本**************
	CFont* pOldFont = (CFont*)pDC->SelectObject(m_pftText);
	if(sUnit.m_bFocus)
		pDC->SetTextColor(m_crTextSelColor); //单元选中色
	else
		pDC->SetTextColor(m_crTextColor); //未选中色
	pDC->DrawText(sUnit.m_szUnitText, &rtUnit, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
	
	//恢复设备环境
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SetBkMode(iMode);
}

//函数功能：响应鼠标点击
//函数参数：point 鼠标点
//函数返回：TURE 重画；FALSE 不重画
BOOL CTypeCtlPic::MouseDown(CPoint point)
{
	int iSel;
	iSel = HitTest(point);
	if(-1 == iSel)
		return FALSE;
	OnSelectFocus(iSel);	
	return TRUE;
}

//函数功能：被回调，激活控制栏中的单元
//函数参数：iSel对应的激活单元
//函数返回：无
void CTypeCtlPic::OnSelectFocus(int iSel)
{
	//没选中或不选中则清除已选中
	if(iSel < 0)
	{
		CleanSelTab();
		return;
	}

	SetUnitFocus(m_iCurFocus,false); //使旧选中单元未激活
	SetUnitFocus(iSel,true); //激活当前选中单元
	m_iCurFocus = iSel; //激活单元赋值
}

//函数功能：获取当前选中页签序号
//函数参数：无
//函数返回：当前选中页签序号
int CTypeCtlPic::GetSelTabID()
{
	return m_iCurFocus;
}

//函数功能：获取选中页签文本
//函数参数：point 鼠标点击点
//函数返回：_T("")：弹出菜单；非空：页签文本
CString CTypeCtlPic::GetSelTabText(CPoint point)
{
	STypeCtlUnit stuUnit;
	stuUnit = m_ArrCtlUnits[m_iCurFocus];
	CString szUnit;
	szUnit = stuUnit.m_szUnitText;
	if(szUnit.Find(_T("◢"),0) < 0)
		return szUnit;
	if(point.x <= stuUnit.m_rtUnitRect.right-m_iMarginWidth-m_sizeArror.cx)
	{
		szUnit = szUnit.Left(szUnit.GetLength()-2);
		return szUnit;
	}
	else
	{
		return _T("");
	}
}

//函数功能：设置当前选中页签序号
//函数参数：iID 页签序号
//函数返回：无
void CTypeCtlPic::SetSelTabID(int iID)
{
	m_iCurFocus = iID;

}

//函数功能：设置当前选中页签文本
//函数参数：lpszText 页签文本
//函数返回：无
void CTypeCtlPic::SetSelTabText(LPCTSTR lpszText)
{
	if(m_iCurFocus < 0 || m_iCurFocus >= m_ArrCtlUnits.GetSize())
		return;
	STypeCtlUnit& stuUnit = m_ArrCtlUnits[m_iCurFocus];
	stuUnit.m_szUnitText = lpszText;
	//m_ArrCtlUnits[m_iCurFocus].m_szUnitText = lpszText;
}

//函数功能：获取类型控制栏左上角坐标
//函数参数：无
//函数返回：左上角坐标
CPoint CTypeCtlPic::GetTopLeftPt()
{
	return m_rectDraw.TopLeft();
}

//函数功能：获取类型控制栏右上角坐标
//函数参数：pDC 设备DC
//函数返回：右上角坐标
CPoint CTypeCtlPic::GetRightTopPt(CDC* pDC)
{
	int iRec;
	int nCount;
	int nTotalWidth;

	nTotalWidth = 0;
	nCount = m_ArrCtlUnits.GetSize();
	for(iRec = 0; iRec < nCount; iRec++)
	{
		nTotalWidth += ComputeUnitRect(pDC,iRec);
	}
	nTotalWidth += m_iMarginWidth;
	
	CPoint pt;
	pt.x = m_rectDraw.left+nTotalWidth;
	pt.y = m_rectDraw.top;
	return pt;
}

//函数功能：设置颜色和字体
void CTypeCtlPic::SetColorsAndFonts()
{
	//获取系统设定字体和颜色
	//m_pFont = CPublic::Instance()->GetWantFont(ID_TABLE_FONT);
	m_crBgColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
	m_crLineColor = CInterfaceManager::Instance()->GetSysColor(ID_COORDINATE_COLOR);
	m_crTextColor = CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR);
	m_crTextSelColor = CInterfaceManager::Instance()->GetSysColor(ID_HIGHLIGHT_COLOR);
	m_crUnitSelBgColor = CInterfaceManager::Instance()->GetSysColor(ID_GRIDROWSEL_COLOR);
	m_crUnitBgColor = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);
}

//函数功能：清除选中页签
void CTypeCtlPic::CleanSelTab()
{
	SetUnitFocus(m_iCurFocus,false);
	m_iCurFocus = 9999;
}