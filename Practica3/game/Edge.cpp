#include "stdafx.h"
#include "Edge.h"
#include "Polygon.h"

USVec2D Edge::MiddlePoint(Polygon* _pOriginPolygon)
{
	return  (_pOriginPolygon->m_tVertex[m_verts[0]] + _pOriginPolygon->m_tVertex[m_verts[1]]) / 2;
}
