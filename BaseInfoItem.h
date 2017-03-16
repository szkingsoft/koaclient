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
	//�������ܣ����û�ͼ����
	//����������rRect ��ͼ����
	//��������:��
	//�����쳣:��
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
	BOOL	m_bSelectStatus;//ѡ��״̬
	int	  m_nUporDown;//��ͣ��־
	CRect m_TotalRect; //������

	CRect m_TitleRect; //̧ͷ����
// 	CRect m_PriceRect;	//�����۸�����
// 	CRect m_MainRect;  //�۸��������
//	CRect m_MaxMinRect;//�����С�۸�����
	CRect m_MaxRect;//��߼�����
	CRect m_MinRect;//��ͼ�����

	CRect m_BuyRect;	//�������
	CRect m_SellRect;	//��������

};

