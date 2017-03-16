// DlgSpotInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgSpotInfo.h"


// CDlgSpotInfo 对话框

IMPLEMENT_DYNAMIC(CDlgSpotInfo, CDialog)

CDlgSpotInfo::CDlgSpotInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSpotInfo::IDD, pParent)
{

}

CDlgSpotInfo::~CDlgSpotInfo()
{
}

void CDlgSpotInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SPOTLIST, m_statSpotLst);
}


BEGIN_MESSAGE_MAP(CDlgSpotInfo, CDialog)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_DATAARRIVE,OnDataArrive)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(M_DBCLICKITEM,OnDbClickItem)
	ON_MESSAGE(M_CLICKITEM,OnClickItem)
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
END_MESSAGE_MAP()


// CDlgSpotInfo 消息处理程序


BOOL CDlgSpotInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrl();
	SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSpotInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (IsWindow(m_statSpotLst.GetSafeHwnd()))
	{
		m_statSpotLst.MoveWindow(2,2,cx-4,cy-9);
	}
}

void CDlgSpotInfo::InitCtrl()
{
	CPublic::Instance()->RegistWnd(9,this,2);
	//STCTITEL stcTitle;
	//STCTITLEITEM stcItem;
	//stcTitle.iItemHight = 25;
	//stcTitle.iItemNum = 12;
	//stcTitle.bSort = TRUE;
	//stcTitle.iSortItem = 1;
	//stcTitle.eSort = 1;
	CString strHead[] = {_T("商品代码"),_T("商品名称"),_T("涨幅%"),_T("最新价"),_T("开盘价"),_T("买一价(数量)"),_T("卖一价(数量)"),
		_T("昨收价"),_T("昨结价"),_T("涨跌"),_T("最高价"),_T("最低价"),_T("订货量"),_T("最新量"),_T("总成交量"),
		_T("总成交额"),_T("总成交笔数")
		};
	m_statSpotLst.InsertColumn(18,15,0);
	int iWith = 10;
	int iAlignment = 0;
	for (int i=0; i < 17; i++)
	{
		iWith = 120;
		if (i == 5||i == 6)
		{
			iWith = 200;
		}
		if (i >= 2/*||i==3||i==5||i==6*/)
		{
			iAlignment = 2;
		}
		else
		{
			iAlignment = 0;
		}
		iWith = 10;
		m_statSpotLst.AddColumn(strHead[i],iWith,iAlignment);
		//stcItem.Init();
		//stcItem.iTitleItemId = i+1;
		//stcItem.strTitleText = strHead[i];/*.Format(_T("第 %d 列"),i+1);*/
		//stcItem.iItemWith = 150;
		//stcTitle.vecTitleItem.push_back(stcItem);
	}
	//m_statSpotLst.SetTitle(stcTitle);
	//m_statSpotLst.SetEveryPageCount(8);
	m_statSpotLst.SetItemHight(23);
	//SetLstDate();

	//OnDataArrive(PUSH_REALDATA_ZYQ,0);
}

void CDlgSpotInfo::SetLstDate()
{
	CRect rca;
	m_statSpotLst.GetClientRect(&rca);
	m_statSpotLst.ClearCtrl();
	USES_CONVERSION;
	int iItemW = 100;
	int x = 100;
	for (int i = 0; i < 50;i++)
	{
		int iItem = m_statSpotLst.InsertItem();
		for (int j = 0; j<12;j++)
		{
			CString strTemp;
			strTemp.Format(_T("显示字段%d-%d"),i,j);
			if (j%3 == 0)
			{
				m_statSpotLst.SetItemText(iItem,j+1,strTemp,2,Color::MakeARGB(255,255,0,0));
			}
			else if (j%5 == 0)
			{
				m_statSpotLst.SetItemText(iItem,j+1,strTemp,1,Color::MakeARGB(255,0,255,0));
			}
			else
				m_statSpotLst.SetItemText(iItem,j+1,strTemp);
		}
	}
	//for (int i = 0; i < 13; i++)
	//{
	//	STATLST ItemLst;
	//	ItemLst.bItmeCheck = FALSE;
	//	ItemLst.iItemId = i+1;
	//	ItemLst.iItemStat = eStatNormal;
	//	ItemLst.pTileText = NULL;
	//	ItemLst.iSubItemCount = 12;
	//	
	//	pSUBITEM psub = new SUBITEM[ItemLst.iSubItemCount];
	//	for (int j = 0; j < 12; j++)
	//	{
	//		psub[j].bSubItemCheck = 0;
	//		psub[j].iSubItemId = j+1;
	//		psub[j].iSubItemStat = eStatNormal;
	//		/////
	//		psub[j].iSubItemType = 1;
	//		psub[j].pSubItemText = new WCHAR[1024];
	//		CString strTemp;
	//		strTemp.Format(_T("显示字段%d-%d"),i,j);
	//		if (j==8)
	//		{
	//			strTemp.Format(_T("显示字段%d"),x--);
	//		}
	//		memcpy(psub[j].pSubItemText,T2W(strTemp.GetBuffer(0)),strTemp.GetLength()*2+2);
	//		psub[j].rcSubItem = CRect(iItemW*j,0,0,0);
	//	}
	//	ItemLst.pSuItem = psub;
	//	m_statSpotLst.AddItem(ItemLst);
	//}
}

void CDlgSpotInfo::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnMouseMove(nFlags, point);
}


BOOL CDlgSpotInfo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//SetCursor(LoadCursor(NULL,IDC_SIZEWE)); 
	//return TRUE;;
	//// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (pWnd->GetSafeHwnd() == m_statSpotLst.GetSafeHwnd())
	//{
	//	SetCursor(LoadCursor(NULL,IDC_SIZEWE)); 
	//	return TRUE;
	//}
	//if (GetDlgItem(IDC_BUTTON1) == pWnd )
	//{
	//	//SetSystemCursor(LoadCursor(NULL,IDC_SIZEWE),(UINT)IDC_ARROW);
	//	//SetCursor(LoadCursor(NULL,IDC_SIZEWE));  
	//	HCURSOR m_hcurStop=LoadCursor(NULL,IDC_SIZEWE);
	//	::SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)m_hcurStop);
	//	return TRUE;
	//}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CDlgSpotInfo::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rcLst;
	//m_statSpotLst.GetWindowRect(&rcLst);
	//if (rcLst.PtInRect(pt))
	//{
	//	::PostMessage(m_statSpotLst.GetSafeHwnd(),WM_MOUSEWHEEL,MAKELONG(nFlags,zDelta),NULL);
	//}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CDlgSpotInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//SetFocus();
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDlgSpotInfo::OnDataArrive(WPARAM wPar,LPARAM lPar)
{
	UINT iRet = 0;
	if (wPar == REPORTDATA_ZYQ)
	{
		mapTuNowDateZYQ maplst;
		mapTuNowDateZYQ::iterator mapIt;
		CDateManage::Instance()->GetRealQuoteInfo(maplst);
		int nDisplayIndex = 1;
		int i = 1;
		m_statSpotLst.ClearCtrl();
		for (mapIt = maplst.begin(); mapIt != maplst.end();mapIt++)
		{
			UpdateReportItem(nDisplayIndex,i,mapIt->second);
			i++;
			nDisplayIndex++;
		}
	}
	else if (wPar == PUSH_REALDATA_ZYQ)
	{
		mapTuNowDateZYQ maplst;
		mapTuNowDateZYQ::iterator mapIt;
		CDateManage::Instance()->GetRealQuoteInfo(maplst);
		int nDisplayIndex = 1;
		int i = 1;
		m_statSpotLst.ClearCtrl();
		for (mapIt = maplst.begin(); mapIt != maplst.end();mapIt++)
		{
			UpdateReportItem(nDisplayIndex,i,mapIt->second);
			i++;
			nDisplayIndex++;
		}
	}
	else if (wPar == OPEN_MAKRKET_ZYQ)
	{
		mapTuNowDateZYQ maplst;
		mapTuNowDateZYQ::iterator mapIt;
		CDateManage::Instance()->GetRealQuoteInfo(maplst);
		int nDisplayIndex = 1;
		int i = 1;
		m_statSpotLst.ClearCtrl();
		for (mapIt = maplst.begin(); mapIt != maplst.end();mapIt++)
		{
			UpdateReportItem(nDisplayIndex,i,mapIt->second);
			i++;
			nDisplayIndex++;
		}
	}
	else if (wPar == CLOSE_MAKRKET_ZYQ)
	{
		mapTuNowDateZYQ maplst;
		mapTuNowDateZYQ::iterator mapIt;
		CDateManage::Instance()->GetRealQuoteInfo(maplst);
		int nDisplayIndex = 1;
		int i = 1;
		m_statSpotLst.ClearCtrl();
		for (mapIt = maplst.begin(); mapIt != maplst.end();mapIt++)
		{
			UpdateReportItem(nDisplayIndex,i,mapIt->second);
			i++;
			nDisplayIndex++;
		}
	}
	return iRet;
}

void CDlgSpotInfo::UpdateReportItem(int nDisplayIndex,int nIndex,const STUNowDataZYQ &tCurRow,int iUpDateItem)
{
	//return;
	int iItem  = 0;
	if (iUpDateItem > 0)
	{
		iItem = m_statSpotLst.FindItem(iUpDateItem);
	}
	else
		iItem = m_statSpotLst.InsertItem();
	if (iItem == 0)return;
	//CString strHead[] = {_T("商品代码"),_T("商品名称"),_T("涨幅%"),_T("最新价"),_T("开盘价"),_T("买一价(数量)"),_T("卖一价(数量)"),
	//	_T("昨收价"),_T("昨结价"),_T("涨跌"),_T("最高价"),_T("最低价"),_T("订货量"),_T("最新量"),_T("总成交量"),
	//	_T("总成交额"),_T("总成交笔数")
	//};

	int j = 1;
	float fPow;
	int iUnit = (int)tCurRow.m_oSymbolInfo.m_cPriceUnit;
	if (iUnit > 10 || iUnit < 0)
	{
		iUnit = 0;
	}
	fPow = pow(10,(double)(iUnit/*tCurRow.m_oSymbolInfo.m_cPriceUnit*/));
	CString strPriceDecFormat;
	strPriceDecFormat.Format(_T("%%.%df"),iUnit);
	CString strFormatValue;
	CString strWightFormat,strNumFormat;
	strWightFormat.Format(_T("%%.%df"),3);
	strNumFormat.Format(_T("%%.%df"),2);
	int fPowNum = 100;

	CString strStandardVl;
	strStandardVl.Format(strPriceDecFormat,(float)tCurRow.m_uiYesSettlPrice/fPow);
	float fStandardPrice = (float)tCurRow.m_uiYesSettlPrice/fPow;
	float fCurPrice = (float)tCurRow.m_uiNewPrice/fPow;
	float fMix = fCurPrice - fStandardPrice;
	BOOL bZr = FALSE;
	if (IsZeroPrice(strPriceDecFormat,fCurPrice)||IsZeroPrice(strPriceDecFormat,fStandardPrice))
	{
		fMix = 0.0;
		bZr = TRUE;
	}
	CSkinAttribute_List *pSkin = (CSkinAttribute_List *)CSkinTransform::Instance()->GetSkinAttribute(1);
	m_statSpotLst.SetItemText(iItem,j++,CString(tCurRow.m_oSymbolInfo.m_szSymbol));
	m_statSpotLst.SetItemText(iItem,j++,CDateManage::Instance()->GetSymbolName(tCurRow.m_oSymbolInfo.m_szSymbol),0,pSkin->clrTextMark);

	if (fStandardPrice <= 0.0)
	{
		fStandardPrice = 1.0;
	}
	float fMixD = fMix/fStandardPrice;
	if (/*IsZeroPrice(strPriceDecFormat,fMixD)&&*/bZr)
	{
		//strFormatValue.Format(_T("%.2f%%"),fMixD*100);
		m_statSpotLst.SetItemText(iItem,j++,_T("--"),0,GetItemColor(fMixD));
	}
	else
	{
		strFormatValue.Format(_T("%.2f%%"),fMixD*100);
		m_statSpotLst.SetItemText(iItem,j++,strFormatValue,0,GetItemColor(fMixD));
	}
	

	strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_uiNewPrice/fPow);
	m_statSpotLst.SetItemText(iItem,j++,IsZeroPrice(strPriceDecFormat,strFormatValue),0,GetItemColor(strStandardVl,strFormatValue));
	strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_uiOpenPrice/fPow);
	m_statSpotLst.SetItemText(iItem,j++,IsZeroPrice(strPriceDecFormat,strFormatValue),0,GetItemColor(strStandardVl,strFormatValue));
	int iVolPriceCount = (int)tCurRow.m_cVolPriceCount;
	if (iVolPriceCount)
	{
		CString strTemp;
		strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_arrVolPrice[0].m_buyVol.m_uiPrice/fPow);
		strTemp = strFormatValue + _T("(");
		strFormatValue.Format(strNumFormat,(float)tCurRow.m_arrVolPrice[0].m_buyVol.m_uiQty/fPowNum);
		strTemp += strFormatValue + _T(")");
		m_statSpotLst.SetItemText(iItem,j++,strTemp,0,pSkin->clrTextMark);

		strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_arrVolPrice[0].m_sellVol.m_uiPrice/fPow);
		strTemp = strFormatValue + _T("(");
		strFormatValue.Format(strNumFormat,(float)tCurRow.m_arrVolPrice[0].m_sellVol.m_uiQty/fPowNum);
		strTemp += strFormatValue + _T(")");;
		m_statSpotLst.SetItemText(iItem,j++,strTemp,0,pSkin->clrTextMark);
	}
	else
	{
		CString strTemp = _T("-.--(-.--)");
		m_statSpotLst.SetItemText(iItem,j++,strTemp,0,pSkin->clrTextMark);
		m_statSpotLst.SetItemText(iItem,j++,strTemp,0,pSkin->clrTextMark);
	}
	
	strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_uiYesClosePrice/fPow);
	m_statSpotLst.SetItemText(iItem,j++,IsZeroPrice(strPriceDecFormat,strFormatValue),0,GetItemColor(strStandardVl,strFormatValue));
	strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_uiYesSettlPrice/fPow);
	m_statSpotLst.SetItemText(iItem,j++,IsZeroPrice(strPriceDecFormat,strFormatValue),0,GetItemColor(strStandardVl,strFormatValue));

	if (bZr)
	{
		strFormatValue.Format(strPriceDecFormat,fMix);
		m_statSpotLst.SetItemText(iItem,j++,IsZeroPrice(strPriceDecFormat,strFormatValue),0,GetItemColor(fMix));
	}
	else
	{
		strFormatValue.Format(strPriceDecFormat,fMix);
		m_statSpotLst.SetItemText(iItem,j++,strFormatValue,0,GetItemColor(fMix));
	}
	

	strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_uiMaxPrice/fPow);
	m_statSpotLst.SetItemText(iItem,j++,IsZeroPrice(strPriceDecFormat,strFormatValue),0,GetItemColor(strStandardVl,strFormatValue));
	strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_uiMinPrice/fPow);
	m_statSpotLst.SetItemText(iItem,j++,IsZeroPrice(strPriceDecFormat,strFormatValue),0,GetItemColor(strStandardVl,strFormatValue));

	//strPriceDecFormat = (_T("%f"));
	strFormatValue.Format(strNumFormat,(float)tCurRow.m_iTotalOrderQty/fPowNum);
	m_statSpotLst.SetItemText(iItem,j++,strFormatValue,0,pSkin->clrTextMark);
	strFormatValue.Format(strNumFormat,(float)tCurRow.m_uiNewQty/fPowNum);
	m_statSpotLst.SetItemText(iItem,j++,strFormatValue,0,pSkin->clrTextMark);
	strFormatValue.Format(strNumFormat,(float)tCurRow.m_iTotalTrunoverQty/fPowNum);
	m_statSpotLst.SetItemText(iItem,j++,strFormatValue,0,pSkin->clrTextMark);
	strFormatValue.Format(strPriceDecFormat,(float)tCurRow.m_iTotalTrunoverMoney/fPow);
	m_statSpotLst.SetItemText(iItem,j++,strFormatValue,0,Color::MakeARGB(255,188,188,188));
	strFormatValue.Format(_T("%d"),(int)tCurRow.m_iTotalTrunoverCount);
	m_statSpotLst.SetItemText(iItem,j++,strFormatValue,0,pSkin->clrTextMark);

	m_statSpotLst.UpDateClient();
}
BOOL CDlgSpotInfo::IsZeroPrice(CString strFormat,float fCurPirce)
{
	BOOL bRet = FALSE;
	CString strZero,strCurPirce;
	strZero.Format(strFormat,0.0);
	strCurPirce.Format(strFormat,fCurPirce);
	if (strZero.CompareNoCase(strCurPirce) == 0)
	{
		bRet = TRUE;
	}
	return bRet;
}
CString CDlgSpotInfo::IsZeroPrice(CString strFormat,CString strCurPirce)
{
	CString strRet = strCurPirce;
	CString strZero;
	strZero.Format(strFormat,0.0);
	if (strZero.CompareNoCase(strCurPirce) == 0)
	{
		strRet = _T("--");
	}
	return strRet;
}
DWORD CDlgSpotInfo::GetItemColor(float fCurP)
{
	DWORD crText;
	CSkinAttribute_Fund *pSkin = (CSkinAttribute_Fund *)CDateManage::Instance()->GetSkinAttribute(4);
	if(fCurP < 0.0)
	{
		crText = pSkin->clrTextDown;//Color::MakeARGB(255,0,255,0);
	}
	else if(fCurP > 0.0)
	{
		crText = pSkin->clrTextUp;//Color::MakeARGB(255,255,0,0);
	}
	else
	{
		crText = pSkin->clrTextNormal;//Color::MakeARGB(255,188,188,188);
	}

	return crText;
}

DWORD CDlgSpotInfo::GetItemColor(CString strStandardVl, CString strCurVl)
{
	DWORD crText;
	CSkinAttribute_Fund *pSkin = (CSkinAttribute_Fund *)CDateManage::Instance()->GetSkinAttribute(4);
	int IsEqual = strStandardVl.CompareNoCase(strCurVl);
	if(IsEqual > 0)
	{
		crText = pSkin->clrTextDown;
	}
	else if(IsEqual < 0)
	{
		crText = pSkin->clrTextUp;
	}
	else
	{
		crText = pSkin->clrTextNormal;
	}

	return crText;
}

BOOL CDlgSpotInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClt;
	GetClientRect(&rcClt);
	Gdiplus::Bitmap cachBmp(rcClt.Width(),rcClt.Height());
	Graphics graphicsDraw(pDC->m_hDC);
	Graphics graphics(&cachBmp);
	
	CDateManage::Instance()->DrawBkRect(graphics,rcClt);

	CSkinAttribute_FrameLine * pSkinFl = (CSkinAttribute_FrameLine *)CSkinTransform::Instance()->GetSkinAttribute(3);
	Pen penBrd1(pSkinFl->clrBkLineOut);
	graphics.DrawRectangle(&penBrd1,Rect(rcClt.left,rcClt.top,rcClt.Width()-1,rcClt.Height()-6));
	Pen penBrd2(pSkinFl->clrBkLineIn);
	graphics.DrawRectangle(&penBrd2,Rect(rcClt.left+1,rcClt.top+1,rcClt.Width()-3,rcClt.Height()-8));

	graphicsDraw.DrawImage(&cachBmp,0,0/*rcClt.top*/);
	graphicsDraw.ReleaseHDC(pDC->m_hDC);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}
LRESULT CDlgSpotInfo::OnDbClickItem(WPARAM wParam,LPARAM lParm)
{
	CWnd *pWnd = CPublic::Instance()->GetView(13,2);
	CString strCode = m_statSpotLst.GetItemText(wParam,1);
	mapTuNowDateZYQ maplst;
	mapTuNowDateZYQ::iterator mapIt;
	CDateManage::Instance()->GetRealQuoteInfo(maplst);
	mapIt = maplst.find(strCode);
	if(mapIt != maplst.end())
	{
		STUSymbolDataZYQ stcSymbolInfo = CDateManage::Instance()->GetSymbolInfo(strCode);
		//GetParent()->SendMessage(WM_RETLST,1,0);
		if (pWnd != NULL &&IsWindow(pWnd->GetSafeHwnd()))
		{
			pWnd->SendMessage(WM_CHANGTRENDCHART,(WPARAM)&stcSymbolInfo,0);
		}
	}
	return 1;
	//int iRet = 1;
	//SCodeInfo* pCurSelCode ;//= GetCurSelCodeInfo(wParam, lParm);

	//CString strCode = m_statSpotLst.GetItemText(wParam,1);
	//mapTuNowDateZYQ maplst;
	//mapTuNowDateZYQ::iterator mapIt;
	//CDateManage::Instance()->GetRealQuoteInfo(maplst);
	//mapIt = maplst.find(strCode);
	//if(mapIt != maplst.end())
	//{
	//	STUSymbolDataZYQ stcSymbolInfo = CDateManage::Instance()->GetSymbolInfo(strCode);
	//	if(/*stcSymbolInfo.m_cTrade == SYMBOLTYPE_TRADEPRODUCT&&*/CPublic::Instance()->GetTradeDll()->m_pGetUserStatus)
	//	{	
	//		if(0 == CPublic::Instance()->GetTradeDll()->m_pQuickOrder(0,strCode.GetBuffer(0),0,1))
	//		{
	//			::MessageBox(this->GetSafeHwnd(),CLanguageInterface::Instance()->GetStr(20067),CLanguageInterface::Instance()->GetStr(20068),MB_OK|MB_ICONWARNING);
	//		}
	//	}
	//}
	//return iRet;
}
LRESULT CDlgSpotInfo::OnClickItem(WPARAM wParam,LPARAM lParm)
{
	int iRet = 1;
	SCodeInfo* pCurSelCode ;//= GetCurSelCodeInfo(wParam, lParm);

	CString strCode = m_statSpotLst.GetItemText(wParam,1);
	mapTuNowDateZYQ maplst;
	mapTuNowDateZYQ::iterator mapIt;
	CDateManage::Instance()->GetRealQuoteInfo(maplst);
	mapIt = maplst.find(strCode);
	if(mapIt != maplst.end())
	{
		STUSymbolDataZYQ stcSymbolInfo = CDateManage::Instance()->GetSymbolInfo(strCode);
		if(/*stcSymbolInfo.m_cTrade == SYMBOLTYPE_TRADEPRODUCT&&*/CPublic::Instance()->GetTradeDll()->m_pGetUserStatus)
		{	
			if(0 == CPublic::Instance()->GetTradeDll()->m_pQuickOrder(0,strCode.GetBuffer(0),0,1))
			{
				::MessageBox(this->GetSafeHwnd(),CLanguageInterface::Instance()->GetStr(20067),CLanguageInterface::Instance()->GetStr(20068),MB_OK|MB_ICONWARNING);
			}
		}
	}
	return iRet;
}
LRESULT CDlgSpotInfo::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	m_statSpotLst.UpDateCtrl(1);
	return 1;
}