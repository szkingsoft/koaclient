#ifndef PUBZYQ_H
#define PUBZYQ_H

typedef struct stcIndexDate
{
	float fEqualLine[5];
}IndexDate,*pIndexDate;
typedef std::map<CString,IndexDate>  TMapIndexDate;
typedef struct stcIndexDateHead
{
	CRect rcItem;
	UINT uiCurves;
	UINT uiParamNum;
	int iParam[5];
	CString strEqlLineTitel[5];
	UINT uiEqualLineType[5];
	CString strTitle;

	float fMaxValue;
	float fMinValue;
	float fMarkValue;

	DWORD dwclr[5];
	stcIndexDateHead()
	{
		rcItem = CRect(0,0,0,0);
		uiCurves = uiParamNum = 0;
		strEqlLineTitel[0] = _T("");
		ZeroMemory(uiEqualLineType,sizeof(UINT)*5);
		fMaxValue = fMinValue = fMarkValue = 0.000;

		dwclr[0] = Color::MakeARGB(255,255,255,255);
		dwclr[1] = Color::MakeARGB(255,255,255,0);
		dwclr[2] = Color::MakeARGB(255,255,0,255);
		dwclr[3] = Color::MakeARGB(255,0,255,0);
		dwclr[4] = Color::MakeARGB(255,0,255,255);
	}
}IndexDateHead,*pIndexDateHead;
typedef std::map<CString,IndexDateHead>  TMapIndexDateHead;
typedef std::vector<CString> TVecIndexKey;
typedef struct stcHisUnitK
{
	UINT64	uiDate;
	float	fOpenPrice;
	float	fClosePrice;
	float	fMaxPrice;
	float	fMinPrice;
	float	fJunPrice;
	float   fTurnoverRate; 
	float   fBeforClosePrice; 
	float   fTrunoverMoney;  
	float   fTrunoverQty;
	float fBeforTrunoverMoney;
	float fBeforTrunoverQty;
	TMapIndexDate mapIndexDate;
	void Init()
	{
		uiDate = 0;
		fOpenPrice = 0;
		fClosePrice = 0;
		fMaxPrice = 0;
		fMinPrice = 0;
		fJunPrice = 0;
		fTurnoverRate = 0;
		fBeforClosePrice = 0; 
		fTrunoverMoney = 0; 
		fTrunoverQty = 0;
		fBeforTrunoverQty = 0;
		fBeforTrunoverMoney = 0;
		mapIndexDate.clear();
	}
	stcHisUnitK()
	{
		Init();
	}
}HisUnitK_ZYQ,*pHisUnitK_ZYQ;
typedef std::map<UINT64,HisUnitK_ZYQ> TmapHisUnitK_ZYQ;

typedef struct stcMainIndex
{
	int iIndexType;
	UINT uiIndexID;
	CString strIndexName;
	CRect rcIndex;
	stcMainIndex()
	{
		uiIndexID = 0;
		iIndexType = 1;
		strIndexName = _T("");
		rcIndex = CRect(0,0,0,0);
	}
}MainIndex,*pMainIndex;
typedef std::vector<MainIndex> TVecMainIndex;
typedef struct stcKDateHead
{
	UINT uiCycle;
	UINT uiHasNum;
	void Init()
	{
		uiCycle = 1;
		uiHasNum = 0;
	}
	stcKDateHead()
	{
		Init();
	}
}KDateHead,*pKDateHead;

typedef struct stcKKeyNum
{
	UINT64 uiDate;
}KKeyNum,*pKKeyNum;
typedef std::map<UINT,KKeyNum> TMapKeyNum;
typedef std::map<INT64,UINT>   TMapT_N;
typedef struct stcHoriPosDate
{
	int iHoriPos;
	UINT64 uiDate;
	stcHoriPosDate()
	{
		iHoriPos = 0;
		uiDate = 0;
	}
}HoriPosDate,*pHoriPosDate;
typedef struct stcKDate
{
	CRect           rcAll_K;
	CRect           rcTime;
	CRect           rcTips;
	CRect           rcIndex;
	float           fKEveryWithAll;
	int             iKEveryWith;
	int             iDisplayNum;
	UINT64          uiCurBeginTm;
	pHoriPosDate    pHPDate;
	UINT            iMaxHPos;
	UINT            uiCurSelIndex;
	KDateHead         stKDateHead;
	TVecIndexKey      vecIndexDisplay;
	TMapIndexDateHead MapIndexDateHead;
	TmapHisUnitK_ZYQ  mapHisUnitK_ZYQ;
	TMapKeyNum        mapKeyNum;
	TMapT_N           mapT_N;
	TVecMainIndex     vecMainIndex;
	stcKDate()
	{
		Init();
	}
	void Init()
	{
		mapHisUnitK_ZYQ.clear();
		fKEveryWithAll = 10;
		iKEveryWith = 6;
		iDisplayNum = 0;
		uiCurBeginTm = 0;
		pHPDate = NULL;
		iMaxHPos = 0;
		MapIndexDateHead.clear();
		vecIndexDisplay.clear();
		vecMainIndex.clear();
		mapKeyNum.clear();
		mapT_N.clear();
	}
	void Clear()
	{
		mapHisUnitK_ZYQ.clear();
		fKEveryWithAll = 10;
		iKEveryWith = 6;
		iDisplayNum = 0;
		uiCurBeginTm = 0;
		iMaxHPos = 0;
		if (pHPDate)
		{
			delete []pHPDate;
			pHPDate = NULL;
		}
		MapIndexDateHead.clear();
		mapHisUnitK_ZYQ.clear();
		stKDateHead.Init();
		mapKeyNum.clear();
		mapT_N.clear();
	}
	void SetItemRect(CRect rcItem,UINT uiItem)
	{
		if (vecIndexDisplay.size()!=3 && uiItem < 3)return;

		MapIndexDateHead[vecIndexDisplay[uiItem]].rcItem = rcItem;
	}
	CRect GetItemRect(UINT uiItem)
	{
		if (vecIndexDisplay.size()!=3 && uiItem < 3)return CRect(0,0,0,0);

		return MapIndexDateHead[vecIndexDisplay[uiItem]].rcItem;
	}
	void SetMaxMinValue(float fMaxV,float fMinV,float fMarkV,UINT uiItem)
	{
		if (vecIndexDisplay.size()!=3 && uiItem < 3)return;
		MapIndexDateHead[vecIndexDisplay[uiItem]].fMaxValue = fMaxV;
		MapIndexDateHead[vecIndexDisplay[uiItem]].fMinValue = fMinV;
		MapIndexDateHead[vecIndexDisplay[uiItem]].fMarkValue = fMarkV;
	}
	void GetMaxMinValue(float &fMaxV,float &fMinV,float &fMarkV,UINT uiItem)
	{
		if (vecIndexDisplay.size()!=3 && uiItem < 3)return;
		fMaxV = MapIndexDateHead[vecIndexDisplay[uiItem]].fMaxValue;
		fMinV = MapIndexDateHead[vecIndexDisplay[uiItem]].fMinValue;
		fMarkV = MapIndexDateHead[vecIndexDisplay[uiItem]].fMarkValue;
	}
}KDate,*pKDate;
struct SPVDate
{
	float fPrice;
	float fQty;
};
struct SBSDate
{
	SPVDate fbuyQuote;
	SPVDate fsellQuote;
	void operator = (STUNowDataVolPirce stNowDataVolPirce)
	{
		fbuyQuote.fPrice = stNowDataVolPirce.m_buyVol.m_uiPrice;
		fbuyQuote.fQty = stNowDataVolPirce.m_buyVol.m_uiQty;
		fsellQuote.fPrice = stNowDataVolPirce.m_sellVol.m_uiPrice;
		fsellQuote.fQty = stNowDataVolPirce.m_sellVol.m_uiQty;
	}
};
typedef struct stcTopBtn
{
	CRect rcTop;
	CRect rcBtnLeft;
	CRect rcBtnRight;
	void Init()
	{
		rcTop = rcBtnRight = rcBtnLeft = CRect(0,0,0,0);
	}
	stcTopBtn()
	{
		Init();
	}
}TopBtn,*pTopBtn;

typedef struct stcTopPartDate
{
	TopBtn stTopBtn;
	float fWibi;
	float fWeicha;
	int iBSNum;
	SBSDate sVolPrice[MAX_ZYQ_VOL_PRICE];
	float fNewPrice;
	float fPinjunPrice;
	float fZhangDie;
	float fOpenPrice;
	float fFudu;
	float fMaxPrice;
	float fNowVol;
	float fMinPrice;
	float fTotalVol;
	float fVolRatio;
	float fDealMoney;
	float fHuanshouRatio;
	float fYesDPrice;
	float fYesSettlPrice;
	float fOutPan;
	float fInPan;
	stcTopPartDate()
	{
		Init();
	}
	void Init()
	{
		fWibi = 0;
		fWeicha = 0;
		iBSNum = 5;
		sVolPrice[MAX_ZYQ_VOL_PRICE];
		memset(sVolPrice,0,sizeof(SBSDate)*MAX_ZYQ_VOL_PRICE);
		fNewPrice = 0;
		fPinjunPrice = 0;
		fZhangDie = 0;
		fOpenPrice = 0;
		fFudu = 0;
		fMaxPrice = 0;
		fNowVol = 0;
		fMinPrice = 0;
		fTotalVol = 0;
		fVolRatio = 0;
		fDealMoney = 0;
		fHuanshouRatio = 0;
		fYesDPrice = 0;
		fYesSettlPrice = 0;
		fOutPan = 0;
		fInPan = 0;
	}
	void Clear()
	{
		fWibi = 0;
		fWeicha = 0;
		iBSNum = 5;
		sVolPrice[MAX_ZYQ_VOL_PRICE];
		memset(sVolPrice,0,sizeof(SBSDate)*MAX_ZYQ_VOL_PRICE);
		fNewPrice = 0;
		fPinjunPrice = 0;
		fZhangDie = 0;
		fFudu = 0;
		fMaxPrice = 0;
		fNowVol = 0;
		fMinPrice = 0;
		fTotalVol = 0;
		fVolRatio = 0;
		fDealMoney = 0;
		fHuanshouRatio = 0;
		fOutPan = 0;
		fInPan = 0;
	}
}TopPartDate,*pTopPartDate;

typedef struct stcHisTTimeDate
{
	int iTradeDay;
	INT64 uiDateTime;
	float fClosePrice;
	float fTrunoverQty;   
	float fOrderQty;  
	float fAveragePrice; 
	float fYesSettlPrice; 
	void Init()
	{
		iTradeDay = 0;
		uiDateTime = 0;  
		fClosePrice = 0;  
		fTrunoverQty = 0;            
		fOrderQty = 0;        
		fAveragePrice = 0;    
		fYesSettlPrice = 0;   
	}
	stcHisTTimeDate()
	{
		Init();
	}
}HisTTimeDate,*pHisTTimeDate;
typedef struct stcHisDateSpot
{
	HisTTimeDate stHisTTimeDate;
	float xPos;
	float yTTPos;
	float yTTPos_Jun;
	float yVolPos;
	stcHisDateSpot()
	{
		stHisTTimeDate.Init();
		xPos = 0;
		yTTPos = 0;
		yTTPos_Jun = 0;
		yVolPos = 0;
	}
}HisDateSpot,*pHisDateSpot;

typedef std::map<UINT64,HisDateSpot> TmapHisDateSpot;
typedef struct stcTimeTrendDate
{
	CRect rcTimeTrend;
	CRect rcVolTrend;
	CRect rcTips;
	float fMaxPrice;
	float fMinPrice;
	float fYsPrice;
	float fMaxVol;
	float fMinVol;
	float fPPVol;
	TmapHisDateSpot mapHisTTDate;
	void Clear()
	{
		fMaxPrice=fMinPrice=fYsPrice=fMaxVol=fMinVol=fPPVol=0.00;
		mapHisTTDate.clear();
	}
}TimeTrendDate,*pTimeTrendDate;
typedef struct stcSymbolInfo_ZYQ
{
	char cMarket;
	char cPriceUnit;
	char  cTrade;
	unsigned short usTradeTimeIndex; 
	CString strSymbolName;
	CString strSymbolCode;
	unsigned int uiTotalShares;
	INT64 iAgreeunit;
	void Clear()
	{
		cMarket = 0;
		cPriceUnit = 0;
		cTrade = 0;
		usTradeTimeIndex = 0;   
		strSymbolName = _T("");
		strSymbolCode = _T("");
		uiTotalShares = 0;
		iAgreeunit = 0;
	}
}SymbolInfo_ZYQ,*pSymbolInfo_ZYQ;
typedef struct stcTradeTimeSlot
{
	char cTheDay;
	int inBeginTime;
	int inEndTime;
}TradeTimeSlot,*pTradeTimeSlot;
typedef struct stcTradeTime
{
	char cTradeTimeCount;
	TradeTimeSlot stTradeTimeSlot[MAX_ZYQ_TRADE_TIME];
}TradeTime,*pTradeTime;
typedef struct stcSymbolTradeInfo_ZYQ
{
	SymbolInfo_ZYQ stSymbolInfo;
	TradeTime stTradeTime;
	void Clear()
	{
		stSymbolInfo.Clear();
		memset(&stTradeTime,0,sizeof(TradeTime));
	}
}SymbolTradeInfo_ZYQ,*pSymbolTradeInfo_ZYQ;

/////////
typedef struct stcDealDetailsDate
{
	int iTradeDay; 
	INT64 uiDateTime;    
	char cBSDirec;   
	float fPrice; 
	float fQty;  
}DealDetailsDate,*pDealDetailsDate;
typedef std::vector<DealDetailsDate> TvecDealDetails;
typedef TvecDealDetails::iterator itTvecDealDetails;
typedef struct stcDealDetailsInfo
{
	int iDisplayNum;
	TvecDealDetails  vecDealDetails;
	void Clear()
	{
		iDisplayNum = 0;
		vecDealDetails.clear();
	}
}DealDetailsInfo,*pDealDetailsInfo;

/////////
typedef struct stcIntiSymbolInfo
{
}IntiSymbolInfo,*pIntiSymbolInfo;

typedef map<CString,SymbolTradeInfo_ZYQ/*IntiSymbolInfo*/> TmapIntiSymbolInfo;
typedef struct stcSymbolClassInfo
{
	unsigned short usSymbolClassType; 
	CString strSymbolClassName;               
	TmapIntiSymbolInfo mapInitSymbol;
}SymbolClassInfo,*pSymbolClassInfo;

typedef map<unsigned short,SymbolClassInfo> TmapSymbolClassInfo;
typedef struct stcInitMarket_ZYQ
{
	unsigned short usMarketType;                 
	TradeTime stTradeTime;
	TmapSymbolClassInfo mapSymbolClass;
}InitMarket_ZYQ,*pInitMarket_ZYQ;


///////
typedef struct stcReportData_ZYQ
{
	unsigned int uiTradeDay; 
	unsigned int uiUpdateDataTime;  
	CString strSymbolCode;
	unsigned int uiNewPrice;
	unsigned int uiNewQty;
	int iPriceRiseDown;
	int iPriceRiseDownRate; 
	unsigned int uiOpenPrice; 
	unsigned int uiMaxPrice;   
	unsigned int uiMinPrice;             
	unsigned int uiYesClosePrice;         
	unsigned int uiYesSetllPrice;         
	unsigned int uiTotalTurnoverQty;      
	INT64 iTotalTurnoverFunds;           
	int uiQtyRate;                        
	int uiOrderRate;                      
	int uiOrderDiff;                    
	unsigned int uiPOrderSaleQty;        
	unsigned int uiPOrderBuyQty;       
	unsigned int iBSNum;                    
	SBSDate stVolPrice[MAX_ZYQ_VOL_PRICE];          
}ReportData_ZYQ,*pReportData_ZYQ;

typedef map<CString,ReportData_ZYQ> TmapReportDate_ZYQ;

typedef struct stcQuoteLstSubItemDate
{
	CString strSItemText;
	DWORD clrSItemText;
	UINT iSWith;
	UINT iSTextAlignmentType;
}QuoteLstSubItemDate,*pQuoteLstSubItemDate;
typedef std::map<UINT,QuoteLstSubItemDate> TmapQuoteLstSubItemDate;

typedef struct stcItemPubDate
{
	float fValue;
}ItemPubDate,*pItemPubDate;
typedef struct stcQuoteItemLstDate
{
	CRect rcItem;
	UINT uiItemID;
	UINT uiItemHight;
	TmapQuoteLstSubItemDate mapQuoteLstSubItemDate;
	QUOTEPRICE stQuotePrice;
}QuoteItemLstDate,*pQuoteItemLstDate;
typedef std::map<UINT,QuoteItemLstDate> TmapQuoteItemLstDate;

typedef struct stcQuoteHeadLstDate
{
	CRect rcHItem;

	UINT uiCurSort;
	BOOL bSort;
	DWORD clrSort;
	UINT uiHHight;

	UINT uiDisplayNum;
	UINT uiHItemBegin;
	UINT uiHKeepBackItemIDEnd;

	UINT uiCorrectionWith;
	TmapQuoteLstSubItemDate mapQuoteLstSubItemDate;
	stcQuoteHeadLstDate()
	{
		Init();
	}
	void Init()
	{
		rcHItem = CRect(0,0,0,0);
		uiCurSort = 0;
		bSort = 0;
		clrSort = 0;
		uiHHight = 0;
		uiDisplayNum = 0;
		uiHItemBegin = 0;
		uiHKeepBackItemIDEnd = 0;
		uiCorrectionWith = 0;
		mapQuoteLstSubItemDate.clear();
	}
	void Clear()
	{
		uiCurSort = 0;
		bSort = 0;
		clrSort = 0;
		uiHItemBegin = 0;
	}
}QuoteHeadLstDate,*pQuoteHeadLstDate;

typedef struct stcQuoteItemLst
{
	CRect rcLstDisplayItem;
	UINT uiCurSel;
	unsigned short uiItemIDBegin;
	UINT uiItemDiplayMax;
	UINT uiItemHight;
	TmapQuoteItemLstDate mapQuoteItemLstDate;
	stcQuoteItemLst()
	{
		Init();
	}
	void Init()
	{
		rcLstDisplayItem = CRect(0,0,0,0);
		uiCurSel = 0;
		uiItemIDBegin = 1;
		uiItemDiplayMax = 0;
		uiItemHight = 18;
		mapQuoteItemLstDate.clear();
	}
	void Clear()
	{
		uiCurSel = 0;
		uiItemIDBegin = 1;
		uiItemDiplayMax = 0;
		uiItemHight = 18;
		mapQuoteItemLstDate.clear();
	}
}QuoteItemLst,*pQuoteItemLst;
//////
typedef struct stcTabItme
{
	CRect rcItem;
	CString strItemName;
	UINT uiItemId;
	UINT uiItemWith;
}TabItme,*pTabItme;
typedef std::map<unsigned short,TabItme> TmapTabItem;
typedef TmapTabItem::iterator itTmapTabItem;
typedef struct stcTabInfo
{
	CRect rcTab;
	UINT uiCurSel;
	TmapTabItem mapTabItme;
	stcTabInfo()
	{
		rcTab = CRect(0,0,0,0);
		uiCurSel = 0;
		mapTabItme.clear();
	}
	void Clear()
	{
		uiCurSel = 0;
	}
}TabInfo,*pTabInfo;
///////////
typedef struct stcHScrollInfo
{
	CRect rcHScroll;
	int iBtnWith;
	int iThumbLeft;
	int iThumbRight;

	int iPageDisPlay;
	int iAllDisPlay;
	stcHScrollInfo()
	{
		Init();
	}
	void Init()
	{
		rcHScroll = CRect(0,0,0,0);
		iBtnWith = 16;
		iThumbLeft = 0;
		iThumbRight = 0;
		iPageDisPlay = 0;
		iAllDisPlay = 0;
	}
	void Clear()
	{
		iThumbLeft = 0;
		iThumbRight = 0;
		iPageDisPlay = 0;
		iAllDisPlay = 0;
	}
}HScrollInfo,*pHScrollInfo;

typedef struct stcQuoteLstDate
{
	CRect rcLst;
	UINT iPressDown;
	QuoteHeadLstDate stHeadLstDate;
	QuoteItemLst stQuoteItemLst;
	TabInfo stTabInfo;
	HScrollInfo stHScrollInfo;
	void Clear()
	{
		rcLst = CRect(0,0,0,0);
		iPressDown = 0;
		stHeadLstDate.Clear();
		stQuoteItemLst.Clear();
		stTabInfo.Clear();
		stHScrollInfo.Clear();
	}
}QuoteLstDate,*pQuoteLstDate;

typedef struct stcChartDate
{
	KDate stKDate;
	TimeTrendDate stTrendDate;
	TopPartDate stTopPartDate;
	DealDetailsInfo stDealDetailsInfo;
	SymbolTradeInfo_ZYQ stSymbolTradeInfo;
	TabInfo stTabInfo;
	TabInfo stTabMain;
	void Clear()
	{
		stKDate.Clear();
		stTrendDate.Clear();
		stTopPartDate.Clear();
		stDealDetailsInfo.Clear();
		stSymbolTradeInfo.Clear();
	}
}ChartDate,*pChartDate;

#endif