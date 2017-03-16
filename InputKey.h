// InputKey.h: interface for the CInputKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTKEY_H__762F1160_A96A_11D3_BF51_0080C8EAED17__INCLUDED_)
#define AFX_INPUTKEY_H__762F1160_A96A_11D3_BF51_0080C8EAED17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyGhostStruct 
{
public:	
	char m_szStockCode[17];	//代码
	char m_szStockName[20]; //名字
	char m_First[9];	//
	char m_Alias[26]; //别名,拼音简写
	UINT m_nAlias;
	char m_Type;  
	DWORD m_dwIndex;
	DWORD m_dwCommand;	//命令
	UINT m_uiMarket;
	CKeyGhostStruct()
	{
		memset(this, 0, sizeof(CKeyGhostStruct));
	}
	CKeyGhostStruct & operator =(const CKeyGhostStruct & dt)
	{
		memcpy(this,&dt,sizeof(*this));
		return *this;
	}	
};//键盘精灵结构

class CInputKey  
{
public:
	static CInputKey* _Instance;
	static CInputKey* Instance();
	static void ExInstance();
public:
	void RemoveAll();
	void AddStockCode(const StuSymbolData *pStockInfo, int nNum,UINT uiMarket = 1);
	void AddStockCode_ZYQ(CString strCode,CString strName);
	void ReadCommand();
	//////////////////////////////////////////////////////////////////////////
	// By David sha [4/2/2002]
	//////////////////////////////////////////////////////////////////////////
	
	CString SearchStock(DWORD dwMarket, CString strStockCode);
	static int compare( const void *arg1, const void *arg2 );
	static int compare1( const void *arg1, const void *arg2 );
	int GetSize();
	void DeleteKeyElement(const char *code,  WORD marketID);
	void UpdateKeyElement(const char * code, const char * name, UINT type, UINT uCommand,UINT uiMarket = 0);
	void SearchFrequency(char * code, int nFrequency);
	void SortKey();
	void WriteFrequency();
	void ReadFrequency();
	void SortWantKey(int num);
	void UpdateKeyElement( CKeyGhostStruct   keyPoint);
	void GetAlias(CKeyGhostStruct & keyPoint);
	//void AddKey(CKeyGhostStruct &keyPoint, char ctype = 0);
	void SearchkeyByAlias(const char * Alias, UINT &uNum, char cType=0,UINT uiMarket = 0);
	void SearchkeyByAlias(const char * Alias);
	void SearchKey(const char *strIndex, UINT &uNum, char cType =0);
	void SearchKey(const char * strIndex);
	int AddKey(CKeyGhostStruct  keyPoint);
	CKeyGhostStruct & CInputKey::GetKeyValue(UINT uIndex);
	LONG ulCurRec;
	CInputKey();
	virtual ~CInputKey();
	CArray<CKeyGhostStruct, CKeyGhostStruct &> key;
	CArray<CKeyGhostStruct, CKeyGhostStruct &> keyWanted;
private:
	void GetKeyWantedByAlia(char *str, const char * Alias,int pos, unsigned int &uNum);
	char * py;

};



#endif // !defined(AFX_INPUTKEY_H__762F1160_A96A_11D3_BF51_0080C8EAED17__INCLUDED_)
