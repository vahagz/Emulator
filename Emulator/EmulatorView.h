
// EmulatorView.h : interface of the CEmulatorView class
//

#pragma once
#include "helpers.h"

constexpr UINT WM_RENDER{ WM_USER + 1 };


class CEmulatorView : public CView
{
protected: // create from serialization only
	CEmulatorView();
	DECLARE_DYNCREATE(CEmulatorView)

// Attributes
public:
	CEmulatorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

// Implementation
public:
	void ResizeWindow(void);
	void ShowFPS(CDC* pDC);
	int GetFPSLimit(void) { return fps; };
	void SetFPSLimit(int _fps) { if (_fps > 0) fps = _fps; };
	void InitTimer(int fps);

	virtual ~CEmulatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	UINT_PTR renderTimerID = 1;

	int offsetLeft = 1;
	int offsetTop = 33;

	std::thread* gameThread = NULL;
	bool drawBackground = true;

	int fps = 30;
	int framesInInterval = 0;
	const int showFPSInterval = 1000;
	unsigned long long lastFPSShowed = getTimeInMilliseconds();

	int fpsBlockWidth = 120;
	int fpsBlockHeight = 16;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnRender(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuToggleStart();
};

#ifndef _DEBUG  // debug version in EmulatorView.cpp
inline CEmulatorDoc* CEmulatorView::GetDocument() const
   { return reinterpret_cast<CEmulatorDoc*>(m_pDocument); }
#endif

