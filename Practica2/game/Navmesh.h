#pragma once
#include <stdafx.h>
#include <tinyxml.h>
#include <vector>
#include "Polygon.h"
class Navmesh
{

public:
	std::vector<Polygon> m_tPolygons;
	Navmesh(const char* filename);
	void DrawDebug();
	bool PointInNavmesh(USVec2D _vPoint);
	Polygon* PolygonFromPoint(USVec2D _vPoint);
};

