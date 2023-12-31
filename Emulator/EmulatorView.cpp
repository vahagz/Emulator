
// EmulatorView.cpp : implementation of the CEmulatorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Emulator.h"
#endif

#include "EmulatorDoc.h"
#include "EmulatorView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <iostream>
#include <thread>
#include <chrono>
#include <string>


// CEmulatorView

IMPLEMENT_DYNCREATE(CEmulatorView, CView)

BEGIN_MESSAGE_MAP(CEmulatorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_MENU_TOGGLE_START, &CEmulatorView::OnMenuToggleStart)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_RENDER, OnRender)
END_MESSAGE_MAP()

// CEmulatorView construction/destruction

CEmulatorView::CEmulatorView(): CView()
{
	// TODO: add construction code here
}

CEmulatorView::~CEmulatorView()
{
}

BOOL CEmulatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEmulatorView drawing

void CEmulatorView::OnDraw(CDC* pDC)
{
	//  First get a pointer to the document
	CEmulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	//  Save the current state of the device context
	int nDCSave = pDC->SaveDC();

	COLORREF clr;
	if (drawBackground) {
		drawBackground = false;

		//  Get the client rectangle
		CRect rcClient;
		GetClientRect(&rcClient);
		//  Get the background color of the board
		clr = pDoc->BackgroundColor();
		//	Draw the background first
		pDC->FillSolidRect(rcClient, clr);

		CRect gameBorder;
		gameBorder.top = offsetTop - 1;
		gameBorder.right = pDoc->GetWidth() * pDoc->GetRows() + 2;
		gameBorder.bottom = pDoc->GetHeight() * pDoc->GetColumns() + gameBorder.top + 2;
		pDC->Rectangle(gameBorder);
	}
	
	//  Create the brush for drawing
	CBrush br;
	br.CreateStockObject(HOLLOW_BRUSH);
	CBrush* pbrOld = pDC->SelectObject(&br);

	GameBoardItem<GameObject*>* item;
	CRect rcBlock;

	//	Draw the squares
	for (int row = 0; row < pDoc->GetRows(); row++)
	{
		for (int col = 0; col < pDoc->GetColumns(); col++)
		{
			//  Get the color for this board space
			item = pDoc->GetBoardSpace(row, col);
			item->lock();
			clr = item->get()->getColor();
			item->unlock();
			//  Calculate the size and position of this space
			rcBlock.top = row * pDoc->GetHeight() + offsetTop;
			rcBlock.left = col * pDoc->GetWidth() + offsetLeft;
			rcBlock.right = rcBlock.left + pDoc->GetWidth();
			rcBlock.bottom = rcBlock.top + pDoc->GetHeight();
			//  Fill in the block with the correct color
			pDC->FillSolidRect(&rcBlock, clr);
			//  Draw the block outline
			//pDC->Rectangle(&rcBlock);
		}
	}

	ShowFPS(pDC);

	//  Restore the device context settings
	pDC->RestoreDC(nDCSave);
	br.DeleteObject();
}


// CEmulatorView printing

BOOL CEmulatorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEmulatorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEmulatorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CEmulatorView diagnostics

#ifdef _DEBUG
void CEmulatorView::AssertValid() const
{
	CView::AssertValid();
}

void CEmulatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEmulatorDoc* CEmulatorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEmulatorDoc)));
	return (CEmulatorDoc*)m_pDocument;
}
#endif //_DEBUG


// CEmulatorView message handlers


void CEmulatorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	ResizeWindow();
	//InitTimer(fps);

	std::cout << "OnInitialUpdate" << std::endl;
	if (!gameThread) gameThread = new std::thread([](CEmulatorView* _this) {
		CEmulatorDoc* pDoc = _this->GetDocument();
		unsigned long long t1 = getTimeInMilliseconds();
		unsigned long long t2;
		double interval;
		while (true) {
			pDoc->WaitTillResume();
			t2 = getTimeInMilliseconds();
			interval = (double)1000 / _this->fps;
			if (t2 - t1 > interval) {
				t1 = t2;
				pDoc->Update();
				_this->PostMessage(WM_RENDER); //pDoc->UpdateAllViews(NULL);
			}
			pDoc->Start();
		}
	}, this);
}

void CEmulatorView::ResizeWindow()
{
	//  First get a pointer to the document
	CEmulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	drawBackground = true;

	//  Get the size of the client area and the window
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetParentFrame()->GetWindowRect(&rcWindow);
	//  Calculate the difference
	int nWidthDiff = rcWindow.Width() - rcClient.Width();
	int nHeightDiff = rcWindow.Height() - rcClient.Height();
	//  Change the window size based on the size of the game board
	rcWindow.right = rcWindow.left + pDoc->GetWidth() * pDoc->GetColumns() + nWidthDiff + offsetLeft + 1;
	rcWindow.bottom = rcWindow.top + pDoc->GetHeight() * pDoc->GetRows() + nHeightDiff + offsetTop + 1;
	//  The MoveWindow function resizes the frame window
	GetParentFrame()->MoveWindow(&rcWindow);
}

void CEmulatorView::InitTimer(int fps)
{
	SetTimer(renderTimerID, (double)1000 / fps, NULL);
}

void CEmulatorView::ShowFPS(CDC* pDC)
{
	framesInInterval++;

	// initializations
	CSize size;
	CRect rcBlock;
	CString str = (_T("max fps: ") + std::to_wstring(fps) + _T("↑↓")).data();
	CEmulatorDoc* pDoc = GetDocument();

	// calculating text height
	size = pDC->GetTextExtent(str);

	// drawing max fps block
	rcBlock.top = 0;
	rcBlock.left = pDoc->GetColumns() * pDoc->GetWidth() - fpsBlockWidth;
	rcBlock.right = rcBlock.left + fpsBlockWidth + 1;
	rcBlock.bottom = rcBlock.top + size.cy /*fpsBlockHeight*/;
	pDC->FillSolidRect(&rcBlock, RGB(255, 102, 0));
	pDC->DrawText(str, rcBlock, DT_TOP | DT_LEFT | DT_WORDBREAK);

	// checking if it's time to update actual fps
	if (getTimeInMilliseconds() - lastFPSShowed < showFPSInterval) return;

	// calculating actual fps
	lastFPSShowed = getTimeInMilliseconds();
	int fps = (int)ceil(framesInInterval * 1000 / (double)showFPSInterval);
	str = (_T("actual fps: ") + std::to_wstring(fps)).data();
	framesInInterval = 0;

	// drawing actual fps
	rcBlock.top = size.cy /*fpsBlockHeight*/;
	rcBlock.left = pDoc->GetColumns() * pDoc->GetWidth() - fpsBlockWidth;
	rcBlock.right = rcBlock.left + fpsBlockWidth + 1;
	rcBlock.bottom = rcBlock.top + size.cy /*fpsBlockHeight*/;
	pDC->FillSolidRect(&rcBlock, RGB(255, 102, 0));
	pDC->DrawText(str, rcBlock, DT_TOP | DT_LEFT | DT_WORDBREAK);
}

void CEmulatorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	GetDocument()->UpdateAllViews(NULL);

	CView::OnTimer(nIDEvent);
}


BOOL CEmulatorView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	//return CView::OnEraseBkgnd(pDC);
}

LRESULT CEmulatorView::OnRender(WPARAM wParam, LPARAM lParam)
{
	GetDocument()->UpdateAllViews(NULL);
	return 0;
}

void CEmulatorView::OnMenuToggleStart()
{
	// TODO: Add your command handler code here
	CEmulatorDoc* pDoc = GetDocument();
	if (pDoc->isStarted()) pDoc->Stop();
	else pDoc->Start();
}

void CEmulatorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP) SetFPSLimit(GetFPSLimit() + 1);
	else if (nChar == VK_DOWN) SetFPSLimit(GetFPSLimit() - 1);
	
	//if (nChar == VK_UP || nChar == VK_DOWN) InitTimer(fps);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}