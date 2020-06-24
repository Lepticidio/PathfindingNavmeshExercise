#pragma once
#include <stdafx.h>
#include <vector>
class Path
{
public:
	USVec2D m_vStart, m_vEnd;
	std::vector<USVec2D> m_tPoints;
	Path(USVec2D _vStart, USVec2D _vEnd);
	void DrawDebug();
};

