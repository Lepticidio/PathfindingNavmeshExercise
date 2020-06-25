#include <stdafx.h>
#include "character.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f), m_fMaxSpeed(100.0f), m_fMaxAcceleration(180.0f), m_fLookAhead(80), m_fTimeAhead(4)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
	const char* sNavmeshName = "navmesh.xml";
	m_pNavmesh = new Navmesh(sNavmeshName);
	m_pPath = new Path();
	m_pSeekSteering = new SeekSteering(this);
	m_pPathSteering = new PathFollowingSteering(m_pSeekSteering, m_pPath, this);
}

Character::~Character()
{

}

void Character::OnStart()
{
	m_pSeekSteering->Initialize();
	m_pPathSteering->Initialize();

}

void Character::OnStop()
{

}

void Character::OnUpdate(float step)
{
	if (!m_bStartSetted)
	{
		m_vStartPosition = GetLoc();
		m_bStartSetted = true;
	}
	USVec2D vAcceleration = m_pPathSteering->GetSteering();
	USVec2D vCurrentVelocity = GetLinearVelocity() + vAcceleration * step;
	SetLinearVelocity(vCurrentVelocity.mX, vCurrentVelocity.mY);
	SetLoc(GetLoc() + GetLinearVelocity() * step);
}
void Character::CheckPathCreation()
{
	if (m_bEndSetted)
	{
		m_pPath->FindPath(m_pNavmesh, m_vStartPosition, m_vEndPosition);
	}
}
void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
	m_pNavmesh->DrawDebug();

	gfxDevice.SetPenColor(1.0f, 0.0f, 1.0f, 1.0f);
	if (m_pNavmesh->PointInNavmesh(m_vStartPosition))
	{
		gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 1.0f);
	}
	MOAIDraw::DrawEllipseFill(m_vStartPosition.mX, m_vStartPosition.mY, 5, 5, 16);
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 1.0f);
	if (m_pNavmesh->PointInNavmesh(m_vEndPosition))
	{
		gfxDevice.SetPenColor(1.0f, 1.0f, 0.0f, 1.0f);
	}
	MOAIDraw::DrawEllipseFill(m_vEndPosition.mX, m_vEndPosition.mY, 5, 5, 16);
	if (m_pPath != nullptr)
	{
		m_pPath->DrawDebug();
	}
	//MOAIDraw::DrawPoint(0.0f, 0.0f);
}





// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ "setStartPosition",		_setStartPosition},
		{ "setEndPosition",			_setEndPosition},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(2, 0.0f);
	self->SetLinearVelocity(pX, pY);
	return 0;	
}

int Character::_setAngularVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float angle = state.GetValue<float>(2, 0.0f);
	self->SetAngularVelocity(angle);

	return 0;
}
int Character::_setStartPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	
	self->SetStartPosition(pX, pY);
	self->CheckPathCreation();
	self->SetLoc(self->m_vStartPosition);
	self->mLinearVelocity = USVec2D(0, 0);
	return 0;
}
 
int Character::_setEndPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	
	self->SetEndPosition(pX, pY);
	self->m_bEndSetted = true;
	self->CheckPathCreation();
	self->SetLoc(self->m_vStartPosition);
	self->mLinearVelocity = USVec2D(0, 0);
	return 0;
}