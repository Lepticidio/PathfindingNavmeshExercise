#pragma once
#include "Edge.h"

class Polygon
{
public:
	Polygon();
	std::vector<USVec2D> m_tVertex;
	std::vector<Edge*> m_tEdge;
	void DrawDebug();
};

