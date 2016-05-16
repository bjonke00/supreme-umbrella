#ifndef SHAPEMORPH_H
#define SHAPEMORPH_H

#include <QVector2D>
#include <QLineF>
#include "Tile.h"

typedef std:: pair<float,int> MyPair;

class shapemorph
{
public:
    shapemorph();
    ~shapemorph();

    void FindSourceDest      (Tile ,Tile);
    void InterPolate         (float, Tile &);

private:
    vector<QVector2D> m_source;
    vector<QVector2D> m_destination;

};

#endif
