#include "stdafx.h"
#include "drawlineDll.h"


CDrawLineDll::CDrawLineDll(void)
{
	Init();
}


CDrawLineDll::~CDrawLineDll(void)
{
	if(NULL != m_dllHandle )
		FreeLibrary(m_dllHandle);
}

void CDrawLineDll::Init()
{
	///////
	m_pSetCurSymbol = NULL;
	m_pDraw = NULL;
	m_pKeyDown = NULL;
	m_pLButtonDown = NULL;
	m_pMouseMove = NULL;
	m_pLButtonUp = NULL;
	m_pCreateGraph = NULL;
	m_pPreStep = NULL;
	m_pNextStep = NULL;
	m_pCanPreStep = NULL;
	m_pCanNextStep = NULL;
	m_pSetGraphicProperty = NULL;
	m_pSetCallbakGetYFromValue = NULL;
	m_pSetCallbakGetXFromTime = NULL;
	m_pSetCallbakGetValueFromY = NULL;
	m_pSetCallbakGetTimeFromX = NULL;
	m_pSetCallbakGetUnitLength = NULL;
	m_pSetCallbakGetKLineRect = NULL;
	m_pSetCallbakGetHighLowFromTime = NULL;
	m_pSetCallbakGetCloseFromXSgmt = NULL;
	m_pSetCallbakGetHighLowFromXSgmt = NULL;
	////////

	m_dllHandle = ::LoadLibrary(_T("DrawLineTool.dll"));
	DWORD dwError = GetLastError();
	if (0 == m_dllHandle)
	{
		MessageBox(NULL,"打开Dll失败1","",MB_OK|MB_ICONERROR);
		return;
	}

	m_pSetCurSymbol = (pSetCurSymbol)::GetProcAddress(m_dllHandle, "SetCurSymbol");
	m_pDraw = (pDraw)::GetProcAddress(m_dllHandle, "Draw");
	m_pKeyDown = (pKeyDown)::GetProcAddress(m_dllHandle, "KeyDown");
	m_pLButtonDown= (pLButtonDown)::GetProcAddress(m_dllHandle, "LButtonDown");
	m_pMouseMove=(pMouseMove)::GetProcAddress(m_dllHandle, "MouseMove");
	m_pLButtonUp=(pLButtonUp)::GetProcAddress(m_dllHandle, "LButtonUp");
	m_pCreateGraph=(pCreateGraph)::GetProcAddress(m_dllHandle, "CreateGraph");
	m_pPreStep =(pPreStep)::GetProcAddress(m_dllHandle, "PreStep");
	m_pNextStep=(pNextStep)::GetProcAddress(m_dllHandle, "NextStep");
	m_pCanPreStep=(pCanPreStep)::GetProcAddress(m_dllHandle, "CanPreStep");
	m_pCanNextStep=(pCanNextStep)::GetProcAddress(m_dllHandle, "CanNextStep");
	m_pSetGraphicProperty=(pSetGraphicProperty)::GetProcAddress(m_dllHandle, "SetGraphicProperty");
	m_pSetCallbakGetYFromValue= (pSetCallbakGetYFromValue)::GetProcAddress(m_dllHandle, "SetCallbakGetYFromValue");
	m_pSetCallbakGetXFromTime= (pSetCallbakGetXFromTime)::GetProcAddress(m_dllHandle, "SetCallbakGetXFromTime");
	m_pSetCallbakGetValueFromY = (pSetCallbakGetValueFromY)::GetProcAddress(m_dllHandle, "SetCallbakGetValueFromY");
	m_pSetCallbakGetTimeFromX = (pSetCallbakGetTimeFromX)::GetProcAddress(m_dllHandle, "SetCallbakGetTimeFromX");
	m_pSetCallbakGetUnitLength = (pSetCallbakGetUnitLength)::GetProcAddress(m_dllHandle, "SetCallbakGetUnitLength");
	m_pSetCallbakGetKLineRect = (pSetCallbakGetKLineRect)::GetProcAddress(m_dllHandle, "SetCallbakGetKLineRect");
	m_pSetCallbakGetHighLowFromTime = (pSetCallbakGetHighLowFromTime)::GetProcAddress(m_dllHandle, "SetCallbakGetHighLowFromTime");
	m_pSetCallbakGetCloseFromXSgmt= (pSetCallbakGetCloseFromXSgmt)::GetProcAddress(m_dllHandle, "SetCallbakGetCloseFromXSgmt");
	m_pSetCallbakGetHighLowFromXSgmt= (pSetCallbakGetHighLowFromXSgmt)::GetProcAddress(m_dllHandle, "SetCallbakGetHighLowFromXSgmt");

	if(!m_pSetCurSymbol||!m_pDraw||!m_pKeyDown||!m_pLButtonDown||!m_pMouseMove||
		!m_pLButtonUp||!m_pCreateGraph||!m_pPreStep||!m_pNextStep||
		!m_pCanPreStep||!m_pCanNextStep||!m_pSetGraphicProperty
		||!m_pSetCallbakGetYFromValue||!m_pSetCallbakGetXFromTime||!m_pSetCallbakGetValueFromY
		||!m_pSetCallbakGetTimeFromX||!m_pSetCallbakGetUnitLength||!m_pSetCallbakGetKLineRect
		||!m_pSetCallbakGetHighLowFromTime||!m_pSetCallbakGetCloseFromXSgmt||!m_pSetCallbakGetHighLowFromXSgmt)
	{
		MessageBox(NULL,"打开Dll失败2","",MB_OK|MB_ICONERROR);
		return ;
	}
}


