// DistractStatTable.h: interface for the CDistractStatTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRACTSTATTABLE_H__882D2B51_1B41_41D4_A054_2400F493A3AF__INCLUDED_)
#define AFX_DISTRACTSTATTABLE_H__882D2B51_1B41_41D4_A054_2400F493A3AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseTable.h"

class CDistractStatTable : public CBaseTable  
{
public:
	virtual void MoveTo(CRect rRect);
	void SetStockInfo(const SCodeInfo* pCodeInfo);
	virtual void Draw(CDC *pDC);
	virtual BOOL UpdateData(LPVOID lpData,int nDataLen);
	virtual BOOL Init();
	CDistractStatTable();
	virtual ~CDistractStatTable();

protected:
	virtual void SetColorsAndFonts();
	const SCodeInfo* m_pCodeInfo;
	int m_nOpenDate; //开始时间
	int m_nCloseDate;//结束时间
	int m_nTotalNum; //总天数
	CString m_szFormat;//价格格式化字串
};

#endif // !defined(AFX_DISTRACTSTATTABLE_H__882D2B51_1B41_41D4_A054_2400F493A3AF__INCLUDED_)
