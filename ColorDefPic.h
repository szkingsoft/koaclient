#if !defined(AFX_COLORDEFPIC_H__BF86AB23_CC1D_4C6F_A1B5_C16DE68B2859__INCLUDED_)
#define AFX_COLORDEFPIC_H__BF86AB23_CC1D_4C6F_A1B5_C16DE68B2859__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorDefPic.h : header file
//////////////////////////////////////////
//类名：    颜色设置-方案预览图元
//创建日期：2004-7-6
//版本：    V4.0.0.1
//作者：    zhongweilong
//////////////////////////////////////////
class CColorDefPic : public CStatic
{
// Construction
public:
	CColorDefPic();	
	virtual ~CColorDefPic();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorDefPic)
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	//{{AFX_MSG(CColorDefPic)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void ResetTempSysColor();

	BOOL SaveTemp2SysColor();

	void UpdateTempSysColor(const TColorDefInfo& tTempColor);

	COLORREF GetTempSysColor(UINT uiID);

	void ResetTempSysFont();
	
	BOOL SaveTemp2SysFont();
	
	void UpdateTempSysFont(const TFontDefInfo& tTempFont);
	
	TFontDefInfo& GetTempSysFont(UINT uiID);

private:

	void FreeTempSysFont();

// Attribute
private:
	vector<TColorDefInfo> m_vecTempSysColor;
	vector<TFontDefInfo> m_vecTempSysFont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDEFPIC_H__BF86AB23_CC1D_4C6F_A1B5_C16DE68B2859__INCLUDED_)
