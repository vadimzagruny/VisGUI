// ParticleSystem.cpp: implementation of the CParticleSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "ObjMan.h"


#include "FrameTimer.h"
#include "GraphicPort.h"
#include "SharedResource.h"
#include "VisGUI.h"
#include "SpriteContainer.h"
#include "ParticleSystem.h"


#include "ObjMan.h"
#include "clients.h"
#include "weather.h"


#ifdef _DETECT_MEMORY_LEAKS

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif // _DETECT_MEMORY_LEAKS


IMPLEMENT_CLASS(CParticleSystem);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParticleSystem::CParticleSystem()
{
	m_fG = 9.8f;
	m_fCurrentTime = 0.f;
	m_fEmitTime = 2.0f;
	m_fCoeffWindInfluence = 1.0f;

	m_bLooped = true;

	m_pSpriteContainer = NULL;
	CBaseResource::m_iPriority = -500;
#ifndef _NONETWORK
	RegisterAsUpdatableObject();
#endif
	Cached(true);
}

CParticleSystem::~CParticleSystem()
{
	if(!m_listParticle.empty())
		m_listParticle.clear();

	theApp.GetSharedResource()->ReleaseResource(m_pSpriteContainer);
#ifndef _NONETWORK
	UnregisterAsUpdatableObject();
#endif
}

void CParticleSystem::AnableLightInfluence(bool UseLightInfluence)
{
	m_pSpriteContainer->AnableLightInfluence( UseLightInfluence);
}

bool CParticleSystem::Init()
{
	int iPos = m_strName.find_first_of(string("_"));
	string name;
	name.assign(m_strName,0,iPos);
	name += "_CSpriteContainer";

	m_pSpriteContainer = (CSpriteContainer*)(theApp.GetSharedResource()->QueryResource(name,string("CSpriteContainer")));
	ASSERT(m_pSpriteContainer);
	m_pSpriteContainer->Cached(false);

	m_pGrPort = theApp.GetGraphicPort();
	return true;
}

bool CParticleSystem::ChangeMaterial(string matName)
{
	if (!m_listParticle.empty())
		m_listParticle.clear();

	theApp.GetSharedResource()->ReleaseResource(m_pSpriteContainer);
	 
	//m_strName = matName;

	int iPos = matName.find_last_of(string("_"));
	string name;
	name.assign(matName, 0, iPos);
	name += "_CSpriteContainer";

	m_pSpriteContainer = (CSpriteContainer*)(theApp.GetSharedResource()->QueryResource(name, string("CSpriteContainer")));
	ASSERT(m_pSpriteContainer);
	m_pSpriteContainer->Cached(false);

	return true;
}

bool CParticleSystem::Update(float fDeltaTime)
{
	if(m_listParticle.empty())
		return true;

	m_fCurrentTime += fDeltaTime;

	// поддержка ветра
	CWeatherInfo *pWI = GetWeatherInfo();
	D3DXVECTOR3 vWindVelocity;
	pWI->GetWindVelocity(&vWindVelocity);
	

	float fScaleSize = (60.0f / 180.0f * PI) / m_pGrPort->GetCurrentFOV();

	LISTPARTICLE::iterator iter = m_listParticle.begin();
	while(iter != m_listParticle.end())
	{
		PARTICLE *particle = (PARTICLE *)(&(*iter));
		if(particle->fTimeLife < particle->fCurrentTime)
		{
			iter = m_listParticle.erase(iter);
			continue;
		}
		iter++;
	
		float fPercentLife = (particle->fTimeLife - particle->fCurrentTime) / particle->fTimeLife;
		
		particle->fCurrentAngle += particle->fAngularVelocity * fDeltaTime;
		float fSizeSprite = particle->fStartSize * fPercentLife + particle->fEndSize * (1 - fPercentLife);
		float fTranspSprite = particle->fStartTransparency * fPercentLife + particle->fEndTransparency * (1 - fPercentLife);
		

		// Вычислить позицию и скорость частиц независимо от частоты кадров
		// с тактом не реже чем 100 раз в секунду
		#define TICK_TIME	0.01f
		particle->fCurrentTime += fDeltaTime;
		int iCountTick = (int)fDeltaTime / TICK_TIME;
		for(int i = 0; i < iCountTick; i++)
		{
			particle->Position += particle->Velocity * TICK_TIME;
			particle->Velocity += D3DXVECTOR3(0.0f,-m_fG * TICK_TIME,0.0f);
			if(particle->fTimeEvalExpKoef > particle->fCurrentTime)
			{
				particle->Velocity *= 1.0f / exp(particle->fExpKoefVelAtten * particle->fCurrentTime);
			}

			// поддержка ветра
			particle->Position += D3DXVECTOR3(-vWindVelocity.x,vWindVelocity.y,vWindVelocity.z) * m_fCoeffWindInfluence * TICK_TIME;

			particle->fCurrentTime += TICK_TIME;
		}

		float fTimeRemainder =  fDeltaTime - iCountTick * TICK_TIME;
		particle->Position += particle->Velocity * fTimeRemainder;
		particle->Velocity += D3DXVECTOR3(0.0f,-m_fG * fTimeRemainder,0.0f);
		if(particle->fTimeEvalExpKoef > particle->fCurrentTime)
		{
			particle->Velocity *= 1.0f / exp(particle->fExpKoefVelAtten * particle->fCurrentTime);
		}
		particle->fCurrentTime += fTimeRemainder;

		
		//Lerp Particle Color
		float lerpCoef = particle->fCurrentTime / particle->fTimeLife;
		D3DXVECTOR3 ResColor;
		D3DXVec3Lerp(&ResColor, &m_LerpColors.startColor, &m_LerpColors.EndColor, lerpCoef > 1.0f ? 1.0f : lerpCoef);
		particle->iColor =  (((int)(ResColor.x		* 255)) << 16)| 
							(((int)(ResColor.y		* 255)) << 8) | 
							(((int)(ResColor.z		* 255)) << 0) | 
							(((int)(fTranspSprite	* 255)) << 24);
		 
		SPRITE sprite;
		
		sprite.pos				= D3DXVECTOR4(particle->Position,1);
		sprite.fAngle			= particle->fCurrentAngle;
		sprite.fSize			= fSizeSprite;
		sprite.iColor = particle->iColor;
		

		m_pSpriteContainer->Push(sprite);
	}	

	return true;
}

bool CParticleSystem::AddParticle(CParticleSystem::PARTICLE &particle)
{
	m_listParticle.push_back(particle);
	return true;
}

void CParticleSystem::Emit(PARTICLE &particle)
{
	if (m_fCurrentTime > m_fEmitTime && !m_bLooped)
		return;
	else
		AddParticle(particle);
}

