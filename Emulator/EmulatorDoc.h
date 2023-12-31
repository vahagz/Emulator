
// EmulatorDoc.h : interface of the CEmulatorDoc class
//

#include "EmulatorGameBoard.h";


#pragma once


class CEmulatorDoc : public CDocument
{
protected: // create from serialization only
	CEmulatorDoc() noexcept;
	DECLARE_DYNCREATE(CEmulatorDoc)

// Attributes
public:

// Operations
public:
	/*  Functions for accessing the game board */
	GameBoardItem<GameObject*>* GetBoardSpace(int row, int col) {
		return m_board.GetBoardSpace(row, col);
	}
	COLORREF BackgroundColor() { return m_board.BackgroundColor(); }
	void SetupBoard(void) { m_board.SetupBoard(); }
	void DeleteBoard(void) { m_board.DeleteBoard(); }
	int GetWidth(void) { return m_board.GetWidth(); }
	int GetHeight(void) { return m_board.GetHeight(); }
	int GetColumns(void) { return m_board.GetColumns(); }
	int GetRows(void) { return m_board.GetRows(); }
	void Start(void) { m_board.Start(); started = true; }
	void Stop(void) { m_board.Stop(); started = false; }
	void WaitTillResume(void) { return m_board.WaitTillResume(); }

	void Update(void) { m_board.Update(); };

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CEmulatorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Instance of the game board
	CEmulatorGameBoard m_board;
	bool started = true;

public:
	bool isStarted() { return started; }

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
