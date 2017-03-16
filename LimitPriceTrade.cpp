// LimitPriceTrade.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOAClient.h"
#include "LimitPriceTrade.h"
#include "afxdialogex.h"


// CLimitPriceTrade �Ի���

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


// CLimitPriceTrade ��Ϣ�������


HBRUSH CLimitPriceTrade::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd, nCtlColor);
	CFont * cFont=CInterfaceManager::Instance()->GetSysFont(ID_DLGINFOFONT);
	//���ض��Ŀؼ����޸�
// 	switch(nCtlColor)
// 	{
// 	case CTLCOLOR_STATIC: //�����о�̬�ı��ؼ�������
// 		{
// 			pDC->SetBkMode(TRANSPARENT); 
// 			//���ñ���Ϊ͸��
// 			pDC->SetTextColor(RGB(255,255,0)); //����������ɫ
// 			pWnd->SetFont(cFont); //��������
// 			return (HBRUSH)m_bkBrush;
// 		}
// 	case CTLCOLOR_EDIT: //�����б༭�������
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
