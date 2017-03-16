#ifndef  __TRANSLATE_STRUCT_H__
#define  __TRANSLATE_STRUCT_H__
#include "quotedefzyq.h"
#include "quotestruct.h"
#include "quoteconst.h"


struct STUSymbolInfo_ZYQ_Client
{
    string m_ssSymbolCode;             // 商品代码
    string m_ssSymbolName;             // 商品名称
    unsigned short m_usTradeTimeIndex; // 交易节索引 (用位表示)
    unsigned short m_usMarketType;     // 市场类型
    unsigned char m_ucPriceUnit;       // 价格小数位
    unsigned char m_ucTrade;           // 是否交易 0：非交易  1：交易
    unsigned int m_uiTotalShares;      // 流通股数(总股本数)
    INT64 m_iAgreeunit;                // 4个小数
};


struct STUMarketInfo_ZYQ_Client
{
    unsigned short m_usMarketType;  // 市场类型
    string m_ssMarketName;          // 市场名称
};

struct STUInitMarket_ZYQ_Client
{
    vector<STUMarketInfo_ZYQ_Client> m_vMarketInfo; // 具体市场信息
    vector<STUSymbolTradeTime_ZYQ> m_vTradeTime;    // 交易节信息
    vector<STUSymbolInfo_ZYQ_Client> m_vSymbolInfo; // 商品信息
};


struct STUSimpleSymbol_ZYQ_Client
{
    unsigned char m_ucPriceUnit;
    unsigned short m_usMarketType; // 市场类型
    string ssSymbolCode;           // 商品代码
};

struct STUOpenMarketInfo_ZYQ_Client
{
    unsigned int m_uiYesClosePrice; // 昨收价
    unsigned int m_uiYesSettlPrice; // 昨结价
    INT64  m_iTotalIssueQty;        // 总量
};

struct STUOpenMarket_ZYQ_Client
{
    STUSimpleSymbol_ZYQ_Client m_oSSymobl; 
    STUQuoteDateTime m_oTDateTime;
    STUOpenMarketInfo_ZYQ_Client m_oOpenInfo;
};

struct STUCloseMarketInfo_ZYQ_Client
{
    unsigned int m_uiYesClosePrice;     // 昨收价
    unsigned int m_uiYesSettlPrice;     // 昨结价
    unsigned int m_uiOpenPrice;         // 开盘价
    unsigned int m_uiMaxPrice;          // 最高价
    unsigned int m_uiMinPrice;          
    unsigned int m_uiClosePrice;        
    INT64 m_iTotalTrunoverQty;         
    INT64 m_iTotalTrunoverMoney;        
    INT64 m_iTotalIssueQty;            
};

struct STUCloseMarket_ZYQ_Client
{
    STUSimpleSymbol_ZYQ_Client m_oSSymobl;
    STUQuoteDateTime m_oTDateTime;
    int m_iNextTradeDay;
    STUCloseMarketInfo_ZYQ_Client m_oCloseInfo;
};

struct STUReportDataResp_ZYQ_Client_Vec
{
    STUSimpleSymbol_ZYQ_Client m_oSSymobl;     // 商品信息
    STUQuoteDateTime m_oTDateTime;
    STUReportPublicData m_oReportPublicData;
    STUNowDataVolPirce m_oQtyPriceV1;           // 一档行情
};

struct STUReportDataResp_ZYQ_Client
{
    STUReportDataIndex_ZYQ m_oRepIndex;
    vector<STUReportDataResp_ZYQ_Client_Vec> m_vReportData;
};

struct STURealDataPush_ZYQ_Client
{
    STUSimpleSymbol_ZYQ_Client m_oSSymobl;     // 商品信息
    STUQuoteDateTime m_oTDateTime;
    STURealPublicDataPush m_oRealPublicData;
    STUNowDataVolPirce m_oQtyPriceV1;   // 一档行情
};

struct STUReportDataDetailResp_ZYQ_Client
{

    STUSimpleSymbol_ZYQ_Client m_oSSymobl;     // 商品信息
    STUQuoteDateTime m_oTDateTime;
    STUReportPublicData m_oReportPublicData;
    STURealDataDetailPush_Ext_ZYQ m_oExt;
    vector<STUNowDataVolPirce> m_vQtyPriceV;     // 档位行情
};

struct STURealDataDetailPush_ZYQ_Client
{
    STUSimpleSymbol_ZYQ_Client m_oSSymobl;     // 商品信息
    STUQuoteDateTime m_oTDateTime;
    STURealPublicDataPush m_oRealPublicData;
    STURealDataDetailPush_Ext_ZYQ m_oExt;
    vector<STUNowDataVolPirce> m_vQtyPriceV;   
};

struct STUHisKUnit_ZYQ_Client
{
  STUSimpleSymbol_ZYQ_Client m_oSymbol;
  vector<STUHisKUnit_ZYQ> m_vHisKUnit;
};

struct STUSTKMinUnit_ZYQ_Client
{
   STUSimpleSymbol_ZYQ_Client m_oSymbol;
   vector<STUSTKMinUnit_ZYQ> m_vMinUnit;
};

struct STUTraceUnit_ZYQ_Client
{
    STUSimpleSymbol_ZYQ_Client m_oSymbol;
    vector<STUTraceUnit_ZYQ> m_vTraceUnit;
};
struct STUAllTypeResp_ZYQ_Client
{
    unsigned short m_usRespType;
    STUInitMarket_ZYQ_Client m_oInitMarket;    //初始化市场
    STUOpenMarket_ZYQ_Client m_oOpenMarket;
    STUCloseMarket_ZYQ_Client m_oCloseMarket;
    STUReportDataResp_ZYQ_Client m_oReportData; 
    STUReportDataDetailResp_ZYQ_Client m_oReportDateDetail; 
    STUTraceUnit_ZYQ_Client m_oTraceUnit; 
    STUSTKMinUnit_ZYQ_Client m_oMinUnit;    //分时成交
    STUHisKUnit_ZYQ_Client   m_oHisKUnit;   
    STURealDataPush_ZYQ_Client  m_oRealData;  //实时行情
    STURealDataDetailPush_ZYQ_Client m_oRealDataDetail; 
};
class CRespStructTrans
{
public:
    CRespStructTrans() {};
    ~CRespStructTrans() {};
    static bool TransRespStruct(const char* from, int len,STUAllTypeResp_ZYQ_Client& to);
};

#endif