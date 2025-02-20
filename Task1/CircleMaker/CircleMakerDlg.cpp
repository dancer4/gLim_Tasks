
// CircleMakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CircleMaker.h"
#include "CircleMakerDlg.h"
#include "afxdialogex.h"
#include <thread>
#include <chrono>
#include <random>
//#include <time.h>

using namespace std::chrono;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------------------------------------------------------------------------
// Processing Thread
void ProcessThread(CWnd* pWnd)
{
	if (NULL == pWnd) return;

	CCircleMakerDlg* pDlg = (CCircleMakerDlg*)pWnd;

	int iDrawCount = 10;	// 10번
	long iInterval = 500;	// 초당 2회(500 ms)

	auto tStartTime = system_clock::now();
	
	while (0 < iDrawCount)
	{
		// Calcu Time
		auto tCurTime = system_clock::now();
		long tDeltaTime = duration_cast<milliseconds>(tCurTime - tStartTime).count();

		if (iInterval < tDeltaTime)
		{
			// Draw Random
			pDlg->DrawRandom();
			pDlg->PostMessageW(UM_UPDATEDATA);

			--iDrawCount;

			tStartTime = tCurTime;		
		}

		Sleep(1);
	}

	int i = 0;
}
//---------------------------------------------------------------------------------------------

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCircleMakerDlg dialog



CCircleMakerDlg::CCircleMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCircleMakerDlg::IDD, pParent)
	, m_strLDown(_T(""))
	, m_strSelPointIdx(_T(""))
	, m_iPointWidth(DEFAULT_POINT_WIDTH)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Display Text 초기화
	for (int i = 0; i < MAX_POINT_COUNT; ++i)
		m_strPos[i] = _T("");
}

void CCircleMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STC_LDOWN, m_strLDown);
	DDX_Text(pDX, IDC_STC_POS00, m_strPos[0]);
	DDX_Text(pDX, IDC_STC_POS01, m_strPos[1]);
	DDX_Text(pDX, IDC_STC_POS02, m_strPos[2]);
	DDX_Text(pDX, IDC_STC_SELPOINT_IDX, m_strSelPointIdx);
	DDX_Text(pDX, IDC_EDT_POINT_W, m_iPointWidth);
}

BEGIN_MESSAGE_MAP(CCircleMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_IMG_BOARD, &CCircleMakerDlg::OnBnClickedBtnImgBoard)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_RAND_3POINT, &CCircleMakerDlg::OnBnClickedBtnRand3point)
	ON_MESSAGE(UM_UPDATEDATA, OnUpdateDataFromMessage)
END_MESSAGE_MAP()


// CCircleMakerDlg message handlers

BOOL CCircleMakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Reset
	Reset();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCircleMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCircleMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// Draw All(Image Board, Image Point, 3Point Circle)
		DrawAll();

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCircleMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//---------------------------------------------------------------------------------------------
// Reset
void CCircleMakerDlg::Reset()
{
	// ImageBoard 생성 - W: 800, H: 600
	CreateImgBoard(800, 600);

	// Drawing Points
	for (int i = 0, n = m_vDrawingPoints.size(); i < n; ++i)
	{
		DrawingPoint* pDrawingPoint = m_vDrawingPoints[i];
		if (NULL == pDrawingPoint) continue;

		delete pDrawingPoint;
	}
	m_vDrawingPoints.clear();
	
	m_Pt3Circle.Reset();			// Reset 3 Point Circle

	m_iPointIdx = -1;				// Point Index
	m_bLDown = false;				// Mouse LDown flag
	m_ptPerMousePos.SetPoint(0, 0);	// Mouse Per Position
		
	// Clear Text
	m_strLDown = "LDown:";
	for (int i = 0; i < MAX_POINT_COUNT; ++i)
	{
		m_strPos[i].Format(_T("Pos%02d:"), i);
	}
	// Display Select Point Index
	m_strSelPointIdx.Format(_T("SelPoint Idx: %d"), m_iPointIdx);

	UpdateData(FALSE);
}
//---------------------------------------------------------------------------------------------
// Create Image
bool CCircleMakerDlg::CreateImgBoard(int iWidth, int iHeight)
{
	bool bRet = false;

	// 생성조건 확인
	if (0 < iWidth || 0 < iHeight)
	{
		// 기존 Image 생성여부 확인
		if (!m_ImgBoard.IsNull())
		{
			m_ImgBoard.Destroy();	// 기존 Image 삭제
		}

		// Image 생성
		int iBpp = 8;
		m_ImgBoard.Create(iWidth, iHeight, iBpp);

		if (8 == iBpp)
		{
			static RGBQUAD RGBTable[256];
			for (int i = 0; i < 256; i++)
			{
				RGBTable[i].rgbRed = RGBTable[i].rgbGreen = RGBTable[i].rgbBlue = i;
			}

			m_ImgBoard.SetColorTable(0, 256, RGBTable);
		}

		// Clear ImageBoard
		ClearImgBoard(255);

		bRet = true;
	}

	return bRet;
}
//---------------------------------------------------------------------------------------------
// Clear ImageBoard
void CCircleMakerDlg::ClearImgBoard(int iClearColor)
{
	// Null Check
	if (!m_ImgBoard.IsNull())
	{
		int iWidth = m_ImgBoard.GetWidth();
		int iHeight = m_ImgBoard.GetHeight();
		
		byte* pbPixels = (byte*)m_ImgBoard.GetBits();
		if (NULL != pbPixels)
		{
			int iSize = sizeof(byte)* iWidth * iHeight;		// Image 메모리 Size
			int iPixelOffset = iSize - iWidth;				// Image 시작지점 Offset
			
			// Clear Image
			memset((pbPixels - iPixelOffset), iClearColor, iSize);
		}
	}
}
//---------------------------------------------------------------------------------------------
// Draw ImageBoard
void CCircleMakerDlg::DrawImgBoard()
{
	if (m_ImgBoard.IsNull()) return;

	CClientDC dcBoard(this);
	m_ImgBoard.Draw(dcBoard, 0, 0);
}
//---------------------------------------------------------------------------------------------
void CCircleMakerDlg::DrawPoint(int iX, int iY)
{
	CClientDC dcPoint(this);
	
	// Create Pen
	CPen penPoint;
	penPoint.CreatePen(PS_DOT, 3, RGB(255, 0, 0));    // 빨간색 펜을 생성
	CPen* pOldPen = dcPoint.SelectObject(&penPoint);

	// Create Brush
	CBrush brhPoint;
	brhPoint.CreateSolidBrush(RGB(255, 0, 0));     // 오렌지색 채움색을 생성
	CBrush* pOldBrush = dcPoint.SelectObject(&brhPoint);

	// Draw
	//dcPoint.Rectangle(iX, iY, iX + 10, iY + 10);
	dcPoint.Ellipse(iX, iY, iX + 10, iY + 10);

	dcPoint.SelectObject(pOldPen);     // 시스템 펜 객체를 돌려줌
	dcPoint.SelectObject(pOldBrush);    // 시스템 브러시 객체를 돌려줌

	// Delete Pen
	penPoint.DeleteObject();
	// Delete Brush
	brhPoint.DeleteObject();
}
//---------------------------------------------------------------------------------------------
// Draw All(Image Board, Image Points, Circle)
void CCircleMakerDlg::DrawAll()
{
	// Clear ImageBoard
	ClearImgBoard(255);

	// Draw Drawing Points
	for (int i = 0, n = m_vDrawingPoints.size(); i < n; ++i)
	{
		DrawingPoint* pDrawingPoint = m_vDrawingPoints[i];
		if (NULL == pDrawingPoint) continue;

		pDrawingPoint->Draw(m_ImgBoard);
	}

	// Draw 3 Points Circle
	m_Pt3Circle.Draw(m_ImgBoard);

	// Draw ImageBoard
	DrawImgBoard();
}
//---------------------------------------------------------------------------------------------
// 주어진 3 Point Random 재배치 및 Circle 재생성 그리기
void CCircleMakerDlg::DrawRandom()
{
	// Check Point Count
	if (MAX_POINT_COUNT > m_vDrawingPoints.size()) return;

	const int iWidth = m_ImgBoard.GetWidth();
	const int iHeight = m_ImgBoard.GetHeight();

	// Random
	random_device rdRand;								// Random Divece
	default_random_engine reGen(rdRand());				// Random Engine
	uniform_int_distribution<int> udDistW(0, iWidth);	// Random Distribution Width
	uniform_int_distribution<int> udDistH(0, iHeight);	// Random Distribution Height

	for (int i = 0; i < MAX_POINT_COUNT; ++i)
	{
		DrawingPoint* pDrawingPoint = m_vDrawingPoints[i];
		if (NULL == pDrawingPoint) continue;
		
		// Board 내에서 값 생성
		//int iX = rand() % iWidth;
		//int iY = rand() % iHeight;
		int iX = udDistW(reGen);
		int iY = udDistH(reGen);
		pDrawingPoint->SetPos(iX, iY);

		// DrawingPoint Pos
		CPoint ptPoint = pDrawingPoint->GetPos();
		m_strPos[i].Format(_T("Pos%02d: %d %d"), i, ptPoint.x, ptPoint.y);
		//m_strPos[i].Format(_T("Pos%02d: %d %d"), i, iX, iY);
	}

	// Make Circle
	Make3PointCircle();

	// Draw All(Image Board, Image Point, 3Point Circle)
	DrawAll();

	// Thread에서 호출오류 발생: 사용자 메시지 호출로 처리
	//UpdateData(FALSE);
}
//---------------------------------------------------------------------------------------------
// Is Select Point
bool CCircleMakerDlg::IsSelectPoint(int iX, int iY)
{
	bool bRet = false;

	int iPointSize = m_vDrawingPoints.size();	// Point 갯수

	// 생성된 Point에 대한 InBound 체크
	for (int i = 0; i < iPointSize; ++i)
	{
		DrawingPoint* pDrawingPoint = m_vDrawingPoints[i];
		if ((NULL != pDrawingPoint) && pDrawingPoint->IsInBound(iX, iY))
		{
			m_iPointIdx = i;
			bRet = true;
			break;
		}
	}

	return bRet;
}
//---------------------------------------------------------------------------------------------
// Add Point
bool CCircleMakerDlg::AddPoint(int iX, int iY)
{
	bool bRet = false;

	int iPointCount = m_vDrawingPoints.size();
	if (MAX_POINT_COUNT > iPointCount)
	{
		UpdateData(TRUE);

		// Add Point
		DrawingPoint* pDrawingPoint = new DrawingPoint(iX, iY, m_iPointWidth);
		m_vDrawingPoints.push_back(pDrawingPoint);

		// Draw in ImageBoard
		pDrawingPoint->Draw(m_ImgBoard, iX, iY, m_iPointWidth);

		// ImagePoint Pos
		m_strPos[iPointCount].Format(_T("Pos%02d: %d %d"), iPointCount, iX, iY);

		bRet = true;
	}

	if (bRet && (MAX_POINT_COUNT == m_vDrawingPoints.size()))
	{
		// Make 3 Point Circle
		Make3PointCircle();

		// Draw in ImageBoard
		m_Pt3Circle.Draw(m_ImgBoard);
	}

	return bRet;
}
//---------------------------------------------------------------------------------------------
// Make3PointCircle
void CCircleMakerDlg::Make3PointCircle()
{
	// Check Image Point Count
	if (MAX_POINT_COUNT > m_vDrawingPoints.size()) return;

	CPoint ptPoints[MAX_POINT_COUNT];
	for (int i = 0; i < MAX_POINT_COUNT; ++i)
	{
		DrawingPoint* pDrawingPoint = m_vDrawingPoints[i];
		if (NULL == pDrawingPoint) continue;

		ptPoints[i] = pDrawingPoint->GetPos();
	}

	// Make 3 Point Circle
	m_Pt3Circle.Make(ptPoints[0], ptPoints[1], ptPoints[2]);
}
//---------------------------------------------------------------------------------------------
void CCircleMakerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// ImageBoard 삭제
	m_ImgBoard.Destroy();
}
//---------------------------------------------------------------------------------------------
void CCircleMakerDlg::OnBnClickedBtnImgBoard()
{
	// Reset
	Reset();
	// Draw ImageBoard
	DrawImgBoard();

	// Update Data
	UpdateData(FALSE);
}
//---------------------------------------------------------------------------------------------
void CCircleMakerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLDown = true;		// Mouse LDown flag

	m_iPointIdx = -1;
	if (!IsSelectPoint(point.x, point.y))
	{
		AddPoint(point.x, point.y);
	}
		
	// Draw ImageBoard
	DrawImgBoard();

	// Display LDown Pos
	m_strLDown.Format(_T("LDown: %d %d"), point.x, point.y);

	// Display Select Point Index
	m_strSelPointIdx.Format(_T("SelPoint Idx: %d"), m_iPointIdx);

	UpdateData(FALSE);

	CDialogEx::OnLButtonDown(nFlags, point);
}
//---------------------------------------------------------------------------------------------
void CCircleMakerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bLDown && 0 <= m_iPointIdx)
	{
		DrawingPoint* pDrawingPoint = m_vDrawingPoints[m_iPointIdx];
		if (NULL != pDrawingPoint)
		{
			// Offset
			CPoint ptOffset = point - m_ptPerMousePos;
			pDrawingPoint->MovePos(ptOffset.x, ptOffset.y);

			// DrawingPoint Pos
			CPoint ptPoint = pDrawingPoint->GetPos();
			m_strPos[m_iPointIdx].Format(_T("Pos%02d: %d %d"), m_iPointIdx, ptPoint.x, ptPoint.y);

			UpdateData(FALSE);
		}

		// Make Circle
		Make3PointCircle();

		// Draw All(Image Board, Image Point, 3Point Circle)
		DrawAll();
	}

	m_ptPerMousePos = point;

	CDialogEx::OnMouseMove(nFlags, point);
}
//---------------------------------------------------------------------------------------------
void CCircleMakerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLDown = false;				// Mouse LDown flag

	CDialogEx::OnLButtonUp(nFlags, point);
}
//---------------------------------------------------------------------------------------------
void CCircleMakerDlg::OnBnClickedBtnRand3point()
{
	std::thread Thread(ProcessThread, this);
	Thread.detach();
}
//---------------------------------------------------------------------------------------------
// Thread에서 MessageMap을 통해 UpdateData(FALSE)처리
LRESULT CCircleMakerDlg::OnUpdateDataFromMessage(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);

	return 0;
}
//---------------------------------------------------------------------------------------------