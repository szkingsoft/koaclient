
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类

#include <stdlib.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

#define ULONG_PTR ULONG 
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include <math.h>
#pragma warning (disable : 4786) // 屏蔽使用stl带来的warning
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
#include "struct.h"		 //数据结构
#include "public.h"      //公用数据 
#include "const.h"		 //常量定义
#include "mymemdc.h"	 //内存DC
#include "message.h"     //传输消息定义
#include "sysdef.h"      //系统配色方案
#include "..\\formula\\FormulaFace.h" //自定义指标系统
#include "InterfaceManager.h" //界面显示设置管理器
#include "MultipleData.h"
#include "LanguageInterface.h"//语言界面
//yfj
#include "DateManage.h"
#include "CSkinTransform.h"
#include "DateManage_ZYQ.h"
//#ifdef _DEBUG
//
//#include <vld.h>     // 检测内存泄漏
//#pragma comment( lib, "vld.lib" )
//
//#endif