// TechIndexFactory.cpp: implementation of the CTechIndexFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TechIndexFactory.h"
#include "techanalysiskline.h"
#include "TechAnalysisSelf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTechIndexFactory::CTechIndexFactory()
{
	
}

CTechIndexFactory::~CTechIndexFactory()
{
}

CTechAnalysisBasePic* CTechIndexFactory::CreateTechAnalysisObj(CString szAnalysist)
{
	return GetTechAnalysisObj(szAnalysist);	
}

CTechAnalysisBasePic* CTechIndexFactory::GetTechAnalysisObj(CString szAnalysist)
{
	CTechAnalysisBasePic* pClass = NULL;
	if(szAnalysist == "KLINE")
	{
		pClass = new CTechAnalysisKLine;	
	}
	else if((szAnalysist == "MA")||(szAnalysist == "BOLL"))
	{
		CTechAnalysisKLine* pKLine;
		pKLine = new CTechAnalysisKLine;
		pKLine->SetMAMode(TRUE,szAnalysist);
		pClass = pKLine;
	}
	else
		pClass = new CTechAnalysisSelf((char*)(LPCTSTR)szAnalysist);
	return pClass;
}
