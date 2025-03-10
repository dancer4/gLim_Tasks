#include "stdafx.h"
#include "Point3Circle.h"

#define PI				3.14159265
#define DEG2RAD(x)		((x * PI) / 180)

//--------------------------------------------------------------------------------------------
// Constructor
Point3Circle::Point3Circle()
{
}
//--------------------------------------------------------------------------------------------
// Destructor
Point3Circle::~Point3Circle()
{
}
//--------------------------------------------------------------------------------------------
void Point3Circle::Reset()
{
	m_ptCenter.SetPoint(0, 0);		// 중심점
	m_iRadius = 0;					// 반지름

	m_iWidth = 1;					// 두께
}
//--------------------------------------------------------------------------------------------
// Make a Circle: P0, P1, P2(3점)으로 Circle 만들기
bool Point3Circle::Make(CPoint ptP0, CPoint ptP1, CPoint ptP2)
{
	vector<CPoint> vPoints;
	vPoints.push_back(ptP0);
	vPoints.push_back(ptP1);
	vPoints.push_back(ptP2);

	return Make(vPoints);
}
//--------------------------------------------------------------------------------------------
// Make a Circle: vector에 저장된 P0, P1, P2(3점)으로 Circle 만들기
bool Point3Circle::Make(const vector<CPoint>& ptPoints)
{
	if (MAX_POINT_COUNT != ptPoints.size()) return false;

	// 0: P0 - P1의 중심점, 1: P1 - P2의 중심점, 2: P2 - P0의 중심점
	CPoint ptCenterLine;
	// 수직 기울기
	vector<float> vfPerp, vfDist;
	for (int i = 0; i < MAX_POINT_COUNT; ++i)
	{
		int iP1 = i + 1;
		if (MAX_POINT_COUNT <= iP1) iP1 = 0;

		// Line Center Pos
		ptCenterLine.x = (ptPoints[i].x + ptPoints[iP1].x) * 0.5f;
		ptCenterLine.y = (ptPoints[i].y + ptPoints[iP1].y) * 0.5f;

		// 수직 기울기
		int iYLen = ptPoints[i].y - ptPoints[iP1].y;
		if (0 != iYLen)
		{
			float fPerp = -1.0f * (float)(ptPoints[i].x - ptPoints[iP1].x) / iYLen;		// Line 기울기
			float fDist = ptCenterLine.y - fPerp * ptCenterLine.x;						// Line 상수

			vfPerp.push_back(fPerp);
			vfDist.push_back(fDist);
		}
	}
	
	// 평행
	if ((2 > vfPerp.size()) || fabs(vfPerp[0] - vfPerp[1]) <= REAL_EPSILON)
		return false;
		
	// 원의 중심점
	CalcuCenter(vfPerp, vfDist, m_ptCenter);
	
	// 반지름
	m_iRadius = sqrt(pow((double)(ptPoints[0].x - m_ptCenter.x), 2) +
		pow((double)(ptPoints[0].y - m_ptCenter.y), 2));
	
	return true;
}
//--------------------------------------------------------------------------------------------
// Calcu Center: 수직 Line을 통한 Center 계산
void Point3Circle::CalcuCenter(const vector<float>& vfPerp, const vector<float>& vfDist, CPoint& ptCenter)
{
	const int iCount = vfPerp.size();
	if (2 > iCount) return;

	vector<CPoint> vTemp;
	CPoint ptTemp;
	// 보정을 위한 Check 값
	int iXOffset = INT_MAX;
	int iYOffset = INT_MAX;
	for (int i = 0; i < iCount; ++i)
	{
		int iP1 = i + 1;
		if (iCount <= iP1) iP1 = 0;

		ptTemp.x = (vfDist[iP1] - vfDist[i]) / (vfPerp[i] - vfPerp[iP1]);
		ptTemp.y = vfPerp[i] * ptTemp.x + vfDist[i];

		vTemp.push_back(ptTemp);

		if (1 < vTemp.size())
		{
			// 계산결과가 유사한 값 적용
			// X 값
			int iOffset = abs(vTemp[i].x - vTemp[i - 1].x);
			if (iXOffset > iOffset)
			{
				ptCenter.x = vTemp[i - 1].x;
				iXOffset = iOffset;
			}

			// Y 값
			iOffset = abs(vTemp[i].y - vTemp[i - 1].y);
			if (iYOffset > iOffset)
			{
				ptCenter.y = vTemp[i - 1].y;
				iYOffset = iOffset;
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
void Point3Circle::Draw(CImage& ImgBoard)
{
	// Null Check
	if (ImgBoard.IsNull() || (0 >= m_iRadius)) return;
	
	for (float fDeg = 0; fDeg <= 90.0f; fDeg += 0.1f)
	{
		// Degree -> Radian
		float fRad = DEG2RAD(fDeg);

		int iOffsetX = m_iRadius * sinf(fRad);
		int iOffsetY = m_iRadius * cosf(fRad); 

		DrawCircle(ImgBoard, m_ptCenter.x + iOffsetX, m_ptCenter.y + iOffsetY);		// 0 ~ 90
		DrawCircle(ImgBoard, m_ptCenter.x - iOffsetX, m_ptCenter.y + iOffsetY);		// 270 ~ 360
		DrawCircle(ImgBoard, m_ptCenter.x + iOffsetX, m_ptCenter.y - iOffsetY);		// 90 ~ 180
		DrawCircle(ImgBoard, m_ptCenter.x - iOffsetX, m_ptCenter.y - iOffsetY);		// 180 ~ 270
	}
}
//--------------------------------------------------------------------------------------------
// Draw Circle
void Point3Circle::DrawCircle(CImage& ImgBoard, int iX, int iY)
{
	if (ImgBoard.IsNull()) return;

	// ImageBoard
	const CPoint ptBoard(ImgBoard.GetWidth(), ImgBoard.GetHeight());

	byte* pbImgBoard = (byte*)ImgBoard.GetBits();
	int iPitchBoard = ImgBoard.GetPitch();

	// 원 두께만큼 Assign
	int iHalfWidth = m_iWidth * 0.5f;
	CPoint ptPos(iX - iHalfWidth, iY - iHalfWidth);
	for (int iH = 0; iH < m_iWidth; ++iH, ++ptPos.y)
	{
		// 영역 체크
		if (ptBoard.y <= ptPos.y) break;
		else if (0 <= ptPos.y)
		{
			int iPos = ptPos.x;
			for (int iW = 0; iW < m_iWidth; ++iW, ++iPos)
			{
				// 영역 체크
				if (ptBoard.x <= iPos) break;
				else if (0 <= iPos)
				{
					DrawPoint(ImgBoard, iX, iY, m_iWidth);
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
// Draw Point for Circle
void Point3Circle::DrawPoint(CImage& ImgBoard, int iX, int iY, int iWidth)
{
	if (ImgBoard.IsNull()) return;

	// ImageBoard
	const CPoint ptBoard(ImgBoard.GetWidth(), ImgBoard.GetHeight());

	byte* pbImgBoard = (byte*)ImgBoard.GetBits();
	int iPitchBoard = ImgBoard.GetPitch();

	// 원 두께만큼 Assign
	int iHalfWidth = iWidth * 0.5f;
	CPoint ptPos(iX - iHalfWidth, iY - iHalfWidth);
	for (int iH = 0; iH < iWidth; ++iH, ++ptPos.y)
	{
		// 영역 체크
		if (ptBoard.y <= ptPos.y) break;
		else if (0 <= ptPos.y)
		{
			int iPos = ptPos.x;
			for (int iW = 0; iW < iWidth; ++iW, ++iPos)
			{
				// 영역 체크
				if (ptBoard.x <= iPos) break;
				else if (0 <= iPos)
				{
					pbImgBoard[ptPos.y * iPitchBoard + iPos] = 0;	// Black
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
