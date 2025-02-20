#include "stdafx.h"
#include "DrawingPoint.h"


//--------------------------------------------------------------------------------------------
// Constructor
DrawingPoint::DrawingPoint()
{
	Reset();
}
//--------------------------------------------------------------------------------------------
// Constructor
DrawingPoint::DrawingPoint(int iXPos, int iYPos, int iWidth)
{
	Reset(iWidth);

	SetPos(iXPos, iYPos);
}
//--------------------------------------------------------------------------------------------
DrawingPoint::~DrawingPoint()
{
}
//--------------------------------------------------------------------------------------------
// Reset()
void DrawingPoint::Reset(int iWidth)
{
	int iHalf = iWidth * 0.5f;
	int iOffset = iWidth - iHalf;		// 소수점에 값에 대한 보정

	// Rect 설정
	m_rtPoint.SetRect(-iHalf, -iHalf, iOffset, iOffset);
}
//--------------------------------------------------------------------------------------------
// Draw in Image with Input Pos(x, y)
void DrawingPoint::Draw(CImage& ImgBoard, int iX, int iY, int iWidth)
{
	// Position
	SetPos(iX, iY);

	Draw(ImgBoard);
}
//--------------------------------------------------------------------------------------------
// Draw in BoardImage
void DrawingPoint::Draw(CImage& ImgBoard)
{
	// Null Check
	if (ImgBoard.IsNull()) return;

	// ImagePoint
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
					if (IsInBound(iPos, ptPos.y))
						pbImgBoard[ptPos.y * iPitchBorad + iPos] = 0;	// Black
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
// Check InBound
bool DrawingPoint::IsInBound(int iXPos, int iYPos)
{
	bool bRet = false;

	// Point 영역 내
	if (m_rtPoint.PtInRect(CPoint(iXPos, iYPos)))
	{
		const float fRadius = m_rtPoint.Width() * 0.5f;
		const CPoint& ptCenter = m_rtPoint.CenterPoint();

		// 반지름과 길이의 제곱값
		float fSquareRadius = pow((float)fRadius, 2);
		float fSquareLength = pow((float)(iXPos - ptCenter.x), 2) + pow((float)(iYPos - ptCenter.y), 2);
			
		// 제곱값 비교(반지름, 길이)
		if (fSquareRadius >= fSquareLength) bRet = true;
	}

	return bRet;
}
//--------------------------------------------------------------------------------------------
// Set Position
void DrawingPoint::SetPos(int iXPos, int iYPos)
{
	// Assign Pos - 입력지점 중심으로 그리기 위해 위치수정
	m_rtPoint.MoveToXY(iXPos - (m_rtPoint.Width() * 0.5f), iYPos - (m_rtPoint.Height() / 2));
}
//--------------------------------------------------------------------------------------------
// Get Position
const CPoint& DrawingPoint::GetPos() const
{
	return m_rtPoint.CenterPoint();
}
//--------------------------------------------------------------------------------------------
// Move Position
void DrawingPoint::MovePos(int iXOffset, int iYOffset)
{
	m_rtPoint += CPoint(iXOffset, iYOffset);
}
//--------------------------------------------------------------------------------------------
// Set Width
void DrawingPoint::SetWidth(int iWidth)
{
	if (0 >= iWidth) return;

	CPoint ptCenter = m_rtPoint.CenterPoint();
	int iHalf = iWidth * 0.5f;
	int iOffset = iWidth - iHalf;	// 소수점에 값에 대한 보정

	// Rect 재설정
	m_rtPoint.SetRect(ptCenter.x - iHalf, ptCenter.y - iHalf,
		ptCenter.x + iOffset, ptCenter.y + iOffset);
}
//--------------------------------------------------------------------------------------------