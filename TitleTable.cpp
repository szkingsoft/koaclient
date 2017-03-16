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

//�������ܣ�����֤ȯ��Ϣ
//����������pStockInfo ֤ȯ��Ϣ
//�������أ���
void CTitleTable::SetStockInfo(const SStockInfo* pStockInfo)
{
	memcpy(&m_StockInfo,pStockInfo,sizeof(m_StockInfo));

	SCodeInfo tCodeInfo;
	tCodeInfo.m_uiCode = pStockInfo->m_uiCode;
	tCodeInfo.m_usMarketType = pStockInfo->m_usMarketType;
	// ����й���Ʒ����Ҫ�ڴ�ת����Դ���룬�Ա�֤��ʾ����ȷ��
	if( CPublic::Instance()->GetTrusteeSrcCodeInfo(tCodeInfo))
	{
		m_StockInfo.m_uiCode = tCodeInfo.m_uiCode;
		m_StockInfo.m_usMarketType = tCodeInfo.m_usMarketType;
	}

}

//�������ܣ����ڳ�ʼ���к��������ԣ����л�����������ش˺�
//��
//������������
//��������:�ɹ�ΪTRUE,����ΪFALSE
//�����쳣:��
BOOL CTitleTable::Init()
{
	return TRUE;
}

//�������ܣ��������ݣ����м̳���������ش˽ӿ�����������
//����������lpData ��̬������
//��������:�ɹ�ΪTRUE,����ΪFALSE
//�����쳣:��
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
	int iMode = pDC->SetBkMode(TRANSPARENT); //���ñ���ģʽΪ͸��

	char szText[20];
	//��Ʊ����
	memset(szText,0,sizeof(szText));
	memcpy(szText,m_StockInfo.m_acName,sizeof(m_StockInfo.m_acName));
	CRect rt;
	rt = m_DrawRect;
	rt.right = rt.left+m_DrawRect.Width()/2;
	pDC->DrawText(szText, &rt, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);

	//��Ʊ����
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