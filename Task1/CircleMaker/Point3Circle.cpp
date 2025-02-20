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
	m_ptCenter.SetPoint(0, 0);		// �߽���
	m_iRadius = 0;					// ������

	m_iWidth = 1;					// �β�
}
//--------------------------------------------------------------------------------------------
// Make a Circle: P0, P1, P2(3��)���� Circle �����
bool Point3Circle::Make(CPoint ptP0, CPoint ptP1, CPoint ptP2)
{
	vector<CPoint> vPoints;
	vPoints.push_back(ptP0);
	vPoints.push_back(ptP1);
	vPoints.push_back(ptP2);

	return Make(vPoints);
}
//--------------------------------------------------------------------------------------------
// Make a Circle: vector�� ����� P0, P1, P2(3��)���� Circle �����
bool Point3Circle::Make(const vector<CPoint>& ptPoints)
{
	if (MAX_POINT_COUNT != ptPoints.size()) return false;

	// 0: P0 - P1�� �߽���, 1: P1 - P2�� �߽���, 2: P2 - P0�� �߽���
	CPoint ptCenterLine;
	// ���� ����
	vector<float> vfPerp, vfDist;
	for (int i = 0; i < MAX_POINT_COUNT; ++i)
	{
		int iP1 = i + 1;
		if (MAX_POINT_COUNT <= iP1) iP1 = 0;

		// Line Center Pos
		ptCenterLine.x = (ptPoints[i].x + ptPoints[iP1].x) * 0.5f;
		ptCenterLine.y = (ptPoints[i].y + ptPoints[iP1].y) * 0.5f;

		// ���� ����
		int iYLen = ptPoints[i].y - ptPoints[iP1].y;
		if (0 != iYLen)
		{
			float fPerp = -1.0f * (float)(ptPoints[i].x - ptPoints[iP1].x) / iYLen;		// Line ����
			float fDist = ptCenterLine.y - fPerp * ptCenterLine.x;						// Line ���

			vfPerp.push_back(fPerp);
			vfDist.push_back(fDist);
		}
	}
	
	// ����
	if ((2 > vfPerp.size()) || fabs(vfPerp[0] - vfPerp[1]) <= REAL_EPSILON)
		return false;
		
	// ���� �߽���: Line0�� Line1�� ����
	//m_ptCenter.x = (vfDist[1] - vfDist[0]) / (vfPerp[0] - vfPerp[1]);
	//m_ptCenter.y = vfPerp[0] * m_ptCenter.x + vfDist[0];
	// ���� �߽���
	CalcuCenter(vfPerp, vfDist, m_ptCenter);
	
	// ������
	m_iRadius = sqrt(pow((double)(ptPoints[0].x - m_ptCenter.x), 2) +
		pow((double)(ptPoints[0].y - m_ptCenter.y), 2));
	
	return true;
}
//--------------------------------------------------------------------------------------------
// Calcu Center: ���� Line�� ���� Center ���
void Point3Circle::CalcuCenter(const vector<float>& vfPerp, const vector<float>& vfDist, CPoint& ptCenter)
{
	const int iCount = vfPerp.size();
	if (2 > iCount) return;

	vector<CPoint> vTemp;
	CPoint ptTemp;
	for (int i = 0; i < iCount; ++i)
	{
		int iP1 = i + 1;
		if (iCount <= iP1) iP1 = 0;

		ptTemp.x = (vfDist[iP1] - vfDist[i]) / (vfPerp[i] - vfPerp[iP1]);
		ptTemp.y = vfPerp[i] * ptTemp.x + vfDist[i];

		vTemp.push_back(ptTemp);

		// �⺻��
		ptCenter = vTemp[i];

		if (1 < vTemp.size())
		{
			// �� ���� ���缺 üũ
			if ((ALLOWED_OFFSET >= abs(vTemp[i].x - vTemp[i - 1].x)) &&
				(ALLOWED_OFFSET >= abs(vTemp[i].y - vTemp[i - 1].y)))
			{
				ptCenter = vTemp[i - 1];
				break;
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

		int iOffsetX = m_iRadius * cosf(fRad);
		int iOffsetY = m_iRadius * sinf(fRad);

		DrawCircle(ImgBoard, m_ptCenter.x + iOffsetX, m_ptCenter.y + iOffsetY);
		DrawCircle(ImgBoard, m_ptCenter.x - iOffsetX, m_ptCenter.y + iOffsetY);
		DrawCircle(ImgBoard, m_ptCenter.x + iOffsetX, m_ptCenter.y - iOffsetY);
		DrawCircle(ImgBoard, m_ptCenter.x - iOffsetX, m_ptCenter.y - iOffsetY);
		//DrawPoint(ImgBoard, m_ptCenter.x + iOffsetX, m_ptCenter.y + iOffsetY);	// 0 ~ 90
		//DrawPoint(ImgBoard, m_ptCenter.x - iOffsetX, m_ptCenter.y + iOffsetY);	// 270 ~ 360
		//DrawPoint(ImgBoard, m_ptCenter.x + iOffsetX, m_ptCenter.y - iOffsetY);	// 90 ~ 180
		//DrawPoint(ImgBoard, m_ptCenter.x - iOffsetX, m_ptCenter.y - iOffsetY);	// 180 ~ 270
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

	// �� �β���ŭ Assign
	int iHalfWidth = m_iWidth * 0.5f;
	CPoint ptPos(iX - iHalfWidth, iY - iHalfWidth);
	for (int iH = 0; iH < m_iWidth; ++iH, ++ptPos.y)
	{
		// ���� üũ
		if (ptBoard.y <= ptPos.y) break;
		else if (0 <= ptPos.y)
		{
			int iPos = ptPos.x;
			for (int iW = 0; iW < m_iWidth; ++iW, ++iPos)
			{
				// ���� üũ
				if (ptBoard.x <= iPos) break;
				else if (0 <= iPos)
				{
					//pbImgBoard[ptPos.y * iPitchBoard + iPos] = 0;	// Black
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

	// �� �β���ŭ Assign
	int iHalfWidth = iWidth * 0.5f;
	CPoint ptPos(iX - iHalfWidth, iY - iHalfWidth);
	for (int iH = 0; iH < iWidth; ++iH, ++ptPos.y)
	{
		// ���� üũ
		if (ptBoard.y <= ptPos.y) break;
		else if (0 <= ptPos.y)
		{
			int iPos = ptPos.x;
			for (int iW = 0; iW < iWidth; ++iW, ++iPos)
			{
				// ���� üũ
				if (ptBoard.x <= iPos) break;
				else if (0 <= iPos)
				{
					//if (IsInPoint(iX, iY, iPos, ptPos.y, iHalfWidth))
					{
						pbImgBoard[ptPos.y * iPitchBoard + iPos] = 0;	// Black
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
//// Check in Point
//bool Point3Circle::IsInPoint(int iPtPosX, int iPtPosY, int iX, int iY, int iRadius)
//{
//	bool bRet = false;
//
//	// �������� ������ ������
//	float fSquareRadius = pow((float)iRadius, 2);
//	float fSquareLength = pow((float)(iPtPosX - iX), 2) + pow((float)(iPtPosY - iY), 2);
//	
//	// ������ ��(������, ����)
//	if (fSquareRadius >= fSquareLength) bRet = true;
//
//	return bRet;
//}
//--------------------------------------------------------------------------------------------
