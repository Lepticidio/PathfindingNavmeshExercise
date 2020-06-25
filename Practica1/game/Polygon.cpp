#include "stdafx.h"
#include "Polygon.h"

bool OnSegment(USVec2D _vP, USVec2D _vQ, USVec2D _vR)
{
    if (_vQ.mX <= Max(_vP.mX, _vR.mX) && _vQ.mX >= Min(_vP.mX, _vR.mX) &&
        _vQ.mY <= Max(_vP.mY, _vR.mY) && _vQ.mY >= Min(_vP.mY, _vR.mY))
    {
        return true;
    }
    return false;
}
int Orientation(USVec2D _vP, USVec2D _vQ, USVec2D _vR)
{
    float val = (_vQ.mY - _vP.mY) * (_vR.mX - _vQ.mX) -
        (_vQ.mX - _vP.mX) * (_vR.mY - _vQ.mY);

    if (val == 0)
    {
        return 0;
    }
    // colinear 
    return (val > 0) ? 1 : 2; // clock or counterclock wise 
}
bool DoIntersect(USVec2D _vP1, USVec2D _vQ1, USVec2D _vP2, USVec2D _vQ2)
{
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = Orientation(_vP1, _vQ1, _vP2);
    int o2 = Orientation(_vP1, _vQ1, _vQ2);
    int o3 = Orientation(_vP2, _vQ2, _vP1);
    int o4 = Orientation(_vP2, _vQ2, _vQ1);

    // General case 
    if (o1 != o2 && o3 != o4)
    {
        return true;

    }
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && OnSegment(_vP1, _vP2, _vQ1)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && OnSegment(_vP1, _vQ2, _vQ1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && OnSegment(_vP2, _vP1, _vQ2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && OnSegment(_vP2, _vQ1, _vQ2)) return true;

    return false; // Doesn't fall in any of the above cases 
}

Polygon::Polygon() : m_fG (INFINITY)
{

}
void Polygon::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    gfxDevice.SetPenColor(0.0f, 0.5f, 1.0f, 0.5f);
    MOAIDraw::DrawPolygon(m_tVertex);
    gfxDevice.SetPenColor(0.0f, 0.5f, 1.0f, 0.1f);
    MOAIDraw::DrawPolygonFilled(m_tVertex);
    for (int i = 0; i < m_tVertex.size(); i++)
    {

        float fBrightness = (float)i / (float)m_tVertex.size();
        gfxDevice.SetPenColor(fBrightness, fBrightness, fBrightness, 1.f);
        MOAIDraw::DrawEllipseFill(m_tVertex[i].mX, m_tVertex[i].mY, 5, 5, 16);

 
    }
    
}

bool Polygon::PointInPolygon(USVec2D _vPoint)
{

    // Create a point for line segment from p to infinite 
    USVec2D vExtreme ( 3000, _vPoint.mY );

    // Count intersections of the above line with sides of polygon 
    int iCount = 0, i = 0;
    do
    {
        int iNext = (i + 1) % m_tVertex.size();

        // Check if the line segment from 'p' to 'extreme' intersects 
        // with the line segment from 'polygon[i]' to 'polygon[next]' 
        //MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

        //gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
        //MOAIDraw::DrawLine(_vPoint, vExtreme);
        //gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
        //MOAIDraw::DrawLine(m_tVertex[i], m_tVertex[iNext]);
        if (DoIntersect(m_tVertex[i], m_tVertex[iNext], _vPoint, vExtreme))
        {
            // If the point 'p' is colinear with line segment 'i-next', 
            // then check if it lies on segment. If it lies, return true, 
            // otherwise false 
            if (Orientation(m_tVertex[i], _vPoint, m_tVertex[iNext]) == 0)
            {
                return OnSegment(m_tVertex[i], _vPoint, m_tVertex[iNext]);
            }
            iCount++;
        }
        i = iNext;
    } while (i != 0);

    // Return true if count is odd, false otherwise 
    return iCount %2 == 1; 
}
USVec2D  Polygon::AveragePoint()
{
    float fTotalX = 0, fTotalY = 0;
    int iSize = m_tVertex.size();
    for (int i = 0; i < iSize; i++)
    {
        fTotalX += m_tVertex[i].mX;
        fTotalY += m_tVertex[i].mY;
    }

    return USVec2D(fTotalX / (float)iSize, fTotalY / (float)iSize);
}
void Polygon::CalculateH(USVec2D _vEndPoint)
{
    m_fH = (_vEndPoint - AveragePoint()).Length();
}
void Polygon::CalculateF()
{
    m_fF = m_fG + m_fH;
}
