#pragma once

//--------------------------------------------------------------------------------------------
// class ImagePoint

class ImagePoint
{
public:
	ImagePoint();							// Constructor
	ImagePoint(int iXPos, int iYPos);		// Constructor
	virtual ~ImagePoint();					// Destructor

public:
	// Load
	bool	Load(LPCTSTR strFilePath);
	// Release
	void	Release();

	// Draw in Dialog
	void	Draw(CDialog& DlgBoard, int iX, int iY);
	// Draw in Image with Input Pos(x, y)
	void	Draw(CImage& ImgBoard, int iX, int iY);
	// Draw in Image
	void	Draw(CImage& ImgBoard);

	// Check InBound
	bool	IsInBound(int iXPos, int iYPos);

	// Set Position
	void	SetPos(int iXPos, int iYPos);
	// Get Position
	const CPoint&	GetPos() const;
	// Move Position
	void	MovePos(int iXOffset, int iYOffset);

private:
	CImage	m_ImgPoint;						// Point Image
	byte*	m_pbImgPoint;					// Bits

	CRect	m_rtPoint;
	float	m_fGapRate;						// Image Size와 Memory Size간의 차이비율

	CString	m_strFilePath;					// Image FilePath
};
//--------------------------------------------------------------------------------------------
