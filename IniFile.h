/*==================================================================
=  �ļ���:CIniFile�ඨ���ļ�                                       =  
=  ��Ҫ����:���Զ�ȡ.�޸ı�����ֵ,���������µ���,�µı���          =
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
	//��������
	BOOL Create(const CString &strFileName,BOOL bCreateIfNotExist = FALSE);
	//��������
	BOOL CreateX(const CString &strFileName, BOOL bCreateNewFile = TRUE);
	//�õ�������������ֵ
	BOOL GetVarInt(const CString &,const CString & ,int &);
	//�õ������ַ�������ֵ
	BOOL GetVarStr(const CString &,const CString & ,CString & );
	//�������ñ�����������ֵ
	BOOL SetVarInt(const CString &,const CString & ,const int &,const int iType = 1);
	//�������ñ����ַ�������ֵ
	BOOL SetVarStr(const CString &,const CString &, const CString &,const int iType = 1);

	//ɾ�����µ�������,bDelKey�Ƿ�ɾ������
	BOOL DeleteSection(const CString &,BOOL bDelSectionName=TRUE);

	//ɾ��������(�����INI�ļ�)
	BOOL DeleteAllSections(void);
	//ɾ�����µ�ĳһ��
	BOOL DeleteValue(const CString &,const CString &);
	//������µ�����
	int CalcItemNums(const CString &);
	//�����������
	BOOL AduItemsSerial(const CString &,const CString &);
	
	BOOL GetSectionList(vector<CString> &rvecSection);     //��ȡ�½��б�

	//�õ�ĳ������������
	BOOL GetSection(const CString& szSection,map<CString,CString>& mapVar);

	//�õ����õ���ɫ  add by witming
	BOOL GetVarColor(const CString & strSection,const CString & strVarName, Gdiplus::Color &crReturnValue);

private:
	BOOL GetVar(const CString &,const CString &,CString &);
	BOOL SetVar(const CString &,const CString &,const CString &,const int iType = 1);
	int  SearchLine(const CString &,const CString &);

private:
//	vector <CString>  FileContainer;
	CArray <CString,CString> FileContainer; //INI�ļ�����
	BOOL  m_bFileExsit;
	CStdioFile m_stfFile;
	CString m_strInIFileName;
	BOOL	m_bIsModify;//�Ƿ��޸�
};
#endif