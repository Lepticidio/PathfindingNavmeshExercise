#pragma once
class Polygon;
class Edge
{
public:
	int m_verts[2]; // Indices de m_verts
	Polygon* m_pNeighbour;
};

