
// KOAClientView.cpp : CKOAClientView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "KOAClient.h"
#endif

#include "KOAClientDoc.h"
#include "KOAClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKOAClientView

IMPLEMENT_DYNCREATE(CKOAClientView, CView)

BEGIN_MESSAGE_MAP(CKOAClientView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKOAClientView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CKOAClientView ����/����

CKOAClientView::CKOAClientView()
{
	// TODO: �ڴ˴���ӹ������

}

CKOAClientView::~CKOAClientView()
{
}

BOOL CKOAClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CKOAClientView ����

void CKOAClientView::OnDraw(CDC* /*pDC*/)
{
	CKOAClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CKOAClientView ��ӡ


void CKOAClientView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKOAClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CKOAClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CKOAClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CKOAClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKOAClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKOAClientView ���

#ifdef _DEBUG
void CKOAClientView::AssertValid() const
{
	CView::AssertValid();
}

void CKOAClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKOAClientDoc* CKOAClientView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKOAClientDoc)));
	return (CKOAClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CKOAClientView ��Ϣ�������
