#pragma once

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
	
	// Draw in Image
	void	Draw(CImage& ImgBoard);

private:
	void	AssignMemory(CImage& ImgBoard, int iX, int iY);

private:
	CPoint		m_ptCenter;		// 중심점
	int			m_iRadius;		// 반지름

	int			m_iWidth;		// 두께
};
//--------------------------------------------------------------------------------------------
