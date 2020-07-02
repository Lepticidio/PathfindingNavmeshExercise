#include "stdafx.h"
#include "Path.h"
#include "Navmesh.h"


USVec2D LinesIntersection(USVec2D _vPoint1, USVec2D _vPoint2, USVec2D _vPoint3, USVec2D _vPoint4)
{
    // Line AB represented as a1x + b1y = c1 
    float a1 = _vPoint2.mY - _vPoint1.mY;
    float b1 = _vPoint1.mX - _vPoint2.mX;
    float c1 = a1 * (_vPoint1.mX) + b1 * (_vPoint1.mY);

    // Line CD represented as a2x + b2y = c2 
    float a2 = _vPoint4.mY - _vPoint3.mY;
    float b2 = _vPoint3.mX - _vPoint4.mX;
    float c2 = a2 * (_vPoint3.mX) + b2 * (_vPoint3.mY);

    float fDeterminant = a1 * b2 - a2 * b1;

    if (fDeterminant == 0)
    {
        // The lines are parallel. This is simplified 
        // by returning a pair of FLT_MAX 
        return _vPoint1;
    }
    else
    {
        float x = (b2 * c1 - b1 * c2) / fDeterminant;
        float y = (a1 * c2 - a2 * c1) / fDeterminant;
        return USVec2D(x, y);
    }
}

USVec2D ClosestPointInSegment(USVec2D _vASegment, USVec2D _vBSegment, USVec2D _vPoint)
{
    USVec2D vV = _vBSegment - _vASegment;
    USVec2D vU = _vASegment - _vPoint;

    float fT = -vV.Dot(vU) / vV.Dot(vV);

    if (fT < 0)
    {
        fT = 0;
    }
    else if (fT > 1)
    {
        fT = 1;
    }

    return USVec2D((1 - fT) * _vASegment.mX, (1 - fT) * _vASegment.mY) + USVec2D(fT * _vBSegment.mX, fT * _vBSegment.mY);
}

Path::Path()
{
}
void Path::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
    if (m_tPoints.size() > 1)
    {

        gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 1.0f);
        for (int i = 1; i < m_tPoints.size(); i++)
        {
            MOAIDraw::DrawLine(m_tPoints[i - 1], m_tPoints[i]);
        }
    }
}
void Path::FindPath(Navmesh* _pNavmesh, USVec2D _vStart, USVec2D _vEnd)
{
    Polygon* pCurrent = _pNavmesh->PolygonFromPoint(_vStart);
    Polygon* pTarget = _pNavmesh->PolygonFromPoint(_vEnd);
    m_tPoints.clear();
    m_tOpenNodes.clear();
    m_tClosedNodes.clear();
    if (pCurrent != nullptr && pTarget != nullptr)
    {
        //Calculate H for all
        for (int i = 0; i < _pNavmesh->m_tPolygons.size(); i++)
        {
            _pNavmesh->m_tPolygons[i].CalculateH(_vEnd);
        }

        //Calculate values for Current, and add to OPEN
        m_tOpenNodes.push_back(pCurrent);
        pCurrent->m_fG = 0;
        pCurrent->CalculateF();
        pCurrent->pParent = nullptr;

        USVec2D vCurrentPoint = _vStart;

        while (pCurrent != pTarget)
        {
            //Set current as node in OPEN with lowest f, and move it to CLOSED
            float fLowestF = INFINITY;
            int iIndexLowestF = 0;
            for (int i = 0; i < m_tOpenNodes.size(); i++)
            {
                if (m_tOpenNodes[i]->m_fF < fLowestF)
                {
                    fLowestF = m_tOpenNodes[i]->m_fF;
                    pCurrent = m_tOpenNodes[i];
                    iIndexLowestF = i;
                }
            }
            m_tOpenNodes.erase(m_tOpenNodes.begin() + iIndexLowestF);
            m_tClosedNodes.push_back(pCurrent);

            for (int i = 0; i < pCurrent->m_tEdge.size(); i++)
            {
                Edge* pEdge = pCurrent->m_tEdge[i];
                Polygon* pNeighbour = pEdge->m_pNeighbour;
                

                //Neighbour not in closed
                if (std::find(m_tClosedNodes.begin(), m_tClosedNodes.end(), pNeighbour) == m_tClosedNodes.end())
                {
                    USVec2D vMiddleOfEdge = pEdge->MiddlePoint(pCurrent);
                    float fNewG = (vMiddleOfEdge - vCurrentPoint).Length() + pCurrent->m_fG;

                    //if the path to neighbour is shorter or neighbour is not in OPEN
                    bool bInOpen = std::find(m_tOpenNodes.begin(), m_tOpenNodes.end(), pNeighbour) != m_tOpenNodes.end();
                    if ( !bInOpen || pNeighbour->m_fG > fNewG)
                    {
                        pNeighbour->m_fG = fNewG;
                        pNeighbour->CalculateF();
                        pNeighbour->pParent = pCurrent;
                        if (!bInOpen)
                        {
                            m_tOpenNodes.push_back(pNeighbour);
                        }
                    }
                }
            }
        }
        std::vector<Polygon*> tPolygons;
        while (pCurrent->pParent != nullptr)
        {
            tPolygons.push_back(pCurrent);
            pCurrent = pCurrent->pParent;           
        }
        tPolygons.push_back(pCurrent);
        std::reverse(std::begin(tPolygons), std::end(tPolygons));
        m_tPoints.push_back(_vStart);
        for(int i = 1; i< tPolygons.size(); i++)
        {
            pCurrent = tPolygons[i];
            for (int j = 0; j < pCurrent->m_tEdge.size(); j++)
            {
                Edge* pEdge = pCurrent->m_tEdge[j];
                if (pEdge->m_pNeighbour == pCurrent->pParent)
                {
                    USVec2D vIntersection = LinesIntersection(pEdge->FirstVertex(pCurrent), pEdge->SecondVertex(pCurrent), m_tPoints[m_tPoints.size() - 1], _vEnd);
                    USVec2D vNear = ClosestPointInSegment(pEdge->FirstVertex(pCurrent), pEdge->SecondVertex(pCurrent), vIntersection);
                    m_tPoints.push_back(vNear);
                }
            }
        }
        m_tPoints.push_back(_vEnd);
    }
}