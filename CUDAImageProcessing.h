
// CUDAImageProcessing.h : main header file for the CUDAImageProcessing application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCUDAImageProcessingApp:
// See CUDAImageProcessing.cpp for the implementation of this class
//

class CCUDAImageProcessingApp : public CWinApp
{
public:
	CCUDAImageProcessingApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
};

extern CCUDAImageProcessingApp theApp;
