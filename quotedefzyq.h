#ifndef __NEW_CHART_DEF_H__
#define __NEW_CHART_DEF_H__

//////////////////////////////////////////////////////////////////////////
// 2.0
// 初始化市场
// 功能号: INITMARKET_ZYQ =0x1a00;
// 请求:SNormaHead
// 回应:SNormaHead + STUInitMarketLenDirec_ZYQ + STUMarketInfo_ZYQ[N] + STUSymbolTradeTime_ZYQ[N] + STUSymbolInfo_ZYQ[N];

// 报价表列表
// 功能号: REPORTDATA_ZYQ =0x02a0;
// 请求: SNormaHead + STUReportDataIndex_ZYQ + STUSimpleSymbol_ZYQ[N]
// 回应: SNormaHead + STUReportDataIndex_ZYQ + STUCommLenDirec + STUReportDataResp_ZYQ[N]

// 报价表详细
// 功能号: REPORTDATA_DETAIL_ZYQ =0x02a1;
// 请求: SNormaHead + STUSimpleSymbol_ZYQ
// 回应: SNormaHead + STUSimpleSymbol_ZYQ + STUReportDataDetailResp_ZYQ + STUNowDataVolPirce[N]

// 请求K线数据
// 功能号: HISKDATA_ZYQ =0x06a0;
// 请求: SNormaHead + STUSimpleSymbol_ZYQ + STUHisKIndexDirec
// 回应: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUHisKUnit_ZYQ  

// 请求分时走势
// 功能号: STKMINUTEDATA_ZYQ =0x11a0;
// 请求: SNormaHead + STUSimpleSymbol_ZYQ
// 回应: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUSTKMinUnit_ZYQ  

// 成交明细
// 功能号: STKTRACEDATA_ZYQ =0x01a0;
// 请求: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec
// 回应: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUTraceUnit_ZYQ  

// (报价)实时表推送
// PUSH_REALDATA_ZYQ       =0x0ba0;
// SNormaHead + STUSimpleSymbol_ZYQ + STURealDataPush_ZYQ(单个)

// (图表详细)实时推送
// PUSH_REALDATA_DETAIL_ZYQ =0x0ba1;
// SNormaHead + STUSimpleSymbol_ZYQ + STURealDataDetailPush_ZYQ + STUNowDataVolPirce[N]

// 实时推送报价表列表(注册改变)
// PUSH_REPORTDATA_LIST_ZYQ =0x0ba2;
// SNormaHead + STUReportDataIndex_ZYQ + STUCommLenDirec + STUReportDataResp_ZYQ[N]


// 请求综合排名
// 请求 COMPLEX_RANKING_ZYQ   =0x02a4;
// 回应 SNormaHead + {STUComplexRankingHead_ZYQ STUComplexRankingData_ZYQ[N]}[M]


// 推送综合排名
// PUSH_COMPLEX_RANKING_ZYQ   =0x0ba3;
// SNormaHead + {STUComplexRankingHead_ZYQ STUComplexRankingData_ZYQ[N]}[M]


//#include "quoteclient.h"
#define MARKET_NAME_ZYQ_LEN 10
#define SYMBOL_CODE_ZYQ_LEN 10
#define SYMBOL_NAME_ZYQ_LEN 10
#define MAX_TRADE_TIME  20

#pragma pack(1)
struct STUSymbolTradeTime_ZYQ
{
    unsigned char m_ucTheDay;     // 是否是昨日, 0:今日, 1:昨日
    int m_nBeginTime;             // 交易节开始时间 
    int m_nEndTime;               // 交易节结束时间
};

struct STUCommLenDirec
{
    unsigned short m_lenDirec;
};


struct STUHisKIndexDirec
{
    unsigned char m_ucHisType;  // K线裂隙
    int m_iHasNum;              // 已有数量
    int m_iReqNum;              // 请求数量
};

struct STUSymbolInfo_ZYQ
{
    char m_szSymbolCode[SYMBOL_CODE_ZYQ_LEN+1];   // 商品代码
    char m_szSymbolName[SYMBOL_NAME_ZYQ_LEN+1];   // 商品名称
    unsigned short m_usTradeTimeIndex;              // 交易节索引 (用位表示)
    unsigned short m_usMarketType;                  // 市场类型
    unsigned char m_ucPriceUnit;                    // 价格小数位
    unsigned char m_ucTrade;                        // 是否交易 0：非交易  1：交易
    unsigned int m_uiTotalShares;                   // 流通股数(总股本数)
    INT64 m_iAgreeunit;                             // 4个小数
    STUSymbolInfo_ZYQ()
    {
        memset(this, 0, sizeof(STUSymbolInfo_ZYQ));
    }
};


struct STUMarketInfo_ZYQ
{
    char m_szMarketName[MARKET_NAME_ZYQ_LEN + 1];   // 市场名称
    unsigned short m_usMarketType;                  // 市场类型
};


struct STUInitMarketLenDirec_ZYQ
{
    unsigned short m_usMarkets;
    unsigned short m_usTradeTimeNum;  
    unsigned short m_usSymboNum;
    STUInitMarketLenDirec_ZYQ()
    {
        memset(this, 0, sizeof(STUInitMarketLenDirec_ZYQ));
    }
};

struct STUSimpleSymbol_ZYQ
{
    unsigned char m_ucPriceUnit;                    // 价格小数位
    unsigned short m_usMarketType;                  // 市场类型
    char m_szSymbolCode[SYMBOL_CODE_ZYQ_LEN + 1];   // 商品代码
};

enum
{
    RPDATA_ORDER_BY_SYMOBL,             // 商品代码
    RPDATA_ORDER_BY_NEWPRICE,           // 最新价
    RPDATA_ORDER_BY_RISEDOWM,           // 涨跌幅
    RPDATA_ORDER_BY_RISEDOWM_RATE,      // 涨跌比例
    RPDATA_ORDER_BY_TRUNOVER_QTY,       // 成交量
    RPDATA_ORDER_BY_TRUNOVER_FUNDS,     // 成交额
    RPDATA_ORDER_BY_QTY_RATE,           // 量比
    RPDATA_ORDER_BY_ORDER_RATE,         // 委比

    //---- 一下内部使用,请无请求, 请求也不会有回应
    RPDATA_ORDER_BY_5MIN_RISEDOWN_RATE, // 5分钟涨跌幅排名
    RPDATA_ORDER_BY_AMPLITUDE,          // 当日振幅排名
};

enum
{
    REDATA_SEQ_TYPE_ASC =0,    // 正序 xiao -> da
    REDATA_SEQ_TYPE_DESC=1   // 逆序
};

enum
{
    TURNOVER_DEIRC_B = 0,  // 买方成交
    TURNOVER_DEIRC_S = 1,  // 卖方成交
    TURNOVER_DEIRC_F = 2,  // 集合竞价或是其他成交
    TURNOVER_DEIRC_C = 3,  // 无成交
};

#define MARKET_TYPE_ALL  0x0000   // 所有市场
struct STUReportDataIndex_ZYQ
{
    unsigned short m_usMarketType;              // 市场类型
    unsigned char  m_ucOrderBy;                 // 排序类型
    unsigned char  m_ucSeqType;                 // 排序(正序,逆序)        
    unsigned short m_usBeginPos;                // 开始位置 ( 从1 开始)
    unsigned short m_usReqNum;                  // 请求个数
    unsigned short m_usReqDesSymoblNum;         // 指定请求个数; 如果是0,那么表示请求当前市场类型下面所有商品
    STUReportDataIndex_ZYQ()
    {
        memset(this, 0, sizeof(STUReportDataIndex_ZYQ));
    }
};


struct STUQuoteDateTime
{
    int m_iTradeDay;                        // 交易日   YYYYMMDD
    INT64 m_iUpdateDataTime;                  // 更新时间 YYYYMMDDHHMMSS
};

struct STUReportPublicData
{
    unsigned int m_uiNewPrice;              //  最新价(当前价)
    unsigned int m_uiNewQty;                //  最新成交量(当前量) 2
    unsigned char m_chTurnoverDirec;        // 成交方向  0= B 1= S 2=平 3=无成交
    int m_iPriceRiseDown;                   // 涨跌幅
    int m_iPriceRiseDownRate;               // 涨跌比例 2
    unsigned int m_uiOpenPrice;             // 开盘价
    unsigned int m_uiMaxPrice;              // 最高价
    unsigned int m_uiMinPrice;              // 最低价
    unsigned int m_uiYesClosePrice;         // 昨收价
    unsigned int m_uiYesSetllPrice;         // 昨结价
    unsigned int m_uiTotalTurnoverQty;      // 总成交量
    INT64 m_iTotalTurnoverFunds;            // 总成交额
    int m_iQtyRate;                        // 量比
    int m_iOrderRate;                      // 委比
};

struct STURealPublicDataPush
{
    unsigned int m_uiNewPrice;          //  最新价(当前价)
    unsigned int m_uiNewQty;            //  最新成交量(当前量)
    unsigned char m_chTurnoverDirec;    // 成交方向  0= B 1= S 2=平 3=无成交
    int m_iPriceRiseDown;               // 涨跌幅
    int m_iPriceRiseDownRate;           // 涨跌比例
    unsigned int m_uiTotalTurnoverQty;  // 总成交量
    INT64 m_iTotalTurnoverFunds;        // 总成交额
    int m_uiQtyRate;                    // 量比
    int m_uiOrderRate;                  // 委比

};
struct STUReportDataResp_ZYQ
{
    STUSimpleSymbol_ZYQ m_oSSymobl;     // 商品信息
    STUQuoteDateTime m_oTDateTime;
    STUReportPublicData m_oReportPublicData;
    STUNowDataVolPirce m_oQtyPriceV1;   // 一档行情

    STUReportDataResp_ZYQ()
    {
        memset(this, 0, sizeof(STUReportDataResp_ZYQ));
    }
};

struct STURealDataPush_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STURealPublicDataPush m_oRealPublicData;
    STUNowDataVolPirce m_oQtyPriceV1;   // 一档行情

    STURealDataPush_ZYQ()
    {
        memset(this, 0, sizeof(STURealDataPush_ZYQ));
    }
};

struct STURealDataDetailPush_Ext_ZYQ
{
    int m_iOrderDiff;                      // 委差(有正副)
    unsigned int m_uiPOrderSaleQty;         // 内盘
    unsigned int m_uiPOrderBuyQty;          // 外盘
   
};

struct STUReportDataDetailResp_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STUReportPublicData m_oReportPublicData;
    STURealDataDetailPush_Ext_ZYQ m_oExt;
    unsigned short m_usVolNum;              // 档位价格数量

    STUReportDataDetailResp_ZYQ()
    {
        memset(this, 0, sizeof(STUReportDataDetailResp_ZYQ));
    }
};

struct STURealDataDetailPush_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STURealPublicDataPush m_oRealPublicData;
    STURealDataDetailPush_Ext_ZYQ m_oExt;
    unsigned short m_usVolNum;              // 档位价格数量

    STURealDataDetailPush_ZYQ()
    {
        memset(this, 0, sizeof(STURealDataDetailPush_ZYQ));
    }

    void CopyTo(STURealDataPush_ZYQ& dst, const STUNowDataVolPirce& v1)
    {
        dst.m_oTDateTime = m_oTDateTime;
        dst.m_oRealPublicData = m_oRealPublicData;
        dst.m_oQtyPriceV1 = v1;   // 一档行情
    }
};

struct STUHisKUnit_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    unsigned int m_uiOpenPrice;       // 开盘价
    unsigned int m_uiClosePrice;      // 收盘价
    unsigned int m_uiMaxPrice;        // 最高价
    unsigned int m_uiMinPrice;        // 最低价
    INT64 m_iTotalTurnoverFunds;      // 成交额
    unsigned int m_uiTrunoverQty;     // 成交量

    STUHisKUnit_ZYQ()
    {
        memset(this, 0, sizeof(STUHisKUnit_ZYQ));
    }
};

struct STUSTKMinUnit_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    unsigned int m_uiTurnoverPrice;     // 成交价  涨跌 = 成交价 - 昨收
    unsigned int m_uiTrunoverFunds;     // 成交额 均价= 成交额/成交量
    unsigned int m_uiTrunoverQty;       // 成交量        

    STUSTKMinUnit_ZYQ()
    {
        memset(this, 0, sizeof(STUSTKMinUnit_ZYQ));
    }
};



struct STUTraceUnit_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    unsigned char m_chTurnoverDirec;             // 成交方向(B/S or up/down) 0= B 1= S 2=平 // TURNOVER_DEIRC_B  TURNOVER_DEIRC_S TURNOVER_DEIRC_F
    unsigned int m_uiPrice;             // 成交价格
    unsigned int m_uiQty;               // 成交数量

    STUTraceUnit_ZYQ()
    {
        memset(this, 0, sizeof(STUTraceUnit_ZYQ));
    }
};

enum
{
    COMPLEX_RANKING_TYPE_DAY_RISE_RATE = 1,     // 当日涨幅排名
    COMPLEX_RANKING_TYPE_DAY_DOWN_RATE,         // 当日跌幅排名
    COMPLEX_RANKING_TYPE_5MIN_RISE_RATE,        // 5分钟涨幅排名
    COMPLEX_RANKING_TYPE_5MIN_DOWN_RATE,        // 5分钟跌幅排名
    COMPLEX_RANKING_TYPE_DAY_ORDER_RATE_TOP,    // 当日委比前几名
    COMPLEX_RANKING_TYPE_DAY_ORDER_RATE_BOTTOM, // 当日委比后几名
    COMPLEX_RANKING_TYPE_DAY_AMPLITUDE,         // 当日振幅排名
    COMPLEX_RANKING_TYPE_DAY_QTY_RATE,          // 当日量比排名
    COMPLEX_RANKING_TYPE_DAY_TRUN_FUNS,         // 当日成交金额排名
};

// 
struct STUComplexRankingHead_ZYQ
{
    unsigned char m_ucRankingType;
    unsigned short m_usRankinkDataNum;
};

struct STUComplexRankingData_ZYQ
{
    STUSimpleSymbol_ZYQ m_oSSymobl;
    unsigned short m_usNewPrice;
    unsigned short m_usYesClose;
    unsigned short m_usYesSettl;
    INT64 m_iValue;
};

// 交易服务 <-> 行情服务  协议

// 初始化市场
// 功能号 INITMARKET_ZYQ =0x1a00;
// 请求              
// 回应 SNormaHead + STUInitMarketLenDirec_ZYQ + STUMarketInfo_ZYQ[N] + STUSymbolTradeTime_ZYQ[N] + STUSymbolInfo_ZYQ[N];

// 报价表
// 功能号  REPORTDATA_DETAIL_ZYQ =0x02a0;
// 请求
// 回应 SNormaHead + STUNowDataDetail_ZYQ[N}

// 实时行情 PUSH_REALDATA_DETAIL_ZYQ =0x0ba1;
// 推送 SNormaHead + STUNowDataDetail_ZYQ(单个)

// 开盘信号 
// 功能号 OPEN_MAKRKET_ZYQ =0x1afe;
// 推送 SNormaHead + STUOpenQuoteInfo_ZYQ(单个)

// 收盘信号 
// 功能号 CLOSE_MAKRKET_ZYQ =0x1aff;
// 推送 SNormaHead + STUCloseQuoteInfo_ZYQ(单个)



struct STUNowData_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STUSimpleSymbol_ZYQ m_oSymbolInfo;
    unsigned int m_uiYesClosePrice;     // 昨收价
    unsigned int m_uiYesSettlPrice;     // 昨结价
    unsigned int m_uiOpenPrice;         // 开盘价
    unsigned int m_uiMaxPrice;          // 最高价
    unsigned int m_uiMinPrice;          // 最低价
    unsigned int m_uiNewPrice;          // 最新价(当前价)
    unsigned int m_uiNewQty;            // 最新量
    unsigned char m_chTurnoverDirec;    // 成交方向  0= B 1= S 2=平 
    unsigned int m_uiQtyRate;           // 量比
    INT64 m_iTotalTrunoverQty;          // 总成交量
    INT64 m_iTotalTrunoverMoney;        // 总成交额
    INT64 m_iTotalTrunoverCount;        // 总成交笔数
    INT64 m_iTotalOrderQty;             // 订货量
    INT64 m_iTotalIssueQty;             // 总量
    INT64 m_iUnTrunoverBuy;             // 总未成交买量
    INT64 m_iUnTrunoverSell;            // 总未成交买量
    INT64 m_iVolTrunoverBuy;            // 总买(五档价格)成交量
    INT64 m_iVolTrunoverSell;           // 总卖(五档价格)成交量
    char m_cVolPriceCount;
    STUNowDataVolPirce m_arrVolPrice[MAX_ZYQ_VOL_PRICE];

    STUNowData_ZYQ()
    {
        memset(this, 0, sizeof(STUNowData_ZYQ));
    }
};

struct STUNowDataDetail_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STUSimpleSymbol_ZYQ m_oSymbol;
    unsigned int m_uiYesClosePrice;     // 昨收价
    unsigned int m_uiYesSettlPrice;     // 昨结价
    unsigned int m_uiOpenPrice;         // 开盘价
    unsigned int m_uiMaxPrice;          // 最高价
    unsigned int m_uiMinPrice;          // 最低价
    unsigned int m_uiNewPrice;          // 最新价(当前价)
    unsigned int m_uiNewQty;            // 最新量
    unsigned char m_chTurnoverDirec;    // 成交方向  0= B 1= S 2=平 
    unsigned int m_uiSettlPrice;        // 结算价
    INT64 m_iBuyOpen;                   // 买订立
    INT64 m_iBuyClose;                  // 卖转让
    INT64 m_iSellOpen;                  // 卖订立
    INT64 m_iSellClose;                 // 买转让
    INT64 m_iTotalOrderCount;           // 总委托单数量
    INT64 m_iTotalTrunoverQty;          // 总成交量
    INT64 m_iTotalTrunoverMoney;        // 总成交额
    INT64 m_iTotalTrunoverCount;        // 总成交笔数
    INT64 m_iTotalOrderQty;             // 订货量
    INT64 m_iTotalIssueQty;            // 总量
    INT64 m_iUnTrunoverBuy;             // 总未成交买量
    INT64 m_iUnTrunoverSell;            // 总未成交买量
    INT64 m_iVolTrunoverBuy;            // 总买(五档价格)成交量
    INT64 m_iVolTrunoverSell;           // 总卖(五档价格)成交量
    char m_cVolPriceCount;
    STUNowDataVolPirce m_arrVolPrice[MAX_ZYQ_VOL_PRICE];

    STUNowDataDetail_ZYQ()
    {
        memset(this, 0, sizeof(STUNowDataDetail_ZYQ));
    }
};

// 开市命令
struct STUOpenQuoteInfo_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STUSimpleSymbol_ZYQ m_oSymbol;
    unsigned int m_uiYesClosePrice;      // 昨收价
    unsigned int m_uiYesSettlPrice;      // 昨结价
    INT64  m_iTotalOrderQty;             // 订货量
    INT64  m_iTotalIssueQty;             // 总量

    STUOpenQuoteInfo_ZYQ()
    {
        memset(this, 0, sizeof(STUOpenQuoteInfo_ZYQ));
    }
};

struct STUCloseQuoteInfo_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    int m_iNextTick;                        //下一交易日期

    STUSimpleSymbol_ZYQ m_oSymbol;
    unsigned int m_uiYesClosePrice;         // 昨收价
    unsigned int m_uiYesSettlPrice;         // 昨结价
    unsigned int m_uiOpenPrice;             // 开盘价
    unsigned int m_uiMaxPrice;              // 最高价
    unsigned int m_uiMinPrice;              // 最低价
    unsigned int m_uiClosePrice;            // 今天收盘价
    unsigned int m_uiSettlPrice;            // 今天结算价
    unsigned int m_uiQtyRate;               // 量比
    INT64 m_iTotalTrunoverQty;             // 总成交量
    INT64 m_iTotalTrunoverMoney;           // 总成交额
    INT64 m_iTotalTrunoverCount;           // 总成交笔数
    INT64 m_iVolTrunoverBuy;                // 总买(五档价格)成交量
    INT64 m_iVolTrunoverSell;               // 总卖(五档价格)成交量
    INT64 m_iTotalOrderQty;                // 订货量
    INT64 m_iTotalIssueQty;                // 总发行量

    STUCloseQuoteInfo_ZYQ()
    {
        memset(this, 0, sizeof(STUCloseQuoteInfo_ZYQ));
    }
};

struct STUSettlQuoteInfo_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STUSimpleSymbol_ZYQ m_oSymbol;
    unsigned int m_uiCommPrice;
    INT64 m_iCommBuy;
    INT64 m_iCommSell;
    INT64 m_iCommTrunoverQty;
    unsigned int m_uiCommTrunoverCount;
    unsigned int m_uiNeutralPrice;
    INT64 m_iNeutralBuy;
    INT64 m_iNeutralSell;
    INT64 m_iNeutralTrunoverQty;
    unsigned int  m_uiNeturalTrunoverCount;
};


#pragma pack()
#endif