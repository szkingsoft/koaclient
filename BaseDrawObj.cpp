#include "stdafx.h"
#include "BaseDrawObj.h"

#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBaseDrawObj::CBaseDrawObj()
{
	m_bActivate= FALSE;
	m_iType = 0;
}

CBaseDrawObj::~CBaseDrawObj()
{
}

void CBaseDrawObj::MoveTo(CRect rRect)
{
	m_DrawRect = rRect;
	RecalSubObjSize();
}


//�������ܣ������Ҽ��˵�
//����������pWnd - ����ָ�룻point - �������
//          uiPopMenuID - �˵�ID��iSel - �˵����
//�������أ�ѡ�в˵�IDֵ
UINT CBaseDrawObj::PopupRBtnMenu(CWnd* pWnd,CPoint point,UINT uiPopMenuID,int iSel)
{
	CPoint scrnPoint;
	CMenu popMenu;
	UINT uiItemID;

	if(NULL == pWnd) return -1; //��ָֹ���

	scrnPoint = point;
	pWnd->ClientToScreen(&scrnPoint);
	popMenu.LoadMenu(uiPopMenuID); //װ�ز˵���Դ
	CLanguageInterface::Instance()->InitRightMenu(&popMenu,iSel);
	uiItemID = popMenu.GetSubMenu(iSel)->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN
		,scrnPoint.x,scrnPoint.y,AfxGetMainWnd()); //�����˵�
	popMenu.DestroyMenu(); //���ٲ˵�
	return uiItemID;
}

void CBaseDrawObj::ActivateObj(BOOL bActivate)
{
	m_bActivate= bActivate;
}
