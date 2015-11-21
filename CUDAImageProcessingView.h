
// CUDAImageProcessingView.h : interface of the CCUDAImageProcessingView class
//

#pragma once


class CCUDAImageProcessingView : public CView
{
protected: // create from serialization only
	CCUDAImageProcessingView();
	DECLARE_DYNCREATE(CCUDAImageProcessingView)

// Attributes
public:
	CCUDAImageProcessingDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CCUDAImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CUDAImageProcessingView.cpp
inline CCUDAImageProcessingDoc* CCUDAImageProcessingView::GetDocument() const
   { return reinterpret_cast<CCUDAImageProcessingDoc*>(m_pDocument); }
#endif

