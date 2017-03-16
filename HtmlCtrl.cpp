//
// Microsoft Systems Journal -- January 2000
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably Windows NT too.
//
#include "StdAfx.h"
#include "HtmlCtrl.h"								//Web browser control
#include "AtlBase.h"
#include <comdef.h>
#include <Mshtml.h>
#include "resource.h"

#import <msxml3.dll> named_guids
using namespace MSXML2;

#import <mshtml.tlb>
//using namespace MSHTML;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////
// Construction...
CHtmlCtrl::CHtmlCtrl()
{
	m_csHTMLContent = "";
	m_bHTMLDocumentLoadedOK = FALSE;

	m_pNewWindow2 = NULL;	

	m_pStaticOutput = NULL;
}

//////////////////
// Destruction...
CHtmlCtrl::~CHtmlCtrl()
{
	this->m_csHTMLContent.ReleaseBuffer();
}


IMPLEMENT_DYNAMIC(CHtmlCtrl, CHtmlView)
BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
  ON_WM_DESTROY()
  ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CHtmlCtrl, CHtmlView)
ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, 263 /* WindowClosing */, OnWindowClosing, VTS_BOOL VTS_BOOL)
ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, 262 /* WindowSetResizable */, OnWindowSetResizable, VTS_BOOL)
END_EVENTSINK_MAP()


//////////////////
// Create control in same position as an existing static control with
// the same ID (could be any kind of control, really)
//
BOOL CHtmlCtrl::CreateFromStatic(UINT nID, CWnd* pParent,CHtmlCtrl *pNewWindow2,CStatic *pOutputWnd)
{
	BOOL BRetorn = FALSE;
	
	CStatic wndStatic;
	CRect rc;
	
	DWORD dwStyle = 0;
	
	if (!wndStatic.SubclassDlgItem(nID, pParent)) return FALSE;
	
	dwStyle = wndStatic.GetStyle();
	
	// Get static control rect, convert to parent's client coords.
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	
	wndStatic.DestroyWindow();
	
	// create HTML control (CHtmlView)
	BRetorn = this->Create(NULL,											// class name
		NULL,											// title
		dwStyle,										// style
		rc,                        // rectangle
		pParent,                   // parent
		nID,                       // control ID
		NULL);                     // frame/doc context not used

	BOOL bRet = LoadFromResource( IDR_HTML);

	this->SetHTMLInDocument();

	m_bHTMLDocumentLoadedOK = TRUE;
		
	//设置打开新窗口的地方
	if( pNewWindow2 == NULL)
	{
		m_pNewWindow2 = this;
	}
	else
	{
		m_pNewWindow2 = pNewWindow2;
	}		

	m_pStaticOutput = pOutputWnd;
	
	return BRetorn;
}

////////////////
// Override to avoid CView stuff that assumes a frame.
//
void CHtmlCtrl::OnDestroy()
{
	MSHTML::IHTMLDocument2Ptr pHTMLDoc2 = NULL;
	
	pHTMLDoc2 = this->GetHtmlDocument();
	
	if (pHTMLDoc2)
	{
		pHTMLDoc2->Release();
	}
	
	// This is probably unecessary since ~CHtmlView does it, but
	// safer to mimic CHtmlView::OnDestroy.
	if (m_pBrowserApp) 
	{
		m_pBrowserApp->Quit();
		m_pBrowserApp = NULL;
	}
	CWnd::OnDestroy(); // bypass CView doc/frame stuff
}

////////////////
// Override to avoid CView stuff that assumes a frame.
//

void CHtmlCtrl::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	if ( m_pBrowserApp )
    {
        // check if we are trying to open a new window though document state is not complete
        //typedef enum tagREADYSTATE{
        //    READYSTATE_UNINITIALIZED = 0,
        //    READYSTATE_LOADING = 1,
        //    READYSTATE_LOADED = 2,
        //    READYSTATE_INTERACTIVE = 3,
        //    READYSTATE_COMPLETE = 4
        //} READYSTATE;

        READYSTATE nReadyState;
        m_pBrowserApp->get_ReadyState(&nReadyState);
        if (nReadyState!=READYSTATE_COMPLETE)
        {
            *Cancel = TRUE;
            return;
        }
    }

    if( m_pNewWindow2 != NULL)
	{
	    *ppDisp = m_pNewWindow2->GetApplication();
	}
	else
	{
		CHtmlView::OnNewWindow2(ppDisp,Cancel);
	}
}

int CHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
{
	// bypass CView doc/frame stuff
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

//////////////////
// Override navigation handler to pass to "app:" links to virtual handler.
// Cancels the navigation in the browser, since app: is a pseudo-protocol.
//
void CHtmlCtrl::OnBeforeNavigate2( LPCTSTR		lpszURL,
								  DWORD			/*nFlags*/,
								  LPCTSTR		/*lpszTargetFrameName*/,
								  CByteArray&	/*baPostedData*/,
								  LPCTSTR		/*lpszHeaders*/,
								  BOOL*			pbCancel )
{
	const TCHAR APP_PROTOCOL[] = _T("app:");
	int len = _tcslen(APP_PROTOCOL);
	if (_tcsnicmp(lpszURL, APP_PROTOCOL, len)==0) 
	{
		OnAppCmd(lpszURL + len);
		*pbCancel = TRUE;
	}
}


// ***************************************************************************
//DESCRIPTION:
//		This is called by OnBeforeNavigate2() when a link starting with
//		"app:" is requested. 
//PARAMS:
//		lpszWhere	String identifing the action to perform.
//CREATED:
//		10-3-2001, 8:05:08 PM by john@mctainsh.com
// ***************************************************************************
void CHtmlCtrl::OnAppCmd(LPCTSTR lpszWhere)
{
	if( _tcscmp( _T("Minimise"), lpszWhere )==0 ) 
	{
		AfxGetMainWnd()->ShowWindow( SW_MINIMIZE );
	}	
	else if( _tcscmp( _T("Beep"), lpszWhere )==0 ) 
	{
		MessageBeep( MB_ICONASTERISK );
	}
	else
	{
		MessageBox( lpszWhere, _T("Unexpected Command"), MB_ICONQUESTION  );
	}
}


void CHtmlCtrl::OnDocumentComplete(LPCTSTR lpszURL)
{
	CHtmlView::OnDocumentComplete(lpszURL);

	if( !m_bHTMLDocumentLoadedOK)
	{
		//this->SetHTMLInDocument();
		m_bHTMLDocumentLoadedOK = TRUE;
	}		
}

void CHtmlCtrl::OnStatusTextChange(LPCTSTR lpszText)
{
	if( m_pStaticOutput && m_pStaticOutput->GetSafeHwnd())
	{
		m_pStaticOutput->SetWindowText(lpszText);
	}
}

void CHtmlCtrl::OnWindowClosing(BOOL IsChildWindow, BOOL *&Cancel)
{
	*Cancel = TRUE;
}

void CHtmlCtrl::OnWindowSetResizable(BOOL Resizable)
{
	Resizable = FALSE;
}


bool CHtmlCtrl::ParseHTMLString(CString &csCadenaATractar)
{
	csCadenaATractar.Replace(_T("?"),_T("&agrave;"));
	csCadenaATractar.Replace(_T("?"),_T("&aacute;"));
	csCadenaATractar.Replace(_T("?"),_T("&egrave;"));
	csCadenaATractar.Replace(_T("?"),_T("&eacute;"));
	csCadenaATractar.Replace(_T("?"),_T("&igrave;"));
	csCadenaATractar.Replace(_T("?"),_T("&iacute;"));
	csCadenaATractar.Replace(_T("?"),_T("&ograve;"));
	csCadenaATractar.Replace(_T("?"),_T("&oacute;"));
	csCadenaATractar.Replace(_T("?"),_T("&ugrave;"));
	csCadenaATractar.Replace(_T("?"),_T("&uacute;"));

	return true;
}

bool CHtmlCtrl::SetNewHTMLContent(CString csHTML, bool bReformat /*=false*/)
{
	//------------------------------------------------
	// Reassign the string.
	//------------------------------------------------
	this->m_csHTMLContent = csHTML;

	//------------------------------------------------
	// Reformat the string if needed.
	//------------------------------------------------
	if (bReformat)	ParseHTMLString(this->m_csHTMLContent);

	this->m_csHTMLContent.ReleaseBuffer();

	//------------------------------------------------
	// If the document has been loaded OK then the HTML
	// content gets modified now.
	//------------------------------------------------
	if (this->m_bHTMLDocumentLoadedOK)
		return this->SetHTMLInDocument();

	return true;
}

bool CHtmlCtrl::SetNewContent(CString strXML, CString strXsltPath,bool bReformat)
{
	CString strHtml = Trans2Html(strXML,strXsltPath);

	return SetNewHTMLContent(strHtml,bReformat);
}

bool CHtmlCtrl::SetHTMLInDocument()
{
	SAFEARRAY *pSafeArray = NULL;

	VARIANT *pElement = NULL;

	MSHTML::IHTMLDocument2Ptr pHTMLDoc2 = NULL;
	LPDISPATCH lpDispatch = NULL;

	HRESULT hr;

	bool bRetorn = false;

	// Processat del document HTML, assignaci?de punters
	// i escriptura del contingut de la cadena de text.
	lpDispatch = this->GetHtmlDocument();

	if (lpDispatch != NULL)
	{
		hr = lpDispatch->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pHTMLDoc2);

		if (!SUCCEEDED(hr))
		{
			bRetorn = false;
		}
		else
		{
			pSafeArray = ::SafeArrayCreateVector(VT_VARIANT, 0, 1);

			if (pSafeArray)
			{
				hr = ::SafeArrayAccessData(pSafeArray,(LPVOID*) &pElement);

				if (!SUCCEEDED(hr))
				{
					bRetorn = false;
				}
				else
				{
					pElement->vt				= VT_BSTR;
					pElement->bstrVal		= this->m_csHTMLContent.AllocSysString();

					hr = SafeArrayUnaccessData(pSafeArray);

					if (!SUCCEEDED(hr))
					{
						bRetorn = false;
					}
					else
					{
						pHTMLDoc2->clear();
						pHTMLDoc2->close();

						hr = pHTMLDoc2->write(pSafeArray);

						if (!SUCCEEDED(hr))
						{
							bRetorn = false;
						}
					}
					SysFreeString(pElement->bstrVal);
				}
				if (pSafeArray) SafeArrayDestroy(pSafeArray);
			}
			if (pHTMLDoc2)
			{
				pHTMLDoc2->close();
				pHTMLDoc2.Release();
			}
		}

		lpDispatch->Release();
	}

	return bRetorn;
}

CString CHtmlCtrl::Trans2Html(CString strXMLData, CString strXsltPath)
{
	variant_t vResult;
	void *output  = NULL;	
    MSXML2::IXMLDOMDocumentPtr pXml(MSXML2::CLSID_DOMDocument);
    MSXML2::IXMLDOMDocumentPtr pXslt(CLSID_FreeThreadedDOMDocument);
	IXSLTemplatePtr pTemplate(CLSID_XSLTemplate);
	IXSLProcessorPtr pProcessor;
	
	CString strHtmlData;		
	
	try{
		vResult = pXml->loadXML(_bstr_t(strXMLData));
		vResult = pXslt->load(_bstr_t(strXsltPath));
	} catch(_com_error &e) {
		CString strError;
		strError.Format(_T("Error transform : %s\n"),(const char*)_bstr_t(e.Description()));
		AfxMessageBox(strError);
		return strHtmlData;
	}
	
	
	try{
		vResult = pTemplate->putref_stylesheet(pXslt);
		pProcessor = pTemplate->createProcessor();
	} catch(_com_error &e) {
		CString strError;
		strError.Format(_T("Error transform : %s\n"),(const char*)_bstr_t(e.Description()));
		AfxMessageBox(strError);
		return strHtmlData;
	}
	
	
	// prepare Stream object to store results of transformation,
	// and set processor output to it
	VARIANT varValue;
	
	// attach to processor XML file we want to transform,
	// add one parameter, maxprice, with a value of 35, and
	// do the transformation
	vResult = pProcessor->put_input(_variant_t((IUnknown*)pXml));
	
	try{
		VARIANT_BOOL _result = pProcessor->transform();
	} catch(_com_error &e) {
		CString strError;
		strError.Format(_T("Error transform : %s\n"),(const char*)_bstr_t(e.Description()));
		AfxMessageBox(strError);
		return strHtmlData;
	}
	
	pProcessor->get_output(&varValue);
	
    //get results of transformation and print them to stdout
	VariantChangeType(&varValue, &varValue, 0, VT_BSTR);
	strHtmlData = varValue.bstrVal;
	
	
	//release before CoUninitialize()
	
	//CoUninitialize();
	
	return strHtmlData;	
}




