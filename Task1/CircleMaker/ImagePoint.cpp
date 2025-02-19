#include "stdafx.h"
#include "ImagePoint.h"

#define IMGPOINT_PATH	_T("../bin/data/Point.bmp")

//--------------------------------------------------------------------------------------------
// Constructor
ImagePoint::ImagePoint() : m_pbImgPoint(NULL), m_fGapRate(0)
{
	// Load Image
	Load(IMGPOINT_PATH);
}
//--------------------------------------------------------------------------------------------
// Constructor
ImagePoint::ImagePoint(int iXPos, int iYPos) : m_pbImgPoint(NULL), m_fGapRate(0)
{
	// Load Image
	Load(IMGPOINT_PATH);
	SetPos(iXPos, iYPos);
}
//--------------------------------------------------------------------------------------------
// Destructor
ImagePoint::~ImagePoint()
{
	Release();
}
//--------------------------------------------------------------------------------------------
// Load
bool ImagePoint::Load(LPCTSTR strFilePath)
{
	bool bRet = false;
	
	// Compare FilePath
	if (0 != m_strFilePath.Compare(strFilePath))
	{
		// Release
		Release();

		// Load Image
		if (S_OK == m_ImgPoint.Load(strFilePath))
		{
			// Assign File Path
			m_strFilePath = strFilePath;

			m_pbImgPoint = (byte*)m_ImgPoint.GetBits();

			// Image 영역에 처리
			m_rtPoint.SetRect(0, 0, m_ImgPoint.GetWidth(), m_ImgPoint.GetHeight());
			SetPos(0, 0);

			int iPitch = m_ImgPoint.GetPitch();
			m_fGapRate = m_rtPoint.Width() / iPitch;

			// 비율 - 양수값으로 처리
			if (0.0f > m_fGapRate)
			{
				m_fGapRate = -m_fGapRate;
			}
		}
	}

	return bRet;
}
//--------------------------------------------------------------------------------------------
// Release
void ImagePoint::Release()
{
	m_ImgPoint.Destroy();
	m_pbImgPoint = NULL;

	m_rtPoint.SetRectEmpty();
	m_fGapRate = 0;

	m_strFilePath.Empty();
}
//--------------------------------------------------------------------------------------------
// Draw in Dialog
void ImagePoint::Draw(CDialog& DlgBoard, int iX, int iY)
{
	// Null Check
	if (m_ImgPoint.IsNull()) return;

	// Image를 입력지점 중심으로 그리기 위해 위치수정
	m_rtPoint.MoveToXY(iX - (m_rtPoint.Width() / 2), iY - (m_rtPoint.Height() / 2));

	// Draw
	CClientDC dcClient(&DlgBoard);
	m_ImgPoint.Draw(dcClient, m_rtPoint);
}
//--------------------------------------------------------------------------------------------
// Draw in Image with Input Pos(x, y)
void ImagePoint::Draw(CImage& ImgBoard, int iX, int iY)
{
	// Position
	SetPos(iX, iY);

	Draw(ImgBoard);
}
//--------------------------------------------------------------------------------------------
// Draw in BoardImage
void ImagePoint::Draw(CImage& ImgBoard)
{
	// Null Check
	if (ImgBoard.IsNull() || m_ImgPoint.IsNull()) return;

	// ImagePoint
	int iPitchPoint = m_ImgPoint.GetPitch();
	const int iWidth = m_rtPoint.Width();
	const int iHeight = m_rtPoint.Height();

	// Draw
	// ImageBoard
	const CPoint ptBoard(ImgBoard.GetWidth(), ImgBoard.GetHeight());

	byte* pbImgBoard = (byte*)ImgBoard.GetBits();
	int iPitchBorad = ImgBoard.GetPitch();

	// ImagePoint LeftTop
	CPoint ptPos(m_rtPoint.TopLeft());
	for (int iH = 0; iH < iHeight; ++iH, ++ptPos.y)
	{
		// 영역 체크
		if (ptBoard.y <= ptPos.y) break;
		else if (0 <= ptPos.y)
		{
			// X 시작위치 초기화
			int iPos = ptPos.x;
			for (int iW = 0; iW < iWidth; ++iW, ++iPos)
			{
				// 영역 체크
				if (ptBoard.x <= iPos) break;
				else if (0 <= iPos)
				{
					// Color 영역만 반영
					int iVal = m_pbImgPoint[iH * iPitchPoint + (int)(iW / m_fGapRate)];
					if (255 > iVal)
						pbImgBoard[ptPos.y * iPitchBorad + iPos] = iVal;
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
// Check InBound
bool ImagePoint::IsInBound(int iXPos, int iYPos)
{
	return m_rtPoint.PtInRect(CPoint(iXPos, iYPos));
}
//--------------------------------------------------------------------------------------------
// Set Position
void ImagePoint::SetPos(int iXPos, int iYPos)
{
	if (m_ImgPoint.IsNull()) return;

	// Assign Pos - Image를 입력지점 중심으로 그리기 위해 위치수정
	m_rtPoint.MoveToXY(iXPos - (m_rtPoint.Width() * 0.5f), iYPos - (m_rtPoint.Height() / 2));
}
//--------------------------------------------------------------------------------------------
// Get Position
const CPoint& ImagePoint::GetPos() const
{
	return m_rtPoint.CenterPoint();
}
//--------------------------------------------------------------------------------------------
// Move Position
void ImagePoint::MovePos(int iXOffset, int iYOffset)
{
	m_rtPoint += CPoint(iXOffset, iYOffset);
}
//--------------------------------------------------------------------------------------------