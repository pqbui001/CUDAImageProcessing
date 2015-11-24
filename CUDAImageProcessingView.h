
// CUDAImageProcessingView.h : interface of the CCUDAImageProcessingView class
//
#pragma once
#include <iostream>


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include <learnopengl/shader.h>

#include <GL/gl.h>     // Include the standard OpenGL headers
#include <GL/glu.h>
//#include <learnopengl/filesystem.h>

class CCUDAImageProcessingView : public CView
{
	// public methods
public: // OpenGL member functions
	virtual bool InitGLWindow(void);

	virtual bool InitGLStates(void);
	virtual bool InitLight();
	virtual bool InitTexture();

	virtual	bool RenderGLView(void);
	virtual void PreRenderScene(void);
	virtual void RenderScene(void);
	//virtual void RenderStaticScene(void);
	virtual void PostRenderScene(void);

	virtual	bool SetupProjection();
	virtual bool SetupViewport();

	virtual	bool SetupViewpoint(void);




protected: // create from serialization only
	CCUDAImageProcessingView();
	DECLARE_DYNCREATE(CCUDAImageProcessingView)

// Attributes
public:
	CCUDAImageProcessingDoc* GetDocument() const;
private:
	HGLRC		m_hRC;	 // holds the rendering context
	CDC*		m_pDC;   // holds the device context
	UINT		m_Timer; // timer ID
	GLdouble	m_AspectRatio; // width/height ratio of rendering window
	GLsizei		m_WindowWidth;
	GLsizei		m_WindowHeight;
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

private:
	//Error Handling
	void SetError(int e);
	static const char* const _ErrorStrings[];
	const char* m_ErrorString;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL SetupPixelFormat(PIXELFORMATDESCRIPTOR* pPFD = 0);
};

#ifndef _DEBUG  // debug version in CUDAImageProcessingView.cpp
inline CCUDAImageProcessingDoc* CCUDAImageProcessingView::GetDocument() const
   { return reinterpret_cast<CCUDAImageProcessingDoc*>(m_pDocument); }
#endif

