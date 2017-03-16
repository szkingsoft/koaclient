///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_REPORTTABLE_409F3A55009B_INCLUDED
#define _INC_REPORTTABLE_409F3A55009B_INCLUDED

#include "BaseTable.h"

typedef struct tagReortItemInfo
{
	float fValue;              // m_ScrollBarRect列项值
	COLORREF crTextColor;      // 显示的字体颜色
	COLORREF crBackColor;      // 背景颜色
	CString strFormatValue;    // 值经过格式化，显示的文本内容

	tagReortItemInfo()
	{
		fValue = 0;
		crTextColor = -1;
		crBackColor = -1;
	}

}TReportItemInfo;

static int s_nSortColIndex = 1;

typedef struct tagCodeRowInfo
{
	int		nImage;
	SCodeInfo tCodeInfo;
	string   ssSymbolName;//商品名称
	vector<TReportItemInfo> vecColValue;

	tagCodeRowInfo()
	{
		nImage = -1;
		tCodeInfo.szCode[0] = '\0';
	}

	bool operator>(const tagCodeRowInfo& tRow2) const
	{
		bool bRet = false;
				
		switch( s_nSortColIndex) 
		{
		case 0:
			bRet = strcmp(this->tCodeInfo.szCode,tRow2.tCodeInfo.szCode) < 0;
			break;
		default:
			{
				int nValueColIndex = s_nSortColIndex - 2; 
				if( (nValueColIndex)  > (int)vecColValue.size()
					|| (nValueColIndex) > (int)tRow2.vecColValue.size())
				{
					return bRet;
				}
			
				bRet = this->vecColValue.at(nValueColIndex-1).fValue< tRow2.vecColValue.at(nValueColIndex-1).fValue;
			 }
			break;
		}
		
		return bRet;
	}
}TCodeRowInfo;

typedef struct 
{
	unsigned char ucColDefine;
	SColumnHead tColHeadInfo;
}TReportColInfo;


struct SUpdateData
{
	SNowData oNowData;
	int nUpDown;//1上涨，2下跌，3平价格
	string   ssSymbolName;//商品名称
	char	 cIsTradeSymbol;//是否交易商品
};

class CReportTable : public CBaseTable
{
// Construction / Destruction
public:	
	CReportTable();	
	~CReportTable();	

// Overrides
public:
	BOOL Init();

	BOOL UpdateData(LPVOID lpData,int nDataLen);

	BOOL OnUserCmd(unsigned int iSel);

	BOOL OnChar(UINT nChar);
	
	void Draw(CDC* pDC);

// Implementation
public:
	// 更新市场
	BOOL UpdateMarket(unsigned short usMarketType,CString strBlockName);

	// 获取当前选择行对应的证券信息
	SCodeInfo* GetCurSelCodeInfo();
	
	// 鼠标右键按下响应
	BOOL OnRButtonDown(CWnd *pParentWnd,UINT nFlags,CPoint point);

    // 鼠标左键按下响应
	BOOL OnLButtonDown(CWnd *pParentWnd,UINT nFlags,CPoint point);

	// 鼠标左键双击按下响应
	BOOL OnLButtonDblClk(CWnd *pParentWnd,UINT nFlags, CPoint point); 

	int	GetTotalCount();

	void ResetTableColor();

	int	HitReport(CPoint pt,int& nCol);

	virtual void SetCurrentSelLine(int nSel);

private:	

	//判断是否点击头，如果是返回当前点击位置，如果否，返回 -1
	int IsClickColumn(CPoint point);

	unsigned char GetColSortDefine(int nColOrderIndex);
	
	void UpdateReport(const vector<TCodeRowInfo> &vecCodeRowInfo);
	
	void UpdateReportItem(int nDisplayIndex,int nIndex,const TCodeRowInfo &tCurRow);
	
	//更新数据
	void UpdateNowData(const SUpdateData* pNow,tagCodeRowInfo &rtRowInfo,int nImage);

	//更新实时数据
	void UpdateRealData(const StuRecvQuoteInfo* pReal,SUpdateData &oUpdateData);
	
	

// Attributes
private:	
	vector<TCodeRowInfo> m_vectCodeRowInfo; //证券数据列表
	
	vector<SCodeInfo> m_vecCurrentCode;//当前代码

	vector<TReportColInfo> m_vecReportCol;
	
//	unsigned short m_usMarketType;
	
	int     m_nCurRequestBeginIndex; //本次请求的开始索引
	int     m_nCurRequestEndIndex;   //本次请求的结束索引
	int     m_nRealDataUpdateBeginIndex;
	
	int		m_nCurBeginIndex;      //当前开始位置

	int	    m_nRBClickHeadStation;//鼠标右键点击头位置

	bool	FindStation(SCodeInfo* pCode,int& nStation);
	
	BOOL    m_nItemStyle;          
	CString	m_strParamPath;//配置参数路径
	vector<int> m_vecShow;//显示列
//	int     m_nTradeTimeCount;     //从开盘到现在的交易时间

private:

	vector<SUpdateData>	m_vecNowData;
	vector<SUpdateData>	m_vecShowData;
	int					m_nTotalLine;
	void				TransData2Table();
public:
	void ShowHideOtherQuote(BOOL bShow);
	void ShowHighLowPrice(BOOL bShow);
	void ShowTime(BOOL bShow);
	void LoadIndexStatusParam();
	void SaveIndexStatusParam();
};

#endif /* _INC_REPORTTABLE_409F3A55009B_INCLUDED */
