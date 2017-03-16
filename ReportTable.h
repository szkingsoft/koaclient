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
	float fValue;              // m_ScrollBarRect����ֵ
	COLORREF crTextColor;      // ��ʾ��������ɫ
	COLORREF crBackColor;      // ������ɫ
	CString strFormatValue;    // ֵ������ʽ������ʾ���ı�����

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
	string   ssSymbolName;//��Ʒ����
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
	int nUpDown;//1���ǣ�2�µ���3ƽ�۸�
	string   ssSymbolName;//��Ʒ����
	char	 cIsTradeSymbol;//�Ƿ�����Ʒ
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
	// �����г�
	BOOL UpdateMarket(unsigned short usMarketType,CString strBlockName);

	// ��ȡ��ǰѡ���ж�Ӧ��֤ȯ��Ϣ
	SCodeInfo* GetCurSelCodeInfo();
	
	// ����Ҽ�������Ӧ
	BOOL OnRButtonDown(CWnd *pParentWnd,UINT nFlags,CPoint point);

    // ������������Ӧ
	BOOL OnLButtonDown(CWnd *pParentWnd,UINT nFlags,CPoint point);

	// ������˫��������Ӧ
	BOOL OnLButtonDblClk(CWnd *pParentWnd,UINT nFlags, CPoint point); 

	int	GetTotalCount();

	void ResetTableColor();

	int	HitReport(CPoint pt,int& nCol);

	virtual void SetCurrentSelLine(int nSel);

private:	

	//�ж��Ƿ���ͷ������Ƿ��ص�ǰ���λ�ã�����񣬷��� -1
	int IsClickColumn(CPoint point);

	unsigned char GetColSortDefine(int nColOrderIndex);
	
	void UpdateReport(const vector<TCodeRowInfo> &vecCodeRowInfo);
	
	void UpdateReportItem(int nDisplayIndex,int nIndex,const TCodeRowInfo &tCurRow);
	
	//��������
	void UpdateNowData(const SUpdateData* pNow,tagCodeRowInfo &rtRowInfo,int nImage);

	//����ʵʱ����
	void UpdateRealData(const StuRecvQuoteInfo* pReal,SUpdateData &oUpdateData);
	
	

// Attributes
private:	
	vector<TCodeRowInfo> m_vectCodeRowInfo; //֤ȯ�����б�
	
	vector<SCodeInfo> m_vecCurrentCode;//��ǰ����

	vector<TReportColInfo> m_vecReportCol;
	
//	unsigned short m_usMarketType;
	
	int     m_nCurRequestBeginIndex; //��������Ŀ�ʼ����
	int     m_nCurRequestEndIndex;   //��������Ľ�������
	int     m_nRealDataUpdateBeginIndex;
	
	int		m_nCurBeginIndex;      //��ǰ��ʼλ��

	int	    m_nRBClickHeadStation;//����Ҽ����ͷλ��

	bool	FindStation(SCodeInfo* pCode,int& nStation);
	
	BOOL    m_nItemStyle;          
	CString	m_strParamPath;//���ò���·��
	vector<int> m_vecShow;//��ʾ��
//	int     m_nTradeTimeCount;     //�ӿ��̵����ڵĽ���ʱ��

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
