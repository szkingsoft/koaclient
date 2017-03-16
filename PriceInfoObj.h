#pragma once
//信息报价模块
#include "basedrawobj.h"
#include "BaseInfoItem.h"

#define  WM_UPDATECTRL    WM_USER + 7000
class CPriceInfoObj :public CBaseDrawObj
{
public:
	CPriceInfoObj(void);
	~CPriceInfoObj(void);


	//////////////////////////////////////////////////////////
	//计算子区域大小
	virtual void RecalSubObjSize(int iRightW = 50);
	////////////yfj
	void CalculateItemRect();
	UINT GetHight() {return m_iLastY;}
	void SetNotifyWnd(HWND hWnd) {m_hNotifyWnd = hWnd;};
	UINT m_iLastY;
	HWND m_hNotifyWnd;
	/////////////////
	//////////////////////////////////////////////////////////
	//设置画图区域
	virtual void MoveTo(CRect rRect);

	//////////////////////////////////////////////////////////
	//传递数据
	virtual BOOL UpdateData(LPVOID pData, int nDataSize);

	//测试点击位置
	virtual int HitTest(CPoint point);

	int	HitRect(CPoint point,int& nBuySellSign);

	BOOL OnLButtonDblClk(CWnd* pWnd,UINT uiFlag,CPoint point);

	int GetCurrentSelLine();



	void  SetCurrentSelLine(int nSel);
	SCodeInfo* GetCurSelCodeInfo();

	//////////////////////////////////////////////////////////
	//画图
	virtual BOOL Draw(CDC* pDC);
	void SetHoriBeginLine(int nBeginLine){m_nBeginLine = nBeginLine;};
	virtual BOOL OnChar(int nChar);


	// 鼠标右键按下响应
	virtual BOOL OnRButtonDown(UINT nFlags,CPoint point);

	// 鼠标左键按下响应
	virtual BOOL OnLButtonDown(UINT nFlags,CPoint point);

	// 鼠标左键双击按下响应
	virtual BOOL OnLButtonDblClk(UINT nFlags, CPoint point); 

	virtual BOOL Init(int nNum);

	void GetTotalLine(int& nTotalNum,int& nDisplayNum){nTotalNum = m_nTotalLine;nDisplayNum = m_nDisplayRow;};
private:
	CImageList		m_ImageList;
	vector<CBaseInfoItem> m_vecInfoItem;
	CRect m_TotalRect;
	int		m_nBeginLine;	//当前开始显示行数
	int		m_nTotalLine;	//当前总行数
	int		m_nDisplayRow;	//当前屏幕可显示行数
	int		m_nDisplayCol;  //当前屏幕显示列数

	int m_nCurrentSel;//当前选中行

	float m_nRowHeight; //单元格高度(行高)
public:
	void ShowHideOtherQuote(bool bShow);
	void LoadIndexStatusParam();
};

