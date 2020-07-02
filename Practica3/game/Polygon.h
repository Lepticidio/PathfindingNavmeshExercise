#pragma once
#include "Edge.h"

class Polygon
{
public:
	float m_fH, m_fG, m_fF;
	Polygon();
	Polygon* pParent = nullptr;
	std::vector<USVec2D> m_tVertex;
	std::vector<Edge*> m_tEdge;
	void DrawDebug();
	void CalculateH(USVec2D _vEndPoint);
	void CalculateF();
	bool PointInPolygon(USVec2D _vPoint);
	USVec2D  AveragePoint();
	
};

