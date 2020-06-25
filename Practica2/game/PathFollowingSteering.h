#pragma once
#include "Path.h"
#include "SeekSteering.h"
class PathFollowingSteering
{
protected:


	USVec2D m_vClosestPoint = USVec2D(0, 0);
	USVec2D m_vDestination = USVec2D(0, 0);
	float m_fLookAhead = 0;
	float m_fTimeAhead = 0;
	float m_fDestinationRadius = 0;
	Character* m_pCharacter;
public:
	SeekSteering* m_pSeek;
	Path* m_pPath;
	PathFollowingSteering(SeekSteering* _pSeek, Path* _pPath, Character* _pCharacter);
	USVec2D GetSteering();
	void Initialize();
	void DrawDebug();
};

