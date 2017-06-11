// ParticleSystem.h: interface for the CParticleSystem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PARTICLESYSTEM_H__INCLUDED_
#define _PARTICLESYSTEM_H__INCLUDED_

#include "BaseResource.h"
class CSpriteContainer;
class CParticleSystem : public CBaseResource
{
	DECLARE_CLASS(CParticleSystem);
public:
typedef struct _PARTICLE
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

	_PARTICLE()
	{
		memset(this,0,sizeof(_PARTICLE));
		fTimeEvalExpKoef = -1.0f;
	}

	_PARTICLE& operator = (_PARTICLE& src)
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

}PARTICLE;


struct _LerpColors{
	_LerpColors(){
		startColor	= D3DXVECTOR3(1, 1, 1);
		EndColor	= D3DXVECTOR3(1, 1, 1);
	}
	D3DXVECTOR3 startColor;
	D3DXVECTOR3 EndColor;
}m_LerpColors;

typedef list<PARTICLE>	LISTPARTICLE;	

protected:
	float		 m_fG;
	float		 m_fCurrentTime;
	float		 m_fEmitTime;
	float		 m_fCoeffWindInfluence;
	LISTPARTICLE m_listParticle;

	bool		m_bLooped;

	CSpriteContainer	*m_pSpriteContainer;

	GraphicPort			*m_pGrPort;
public:
	CParticleSystem();
	virtual ~CParticleSystem();

	bool Init();
	bool Update(float fDeltaTime);

	bool ChangeMaterial(string matName);

	bool AddParticle(PARTICLE &particle);
	void Emit(PARTICLE &particle);
	void SetGravity(float g){m_fG = g;}
	float GetGravity(){return m_fG;}
	int  GetCountParticle(){return m_listParticle.size();}
	void  SetCoeffWindInfluence(float fCoeff){m_fCoeffWindInfluence = fCoeff;}

	void SetStartColor(D3DXVECTOR3 &startColor){ m_LerpColors.startColor = startColor; }
	void SetEndColor(D3DXVECTOR3 &endColor){ m_LerpColors.EndColor = endColor; }
	void GetStartColor(D3DXVECTOR3 &startColor){startColor = m_LerpColors.startColor; }
	void GetEndColor(D3DXVECTOR3 &endColor){ endColor = m_LerpColors.EndColor; }

	void SetLoopState(bool bLooped){ m_bLooped = bLooped; }
	void SetEmitTime(float fEmitTime){ m_fEmitTime = fEmitTime; }

	void AnableLightInfluence(bool UseLightInfluence);
};

#endif // _PARTICLESYSTEM_H__INCLUDED_
