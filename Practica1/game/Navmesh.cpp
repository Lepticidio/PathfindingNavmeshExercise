#include "stdafx.h"
#include "Navmesh.h"

Navmesh::Navmesh(const char* filename)
{
    TiXmlDocument doc(filename);
    if (!doc.LoadFile())
    {
        fprintf(stderr, "Couldn't read params from %s", filename);

    }
    TiXmlHandle hDoc(&doc);
    TiXmlElement* pElem;
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem)
    {
        fprintf(stderr, "Invalid format for %s", filename);

    }
    TiXmlHandle hRoot(pElem);
    TiXmlHandle hPolygons = hRoot.FirstChildElement("polygons");

    TiXmlElement* xPolygon = hPolygons.FirstChildElement("polygon").Element();
    while (xPolygon)
    {
        Polygon polygon;
        TiXmlElement* xPoint = xPolygon->FirstChildElement("point")->ToElement();
        while (xPoint)
        {
            USVec2D* pPoint = new USVec2D(0, 0);
            float fX = 0;
            float fY = 0;
            xPoint->Attribute("x", &fX);
            xPoint->Attribute("y", &fY);
            pPoint->mX = fX;
            pPoint->mY = fY;
            polygon.m_tVertex.push_back(*pPoint);

            TiXmlNode* xPointNode = xPoint->NextSibling();;
            if (xPointNode == nullptr)
            {
                xPoint = nullptr;
            }
            else
            {
                xPoint = xPointNode->ToElement();
            }

        }
        m_tPolygons.push_back(polygon);

        TiXmlNode* xPolygonNode = xPolygon->NextSibling();;
        if (xPolygonNode == nullptr)
        {
            xPolygon = nullptr;
        }
        else
        {
            xPolygon = xPolygonNode->ToElement();
        }
    }
    TiXmlNode* xPolygonsNode = hPolygons.ToNode();
    TiXmlNode* xLinksNode = xPolygonsNode->NextSibling();


    TiXmlElement* xLink = xLinksNode->FirstChildElement()->ToElement();
    while (xLink)
    {
        TiXmlElement* xEdge = xLink->FirstChildElement("start")->ToElement();
        Polygon* pPreviousPolygon = nullptr;
        Edge* pPreviousEdge = nullptr;
        while (xEdge)
        {
            Edge* pEdge = new Edge();
            int iPolygonIndex;
            xEdge->Attribute("polygon", &iPolygonIndex);
            xEdge->Attribute("edgestart", &pEdge->m_verts[0]);
            xEdge->Attribute("edgeend", &pEdge->m_verts[1]);
            Polygon* pPolygon = &m_tPolygons[iPolygonIndex];
            pPolygon->m_tEdge.push_back(pEdge);
            if (pPreviousPolygon != nullptr && pPreviousEdge != nullptr)
            {
                pEdge->m_pNeighbour = pPreviousPolygon;
                pPreviousEdge->m_pNeighbour = pPolygon;

            }
            printf("something");
            pPreviousEdge = pEdge;
            pPreviousPolygon = pPolygon;

            TiXmlNode* xEdgeNode = xEdge->NextSibling();;
            if (xEdgeNode == nullptr)
            {
                xEdge = nullptr;
            }
            else
            {
                xEdge = xEdgeNode->ToElement();
            }

        }

        TiXmlNode* xLinkNode = xLink->NextSibling();;
        if (xLinkNode == nullptr)
        {
            xLink = nullptr;
        }
        else
        {
            xLink = xLinkNode->ToElement();
        }
    }
}
void Navmesh::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
    gfxDevice.SetPenWidth(4);
    gfxDevice.SetPenColor(0.0f, 0.5f, 1.0f, 0.5f);
    for (int i = 0; i < m_tPolygons.size(); i++)
    {
        m_tPolygons[i].DrawDebug();
    }
}