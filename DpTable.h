#pragma once


// CDpTable

class CDpTable : public CDockablePane
{
	DECLARE_DYNAMIC(CDpTable)

public:
	CDpTable();
	virtual ~CDpTable();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CStatic m_statTab;

};


