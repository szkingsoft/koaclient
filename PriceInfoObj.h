#pragma once
//��Ϣ����ģ��
#include "basedrawobj.h"
#include "BaseInfoItem.h"

#define  WM_UPDATECTRL    WM_USER + 7000
class CPriceInfoObj :public CBaseDrawObj
{
public:
	CPriceInfoObj(void);
	~CPriceInfoObj(void);


	//////////////////////////////////////////////////////////
	//�����������С
	virtual void RecalSubObjSize(int iRightW = 50);
	////////////yfj
	void CalculateItemRect();
	UINT GetHight() {return m_iLastY;}
	void SetNotifyWnd(HWND hWnd) {m_hNotifyWnd = hWnd;};
	UINT m_iLastY;
	HWND m_hNotifyWnd;
	/////////////////
	//////////////////////////////////////////////////////////
	//���û�ͼ����
	virtual void MoveTo(CRect rRect);

	//////////////////////////////////////////////////////////
	//��������
	virtual BOOL UpdateData(LPVOID pData, int nDataSize);

	//���Ե��λ��
	virtual int HitTest(CPoint point);

	int	HitRect(CPoint point,int& nBuySellSign);

	BOOL OnLButtonDblClk(CWnd* pWnd,UINT uiFlag,CPoint point);

	int GetCurrentSelLine();



	void  SetCurrentSelLine(int nSel);
	SCodeInfo* GetCurSelCodeInfo();

	//////////////////////////////////////////////////////////
	//��ͼ
	virtual BOOL Draw(CDC* pDC);
	void SetHoriBeginLine(int nBeginLine){m_nBeginLine = nBeginLine;};
	virtual BOOL OnChar(int nChar);


	// ����Ҽ�������Ӧ
	virtual BOOL OnRButtonDown(UINT nFlags,CPoint point);

	// ������������Ӧ
	virtual BOOL OnLButtonDown(UINT nFlags,CPoint point);

	// ������˫��������Ӧ
	virtual BOOL OnLButtonDblClk(UINT nFlags, CPoint point); 

	virtual BOOL Init(int nNum);

	void GetTotalLine(int& nTotalNum,int& nDisplayNum){nTotalNum = m_nTotalLine;nDisplayNum = m_nDisplayRow;};
private:
	CImageList		m_ImageList;
	vector<CBaseInfoItem> m_vecInfoItem;
	CRect m_TotalRect;
	int		m_nBeginLine;	//��ǰ��ʼ��ʾ����
	int		m_nTotalLine;	//��ǰ������
	int		m_nDisplayRow;	//��ǰ��Ļ����ʾ����
	int		m_nDisplayCol;  //��ǰ��Ļ��ʾ����

	int m_nCurrentSel;//��ǰѡ����

	float m_nRowHeight; //��Ԫ��߶�(�и�)
public:
	void ShowHideOtherQuote(bool bShow);
	void LoadIndexStatusParam();
};

