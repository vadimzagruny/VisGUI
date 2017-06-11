#pragma once

#include "BaseResource.h"

class CSpriteContainer;

struct CommonParticle
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Velocity;
	float		fExpKoefVelAtten;
	float		fTimeEvalExpKoef;
	float		fTimeLife;
	float		fCurrentTime;
	float		fStartSize;
	float		fEndSize;
	float		fStartTransparency;
	float		fEndTransparency;
	float		fAngularVelocity;
	float		fCurrentAngle;
	unsigned int iColor;

	CommonParticle()
	{
		memset(this,0,sizeof(CommonParticle));
		fTimeEvalExpKoef = -1.0f;
	}

	CommonParticle& operator = (CommonParticle& src)
	{
		Position			= src.Position;
		Velocity			= src.Velocity;
		fExpKoefVelAtten	= src.fExpKoefVelAtten;
		fTimeEvalExpKoef	= src.fTimeEvalExpKoef;
		fTimeLife			= src.fTimeLife;
		fCurrentTime		= src.fCurrentTime;
		fStartSize			= src.fStartSize;
		fEndSize			= src.fEndSize;
		fStartTransparency	= src.fStartTransparency;
		fEndTransparency	= src.fEndTransparency;
		fAngularVelocity	= src.fAngularVelocity;
		fCurrentAngle		= src.fCurrentAngle;
		iColor				= src.iColor;

		return *this;
	}
};

class CParticleProcSet;

class IStrategyType
{
public:
	inline virtual void operator () (CommonParticle *P, double FrameTime) = 0;
};


class CStrategyParticleSystem: public CBaseResource
{
	DECLARE_CLASS(CStrategyParticleSystem);

	CSpriteContainer	*m_pSpriteContainer;

	//CParticleProcSet    *pInitCompositor;
	CParticleProcSet    *pUpdateProcSet;
	
	list<CommonParticle>m_ParticleList;
	
	UINT      CurrCount;
	UINT      i;
	bool      Alive;

public:
	CStrategyParticleSystem();
	~CStrategyParticleSystem();

	bool Init();

	void Emit (CommonParticle particle,UINT Cnt = 1);
	void Update(double FrameTime);

	void AddStrategy(IStrategyType*pUpdateStrategyType);
	void AddParticle(CommonParticle*pParticle);

};

