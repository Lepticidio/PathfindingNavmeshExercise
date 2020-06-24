#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <moaicore/MOAIEntity2D.h>
#include "Navmesh.h"
#include "Path.h"

class Character: public MOAIEntity2D
{
public:
    DECL_LUA_FACTORY(Character)
protected:
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnUpdate(float step);
public:
	virtual void DrawDebug();

	Character();
	~Character();
	
	void SetLinearVelocity(float x, float y) { mLinearVelocity.mX = x; mLinearVelocity.mY = y;}
	void SetAngularVelocity(float angle) { mAngularVelocity = angle;}
	
	USVec2D GetLinearVelocity() const { return mLinearVelocity;}
	float GetAngularVelocity() const { return mAngularVelocity;}
	void CheckPathCreation();
private:
	USVec2D mLinearVelocity;
	float mAngularVelocity;
	Navmesh* m_pNavmesh;
	USVec2D m_vStartPosition, m_vEndPosition;
	void SetStartPosition(float x, float y) { m_vStartPosition = USVec2D(x, y);}
	void SetEndPosition(float x, float y) { m_vEndPosition = USVec2D(x, y);}
	bool m_bStartSetted = false, m_bEndSetted = false;
	Path* m_pPath = nullptr;
	
	
	// Lua configuration
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setLinearVel(lua_State* L);
	static int _setAngularVel(lua_State* L);
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
};

#endif