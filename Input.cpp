// Input.cpp : implementation file
//

#include "stdafx.h"
#include "KOAclient.h"

#include "Input.h"
#include "Inputkey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CInput dialog
CInput::CInput(CWnd* pParent /*=NULL*/)
	: CDialog(CInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInput)
	//}}AFX_DATA_INIT
}

void CInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInput)
	DDX_Control(pDX, IDC_INPUT_EDIT, m_CodeEdit);
	DDX_Control(pDX, IDC_INDEX_LIST, m_Name);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_INPUTICON, m_stcIcon);
	DDX_Control(pDX, IDC_STATIC_INPUTTITLE, m_stcTitle);
}


BEGIN_MESSAGE_MAP(CInput, CDialog)
	//{{AFX_MSG_MAP(CInput)
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_COMMAND(IDC_CLOSE, OnClose)
	ON_EN_CHANGE(IDC_INPUT_EDIT, OnChangeInputEdit)
	ON_LBN_DBLCLK(IDC_INDEX_LIST, OnDblclkIndexList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInput message handlers

BOOL CInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect dlgRect;
	GetClientRect(dlgRect);
	CRect rect(0,0,20,20);
	rect.left = dlgRect.right-20;
	rect.right =dlgRect.right;
	button_close.Create(_T("close"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,IDC_CLOSE);

	button_close.LoadBitmaps(IDB_CLOSE_BITMAP2,IDB_CLOSE_BITMAP1);
	SendKeyborad(cKey,1, 0);
	Reposition();


	//////////////////////////////////////////////////////////////////////////
	CString strIconPath;
	strIconPath = CPublic::Instance()->GetSysPath()+_T("/interface/koa.ico");
	HICON   hIcon=(HICON)::LoadImage(AfxGetInstanceHandle(),strIconPath,IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
	m_stcIcon.SetIcon(hIcon);
	m_stcTitle.SetWindowText(CPublic::Instance()->GetProductName());
	//////////////////////////////////////////////////////////////////////////
	CLanguageInterface::Instance()->InitDialog(IDD,this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInput::OnOK() 
{
	int index = m_Name.GetCurSel();
	if( -1 != index)
	{
		CKeyGhostStruct& keycode = CInputKey::Instance()->keyWanted[index];
		m_pParentWnd->SetActiveWindow();
		m_pParentWnd->SetFocus();	
		
		UINT iRetMsg = WM_REQUESTDATA;
		if (m_uiMarket == 2) iRetMsg = WM_KEYGHOSTMSG;
		if(keycode.m_Type == KEYGHOST_STOCK)
		{//代码信息
			SCodeInfo *pCodeInfo = new SCodeInfo;
			strcpy(pCodeInfo->szCode,keycode.m_szStockCode);//代码信息
			m_pParentWnd->PostMessage(iRetMsg,KEYGHOST_STOCK,(LPARAM)pCodeInfo);
		}
		else if(keycode.m_Type == KEYGHOST_COMMAND)
		{//命令消息
			m_pParentWnd->PostMessage(iRetMsg,KEYGHOST_COMMAND,(LPARAM)keycode.m_dwCommand);
		}
		else if(keycode.m_Type == KEYGHOST_FORMULA)
		{//公式命令
			m_pParentWnd->PostMessage(iRetMsg,KEYGHOST_FORMULA,(LPARAM)keycode.m_szStockCode);
		}
	}
	m_CodeEdit.SetSel (0, -1);  // All
    m_CodeEdit.Clear();
	CDialog::OnOK();
}


void CInput::OnClose()
{
	OnCancel();
}


LRESULT CInput::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler m_szStockCode here and/or call default
	return HTCAPTION;
}


void CInput::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler m_szStockCode here and/or call default
	RECT rc;
	GetWindowRect(&rc);

	UINT g_cyCaption = GetSystemMetrics(SM_CYCAPTION);
	UINT g_cxSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME);
	UINT g_cySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME) ;
	
	rc.left += g_cxSizeFrame+2;
	rc.top += g_cySizeFrame+4;
	rc.right = rc.left + g_cyCaption;
	rc.bottom =rc.top + 14; 
				
	CDialog::OnNcLButtonDown(nHitTest, point);
		
	if ( PtInRect(&rc,point))
	{
		PostMessage(WM_SYSCOMMAND,SC_CLOSE,0);
	}
}

void CInput::OnChangeInputEdit() 
{
	UINT uNum = 0;;
	UpdateData(TRUE);
	m_CodeEdit.GetWindowText(m_szStockCode);

	m_Name.ResetContent();
	
	if(m_szStockCode.GetLength() <= 0) return;

	m_Name.SetRedraw( FALSE );

	CInputKey::Instance()->SearchkeyByAlias((CHAR*)(LPCTSTR)m_szStockCode, uNum,0,m_uiMarket);	
	if(uNum>0)		CInputKey::Instance()->SortWantKey(uNum);
	for(UINT i= 0, DN = 0; i< uNum && DN< 100; i++)
	{
		char s[256], r[256];
		memset(s,' ', sizeof(s));
		memset(r, 0, sizeof(r));
		s[19] = 0;
		CKeyGhostStruct& keywant = CInputKey::Instance()->keyWanted[i];
		int len = strlen(keywant.m_First);
		if(len)
			strncpy(r, keywant.m_First, 19);
		else
			strncpy(r, keywant.m_szStockCode,19);
		
		strncpy(s,r,strlen(r));
		
		//if(keywant.m_Type)
		{
			if(strlen(keywant.m_szStockName)>0 
				&& strlen(keywant.m_szStockName) < 20)
			{
				int pos = 7 - strlen(keywant.m_szStockName);
				strncpy(s+8+pos,keywant.m_szStockName, sizeof(keywant.m_szStockName));
			}
			else
			{
				continue;
			}
		//	strncpy(s+8,CInputKey::Instance()->keyWanted[i].m_szStockName, sizeof(CInputKey::Instance()->keyWanted[i].m_szStockName));
			
			s[19] = 0;
			if(m_szStockCode == r)
			{
				char r0[256];
				
				int len = strlen(CInputKey::Instance()->keyWanted[0].m_First);
				if(len)
					strncpy(r0, CInputKey::Instance()->keyWanted[0].m_First, 19);
				else
					strncpy(r0, CInputKey::Instance()->keyWanted[0].m_szStockCode,19);

				if(!strcmp(r, r0))
				{
					if(CInputKey::Instance()->keyWanted[i].m_dwIndex > CInputKey::Instance()->keyWanted[0].m_dwIndex)
					{
						CKeyGhostStruct temp = CInputKey::Instance()->keyWanted[i];
						CInputKey::Instance()->keyWanted.InsertAt(0, temp);
						CInputKey::Instance()->keyWanted.RemoveAt(i+1);
						m_Name.InsertString(0, CString(s));
					}
					else
					{
						 m_Name.AddString(CString(s));
					}

				}
				else
				{
					CKeyGhostStruct temp = CInputKey::Instance()->keyWanted[i];
					CInputKey::Instance()->keyWanted.InsertAt(0, temp);
					CInputKey::Instance()->keyWanted.RemoveAt(i+1);
				   	m_Name.InsertString(0, CString(s));
				}
			}
			else
			{
				m_Name.AddString(CString(s));
			}
			DN++;
		}
	}

	m_Name.SetRedraw(TRUE);	
	m_Name.SetCurSel(0);

	UpdateData(FALSE);

}


BOOL CInput::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized m_szStockCode here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
        {
            switch( pMsg->wParam )
                {
                case VK_UP:
                case VK_DOWN:
				case VK_PRIOR:
                case VK_NEXT: 
					m_Name.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam );
					return TRUE;
                default:
                    break;
			}
        }

	if(pMsg->message == WM_CHAR)
	{
		m_CodeEdit.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam );
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CInput::OnDblclkIndexList() 
{
	// TODO: Add your control notification handler m_szStockCode here
	OnOK();
}

void CInput::SendKeyborad(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_CodeEdit.PostMessage(WM_CHAR, nChar, MAKELPARAM(nRepCnt, nFlags));
}

void CInput::Reposition()
{ 
	CRect rect(0,0,20,20);
	RECT  dlgRect;
	
	if (!IsWindow(m_PosWnd->GetSafeHwnd()))return;

	m_PosWnd->GetWindowRect(&rect);
    //m_PosWnd->ClientToScreen(&rect);
	GetWindowRect(&dlgRect);
	
	dlgRect.left = rect.right - (dlgRect.right - dlgRect.left);
	dlgRect.top = rect.bottom - (dlgRect.bottom - dlgRect.top) ;
	
	SetWindowPos(&CWnd::wndBottom , dlgRect.left, dlgRect.top, dlgRect.right -dlgRect.left , dlgRect.bottom - dlgRect.top, SWP_NOSIZE); 
	
	m_CodeEdit.SetFocus();

}

BOOL CInput::DestroyWindow() 
{
	return CDialog::DestroyWindow();
}
