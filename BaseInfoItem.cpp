#include "StdAfx.h"
#include "resource.h"
#include "BaseInfoItem.h"
const int TITLEHEIGHT = 30;
const int PRICEHEIGHT  = 20;
const int BIGPRICEHEIGHT = 30;
const int BOTTOMHEIGHT = 80;
const int ITEMWIDTH = 200;

CBaseInfoItem::CBaseInfoItem(void)
{
	memset(&m_oCodeInfo,0,sizeof(SCodeInfo));	
	memset(&m_NowData,0,sizeof(SNowData));
	m_NowData.uiMinPrice = 10000000;
	m_pImageList = NULL;
	m_nUporDown = 2;
	m_bSelectStatus = FALSE;
}


CBaseInfoItem::~CBaseInfoItem(void)
{
}
void CBaseInfoItem::SetImageList(CImageList* pImageList)
{
	m_pImageList = pImageList;
}

BOOL CBaseInfoItem::SetStockInfo(SCodeInfo* pCodeInfo,string ssSymbolName)
{
	const StuSymbolData* pNewCode;
	pNewCode = CPublic::Instance()->GetStockInfo(pCodeInfo);
	if(NULL  == pNewCode)return FALSE;
	memcpy(&m_oCodeInfo,&pNewCode->sCode,sizeof(SCodeInfo));
	m_ssSymbolName = ssSymbolName;
	return TRUE; 
}

BOOL CBaseInfoItem::SetQuote(SNowData* pNow)
{
	if((pNow->uiNewPrice == m_NowData.uiNewPrice)||(m_NowData.uiNewPrice == 0))
	{
		m_nUporDown = 2;
	}
	else if(pNow->uiNewPrice <m_NowData.uiNewPrice)
	{
		m_nUporDown = 1;
	}
	else
	{
		m_nUporDown = 0;
	}
	memcpy(&m_NowData,pNow,sizeof(SNowData));
	return TRUE;
}
BOOL CBaseInfoItem::Draw(CDC* pDC)
{
	if(m_TotalRect.IsRectNull())return FALSE;
	if(0  == m_oCodeInfo.szCode[0])return FALSE;
	//画抬头区域
	int iOldDrawMode;
	iOldDrawMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF crPen;
	if(m_bSelectStatus)	
	{
		crPen = CInterfaceManager::Instance()->GetSysColor(ID_PRICEBOARD_SELECTCOLOR);//外框色
	}
	else
	{
		crPen = CInterfaceManager::Instance()->GetSysColor(ID_GRIDLINE_COLOR);//外框色
	}	
	CPen drawpen(PS_SOLID,1,crPen);
	CPen* pOldPen = pDC->SelectObject(&drawpen);
//	COLORREF crBrush;
//	CBrush bkbrush;
//
//	crBrush = CInterfaceManager::Instance()->GetSysColor(ID_PRICEBOARD_SINGLEBK);//背景色
//	bkbrush.CreateSolidBrush(crBrush);
//
//	CBrush* pOldBrush;
//	pOldBrush = pDC->SelectObject(&bkbrush);
////	pDC->Rectangle(m_TotalRect);//整体背景
//	pDC->RoundRect(m_TotalRect,CPoint(20,20));

// 	if(!m_bSelectStatus)
// 	{
// 		CRect fillRect;
// 		fillRect.left = m_PriceRect.left;
// 		fillRect.right = nMiddlePoint;
// 		fillRect.top = m_PriceRect.top;
// 		fillRect.bottom = m_TotalRect.bottom;
// 		COLORREF crFill = CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR);//ID_PRICEBOARD_BUYBK);
// 		pDC->FillSolidRect(fillRect,crFill);
// 	}


// 	pDC->MoveTo(m_PriceRect.left,m_PriceRect.top);
// 	pDC->LineTo(m_PriceRect.right,m_PriceRect.top);
// 	pDC->MoveTo(m_MainRect.left,m_MainRect.top);
// 	pDC->LineTo(m_MainRect.right,m_MainRect.top);
// 	pDC->MoveTo(m_MaxMinRect.left,m_MaxMinRect.top);
// 	pDC->LineTo(m_MaxMinRect.right,m_MaxMinRect.top);
// 	pDC->MoveTo(nMiddlePoint,m_PriceRect.top);
// 	pDC->LineTo(nMiddlePoint,m_TotalRect.bottom);
	//wzm-test
	CString szPath;
	HBITMAP hbitmap;
	szPath = CPublic::Instance()->GetSysPath();
	szPath +="interface/";
	szPath += "board.bmp";	
	hbitmap=(HBITMAP)::LoadImage(NULL,szPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if(hbitmap)
	{
		CBitmap	BuySellBmp;
		BuySellBmp.Attach(hbitmap);
		BITMAP bm1; 
		BuySellBmp.GetObject(sizeof(BITMAP),&bm1); 

		CDC dcImage; 
		if (!dcImage.CreateCompatibleDC(pDC)) 
			return FALSE; 
		CBitmap* pOldBitmap = dcImage.SelectObject(&BuySellBmp); 

		pDC->BitBlt(m_TotalRect.left,m_TotalRect.top,bm1.bmWidth,bm1.bmHeight, &dcImage, 0, 0, SRCCOPY);

		dcImage.SelectObject(&pOldBitmap);
	}
	//wz-test

	CFont* pOldFont;
	COLORREF crOldTextColor;
	pOldFont = pDC->SelectObject(CInterfaceManager::Instance()->GetSysFont(ID_PRICEBOARD_PRICEBOARDFONT));
	//crOldTextColor  = pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_PRICEBOARD_TEXTCOLOR));
	CString strTitle;
	//////////////////////////////////////////////////////////////////////////
	//modify by david gong at 20140207
// 	strTitle = m_oCodeInfo.szCode;
// 	strTitle += m_ssSymbolName.c_str();	
	strTitle = m_ssSymbolName.c_str();
	strTitle += " ";
	strTitle += m_oCodeInfo.szCode;

	//////////////////////////////////////////////////////////////////////////
// 	CPoint arrowpt;
// 
// 	arrowpt.x = m_TitleRect.left +10;
// 	arrowpt.y = m_TitleRect.top + (m_TitleRect.Height()-18)/2;
// 	//画出图标
// 	if(m_pImageList&&(m_nUporDown != -1))
// 	{
// 		m_pImageList->Draw(pDC,m_nUporDown,arrowpt,ILD_TRANSPARENT);
// 	}
	CFont* pOldFont1;
	pOldFont1 = new CFont;
	pOldFont1->CreateFont(15, 0, 0, 0, 700, 0, 0, 0, 1, 0, 0, 0, 0, _T("宋体"));
	pDC->SelectObject(pOldFont1);
	//pDC->DrawText(strTitle,m_TitleRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	//绘制字体
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(170,170,170));//COLOR_3DDKSHADOW COLOR_3DHILIGHT
	pDC->DrawText(strTitle,m_TitleRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	CRect tRect; //阴影区域
	tRect = m_TitleRect;
	tRect.left += 2;
	tRect.top -= 2;
	
	crOldTextColor  = pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_PRICEBOARD_TEXTCOLOR));
	pDC->DrawText(strTitle,tRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pOldFont1->DeleteObject();
	delete pOldFont1;
	pOldFont1 = NULL;
	//恢复设备上下文的原有设置
	pOldFont = pDC->SelectObject(CInterfaceManager::Instance()->GetSysFont(ID_PRICEBOARD_PRICEBOARDFONT));

	////////////////////////////////////////////////////////////////////////
	//设置字体
	//画最高最低价区域
	CString strPrice,strBidPrice,strAskPrice;
	float fPow;
	fPow = pow(10,(double)m_oCodeInfo.cPriceUnit);
//	pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR));
//	pDC->SelectObject(CInterfaceManager::Instance()->GetSysFont(ID_INFOTEXT_FONT));

	//strPrice  = CLanguageInterface::Instance()->GetStr(20024);
	//pDC->DrawText(strPrice,m_MaxRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	float fMaxPrice,fMinPrice;
	fMaxPrice = (float)m_NowData.uiMaxPrice/fPow;
	fMinPrice = (float)m_NowData.uiMinPrice/fPow;
	strPrice = CPublic::Instance()->GetFormateValue(fMaxPrice,m_oCodeInfo.cPriceUnit);
	pDC->DrawText(strPrice,m_MaxRect,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);


	//strPrice = CLanguageInterface::Instance()->GetStr(20025);
	//pDC->DrawText(strPrice,m_MinRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strPrice = CPublic::Instance()->GetFormateValue(fMinPrice,m_oCodeInfo.cPriceUnit);
	pDC->DrawText(strPrice,m_MinRect,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	///////////////////////////////////////////////////////////////////////
	//画报价区域
//	POINT arrowpt;


	//CString szPath;
	//szPath = CPublic::Instance()->GetSysPath();
	//szPath +="interface/";
	//szPath += CInterfaceManager::Instance()->GetPriceBoardBkPic();	
	////HBITMAP 
	//hbitmap=(HBITMAP)::LoadImage(NULL,szPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	//if(hbitmap)
	//{
	//	CBitmap	BuySellBmp;
	//	BuySellBmp.Attach(hbitmap);
	//	BITMAP bm1; 
	//	BuySellBmp.GetObject(sizeof(BITMAP),&bm1); 

	//	CDC dcImage; 
	//	if (!dcImage.CreateCompatibleDC(pDC)) 
	//		return FALSE; 
	//	CBitmap* pOldBitmap = dcImage.SelectObject(&BuySellBmp); 

	//	pDC->BitBlt(m_BuyRect.left,m_BuyRect.top,bm1.bmWidth,bm1.bmHeight, &dcImage, 0, 0, SRCCOPY);

	//	pDC->BitBlt(m_SellRect.left,m_SellRect.top,bm1.bmWidth,bm1.bmHeight, &dcImage, 0, 0, SRCCOPY);
	//	dcImage.SelectObject(&pOldBitmap);
	//}	
	//////////////////////////////////////////////////////////////////////////
	//分配区域
	CRect buyrectTop,buyrectBottom;
	buyrectTop =buyrectBottom= m_BuyRect;
	buyrectTop.bottom= buyrectBottom.top = buyrectTop.top+ buyrectTop.Height()/2;


	CRect sellrectTop,sellrectBottom;
	sellrectTop =sellrectBottom= m_SellRect;
	sellrectTop.bottom= sellrectBottom.top = sellrectTop.top+ sellrectTop.Height()/2;
	buyrectTop.left +=15;
	sellrectTop.left +=15;

	buyrectBottom.left += 2;
	sellrectBottom.left += 6;

	buyrectBottom.right += 2;
	sellrectBottom.right += 6;

	////////////////////////////////////////////////////////////////////////////
	//strPrice  = CLanguageInterface::Instance()->GetStr(20071);
	//pDC->DrawText(strPrice,sellrectTop,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	//strPrice  = CLanguageInterface::Instance()->GetStr(20072);
	//pDC->DrawText(strPrice,buyrectTop,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CPoint BuyIconPt,SellIconPt;
	BuyIconPt.x = buyrectTop.right -30;
	BuyIconPt.y = buyrectTop.top +8;

	SellIconPt.x = sellrectTop.right -30;
	SellIconPt.y = sellrectTop.top +8;
	switch(m_nUporDown)
	{
	case 0:
		{
			pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_RISE_COLOR));
			m_pImageList->Draw(pDC,0,BuyIconPt,ILD_TRANSPARENT);
			m_pImageList->Draw(pDC,0,SellIconPt,ILD_TRANSPARENT);
		}
		break;
	case 1:
		{
			pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_DOWN_COLOR));
			m_pImageList->Draw(pDC,1,BuyIconPt,ILD_TRANSPARENT);
			m_pImageList->Draw(pDC,1,SellIconPt,ILD_TRANSPARENT);
		}
		break;
	case 2:
		{
			pDC->SetTextColor(CInterfaceManager::Instance()->GetSysColor(ID_EQUAL_COLOR));
		}
		break;
	}


// 	CBrush textbrush;
// 	textbrush.CreateSolidBrush(CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR));//ID_PRICEBOARD_SINGLEBK));
// 	pDC->SelectObject(&textbrush);

	float fBuyPrice,fSellPrice;
	float fBuyUnit,fSellUnit;
	fBuyUnit = fSellUnit = 1;
	int nSort;
	CPublic::Instance()->GetCustomUnit(m_oCodeInfo.szCode,fBuyUnit,fSellUnit,nSort);
	fBuyPrice = (float)m_NowData.uiNewPrice/fPow + fBuyUnit;
	fSellPrice = (float)m_NowData.uiNewPrice/fPow + fSellUnit;
	strAskPrice = CPublic::Instance()->GetFormateValue(fBuyPrice,m_oCodeInfo.cPriceUnit);
	strBidPrice = CPublic::Instance()->GetFormateValue(fSellPrice,m_oCodeInfo.cPriceUnit); 
	//设置字体
	pDC->SelectObject(CInterfaceManager::Instance()->GetSysFont(ID_PRICEBOARD_PRICEBIGFONT));
	strPrice = strBidPrice;//卖出价格	
	pDC->DrawText(strPrice,sellrectBottom,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	strPrice  = strAskPrice;//买入价格
	pDC->DrawText(strPrice,buyrectBottom,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	
	pDC->SetBkMode(iOldDrawMode);
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(crOldTextColor);
	pDC->SelectObject(pOldPen);
	return TRUE;
}

void CBaseInfoItem::MoveTo(CRect rRect)
{
	m_TotalRect = rRect;
	m_TitleRect = rRect;
	m_TitleRect.top += 3;
	m_TitleRect.bottom = m_TitleRect.top+TITLEHEIGHT;

	m_MaxRect.left= m_MinRect.left= rRect.left+20;
	m_MaxRect.right= m_MinRect.right= rRect.right-20;
	m_MaxRect.top = m_TitleRect.bottom - 3;
	m_MaxRect.bottom = m_MaxRect.top+ PRICEHEIGHT;
	m_MinRect.top = m_MaxRect.bottom + 3;
	m_MinRect.bottom = m_MinRect.top+ PRICEHEIGHT;


//	m_BuyRect= m_SellRect = m_TotalRect;//买卖区域
//	m_BuyRect.top =m_SellRect.top = m_MinRect.bottom;
	
	CBitmap	BuySellBmp;
	BuySellBmp.LoadBitmap(IDB_BITMAP_BUYSELL);
	BITMAP bm1; 
	BuySellBmp.GetObject(sizeof(BITMAP),&bm1); //得到图片大小

	int nSplitteSide;
	//nSplitteSide = (m_TotalRect.Width()-bm1.bmWidth*2)/3;
	nSplitteSide = (m_TotalRect.Width()-bm1.bmWidth*2)/4;
	m_SellRect.left = m_TotalRect.left+2;//+ nSplitteSide;
	m_SellRect.right= m_SellRect.left+bm1.bmWidth;

	m_BuyRect.left =m_SellRect.right + nSplitteSide*2;
	m_BuyRect.right = m_BuyRect.left+bm1.bmWidth;

	nSplitteSide = (m_TotalRect.Height() -TITLEHEIGHT-2*PRICEHEIGHT -bm1.bmHeight)/2;

	m_SellRect.top = m_BuyRect.top = m_MinRect.bottom+nSplitteSide;
	m_SellRect.bottom = m_BuyRect.bottom = m_SellRect.top +bm1.bmHeight;

}

void CBaseInfoItem::GetItemWidthHeight(int& nWidth,int& nHeight)
{
	nWidth = ITEMWIDTH;
	nHeight =TITLEHEIGHT+2*PRICEHEIGHT+BOTTOMHEIGHT;
}

void CBaseInfoItem::GetRect(CRect& rt)
{
	rt = m_TotalRect;
}

BOOL CBaseInfoItem::HitTest(CPoint pt)
{
	return  m_TotalRect.PtInRect(pt);
}

void CBaseInfoItem::SetSelectStatus(BOOL bSel)
{
	m_bSelectStatus = bSel;
}

int CBaseInfoItem::HistRect(CPoint pt)
{
	if(m_SellRect.PtInRect(pt))return 1;
	if(m_BuyRect.PtInRect(pt))return 2;
	return 0;
}