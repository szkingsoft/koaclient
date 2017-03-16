// TechAnalysisSelf.h: interface for the CTechAnalysisSelf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TECHANALYSISSELF_H__36D9FAF2_99FE_4900_B55D_CE689C3848C3__INCLUDED_)
#define AFX_TECHANALYSISSELF_H__36D9FAF2_99FE_4900_B55D_CE689C3848C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TechAnalysisBasePic.h"

//必须单实例工作,对于计算多个事例没有意义 0x7f ff ff ff
//统一的无效数字 0X7FFFFFFF
//判断是否是无效数字
BOOL    InValid(float  x);

class CTechAnalysisSelf : public CTechAnalysisBasePic  
{
public:
	void SetVertZoomRaw(float fRaw);
	virtual BOOL UpdateData(LPVOID pNewData);
	//构造函数指定指标名。
	CTechAnalysisSelf(const char * zbCode);
	virtual ~CTechAnalysisSelf();
	//////////////////////////////////////////////////////////////////////////
	//计算自定义指标数据
	virtual void CalculateValue();
protected:
	//市面上公式中用的的画线方式转换成可画的方式。
	int		GetLineType(int nGSType);

	//重载，以便按照指定义指标方式，得到信息。
	virtual CString GetAnalysistName(int nType);
private:
	char			m_szKey[255];	//保存公式的名称
	CFormulaFace	m_thCalc;
};

#endif // !defined(AFX_TECHANALYSISSELF_H__36D9FAF2_99FE_4900_B55D_CE689C3848C3__INCLUDED_)
