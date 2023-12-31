
// Emulator.h : main header file for the Emulator application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CEmulatorApp:
// See Emulator.cpp for the implementation of this class
//

class CEmulatorApp : public CWinApp
{
public:
	CEmulatorApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEmulatorApp theApp;
