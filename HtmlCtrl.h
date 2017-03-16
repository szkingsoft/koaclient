#ifndef CLASS_HTMLCTRL_H
#define CLASS_HTMLCTRL_H

#include <afxhtml.h>		// MFC support for Web browser

//
// Microsoft Systems Journal -- January 2000
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably Windows NT too.
//
class CHtmlCtrl : public CHtmlView 
{
public:
	
	CHtmlCtrl();
	~CHtmlCtrl();
	
	// Generated message map functions
	//{{AFX_MSG(CHtmlCtrl)
	// overrides to bypass MFC doc/view frame dependencies
	afx_msg void OnDestroy();
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd,UINT nHitTest,UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CHtmlCtrl)
	DECLARE_EVENTSINK_MAP()
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlCtrl)
public:
	void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	void PostNcDestroy() {};
	void OnAppCmd(LPCTSTR lpszWhere);
	void OnDocumentComplete(LPCTSTR lpszURL);
	void OnStatusTextChange(LPCTSTR lpszText);

	virtual void OnWindowClosing(BOOL IsChildWindow, BOOL *&Cancel);
	virtual void OnWindowSetResizable( BOOL Resizable);	
	
	//}}AFX_VIRTUAL

// Implementation
public:		
	BOOL CreateFromStatic(UINT nID, CWnd* pParent,CHtmlCtrl *pNewWindow2=NULL,CStatic *pOutputWnd = NULL);	

	bool SetNewContent(CString strXML, CString strXsltPath,bool bReformat = false);
	
	bool SetNewHTMLContent(CString csHTML, bool bReformat = false);
		
protected:	
    bool ParseHTMLString(CString &csCadenaATractar);

	bool SetHTMLInDocument();
	
	CString Trans2Html(CString strXMLData, CString strXsltPath);	
	
// Attributes
private:
	CString m_csHTMLContent;
	
	bool m_bHTMLDocumentLoadedOK;

    CHtmlCtrl *m_pNewWindow2;
	
	CStatic *m_pStaticOutput;	
};

#endif //CLASS_APXGATE_HTMLCTRL_H