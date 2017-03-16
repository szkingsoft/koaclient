/////////////////////////////////////////////////////////////////////////////
// sysdef.h : header file
//
#ifndef __SYSDEF_H__
#define __SYSDEF_H__

/////////////////////////////////////////////////////////////////////////////
//**********颜色定义************
const int ID_SYSBACK_COLOR           =1; //系统背景色
const int ID_RISE_COLOR              =2; //涨色(阳线色，买盘色)
const int ID_EQUAL_COLOR             =3; //平色
const int ID_DOWN_COLOR              =4; //跌色(卖盘色)
const int ID_COORDTEXT_COLOR         =5; //坐标刻度，文字色
const int ID_COORDINATE_COLOR        =6; //坐标轴，框颜色
const int ID_INDEXLINE1_COLOR        =7; //指标线色(1)
const int ID_INDEXLINE2_COLOR        =8; //指标线色(2)
const int ID_INDEXLINE3_COLOR        =9; //指标线色(3)
const int ID_INDEXLINE4_COLOR        =10; //指标线色(4)
const int ID_INDEXLINE5_COLOR        =11; //指标线色(5)
const int ID_INDEXLINE6_COLOR        =12; //指标线色(6)
const int ID_INDEXLINE7_COLOR        =13; //指标线色(7)
const int ID_TOTALVOL_COLOR          =19; //成交量色，总手色
const int ID_TITLE_COLOR             =20; //大标题，标题色
const int ID_INFOTEXT_COLOR          =21; //资讯信息文本颜色
const int ID_GRIDLINE_COLOR          =22; //网格框线色
const int ID_HIGHLIGHT_COLOR         =23; //高亮背景颜色
const int ID_KLINEDOWN_COLOR         =24; //阴线色
const int ID_GRIDFIXED_COLOR         =25; //网格固定行/列文本色
const int ID_STOCK_COLOR             =26; //商品代码和名称颜色(网格中)
const int ID_OTHER_COLOR             =27; //其它颜色(总额色等)
const int ID_SORTARROR_COLOR         =28; //网格排序箭头色
const int ID_GRIDROWSEL_COLOR        =29; //网格行选中背景色
const int ID_PRICEBOARD_SINGLEBK	 =30;//报价板单个背景色
const int ID_PRICEBOARD_SELECTCOLOR  =31;//报价板选择色
const int ID_PRICEBOARD_BACKGROUNDCOLOR	=32;//报价板背景色
const int ID_PRICEBOARD_TEXTCOLOR	=33;//报价板文字色

const int ID_TIMELETTER_COLOR        =36; //系统时间色
const int ID_INFOLETTER_COLOR        =37; //状态栏文本色
const int ID_FORMTITLE_COLOR         =38; //K线指标标题色，统计信息表格标题色
const int ID_ColorNowHandLetter      =39; //公式抬头色
const int ID_STATCOLOR               =40; //现手色
const int ID_BUYVOLCOLOR             =41; //买量色
const int ID_SELLVOLCOLOR            =42; //卖量色
const int ID_INFOBACKGROUND_COLOR    =43; //资讯信息背景颜色
const int ID_CHANGE_COLOR			 =44; //变化色
const int ID_TRADESYMBOL_COLOR		 =45; //可交易商品底色 
const int ID_UNTRADESYMBOL_COLOR	 =46; //不可交易商品底色
const int ID_DRAWLINE_COLOR			 =47; //画线颜色
const int ID_TREND_TRACECOLOR		 =48; //未来价位线颜色
const int ID_TREND_COMECOLOR		 =49;//趋势跟踪进场色
const int ID_TREND_WATCHCLOLOR		 =50;//趋势跟踪观察色
const int ID_TREND_GOCOLOR			 =51;//趋势跟踪离场色
const int ID_FUTURE_DOTCOLOR		 =52;//价格虚线
const int ID_FUTURE_SOLIDCLOLOR      =53;//价格实线
const int ID_FUTURETREND_INFOTITLE   =54;//提示信息色

/////////////////////////////////////////////////////////////////////////////
//************字体定义**************
const int ID_TABLE_FONT              =1; //列表字体(报价表，统计信息表等)
const int ID_NUMBER_FONT             =2; //图形字体(K线图，分时图等)
const int ID_INFOTEXT_FONT           =3; //资讯字体(F10资料，信息地雷等)
	 //固定字体
const int ID_TITLE_FONT              =4; //标题字体
const int ID_STOCKTRACE_FONT         =5; //状态栏字体
const int ID_PROPORTION_FONT         =6; //走势涨跌比例字体
const int ID_FUTURETRENDTITLE_FONT   =7;//未来趋势价位抬头字体
const int ID_PRICEBOARD_PRICEBIGFONT =8; //报价版大字体
const int ID_PRICEBOARD_PRICEBOARDFONT =9; //报价版大字体
const int ID_HISTORYPICTITLEFONT	  =10;//分析抬头字体
const int ID_DLGINFOFONT			 =11;//对话框文字色
//////////////////////////////////////////////////////////////////////////
//**************字体风格定义****************
const int FONT_STYLE_BOLD			 =1;
const int FONT_STYLE_ITALIC		     =2;
const int FONT_STYLE_UNDERLINE		 =4;
const int FONT_STYLE_NORMAL			 =8;
#endif