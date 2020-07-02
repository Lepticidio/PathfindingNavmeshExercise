#pragma once
#include <stdafx.h>
#include <vector>
class Navmesh;
class Polygon;
class Path
{
public:
	std::vector<USVec2D> m_tPoints;
	std::vector<Polygon*> m_tOpenNodes;
	std::vector<Polygon*> m_tClosedNodes;
	Path();
	void FindPath(Navmesh* _pNavmesh, USVec2D _vStart, USVec2D _vEnd);
	void DrawDebug();
};

