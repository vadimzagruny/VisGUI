// World.h: interface for the CWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_WORLD_H__INCLUDED_)
#define _WORLD_H__INCLUDED_


class CBase;
class CLand;
class CComponent;
class CWeather;
class CStrategyParticleSystem;
class CParticleSystem;

struct SETWEATHER;

class CWorld  
{
protected:

	CNetObject	*m_pInputPort;

	list<CNetObject *>		m_listObjects; 
	list<CNetObject *>		m_listTagets; 
	list<CNetObject *>		m_listDebugObjects;
	list<CParticleSystem *>	m_listParticleSystem;
	list<CComponent	*>		m_listComponents;

	D3DXMATRIX	m_matTransform;
	
	CWeather	*m_pWeather;

	CLand *m_pLand;
	char *m_pKeyBuffer;

	enum EnObjType{eCOMPONENT,ePARTICLE_SYSTEM};

	VOID CheckName(string &name, EnObjType type);

public:
	CWorld();
	virtual ~CWorld();

	BOOL Create(HWND);
	BOOL Update(float fDeltaTime);
	BOOL Destroy();

	CParticleSystem* CreateParticleSystem(string &name);

	VOID ReleaseParticleSystem(string name);
	VOID SaveTargetData(string name);
	

	BOOL LoadModel(string &name);
	BOOL ReleaseModel();
	
	BOOL ReleaseComponent(string name);

	CComponent	*GetModel(string name);

	VOID SetWeather(SETWEATHER *pSetWeather);
};

#endif // !defined(_WORLD_H__INCLUDED_)
