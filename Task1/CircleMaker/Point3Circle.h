#pragma once

#include <vector>

using namespace std;

#define MAX_POINT_COUNT		3

//--------------------------------------------------------------------------------------------
// class Point3Circle

class Point3Circle
{
public:
	Point3Circle();				// Constructor
	virtual ~Point3Circle();	// Destructor

public:
	// Reset
	void	Reset();

	// Make a Circle: P0, P1, P2(3��)���� Circle �����
	bool	Make(CPoint ptP0, CPoint ptP1, CPoint ptP2);
	// Make a Circle: vector�� ����� P0, P1, P2(3��)���� Circle �����
	bool	Make(const vector<CPoint>& vPoints);
	
	// Draw in Image
	void	Draw(CImage& ImgBoard);

private:
	// Draw Circle
	void	DrawCircle(CImage& ImgBoard, int iX, int iY);
	// Draw Point for Circle
	void	DrawPoint(CImage& ImgBoard, int iX, int iY, int iWidth);

	// Check in Point
	bool	IsInPoint(int iPtPosX, int iPtPosY, int iX, int iY, int iWidth);
	
	// Calcu Center: ���� Line�� ���� Center ���
	void	CalcuCenter(const vector<float>& vfPerp, const vector<float>& vfDist, CPoint& ptCenter);
		
private:
	CPoint		m_ptCenter;		// �߽���
	int			m_iRadius;		// ������

	int			m_iWidth;		// �β�
};
//--------------------------------------------------------------------------------------------
