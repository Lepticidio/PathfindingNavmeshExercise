#include <stdafx.h>
#include "character.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
	const char* sNavmeshName = "navmesh.xml";
	m_pNavmesh = new Navmesh(sNavmeshName);
	m_pPath = new Path();
}

Character::~Character()
{

}

void Character::OnStart()
{

}

void Character::OnStop()
{

}

void Character::OnUpdate(float step)
{
}
void Character::CheckPathCreation()
{
	if (m_bStartSetted && m_bEndSetted)
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
	MOAIDraw::DrawEllipseFill(m_vStartPosition.mX, m_vStartPosition.mY, 10, 10, 16);
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 1.0f);
	if (m_pNavmesh->PointInNavmesh(m_vEndPosition))
	{
		gfxDevice.SetPenColor(1.0f, 1.0f, 0.0f, 1.0f);
	}
	MOAIDraw::DrawEllipseFill(m_vEndPosition.mX, m_vEndPosition.mY, 10, 10, 16);
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
	self->m_bStartSetted = true;
	self->CheckPathCreation();
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
	return 0;
}