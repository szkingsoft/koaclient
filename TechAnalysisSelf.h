// TechAnalysisSelf.h: interface for the CTechAnalysisSelf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TECHANALYSISSELF_H__36D9FAF2_99FE_4900_B55D_CE689C3848C3__INCLUDED_)
#define AFX_TECHANALYSISSELF_H__36D9FAF2_99FE_4900_B55D_CE689C3848C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TechAnalysisBasePic.h"

//���뵥ʵ������,���ڼ���������û������ 0x7f ff ff ff
//ͳһ����Ч���� 0X7FFFFFFF
//�ж��Ƿ�����Ч����
BOOL    InValid(float  x);

class CTechAnalysisSelf : public CTechAnalysisBasePic  
{
public:
	void SetVertZoomRaw(float fRaw);
	virtual BOOL UpdateData(LPVOID pNewData);
	//���캯��ָ��ָ������
	CTechAnalysisSelf(const char * zbCode);
	virtual ~CTechAnalysisSelf();
	//////////////////////////////////////////////////////////////////////////
	//�����Զ���ָ������
	virtual void CalculateValue();
protected:
	//�����Ϲ�ʽ���õĵĻ��߷�ʽת���ɿɻ��ķ�ʽ��
	int		GetLineType(int nGSType);

	//���أ��Ա㰴��ָ����ָ�귽ʽ���õ���Ϣ��
	virtual CString GetAnalysistName(int nType);
private:
	char			m_szKey[255];	//���湫ʽ������
	CFormulaFace	m_thCalc;
};

#endif // !defined(AFX_TECHANALYSISSELF_H__36D9FAF2_99FE_4900_B55D_CE689C3848C3__INCLUDED_)
