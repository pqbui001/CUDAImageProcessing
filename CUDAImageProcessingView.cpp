
// CUDAImageProcessingView.cpp : implementation of the CCUDAImageProcessingView class
//

#include "stdafx.h"
#include <afxwin.h>
#include "Shader.h"
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

//Init
void InitializeOpenGL();

//Resize
bool ChangeSize(int w, int h);

// CCUDAImageProcessingView
IMPLEMENT_DYNCREATE(CCUDAImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CCUDAImageProcessingView, CView)
END_MESSAGE_MAP()

const char* const CCUDAImageProcessingView::_ErrorStrings[] = {
	{ "No Error" },                     // 0
	{ "Unable to get a DC" },           // 1
	{ "ChoosePixelFormat failed" },     // 2
	{ "SelectPixelFormat failed" },     // 3
	{ "wglCreateContext failed" },      // 4
	{ "wglMakeCurrent failed" },        // 5
	{ "wglDeleteContext failed" },      // 6
	{ "SwapBuffers failed" },           // 7
};

CCUDAImageProcessingView::CCUDAImageProcessingView() :
m_hRC(0),
m_pDC(0),
m_Timer(0),
m_AspectRatio(0.0f),
m_WindowWidth(0),
m_WindowHeight(0)
{
	// TODO: add construction code here

}
//
//CCUDAImageProcessingView::CCUDAImageProcessingView()
//{
//	// TODO: add construction code here
//
//}

CCUDAImageProcessingView::~CCUDAImageProcessingView()
{
}


bool CCUDAImageProcessingView::InitGLWindow()
{
	m_pDC = new CClientDC(this);
	if (NULL == m_pDC) // failure to get DC
	{
		AfxMessageBox(L"DC failed.\n", MB_OK);
		return false;
	}

	if (!SetupPixelFormat())
	{
		AfxMessageBox(L"SetupPixelFormat failed.\n", MB_OK);
		return false;
	}

	//CreateRGBPalette();

	if (0 == (m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		AfxMessageBox(L"wglCreateContext failed.", MB_OK);
		return false;
	}

	if (false == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		AfxMessageBox(L"wglMakeCurrent failed.", MB_OK);
		return false;
	}

	return (InitGLStates());

}
// 
// Initialize OpenGL rendering states
// 
bool CCUDAImageProcessingView::InitGLStates()
{
	// 
	// Modify following GL states or add additional GL states here
	//
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);	// Enable depth testing 
	// (i.e. enable hidden surface removal)
	glDepthFunc(GL_LESS);		// This is the default depth test function

	// 
	// Turn on any of the following capabilities if needed.
	//
	InitLight();
	//InitTexture ();
	//tbInit(LEFT_BUTTON, RIGHT_BUTTON); // trackball emulation

	return true;
}

bool CCUDAImageProcessingView::InitLight()
{
	//
	// Initialize light sources if needed
	//
	GLfloat light_position1[] = { 10.0f, 1.0f, 20.0f, 1.0f };
	GLfloat light_position2[] = { -20.0f, -60.0f, 20.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glPushMatrix();
	glLoadIdentity(); // the poitions are in the eye coordinates
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glPopMatrix();

	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

	// 
	//	Initialize front and/or back material colors if needed
	//
	GLfloat front_specular[] = { .99f, .94f, .81f };
	GLfloat front_diffuse[] = { .78f, .57f, .11f }; 	// bronze
	GLfloat front_ambient[] = { .33f, .22f, .27f };
	GLfloat front_shininess[] = { 90.6f };

	GLfloat back_specular[] = { 0.633f, 0.727811f, 0.633f, 1.0f };
	GLfloat back_diffuse[] = { 0.07568f, 0.91424f, 0.07568f, 1.0f };
	GLfloat back_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
	GLfloat back_shininess[] = { 90.6f };

	//  Define front face material properties
	glMaterialfv(GL_FRONT, GL_SPECULAR, front_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, front_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, front_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, front_ambient);

	//  Define back face material properties
	glMaterialfv(GL_BACK, GL_SPECULAR, back_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, back_shininess);
	glMaterialfv(GL_BACK, GL_DIFFUSE, back_diffuse);
	glMaterialfv(GL_BACK, GL_AMBIENT, back_ambient);

	//
	// Enable lighting, light sources and desired lighting model
	//
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // two-sided lighting

	//	glEnable(GL_AUTO_NORMAL); // Enable automatic normal generation
	//  glEnable(GL_NORMALIZE);	  // Automatic normalization of normals

	return true;
}

bool  CCUDAImageProcessingView::InitTexture()
{
	//
	//	Initialize texture mapping parameters
	//
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE); // or GL_DECAL (i.e. no lighting)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_REPEAT); // or GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_REPEAT); // or GL_CLAMP

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR); // or GL_NEAREST which is faster
	// at the expense of image qulity
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR); // GL_NEAREST which is faster at 
	// the expense of image qulity

	// 
	//	Initialize the texture image here, if there is any
	//


	//
	//	Set automatic texture coordinates generation if desired.
	//
	GLfloat splane[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	GLfloat tplane[] = { 0.0f, 1.0f, 0.0f, 0.0f };

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, splane);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, tplane);

	//
	//	Enable automatic texture coordinates generation and texture mapping
	//
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);

	return true;
}

BOOL CCUDAImageProcessingView::SetupPixelFormat(PIXELFORMATDESCRIPTOR* pPFD)
{
	// default pixel format for a single-buffered,
	// OpenGL-supporting, hardware-accelerated, 
	// RGBA-mode format. Pass in a pointer to a different
	// pixel format if you want something else
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),// size of this pfd
		1,                      // version number
		PFD_DRAW_TO_WINDOW |    // support window
		PFD_SUPPORT_OPENGL |	// support OpenGL
		PFD_DOUBLEBUFFER,       // support double buffer
		PFD_TYPE_RGBA,          // RGBA type
		24,                     // 24-bit color depth
		0, 0, 0, 0, 0, 0,       // color bits ignored
		0,                      // no alpha buffer
		0,                      // shift bit ignored
		0,                      // no accumulation buffer
		0, 0, 0, 0,             // accum bits ignored
		16,                     // 16-bit z-buffer
		0,                      // no stencil buffer
		0,                      // no auxiliary buffer
		PFD_MAIN_PLANE,         // main layer
		0,                      // reserved
		0, 0, 0                 // layer masks ignored
	};

	int pixelformat;

	PIXELFORMATDESCRIPTOR* pPFDtoUse;

	// let the user override the default pixel format
	pPFDtoUse = (0 == pPFD) ? &pfd : pPFD;

	if (0 == (pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), pPFDtoUse)))
	{
		AfxMessageBox(L"ChoosePixelFormat failed.");
		return FALSE;
	}

	if (FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, pPFDtoUse))
	{
		AfxMessageBox(L"SetPixelFormat failed.");
		return FALSE;
	}

	return TRUE;
}

int CCUDAImageProcessingView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	// Set up winodw for OpenGL rendering
	InitGLWindow();

	// Create a timer for timing drawing
	m_Timer = SetTimer(1,   // Timer ID
		50,  // Time-out value in millisecond
		NULL // Timer procedure
		);
	ASSERT(m_Timer != NULL);

	return 0;
}

BOOL CCUDAImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

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
	RenderGLView();
}

bool CCUDAImageProcessingView::RenderGLView(void)
{
	if (!m_WindowWidth || !m_WindowHeight)
		return false;

	// Perform any needed settings or calculations before rendering
	PreRenderScene();

	// If there's a static scene, put it here
	//RenderStaticScene();

	// Render scene (usually a main programming task)
	RenderScene();

	// Perform any calculations that don't involve rendering, like
	// bookkeeping, or dynamics calculations here for the next scene.
	PostRenderScene();

	return true;
}

void CCUDAImageProcessingView::PostRenderScene()
{
	// TODO: Draw dynamic scene here


}

void CCUDAImageProcessingView::PreRenderScene()
{
	// Set up viewing parameters
	SetupProjection();
	SetupViewport();
	SetupViewpoint();

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: do other pre-rendering shores here.

}

bool CCUDAImageProcessingView::SetupProjection()
{
	// TODO: Set up a projection matrix here.
	//     
	// For example, use either 1) or 2) in the following
	//
	// 1) for perspective transformation:
	/*
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60.0, m_AspectRatio, 1.0, 100.0);
	glMatrixMode (GL_MODELVIEW);

	// 2) for othographic transformation:
	*/
	GLdouble s;
	s = 5.0; // change s if needed to adjust the view window size

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (m_WindowWidth <= m_WindowHeight) // maintain the aspect ratio
		glOrtho(-s, s,
		-s*(GLdouble)m_WindowHeight / (GLdouble)m_WindowWidth,
		s*(GLdouble)m_WindowHeight / (GLdouble)m_WindowWidth,
		-10.0, 10.0);
	else
		glOrtho(-s*(GLdouble)m_WindowWidth / (GLdouble)m_WindowHeight,
		s*(GLdouble)m_WindowWidth / (GLdouble)m_WindowHeight,
		-s, s, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);

	return true;

}
bool CCUDAImageProcessingView::SetupViewpoint()
{

	// TODO: Set the viewpoint here.
	// For example, call:
	//
	//	glMatrixMode (GL_MODELVIEW);
	//	glLoadIdentity ();
	//  .......... any model-view trnsformations, for example:
	//	glTranslatef (0.0f, 0.0f, -3.0f);
	//
	//			OR
	//
	//	glMatrixMode (GL_MODELVIEW);
	//	glLoadIdentity ();
	//	gluLookAt (0.0, 0.0, 0.0,	// position
	//			  0.0, 0.0, -1.0,	// Aim at vector
	//			  0.0, 1.0, 0.0);	// Up vector
	// 

	return true;
}

bool CCUDAImageProcessingView::SetupViewport()
{
	// Set viewport to the full view window client area; if
	// you want a diferent viewport, change the following code.
	// If you choose to use a different viewport and want to
	// maintain aspect ration, be sure to set up the projection
	// properly.

	glViewport(0, 0, m_WindowWidth, m_WindowHeight);

	return true;
}

void CCUDAImageProcessingView::RenderScene()
{
	Shader *ourShader;
	ourShader = new Shader("basic.vs", "basic.frag");
	GLuint VBO, VAO, EBO;
	GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //point 0
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //point 1
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //point 2
		-1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, //point 3
		-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, //point 4
		0.0f, -1.0f, 0.0f, 0.5f, 0.5f, 0.5f, //point 5
		0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, //point 6
		1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, //point 7
	};

	GLuint indices[] = {
		0, 1, 2,
		1, 4, 3,
		4, 6, 5,
		2, 6, 7,
		1, 4, 6,
		1, 2, 6
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

	ourShader->Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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