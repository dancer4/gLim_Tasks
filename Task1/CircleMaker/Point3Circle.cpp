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
	// P0 - P1�� �߽���
	CPoint ptCenterP0_1;
	ptCenterP0_1.x = (ptP0.x + ptP1.x) * 0.5f;
	ptCenterP0_1.y = (ptP0.y + ptP1.y) * 0.5f;

	// P1 - P2�� �߽���
	CPoint ptCenterP1_2;
	ptCenterP1_2.x = (ptP1.x + ptP2.x) * 0.5f;
	ptCenterP1_2.y = (ptP1.y + ptP2.y) * 0.5f;

	// Line0: P0 - P1�� ���� ����
	float fPerp0 = -1.0f * (ptP0.x - ptP1.x) / (ptP0.y - ptP1.y);	// Line0 ����
	float fDist0 = ptCenterP0_1.y - fPerp0 * ptCenterP0_1.x;		// Line0 ���
	// Line1: P1 - P2�� ���� ����
	float fPerp1 = -1.0f * (ptP1.x - ptP2.x) / (ptP1.y - ptP2.y);	// Line1 ����
	float fDist1 = ptCenterP1_2.y - fPerp1 * ptCenterP1_2.x;		// Line1 ���
	
	// ����
	if (fabs(fPerp0 - fPerp1) <= REAL_EPSILON)
		return false;

	// ���� �߽���: Line0�� Line1�� ����
	m_ptCenter.x = (fDist1 - fDist0) / (fPerp0 - fPerp1);
	m_ptCenter.y = fPerp0 * m_ptCenter.x + fDist0;

	// ������
	m_iRadius = sqrt(pow((float)(ptP0.x - m_ptCenter.x), 2) + pow((float)(ptP0.y - m_ptCenter.y), 2));

	return true;
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
