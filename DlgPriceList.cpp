// DlgPriceList.cpp : implementation file
//

#include "stdafx.h"
#include "KOAClient.h"
#include "DlgPriceList.h"
#include "afxdialogex.h"


// CDlgPriceList dialog

IMPLEMENT_DYNAMIC(CDlgPriceList, CDialog)

CDlgPriceList::CDlgPriceList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPriceList::IDD, pParent)
{
	m_vectCodeRowInfo.clear();
	m_vecNowData.clear();
	m_vecShowData.clear();
	m_nTotalLine=0;

	m_iRightWith = 11;
	m_iWndReArrayType = 0;
	m_pCurCodeInfo = NULL;
}

CDlgPriceList::~CDlgPriceList()
{
	if(m_pCurCodeInfo)
	{
		delete m_pCurCodeInfo;
		m_pCurCodeInfo = NULL;
	}
}

void CDlgPriceList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LST, m_statLst);
}


BEGIN_MESSAGE_MAP(CDlgPriceList, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_DATAARRIVE,OnDataMessage)
	ON_MESSAGE(M_CLICKITEM,OnClickItem)
	ON_MESSAGE(M_DBCLICKITEM,OnDbClickItem)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(M_MODIFYTITLEITEM,OnModifyTitleItem)
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_SKINCHANGE,OnSkinChange)
	ON_MESSAGE(WM_STOCKCHANGE,OnStockChange)
END_MESSAGE_MAP()


// CDlgPriceList message handlers


void CDlgPriceList::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//CRect rcClt(2,2,cx-6,cy-4);
	//SetWndPos(rcClt);

	if (!IsWindow(m_statLst.GetSafeHwnd()))return;
	//m_statLst.MoveWindow(2,2,cx-4,cy-4);
	m_statLst.MoveWindow(2,2,cx-m_iRightWith-4,cy-4);
}


BOOL CDlgPriceList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPriceList::InitCtrl()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE|LVS_LIST| WS_CLIPCHILDREN|WM_NOTIFY;;
	//if (!m_PriceListWnd.Create(dwViewStyle, rectDummy, this, 4))
	//{
	//	TRACE0("未能创建文件视图\n");
	//	return ;      // 未能创建
	//}
	CString strHead[] = {_T("商品代码"),_T("商品名称"),_T("卖出价"),_T("买入价"),_T("报价时间"),_T("最高价"),_T("最低价")};
	m_statLst.InsertColumn(18,7,0); 
	int iWith = 120;
	int iAlignment = 0;
	int *pItemWith = CPublic::Instance()->GetQuoteLstItemWith();
	for (int i=0; i < 7; i++)
	{ 
		//if (i == 4)
		//{
		//	iWith = 120;
		//}
		//else
		//{
		//	iWith = 100;
		//}
		if (i >= 2/*||i==3||i==5||i==6*/)
		{
			iAlignment = 2;
		}
		else
		{
			iAlignment = 0;
		}
		m_statLst.AddColumn(strHead[i],pItemWith[i],iAlignment);

	}
	m_statLst.SetItemHight(23);
	//m_statLst.ShowWindow(SW_HIDE);
	//m_PriceListWnd.ShowWindow(SW_HIDE);

	CRect rcClt;
	GetClientRect(&rcClt);
	SetWndPos(rcClt);
	CPublic::Instance()->RegistWnd(6,this);
}

void CDlgPriceList::SetWndPos(CRect &rcWnd)
{
	//if (IsWindow(m_PriceListWnd.GetSafeHwnd()))
	//{
	//	m_PriceListWnd.SetWindowPos(NULL, rcWnd.left , rcWnd.top , rcWnd.Width(), rcWnd.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	//	m_PriceListWnd.SetFocus();
	//}
}

LRESULT CDlgPriceList::OnDataMessage(WPARAM wParam,LPARAM lParm)
{
	int iRet = 1;
    //m_PriceListWnd.UpdateData((char*)wParam,(int)lParm);
	m_lock.Lock();
	UpdateData2((char*)wParam,(int)lParm);
	m_lock.Unlock();
	return iRet;
}

void CDlgPriceList::UpdateNowData(const SUpdateData *pNow,TCodeRowInfo &rtRowInfo,int nUpDown)
{
	strcpy(rtRowInfo.tCodeInfo.szCode,pNow->oNowData.sCode.szCode);//名称
	rtRowInfo.ssSymbolName = pNow->ssSymbolName;//商品名字

	rtRowInfo.nImage = nUpDown;
	const StuSymbolData* pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow->oNowData.sCode);	
	if(NULL == pCodeInfo)return;
	COLORREF crText;
	//	COLORREF crBack;

	CSkinAttribute_Fund * pSkinFund = (CSkinAttribute_Fund*)CDateManage::Instance()->GetSkinAttribute(4);
	if(1 == nUpDown)
	{
		crText = pSkinFund->clrTextDown;//Color::MakeARGB(255,0,255,0);
	}
	else if(0 == nUpDown)
	{
		crText = pSkinFund->clrTextUp;//Color::MakeARGB(255,255,0,0);
	}
	else
	{
		crText = pSkinFund->clrTextNormal;//Color::MakeARGB(255,255,255,255);
	}

	CString strPriceDecFormat;

	strPriceDecFormat.Format(_T("%%.%df"),CPublic::Instance()->GetDecimal(&pCodeInfo->sCode));

	float fBuyUnit,fSellUnit;
	int nSort;
	nSort = -1;
	CPublic::Instance()->GetCustomUnit(pNow->oNowData.sCode.szCode,fBuyUnit,fSellUnit,nSort);


	float fPow;
	fPow = pow(10,(double)pCodeInfo->sCode.cPriceUnit);

	vector<TReportItemInfo>::iterator it = rtRowInfo.vecColValue.begin();

	it->fValue =  (float)pNow->oNowData.uiNewPrice/fPow+fSellUnit;//卖出
	it->strFormatValue.Format(strPriceDecFormat,it->fValue);
	it->crTextColor = crText;
	it++;


	it->fValue = (float)pNow->oNowData.uiNewPrice/fPow +fBuyUnit;//买入
	it->strFormatValue.Format(strPriceDecFormat,it->fValue);
	it->crTextColor = crText;
	/////////
	it++;
	CTime  mytime(pNow->oNowData.timeUpdate);

	it->fValue  = 0.00f;
	//it->strFormatValue = mytime.Format(_T("%H:%M:%S"));
	CString strTmp;
	strTmp = mytime.Format(_T("%H:%M:%S"));
	if (strTmp == "00:00:00")
		strTmp = "--:--:--";
	it->strFormatValue = strTmp;
	it->crTextColor = crText;
	//////////////
	it++;

	it->fValue =  (float)pNow->oNowData.uiMaxPrice/fPow;//最高
	it->strFormatValue.Format(strPriceDecFormat,it->fValue);
	it->crTextColor = crText;
	it++;

	it->fValue =  (float)pNow->oNowData.uiMinPrice/fPow;//最低
	it->strFormatValue.Format(strPriceDecFormat,it->fValue);
	it->crTextColor = crText;

}
void CDlgPriceList::TransData2Table()
{
	TCodeRowInfo codeinfo;
	codeinfo.vecColValue.resize(6);//设置长度
	m_vectCodeRowInfo.clear();
	int nDispalayNum;
	
	for(int nCount=0;nCount<m_vecShowData.size();nCount++)
	{
		UpdateNowData(&m_vecShowData[nCount],codeinfo,m_vecShowData[nCount].nUpDown);
		m_vectCodeRowInfo.push_back(codeinfo);
	}
	UpdateReport(m_vectCodeRowInfo);

	SetLstCurSel(CPublic::Instance()->GetCurStock());
}

void CDlgPriceList::UpdateReport(const vector<TCodeRowInfo> &vecCodeRowInfo)
{
	//m_lock.Lock();
	m_statLst.ClearCtrl();

	int nDisplayIndex = 1;
	int i = 1;
	vector<TCodeRowInfo>::const_iterator iterCurRow;
	for( iterCurRow = vecCodeRowInfo.begin();
		iterCurRow != vecCodeRowInfo.end();
		iterCurRow++)
	{
		UpdateReportItem(nDisplayIndex,i,*iterCurRow);	
		i++;
		nDisplayIndex++;
	}
	//m_lock.Unlock();
}

void CDlgPriceList::UpdateReportItem(int nDisplayIndex,int nIndex,const TCodeRowInfo &tCurRow,int iUpDateItem)
{
	//return;
	int iItem  = 0;
	if (iUpDateItem > 0)
	{
		iItem = m_statLst.FindItem(iUpDateItem);
	}
	else
		iItem = m_statLst.InsertItem();
	if (iItem == 0)return;

	int j = 1;

	CSkinAttribute_List *pSkin = (CSkinAttribute_List *)CSkinTransform::Instance()->GetSkinAttribute(1);

	m_statLst.SetItemText(iItem,j++,CString(tCurRow.tCodeInfo.szCode)/*,0,Color::MakeARGB(255,0,255,255)*/);
	m_statLst.SetItemText(iItem,j++,CString(tCurRow.ssSymbolName.c_str()),0,pSkin->clrTextMark);

	int iAlignment = 2;
	vector<TReportItemInfo>::const_iterator iterCurRowItem;
	for( iterCurRowItem = tCurRow.vecColValue.begin();
		iterCurRowItem != tCurRow.vecColValue.end();
		iterCurRowItem++)
	{
		m_statLst.SetItemText(iItem,j++,iterCurRowItem->strFormatValue,iAlignment,iterCurRowItem->crTextColor);
	}

	m_statLst.UpDateClient();
}
//void ReArrayVector(vector<int> &vecRA)
//{
//	vector<int> vecTemp;
//	vector<int>::iterator itVec;
//	if (vecRA.size() <= 0)return;
//	vecTemp.insert(vecTemp.end(),vecRA.begin(),vecRA.end());
//	int iMin = 100;
//	int iTempIndex = 0;
//	int iValue = 0;
//	int iNum = vecTemp.size();
//	for (int i = 0; i < iNum;i++)
//	{
//		int j = 0;
//		iMin = 100;
//		for (itVec = vecTemp.begin();itVec != vecTemp.end();itVec++)
//		{
//			iValue = *itVec;
//			if (iValue < iMin &&iValue != -1)
//			{
//				iMin = iValue;
//				iTempIndex = j;
//			}
//			j++;
//		}
//		vecTemp[iTempIndex] = -1;
//		vecRA[iTempIndex] = i;
//	}
//}
BOOL CDlgPriceList::UpdateData2(LPVOID lpData,int nDataLen)
{
	
	SNormaHead *pDataHead = (SNormaHead*)lpData;

	if(pDataHead->m_usType == REPORTDATA)
	{	
		SNowData* pNow = (SNowData*)(pDataHead+1);
		int nCount;
		int nNum;
		nNum = 0;
		const StuSymbolData* pCodeInfo;
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			nNum++;
		}
		int nPostion = 0;

		vector<int> vecSort;
		vecSort.resize(nNum);
		for(int i=0;i < vecSort.size();i++)
			vecSort[i] = -1;

		float fBuy,fSell;
		int nTradeProductNum = 0;//可交易产品数量
		//此处统计可交易产品位置
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '1')
			{
				nPostion = 0;
				CPublic::Instance()->GetTradeDll()->m_pGetCustomUnit(pNow[nCount].sCode.szCode,fBuy,fSell,nPostion);
				//if(nPostion == 0)
				//{
				//	vecSort[nTradeProductNum]= nTradeProductNum;
				//}
				//else
				//{
				//	vecSort[nTradeProductNum] = nPostion-1;
				//}

				//nTradeProductNum++;
				if(nPostion == 0)
				{
					//vecSort[nTradeProductNum]= nTradeProductNum;
					CPublic::Instance()->SetStockTradeStatus(&pNow[nCount].sCode);
				}
				else
				{
					vecSort[nTradeProductNum] = nPostion-1;
					//	}
					nTradeProductNum++;
				}
			}
		}
		nTradeProductNum = 0;
		//此处统计非交易产品位置
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{	
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '0')
			{
				int i = 0;

				for(i = 0;i < vecSort.size(); i++)
				{
					if(vecSort[i] == nTradeProductNum)
					{
						nTradeProductNum++;
						i = -1;
					}
				}

				for(i = 0;i < vecSort.size(); i++)
				{
					if (vecSort[i] == -1)
					{
						vecSort[i] = nTradeProductNum;
						break;
					}
				}
				nTradeProductNum++;
			}
		}
		CDateManage::Instance()->ReArrayVector(vecSort);
		m_vecNowData.resize(nNum);	
		int nStation =0;
		CString strTmp;
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '1')
			{
				nPostion = vecSort[nStation];

				m_vecNowData[nPostion].oNowData = pNow[nCount];
				m_vecNowData[nPostion].nUpDown = -1;
				m_vecNowData[nPostion].ssSymbolName = pCodeInfo->szName;
				m_vecNowData[nPostion].cIsTradeSymbol = pCodeInfo->cTrade;
				nStation++;
			}
		}
		for(nCount =0;nCount<pDataHead->m_usSize;nCount++)
		{
			pCodeInfo = CPublic::Instance()->GetStockInfo(&pNow[nCount].sCode);
			if(NULL == pCodeInfo)continue;
			if(pCodeInfo->cTrade == '0')
			{
				nPostion = vecSort[nStation];

				m_vecNowData[nPostion].oNowData = pNow[nCount];
				m_vecNowData[nPostion].nUpDown = -1;
				m_vecNowData[nPostion].ssSymbolName = pCodeInfo->szName;
				m_vecNowData[nPostion].cIsTradeSymbol = pCodeInfo->cTrade;
				nStation++;
			}
		}
		m_nTotalLine = nNum;
		m_vecShowData = m_vecNowData;
		TransData2Table();
		//SetCurrentSelLine(0);
	}
	else if(pDataHead->m_usType == PUSH_REALDATA)
	{
		//return TRUE;
		if((m_vectCodeRowInfo.empty())||m_vecShowData.empty())return FALSE;
		StuRecvQuoteInfo* pRecvQuote= (StuRecvQuoteInfo*)(pDataHead+1);
		//		TRACE("pointer: %x\r\n",pRecvQuote);
		CString strTmp;
		int nStation;		
		//查找行列数
		for(int iCount =0;iCount<pDataHead->m_usSize;iCount++)
		{
			StuRecvQuoteInfo* pQuote = (StuRecvQuoteInfo*)(pRecvQuote+iCount);
			if(!FindStation(&pQuote->sCode,nStation))continue;
			UpdateRealData(pQuote,m_vecShowData[nStation]);

			UpdateNowData(&m_vecShowData[nStation],m_vectCodeRowInfo[nStation],m_vecShowData[nStation].nUpDown);
			UpdateReportItem(0,0,m_vectCodeRowInfo[nStation],nStation+1);

		}
	}	
	return TRUE;
}

void CDlgPriceList::UpdateRealData(const StuRecvQuoteInfo* pReal,SUpdateData &oUpdateData)
{

	//转换
	if(pReal->uiPrice > oUpdateData.oNowData.uiNewPrice)
		oUpdateData.nUpDown = 0;
	else if(pReal->uiPrice < oUpdateData.oNowData.uiNewPrice)
		oUpdateData.nUpDown = 1;
	else
		oUpdateData.nUpDown = -1;
	oUpdateData.oNowData.uiNewPrice = pReal->uiPrice;//最新价
	if(pReal->uiPrice >oUpdateData.oNowData.uiMaxPrice)//最高价
		oUpdateData.oNowData.uiMaxPrice = pReal->uiPrice;
	if(pReal->uiPrice <oUpdateData.oNowData.uiMinPrice)//最低价
		oUpdateData.oNowData.uiMinPrice = pReal->uiPrice;

	oUpdateData.oNowData.timeUpdate = pReal->lastTick;
}

bool CDlgPriceList::FindStation(SCodeInfo* pCode,int& nStation)
{
	for(nStation = 0;nStation < m_vecShowData.size();nStation++)
	{
		if( 0 == strcmp(m_vecShowData[nStation].oNowData.sCode.szCode,pCode->szCode))
			return true;
	}
	return false;
}

BOOL CDlgPriceList::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClt;
	GetClientRect(&rcClt);
	Gdiplus::Bitmap cachBmp(rcClt.Width(),rcClt.Height());
	Graphics graphicsDraw(pDC->m_hDC);
	Graphics graphics(&cachBmp);
	
	CDateManage::Instance()->DrawBkRect(graphics,rcClt);

	CSkinAttribute_Gap *pSkin = (CSkinAttribute_Gap *)CSkinTransform::Instance()->GetSkinAttribute(2);
	Gdiplus::LinearGradientBrush lineGr(Point(rcClt.right-m_iRightWith-1,rcClt.top),Point(rcClt.right,rcClt.top),
		pSkin->clrBk1,pSkin->clrBk2);
	//Gdiplus::LinearGradientBrush lineGr2(Point(rcClt.right-m_iRightWith/2-1,rcClt.top),Point(rcClt.right+m_iRightWith,rcClt.top),
	//	pSkin->clrBk2,pSkin->clrBk1);
	graphics.FillRectangle(&lineGr,Rect(rcClt.right-m_iRightWith,rcClt.top,m_iRightWith,rcClt.Height()));
	//graphics.FillRectangle(&lineGr2,Rect(rcClt.right-m_iRightWith/2-1,rcClt.top,m_iRightWith/2+1,rcClt.Height()));

	CSkinAttribute_FrameLine * pSkinFl = (CSkinAttribute_FrameLine *)CSkinTransform::Instance()->GetSkinAttribute(3);
	Pen penBrd1(pSkinFl->clrBkLineOut);
	graphics.DrawRectangle(&penBrd1,Rect(rcClt.left,rcClt.top,rcClt.Width()-1-m_iRightWith,rcClt.Height()-1));
	Pen penBrd2(pSkinFl->clrBkLineIn);
	graphics.DrawRectangle(&penBrd2,Rect(rcClt.left+1,rcClt.top+1,rcClt.Width()-3-m_iRightWith,rcClt.Height()-3));

	graphicsDraw.DrawImage(&cachBmp,0,0/*rcClt.top*/);
	graphicsDraw.ReleaseHDC(pDC->m_hDC);

	CRect rc1(rcClt.right-m_iRightWith+1,rcClt.top+rcClt.Height()/2-42-4,rcClt.right-1,rcClt.top+rcClt.Height()/2-4);
	CRect rc2(rcClt.right-m_iRightWith+1,rcClt.top+rcClt.Height()/2+4,rcClt.right-1,rcClt.top+rcClt.Height()/2+42+4);
	m_rcLeftHide = rc1;
	m_rcRightHide = rc2;
	DrawLeftHide();
	DrawRightHide();

	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}
void CDlgPriceList::DrawLeftHide(int iStat)
{
	CDateManage::Instance()->DrawLeftHide(this,m_rcLeftHide,iStat);
}
void CDlgPriceList::DrawRightHide(int iStat)
{
	CDateManage::Instance()->DrawRightHide(this,m_rcRightHide,iStat);
}
LRESULT CDlgPriceList::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect,rcLeftHide,rcRightHide;  
	GetWindowRect(&rect);
	rcLeftHide = m_rcLeftHide;
	rcRightHide = m_rcRightHide;
	ClientToScreen(&rcLeftHide);
	ClientToScreen(&rcRightHide);
	if (rcLeftHide.PtInRect(point))
	{
		DrawLeftHide(1);
		return CWnd::OnNcHitTest(point);  
	}
	if (rcRightHide.PtInRect(point))
	{
		DrawRightHide(1);
		return CWnd::OnNcHitTest(point);  
	}
	DrawLeftHide();
	DrawRightHide();
	if (point.x >= rect.right-m_iRightWith)  
		return HTRIGHT;  
	else  
	{  
		return CWnd::OnNcHitTest(point);  
	}  
	return 0;  
	//return CDialog::OnNcHitTest(point);
}


void CDlgPriceList::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest == HTRIGHT)  
	{
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y)); 
		::SendMessage(GetParent()->GetSafeHwnd(),WM_WNDSIZECHANGE,1,0);
	}
	//CDialog::OnNcLButtonDown(nHitTest, point);
}


void CDlgPriceList::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	//CRect rcClt;
	//GetClientRect(&rcClt);
	//if(NULL == pWnd) return ; //防止指针空	
	//CPoint ptClient;
	//ptClient = point;
	//ScreenToClient(&ptClient);
	//rcClt.InflateRect(-5,-5,-5,-5);
	//if (!rcClt.PtInRect(ptClient))
	//{
	//	return;
	//}
	//CMenu popMenu;
	//m_PriceListWnd.RButtonDown(ptClient);
	//popMenu.LoadMenu(IDR_RIGHTMENU); //装载菜单资源
	//CMenu* pMenu;
	//pMenu = popMenu.GetSubMenu(POPSUBMENU_INDEX_REPORTTABLE);
	//CLanguageInterface::Instance()->InitRightMenu(pMenu,POPSUBMENU_INDEX_REPORTTABLE);
	//pMenu->CheckMenuItem(ID_QUOTE_SHOWHIGHLOW,m_PriceListWnd.GetShowHighLowStatus()?MF_CHECKED:MF_UNCHECKED);
	//pMenu->CheckMenuItem(ID_QUOTE_SHOWUPDATETIME,m_PriceListWnd.GetShowUpdateTimeStatus()?MF_CHECKED:MF_UNCHECKED);
	//pMenu->CheckMenuItem(ID_QUOTE_SHOWOTHERQUOTE,m_PriceListWnd.GetShowOtherSymbol()?MF_CHECKED:MF_UNCHECKED);
	//UINT uiCmd;
	//uiCmd = pMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN|TPM_RETURNCMD
	//	,point.x,point.y,AfxGetMainWnd()); //弹出菜单
	//popMenu.DestroyMenu(); //销毁菜单

	//if((uiCmd == ID_QUOTE_SHOWUPDATETIME))
	//{
	//	m_PriceListWnd.ShowHideUpdateTime();
	//}
	//else if(uiCmd == ID_QUOTE_SHOWHIGHLOW)
	//{
	//	m_PriceListWnd.ShowHideHighLow();
	//}
	//else if(uiCmd == ID_QUOTE_SHOWOTHERQUOTE)
	//{
	//	m_PriceListWnd.ShowHideOtherSymbol();
	//}
	//else
	//{
	//	SCodeInfo* pCode;
	//	pCode = m_PriceListWnd.GetCurSel();
	//	CPublic::Instance()->CallStockMenu(uiCmd,pCode);
	//}
	//m_PriceListWnd.RedrawWindow();
}

BOOL CDlgPriceList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE) && pMsg->message == WM_KEYDOWN)
	{
		return 1;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CDlgPriceList::OnDbClickItem(WPARAM wParam,LPARAM lParm)
{
	int iRet = 1;
	SCodeInfo* pCurSelCode = GetCurSelCodeInfo(wParam, lParm);
	if (pCurSelCode == NULL)return iRet;

	CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
	const StuSymbolData* pSymbolInfo = CPublic::Instance()->GetStockInfo(pCurSelCode);

	if(pSymbolInfo->cTrade == SYMBOLTYPE_TRADEPRODUCT&&CPublic::Instance()->GetTradeDll()->m_pGetUserStatus)
	{	
		/*nBuySellSign =(nBuySellSign==1)?1:0;*/
		CPublic::Instance()->CallQuickTrade(pCurSelCode,((lParm == 3)?1:0));
	}
	
	return iRet;
}
LRESULT CDlgPriceList::OnClickItem(WPARAM wParam,LPARAM lParm)
{
	//return 1;
	int iRet = 1;
	SCodeInfo* pCurSelCode = GetCurSelCodeInfo(wParam, lParm);
	if (pCurSelCode == NULL)return iRet;

	CPublic::Instance()->ChangeStockEvent(pCurSelCode,TechObj);
	const StuSymbolData* pSymbolInfo = CPublic::Instance()->GetStockInfo(pCurSelCode);

	return 1;
	if(pSymbolInfo->cTrade == SYMBOLTYPE_TRADEPRODUCT&&CPublic::Instance()->GetTradeDll()->m_pGetUserStatus)
	{	
		/*nBuySellSign =(nBuySellSign==1)?1:0;*/
		CPublic::Instance()->CallQuickTrade(pCurSelCode,((lParm == 3)?1:0));
	}
	return iRet;
}
SCodeInfo* CDlgPriceList::GetNextSelCodeInfo(const SCodeInfo* pCurrentStock,int iMode)
{
	int iSel = GetCurSel(pCurrentStock);
	int iNum = m_vectCodeRowInfo.size();
	SCodeInfo* pNextSCode = NULL;
	
	if (iSel >= 0 && iNum > 0)
	{
		int iNext = iSel+iMode;
		if (iNext < iNum && iNext >= 0)
		{
			pNextSCode =  &m_vectCodeRowInfo[iNext].tCodeInfo;
		}
		else
		{
			if (iNext < 0)pNextSCode = &m_vectCodeRowInfo[iNum - 1].tCodeInfo;
			if (iNext >= iNum)pNextSCode = &m_vectCodeRowInfo[0].tCodeInfo;
		}
	}
	CPublic::Instance()->ChangeStockEvent(pNextSCode,TechObj);
	return NULL;
	//m_statLst.get
}
SCodeInfo* CDlgPriceList::GetCurSelCodeInfo(int iItem,int iSubItem)
{
	SCodeInfo* pCurSelCode = NULL;
	int iNum = m_vectCodeRowInfo.size();
	if (iItem <= 0 || iSubItem <= 0 || iItem > iNum ) return pCurSelCode;

	return &m_vectCodeRowInfo[iItem-1].tCodeInfo;
}
LRESULT CDlgPriceList::OnModifyTitleItem(WPARAM wPar,LPARAM lPar)
{
	CPublic::Instance()->WriteQuoteLstItemWith(wPar,lPar);
	return 1;
}

void CDlgPriceList::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL bResh = FALSE;
	do 
	{
		if (m_rcLeftHide.PtInRect(point))
		{
			bResh = TRUE;
			GetParent()->PostMessage(WM_WNDREARRAY,1,0);
			break;
		}
		if (m_rcRightHide.PtInRect(point))
		{
			bResh = TRUE;
			GetParent()->PostMessage(WM_WNDREARRAY,2,0);
			break;
		}
	} while (0);

	if (bResh)
	{
		Invalidate();
	}
	CDialog::OnLButtonUp(nFlags, point);
}
LRESULT CDlgPriceList::OnSkinChange(WPARAM wPar,LPARAM lPar)
{
	Invalidate();
	m_statLst.UpDateCtrl(1);
	return 1;
}
LRESULT CDlgPriceList::OnStockChange(WPARAM wPar,LPARAM lPar)
{
	const SCodeInfo* pCodeInfo = (const SCodeInfo*)wPar;
	if(NULL == pCodeInfo)return 0;
	//if (!m_pCurCodeInfo) m_pCurCodeInfo = new SCodeInfo;
	//memcpy(m_pCurCodeInfo,pCodeInfo,sizeof(SCodeInfo));

	SetLstCurSel(pCodeInfo);
	return 1;
}
int CDlgPriceList::GetCurSel(const SCodeInfo* pSel)
{
	int iNum = m_vectCodeRowInfo.size();
	if (iNum <= 0)return -1;

	int iItem = 0;
	for(; iItem < iNum; iItem++)
	{
		if(0 == strcmp(m_vectCodeRowInfo[iItem].tCodeInfo.szCode,pSel->szCode))
			break;
	}
	if (iItem >= iNum)
	{
		iItem = -1;
	}
	return iItem;
}
void CDlgPriceList::SetLstCurSel(const SCodeInfo* pSel)
{
	m_statLst.SetCurSel(GetCurSel(pSel));
}