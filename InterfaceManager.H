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
	TCHAR szName[24];     //���嶨������
	UINT uiID;           //����ID
	int  nSize;          //�����С
	int  nStyle;         //������(����)
	TCHAR szFontName[36]; //��������
	CFont* pFont;        // ����������ָ��

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
    // ��ʼ��
	BOOL Init();

	BOOL ReloadSyStyle(int nSel,CString strStyle);

	//��ȡϵͳ�ķ��
	int GetSysStyle();
	
	// ��ȡϵͳ��������
	BOOL GetSysStyleFileName(int nSysStyle,CString &rstrSysStyleName);
 
	// ����ϵͳָ��������ɫ
	BOOL UpdateSysColor(const TColorDefInfo& tColorDef);    

	// ��ȡϵͳָ��������ɫ
	COLORREF GetSysColor(UINT uiColorID);

	// ����ϵͳָ����������
	BOOL UpdateSysFont(const TFontDefInfo& tFontDef);

	// ��ȡϵͳָ����������
	CFont* GetSysFont(UINT uiFontID);

	// ��ȡ������ͼƬ
	CBitmap* GetSysScrollBmp();

	// ����ϵͳ��ɫ�������ļ�
	BOOL SaveSysColorList(int nSysStyle,const vector<TColorDefInfo>& vecRefColor);

	// ����ϵͳ����������ļ�
	BOOL SaveSysFontList(int nSysStyle,const vector<TFontDefInfo>& vecRefFont);

	// ��ȡϵͳ��ɫ�б�
	const vector<TColorDefInfo>& GetSysColorList();

	// ��ȡϵͳ�����б�
	const vector<TFontDefInfo>& GetSysFontList();

	// ���ݲ��ռ۸񣬻�ȡĿ��۸�ı�����ɫ
	COLORREF GetPriceBackColor(float fDesPrice,float fRefPrice);

	//���崴��(��ϵͳ����ĵ���)
	CFont* CreateFontEx(const TCHAR* cFontName, int nFontSize, int FontStyle, 
		int nCreateType = CREATE_INDIRECT);
	
	int	 GetSysStyleNum(){return m_vecSysStyle.size();};
	BOOL  GetSysStyleName(int nStyle,CString& strName);

	CString GetPriceBoardBkPic(){return m_strPriceBoardBkPic;};
private:
	// ��ʼ��װ��ϵͳ���
	BOOL InitSysStyle();

	// װ��ָ��ϵͳ��������
	BOOL LoadSysFont(CString strSysStyleName);

	// �ͷ������б�
	void FreeSysFont();
		
	// װ��ָ��ϵͳ������ɫ
	BOOL LoadSysColor(CString strSysStyleName);


	// ��ʼ��ϵͳ��ɫ
	void ResetSysColor(int nCount, TColorDefInfo *pColorDefList);	
	
	// ��ʼ��ϵͳ����
	void ResetSysFont(int nCount, TFontDefInfo *pFontDefList);

	// ����������Դ
	void DeleteColorAndFont();	

	//���崴��
	CFont* CreateFont(const TCHAR* cFontName, int nFontSize, int FontStyle, 
		       int nCreateType = CREATE_INDIRECT);

	BOOL ConvertColorParaFile();

	BOOL ConvertFontParaFile();
// Attributes
private:	
	int   m_nCurSysStyle;    //��ǰʹ�õ�ϵͳ���
   
 	vector<TSysStyleDefInfo> m_vecSysStyle;
	vector<TColorDefInfo> m_vecSysColor;
	vector<TFontDefInfo> m_vecSysFont;

	CString     m_strInterfaceParamPath; //�������ò���·��

	CString			m_strPriceBoardBkPic;//���۰屳��ͼ
	
};
#endif // CLASS_INTERFACE_MANAGER_H
