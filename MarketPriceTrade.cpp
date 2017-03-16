// MarketPriceTrade.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "MarketPriceTrade.h"
#include "afxdialogex.h"


// CMarketPriceTrade 对话框

IMPLEMENT_DYNAMIC(CMarketPriceTrade, CDialog)

CMarketPriceTrade::CMarketPriceTrade(CWnd* pParent /*=NULL*/)
	: CDialog(CMarketPriceTrade::IDD, pParent)
{

}

CMarketPriceTrade::~CMarketPriceTrade()
{
}

void CMarketPriceTrade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMarketPriceTrade, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CMarketPriceTrade 消息处理程序


HBRUSH CMarketPriceTrade::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd, nCtlColor);
	CFont * cFont=CInterfaceManager::Instance()->GetSysFont(ID_DLGINFOFONT);
// 	//对特定的控件做修改
// 	switch(nCtlColor)
// 	{
// 	case CTLCOLOR_STATIC: //对所有静态文本控件的设置
// 		{
// 			pDC->SetBkMode(TRANSPARENT); 
// 			//设置背景为透明
// 			pDC->SetTextColor(RGB(255,255,0)); //设置字体颜色
// 			pWnd->SetFont(cFont); //设置字体
// 			return (HBRUSH)m_bkBrush;
// 		}
// 	case CTLCOLOR_EDIT: //对所有编辑框的设置
// 		{
// 			pDC->SetBkMode(TRANSPARENT); 
// 			pDC->SetTextColor(RGB(255,255,0)); 
// 			pWnd->SetFont(cFont); 
// 			return (HBRUSH)m_bkBrush;
// 		}
// 	default:
// 		break;
// 	}
	return hbr;
}


int CMarketPriceTrade::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bkBrush.CreateSolidBrush(CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR));

	return 0;
}
