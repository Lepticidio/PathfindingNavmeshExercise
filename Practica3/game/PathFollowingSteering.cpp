#include "stdafx.h"
#include "PathFollowingSteering.h"
#include "character.h"


USVec2D ClosestPointInSegment(USVec2D _vASegment, USVec2D _vBSegment, USVec2D _vPoint)
{
	USVec2D vV = _vBSegment - _vASegment;
	USVec2D vU = _vASegment - _vPoint;

	float fT = - vV.Dot(vU) / vV.Dot(vV);

	if (fT < 0)
	{
		fT = 0;
	}
	else if (fT > 1)
	{
		fT = 1;
	}

	return USVec2D((1 - fT) * _vASegment.mX, (1 - fT)* _vASegment.mY) +  USVec2D( fT * _vBSegment.mX, fT* _vBSegment.mY);
}

PathFollowingSteering::PathFollowingSteering(SeekSteering* _pSeek, Path* _pPath, Character* _pCharacter) : m_pSeek(_pSeek), m_pPath(_pPath), m_pCharacter(_pCharacter)
{
}

USVec2D PathFollowingSteering::GetSteering()
{
	int iClosestDestPointIndex = 1;
	float fClosestDistance = FLT_MAX;
	for (int i = 1; i < m_pPath->m_tPoints.size(); i++)
	{
		USVec2D vPointA = m_pPath->m_tPoints[i - 1];
		USVec2D vPointB = m_pPath->m_tPoints[i];
		USVec2D vClosestPointSegment = ClosestPointInSegment(vPointA, vPointB, m_pCharacter->GetLoc());
		float fDistance = (vClosestPointSegment - m_pCharacter->GetLoc()).Length();
		if ( fDistance < fClosestDistance)
		{
			m_vClosestPoint = vClosestPointSegment;
			fClosestDistance = fDistance;
			iClosestDestPointIndex = i;
		}
	}
	


	float fPreviewedDistance = 0;
	if (m_pPath->m_tPoints.size() < 2)
	{
		m_vDestination = m_pCharacter->GetLoc();
	}
	else
	{
		m_vDestination = m_vClosestPoint;
		for (int i = iClosestDestPointIndex; i < m_pPath->m_tPoints.size(); i++)
		{
			USVec2D vDestSegmentPoint = m_pPath->m_tPoints[i];
			float fDistanceToSegmentEnd = (vDestSegmentPoint - m_vDestination).Length();
			if (fPreviewedDistance + fDistanceToSegmentEnd <= m_fLookAhead)
			{
				fPreviewedDistance += fDistanceToSegmentEnd;
				m_vDestination = m_pPath->m_tPoints[i];
			}
			else
			{
				USVec2D vDirection = m_pPath->m_tPoints[i] - m_vDestination;
				vDirection.SetLength(m_fLookAhead - fPreviewedDistance);

				m_vDestination += vDirection;
				break;
			}
		}

	}

	return m_pSeek->GetSteering(m_vDestination);
	
}

void PathFollowingSteering::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	USVec2D vLoc2D = USVec2D(m_pCharacter->GetLoc().mX, m_pCharacter->GetLoc().mY);

	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine( vLoc2D, m_vDestination);

	gfxDevice.SetPenColor(1.0f, 0.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(vLoc2D, m_vClosestPoint);

}
void PathFollowingSteering::Initialize()
{
	m_fLookAhead = m_pCharacter->m_fLookAhead;
	m_fTimeAhead = m_pCharacter->m_fTimeAhead;
	//m_fDestinationRadius = m_pCharacter->GetParams().dest_radius;
}