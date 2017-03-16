///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#ifndef CLASS_INTERFACE_MANAGER_H
#define CLASS_INTERFACE_MANAGER_H

#pragma pack(1)
typedef struct tagColorDef
{
	int nStyle;
	TCHAR szName[20];
	TCHAR szFileName[20];
}TSysStyleDefInfo;

typedef struct tagColorDefine
{
	TCHAR     szName[30];
	UINT	 uiID;
	COLORREF crColor;
}TColorDefInfo;

typedef struct tagFontDefine
{
	TCHAR szName[24];     //字体定义名称
	UINT uiID;           //字体ID
	int  nSize;          //字体大小
	int  nStyle;         //字体风格(字形)
	TCHAR szFontName[36]; //字体名称
	CFont* pFont;        // 创建的字体指针

	BOOL operator ==(const tagFontDefine& rv)
	{
		if( (_tcscmp( this->szFontName,rv.szFontName) == 0)
			&& ( this->nSize == rv.nSize)
			&& ( this->nStyle == rv.nStyle))
		{
			return TRUE;
		}

		return FALSE;
	}

}TFontDefInfo;

enum
{
	CREATE_INDIRECT,
	CREATE_POINT
};
#pragma pack()


class CInterfaceManager 
{
// Construc1tion / Destruction
public:
	static CInterfaceManager* Instance();
	static void ExitInstance();
	static CInterfaceManager* _instance;
	
private:	
	CInterfaceManager();	
	~CInterfaceManager();	

// Implementation
public:
    // 初始化
	BOOL Init();

	BOOL ReloadSyStyle(int nSel,CString strStyle);

	//获取系统的风格
	int GetSysStyle();
	
	// 获取系统类型名称
	BOOL GetSysStyleFileName(int nSysStyle,CString &rstrSysStyleName);
 
	// 设置系统指定类型颜色
	BOOL UpdateSysColor(const TColorDefInfo& tColorDef);    

	// 获取系统指定类型颜色
	COLORREF GetSysColor(UINT uiColorID);

	// 设置系统指定类型字体
	BOOL UpdateSysFont(const TFontDefInfo& tFontDef);

	// 获取系统指定类型字体
	CFont* GetSysFont(UINT uiFontID);

	// 获取滚动条图片
	CBitmap* GetSysScrollBmp();

	// 保存系统颜色参数到文件
	BOOL SaveSysColorList(int nSysStyle,const vector<TColorDefInfo>& vecRefColor);

	// 保存系统字体参数到文件
	BOOL SaveSysFontList(int nSysStyle,const vector<TFontDefInfo>& vecRefFont);

	// 获取系统颜色列表
	const vector<TColorDefInfo>& GetSysColorList();

	// 获取系统字体列表
	const vector<TFontDefInfo>& GetSysFontList();

	// 依据参照价格，获取目标价格的背景颜色
	COLORREF GetPriceBackColor(float fDesPrice,float fRefPrice);

	//字体创建(含系统字体的调整)
	CFont* CreateFontEx(const TCHAR* cFontName, int nFontSize, int FontStyle, 
		int nCreateType = CREATE_INDIRECT);
	
	int	 GetSysStyleNum(){return m_vecSysStyle.size();};
	BOOL  GetSysStyleName(int nStyle,CString& strName);

	CString GetPriceBoardBkPic(){return m_strPriceBoardBkPic;};
private:
	// 初始化装载系统风格
	BOOL InitSysStyle();

	// 装载指定系统风格的字体
	BOOL LoadSysFont(CString strSysStyleName);

	// 释放字体列表
	void FreeSysFont();
		
	// 装载指定系统风格的颜色
	BOOL LoadSysColor(CString strSysStyleName);


	// 初始化系统颜色
	void ResetSysColor(int nCount, TColorDefInfo *pColorDefList);	
	
	// 初始化系统字体
	void ResetSysFont(int nCount, TFontDefInfo *pFontDefList);

	// 清除分配的资源
	void DeleteColorAndFont();	

	//字体创建
	CFont* CreateFont(const TCHAR* cFontName, int nFontSize, int FontStyle, 
		       int nCreateType = CREATE_INDIRECT);

	BOOL ConvertColorParaFile();

	BOOL ConvertFontParaFile();
// Attributes
private:	
	int   m_nCurSysStyle;    //当前使用的系统风格
   
 	vector<TSysStyleDefInfo> m_vecSysStyle;
	vector<TColorDefInfo> m_vecSysColor;
	vector<TFontDefInfo> m_vecSysFont;

	CString     m_strInterfaceParamPath; //界面设置参数路径

	CString			m_strPriceBoardBkPic;//报价板背景图
	
};
#endif // CLASS_INTERFACE_MANAGER_H

