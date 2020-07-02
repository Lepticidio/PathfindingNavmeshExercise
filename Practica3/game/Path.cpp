#include "stdafx.h"
#include "Path.h"
#include "Navmesh.h"
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
        m_tPoints.push_back(_vEnd);
        while (pCurrent->pParent != nullptr)
        {
            for (int i = 0; i < pCurrent->m_tEdge.size(); i++)
            {
                Edge* pEdge = pCurrent->m_tEdge[i];
                if (pEdge->m_pNeighbour == pCurrent->pParent)
                {
                    m_tPoints.push_back(pEdge->MiddlePoint(pCurrent));
                    pCurrent = pCurrent->pParent;
                }
            }
        }
        m_tPoints.push_back(_vStart);
        std::reverse(std::begin(m_tPoints), std::end(m_tPoints));
    }
}