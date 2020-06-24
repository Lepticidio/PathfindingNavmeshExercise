#include "stdafx.h"
#include "Polygon.h"
Polygon::Polygon()
{

}
void Polygon::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    gfxDevice.SetPenColor(0.0f, 0.5f, 1.0f, 0.5f);
    MOAIDraw::DrawPolygon(m_tVertex);
    gfxDevice.SetPenColor(0.0f, 0.5f, 1.0f, 0.05f);
    MOAIDraw::DrawPolygonFilled(m_tVertex);
}