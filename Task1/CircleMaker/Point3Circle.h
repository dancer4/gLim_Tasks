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

	// Make a Circle: P0, P1, P2(3��)���� Circle �����
	bool	Make(CPoint ptP0, CPoint ptP1, CPoint ptP2);
	
	// Draw in Image
	void	Draw(CImage& ImgBoard);

private:
	void	AssignMemory(CImage& ImgBoard, int iX, int iY);

private:
	CPoint		m_ptCenter;		// �߽���
	int			m_iRadius;		// ������

	int			m_iWidth;		// �β�
};
//--------------------------------------------------------------------------------------------
