// 键盘宝实现模块
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "koaclient.h"
#include "InputKey.h"
#include "Input.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXCODENUM 30

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CInputKey* CInputKey::_Instance = NULL;
CInputKey* CInputKey::Instance()
{
	if(NULL == _Instance)
	{
		_Instance = new CInputKey;
	}
	return _Instance;	
}
void CInputKey::ExInstance()
{
	if(_Instance) 
	{
		delete _Instance;
		_Instance = NULL;
	}	
}

CInputKey::CInputKey()
{
	key.SetSize(0, 1000); 
}

CInputKey::~CInputKey()
{
	WriteFrequency();
	key.RemoveAll();
	keyWanted.RemoveAll();	
}

int CInputKey::AddKey(CKeyGhostStruct  keyPoint)
{
	GetAlias(keyPoint);
  	key.Add(keyPoint);
	return 0;
}

void  CInputKey::SearchKey(const char *strIndex)
{
	int len;
	len= strlen(strIndex);
	
	keyWanted.RemoveAll();
	
	if(!key.GetSize()) return;
	
	for(int i = 0; i<key.GetSize(); i++)
	{
		if(strstr(key[i].m_szStockCode, strIndex))
		{
			keyWanted.Add(key[i]);
		}
	}
}

CKeyGhostStruct & CInputKey::GetKeyValue(UINT uIndex)
{
	return (key[uIndex]);
}
void CInputKey::SearchKey(const char *strIndex, UINT &uNum, char cType)
{
	int len = strlen(strIndex);
	if(len == 0) return ;
	
	if(!key.GetSize()) return;
	
	
	if(key.GetSize() != keyWanted.GetSize()) keyWanted.SetSize(key.GetSize());
	
	
    UINT keyNum = key.GetSize();	 
	
	for(DWORD i = 0; i<keyNum;  i++)
	{
        if(cType != 0)
		{
			if(key[i].m_Type != 0 ) continue;
		}
		if(strstr(key[i].m_szStockCode, strIndex))
		{
			keyWanted.SetAt(uNum++, (key[i]));
		}
	}
}

void CInputKey::SearchkeyByAlias(const char *Alias)
{
	int len;
	len= strlen(Alias);
	
	keyWanted.RemoveAll();
	
	if(!key.GetSize()) return;

	if(key.GetSize() != keyWanted.GetSize()) keyWanted.SetSize(key.GetSize());
	char *pAliasString;
	for(int i = 0; i<key.GetSize(); i++)
	{
        pAliasString = key[i].m_Alias;

		size_t  lAliasString = strlen(key[i].m_Alias);

		while(lAliasString > 0)
		{
			if(strstr(pAliasString, Alias))
			{
				keyWanted.Add(key[i]);
				break;
			}
			pAliasString += lAliasString + 1;
		    lAliasString = strlen( pAliasString );
		}
	}
}

void CInputKey::SearchkeyByAlias(const char *Alias, UINT &uNum, char cType,UINT uiMarket)
{
	int len = strlen(Alias);
	if(len == 0) return ;
	if(!key.GetSize()) return;
	
    UINT keyNum = key.GetSize();	

	char tmp[40];
    CString temPtr;
	
	
	for(DWORD i = 0; i<keyNum;  i++)
	{
		CKeyGhostStruct& keyinfo = key[i];
		if (keyinfo.m_uiMarket != 0 && keyinfo.m_uiMarket != uiMarket)
		{
			continue;
		}
		if((strstr(keyinfo.m_szStockCode, Alias)))//股票代码相似
		{
			keyWanted.SetAtGrow(uNum++, keyinfo);
		}
		char s[20];
		memset(s,0,sizeof(s));
		memset(tmp,0, sizeof(40));

		char *p0=keyinfo.m_Alias;
		char *p1 = keyinfo.m_Alias+5;
		char *p2 = keyinfo.m_Alias+10;
		char *p3 =keyinfo.m_Alias+15;
		char *p4 = keyinfo.m_Alias+20;

		for(char *s0 = p0;*s0; *s0++)
		{
            memset(s, 0, sizeof(s));
			s[0]=*(s0);

			if(*p1)
			{
				for(char *s1 = p1; *s1;  *s1++)
				{
					s[1]=*(s1);
					if(*p2)
					{
						for(char *s2 = p2; *s2; *s2++)
						{
							s[2]=*(s2);
							if(*p3)
							{
								for(char *s3 = p3; *s3; *s3++)
								{  
									s[3]=*(s3);
									if(*p4)
									{
										for(char *s4 = p4; *s4; * s4++)
										{
											s[4]=*(s4);
											GetKeyWantedByAlia(s, Alias, i, uNum);
										}
									}
									else
									{
										GetKeyWantedByAlia(s, Alias, i, uNum);
									}
								}
							}
							else
							{
								GetKeyWantedByAlia(s, Alias, i, uNum);
							}
						}
					}
					else
					{
						GetKeyWantedByAlia(s, Alias, i, uNum);
					}
				}
			}
			else
			{
               GetKeyWantedByAlia(s, Alias, i, uNum);
			}
		}					
	}
}


void CInputKey::GetAlias(CKeyGhostStruct &keyPoint)
{
    UINT	nSize;
	HGLOBAL	hRes;
	LPVOID	p;
	HRSRC hSrc;
	HINSTANCE	g_hInstance;
	g_hInstance = AfxGetApp()->m_hInstance;
	hSrc = FindResource(g_hInstance, MAKEINTRESOURCE(IDR_PY_DATA),_T("PY_DATA"));

	nSize = SizeofResource(g_hInstance, hSrc);
	hRes = LoadResource(g_hInstance, hSrc);
	
	
	p = LockResource(hRes);

	py = (char *)p;


	struct hz
	{
		unsigned char col;
		unsigned char row;
	}AHZ;


	UINT nAlias = 0;
	
	char Alias[256];
	memset(Alias, 0,sizeof(Alias));
	
	char *str;
	str = Alias;

	for(DWORD i = 0; i < strlen(keyPoint.m_szStockName); i++)
	{
		if(keyPoint.m_szStockName[i] < 0)
		{
			AHZ.col = keyPoint.m_szStockName[i];
			AHZ.row = keyPoint.m_szStockName[++i];
			BYTE col,row;

			if(AHZ.col!=0xa3)
			{
                if( (AHZ.col>=0xa0+16)&&(AHZ.col<=0xa0+87))
				{
                   if( (AHZ.row>0xa0) &&(AHZ.row <=0xa0+94))
				   {
					col = byte(AHZ.col-0xa0-16);
			        row = byte(AHZ.row-0xa0-1);
					UINT lOff = col * 94 + row;
			        if(lOff * 4 <= nSize) memcpy(str,py+lOff*4,4);
			        str+=5;
					nAlias++;
				   }
				}
				if(AHZ.col == 0xa2) 
				{
					if((AHZ.row >= 0xc5) && (AHZ.row <= 0xce))
					{
						*str=char('1'+AHZ.row-0xc5);
						str+=5;
						nAlias++;
					}

					if((AHZ.row >= 0xE5) && (AHZ.row <= 0xEe))
					{
						*str=char('1'+AHZ.row-0xE5);
						str+=5;
						nAlias++;
					}
				}
			}
			else
			{
				if( (AHZ.row>=0xc1) &&(AHZ.row<=(0xc1+26)))
				{
						*str=char('A'+AHZ.row-0xc1);
						str+=5;
						nAlias++;
				}
				else
				{
					if( (AHZ.row>=0xB0) && (AHZ.row<=0xB9))
					{
						*str = char('0'+AHZ.row-0xB0);
						str+=5;
						nAlias++;
					}
				}

			}
		}
		else
		{
            if( (keyPoint.m_szStockName[i]!= 32))
			{
				*str = keyPoint.m_szStockName[i];
			    str+=5;
				nAlias++;
			}
		}
	}
	memcpy(keyPoint.m_Alias, Alias, 25);
	keyPoint.m_nAlias = nAlias < 20? nAlias:20;
	UnlockResource(hRes);
	FreeResource(hRes);

}

void CInputKey::UpdateKeyElement(CKeyGhostStruct keyPoint)
{
	int len = strlen(keyPoint.m_szStockCode);
	if(len == 0)
		return ;
	
	if(!key.GetSize()) 
	{
		GetAlias(keyPoint);
		key.Add(keyPoint);
		return;
	}
	
    UINT keyNum = key.GetSize();	 
	
	int i;
	for( i = 0; i<keyNum;  i++)
	{
		if(!strcmp(key[i].m_szStockCode, keyPoint.m_szStockCode))
		{
			 GetAlias(keyPoint);
			 key.SetAt(i, keyPoint);
			 break;
		}
	}

	if(i>=keyNum)
	{
        GetAlias(keyPoint);
		key.SetAtGrow(i, keyPoint);
	}
}
int CInputKey::compare( const void *arg1, const void *arg2 )
{
   /* Compare all of both strings: */
	CKeyGhostStruct *key1, *key2;
	key1 =(CKeyGhostStruct*)(arg1);
	key2= (CKeyGhostStruct*)(arg2);
	char *p1, *p2;

	if(key1->m_dwIndex != key2->m_dwIndex) return (key2->m_dwIndex - key1->m_dwIndex);

	if(strlen(key1->m_First)) p1=key1->m_First;
	else p1 = key1->m_szStockCode;

	if(strlen(key2->m_First)) p2=key2->m_First;
	else p2 = key2->m_szStockCode;


	if((strlen(p1)-strlen(p2))) return (strlen(p1)-strlen(p2));
   return _stricmp(p1, p2);
}



void CInputKey::SortWantKey(int num)
{
	qsort( (void *)(keyWanted.GetData()), (size_t)num, sizeof(CKeyGhostStruct), compare );
}

void CInputKey::ReadFrequency()
{

	CKeyGhostStruct keys;
	DWORD dwValueLen, dwDataLen, dwDataType;
	char bData[20];
	TCHAR szValueName[20];
	
	int iRc ;
	DWORD dwIndex = 0;
	DWORD num =0;

	HKEY hReadKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\FutureTrend\\KeyInput"), 0, KEY_READ, &hReadKey);

	//PFILETIME ftLastWriteTime;

	do 
	{
		dwValueLen= sizeof(szValueName);
		dwDataLen = sizeof(bData);

		iRc = RegEnumValue(hReadKey, dwIndex, szValueName,&dwValueLen, NULL, &dwDataType, (LPBYTE)bData, &dwDataLen);
		if(iRc == ERROR_SUCCESS)
		{
            if(dwDataType == REG_DWORD)
			{
				if(num++ >= MAXCODENUM) break;
				UINT uIndex = 0;
				uIndex = (*bData) - 1;
				if( ((*bData) > 0) && (uIndex < 101)) SearchFrequency((char*)(LPCTSTR)CString(szValueName), (DWORD )(*bData)-1);
			}
		}
		dwIndex ++;
	}while(iRc==ERROR_SUCCESS);

	RegCloseKey(hReadKey);
}

void CInputKey::WriteFrequency()
{
	SortKey();
	HKEY hWriteKey;
	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,
									_T("Software\\FutureTrend"),
									0,
									KEY_ALL_ACCESS,
									&hWriteKey))
	{//删除原来的键值
		RegDeleteKey(hWriteKey,_T("KeyInput"));
		RegCloseKey(hWriteKey);
	}
	DWORD dwReturn;
	if(	ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE,
										_T("Software\\FutureTrend"),
										0,
										_T("KeyInput"),
										REG_OPTION_VOLATILE,
										KEY_ALL_ACCESS,
										NULL,
										&hWriteKey,
										&dwReturn))
	{

		int num =0;
		
		for(int i= 0; i<key.GetSize();i++)
		{
		   if( CInputKey::Instance()->key[i].m_dwIndex > 0 )
			{
				if( num++ >= MAXCODENUM) break;
				RegSetValueEx(hWriteKey,
								CString(CInputKey::Instance()->key[i].m_szStockCode),
								0,
								REG_DWORD,
								(BYTE*)CInputKey::Instance()->key[i].m_dwIndex,
								sizeof(DWORD));
				RegCloseKey(hWriteKey);
			}
		}
	}
}
int CInputKey::compare1( const void *arg1, const void *arg2 )
{
	CKeyGhostStruct *key1, *key2;
	key1 =(CKeyGhostStruct*)(arg1);
	key2= (CKeyGhostStruct*)(arg2);
	return (key2->m_dwIndex - key1->m_dwIndex);
}


void CInputKey::SortKey()
{
	qsort( (void *)(key.GetData()), (size_t)key.GetSize(), sizeof(CKeyGhostStruct), compare1);
}

void CInputKey::SearchFrequency(char * szStockCode, int nFrequency)
{
	int len = strlen(szStockCode);
	if(len == 0) return ;
	
    UINT keyNum = key.GetSize();	 
	
	for(DWORD i = 0; i<keyNum;  i++)
	{
		if(!strcmp(key[i].m_szStockCode, szStockCode))
		{
			key[i].m_dwIndex = nFrequency;
			break;
		}
	}
}

void CInputKey::GetKeyWantedByAlia(char *str, const char * Alias, int pos , unsigned int &uNum)
{
	if(strlen(str) == key[pos].m_nAlias)
	{
		if(strstr(str, Alias))
		{
			CKeyGhostStruct keyPoint;
			keyPoint = key[pos];
			strcpy(keyPoint.m_First,str);
		    keyWanted.SetAtGrow(uNum++, keyPoint);
		}
	}
}

void CInputKey::UpdateKeyElement(const char *code, const char *name, UINT type, UINT command,UINT uiMarket)
{
	CKeyGhostStruct keyPoint;
	memset(&keyPoint, 0, sizeof(keyPoint));
	strncpy(keyPoint.m_szStockCode, code, sizeof(keyPoint.m_szStockCode)-1);
	if(type != KEYGHOST_STOCK)
	{
		strupr(keyPoint.m_szStockCode);
	}

	strncpy(keyPoint.m_szStockName, name, sizeof(keyPoint.m_szStockName)-1);
	if(type != KEYGHOST_STOCK)
	{
		strupr(keyPoint.m_szStockName);
	}
	keyPoint.m_Type = (char)type;
	keyPoint.m_dwCommand = command;
	keyPoint.m_uiMarket = uiMarket;
	UpdateKeyElement(keyPoint);

}

void CInputKey::DeleteKeyElement(const char *code, WORD marketID)
{
	int len = strlen(code);
	if(len == 0)
		return ;
	
    UINT keyNum = key.GetSize();	 
	
	for(DWORD i = 0; i<keyNum;  i++)
	{
		if(!stricmp(key[i].m_szStockCode, code))
		{
			key.RemoveAt(i);
			break;
		}
	}
}

int CInputKey::GetSize()
{
	return key.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// By David sha [3/28/2002]
CString CInputKey::SearchStock(DWORD dwMarket, CString strStockCode)
{
	CString strStockName;
	if(strStockCode.IsEmpty()) return strStockName;
	
    UINT keyNum = key.GetSize();	 
	
	for(DWORD i = 0; i<keyNum;  i++)
	{
		if((strStockCode == key[i].m_szStockCode))
		{
			strStockName = key[i].m_szStockName;
			break;
		}
	}
	return strStockName;
}

void CInputKey::ReadCommand()
{
	int nCount,nTotalNum;
	CString szCmdFilePath;
	szCmdFilePath = CPublic::Instance()->GetSysPath() +_T("\\normalcmd.dat");	

	CFile cmdFile;

	struct SCodeList
	{
		SCodeList(){memset(this,0,sizeof(*this));};//初始化
		int iCodeType;//类型，指令或者代码
		char m_szCode[16];//代码
		char m_szName[24];//简称
	};

// 	SCodeList code[39];
// 	code[0].iCodeType = ID_SYSTEM_CONNECTTOHOST;
// 	strcpy(code[0].m_szCode,"LOGIN");
// 	strcpy(code[0].m_szName,"系统登录");
// 
// 	code[1].iCodeType = ID_SYSTEM_DISCONNECTHOST;
// 	strcpy(code[1].m_szCode,"LOGOUT");
// 	strcpy(code[1].m_szName,"系统注销");
// 
// 	code[2].iCodeType = ID_SYSTEM_SAVEASPIC;
// 	strcpy(code[2].m_szCode,"SAVEASPIC");
// 	strcpy(code[2].m_szName,"保存为图片");
// 
// 	code[3].iCodeType = ID_SYSTEM_AUTOUPDATE;
// 	strcpy(code[3].m_szCode,"AUTOUPDATE");
// 	strcpy(code[3].m_szName,"自动升级");
// 
// 	code[4].iCodeType = ID_APP_EXIT;
// 	strcpy(code[4].m_szCode,"EXIT");
// 	strcpy(code[4].m_szName,"系统退出");
// 
// 	code[5].iCodeType = ID_INTELLIG_FUNCTIONMANAGE;
// 	strcpy(code[5].m_szCode,"FORMULAMANAGE");
// 	strcpy(code[5].m_szName,"公式管理器");
// 
// 	code[6].iCodeType = ID_TECHANALY_1MINLINE;
// 	strcpy(code[6].m_szCode,"1MIN");
// 	strcpy(code[6].m_szName,"一分钟线");
// 
// 	code[7].iCodeType = ID_TECHANALY_5MINLINE;
// 	strcpy(code[7].m_szCode,"5MIN");
// 	strcpy(code[7].m_szName,"5分钟线");
// 
// 	code[8].iCodeType = ID_TECHANALY_15MINLINE;
// 	strcpy(code[8].m_szCode,"15MIN");
// 	strcpy(code[8].m_szName,"15分钟线");
// 
// 	code[9].iCodeType = ID_TECHANALY_30MINLINE;
// 	strcpy(code[9].m_szCode,"30MIN");
// 	strcpy(code[9].m_szName,"30分钟线");
// 
// 	code[10].iCodeType = ID_TECHANALY_60MINLINE;
// 	strcpy(code[10].m_szCode,"HOUR");
// 	strcpy(code[10].m_szName,"小时线");
// 
// 	code[11].iCodeType = ID_TECHANALY_DAYLINE;
// 	strcpy(code[11].m_szCode,"DAY");
// 	strcpy(code[11].m_szName,"日线");
// 
// 	code[12].iCodeType = ID_TECHANALY_WEEKLINE;
// 	strcpy(code[12].m_szCode,"WEEK");
// 	strcpy(code[12].m_szName,"周线");
// 
// 	code[13].iCodeType = ID_TECHANALY_MONLINE;
// 	strcpy(code[13].m_szCode,"MONTH");
// 	strcpy(code[13].m_szName,"月线");
// 
// 	code[14].iCodeType = ID_ANALYSIT_PICCOMMBIN_ONEPIC;
// 	strcpy(code[14].m_szCode,"1PIC");
// 	strcpy(code[14].m_szName,"一图组合");
// 
// 	code[5].iCodeType = ID_ANALYSIT_PICCOMMBIN_TWOPIC;
// 	strcpy(code[15].m_szCode,"2PIC");
// 	strcpy(code[15].m_szName,"二图组合");
// 
// 	code[16].iCodeType = ID_ANALYSIT_PICCOMMBIN_THREEPIC;
// 	strcpy(code[16].m_szCode,"3PIC");
// 	strcpy(code[16].m_szName,"三图组合");
// 
// 	code[17].iCodeType = ID_ANALYSIT_PICCOMMBIN_FOURPIC;
// 	strcpy(code[17].m_szCode,"4PIC");
// 	strcpy(code[17].m_szName,"四图组合");
// 
// 	code[18].iCodeType = ID_ANALYSIT_PICCOMMBIN_SIXPIC;
// 	strcpy(code[18].m_szCode,"6PIC");
// 	strcpy(code[18].m_szName,"六图组合");
// 
// 	code[19].iCodeType = ID_ANALYSIT_PICCOMMBIN_NINEPIC;
// 	strcpy(code[19].m_szCode,"9PIC");
// 	strcpy(code[19].m_szName,"九图组合");
// 
// 	code[20].iCodeType = ID_TOOL_DRAWTOOL;
// 	strcpy(code[20].m_szCode,"DRAWLINE");
// 	strcpy(code[20].m_szName,"画线工具");
// 
// 	code[21].iCodeType = ID_TECHANALY_ZOOMOUT;
// 	strcpy(code[21].m_szCode,"ZOOMOUT");
// 	strcpy(code[21].m_szName,"放大视图");
// 
// 	code[22].iCodeType = ID_TECHANALY_ZOOMIN;
// 	strcpy(code[22].m_szCode,"ZOOMIN");
// 	strcpy(code[22].m_szName,"缩小视图");
// 
// 	code[23].iCodeType = ID_TECHANALY_TOTAL;
// 	strcpy(code[23].m_szCode,"DISTSTAT");
// 	strcpy(code[23].m_szName,"区域统计");
// 
// 	code[24].iCodeType = ID_TOOL_FULLSCREEN;
// 	strcpy(code[24].m_szCode,"FULLSCREEN");
// 	strcpy(code[24].m_szName,"全屏显示");
// 
// 	code[25].iCodeType = ID_SYSTEM_NEWORDER;
// 	strcpy(code[25].m_szCode,"NEWORDER");
// 	strcpy(code[25].m_szName,"新订单");
// 
// 	code[26].iCodeType = ID_TRADE_OUTMONEY;
// 	strcpy(code[26].m_szCode,"OUTMONEY");
// 	strcpy(code[26].m_szName,"出金申请");
// 
// 	code[27].iCodeType = ID_TRADE_INMONEY;
// 	strcpy(code[27].m_szCode,"INMONEY");
// 	strcpy(code[27].m_szName,"入金申请");
// 
// 	code[28].iCodeType = ID_TRADE_HOLD;
// 	strcpy(code[28].m_szCode,"HOLD");
// 	strcpy(code[28].m_szName,"持仓单");
// 
// 	code[29].iCodeType = ID_TRADE_CLOSED;
// 	strcpy(code[29].m_szCode,"CLOSE");
// 	strcpy(code[29].m_szName,"平仓单");
// 
// 	code[30].iCodeType = ID_TRADE_LIMITE;
// 	strcpy(code[30].m_szCode,"LIMIT");
// 	strcpy(code[30].m_szName,"限价单");
// 
// 	code[31].iCodeType = ID_TRADE_CAPITAL;
// 	strcpy(code[31].m_szCode,"MONEYINFO");
// 	strcpy(code[31].m_szName,"资金信息");
// 
// 	code[32].iCodeType = ID_TRADE_PRODUCT;
// 	strcpy(code[32].m_szCode,"PRODUCTINFO");
// 	strcpy(code[32].m_szName,"商品信息");
// 
// 	code[33].iCodeType = ID_TRADE_MODIFYPWD;
// 	strcpy(code[33].m_szCode,"MODIFYPWD");
// 	strcpy(code[33].m_szName,"修改密码");
// 
// 	code[34].iCodeType = ID_TOOL_BOSSKEY;
// 	strcpy(code[34].m_szCode,"BOSSKEY");
// 	strcpy(code[34].m_szName,"老板键");
// 
// 	code[35].iCodeType = ID_TOOL_SYSTEMSETTING;
// 	strcpy(code[35].m_szCode,"SYSTEMSET");
// 	strcpy(code[35].m_szName,"系统设置");
// 
// 	code[36].iCodeType = ID_APP_ABOUT;
// 	strcpy(code[36].m_szCode,"HELP");
// 	strcpy(code[36].m_szName,"帮助信息");
// 
// 	code[37].iCodeType = ID_TECHANALY_NEXTSTK;
// 	strcpy(code[37].m_szCode,"NEXTPRODUCT");
// 	strcpy(code[37].m_szName,"下支产品");
// 
// 	code[38].iCodeType = ID_TECHANALY_PREVSTK;
// 	strcpy(code[38].m_szCode,"PREVPRODUCT");
// 	strcpy(code[38].m_szName,"上支产品");


	if(cmdFile.Open(szCmdFilePath,CFile::modeReadWrite|CFile::typeBinary))
	{//读取标准命令

//		cmdFile.Write(&code,sizeof(code));
// 
		SCodeList  arrCode[100];
		nTotalNum = cmdFile.GetLength()/sizeof(SCodeList);
		cmdFile.Read(arrCode,cmdFile.GetLength());

		for(nCount=0;nCount<nTotalNum;nCount++)
		{
			UpdateKeyElement(arrCode[nCount].m_szCode,arrCode[nCount].m_szName,KEYGHOST_COMMAND,arrCode[nCount].iCodeType);
		}
		cmdFile.Close();
	}
	//////////////////////////////////////////////////////////////////////////
	//读取公式命令
	int nCommand;

	nCommand = CFormulaFace::SizeAtSet(F_ZB);
	char szShort[11];
	for(int i = 0;i<nCommand;i++)
	{
		strcpy(szShort,CFormulaFace::FormuCode(i));
		UpdateKeyElement(_strupr(szShort),CFormulaFace::FormuName(i),KEYGHOST_FORMULA,0);
	}
	ReadFrequency();
}
void CInputKey::AddStockCode_ZYQ(CString strCode,CString strName)
{
	if (strCode.IsEmpty() || strName.IsEmpty())return;
	UpdateKeyElement(strCode,strName,KEYGHOST_STOCK,0,2);
}
void CInputKey::AddStockCode(const StuSymbolData *pStockInfo, int nNum,UINT uiMarket)
{
	//////////////////////////////////////////////////////////////////////////
	//增加新代码
	char szCode[20];
	char szName[20];
	int nCount;
	for(nCount = 0;nCount <nNum;nCount++)
	{
		strcpy(szCode,pStockInfo[nCount].sCode.szCode);//证券代码
		strcpy(szName,pStockInfo[nCount].szName);//证券名称
		UpdateKeyElement(szCode,szName,KEYGHOST_STOCK,0,uiMarket);
	}
}

void CInputKey::RemoveAll()
{
	key.RemoveAll();
	keyWanted.RemoveAll();
}
