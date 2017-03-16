// ColorDefPic.cpp : implementation file
#include "stdafx.h"
#include "koaclient.h"
#include "ColorDefPic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDefPic

CColorDefPic::CColorDefPic()
{
	ResetTempSysColor();

	ResetTempSysFont();
}

CColorDefPic::~CColorDefPic()
{
	FreeTempSysFont();
}

BEGIN_MESSAGE_MAP(CColorDefPic, CStatic)
	//{{AFX_MSG_MAP(CColorDefPic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDefPic message handlers

void CColorDefPic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//获取画图区域
	CRect mainRect;
	GetClientRect(&mainRect);

	//创建内存DC
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bitmapMem;
	bitmapMem.CreateCompatibleBitmap(&dc, mainRect.Width(), mainRect.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bitmapMem);

	//以下开始绘制图形
	//////////////////////////////////////////////////////////////////////////
	//有关变量定义
	CSize sizeText;
	CRect rtDraw;
	CRect rtTmp;
	CPen  penDraw;
	CFont ftDraw;
	//创建并设置字体
	ftDraw.CreateFont(12,0,0,0,0,FALSE,FALSE
		,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH,_T("Tohoma"));
	CFont* pOldFont = (CFont*)dcMem.SelectObject(&ftDraw);
	int iMode = dcMem.SetBkMode(TRANSPARENT);//设置背景模式为透明

	//设置系统背景颜色
	dcMem.FillSolidRect(mainRect,GetTempSysColor(ID_SYSBACK_COLOR));
	//绘制"大标题"
	dcMem.SetTextColor(GetTempSysColor(ID_TITLE_COLOR));
	sizeText = dcMem.GetTextExtent(CLanguageInterface::Instance()->GetStr(20090) );
	rtDraw.left   = mainRect.left + 2;
	rtDraw.top    = mainRect.top + 2;
	rtDraw.right  = rtDraw.left + sizeText.cx;
	rtDraw.bottom = rtDraw.top + sizeText.cy;
	dcMem.DrawText(CLanguageInterface::Instance()->GetStr(20090), rtDraw
		, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
	//绘制"高亮背景色"
	sizeText = dcMem.GetTextExtent( CLanguageInterface::Instance()->GetStr(20091) );
	rtDraw.left  = rtDraw.right + 6;
	rtDraw.right = rtDraw.left + sizeText.cx;
	dcMem.FillSolidRect(rtDraw,GetTempSysColor(ID_HIGHLIGHT_COLOR));
	dcMem.SetTextColor( CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR));
	dcMem.DrawText( CLanguageInterface::Instance()->GetStr(20091), rtDraw
		, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
	//绘制坐标轴
	rtDraw.top = rtDraw.bottom;
	rtDraw.left = mainRect.left + 2;
	rtDraw.right = rtDraw.left + mainRect.Width()/2 + 10;
	rtDraw.bottom = rtDraw.top + mainRect.Height()/2 - 24;
	rtTmp = rtDraw;
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_COORDINATE_COLOR));
	CPen* pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(rtDraw.left, rtDraw.bottom);
	dcMem.LineTo(rtDraw.right, rtDraw.bottom);
	dcMem.SetTextColor(GetTempSysColor(ID_COORDINATE_COLOR));
	dcMem.TextOut( rtDraw.left, rtDraw.bottom+2, CLanguageInterface::Instance()->GetStr(20092) );
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制坐标刻度
	sizeText = dcMem.GetTextExtent( CLanguageInterface::Instance()->GetStr(20092));
	rtDraw.left = rtDraw.left+sizeText.cx+6;
	rtDraw.top  = rtDraw.bottom+2;
	dcMem.SetTextColor(GetTempSysColor(ID_COORDTEXT_COLOR));
	dcMem.TextOut( rtDraw.left, rtDraw.top, _T("04/07/07 13:00") );
	//绘制成交量
	rtDraw.right += 6;
	rtDraw.top = mainRect.top + 2;
	dcMem.SetTextColor(GetTempSysColor(ID_TOTALVOL_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20093) );
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_TOTALVOL_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	sizeText = dcMem.GetTextExtent( CLanguageInterface::Instance()->GetStr(20093) );
	rtDraw.left = rtDraw.right + sizeText.cx;
	rtDraw.top  += (sizeText.cy/2);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(mainRect.right-4, rtDraw.top);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制指标1
	rtDraw.top = mainRect.top+sizeText.cy+4;
	dcMem.SetTextColor(GetTempSysColor(ID_INDEXLINE1_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20094) );
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_INDEXLINE1_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	rtDraw.top  += (sizeText.cy/2);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(mainRect.right-4, rtDraw.top);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制指标2
	rtDraw.top = mainRect.top+2*sizeText.cy+6;
	dcMem.SetTextColor(GetTempSysColor(ID_INDEXLINE2_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20095));
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_INDEXLINE2_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	rtDraw.top  += (sizeText.cy/2);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(mainRect.right-4, rtDraw.top);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制指标3
	rtDraw.top = mainRect.top+3*sizeText.cy+8;
	dcMem.SetTextColor(GetTempSysColor(ID_INDEXLINE3_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20096) );
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_INDEXLINE3_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	rtDraw.top  += (sizeText.cy/2);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(mainRect.right-4, rtDraw.top);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制指标4
	rtDraw.top = mainRect.top+4*sizeText.cy+10;
	dcMem.SetTextColor(GetTempSysColor(ID_INDEXLINE4_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20097));
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_INDEXLINE4_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	rtDraw.top  += (sizeText.cy/2);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(mainRect.right-4, rtDraw.top);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制指标5
	rtDraw.top = mainRect.top+5*sizeText.cy+12;
	dcMem.SetTextColor(GetTempSysColor(ID_INDEXLINE5_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20098) );
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_INDEXLINE5_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	rtDraw.top  += (sizeText.cy/2);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(mainRect.right-4, rtDraw.top);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制指标6
	rtDraw.top = mainRect.top+6*sizeText.cy+14;
	dcMem.SetTextColor(GetTempSysColor(ID_INDEXLINE6_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20099));
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_INDEXLINE6_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	rtDraw.top  += (sizeText.cy/2);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(mainRect.right-4, rtDraw.top);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制指标7
	rtDraw.top = mainRect.top+7*sizeText.cy+16;
	dcMem.SetTextColor(GetTempSysColor(ID_INDEXLINE7_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20100) );
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_INDEXLINE7_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	rtDraw.top  += (sizeText.cy/2);
	dcMem.MoveTo(rtDraw.left, rtDraw.top);
	dcMem.LineTo(mainRect.right-4, rtDraw.top);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();	
	//绘制"资讯文本"
	rtDraw.top = mainRect.top+8*sizeText.cy+18;
	dcMem.SetTextColor(GetTempSysColor(ID_INFOTEXT_COLOR));
	dcMem.TextOut(rtDraw.right, rtDraw.top, CLanguageInterface::Instance()->GetStr(20101) );
	//绘制阴线
	sizeText = dcMem.GetTextExtent(CLanguageInterface::Instance()->GetStr(20102));
	rtDraw = rtTmp;
	rtDraw.left += 8;
	rtDraw.top += 16;
	rtDraw.right = rtDraw.left+sizeText.cx;
	rtDraw.bottom = rtDraw.top+68;
	rtTmp = rtDraw;
	dcMem.SetTextColor(GetTempSysColor(ID_KLINEDOWN_COLOR));
	dcMem.TextOut(rtDraw.left, rtDraw.top, CLanguageInterface::Instance()->GetStr(20102));
	rtDraw.top += (sizeText.cy+4);
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_KLINEDOWN_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	dcMem.MoveTo(rtDraw.left+rtDraw.Width()/2, rtDraw.top);
	dcMem.LineTo(rtDraw.left+rtDraw.Width()/2, rtDraw.top+8);
	dcMem.MoveTo(rtDraw.left+rtDraw.Width()/2, rtDraw.bottom-8);
	dcMem.LineTo(rtDraw.left+rtDraw.Width()/2, rtDraw.bottom);
	rtDraw.left = rtDraw.left+rtDraw.Width()/2 - 6;
	rtDraw.top += 8;
	rtDraw.right = rtDraw.left+12;
	rtDraw.bottom -= 8;
	dcMem.FillSolidRect(rtDraw,GetTempSysColor(ID_KLINEDOWN_COLOR));
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制阳线
	rtDraw = rtTmp;
	rtDraw.left += (sizeText.cx+6);
	rtDraw.right = rtDraw.left+sizeText.cx;
	rtTmp = rtDraw;
	dcMem.SetTextColor(GetTempSysColor(ID_RISE_COLOR));
	dcMem.TextOut(rtDraw.left, rtDraw.top, CLanguageInterface::Instance()->GetStr(20103) );
	rtDraw.top += (sizeText.cy+4);
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_RISE_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	dcMem.MoveTo(rtDraw.left+rtDraw.Width()/2, rtDraw.top);
	dcMem.LineTo(rtDraw.left+rtDraw.Width()/2, rtDraw.top+8);
	dcMem.MoveTo(rtDraw.left+rtDraw.Width()/2, rtDraw.bottom-8);
	dcMem.LineTo(rtDraw.left+rtDraw.Width()/2, rtDraw.bottom);
	rtDraw.left = rtDraw.left+rtDraw.Width()/2 - 6;
	rtDraw.top += 8;
	rtDraw.right = rtDraw.left+12;
	rtDraw.bottom -= 8;
	dcMem.FillSolidRect(rtDraw,GetTempSysColor(ID_RISE_COLOR));
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制平线
	rtDraw = rtTmp;
	rtDraw.left += (sizeText.cx+6);
	rtDraw.right = rtDraw.left+sizeText.cx;
	dcMem.SetTextColor(GetTempSysColor(ID_EQUAL_COLOR));
	dcMem.TextOut(rtDraw.left, rtDraw.top,CLanguageInterface::Instance()->GetStr(20104));
	rtDraw.top += (sizeText.cy+4);
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_EQUAL_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	dcMem.MoveTo(rtDraw.left+rtDraw.Width()/2, rtDraw.top);
	dcMem.LineTo(rtDraw.left+rtDraw.Width()/2, rtDraw.bottom);
	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();
	//绘制表格线
	int nTableRowCount = 2;
	rtDraw.left   = mainRect.left;
	rtDraw.top    = mainRect.top+mainRect.Height()/2+8;
	rtDraw.right  = mainRect.right-4;
	rtDraw.bottom = rtDraw.top + dcMem.GetTextExtent(_T("1")).cy * 4;
	UINT uiArrCol[5]; //列宽数组
	UINT uiRowHigh;   //行高
	uiRowHigh = rtDraw.Height()/nTableRowCount;
	uiArrCol[0] = 30;
	sizeText = dcMem.GetTextExtent(_T("AUDUSDpro"));
	uiArrCol[1] = sizeText.cx+8;
	sizeText = dcMem.GetTextExtent(_T("18.88999"));
	uiArrCol[2] = sizeText.cx+8;
	sizeText = dcMem.GetTextExtent(_T("1324568"));
	uiArrCol[3] = sizeText.cx+8;
	uiArrCol[4] = uiArrCol[2];
	int iCol,iRow;
	int iOffset,iTmp;
	CString szText;
	UINT uiFormat;
	penDraw.CreatePen(PS_SOLID, 1, GetTempSysColor(ID_GRIDLINE_COLOR));
	pOldPen = (CPen*) dcMem.SelectObject(&penDraw);
	for(iRow = 0; iRow < nTableRowCount; iRow++)
	{
		for(iCol = 0; iCol < 5; iCol++)
		{
			iOffset = 0;
			if(iCol > 0)
			{
				for(iTmp=0;iTmp<iCol;iTmp++)
				{
					iOffset += uiArrCol[iTmp];
				}
			}
			rtTmp.left = rtDraw.left+iOffset;
			rtTmp.right = rtTmp.left+uiArrCol[iCol];
			rtTmp.top = rtDraw.top+iRow*uiRowHigh;
			rtTmp.bottom = rtTmp.top+uiRowHigh;

			//填充网格背景色
			if(1 == iRow)
			{
				dcMem.FillSolidRect(rtTmp, GetTempSysColor(ID_GRIDROWSEL_COLOR));
			}
			//画网格线
			dcMem.MoveTo(rtTmp.left, rtTmp.top);
			dcMem.LineTo(rtTmp.right, rtTmp.top);
			dcMem.LineTo(rtTmp.right, rtTmp.bottom);
			dcMem.LineTo(rtTmp.left, rtTmp.bottom);
			dcMem.LineTo(rtTmp.left,rtTmp.top);
			switch(iRow)
			{
			case 0:
				{
					dcMem.SetTextColor(GetTempSysColor(ID_GRIDFIXED_COLOR));
					uiFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
					switch(iCol)
					{
					case 0:
						szText = _T("");
						break;
					case 1:
						szText =CLanguageInterface::Instance()->GetStr(20020);
						break;
					case 2:
						szText =CLanguageInterface::Instance()->GetStr(20027);
						break;
					case 3:
						szText =CLanguageInterface::Instance()->GetStr(20028);
						break;
					case 4:
						szText =CLanguageInterface::Instance()->GetStr(20029);
						break;
					}
					//输出文本
					dcMem.DrawText( szText, rtTmp, uiFormat);
					//绘制箭头
					if(2 == iCol)
					{
						dcMem.SetTextColor(GetTempSysColor(ID_SORTARROR_COLOR));
						dcMem.TextOut( rtTmp.right-10, rtTmp.top+7, _T("↓") );
					}
				}
				break;
			case 1:
				{
					switch(iCol)
					{
					case 0:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_GRIDFIXED_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("1");
						}
						break;
					case 1:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_STOCK_COLOR));
							uiFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("XAUUSD");
						}
						break;
					case 2:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_DOWN_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("18.88");
						}
						break;
					case 3:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_TOTALVOL_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("1338870");
						}
						break;
					case 4:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_OTHER_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("10.03");
						}
						break;
					}
					//输出文本
					dcMem.DrawText( szText, rtTmp, uiFormat);
				}
				break;
			/*
			case 2:
				{
					switch(iCol)
					{
					case 0:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_GRIDFIXED_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("2");
						}
						break;
					case 1:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_STOCK_COLOR));
							uiFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("深万科");
						}
						break;
					case 2:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_EQUAL_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("12.38");
						}
						break;
					case 3:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_TOTALVOL_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("103022");
						}
						break;
					case 4:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_OTHER_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("10.03");
						}
						break;
					}
					//输出文本
					dcMem.DrawText( szText, rtTmp, uiFormat);
				}
				break;
			case 3:
				{
					switch(iCol)
					{
					case 0:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_GRIDFIXED_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("3");
						}
						break;
					case 1:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_STOCK_COLOR));
							uiFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("深纺织");
						}
						break;
					case 2:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_RISE_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("30.09");
						}
						break;
					case 3:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_TOTALVOL_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("208808");
						}
						break;
					case 4:
						{
							dcMem.SetTextColor(GetTempSysColor(ID_OTHER_COLOR));
							uiFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP;
							szText = _T("10.03");
						}
						break;
					}
					//输出文本
					dcMem.DrawText( szText, rtTmp, uiFormat);
				}
				break;
			*/
			}
		}
	}

    //绘制字体样本
	dcMem.SelectObject(pOldFont);
	TFontDefInfo tTempFontInfo = GetTempSysFont(ID_TABLE_FONT); 
	rtDraw.top = rtDraw.bottom + dcMem.GetTextExtent(tTempFontInfo.szName).cy;
	pOldFont = dcMem.SelectObject(tTempFontInfo.pFont);
	dcMem.SetTextColor(GetTempSysColor(ID_GRIDFIXED_COLOR));
	dcMem.TextOut(rtDraw.left, rtDraw.top, tTempFontInfo.szName );
	rtDraw.bottom = rtDraw.top+dcMem.GetTextExtent(tTempFontInfo.szName).cy*1.5;

	dcMem.SelectObject(pOldFont);
	tTempFontInfo = GetTempSysFont(ID_NUMBER_FONT);
	pOldFont = dcMem.SelectObject(tTempFontInfo.pFont);
	rtDraw.left = mainRect.Width()/3;
	dcMem.SetTextColor(GetTempSysColor(ID_COORDTEXT_COLOR));
	dcMem.TextOut(rtDraw.left, rtDraw.top, tTempFontInfo.szName );

	dcMem.SelectObject(pOldFont);
	tTempFontInfo = GetTempSysFont(ID_INFOTEXT_FONT);
	pOldFont = dcMem.SelectObject(tTempFontInfo.pFont);
	rtDraw.left = mainRect.Width()*2/3;
	dcMem.SetTextColor(GetTempSysColor(ID_INFOTEXT_FONT));
	dcMem.TextOut(rtDraw.left, rtDraw.top, tTempFontInfo.szName );

	dcMem.SelectObject(pOldFont);
	tTempFontInfo = GetTempSysFont(ID_TITLE_FONT);
	pOldFont = dcMem.SelectObject(tTempFontInfo.pFont);
	rtDraw.top = rtDraw.bottom;
	rtDraw.left = mainRect.left;
    dcMem.SetTextColor(GetTempSysColor(ID_TITLE_COLOR));
	dcMem.TextOut(rtDraw.left, rtDraw.top, tTempFontInfo.szName );
	rtDraw.bottom = rtDraw.top+dcMem.GetTextExtent(tTempFontInfo.szName).cy;

	dcMem.SelectObject(pOldFont);
	tTempFontInfo = GetTempSysFont(ID_STOCKTRACE_FONT);
	pOldFont = dcMem.SelectObject(tTempFontInfo.pFont);
	rtDraw.left = mainRect.Width()/2;
	dcMem.SetTextColor(GetTempSysColor(ID_TIMELETTER_COLOR));
	dcMem.TextOut(rtDraw.left, rtDraw.top, tTempFontInfo.szName );

	dcMem.SelectObject(pOldPen);
	penDraw.DeleteObject();

	//恢复DC
	dcMem.SetBkMode(iMode);
	dcMem.SelectObject(pOldFont);
	ftDraw.DeleteObject();

	//////////////////////////////////////////////////////////////////////////

	//拷贝图形
	dc.BitBlt(mainRect.left,mainRect.top,mainRect.Width()
		,mainRect.Height(),&dcMem,0,0,SRCCOPY);

	//恢复原DC并释放内存
    dcMem.SelectObject(pOldBit);
	DeleteObject(bitmapMem.m_hObject);
	dcMem.DeleteDC();
}

COLORREF CColorDefPic::GetTempSysColor(UINT uiColorID)
{
	vector<TColorDefInfo>::iterator iterColorDef;
	for( iterColorDef = m_vecTempSysColor.begin();
	iterColorDef != m_vecTempSysColor.end();
	iterColorDef++)
	{
		if( iterColorDef->uiID == uiColorID)
		{
			return iterColorDef->crColor;
		}
	}
	
	return -1;
}

void CColorDefPic::UpdateTempSysColor(const TColorDefInfo& tUpdateColor)
{
	vector<TColorDefInfo>::iterator iterColorDef;
	for( iterColorDef = m_vecTempSysColor.begin();
	iterColorDef != m_vecTempSysColor.end();
	iterColorDef++)
	{
		if( iterColorDef->uiID == tUpdateColor.uiID)
		{
			iterColorDef->crColor = tUpdateColor.crColor;
			Invalidate();
			break;
		}
	}
}

void CColorDefPic::ResetTempSysColor()
{
	m_vecTempSysColor.clear();
	
	const vector<TColorDefInfo>& vecSrcSysColor = CInterfaceManager::Instance()->GetSysColorList();
	
    TColorDefInfo tTempColor;
	vector<TColorDefInfo>::const_iterator iterSrcColor;
	for( iterSrcColor = vecSrcSysColor.begin();
	iterSrcColor != vecSrcSysColor.end();
	iterSrcColor++)
	{
		_tcscpy(tTempColor.szName,iterSrcColor->szName);
		tTempColor.uiID = iterSrcColor->uiID;
		tTempColor.crColor = iterSrcColor->crColor;
		m_vecTempSysColor.push_back(tTempColor);
	}

	if( GetSafeHwnd())
	{
		Invalidate();
	}
}

BOOL CColorDefPic::SaveTemp2SysColor()
{
	return CInterfaceManager::Instance()->SaveSysColorList(
		CInterfaceManager::Instance()->GetSysStyle(),m_vecTempSysColor);
}

void CColorDefPic::ResetTempSysFont()
{
    FreeTempSysFont();
	
	const vector<TFontDefInfo>& vecSrcSysFont = CInterfaceManager::Instance()->GetSysFontList();
		
	TFontDefInfo tTempFont;
	vector<TFontDefInfo>::const_iterator iterSrcFont;
	for( iterSrcFont = vecSrcSysFont.begin();
	iterSrcFont != vecSrcSysFont.end();
	iterSrcFont++)
	{
		_tcscpy(tTempFont.szName,iterSrcFont->szName);
		tTempFont.uiID = iterSrcFont->uiID;
		tTempFont.nSize = iterSrcFont->nSize;
		tTempFont.nStyle = iterSrcFont->nStyle;
		_tcscpy(tTempFont.szFontName,iterSrcFont->szFontName);
		tTempFont.pFont = CInterfaceManager::Instance()->CreateFontEx(
			              tTempFont.szFontName,tTempFont.nSize,tTempFont.nStyle);
		m_vecTempSysFont.push_back(tTempFont);
	}

	if( GetSafeHwnd())
	{
		Invalidate();
	}
}

BOOL CColorDefPic::SaveTemp2SysFont()
{
	return CInterfaceManager::Instance()->SaveSysFontList(
	                        	CInterfaceManager::Instance()->GetSysStyle(),m_vecTempSysFont);
}

void CColorDefPic::UpdateTempSysFont(const TFontDefInfo& tUpdateFont)
{
	vector<TFontDefInfo>::iterator iterTempFont;
	for( iterTempFont = m_vecTempSysFont.begin();
	iterTempFont != m_vecTempSysFont.end();
	iterTempFont++)
	{
		if( iterTempFont->uiID == tUpdateFont.uiID)
		{
			iterTempFont->nSize = tUpdateFont.nSize;
			iterTempFont->nStyle = tUpdateFont.nStyle;
			_tcscpy(iterTempFont->szFontName,tUpdateFont.szFontName);
			if( iterTempFont->pFont != NULL)
			{
				iterTempFont->pFont->DeleteObject();
				delete iterTempFont->pFont;
				iterTempFont->pFont = NULL;
			}
			iterTempFont->pFont = CInterfaceManager::Instance()->CreateFontEx(
				tUpdateFont.szFontName,tUpdateFont.nSize,tUpdateFont.nStyle);
			Invalidate();
			break;
		}
	}		
}

TFontDefInfo& CColorDefPic::GetTempSysFont(UINT uiID)
{
	vector<TFontDefInfo>::iterator iterTempFont;
	for( iterTempFont = m_vecTempSysFont.begin();
	iterTempFont != m_vecTempSysFont.end();
	iterTempFont++)
	{
		if( iterTempFont->uiID == uiID)
		{
		    return *iterTempFont;
		}
	}		

	return *m_vecTempSysFont.begin();
}

////**** private

void CColorDefPic::FreeTempSysFont() 
{
	vector<TFontDefInfo>::iterator iterTempFont;
	for( iterTempFont = m_vecTempSysFont.begin();
	iterTempFont != m_vecTempSysFont.end();
	iterTempFont++)
	{
		if( iterTempFont->pFont != NULL)
		{
			iterTempFont->pFont->DeleteObject();
			delete iterTempFont->pFont;
			iterTempFont->pFont = NULL;
		}
	}		
	m_vecTempSysFont.clear();
}
