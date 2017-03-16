#pragma once
#include "markup.h"

typedef struct stcSkinInfo
{
	CString strSkinName;
	int iId;
}SKININFO,*pSKININFO;

typedef std::map<CString,STUNowDataZYQ> mapTuNowDateZYQ;

#define  V1    1
#define  V2    2
class CDateManage
{
public:
	CDateManage(void);
	~CDateManage(void);
public:
	static void ExitInstance();
	static CDateManage* Instance();
	static CDateManage* _instance;
	static int g_iProtocolVersion;
	static CString DoubleToString( double dblValue,int nDec );
	void ReArrayVector(vector<int> &vecRA);
public:
	CSkinAttribute * GetSkinAttribute(UINT iId);
	void InitSkinAttribute();
	void InitSkinAttribute_Black();
	void InitSkinAttribute_White();
	void InitSkinAttribute_White1();
public:
	void GetSkinInfo(std::vector<SKININFO> &vecSkin);
private:
	CCriticalSection m_lockMapSkin;
	std::map<int,SKININFO> m_mapSkin;
public:
	void DrawBkRect(Graphics &gpCach,CRect rcDraw,DWORD clrFill);
	void DrawBkRect(Graphics &gpCach,CRect rcDraw);
	void DrawBkRect(CDC * pDc,CRect rcDraw,DWORD clrFill);
	void DrawBkRect(CDC * pDc,CRect rcDraw);
	void DrawBkRect_DC(CDC * pDc,CRect rcDraw,DWORD clrFill);

	DWORD ColorToRGB(DWORD clrTR) { return Color::Color(clrTR).ToCOLORREF();}
	DWORD ColorToRGB(){ return Color::Color(m_clrFill).ToCOLORREF(); }

	void SetBkColor(DWORD clrBk) {m_clrFill = clrBk;}
	DWORD GetBkColor(){ return m_clrFill; }
private:
	DWORD m_clrFill;
public:
	void DrawTriangle(Graphics &GraphCach,CRect rcDraw);

	void DrawBottomHide(CWnd *pWnd,CRect rcDraw,int iStat=0);
	void DrawTopHide(CWnd *pWnd,CRect rcDraw,int iStat=0);
	void DrawLeftHide(CWnd *pWnd,CRect rcDraw,int iStat=0);
	void DrawRightHide(CWnd *pWnd,CRect rcDraw,int iStat=0);

	void AddRoundRect(GraphicsPath &gp,INT x, INT y, INT width, INT height, INT cornerX, INT cornerY);
	void DrawBtnByString(Graphics & gpCach,STRBTN &stcStrBtn,int iCheckId=0);

	void DrawToolBar(Graphics & gpCach,DRAWTOOLBAR stcDraw);
private:
	Gdiplus::Image * m_ImgBottomHide;
	Gdiplus::Image * m_ImgTopHide;
	Gdiplus::Image * m_ImgLeftide;
	Gdiplus::Image * m_ImgRightHide;

	void InitImg();
	void ResImg(Gdiplus::Image ** pImg);
	void ReleaseImg();
public:
	int ExecuteUrl(CString strUrl);
	int GetDaysInMonth(int iYear,int iMonth);
	CString FormatString(int iDate,int iFormatType=0);
	CString FormatTime(int iDate);
	CString FormatTime(int iDate,int iType);
	CString FormatTimeY(INT64 iDate,int iMode = 0);

	void NotifyTrade(UINT iMsg,WPARAM wPar, LPARAM lPar,int iNotifyWnd);
	void GetFontFamily();
	void NotifyWndReLayout();
public:
	short AddRequestIndex();
	short GetRequestIndex();

	void ReqInitMarket_Sp();
	void ReqReportData_Sp();
	void ReqStockInfo_SP();
	const STUSymbolInfo* GetDefaultStock_SP();
	void ReqRegRealQuotationDate(int iType = 0);
	void ReqReportData_Settle_SP();

	void RequestData_SP(SCodeInfo *pCodeInfo);
public:
	int  GetSymbolNum() {return m_vecSymbol.size();}

	void ManageMsg(const char* lpData,int nDataLen);
	void AnalysisMsg(const char* lpData,int nDataLen);
	void InitSymbolInfo(const char* lpData);
	void SetNotifyWnd(int iWndType,CWnd *pWnd);

	void GetRealQuoteInfo(mapTuNowDateZYQ & maplst);
	CString GetSymbolName(CString strCode);
	void GetRealQuoteInfo(QUOTEINFO_M & stcQuoteInfo){stcQuoteInfo = m_stcQuoteInfo;}
	
	STUSymbolDataZYQ GetSymbolInfo(CString strCode);
	CString GetImgPath(){return m_strImgPath;}
	int GetMarketType(){return m_iMarketType;}
	void SetMarketType(int iMarketType){m_iMarketType = iMarketType;}
public:
	const long * GetStockInfo(const void * pCode, UINT iMode = 1);
	void ChangeStockEvent(const void * pCodeInfo,UINT uiInPage,UINT iMode = 1);

private:
	void Init();
	void AddTestDate();
	void RegRealMetalsQuotationDate();
	void RegRealSpotQuotationDate();
	int GetNeedRegRealQuoSymbol(STUSSymbol * pstcSymbol);
	void PackSend(int iMsgType,int MsgIndex);
private:
	CPublic * m_pPublic;
	short m_sRequestIndex;

	int m_iMarketType;
	CString m_strImgPath;

	STUSymbolInfo * m_oCurrentCode;
	CString	 m_ssDefaultCode;

	std::map<int,CWnd *> m_mapNotifyWnd;
	CCriticalSection m_lockRealQuoteLst;
	mapTuNowDateZYQ  m_mapQuotationLst;
	QUOTEINFO_M m_stcQuoteInfo;
	vector <STUSymbolDataZYQ> m_vecSymbol;

	//std::map<CString,STURealQuoteInfoZQY> m_mapRealQuoteInfo;
	std::map<UINT,STUSettlQuoteInfo> m_mapSettleQuoteLst;
};