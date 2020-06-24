#include "stdafx.h"
#include "Path.h"
Path::Path(USVec2D _vStart, USVec2D _vEnd) : m_vStart(_vStart), m_vEnd(_vEnd)
{

}
void Path::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

}