// ChangePswdForm.cpp : 实现文件
//
#include "stdafx.h"
#include "ChangePswdForm.h"
#include "Public.h"
// CChangePswdForm 对话框 

IMPLEMENT_DYNAMIC(CChangePswdForm, CDialog)

CChangePswdForm::CChangePswdForm(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePswdForm::IDD, pParent)
{
}

CChangePswdForm::~CChangePswdForm()
{
}

void CChangePswdForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CURPSWD, m_edtCurPswd);
	DDX_Control(pDX, IDC_EDIT_NEWPSWD, m_edtNewPswd);
	DDX_Control(pDX, IDC_EDIT_NEWPSWD2, m_edtNewPswd2);
	DDX_Control(pDX, IDC_COMBO_PWDTYPE, m_comPwdType);
}


BEGIN_MESSAGE_MAP(CChangePswdForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CChangePswdForm::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDCANCEL, &CChangePswdForm::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_PWDTYPE, &CChangePswdForm::OnCbnSelchangeComboPwdtype)
END_MESSAGE_MAP()


// CChangePswdForm 消息处理程序

void CChangePswdForm::OnBnClickedButtonOk()
{
	UpdateData();
	/// 当前密码

	CString strOldPwd;
	m_edtCurPswd.GetWindowText(strOldPwd);
	strOldPwd.Trim();
	char szOldPwd[100];
	memset(szOldPwd,0,sizeof(szOldPwd));
	int nDestLen;
	CPublic::Instance()->GetTradeDll()->m_pSHA1Encrypt((char*)(LPCTSTR)strOldPwd,strOldPwd.GetLength(),szOldPwd,nDestLen);
	//if(!CPublic::Instance()->ConfirmPwd(CString(szOldPwd)))
	//{
	//	MessageBox(CLanguageInterface::Instance()->GetStr(10001),CPublic::Instance()->GetProductName(),MB_OK);
	//	return;
	//}
	/// 新密码
	CString strNewPwd,strConfirmPwd;
	m_edtNewPswd.GetWindowText(strNewPwd);
	m_edtNewPswd2.GetWindowText(strConfirmPwd);
	int nSelType;
	nSelType = m_comPwdType.GetCurSel();
	strNewPwd.Trim();
	strConfirmPwd.Trim();
	if(strNewPwd.GetLength()<6&&nSelType != 2)
	{//新密码不能低于6位
		MessageBox(CLanguageInterface::Instance()->GetStr(10039),CPublic::Instance()->GetProductName(),MB_OK);
		return;
	}
	else if (nSelType == 2 && strNewPwd.GetLength() != 6)
	{
		MessageBox(_T("资金密码必须是6位的数字."),CPublic::Instance()->GetProductName(),MB_OK);
		return;
	}
	if(strNewPwd != strConfirmPwd)
	{
		MessageBox(CLanguageInterface::Instance()->GetStr(10002),CPublic::Instance()->GetProductName(),MB_OK|MB_ICONINFORMATION);
		return;
	}

	if (nSelType == 1)
		nSelType = 2;//电话密码
	else
		if(nSelType == 2)
			nSelType = 3;//资金密码

	char szNewPwd[100];
	memset(szNewPwd,0,sizeof(szNewPwd));
	CPublic::Instance()->GetTradeDll()->m_pSHA1Encrypt((char*)(LPCTSTR)strNewPwd,strNewPwd.GetLength(),szNewPwd,nDestLen);
	CPublic::Instance()->GetTradeDll()->m_pChangePassword(nSelType,szOldPwd,(char*)szNewPwd);
	if(nSelType == 0)
	{//只有修改主密码才同步修改内存信息
		CPublic::Instance()->ChangePwd(CString(szNewPwd));
	}
	CDialog::OnOK();
}

void CChangePswdForm::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


BOOL CChangePswdForm::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_comPwdType.AddString(CLanguageInterface::Instance()->GetStr(10017));
	//m_comPwdType.AddString(CLanguageInterface::Instance()->GetStr(10018));
	m_comPwdType.AddString(CLanguageInterface::Instance()->GetStr(10019));
	m_comPwdType.AddString(CLanguageInterface::Instance()->GetStr(10037));
	m_comPwdType.SetCurSel(0);

	CLanguageInterface::Instance()->InitDialog(IDD,this);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CChangePswdForm::OnCbnSelchangeComboPwdtype()
{
	UpdateData();
	int nSel = m_comPwdType.GetCurSel();
	CString strCurText;
	m_comPwdType.GetLBText(0,strCurText);
	GetDlgItem(IDC_EDIT_NEWPSWD)->ModifyStyle(ES_NUMBER,0,TRUE);
	GetDlgItem(IDC_EDIT_NEWPSWD2)->ModifyStyle(ES_NUMBER,0,TRUE);
	if (nSel == 2)
	{
		m_comPwdType.GetLBText(nSel,strCurText);
	    strCurText = _T("当前") + strCurText;

		//CEdit *pWnd = (CEdit *)GetDlgItem(IDC_STATIC_NEWPWD);
		//DWORD   dwStyle = pWnd->GetStyle();

		//dwStyle |= ES_NUMBER;
		//SetWindowLong(pWnd->m_hWnd, GWL_STYLE, dwStyle);
		//SetWindowLong(GetDlgItem(IDC_STATIC_CONFIRMPWD)->m_hWnd, GWL_STYLE, dwStyle);

		GetDlgItem(IDC_EDIT_NEWPSWD)->ModifyStyle(0,ES_NUMBER,TRUE);
		GetDlgItem(IDC_EDIT_NEWPSWD2)->ModifyStyle(0,ES_NUMBER,TRUE);
	}
	else if (nSel == 0)
	{
		strCurText = _T("当前") + strCurText;
	}
	GetDlgItem(IDC_STATIC_CURRENTPWD)->SetWindowText(strCurText);
	GetDlgItem(IDC_STATIC_CURRENTPWD)->RedrawWindow();
}
