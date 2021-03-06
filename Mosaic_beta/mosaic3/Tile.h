// ======================================================================
//
// Tile.h - Tile class
//
// ======================================================================

#ifndef TILE_H
#define TILE_H

#include <vector>
#include <QVector2D>
#include <QVector3D>
#include <QColor>

using namespace std;

class Tile {
public:
	Tile();
	void		setNum(int num);
	int		num();
	void		addVertex(QVector2D &);
	void		setRandColor();
	float		depth();
	float		angleX();
	float		angleY();
	float		angleZ();
	void		setDepth(float);
	void		updateDepth(float);
	void		updateAngles(float, float, float);
	QColor		color();
	QVector2D	vertex(int);
	void		setCentroid();
	QVector2D	centroid();

private:
	int			m_num;		// number of vertices 
	vector<QVector2D>	m_vertices;	// coordinates
	QVector2D		m_centroid;	// tile centroid
	QColor			m_color;	// tile color
	float			m_depth;
	float			m_angleX;
	float			m_angleY;
	float			m_angleZ;
};

#endif	// TILE_H
