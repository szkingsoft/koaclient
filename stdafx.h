
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����

#include <stdlib.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#define ULONG_PTR ULONG 
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include <math.h>
#pragma warning (disable : 4786) // ����ʹ��stl������warning
#include <map>
#include <vector>
#include <list>
using namespace std;
//yfj
//#include "quoteconst.h"
#include "mytoolkit.h"
#include "resource.h"
#include "CCustomUser.h"
#include "CPucHead.h"
#include "struct.h"		 //���ݽṹ
#include "public.h"      //�������� 
#include "const.h"		 //��������
#include "mymemdc.h"	 //�ڴ�DC
#include "message.h"     //������Ϣ����
#include "sysdef.h"      //ϵͳ��ɫ����
#include "..\\formula\\FormulaFace.h" //�Զ���ָ��ϵͳ
#include "InterfaceManager.h" //������ʾ���ù�����
#include "MultipleData.h"
#include "LanguageInterface.h"//���Խ���
//yfj
#include "DateManage.h"
#include "CSkinTransform.h"
#include "DateManage_ZYQ.h"
//#ifdef _DEBUG
//
//#include <vld.h>     // ����ڴ�й©
//#pragma comment( lib, "vld.lib" )
//
//#endif