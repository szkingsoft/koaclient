#ifndef __NEW_CHART_DEF_H__
#define __NEW_CHART_DEF_H__

//////////////////////////////////////////////////////////////////////////
// 2.0
// ��ʼ���г�
// ���ܺ�: INITMARKET_ZYQ =0x1a00;
// ����:SNormaHead
// ��Ӧ:SNormaHead + STUInitMarketLenDirec_ZYQ + STUMarketInfo_ZYQ[N] + STUSymbolTradeTime_ZYQ[N] + STUSymbolInfo_ZYQ[N];

// ���۱��б�
// ���ܺ�: REPORTDATA_ZYQ =0x02a0;
// ����: SNormaHead + STUReportDataIndex_ZYQ + STUSimpleSymbol_ZYQ[N]
// ��Ӧ: SNormaHead + STUReportDataIndex_ZYQ + STUCommLenDirec + STUReportDataResp_ZYQ[N]

// ���۱���ϸ
// ���ܺ�: REPORTDATA_DETAIL_ZYQ =0x02a1;
// ����: SNormaHead + STUSimpleSymbol_ZYQ
// ��Ӧ: SNormaHead + STUSimpleSymbol_ZYQ + STUReportDataDetailResp_ZYQ + STUNowDataVolPirce[N]

// ����K������
// ���ܺ�: HISKDATA_ZYQ =0x06a0;
// ����: SNormaHead + STUSimpleSymbol_ZYQ + STUHisKIndexDirec
// ��Ӧ: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUHisKUnit_ZYQ  

// �����ʱ����
// ���ܺ�: STKMINUTEDATA_ZYQ =0x11a0;
// ����: SNormaHead + STUSimpleSymbol_ZYQ
// ��Ӧ: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUSTKMinUnit_ZYQ  

// �ɽ���ϸ
// ���ܺ�: STKTRACEDATA_ZYQ =0x01a0;
// ����: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec
// ��Ӧ: SNormaHead + STUSimpleSymbol_ZYQ + STUCommLenDirec + STUTraceUnit_ZYQ  

// (����)ʵʱ������
// PUSH_REALDATA_ZYQ       =0x0ba0;
// SNormaHead + STUSimpleSymbol_ZYQ + STURealDataPush_ZYQ(����)

// (ͼ����ϸ)ʵʱ����
// PUSH_REALDATA_DETAIL_ZYQ =0x0ba1;
// SNormaHead + STUSimpleSymbol_ZYQ + STURealDataDetailPush_ZYQ + STUNowDataVolPirce[N]

// ʵʱ���ͱ��۱��б�(ע��ı�)
// PUSH_REPORTDATA_LIST_ZYQ =0x0ba2;
// SNormaHead + STUReportDataIndex_ZYQ + STUCommLenDirec + STUReportDataResp_ZYQ[N]


// �����ۺ�����
// ���� COMPLEX_RANKING_ZYQ   =0x02a4;
// ��Ӧ SNormaHead + {STUComplexRankingHead_ZYQ STUComplexRankingData_ZYQ[N]}[M]


// �����ۺ�����
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
    unsigned char m_ucTheDay;     // �Ƿ�������, 0:����, 1:����
    int m_nBeginTime;             // ���׽ڿ�ʼʱ�� 
    int m_nEndTime;               // ���׽ڽ���ʱ��
};

struct STUCommLenDirec
{
    unsigned short m_lenDirec;
};


struct STUHisKIndexDirec
{
    unsigned char m_ucHisType;  // K����϶
    int m_iHasNum;              // ��������
    int m_iReqNum;              // ��������
};

struct STUSymbolInfo_ZYQ
{
    char m_szSymbolCode[SYMBOL_CODE_ZYQ_LEN+1];   // ��Ʒ����
    char m_szSymbolName[SYMBOL_NAME_ZYQ_LEN+1];   // ��Ʒ����
    unsigned short m_usTradeTimeIndex;              // ���׽����� (��λ��ʾ)
    unsigned short m_usMarketType;                  // �г�����
    unsigned char m_ucPriceUnit;                    // �۸�С��λ
    unsigned char m_ucTrade;                        // �Ƿ��� 0���ǽ���  1������
    unsigned int m_uiTotalShares;                   // ��ͨ����(�ܹɱ���)
    INT64 m_iAgreeunit;                             // 4��С��
    STUSymbolInfo_ZYQ()
    {
        memset(this, 0, sizeof(STUSymbolInfo_ZYQ));
    }
};


struct STUMarketInfo_ZYQ
{
    char m_szMarketName[MARKET_NAME_ZYQ_LEN + 1];   // �г�����
    unsigned short m_usMarketType;                  // �г�����
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
    unsigned char m_ucPriceUnit;                    // �۸�С��λ
    unsigned short m_usMarketType;                  // �г�����
    char m_szSymbolCode[SYMBOL_CODE_ZYQ_LEN + 1];   // ��Ʒ����
};

enum
{
    RPDATA_ORDER_BY_SYMOBL,             // ��Ʒ����
    RPDATA_ORDER_BY_NEWPRICE,           // ���¼�
    RPDATA_ORDER_BY_RISEDOWM,           // �ǵ���
    RPDATA_ORDER_BY_RISEDOWM_RATE,      // �ǵ�����
    RPDATA_ORDER_BY_TRUNOVER_QTY,       // �ɽ���
    RPDATA_ORDER_BY_TRUNOVER_FUNDS,     // �ɽ���
    RPDATA_ORDER_BY_QTY_RATE,           // ����
    RPDATA_ORDER_BY_ORDER_RATE,         // ί��

    //---- һ���ڲ�ʹ��,��������, ����Ҳ�����л�Ӧ
    RPDATA_ORDER_BY_5MIN_RISEDOWN_RATE, // 5�����ǵ�������
    RPDATA_ORDER_BY_AMPLITUDE,          // �����������
};

enum
{
    REDATA_SEQ_TYPE_ASC =0,    // ���� xiao -> da
    REDATA_SEQ_TYPE_DESC=1   // ����
};

enum
{
    TURNOVER_DEIRC_B = 0,  // �򷽳ɽ�
    TURNOVER_DEIRC_S = 1,  // �����ɽ�
    TURNOVER_DEIRC_F = 2,  // ���Ͼ��ۻ��������ɽ�
    TURNOVER_DEIRC_C = 3,  // �޳ɽ�
};

#define MARKET_TYPE_ALL  0x0000   // �����г�
struct STUReportDataIndex_ZYQ
{
    unsigned short m_usMarketType;              // �г�����
    unsigned char  m_ucOrderBy;                 // ��������
    unsigned char  m_ucSeqType;                 // ����(����,����)        
    unsigned short m_usBeginPos;                // ��ʼλ�� ( ��1 ��ʼ)
    unsigned short m_usReqNum;                  // �������
    unsigned short m_usReqDesSymoblNum;         // ָ���������; �����0,��ô��ʾ����ǰ�г���������������Ʒ
    STUReportDataIndex_ZYQ()
    {
        memset(this, 0, sizeof(STUReportDataIndex_ZYQ));
    }
};


struct STUQuoteDateTime
{
    int m_iTradeDay;                        // ������   YYYYMMDD
    INT64 m_iUpdateDataTime;                  // ����ʱ�� YYYYMMDDHHMMSS
};

struct STUReportPublicData
{
    unsigned int m_uiNewPrice;              //  ���¼�(��ǰ��)
    unsigned int m_uiNewQty;                //  ���³ɽ���(��ǰ��) 2
    unsigned char m_chTurnoverDirec;        // �ɽ�����  0= B 1= S 2=ƽ 3=�޳ɽ�
    int m_iPriceRiseDown;                   // �ǵ���
    int m_iPriceRiseDownRate;               // �ǵ����� 2
    unsigned int m_uiOpenPrice;             // ���̼�
    unsigned int m_uiMaxPrice;              // ��߼�
    unsigned int m_uiMinPrice;              // ��ͼ�
    unsigned int m_uiYesClosePrice;         // ���ռ�
    unsigned int m_uiYesSetllPrice;         // ����
    unsigned int m_uiTotalTurnoverQty;      // �ܳɽ���
    INT64 m_iTotalTurnoverFunds;            // �ܳɽ���
    int m_iQtyRate;                        // ����
    int m_iOrderRate;                      // ί��
};

struct STURealPublicDataPush
{
    unsigned int m_uiNewPrice;          //  ���¼�(��ǰ��)
    unsigned int m_uiNewQty;            //  ���³ɽ���(��ǰ��)
    unsigned char m_chTurnoverDirec;    // �ɽ�����  0= B 1= S 2=ƽ 3=�޳ɽ�
    int m_iPriceRiseDown;               // �ǵ���
    int m_iPriceRiseDownRate;           // �ǵ�����
    unsigned int m_uiTotalTurnoverQty;  // �ܳɽ���
    INT64 m_iTotalTurnoverFunds;        // �ܳɽ���
    int m_uiQtyRate;                    // ����
    int m_uiOrderRate;                  // ί��

};
struct STUReportDataResp_ZYQ
{
    STUSimpleSymbol_ZYQ m_oSSymobl;     // ��Ʒ��Ϣ
    STUQuoteDateTime m_oTDateTime;
    STUReportPublicData m_oReportPublicData;
    STUNowDataVolPirce m_oQtyPriceV1;   // һ������

    STUReportDataResp_ZYQ()
    {
        memset(this, 0, sizeof(STUReportDataResp_ZYQ));
    }
};

struct STURealDataPush_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STURealPublicDataPush m_oRealPublicData;
    STUNowDataVolPirce m_oQtyPriceV1;   // һ������

    STURealDataPush_ZYQ()
    {
        memset(this, 0, sizeof(STURealDataPush_ZYQ));
    }
};

struct STURealDataDetailPush_Ext_ZYQ
{
    int m_iOrderDiff;                      // ί��(������)
    unsigned int m_uiPOrderSaleQty;         // ����
    unsigned int m_uiPOrderBuyQty;          // ����
   
};

struct STUReportDataDetailResp_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STUReportPublicData m_oReportPublicData;
    STURealDataDetailPush_Ext_ZYQ m_oExt;
    unsigned short m_usVolNum;              // ��λ�۸�����

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
    unsigned short m_usVolNum;              // ��λ�۸�����

    STURealDataDetailPush_ZYQ()
    {
        memset(this, 0, sizeof(STURealDataDetailPush_ZYQ));
    }

    void CopyTo(STURealDataPush_ZYQ& dst, const STUNowDataVolPirce& v1)
    {
        dst.m_oTDateTime = m_oTDateTime;
        dst.m_oRealPublicData = m_oRealPublicData;
        dst.m_oQtyPriceV1 = v1;   // һ������
    }
};

struct STUHisKUnit_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    unsigned int m_uiOpenPrice;       // ���̼�
    unsigned int m_uiClosePrice;      // ���̼�
    unsigned int m_uiMaxPrice;        // ��߼�
    unsigned int m_uiMinPrice;        // ��ͼ�
    INT64 m_iTotalTurnoverFunds;      // �ɽ���
    unsigned int m_uiTrunoverQty;     // �ɽ���

    STUHisKUnit_ZYQ()
    {
        memset(this, 0, sizeof(STUHisKUnit_ZYQ));
    }
};

struct STUSTKMinUnit_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    unsigned int m_uiTurnoverPrice;     // �ɽ���  �ǵ� = �ɽ��� - ����
    unsigned int m_uiTrunoverFunds;     // �ɽ��� ����= �ɽ���/�ɽ���
    unsigned int m_uiTrunoverQty;       // �ɽ���        

    STUSTKMinUnit_ZYQ()
    {
        memset(this, 0, sizeof(STUSTKMinUnit_ZYQ));
    }
};



struct STUTraceUnit_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    unsigned char m_chTurnoverDirec;             // �ɽ�����(B/S or up/down) 0= B 1= S 2=ƽ // TURNOVER_DEIRC_B  TURNOVER_DEIRC_S TURNOVER_DEIRC_F
    unsigned int m_uiPrice;             // �ɽ��۸�
    unsigned int m_uiQty;               // �ɽ�����

    STUTraceUnit_ZYQ()
    {
        memset(this, 0, sizeof(STUTraceUnit_ZYQ));
    }
};

enum
{
    COMPLEX_RANKING_TYPE_DAY_RISE_RATE = 1,     // �����Ƿ�����
    COMPLEX_RANKING_TYPE_DAY_DOWN_RATE,         // ���յ�������
    COMPLEX_RANKING_TYPE_5MIN_RISE_RATE,        // 5�����Ƿ�����
    COMPLEX_RANKING_TYPE_5MIN_DOWN_RATE,        // 5���ӵ�������
    COMPLEX_RANKING_TYPE_DAY_ORDER_RATE_TOP,    // ����ί��ǰ����
    COMPLEX_RANKING_TYPE_DAY_ORDER_RATE_BOTTOM, // ����ί�Ⱥ���
    COMPLEX_RANKING_TYPE_DAY_AMPLITUDE,         // �����������
    COMPLEX_RANKING_TYPE_DAY_QTY_RATE,          // ������������
    COMPLEX_RANKING_TYPE_DAY_TRUN_FUNS,         // ���ճɽ��������
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

// ���׷��� <-> �������  Э��

// ��ʼ���г�
// ���ܺ� INITMARKET_ZYQ =0x1a00;
// ����              
// ��Ӧ SNormaHead + STUInitMarketLenDirec_ZYQ + STUMarketInfo_ZYQ[N] + STUSymbolTradeTime_ZYQ[N] + STUSymbolInfo_ZYQ[N];

// ���۱�
// ���ܺ�  REPORTDATA_DETAIL_ZYQ =0x02a0;
// ����
// ��Ӧ SNormaHead + STUNowDataDetail_ZYQ[N}

// ʵʱ���� PUSH_REALDATA_DETAIL_ZYQ =0x0ba1;
// ���� SNormaHead + STUNowDataDetail_ZYQ(����)

// �����ź� 
// ���ܺ� OPEN_MAKRKET_ZYQ =0x1afe;
// ���� SNormaHead + STUOpenQuoteInfo_ZYQ(����)

// �����ź� 
// ���ܺ� CLOSE_MAKRKET_ZYQ =0x1aff;
// ���� SNormaHead + STUCloseQuoteInfo_ZYQ(����)



struct STUNowData_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STUSimpleSymbol_ZYQ m_oSymbolInfo;
    unsigned int m_uiYesClosePrice;     // ���ռ�
    unsigned int m_uiYesSettlPrice;     // ����
    unsigned int m_uiOpenPrice;         // ���̼�
    unsigned int m_uiMaxPrice;          // ��߼�
    unsigned int m_uiMinPrice;          // ��ͼ�
    unsigned int m_uiNewPrice;          // ���¼�(��ǰ��)
    unsigned int m_uiNewQty;            // ������
    unsigned char m_chTurnoverDirec;    // �ɽ�����  0= B 1= S 2=ƽ 
    unsigned int m_uiQtyRate;           // ����
    INT64 m_iTotalTrunoverQty;          // �ܳɽ���
    INT64 m_iTotalTrunoverMoney;        // �ܳɽ���
    INT64 m_iTotalTrunoverCount;        // �ܳɽ�����
    INT64 m_iTotalOrderQty;             // ������
    INT64 m_iTotalIssueQty;             // ����
    INT64 m_iUnTrunoverBuy;             // ��δ�ɽ�����
    INT64 m_iUnTrunoverSell;            // ��δ�ɽ�����
    INT64 m_iVolTrunoverBuy;            // ����(�嵵�۸�)�ɽ���
    INT64 m_iVolTrunoverSell;           // ����(�嵵�۸�)�ɽ���
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
    unsigned int m_uiYesClosePrice;     // ���ռ�
    unsigned int m_uiYesSettlPrice;     // ����
    unsigned int m_uiOpenPrice;         // ���̼�
    unsigned int m_uiMaxPrice;          // ��߼�
    unsigned int m_uiMinPrice;          // ��ͼ�
    unsigned int m_uiNewPrice;          // ���¼�(��ǰ��)
    unsigned int m_uiNewQty;            // ������
    unsigned char m_chTurnoverDirec;    // �ɽ�����  0= B 1= S 2=ƽ 
    unsigned int m_uiSettlPrice;        // �����
    INT64 m_iBuyOpen;                   // ����
    INT64 m_iBuyClose;                  // ��ת��
    INT64 m_iSellOpen;                  // ������
    INT64 m_iSellClose;                 // ��ת��
    INT64 m_iTotalOrderCount;           // ��ί�е�����
    INT64 m_iTotalTrunoverQty;          // �ܳɽ���
    INT64 m_iTotalTrunoverMoney;        // �ܳɽ���
    INT64 m_iTotalTrunoverCount;        // �ܳɽ�����
    INT64 m_iTotalOrderQty;             // ������
    INT64 m_iTotalIssueQty;            // ����
    INT64 m_iUnTrunoverBuy;             // ��δ�ɽ�����
    INT64 m_iUnTrunoverSell;            // ��δ�ɽ�����
    INT64 m_iVolTrunoverBuy;            // ����(�嵵�۸�)�ɽ���
    INT64 m_iVolTrunoverSell;           // ����(�嵵�۸�)�ɽ���
    char m_cVolPriceCount;
    STUNowDataVolPirce m_arrVolPrice[MAX_ZYQ_VOL_PRICE];

    STUNowDataDetail_ZYQ()
    {
        memset(this, 0, sizeof(STUNowDataDetail_ZYQ));
    }
};

// ��������
struct STUOpenQuoteInfo_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    STUSimpleSymbol_ZYQ m_oSymbol;
    unsigned int m_uiYesClosePrice;      // ���ռ�
    unsigned int m_uiYesSettlPrice;      // ����
    INT64  m_iTotalOrderQty;             // ������
    INT64  m_iTotalIssueQty;             // ����

    STUOpenQuoteInfo_ZYQ()
    {
        memset(this, 0, sizeof(STUOpenQuoteInfo_ZYQ));
    }
};

struct STUCloseQuoteInfo_ZYQ
{
    STUQuoteDateTime m_oTDateTime;
    int m_iNextTick;                        //��һ��������

    STUSimpleSymbol_ZYQ m_oSymbol;
    unsigned int m_uiYesClosePrice;         // ���ռ�
    unsigned int m_uiYesSettlPrice;         // ����
    unsigned int m_uiOpenPrice;             // ���̼�
    unsigned int m_uiMaxPrice;              // ��߼�
    unsigned int m_uiMinPrice;              // ��ͼ�
    unsigned int m_uiClosePrice;            // �������̼�
    unsigned int m_uiSettlPrice;            // ��������
    unsigned int m_uiQtyRate;               // ����
    INT64 m_iTotalTrunoverQty;             // �ܳɽ���
    INT64 m_iTotalTrunoverMoney;           // �ܳɽ���
    INT64 m_iTotalTrunoverCount;           // �ܳɽ�����
    INT64 m_iVolTrunoverBuy;                // ����(�嵵�۸�)�ɽ���
    INT64 m_iVolTrunoverSell;               // ����(�嵵�۸�)�ɽ���
    INT64 m_iTotalOrderQty;                // ������
    INT64 m_iTotalIssueQty;                // �ܷ�����

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