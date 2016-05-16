// ======================================================================
//
// Tile.cpp - Tile class
//
// ======================================================================

#include "Tile.h"

Tile::Tile()
    : m_num(0)
{}



void Tile::setNum(int num)
{
	m_num = num;
}



int Tile::num()
{
	return m_num;
}



void Tile::addVertex(QVector2D &vtx)
{
	m_vertices.push_back(vtx);
}



QVector2D Tile::vertex(int k)
{
	return m_vertices[k];
}



void Tile::setRandColor()
{
	m_color.setRgb((rand() % 256), (rand() % 256), (rand() % 256));
}



QColor Tile::color()
{
	return m_color;
}


QVector2D Tile::centroid()

{

    float sum_x = 0, sum_y = 0;
    qreal x_centroid, y_centroid;

    for(int v=0; v<m_num; v++)
    {
      sum_x += vertex(v).x();
      sum_y += vertex(v).y();
    }

    x_centroid = sum_x / m_num;
    y_centroid = sum_y / m_num;

    m_centroid.setX(x_centroid);
    m_centroid.setY(y_centroid);

    return m_centroid;
}

