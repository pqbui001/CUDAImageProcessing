
// CUDAImageProcessingView.cpp : implementation of the CCUDAImageProcessingView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CUDAImageProcessing.h"
#endif

#include "CUDAImageProcessingDoc.h"
#include "CUDAImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCUDAImageProcessingView

IMPLEMENT_DYNCREATE(CCUDAImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CCUDAImageProcessingView, CView)
END_MESSAGE_MAP()

// CCUDAImageProcessingView construction/destruction

CCUDAImageProcessingView::CCUDAImageProcessingView()
{
	// TODO: add construction code here

}

CCUDAImageProcessingView::~CCUDAImageProcessingView()
{
}

BOOL CCUDAImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCUDAImageProcessingView drawing

void CCUDAImageProcessingView::OnDraw(CDC* /*pDC*/)
{
	CCUDAImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// TODO: add draw code for native data here
}


// CCUDAImageProcessingView diagnostics

#ifdef _DEBUG
void CCUDAImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CCUDAImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCUDAImageProcessingDoc* CCUDAImageProcessingView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCUDAImageProcessingDoc)));
	return (CCUDAImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CCUDAImageProcessingView message handlers
