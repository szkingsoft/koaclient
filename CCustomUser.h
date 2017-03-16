#define ID_CUSBEGIN           WM_USER + 3599
#define ID_CUSTOMTYPE_1       WM_USER + 3600
#define ID_CUSEND             WM_USER + 3700
class CCustomUser
{
public:
	CCustomUser() { Init(); };
	~CCustomUser() {};

	UINT m_iType;
public:
	virtual void Init(){m_iType=0;};
};
class CCustomUser_Type1: public CCustomUser
{
public:
	CCustomUser_Type1() { Init(); };
	~CCustomUser_Type1() {};
public:
	CString m_strCustomMenuText;
	CString m_strCustomTips;

	virtual void Init()
	{
		m_iType = 0;
		m_strCustomMenuText = _T("");
		m_strCustomTips = _T("");
	};
};