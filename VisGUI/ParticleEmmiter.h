#pragma once

#include"ParticleSystem.h"

class CParticleEmmiter
{
	CParticleSystem* m_pPatricleSystem;

	CParticleSystem::_PARTICLE m_ParticleCfg;

	float	m_fCurrentTime;
	float	m_fEmitTime;
	bool	m_bLooped;


	string m_strName;

public:
	CParticleEmmiter(CParticleSystem*pPatricleSystem);
	~CParticleEmmiter();

	void SetName(string &name){ m_strName = name; }
	void GetName(string &name){ name = m_strName; }

	CParticleSystem *GetParticeSystem(){ return m_pPatricleSystem; }

	virtual void Emit(float fDeltaTime);

	void Start(){ m_fCurrentTime = 0.0f;}
	void SetLoopState(bool bLooped){ m_bLooped = bLooped; }
	void SetEmitTime(float fEmitTime){ m_fEmitTime = fEmitTime; }


	void SetParticlePosition(D3DXVECTOR3*pvPos)					{ m_ParticleCfg.Position		 = *pvPos; }
	void SetParticleVelocity(D3DXVECTOR3*pvVel)					{ m_ParticleCfg.Velocity		 = *pvVel; }
	void SetParticlefExpKoefVelAtten(float fExpKoefVelAtten)	{ m_ParticleCfg.fExpKoefVelAtten = fExpKoefVelAtten; }
	void SetParticlefTimeEvalExpKoef(float fTimeEvalExpKoef)	{ m_ParticleCfg.fTimeEvalExpKoef = fTimeEvalExpKoef; }
	void SetParticlefTimeLife(float fTimeLife)					{ m_ParticleCfg.fTimeLife		 = fTimeLife; }
	void SetParticlefStartSize(float fStartSize)				{ m_ParticleCfg.fStartSize		 = fStartSize; }
	void SetParticlefEndSize(float fEndSize)					{ m_ParticleCfg.fEndSize		 = fEndSize; }
	void SetParticlefStartTransparency(float fStartTransparency){ m_ParticleCfg.fStartTransparency = fStartTransparency; }
	void SetParticlefEndTransparency(float fEndTransparency)	{ m_ParticleCfg.fEndTransparency = fEndTransparency; }
	void SetParticlefAngularVelocity(float fAngularVelocity)	{ m_ParticleCfg.fAngularVelocity = fAngularVelocity; }
	void SetParticlefCurrentAngle(float){}
	void SetParticleiColor(UINT iColor)							{ m_ParticleCfg.iColor = iColor; }
	void SetParticleSysGravity(float gravity)					{ m_pPatricleSystem->SetGravity( gravity); }

	D3DXVECTOR3 GetfParticlePosition()	{ return m_ParticleCfg.Position; }
	D3DXVECTOR3 GetfParticleVelocity()	{ return m_ParticleCfg.Velocity; }
	float GetfParticlefExpKoefVelAtten(){ return m_ParticleCfg.fExpKoefVelAtten; }
	float GetfParticlefTimeEvalExpKoef(){ return m_ParticleCfg.fTimeEvalExpKoef; }
	float GetfParticlefTimeLife()		{ return m_ParticleCfg.fTimeLife; }
	float GetfParticlefCurrentTime()	{ return m_ParticleCfg.fCurrentTime; }
	float GetfParticlefStartSize()		{ return m_ParticleCfg.fStartSize; }
	float GetfParticlefEndSize()		{ return m_ParticleCfg.fEndSize; }
	float GetfParticlefStartTransparency(){ return m_ParticleCfg.fStartTransparency; }
	float GetfParticlefEndTransparency(){ return m_ParticleCfg.fEndTransparency; }
	float GetfParticlefAngularVelocity(){ return m_ParticleCfg.fAngularVelocity; }
	float GetfParticlefCurrentAngle()	{ return m_ParticleCfg.fCurrentAngle; }
	UINT  GetiParticleiColor()			{ return m_ParticleCfg.iColor; }
	float GetParticleSysGravity()		{ return m_pPatricleSystem->GetGravity(); }
};

