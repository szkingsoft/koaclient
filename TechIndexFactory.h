// TechIndexFactory.h: interface for the CTechIndexFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TECHINDEXFACTORY_H__4AB98032_87F3_4A23_B873_C2D3F784EC75__INCLUDED_)
#define AFX_TECHINDEXFACTORY_H__4AB98032_87F3_4A23_B873_C2D3F784EC75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TechAnalysisBasePic.h"

class CTechIndexFactory  
{
public:
	virtual ~CTechIndexFactory();
	static CTechAnalysisBasePic* CreateTechAnalysisObj(CString szAnalysist);

protected:
	static CTechAnalysisBasePic* GetTechAnalysisObj(CString szAnalysist);
	CTechIndexFactory();
private:

};

#endif // !defined(AFX_TECHINDEXFACTORY_H__4AB98032_87F3_4A23_B873_C2D3F784EC75__INCLUDED_)
