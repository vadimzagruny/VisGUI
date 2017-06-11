#include "stdafx.h"
#include "SpriteContainer.h"
#include "StrategyParticleSystem.h"


#ifdef _DETECT_MEMORY_LEAKS

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif // _DETECT_MEMORY_LEAKS


IMPLEMENT_CLASS(CStrategyParticleSystem);

// Инициализация цвета. Произвольный.
struct remInitColor : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime)
	{
		D3DXVECTOR4 col = D3DXVECTOR4(1.0f / (float)(rand() % 10), 1.0f / (float)(rand() % 10),1.0f / (float)(rand() % 10), 0.7f);
			
		P->iColor =  P->iColor	| (((int)(255))<<24) |(((int)(col.x * 255))<<16) |(((int)(col.y * 255))<<8 )|(((int)(col.z * 255))<<0) ;
	}
};

// Инициализация размера
struct remInitSize : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime)
	{
		P->fStartSize = 0.2f;
		P->fStartSize = 0.2f;
	}
};

// Инициализация жизни.1000
struct remInitLife_Smoke : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime)
	{
		P->fTimeLife = 1000;
		P->fCurrentTime = 1000;
	}
};

// Инициализация скорости для взрыва. Во все стороны. 
struct remInitVel_Explosion : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime)
	{
		P->Velocity = D3DXVECTOR3(10 - (float)(rand() % 200) / 10,
			10 - (float)(rand() % 200) / 10,
			10 - (float)(rand() % 200) / 10);

	}
};

// Инициализация скорости для дыма. Вверх.
struct remInitVel_Smoke : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime)
	{
		P->Velocity = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	}
};


// Обработчик позиции дыма. Добавляем немного тряски в горизонтальной 
// плоскости
struct remProcPos_Noise : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime)
	{
		D3DXVECTOR3 g((float)(5 - rand() % 10),0.0f,	(float)(5 - rand() % 10));
			
		
		g += P->Velocity;
		g.x *= FrameTime;
		g.y *= FrameTime;
		g.z *= FrameTime;
		P->Position = P->Position + g;
	}
};

// Просто линейное приращение скорости к текущей позиции
// Для взрыва вполне подходит
struct remProcPos_Linear : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime)
	{
		D3DXVECTOR3 g = P->Velocity;
		g.x *= FrameTime;
		g.y *= FrameTime;
		g.z *= FrameTime;
		P->Position = P->Position + g;
	}
};

// И не забыть уменьшить жизнь частицам.
struct remProcLife : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime)
	{
		P->fCurrentTime += FrameTime;

		float fPercentLife = (P->fTimeLife - P->fCurrentTime) / P->fTimeLife;
		float fTranspSprite = P->fStartTransparency * fabs(fPercentLife) ;

		P->iColor = 0x00FFFFFF | ((UINT)(fTranspSprite * 255))<< 24;

	}
};


// Пустышка
struct remNullPolicy : public IStrategyType
{
	inline void operator () (CommonParticle *P, double FrameTime) {}
};


class CParticleProcSet 
{

	list<IStrategyType*>m_StrategyList;
public:
	
	~CParticleProcSet(){
		for each(auto it in m_StrategyList){
			delete it;
		}
		m_StrategyList.clear(); 
		
	}


	inline void operator () (CommonParticle *P, double FrameTime)
	{
		for each(auto it in m_StrategyList){
			(*it)(P, FrameTime);
		}
	}

	void AddStrategy(IStrategyType*pStrategyType){ m_StrategyList.push_back(pStrategyType); }
};


CStrategyParticleSystem::CStrategyParticleSystem()
{
	Alive = true;
	m_pSpriteContainer = NULL;
}


CStrategyParticleSystem::~CStrategyParticleSystem()
{
	if (!m_ParticleList.empty())
		m_ParticleList.clear();

	theApp.GetSharedResource()->ReleaseResource(m_pSpriteContainer);

	//delete pInitCompositor		;
	delete pUpdateProcSet;
}

void CStrategyParticleSystem::Emit(CommonParticle particle,  UINT Cnt)
{
	//srand((UINT)TickTime());
	UINT tC = CurrCount;

	

	// Инициализируем каждую частицу
	//(*pInitCompositor)(&(particle), a);
	m_ParticleList.push_back(particle);

}

void CStrategyParticleSystem::Update(double FrameTime)
{
	if (Alive)
	{
		//for (i = 0; i < CurrCount; i++)
		//{
		//	// Particle dead. Switch with last
		//	if (PArr[i].Life <= 0)
		//	{
		//		PArr[i] = PArr[--CurrCount];
		//	}
		//	if (CurrCount == 0) Alive = false;
		//}

		list<CommonParticle>::iterator iter = m_ParticleList.begin();

		while(iter != m_ParticleList.end()){

			CommonParticle *particle = (CommonParticle*)(&(*iter));
			if(particle->fCurrentTime > particle->fTimeLife)
			{
				iter = m_ParticleList.erase(iter);
				continue;
			}
			iter++;

			// Обрабатываем (обновляем) каждую частицу
			(*pUpdateProcSet)(&(*particle), FrameTime);


			SPRITE sprite;
		
			sprite.pos				= D3DXVECTOR4((particle)->Position,1);
			sprite.fAngle			= 0.0f;
			sprite.fSize			= particle->fCurrentTime*1;
			sprite.iColor			= particle->iColor;//(particle)->Color.x;//| (((int)(fTranspSprite * 255)) << 24);
		

			m_pSpriteContainer->Push(sprite);

		}


		//if (m_ParticleList.size() == 0)Alive = false;
	}
	else CurrCount = 0;
}

void CStrategyParticleSystem::AddStrategy(IStrategyType*pUpdateStrategyType)
{
	/*if(pInitStrategyType)
	pInitCompositor->AddStrategy(pInitStrategyType);*/

	if(pUpdateStrategyType)
		pUpdateProcSet->AddStrategy(pUpdateStrategyType);
}
void CStrategyParticleSystem::AddParticle(CommonParticle*pParticle)
{
	m_ParticleList.push_back(*pParticle);
}

bool CStrategyParticleSystem::Init()
{
	m_pSpriteContainer = (CSpriteContainer*)(theApp.GetSharedResource()->QueryResource(string("dust_CSpriteContainer"), string("CSpriteContainer")));
	ASSERT(m_pSpriteContainer);
	m_pSpriteContainer->Cached(false);

	//pInitCompositor		= new remCompletePolicy;
	pUpdateProcSet	= new CParticleProcSet;


	/*remInitColor			*pInitColor			= new remInitColor		;
	remInitSize				*pInitSize			= new remInitSize		;
	remInitLife_Smoke		*pInitLife_Smoke	= new remInitLife_Smoke	;
	remInitVel_Explosion 	*pInitVel_Explosion	= new remInitVel_Explosion;
	remInitVel_Smoke		*pInitVel_Smoke		= new remInitVel_Smoke	;*/

	remProcPos_Noise		*pProcPos_Noise		= new remProcPos_Noise	;
	remProcPos_Linear 		*pProcPos_Linear 	= new remProcPos_Linear ;
	remProcLife				*pProcLife			= new remProcLife		;

	//AddStrategy(pInitVel_Smoke,0);
	AddStrategy(pProcPos_Noise);
	AddStrategy(pProcLife);
	

	/*AddStrategy(pInitLife_Smoke,			0);
	AddStrategy(pInitSize,			0);
	AddStrategy(pInitColor,			pProcPos_Linear);
	AddStrategy(pInitVel_Explosion,	pProcLife);*/
	
	

	return true;
}
