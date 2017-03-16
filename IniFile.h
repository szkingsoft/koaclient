/*==================================================================
=  文件名:CIniFile类定义文件                                       =  
=  主要功能:可以读取.修改变量数值,可以设置新的组,新的变量          =
====================================================================*/
#ifndef _CINIFILE_H_
#define _CINIFILE_H_
#include <map>
using namespace std;
class CIniFile
{
public:
	CIniFile();
	virtual ~CIniFile();
public:
	//创建函数
	BOOL Create(const CString &strFileName,BOOL bCreateIfNotExist = FALSE);
	//创建函数
	BOOL CreateX(const CString &strFileName, BOOL bCreateNewFile = TRUE);
	//得到变量整数型数值
	BOOL GetVarInt(const CString &,const CString & ,int &);
	//得到变量字符串型数值
	BOOL GetVarStr(const CString &,const CString & ,CString & );
	//重新设置变量整数型数值
	BOOL SetVarInt(const CString &,const CString & ,const int &,const int iType = 1);
	//重新设置变量字符串型数值
	BOOL SetVarStr(const CString &,const CString &, const CString &,const int iType = 1);

	//删除键下的所有项,bDelKey是否删除键名
	BOOL DeleteSection(const CString &,BOOL bDelSectionName=TRUE);

	//删除所有项(即清空INI文件)
	BOOL DeleteAllSections(void);
	//删除键下的某一项
	BOOL DeleteValue(const CString &,const CString &);
	//计算键下的项数
	int CalcItemNums(const CString &);
	//调整各项序号
	BOOL AduItemsSerial(const CString &,const CString &);
	
	BOOL GetSectionList(vector<CString> &rvecSection);     //获取章节列表

	//得到某节下所有数据
	BOOL GetSection(const CString& szSection,map<CString,CString>& mapVar);

	//得到设置的颜色  add by witming
	BOOL GetVarColor(const CString & strSection,const CString & strVarName, Gdiplus::Color &crReturnValue);

private:
	BOOL GetVar(const CString &,const CString &,CString &);
	BOOL SetVar(const CString &,const CString &,const CString &,const int iType = 1);
	int  SearchLine(const CString &,const CString &);

private:
//	vector <CString>  FileContainer;
	CArray <CString,CString> FileContainer; //INI文件容器
	BOOL  m_bFileExsit;
	CStdioFile m_stfFile;
	CString m_strInIFileName;
	BOOL	m_bIsModify;//是否修改
};
#endif