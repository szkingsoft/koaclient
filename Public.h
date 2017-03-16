//////////////////////////////////////////////////////////////////////////
//Function:����������
//Author:gongxiaofeng
//////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PUBLIC_409F7B100305_INCLUDED
#define _INC_PUBLIC_409F7B100305_INCLUDED
#include "quotestruct.h"
#include "TradeDll.h"
#include "drawlineDll.h"
//ͨѶ��
#include "clientDataSource.h"
#define		VIEWINFO_PRICEINFO		1
#define		VIEWINFO_PRICELIST		2
#define		VIEWINFO_STOCK			3
#define		VIEWINFO_TRADE			4
#define		VIEWINFO_DEFTRADE		5

struct SViewInfo
{
	SViewInfo()
	{
		nViewType = 0;
		pWnd = NULL;
	}
	int nViewType;
	CWnd* pWnd;
};

struct StuServerInfo
{
	CString strServerName;
	CString strServerUrl;
	int		nServerPort;
	int		nServerType;
};

#define WM_UPDATE_LINKSTATUS WM_USER + 555
#define WM_UPDATE_USERINFO   WM_USER + 556
#define WM_UPDATE_SGRN       WM_USER + 557
class CPublic 
{
private:
	CPublic();
	virtual ~CPublic();

public:
	//////////////////////////////////////////////////////////////////////////
	//Singlonģʽ
	static void ExitInstance();
	static CPublic* Instance();
	static CPublic* _instance;

	
	//���ܣ���ȡϵͳ·��
	CString GetSysPath();

	//////////////////////////////////////////////////////////////////////////
	//�������ܵõ���ǰ֤ȯ��ǰһ��֤ȯ����
	//��������: CurrentCode ��ǰ��֤ȯ����
	//�������أ���ǰ֤ȯ��ǰһ������
	//�����쳣����
	const SCodeInfo* GetPrevStockInfo(const SCodeInfo* pCurrentStock);
	////////////////////////////////////////////////
	//�������ܣ��õ���ǰ֤ȯ�ĺ�һ������
	//������������ǰ֤ȯ����
	//�������أ���ǰ֤ȯ�ĺ�һ������
	//�����쳣:��
	const SCodeInfo* GetNextStockInfo(const SCodeInfo* pCurrentStock);
	////////////////////////////////////////////////
	//�������ܣ��õ������г�����
	//����������lpCodeInfo �õ����г�����
	//����������dwCodeNum ���صĴ�������
	//�������أ��ɹ�ΪTRUE,����ΪFALSE
	//�����쳣:��
	BOOL GetAllMarketCode(SCodeInfo* pCode,int& nCodeNum);
	//////////////////////////////////////////////////////////////////////////
	//�������ܣ��õ�ָ���г���֤ȯ������
	//��������: nMarket �г�����
	//�������أ��г�����
	//�����쳣����
	int GetStockTotal();

	//////////////////////////////////////////////////////////////////////////
	//��������:�õ�֤ȯ��Ϣ
	//��������:��Ҫ�õ�֤ȯ��Ϣ�Ĵ�����Ϣ
	const StuSymbolData* GetStockInfo(const SCodeInfo* pCode = NULL);

	void SetStockTradeStatus(SCodeInfo* pCode);
	//////////////////////////////////////////////////////////////////////////
	//���ܣ���ʼ����Ʊ
	//������lpData ���ݿ�
	//���أ���
	void InitialStockInfo(const char* lpData);

	//���ܣ����ؽ�������
	//��������
	//���أ���������
	int GetDate();


	//���ܣ������г����͵õ�֤ȯС����λ��
	//������sMarketType �г�����
	//���أ�С����λ��
	unsigned short GetDecimal(const SCodeInfo* pCodeInfo);
	
	
	// ���ݼ���ָ������ƣ���ȡָ��ֵ��ʾ��С����λ��
	unsigned short GetTechIndexValueDecimal(CString strTypeName);
	
	// ��ʽ��ָ��ֵ
	CString GetFormateValue(float fValue,int nDeimal);

	//���ܣ��������������������
	BOOL SendRequestData(const char* lpData,unsigned int uiDataNum);



	//�жϾ����Ƿ���Ч
	BOOL IsRectValid(CRect rect);

	// �ж�ָ������ο����Ƿ���ͬһ����	
	BOOL IsSameMonth(unsigned int uiRefDate,unsigned int uiCurDate);

	// �ж�ָ������ο����Ƿ���ͬһ��
	BOOL IsSameWeek(unsigned int uiRefDate,unsigned int uiCurDate);

	//�Ƿ���ʾ���̾���
	int NeedKeyGhost(){return m_nNeedKeyGhost;};

public:

	//���͵�½��֤��Ϣ
	void ReqLoginReq();

	//���ͳ�ʼ����Ϣ
	void ReqInitReq();

	//�����ı���Ϣ����
	void ReqTxtReq();

	//���󱨼���Ϣ
	void ReqReportInfo();
	//����֤ȯ��Ϣ
	void ReqStockInfo();

	//���ܣ�ʹ��������������
	short AddRequestIndex();

	//���ܣ��õ�����������
	short GetRequestIndex();

	//  ����������ʷ���ݱ�ʶ
	void SetRequestHisDataFlag(bool bGetting);
	//  ��ȡ������ʷ���ݱ�ʶ
	bool GetRequestHisDataFlag();

	//�õ���Ʒ����
	CString GetProductName()
	{
		return m_strProductName;
	};
	//�õ�ע������
	CString GetDemoLink(){return m_strDemoLink;};
	//�õ���������
	int GetLanguageType(){return m_nLanguageType;};

	//�õ��Ƿ���ʾ������Ϣ
	int GetWarnShowStatus(){return m_nShowWarningInfo;};

	//�õ����Ӳ˵���Ϣ
	CString GetAppendMenu(){return m_strAppendMenu;};
	CString GetAppendMenuLink(){return m_strAppendMenuLink;};



	int		GetMainViewMode(){return m_nMainViewMode;};

	int		GetLoginMode(){return m_nLoginMode;};

	int		GetMoneyInOutEnable(){return m_nMoneyInOutEnable;};


	const SCodeInfo* GetCurStock();

	void	GetUpdateServer(CString& strServer,WORD& wServerPort,CString& strTag,DWORD &dwOutTm);

	CString	GetEducationStr(){return m_strEducation;};
	CString GetHow2UseWebPage(){return m_strHow2UsePage;};
	CString	GetSoftwareVersion();
private:
	//���ܣ���ʼ��ϵͳ·��
	void InitSysPath();


private:
	vector <StuSymbolData> m_vecSymbolInfo;//��Ʒ��Ϣ
    CString           m_szSysPath;//ϵͳ·��
	short			  m_sRequestIndex;//����������
	
	int				  m_iTodayDate;//����ʱ��

	int				  m_nNeedKeyGhost;//�Ƿ���Ҫ���̾���

	CString			  m_strProductName;//��Ʒ���� 
	CString			  m_strDemoLink;//ģ�������

	int				  m_nLanguageType;//���԰汾

	int				  m_nShowWarningInfo;//�Ƿ���ʾ������Ϣ

	CString			  m_strEducation;//Ͷ���߽�������


	CString			  m_strAppendMenu;//���Ӳ˵���Ϣ
	CString			  m_strAppendMenuLink;//���Ӳ˵�����

	CString			  m_strHow2UsePage;//����ʹ�õ���ҳ

	int				  m_nMainViewMode;//������ģʽ

	int				  m_nLoginMode;		//��½ģʽ 

	int				  m_nMoneyInOutEnable;//��������

	SCodeInfo		  m_oCurrentCode;//��ǰ֤ȯ

	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 20140105
	//�Զ�����ʹ�� 
	CString			  m_strUpdateServer;//����������
	CString			  m_strUpdateTag;		//��־�ļ�
	int				  m_nUpdateServerPort;//������������ַ
	int             m_iUpdateTmOut;

//	CWnd* m_pMainWnd;				//����ܾ��
	/////////yfj
private:
	CWnd * m_pStatusWnd;
	CWnd * m_pToolBarWnd;
	int m_iClientType;
	int	m_IsTrendChart;
	int m_iTrendChart;
	int m_iPriceHight;
	int m_iFundPos;
	int m_iQuoteLstItem[7];
	CCustomUser * m_pCusUser;
	int m_iSkinType;
	int m_iDPUnTrade;
	int m_iIsNeedLock;
	BOOL m_bLock;
	int m_iUseLgOld;
	int m_iInitiAtiveShowWarn;
	BOOL m_bIsClear;
	int m_nShowEducationMenu;
	int m_nShowUseHelpMenu;

	std::map<UINT,CString> m_mapUrl;

	CCriticalSection m_lockfLog;
	CFile fLog;

public:
	BOOL m_bIsShowMall;
	CString m_strMallUrl;
	int GetUseLgOld(){return m_iUseLgOld;}
	CString GetURL(UINT uiKey);
	BOOL IsClear(){return m_bIsClear;}
	BOOL IsShowEducationMenum(){return m_nShowEducationMenu == 0 ? FALSE : TRUE;}
	BOOL IsShowUseHelpMenum(){return m_nShowUseHelpMenu == 0 ? FALSE : TRUE;}
	BOOL IsLockMainClient(){return m_bLock;};
	void SetLockMainClient(BOOL bLock) {m_bLock = bLock;}
	int IsNeedLock(){return m_iIsNeedLock;}

	const SCodeInfo*  GetNextSymbol(const SCodeInfo* pCurStock,int nDec = 1);
	int GetInitiAtiveShowWarn(){return m_iInitiAtiveShowWarn;}
	int GetDPUnTrade(){return m_iDPUnTrade;}
	int GetSkinType(){return m_iSkinType;}
	void WriteSkinType(int iSkinType);
	CCustomUser * GetCustomUser(){ return m_pCusUser; }
	int * GetQuoteLstItemWith(){return m_iQuoteLstItem;}
	int GetFundPos() {return m_iFundPos;}
	int GetPriceHight() {return m_iPriceHight;} 
	void WritePriceHight(int iHight);
	void WriteTrendChartRatio(int iRatio);
	void WriteQuoteLstItemWith(int iItem,int iItemWith);
	int GetTrendChartRatio() {return m_iTrendChart;}
	BOOL IsTrendChart() { return (m_IsTrendChart==0)?TRUE:FALSE;}
	int GetClientType();
	void SetToolBarWnd(CWnd *pWnd = NULL) {m_pToolBarWnd = pWnd;}
	BOOL SendUserChangeMsg(BOOL bChange = TRUE);
	BOOL SendUserSGRNChangeMsg(BOOL bChange = TRUE);

	void SetStatusWnd(CWnd *pWnd = NULL) {m_pStatusWnd = pWnd;}
	BOOL SendLinkStatus(int iServerId,BOOL bLink);

	int GetNeedRegRealQuoSymbol(STUSSymbol * pstcSymbol);

	void NotifyWnd(const char* lpData,int nDataLen,UINT iNotifyMode,UINT iNotifyWndType=0);
	void NotifyWnd(UINT iNotifyMode,int iTypeMsg);
	/////////////////
public:
	bool    InitBalanceServer();//��ʼ�����Է�����
	int	InitTrade(HWND hWnd);//init trade
	int	InitQuote(HWND hWnd);//init quote
	void	UnLinkTrade();
	void	UnLinkQuote();	

	CString GetQuoteServer(){return m_strQuoteIP;};
	CString GetTradeServer(){return m_strTradeIP;};

	void SetCurSelServer(int nSel){m_nSelServer = nSel;};
	int GetCurSelServer(){return m_nSelServer;};
	void GetServerNameList(vector<CString>& vecServer);
	
private:
	CString m_strQuoteIP;//����IP
	CString m_strTradeIP;//����IP
	int	   m_nQuotePort;//����˿�
	int		m_nTradePort;//���׶˿�
	//���ݷ�����
	CString m_strQuoteIP1;//����IP
	CString m_strTradeIP1;//����IP
	int	   m_nQuotePort1;//����˿�
	int		m_nTradePort1;//���׶˿�

	CString m_strQuoteIP2;//����IP
	CString m_strTradeIP2;//����IP
	int	   m_nQuotePort2;//����˿�
	int		m_nTradePort2;//���׶˿�

	CString m_strQuoteIP3;//����IP
	CString m_strTradeIP3;//����IP
	int	   m_nQuotePort3;//����˿�
	int		m_nTradePort3;//���׶˿�

	CString m_strQuoteIP4;//����IP
	CString m_strTradeIP4;//����IP
	int	   m_nQuotePort4;//����˿�
	int		m_nTradePort4;//���׶˿�

	CString	m_strUserName;
	CString	m_strPassword;

	BOOL	m_bSavePassword;
	BOOL	m_bSaveAccount;
	BOOL    m_bIsMinuteMode;
	BOOL    m_bNeedCFCA;
	BOOL    m_bSupportCFCA;

// 	CString m_strBalanceServer1;
// 	int     m_nBalanceServerPort1;


	vector<StuServerInfo> m_vecServerList;

	int		m_nSelServer;

	void ReadIniInfo();

public:
	void GetLoginInfo(CString& strUserName,CString& strPwd,BOOL& bSavePwd,BOOL& bSaveAccount,BOOL& bNeedCFCA,BOOL& bSupportCFCA);
	void WriteIniInfo(CString strUserName,CString strPwd,BOOL bSavePwd,BOOL bSaveAccount= TRUE,BOOL bNeedCFCA = FALSE);
	BOOL IsMinuteDataMode(){return m_bIsMinuteMode;};
	CClientDataSource* GetDataSource(){return &m_QuoteDataSource;}
	CWnd*	GetView(int nType= VIEWINFO_STOCK,UINT iNotifyMode = 1);
	int		GetViewNum(UINT iNotifyMode = 1)
	{
		std::map<UINT,vector<SViewInfo>>::iterator itMap;
		itMap = m_mapNotifyWnd.find(iNotifyMode);
		if (itMap == m_mapNotifyWnd.end())return 0;

		return /*m_vecViewInfo.size()*/itMap->second.size();
	};

	const SCodeInfo* GetDefaultStock();
public:
	//��ʽ����
	void OnFormulaCommand(char* pCommand);

	//���ܲ��ַ�������������������
	void UpdateData(const char* lpData,int nDataLen,UINT iNotifyMode = 1); 
	//������̾���ӿ�(��Ʊ����͹�Ʊ����),Ĭ��Ϊֻ�гɽ��ۡ��ɽ���ͼ
	void ChangeStockEvent(const SCodeInfo* pCodeInfo,UINT uiInPage);


	int	GetCustomUnit(const char* pCode,float& fBuyUnit,float& fSellUnit,int& nSort);
	//���ٽ���
	int  CallQuickTrade(const SCodeInfo* pCode,int nMode =0,int nType=0,float fPrice=0.0f);
	HWND  GetLoginWnd()
	{
		if (IsWindow(m_hLoginWnd))
		{
			return m_hLoginWnd;
		}
		return NULL;
	};
	void  ResetLoginWnd(HWND hWnd = NULL){m_hLoginWnd = hWnd;};

	void RegistWnd(int nViewType,CWnd* pWnd,UINT iNotifyMode = 1);
	void UnRegistWnd(int nViewType,CWnd* pWnd,UINT iNotifyMode = 1);

	void CallStockMenu(UINT uID,const SCodeInfo* pCode);

	CDrawLineDll* GetDrawDll(){return &m_dllDrawline;};
	CTradeDll* GetTradeDll(){return &m_dllTrade;};
	BOOL ConfirmPwd(CString strPwd);
	void ChangePwd(CString strPwd);//�޸�����
private:
	CClientDataSource		m_QuoteDataSource;
	HWND	m_hLoginWnd;
	//vector<SViewInfo> m_vecViewInfo; //��ͼ��Ϣ
	std::map<UINT,vector<SViewInfo>> m_mapNotifyWnd;
	string	 m_ssDefaultCode;
	CTradeDll		m_dllTrade;
	CDrawLineDll    m_dllDrawline;
	bool            m_bIsGettingHisData;

private:
	//add by witming 
	CString m_strMallName;
	Color   m_crMallFontColor;
	//end by witming 

public:
	CString GetMallName(){ return m_strMallName;}
	Color   GetMallFontColor(){ return m_crMallFontColor; }
};
#endif /* _INC_PUBLIC_409F7B100305_INCLUDED */
