#ifndef ____OUTPUTDEFINE___H_20040427_ANN___
#define ____OUTPUTDEFINE___H_20040427_ANN___

//输出属性宏
#define ENTERLONG 265
#define EXITLONG 266
#define ENTERSHORT 267
#define EXITSHORT 268
#define RETURN 269
#define COLORRED 270
#define COLORBLUE 271
#define COLORGREEN 272
#define COLORCYAN 273
#define COLORBLACK 274
#define COLORMAGENTA 275
#define COLORBROWN 276
#define COLORLIGHTGRAY 277
#define COLORDARKGRAY 278
#define COLORLIGHTBLUE 279
#define COLORLIGHTGREEN 280
#define COLORLIGHTCYAN 281
#define COLORLIGHTRED 282
#define COLORLIGHTMAGENTA 283
#define COLORYELLOW 284
#define COLORWHITE 285
#define DEFAULTKCOLOR 286
#define DEFAULT 287
#define VOLSTICK 288
#define COLORSTICK 289
#define CIRCLEDOT 290
#define NODRAW 291

enum	FORMU_TYPE
{
	F_ZB = 0,	//指标类型公式
	F_TJ = 1,	//条件选股类型公式
	F_JY = 2,	//交易系统公式
	F_WC = 3,	//五彩K线公式
	F_FC = 4,	//自定义函数公式
};

enum	DATA_TYPE
{
	    K_DATA,		//得到K线数据
		M_DATA,		//得到分钟数据
		HQ_DATA,	//得到行情数据
		FN_DATA,	//得到财务数据
};

typedef	float 	FPERIOD[12];
//注册原生函数的通用指针类型
typedef void ( *FP_FUNC)();
//回调函数:读取股票数据的
typedef long(CALLBACK * FP_DATAIO)(SCodeInfo stk,
								   DATA_TYPE nDType,void * pData,short nDataNum);
#endif