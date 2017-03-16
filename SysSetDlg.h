#ifndef CLASS_SYSSET_DLG_H
#define CLASS_SYSSET_DLG_H

#include "ColorDefPic.h"
#include "XPButton.h"
class CSysSetDlg : public CPropertyPage
{
	//DECLARE_DYNCREATE(CSysSetDlg)
	
// Construction / Destruction
public:	
	CSysSetDlg();	
	~CSysSetDlg();	
	
// Overrides
protected:	
	BOOL OnSetActive();
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSetDlg)
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	//}}AFX_VIRTUAL
	
// Messages
protected:
	// Generated message map functions
	//{{AFX_MSG(CSysSetDlg)
	afx_msg void OnCmbSelchangeStyle();
	
	afx_msg void OnListSelchangeColorName();
	afx_msg void OnUpdateR();
	afx_msg void OnUpdateG();
	afx_msg void OnUpdateB();
	afx_msg void OnBtnSelColor();
	afx_msg void OnBtnDefaultColor();
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnBtnSelFont();
	afx_msg void OnBtnDefaultFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
// Implementation
public:
	BOOL IsSetActived();//是否被激活

	BOOL SaveInterfaceParam();
	
private:	
	
	void UpdateCurItemTempSysColor(COLORREF crDef);//设置颜色
	
	void DisplayCurColor(); // 更新当前色的显示
	
// Attributes
	// Dialog Data
	//{{AFX_DATA(CSysSetDlg)
    public:
	enum { IDD = IDD_APPERENCE };
    protected:
	CComboBox	m_cmbSysStyle;
	
	CListBox	m_lbSysColorName;
	CXPButton	m_btnCurColor;
	CSliderCtrl m_slidR; 
	CSliderCtrl m_slidG; 
	CSliderCtrl m_slidB; 
	int         m_nR;
	int         m_nG;
	int         m_nB;	
	
	CComboBox  m_cmbSysFontName;
	
	CColorDefPic m_DrawPic; //图形控件
	//}}AFX_DATA
	
private:	
	BOOL m_bActive;//激活标志	
public:
//	afx_msg void OnBnClickedButtonCurcolor();
};
#endif // CLASS_SYSSET_DLG_H

