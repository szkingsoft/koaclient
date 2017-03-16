//////////////////////////////////////////////////////////////////////////
//Function:公共数据类
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
//通讯库
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
	//Singlon模式
	static void ExitInstance();
	static CPublic* Instance();
	static CPublic* _instance;

	
	//功能：获取系统路径
	CString GetSysPath();

	//////////////////////////////////////////////////////////////////////////
	//函数功能得到当前证券的前一个证券代码
	//函数参数: CurrentCode 当前的证券代码
	//函数返回：当前证券的前一个代码
	//函数异常：无
	const SCodeInfo* GetPrevStockInfo(const SCodeInfo* pCurrentStock);
	////////////////////////////////////////////////
	//函数功能：得到当前证券的后一个代码
	//函数参数：当前证券代码
	//函数返回：当前证券的后一个代码
	//函数异常:无
	const SCodeInfo* GetNextStockInfo(const SCodeInfo* pCurrentStock);
	////////////////////////////////////////////////
	//函数功能：得到所有市场代码
	//函数参数：lpCodeInfo 得到的市场代码
	//　　　　　dwCodeNum 返回的代码数量
	//函数返回：成功为TRUE,错误为FALSE
	//函数异常:无
	BOOL GetAllMarketCode(SCodeInfo* pCode,int& nCodeNum);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：得到指定市场的证券总数量
	//函数参数: nMarket 市场代码
	//函数返回：市场数量
	//函数异常：无
	int GetStockTotal();

	//////////////////////////////////////////////////////////////////////////
	//函数功能:得到证券信息
	//函数参数:需要得到证券信息的代码信息
	const StuSymbolData* GetStockInfo(const SCodeInfo* pCode = NULL);

	void SetStockTradeStatus(SCodeInfo* pCode);
	//////////////////////////////////////////////////////////////////////////
	//功能：初始化股票
	//参数：lpData 数据块
	//返回：无
	void InitialStockInfo(const char* lpData);

	//功能：返回今日日期
	//参数：无
	//返回：当日日期
	int GetDate();


	//功能：根据市场类型得到证券小数点位数
	//参数：sMarketType 市场类型
	//返回：小数点位数
	unsigned short GetDecimal(const SCodeInfo* pCodeInfo);
	
	
	// 根据技术指标的名称，获取指标值显示的小数点位数
	unsigned short GetTechIndexValueDecimal(CString strTypeName);
	
	// 格式化指定值
	CString GetFormateValue(float fValue,int nDeimal);

	//功能：向服务器发送请求数据
	BOOL SendRequestData(const char* lpData,unsigned int uiDataNum);



	//判断矩形是否有效
	BOOL IsRectValid(CRect rect);

	// 判断指定日与参考日是否在同一个月	
	BOOL IsSameMonth(unsigned int uiRefDate,unsigned int uiCurDate);

	// 判断指定日与参考日是否在同一周
	BOOL IsSameWeek(unsigned int uiRefDate,unsigned int uiCurDate);

	//是否显示键盘精灵
	int NeedKeyGhost(){return m_nNeedKeyGhost;};

public:

	//发送登陆验证信息
	void ReqLoginReq();

	//发送初始化信息
	void ReqInitReq();

	//发送文本信息请求
	void ReqTxtReq();

	//请求报价信息
	void ReqReportInfo();
	//请求证券信息
	void ReqStockInfo();

	//功能：使请求索引号增加
	short AddRequestIndex();

	//功能：得到请求索引号
	short GetRequestIndex();

	//  设置请求历史数据标识
	void SetRequestHisDataFlag(bool bGetting);
	//  获取请求历史数据标识
	bool GetRequestHisDataFlag();

	//得到产品名称
	CString GetProductName()
	{
		return m_strProductName;
	};
	//得到注册链接
	CString GetDemoLink(){return m_strDemoLink;};
	//得到语言种类
	int GetLanguageType(){return m_nLanguageType;};

	//得到是否显示警告信息
	int GetWarnShowStatus(){return m_nShowWarningInfo;};

	//得到附加菜单信息
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
	//功能：初始化系统路径
	void InitSysPath();


private:
	vector <StuSymbolData> m_vecSymbolInfo;//产品信息
    CString           m_szSysPath;//系统路径
	short			  m_sRequestIndex;//请求索引号
	
	int				  m_iTodayDate;//今日时间

	int				  m_nNeedKeyGhost;//是否需要键盘精灵

	CString			  m_strProductName;//产品名称 
	CString			  m_strDemoLink;//模拟版连接

	int				  m_nLanguageType;//语言版本

	int				  m_nShowWarningInfo;//是否显示警告信息

	CString			  m_strEducation;//投资者教育连接


	CString			  m_strAppendMenu;//附加菜单信息
	CString			  m_strAppendMenuLink;//附加菜单链接

	CString			  m_strHow2UsePage;//帮助使用的网页

	int				  m_nMainViewMode;//主窗口模式

	int				  m_nLoginMode;		//登陆模式 

	int				  m_nMoneyInOutEnable;//允许出入金

	SCodeInfo		  m_oCurrentCode;//当前证券

	//////////////////////////////////////////////////////////////////////////
	//add by david gong at 20140105
	//自动升级使用 
	CString			  m_strUpdateServer;//升级服务器
	CString			  m_strUpdateTag;		//标志文件
	int				  m_nUpdateServerPort;//升级服务器地址
	int             m_iUpdateTmOut;

//	CWnd* m_pMainWnd;				//主框架句柄
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
	bool    InitBalanceServer();//初始化策略服务器
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
	CString m_strQuoteIP;//行情IP
	CString m_strTradeIP;//交易IP
	int	   m_nQuotePort;//行情端口
	int		m_nTradePort;//交易端口
	//备份服务器
	CString m_strQuoteIP1;//行情IP
	CString m_strTradeIP1;//交易IP
	int	   m_nQuotePort1;//行情端口
	int		m_nTradePort1;//交易端口

	CString m_strQuoteIP2;//行情IP
	CString m_strTradeIP2;//交易IP
	int	   m_nQuotePort2;//行情端口
	int		m_nTradePort2;//交易端口

	CString m_strQuoteIP3;//行情IP
	CString m_strTradeIP3;//交易IP
	int	   m_nQuotePort3;//行情端口
	int		m_nTradePort3;//交易端口

	CString m_strQuoteIP4;//行情IP
	CString m_strTradeIP4;//交易IP
	int	   m_nQuotePort4;//行情端口
	int		m_nTradePort4;//交易端口

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
	//公式命令
	void OnFormulaCommand(char* pCommand);

	//接受并分发服务器传过来的数据
	void UpdateData(const char* lpData,int nDataLen,UINT iNotifyMode = 1); 
	//处理键盘精灵接口(股票代码和股票名称),默认为只有成交价、成交量图
	void ChangeStockEvent(const SCodeInfo* pCodeInfo,UINT uiInPage);


	int	GetCustomUnit(const char* pCode,float& fBuyUnit,float& fSellUnit,int& nSort);
	//快速交易
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
	void ChangePwd(CString strPwd);//修改密码
private:
	CClientDataSource		m_QuoteDataSource;
	HWND	m_hLoginWnd;
	//vector<SViewInfo> m_vecViewInfo; //视图信息
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
