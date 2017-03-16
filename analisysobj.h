// AnalisysObj.h: interface for the CAnalisysObj class.
#if !defined(AFX_ANALISYSOBJ_H__2DF68C4A_7D7E_4A86_88C5_50BC0A5B5D71__INCLUDED_)
#define AFX_ANALISYSOBJ_H__2DF68C4A_7D7E_4A86_88C5_50BC0A5B5D71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "koaclient.h"
#include "BaseDrawObj.h"
#include "pricevolobj.h"
#include "histechobj.h"
#include "TypeCtlPic.h"

class CAnalisysObj : public CBaseDrawObj
{
public:
	void SetType(int iType);
	void SetDrawLineCallBcakFun();
public:
	BOOL MouseRBtnUp(CPoint point);
	void SetFormulaCommand(char* pCommand);
	CAnalisysObj();
	virtual ~CAnalisysObj();

	//�������ܣ�����֤ȯ��Ϣ
	//����������pCodeInfo ֤ȯ��Ϣ
	//�������أ���
	void SetStockInfo(const SCodeInfo *pCodeInfo);
	/////////////////////////////////////////////////////////
	//�������ܣ���ʼ������
	//������������
	//�������أ���
	void Initialization();
	//////////////////////////////////////////////////////////

	//�������ܣ������������С
	//������������
	//�������أ���
	virtual void RecalSubObjSize(int iRightW=50);
	//////////////////////////////////////////////////////////

	//�������ܣ�������������
	//����������pData ���ݻ�����ָ�룻nDataSize ��������
	//�������أ�TRUE/FALSE
	virtual BOOL UpdateData(LPVOID pData, int nDataSize);
	//////////////////////////////////////////////////////////

	//�������ܣ����Ե��λ��
	//����������point �������
	//�������أ�TRUE - ���У�FALSE - δ����
	virtual BOOL HitTest(CPoint point);
	//////////////////////////////////////////////////////////

	//�������ܣ�������Ӧ����
	//����������nChar ����ID
	//�������أ�TRUE/FALSE
	virtual BOOL OnChar(int nChar);
	//////////////////////////////////////////////////////////
	//�����������������
	//�������أ�TRUE/FALSE
	virtual int/*BOOL*/ MouseLBtnDown(CPoint point);
	//�����������������
	virtual BOOL MouseMove(CPoint point);
	//�����������������
	virtual BOOL MouseLBtnUp(CPoint point);
	//�������ܣ�������˫��
	virtual BOOL MouseLBtnDbClick(CPoint point);
	//�������ܣ���Ӧ����Ҽ������Ϣ
	virtual BOOL MouseRBtnDown(LPVOID lpWnd,CPoint point);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ���ͼ
	virtual BOOL Draw(CDC* pDC);
	//////////////////////////////////////////////////////////
	//�������ܣ����û�ͼģʽ
	void SetAnalisysMode(int nDrawMode,BOOL bRequstData = TRUE);
	//�������ܣ����û�ͼģʽ
	int GetAnalisysMode() { return m_nDrawMode;}
	
	int GetCurKLinePriodID(){return m_HisTechObj.GetCurKLinePriodID();}

	//�������ܣ��жϻ�ͼģʽ�Ƿ���������
	BOOL IsExistDrawMode(int nDrawMode);
	//�������ܣ����û�ͼģʽ���������Ϳ���������
	void SetModeAndFocus(int nSel,BOOL bRequestData = TRUE);
	//�������ܣ�����ǰһ����ͼģʽ
	int GetPrevDrawMode();
	//�������ܣ����û�ͼģʽ
	void SetDrawMode(int nDrawMode);
	//�������ܣ����³�ʼ�����
	void ReInitTable();

	// ��Ӧϵͳ�˵���������������
	BOOL OnUserCmd(int nSel);	
	BOOL OnUpdateUserCmd(CCmdUI *pCmdUI);
private:
	const SCodeInfo* m_pCodeInfo;//������Ϣ�ṹ
	int           m_nDrawMode;    //��ͼģʽ
	CUIntArray    m_ArrDrawMode;  //��ͼģʽ����
	CUIntArray    m_ArrDrawMode2; //��ͼģʽ����2

	CPriceVolObj  m_PriceVolObj;  //������ͼ����
	CHisTechObj	  m_HisTechObj;	  //������������
	CTypeCtlPic	  m_TypeCtlPic;  //���Ϳ�����
	int			  m_nSel;//��ǰѡ��
};

#endif // !defined(AFX_ANALISYSOBJ_H__2DF68C4A_7D7E_4A86_88C5_50BC0A5B5D71__INCLUDED_)