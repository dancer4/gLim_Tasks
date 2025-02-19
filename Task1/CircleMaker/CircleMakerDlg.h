
// CircleMakerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ImagePoint.h"
#include "Point3Circle.h"
#include <vector>

using std::vector;

#define MAX_POINT_COUNT		3

// CCircleMakerDlg dialog
class CCircleMakerDlg : public CDialogEx
{
// Construction
public:
	CCircleMakerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CIRCLEMAKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


public:
	void		Reset();

private:
	// Create ImageBoard
	bool		CreateImgBoard(int iWidth, int iHeight);
	// Clear ImageBoard
	void		ClearImgBoard(int iClearColor);

	// Draw ImageBoard
	void		DrawImgBoard();
	// Draw Point
	void		DrawPoint(int iX, int iY);
	// Draw All(Image Board, Image Points, Circle)
	void		DrawAll();

	// Is SelectPoint
	bool		IsSelectPoint(int iX, int iY);

	// Add Point
	bool		AddPoint(int iX, int iY);
	// Make 3 Point Circle
	void		Make3PointCircle();

private:
	CImage					m_ImgBoard;						// Image Board
	vector<ImagePoint*>		m_vImgPoints;					// Image Point
	Point3Circle			m_Pt3Circle;

	int						m_iPointIdx;					// Point Index
	bool					m_bLDown;						// Mouse LDown flag
	CPoint					m_ptPerMousePos;				// Mouse Per Position

	// Display Text
	CString					m_strLDown;						// Mouse LDown
	CString					m_strPos[MAX_POINT_COUNT];		// 3 Point 
	CString					m_strSelPointIdx;				// Select Point Index
	int						m_iCircleWidth;					// Circle Width

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnImgBoard();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
