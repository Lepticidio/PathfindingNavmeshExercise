#pragma once
class Polygon;
class Edge
{
public:
	int m_verts[2]; // Indices de m_verts
	Polygon* m_pNeighbour;
	USVec2D MiddlePoint(Polygon* _pOriginPolygon);
	USVec2D FirstVertex (Polygon* _pOriginPolygon);
	USVec2D SecondVertex(Polygon* _pOriginPolygon);
};

