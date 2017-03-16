/////////////////////////////////////////////////////////////////////////
//实现功能：定义程序中所需要的常量
//////////////////////////////////////////////////////////////////////////
#ifndef INC_CONST_H
#define INC_CONST_H
#include "quoteconst.h"

#define MENUCOLUMN_MAXNUM 18 //菜单列最大项数
// 右键菜单的装载索引
const  unsigned int POPSUBMENU_INDEX_PRICEVOLOBJ = 3;   // 分时走势
const  unsigned int POPSUBMENU_INDEX_HISTECHOBJ = 0;    //技术分析
const  unsigned int POPSUBMENU_INDEX_TECHINDEX = 1;     //技术指标
const  unsigned int POPSUBMENU_INDEX_REPORTTABLE = 2;   //报价（排行）表

const   unsigned char   PRICE_NORMAL = 0; // 报价

////////////////////////////////////////////////////////////////////////
//请求类型定义
const   unsigned short PT_NONE       =0x0001;//无子类
const unsigned int PRICEREPORT= 1;	//行情列表
const unsigned int PRICEDETAIL = 2; //行情信息



//个股分析视图子类型(左边Obj类型)
const   unsigned int RealObj          =0x0010;  //实时走势Obj
const   unsigned int TechObj          =0x0020;  //历史走势Obj
//RealObj子模块Pic类型
const   unsigned int PRICEVOLPIC      =0x0000;  //只有成交价、成交量图形

////////////////////////////////////////////////////////////////////////
//自定义
////////////////////////////////////////////////////////////////////////
const int INITVIEWINFO     = 0;//初始化视图信息
const int CURRENTSTOCKCODE = 2;//当前证券代码
const int INITBLKCONST     = 4;//板块名称和简称对照表



///////////////////////////////////////////////////////////////////////

const int QUOTELINKSUCCESS	 = 1;
const int QUOTELINKFAIL		 = 2;
const int QUOTELINKDOWN		 = 3;
const int QUOTELOGINSUCCESS  = 4;
const int QUOTELOGINFALIED   = 5;
const int QUOTEOTHERERROR	 = 6;
const int QUOTEOTHERSUCCESS  = 7;
const int TRADELINKSUCCESS	 = 8;
const int TRADELINKFAILED	 = 9;
const int TRADELINKDOWN		 = 10;
const int TRADELOGINSUCCESS  = 11;
const int TRADELOGINFAILED   = 12;



//////////////////////////////////////////////////////////////////////////
//请求索引序号
//////////////////////////////////////////////////////////////////////////
const int INDEX_MAX = 99999;   //业务请求最大值

const	BYTE	INVALIDF[8] = {0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
#endif