/*==================================================================
=  文件名:CIniFile类实现文件                                       =
====================================================================*/
#include "stdafx.h"
#include "IniFile.h"




#ifdef _DEBUG                              //for debug builds only
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIniFile::CIniFile():m_bFileExsit(FALSE)
{
	m_bIsModify = FALSE;
}

CIniFile::~CIniFile()
{
	if(m_bFileExsit&&m_bIsModify)
	{
		if(m_stfFile.Open(m_strInIFileName,CFile::modeCreate | CFile::modeWrite) && FileContainer.GetSize() > 0)
		{
			CString strParam;
			for(int i = 0; i< FileContainer.GetSize();i++)
			{
				strParam = FileContainer[i];
			//	m_stfFile.WriteString(strParam);
				m_stfFile.WriteString(strParam+"\n");
			}
			m_stfFile.Close();
		}
	}
	if(FileContainer.GetSize() > 0)
	{
		FileContainer.RemoveAll();
	}
}
BOOL CIniFile::Create(const CString & strFileName,BOOL bCreateIfNotExist)
{
	m_bFileExsit = FALSE;
	m_strInIFileName = strFileName;
	FileContainer.RemoveAll();

	if(!m_stfFile.Open(strFileName,CFile::modeRead))
	{
		if( bCreateIfNotExist )
		{
			if(!m_stfFile.Open(strFileName,CFile::modeCreate))
			{
				return m_bFileExsit;
			}
			else
			{
				m_stfFile.Close();
				m_bFileExsit = TRUE;
				return m_bFileExsit;
			}
		}
		return m_bFileExsit;
	}

	CString strFileLine;
	while(m_bFileExsit = m_stfFile.ReadString(strFileLine))
	{
		if(m_bFileExsit == FALSE) return m_bFileExsit;
		strFileLine.Replace(_T("\""),_T(""));
		FileContainer.Add(strFileLine);
	}
	m_stfFile.Close();
	m_bFileExsit = TRUE;
	return m_bFileExsit;
}

BOOL CIniFile::CreateX(const CString &strFileName, BOOL bCreateNewFile /* = TRUE */)
{
	m_strInIFileName = strFileName;
	FileContainer.RemoveAll();

	if(bCreateNewFile) //创建新文件
	{
		if(!m_stfFile.Open(strFileName,CFile::modeCreate|CFile::typeText))
		{//创建失败
			m_bFileExsit = FALSE;
		}
		else
		{//创建成功
			m_bFileExsit = TRUE;
			m_stfFile.Close();
		}
	}
	else //文件存在，读取内容
	{
		if(!m_stfFile.Open(strFileName, CFile::modeRead))
		{ //文件打开失败则退出
			m_bFileExsit = FALSE;
			return FALSE;
		}

		CString strFileLine;
		while(m_stfFile.ReadString(strFileLine))
		{
			FileContainer.Add(strFileLine);
		}
		m_stfFile.Close();
		m_bFileExsit = TRUE;
	}
	return m_bFileExsit;
}

//获取配置的颜色 add by witming 
BOOL CIniFile::GetVarColor(const CString & strSection,const CString & strVarName,Gdiplus:: Color &crReturnValue)
{
	CString strtemp;
	int nA;
	int nR;
	int nG;
	int nB;

	if (!GetVar(strSection,strVarName,strtemp))
	{
		nA=255;
		nR=245;
		nG=245;
		nB=245;
		crReturnValue.SetValue(Gdiplus::Color::MakeARGB(nA,nR,nG,nB));
		return FALSE;
	}
	

	sscanf(strtemp,"%d %d %d %d",&nA,&nR,&nG,&nB);
	crReturnValue.SetValue(Gdiplus::Color::MakeARGB(nA,nR,nG,nB));

	return TRUE;
}

BOOL CIniFile::GetVar(const CString & strSection,const CString & strVarName,CString &strReturnValue)
{
	if(m_bFileExsit == FALSE || FileContainer.GetSize() < 0)
		return m_bFileExsit;
	
	int iLine = SearchLine(strSection,strVarName);
	if(iLine > 0)
	{
		CString	strParam = FileContainer[iLine-1];
	    strReturnValue = strParam.Mid(strParam.Find(_T("=")) + 1);
		return TRUE;
	}
	return FALSE;
}
BOOL CIniFile::GetVarStr(const CString & strSection,const CString & strVarName,CString &strReturnValue)
{
	return(GetVar(strSection,strVarName,strReturnValue));
}
BOOL CIniFile::GetVarInt(const CString & strSection,const CString & strVarName,int & iValue)
{
	CString  strReturnVar;
	if(GetVar(strSection,strVarName,strReturnVar))
	{
		strReturnVar.TrimLeft();
		int iLen = strReturnVar.GetLength();
		iValue = atoi((char*)(LPCTSTR)strReturnVar.GetBuffer(iLen));
		return TRUE;
	}
	return FALSE;
}
BOOL CIniFile::SetVar(const CString & strSection,const CString & strVarName,const CString & strVar,const int iType)
{
	if(m_bFileExsit == FALSE )
		return m_bFileExsit;

	if(!m_bIsModify)m_bIsModify = TRUE;
	if(FileContainer.GetSize() == 0)
	{
		FileContainer.Add(_T("[") + strSection + _T("]"));
		FileContainer.Add(strVarName + _T("=") + strVar);
		return TRUE;
	}

	int i = 0;
	int iFileLines = FileContainer.GetSize();
	CString strTag;
	//for(pInterator;pInterator != FileContainer.end();++pInterator)
	//{
	while(i< iFileLines)
	{
	
		CString		strValue = FileContainer.GetAt(i++);
		strValue.TrimLeft();
		if((strValue.Find(_T("[")) >=0) && (strValue.Find(strSection) >=0))
		{	
			while(i < iFileLines)
			{
			
				CString strSectionList = FileContainer[i++];
				strSectionList.TrimLeft();
				if(strSectionList.Find(_T("//")) >=0)//找到注释行
					continue;
				strTag = strVarName +_T("=");
				if(strSectionList.Find(strTag)>=0)//找到
				{
					CString strParam = strVarName + _T("=") + strVar;
					
					//FileContainer.SetAt(i-1,strParam);
					FileContainer[i-1] = strParam;

					return TRUE;
				}
				//在原来文件的SECTION中,没有相应的变量需要添加而且,这种情况下,下边还有别的section
				if(strSectionList.Find(_T("["),0)>=0)
				{
					//处理流程是这样的,首先把当前的数值依次向后移动,然后在当前位置加入新出现的数值
					if(iType !=0)
						return FALSE;
					CString strParam;
					FileContainer.Add(strParam);
					int iPre = FileContainer.GetSize()-1;
					while(iPre >= i)
					{
						CString strBehind = FileContainer[iPre -1];
						FileContainer[iPre] = strBehind;
						iPre --;

					}//*/
					strParam = strVarName + _T("=") + strVar;
					FileContainer.SetAt(i-1,strParam);
					return TRUE;
				}
				if(i == iFileLines && iType == 0)
				{
					FileContainer.Add(strVarName + _T("=") + strVar);
					return TRUE;
				}
			}
		}
	}
	if(iType == 0)
	{
		FileContainer.Add(_T("[") + strSection + _T("]"));
		FileContainer.Add(strVarName + _T("=") + strVar);
	}
	
	return TRUE;

}
BOOL CIniFile::SetVarStr(const CString & strSection,const CString & strVarName,const CString & strValue,const int iType)
{
	return SetVar(strSection,strVarName,strValue,iType);
}
BOOL CIniFile::SetVarInt(const CString & strSection,const CString & strVarName,const int & iValue,const int iType)
{
	CString strVar;
	strVar.Format(_T("%d"),iValue);
	return (SetVar(strSection,strVarName,strVar,iType));
}
int CIniFile::SearchLine(const CString & strSection,const CString & strVarName)
{
	if(FileContainer.GetSize() > 0)
	{
		int i = 0;
		int iFileLines = FileContainer.GetSize();
		CString strVarNameTag;
		strVarNameTag = strVarName+_T("=");
		while(i< iFileLines)
		{
			
			CString strValue = FileContainer[i++];
			strValue.TrimLeft();
			if(strValue.Find(_T("[")) >=0 && strValue.Find(strSection,1)>=0)
			{	
				while(i < iFileLines)
				{
				
					CString strSectionList = FileContainer[i++];
					strSectionList.TrimLeft();
					int nSignPos;
					nSignPos = strSectionList.Find(_T("//"));
//					if(nSignPos >=0)//找到注释行
					{
						if((nSignPos == 0)||((nSignPos > 0)&&(strSectionList[nSignPos-1] == ' ')))
						{//第一个字符位或前面一位为空
							continue;
						}
					}
					if(strSectionList.Find(strVarNameTag)>=0)//找到
					{
						return i;
					}
					if(strSectionList.Find(_T("["),0) >= 0)//另外一个段落出现,寻找失败
					{
						return -2;
					}
					
				}
			}
		}
	}
	return -1;
}

BOOL CIniFile::DeleteSection(const CString & strSection,BOOL bDelSectionName)
{
	int iSize; //文件大小
	int iRec;  //循环变量
	CString szItem;
	CString szSection;

	szSection = _T("[") + strSection + _T("]");
	iSize = FileContainer.GetSize();
	bool bFinded = false;
	for(iRec = 0; iRec < iSize && !bFinded; iRec++)
	{
		szItem = FileContainer.GetAt(iRec);
		szItem.TrimLeft();
		szItem.TrimRight();
		if(szItem == szSection)
			bFinded = true;
	}
	if(bFinded)
	{
		// 不删除键名，则将其略过
		if( !bDelSectionName)
			iRec++;
          
		while(iRec < iSize)
		{
			szItem = FileContainer.GetAt(iRec);
			if(szItem.Find(_T("["),0) < 0)
			{
				FileContainer.RemoveAt(iRec);
				iSize = FileContainer.GetSize();
			}
			else
			{
				break; //exit while
			}
		}
		iRec--;
		FileContainer.RemoveAt(iRec);
		return TRUE;
	}

	return FALSE;
}

BOOL CIniFile::DeleteValue(const CString & szSection,const CString & szVarName)
{
	if(m_bFileExsit == FALSE || FileContainer.GetSize() < 0)
		return m_bFileExsit;

	int iLine = SearchLine(szSection,szVarName);
	if(iLine > 0)
	{
		FileContainer.RemoveAt(iLine-1);
		return TRUE;
	}
	return FALSE;
}

BOOL CIniFile::DeleteAllSections()
{
	FileContainer.RemoveAll();
	return TRUE;
}

int CIniFile::CalcItemNums(const CString & szSection)
{
	int iSize; //文件大小
	int iRec;  //循环变量
	int iNums; //项数
	CString szItem;
	CString strSection;

	iNums = 0;
	strSection = _T("[") + szSection + _T("]");
	iSize = FileContainer.GetSize();
	bool bFinded = false;
	for(iRec = 0; iRec < iSize && !bFinded; iRec++)
	{
		szItem = FileContainer.GetAt(iRec);
		szItem.TrimLeft();
		szItem.TrimRight();
		if(szItem == strSection)
			bFinded = true;
	}
	if(bFinded)
	{
		while(iRec < iSize)
		{
			szItem = FileContainer.GetAt(iRec);
			if(!szItem.IsEmpty() && szItem.Find(_T("["),0) < 0)
			{
				iNums++;
				iRec++;
			}
			else
			{
				break; //exit while
			}
		}
	}
	return iNums;
}

BOOL CIniFile::AduItemsSerial(const CString & szSection,const CString & szVarFirst)
{
	int iVarFirst;
	int iVarLast;
	CString szItem;
	CString szValue;
	CStringArray arrValue;

	iVarFirst = atoi((char*)(LPCTSTR)szVarFirst);
	iVarLast = CalcItemNums(szSection);
	iVarLast = iVarLast+1;
	arrValue.SetSize(0,100);
	int iRec;
	for(iRec = iVarFirst; iRec <= iVarLast; iRec++)
	{
		szItem.Empty();
		szItem.Format(_T("%03d"),iRec);
		GetVarStr(szSection,szItem,szValue);
		arrValue.Add(szValue);
		DeleteValue(szSection,szItem);
	}
	iVarFirst = CalcItemNums(szSection);
	iVarFirst++;
	for(iRec = 0; iRec < arrValue.GetSize(); iRec++)
	{
		szItem.Empty();
		szItem.Format(_T("%03d"),iVarFirst+iRec);
		SetVarStr(szSection,szItem,arrValue[iRec],0);
	}
	return TRUE;
}

//获取章节数
BOOL  CIniFile::GetSectionList(vector<CString> &rvecSection)
{
	rvecSection.clear();
	
	if(m_bFileExsit == FALSE )
		return FALSE;

	int iSize = FileContainer.GetSize(); //文件大小
	CString strItem;
	CString strSection;
	
	for(int i = 0; i < iSize; i++)
	{
		strItem = FileContainer.GetAt(i);
		strItem.TrimLeft();
		strItem.TrimRight();
		if(strItem.Find(_T("["),0) == 0)
		{
			int nLen = strItem.GetLength();
			strItem = strItem.Mid(1,nLen -2);						
			rvecSection.push_back(strItem);
		}
	}

	return TRUE;
}

BOOL CIniFile::GetSection(const CString& szSection,map<CString,CString>& mapVar)
{
	int iSize; //文件大小
	int iRec;  //循环变量
	CString szItem;
	CString strSection;

	strSection = _T("[") + szSection + _T("]");
	iSize = FileContainer.GetSize();
	bool bFinded = false;
	for(iRec = 0; iRec < iSize && !bFinded; iRec++)
	{
		szItem = FileContainer.GetAt(iRec);
		szItem.TrimLeft();
		szItem.TrimRight();
		if(szItem == strSection)
			bFinded = true;
	}
	if(!bFinded)return FALSE;
	CString strKey,strValue;
	int nPos;
	while(iRec < iSize)
	{
		szItem = FileContainer.GetAt(iRec);
		if(!szItem.IsEmpty() && szItem.Find(_T("["),0) != 0)
		{	
			iRec++;
			nPos = szItem.Find(_T("="));
			if(nPos== -1)continue;
			strKey = szItem.Left(nPos);
			szItem.Delete(0,nPos+1);
			strValue= szItem;
			mapVar[strKey]= strValue;

		}
		else
		{
			break; //exit while
		}
	}
	return TRUE;
}