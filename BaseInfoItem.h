#pragma once
class CBaseInfoItem
{
public:
	CBaseInfoItem(void);
	~CBaseInfoItem(void);
	virtual void SetImageList(CImageList* pImageList);
	virtual BOOL SetStockInfo(SCodeInfo* pCodeInfo,string ssSymbolName);
	virtual BOOL Draw(CDC* pDC);
	virtual BOOL SetQuote(SNowData* pNow);
	//////////////////////////////////////////////////////////////////////////
	//函数功能：设置画图区间
	//函数参数：rRect 画图区间
	//函数返回:无
	//函数异常:无
	virtual void MoveTo(CRect rRect);	
	void GetRect(CRect& rt);
	BOOL HitTest(CPoint pt);
	inline SCodeInfo* GetStockInfo(){return &m_oCodeInfo;};
	inline SNowData* GetNowData(){return &m_NowData;};
	static void GetItemWidthHeight(int& nWidth,int& nHeight);

	void SetSelectStatus(BOOL bSel);
	int HistRect(CPoint pt);	
	CString GetSymbolName(){return m_ssSymbolName.c_str();}
private:
	CImageList* m_pImageList;
	SCodeInfo m_oCodeInfo;
	string    m_ssSymbolName;
	SNowData m_NowData;
	BOOL	m_bSelectStatus;//选择状态
	int	  m_nUporDown;//涨停标志
	CRect m_TotalRect; //总区域

	CRect m_TitleRect; //抬头区域
// 	CRect m_PriceRect;	//买卖价格区域
// 	CRect m_MainRect;  //价格大字区域
//	CRect m_MaxMinRect;//最大最小价格区域
	CRect m_MaxRect;//最高价区域
	CRect m_MinRect;//最低价区域

	CRect m_BuyRect;	//买价区域
	CRect m_SellRect;	//卖价区域

};

