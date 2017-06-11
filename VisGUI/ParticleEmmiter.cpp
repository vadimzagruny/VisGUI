#include "stdafx.h"
#include "ParticleEmmiter.h"


CParticleEmmiter::CParticleEmmiter(CParticleSystem*pPatricleSystem)
{
	m_pPatricleSystem = pPatricleSystem;

	m_ParticleCfg.Position				= D3DXVECTOR3(0,0,0);
	m_ParticleCfg.Velocity				= D3DXVECTOR3(0,0.5f,0);
	m_ParticleCfg.fExpKoefVelAtten		= 0.0f;
	m_ParticleCfg.fTimeEvalExpKoef		= 0.0f;
	m_ParticleCfg.fTimeLife				= 2.0f;
	m_ParticleCfg.fCurrentTime			= 0.0f;
	m_ParticleCfg.fStartSize			= 0.2f;
	m_ParticleCfg.fEndSize				= 1.5f;
	m_ParticleCfg.fStartTransparency	= 0.5f;
	m_ParticleCfg.fEndTransparency		= 0.0f;
	m_ParticleCfg.fAngularVelocity		= 0.5f;
	m_ParticleCfg.fCurrentAngle			= 0.0f;
	m_ParticleCfg.iColor				= 0x00FFFFFF;


}


CParticleEmmiter::~CParticleEmmiter()
{
}


void CParticleEmmiter::Emit(float fDeltaTime)
{
	if (m_fCurrentTime > m_fEmitTime && !m_bLooped)
		return;
	else{
		float fRand = ((float)(RAND_MAX / 2 - rand())) / RAND_MAX;
		m_ParticleCfg.fCurrentAngle = fRand * 6.28f;

		m_pPatricleSystem->AddParticle(m_ParticleCfg);
	}

	m_fCurrentTime += fDeltaTime;
}