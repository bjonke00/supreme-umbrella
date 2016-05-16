#include "shapemorph.h"

bool comparator(const MyPair l, const MyPair r) {return (l.first < r.first);}

shapemorph::shapemorph(){
}

shapemorph::~shapemorph(){
};

void shapemorph::FindSourceDest(Tile tile1, Tile tile2)
{
    // tiles
    vector<QVector2D> source;       
    vector<QVector2D> destination;  

    // # of tile vertices
    int n1,n2;
    n1 = tile1.num();
    n2 = tile2.num();

    // centroids of tiles
    QVector2D cen1 = tile1.centroid();
    QVector2D cen2 = tile2.centroid();
    QVector2D t = cen2 - cen1;

    // align centroids - tile 2 over tile 1
    for(int i = 0; i < n2 ; ++i) {
        QVector2D vtx = tile2.vertex(i);
        vtx = vtx - t;
        tile2.setVertex(i,vtx);
    }

    // set centroid
    tile2.setCentroid();

    // intersections
    for(int i = 0; i < n1 ; ++i) {
        // draw line one - from centroid to first tile vertices
        const QVector2D vtx = tile1.vertex(i);
        float len = (vtx - cen1).length()*10;  				// length * 10 to insure intersection
        QLineF line1(cen1.x(),cen1.y(),vtx.x(),vtx.y());
        line1.setLength(len);
		
        int k = n2-1;

        // find intersecting vertices
        for (int j = 0; j < n2; ++j) {
            QVector2D vtx1 = tile2.vertex(k);
            QVector2D vtx2 = tile2.vertex(j);

            // create line 2 - from vertice to vertice creates edge
            QLineF line2(vtx1.x(),vtx1.y(),vtx2.x(),vtx2.y());
            QPointF IntersectPoint;			// Constructs a Copy of the point
            QLineF::IntersectType intersectType = line1.intersect(line2,&IntersectPoint);

            // pushback if intersection
            if(intersectType == QLineF::BoundedIntersection) {
                source.push_back(vtx);
                QVector2D dest(IntersectPoint.x(),IntersectPoint.y());
                destination.push_back(dest);
                break;
            }
            k = j;
        }
    }
    // intersections in source tile
    //line one from centroid to first tile vertices
    for(int i = 0; i < n2; ++i) {
        const QVector2D vtx = tile2.vertex(i);
        float len = (vtx - cen2).length()*10;
        QLineF line1(cen2.x(),cen2.y(),vtx.x(),vtx.y());
        line1.setLength(len);
		
        int k = n1-1;

        // find pair of vertices intersecting
        for (int j = 0; j < n1; ++j) {
            QVector2D vtx1 = tile1.vertex(k);
            QVector2D vtx2 = tile1.vertex(j);

            // create second line - line between the two pairs of vertices
            QLineF line2(vtx1.x(),vtx1.y(),vtx2.x(),vtx2.y());
            QPointF IntersectPoint;
            QLineF::IntersectType intersectType = line1.intersect(line2,&IntersectPoint);

            // push back points if intersection
            if(intersectType == QLineF::BoundedIntersection) {
                destination.push_back(vtx);
                QVector2D dest(IntersectPoint.x(),IntersectPoint.y());
                source.push_back(dest);
                break;
            }
            k = j;
        }
    }

    // start angle at 0 to start
    vector<MyPair> pairs;
    QVector2D &vtx = source[0]; 
    QVector2D v1 = vtx - cen1;
    MyPair pair(0.0f,0);
    pairs.push_back(pair);

    for (unsigned int i = 1; i < source.size(); ++i) {
        QVector2D &vtx = source[i];
        QVector2D v2   = vtx - cen1;
        float x1 = v1.x();
        float y1 = v1.y();
        float x2 = v2.x();
        float y2 = v2.y();

        //find the angles
        float dot = x1*x2 + y1*y2;
        float det = x1*x2 - y1*y2;
        float angle = atan2(det, dot);
        
        MyPair pair(angle,i);
        pairs.push_back(pair);
    }

    std::sort(pairs.begin(),pairs.end(),comparator);

    for(unsigned int i = 0; i < pairs.size();++i) {
        m_source.push_back(source[pairs[i].second]);
        m_destination.push_back(destination[pairs[i].second]);
    }
}

void shapemorph::InterPolate(float t, Tile &tile)
{
    tile.setNum(m_source.size());
    for(unsigned int i = 0; i < m_source.size(); ++i) {
        QVector2D &src = m_source[i];
        QVector2D &des = m_destination[i];
        QVector2D inter(src*(1 - t) + des*t);
        tile.addVertex(inter);
    }
}

