#ifndef  __TRANSLATE_STRUCT_H__
#define  __TRANSLATE_STRUCT_H__
#include "quotedefzyq.h"
#include "quotestruct.h"
#include "quoteconst.h"


struct STUSymbolInfo_ZYQ_Client
{
    string m_ssSymbolCode;             // ��Ʒ����
    string m_ssSymbolName;             // ��Ʒ����
    unsigned short m_usTradeTimeIndex; // ���׽����� (��λ��ʾ)
    unsigned short m_usMarketType;     // �г�����
    unsigned char m_ucPriceUnit;       // �۸�С��λ
    unsigned char m_ucTrade;           // �Ƿ��� 0���ǽ���  1������
    unsigned int m_uiTotalShares;      // ��ͨ����(�ܹɱ���)
    INT64 m_iAgreeunit;                // 4��С��
};


struct STUMarketInfo_ZYQ_Client
{
    unsigned short m_usMarketType;  // �г�����
    string m_ssMarketName;          // �г�����
};

struct STUInitMarket_ZYQ_Client
{
    vector<STUMarketInfo_ZYQ_Client> m_vMarketInfo; // �����г���Ϣ
    vector<STUSymbolTradeTime_ZYQ> m_vTradeTime;    // ���׽���Ϣ
    vector<STUSymbolInfo_ZYQ_Client> m_vSymbolInfo; // ��Ʒ��Ϣ
};


struct STUSimpleSymbol_ZYQ_Client
{
    unsigned char m_ucPriceUnit;
    unsigned short m_usMarketType; // �г�����
    string ssSymbolCode;           // ��Ʒ����
};

struct STUOpenMarketInfo_ZYQ_Client
{
    unsigned int m_uiYesClosePrice; // ���ռ�
    unsigned int m_uiYesSettlPrice; // ����
    INT64  m_iTotalIssueQty;        // ����
};

struct STUOpenMarket_ZYQ_Client
{
    STUSimpleSymbol_ZYQ_Client m_oSSymobl; 
    STUQuoteDateTime m_oTDateTime;
    STUOpenMarketInfo_ZYQ_Client m_oOpenInfo;
};

struct STUCloseMarketInfo_ZYQ_Client
{
    unsigned int m_uiYesClosePrice;     // ���ռ�
    unsigned int m_uiYesSettlPrice;     // ����
    unsigned int m_uiOpenPrice;         // ���̼�
    unsigned int m_uiMaxPrice;          // ��߼�
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
    STUSimpleSymbol_ZYQ_Client m_oSSymobl;     // ��Ʒ��Ϣ
    STUQuoteDateTime m_oTDateTime;
    STUReportPublicData m_oReportPublicData;
    STUNowDataVolPirce m_oQtyPriceV1;           // һ������
};

struct STUReportDataResp_ZYQ_Client
{
    STUReportDataIndex_ZYQ m_oRepIndex;
    vector<STUReportDataResp_ZYQ_Client_Vec> m_vReportData;
};

struct STURealDataPush_ZYQ_Client
{
    STUSimpleSymbol_ZYQ_Client m_oSSymobl;     // ��Ʒ��Ϣ
    STUQuoteDateTime m_oTDateTime;
    STURealPublicDataPush m_oRealPublicData;
    STUNowDataVolPirce m_oQtyPriceV1;   // һ������
};

struct STUReportDataDetailResp_ZYQ_Client
{

    STUSimpleSymbol_ZYQ_Client m_oSSymobl;     // ��Ʒ��Ϣ
    STUQuoteDateTime m_oTDateTime;
    STUReportPublicData m_oReportPublicData;
    STURealDataDetailPush_Ext_ZYQ m_oExt;
    vector<STUNowDataVolPirce> m_vQtyPriceV;     // ��λ����
};

struct STURealDataDetailPush_ZYQ_Client
{
    STUSimpleSymbol_ZYQ_Client m_oSSymobl;     // ��Ʒ��Ϣ
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
    STUInitMarket_ZYQ_Client m_oInitMarket;    //��ʼ���г�
    STUOpenMarket_ZYQ_Client m_oOpenMarket;
    STUCloseMarket_ZYQ_Client m_oCloseMarket;
    STUReportDataResp_ZYQ_Client m_oReportData; 
    STUReportDataDetailResp_ZYQ_Client m_oReportDateDetail; 
    STUTraceUnit_ZYQ_Client m_oTraceUnit; 
    STUSTKMinUnit_ZYQ_Client m_oMinUnit;    //��ʱ�ɽ�
    STUHisKUnit_ZYQ_Client   m_oHisKUnit;   
    STURealDataPush_ZYQ_Client  m_oRealData;  //ʵʱ����
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