// Copyright (C) 1991 - 2004 futuretrend Software Corporation

#include "stdafx.h"
#include "TitleTable.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTitleTable::CTitleTable()
{
	// TODO: Add your specialized code here.
}

CTitleTable::~CTitleTable()
{
	// TODO: Add your specialized code here.
}

//函数功能：设置证券信息
//函数参数：pStockInfo 证券信息
//函数返回：无
void CTitleTable::SetStockInfo(const SStockInfo* pStockInfo)
{
	memcpy(&m_StockInfo,pStockInfo,sizeof(m_StockInfo));

	SCodeInfo tCodeInfo;
	tCodeInfo.m_uiCode = pStockInfo->m_uiCode;
	tCodeInfo.m_usMarketType = pStockInfo->m_usMarketType;
	// 针对托管商品，需要在此转换成源代码，以保证显示的正确性
	if( CPublic::Instance()->GetTrusteeSrcCodeInfo(tCodeInfo))
	{
		m_StockInfo.m_uiCode = tCodeInfo.m_uiCode;
		m_StockInfo.m_usMarketType = tCodeInfo.m_usMarketType;
	}

}

//函数功能：用于初始化列和设置属性，所有基层类必须重载此函
//数
//函数参数：无
//函数返回:成功为TRUE,错误为FALSE
//函数异常:无
BOOL CTitleTable::Init()
{
	return TRUE;
}

//函数功能：传递数据，所有继承类必须重载此接口来设置数据
//函数参数：lpData 多态数据流
//函数返回:成功为TRUE,错误为FALSE
//函数异常:无
BOOL CTitleTable::UpdateData(LPVOID lpData)
{
	return TRUE;
}

void CTitleTable::Draw(CDC* pDC)
{
	COLORREF crText;
	CFont* pFont;

	crText = CInterfaceManager::Instance()->GetSysColor(ID_TOTALVOL_COLOR);
	pFont = CInterfaceManager::Instance()->GetSysFont(ID_TITLE_FONT);
	pDC->SetTextColor(crText);
	CFont* pOldFont = (CFont*) pDC->SelectObject(pFont);
	int iMode = pDC->SetBkMode(TRANSPARENT); //设置背景模式为透明

	char szText[20];
	//股票名称
	memset(szText,0,sizeof(szText));
	memcpy(szText,m_StockInfo.m_acName,sizeof(m_StockInfo.m_acName));
	CRect rt;
	rt = m_DrawRect;
	rt.right = rt.left+m_DrawRect.Width()/2;
	pDC->DrawText(szText, &rt, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);

	//股票代码
	crText = CInterfaceManager::Instance()->GetSysColor(ID_TITLE_COLOR);
	pDC->SetTextColor(crText);
	memset(szText,0,sizeof(szText));
	sprintf(szText,"%06d",m_StockInfo.m_uiCode);
	rt.left = rt.right;
	rt.right = m_DrawRect.right;
	pDC->DrawText(szText, &rt, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);

	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(iMode);
}