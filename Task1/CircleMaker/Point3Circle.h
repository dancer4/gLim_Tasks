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

	// Make a Circle: P0, P1, P2(3점)으로 Circle 만들기
	bool	Make(CPoint ptP0, CPoint ptP1, CPoint ptP2);
	// Make a Circle: vector에 저장된 P0, P1, P2(3점)으로 Circle 만들기
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
	
	// Calcu Center: 수직 Line을 통한 Center 계산
	void	CalcuCenter(const vector<float>& vfPerp, const vector<float>& vfDist, CPoint& ptCenter);
		
private:
	CPoint		m_ptCenter;		// 중심점
	int			m_iRadius;		// 반지름

	int			m_iWidth;		// 두께
};
//--------------------------------------------------------------------------------------------
