#pragma once

#define DEFAULT_POINT_WIDTH		30

//--------------------------------------------------------------------------------------------
// class DrawingPoint

class DrawingPoint
{
public:
	DrawingPoint();															// Constructor
	DrawingPoint(int iXPos, int iYPos, int iWidth = DEFAULT_POINT_WIDTH);	// Constructor
	virtual ~DrawingPoint();												// Destructor

public:
	// Reset
	void	Reset(int iWidth = DEFAULT_POINT_WIDTH);

	// Draw at Input Pos(x, y)
	void	Draw(CImage& ImgBoard, int iX, int iY, int iWidth);
	// Draw
	void	Draw(CImage& ImgBoard);

	// Check InBound
	bool	IsInBound(int iXPos, int iYPos);

	// Set Position
	void	SetPos(int iXPos, int iYPos);
	// Get Position
	const CPoint&	GetPos() const;
	// Move Position
	void	MovePos(int iXOffset, int iYOffset);

	// Set Width
	void	SetWidth(int iWidth);

private:
	CRect	m_rtPoint;
};
//--------------------------------------------------------------------------------------------
