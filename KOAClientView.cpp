
// KOAClientView.cpp : CKOAClientView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKOAClientView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CKOAClientView 构造/析构

CKOAClientView::CKOAClientView()
{
	// TODO: 在此处添加构造代码

}

CKOAClientView::~CKOAClientView()
{
}

BOOL CKOAClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CKOAClientView 绘制

void CKOAClientView::OnDraw(CDC* /*pDC*/)
{
	CKOAClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CKOAClientView 打印


void CKOAClientView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKOAClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CKOAClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CKOAClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CKOAClientView 诊断

#ifdef _DEBUG
void CKOAClientView::AssertValid() const
{
	CView::AssertValid();
}

void CKOAClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKOAClientDoc* CKOAClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKOAClientDoc)));
	return (CKOAClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CKOAClientView 消息处理程序
