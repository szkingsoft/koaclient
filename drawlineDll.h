#pragma once
class CDrawLineDll
{
public:
	CDrawLineDll(void);
	~CDrawLineDll(void);
	void Init();
private:
	HINSTANCE  m_dllHandle;
	typedef	void (WINAPI *pSetCurSymbol)(WCHAR * pSymbolCode);
	typedef	void (WINAPI *pDraw)(class CDC * pDC);
	typedef	int  (WINAPI *pKeyDown)(char cChar);
	typedef int  (WINAPI *pLButtonDown)(const CPoint& pt);
	typedef int  (WINAPI *pMouseMove)(const CPoint& pt);
	typedef int  (WINAPI *pLButtonUp)(const CPoint& pt);
	typedef int  (WINAPI *pCreateGraph)(unsigned int uiType);
	typedef void (WINAPI *pPreStep)();
	typedef void (WINAPI *pNextStep)();
	typedef int  (WINAPI *pCanPreStep)();
	typedef int  (WINAPI *pCanNextStep)();
	typedef BOOL (WINAPI *pSetGraphicProperty)(CWnd* pWnd,const CPoint& pt);

	typedef  void*  (WINAPI *pGetPaintor)();
	typedef  void  (WINAPI *pSetCallbakGetYFromValue)(void* pFunc);
	typedef  void  (WINAPI *pSetCallbakGetXFromTime)(void* pFunc);
	typedef  void  (WINAPI *pSetCallbakGetValueFromY)(void* pFunc);
	typedef  void  (WINAPI *pSetCallbakGetTimeFromX)(void* pFunc);
	typedef  void  (WINAPI *pSetCallbakGetUnitLength)(void* pFunc);
	typedef  void  (WINAPI *pSetCallbakGetKLineRect)(void* pFunc);
	typedef  void  (WINAPI *pSetCallbakGetHighLowFromTime)(void* pFunc);
	typedef  void  (WINAPI *pSetCallbakGetCloseFromXSgmt)(void* pFunc);
	typedef  void  (WINAPI *pSetCallbakGetHighLowFromXSgmt)(void* pFunc);
public:
	pSetCurSymbol m_pSetCurSymbol;
	pDraw m_pDraw;
	pKeyDown m_pKeyDown;
	pLButtonDown m_pLButtonDown;
	pMouseMove m_pMouseMove;
	pLButtonUp m_pLButtonUp;
	pCreateGraph m_pCreateGraph;
	pPreStep  m_pPreStep;
	pNextStep m_pNextStep;
	pCanPreStep m_pCanPreStep;
	pCanNextStep m_pCanNextStep;
	pSetGraphicProperty m_pSetGraphicProperty;
	pSetCallbakGetYFromValue m_pSetCallbakGetYFromValue;
	pSetCallbakGetXFromTime m_pSetCallbakGetXFromTime;
	pSetCallbakGetValueFromY m_pSetCallbakGetValueFromY;
	pSetCallbakGetTimeFromX m_pSetCallbakGetTimeFromX;
	pSetCallbakGetUnitLength m_pSetCallbakGetUnitLength;
	pSetCallbakGetKLineRect  m_pSetCallbakGetKLineRect;
	pSetCallbakGetHighLowFromTime m_pSetCallbakGetHighLowFromTime;
	pSetCallbakGetCloseFromXSgmt  m_pSetCallbakGetCloseFromXSgmt;
	pSetCallbakGetHighLowFromXSgmt  m_pSetCallbakGetHighLowFromXSgmt;

};

