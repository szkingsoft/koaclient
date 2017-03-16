// LimitPriceTrade.cpp : 实现文件
//

#include "stdafx.h"
#include "KOAClient.h"
#include "LimitPriceTrade.h"
#include "afxdialogex.h"


// CLimitPriceTrade 对话框

IMPLEMENT_DYNAMIC(CLimitPriceTrade, CDialog)

CLimitPriceTrade::CLimitPriceTrade(CWnd* pParent /*=NULL*/)
	: CDialog(CLimitPriceTrade::IDD, pParent)
{

}

CLimitPriceTrade::~CLimitPriceTrade()
{
}

void CLimitPriceTrade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLimitPriceTrade, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CLimitPriceTrade 消息处理程序


HBRUSH CLimitPriceTrade::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd, nCtlColor);
	CFont * cFont=CInterfaceManager::Instance()->GetSysFont(ID_DLGINFOFONT);
	//对特定的控件做修改
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


int CLimitPriceTrade::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bkBrush.CreateSolidBrush(CInterfaceManager::Instance()->GetSysColor(ID_SYSBACK_COLOR));

	return 0;
}
