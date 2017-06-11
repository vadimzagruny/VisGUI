#pragma once

struct SETWEATHER;

class IMainView;
class CWorld;
class GraphicPort;
class CComponent;
class CParticleSystem;
class CParticleEmmiter;

/*!
*  \brief     Presenter Interface Class
*  \details   This class is used to Hide CPresenter class implementation.
*  \author    Vadim Zagruny
*  \version   2.1
*  \date      2017
*  \copyright  Vadim Zagruny KhMBDB .
*/
class IPresenter
{
	
public:

	//! IPresenter constructor.
	IPresenter(){}
	//! IPresenter destructor.
	/*!
	A more elaborate description of the constructor.
	*/
	~IPresenter(){}

	//! Init pure virtual member for initialize Presenter Object.
	/*!
	\param IMainView pointer to MainFrame Interface  argument.
	\param CWorld* Engine Core pointer.
	\return true value if no errors 
	\sa Init(IMainView*, CWorld*, GraphicPort*)
	*/
	virtual bool Init(IMainView*, CWorld*, GraphicPort*) = 0;
	
	//! Destroy pure virtual member Relese Presenter object.
	/*!
	\return true value if no errors 
	\sa Destroy()
	*/
	virtual bool Destroy() = 0;
	
	//! Update pure virtual member  Updating Presenter object.
	/*!
	\param  fdeltaTime is a float argument.
	\return true value if no errors
	\sa Update(float fdeltaTime)
	*/
	virtual bool Update(float) = 0;

	//! CreateDevice pure virtual member Creating DirectX Device.
	/*!
	\param  hWnd is a window handler (HWND) argument.
	\return true value if no errors
	\sa CreateDevice(HWND hWnd)
	*/
	virtual bool CreateDevice(HWND) = 0;

	//! CreateDevice pure virtual member starts rendering.
	/*!
	\return true value if no errors
	\sa Paint()
	*/
	virtual bool Paint() = 0;

	//! LoadComponent pure virtual member loading component which represent 3D model in scene.
	/*!
	\return true value if no errors
	\sa LoadComponent()
	*/
	virtual bool LoadComponent() = 0;

	//! DeleteComponent pure virtual member deleting component which represent 3D model in scene.
	/*!
	\param  name is a string class argument representing components name.
	\return true value if no errors
	\sa DeleteComponent(string name)
	*/
	virtual bool DeleteComponent(string name) = 0;

	//! Pure virtual member for copy/paste component which represent 3D model in scene.
	/*!
	\param  name is a string class argument representing components name.
	\return true value if no errors
	\sa DeleteComponent(string name)
	*/
	virtual bool PasteComponent(string name) = 0;

	//! ResetScene pure virtual member deleting all components in scene.
	/*!
	\return true value if no errors
	*/
	virtual bool ResetScene() = 0;

	//! pure virtual member setting component properties.
	/*!
	\param  type is a DWORD argument representing parameter type.
	\param  pData is a void  Data pointer argument.
	\return true value if no errors
	*/
	virtual bool SetComponentProperty(DWORD type,void *pData) = 0;

	//! pure virtual member setting component parent.
	/*!
	\param  ParentName is a string class argument representing parent name.
	\param  ChildName is a string class argument representing Child name.
	\return true value if no errors
	*/
	virtual bool SetComponentParent(string ParentName, string ChildName) = 0;
	virtual bool SetMaterialProperty() = 0;

	//! pure virtual member setting component parent.
	/*!
	\param  setWeather is a SETWEATHER struct pointer representing weather params.
	\return true value if no errors
	*/
	virtual bool SetWeather(SETWEATHER*setWeather) = 0;

	
	virtual bool LoadTargetData(string) = 0;

	//! pure virtual member setting component parent.
	/*!
		The member function create XML file with target configs 
	\return true value if no errors
	*/
	virtual bool SaveTargetData(string) = 0;


	//ParticleSystem
	virtual bool CreateParticleSystem(string name) = 0;
	virtual bool ReleaseParticleSystem(string name) = 0;

	virtual bool SetParticleSystemMaterial(string name) = 0;
	virtual bool SetParticleSystemStartColor(D3DXVECTOR3 startColor) = 0;
	virtual bool SetParticleSystemEndColor(D3DXVECTOR3 endColor) = 0;
	virtual bool SetParticleSysProperty(DWORD type, void *pData) = 0;

	virtual bool GetParticleSysProperty(DWORD type, void *pData) = 0;



	virtual void SelectComponent(string name) = 0;
	virtual void SelectParticleSys(string name) = 0;
};


class CPresenter : public IPresenter
{

	CComponent *m_pCurrentComponent;

	list<CParticleEmmiter*>	m_listParticleEmmiter;
	CParticleEmmiter	*m_pCurrentParticleEmmiter;

	CWorld		*m_pWorld;			//Окружающий мир
	GraphicPort	*m_pGrPort;			//Графика
	IMainView	*m_pIMainFrame;

	CParticleEmmiter*GetParticleEmmiter(string &name);

public:
	CPresenter();
	~CPresenter();

	CWorld	*GetWorld()	{return m_pWorld;}


	bool Init(IMainView*, CWorld*, GraphicPort*);
	bool Update(float timeDelta);
	bool Destroy();
	bool CreateDevice(HWND);
	bool Paint();

	bool LoadComponent();
	bool PasteComponent(string name);
	bool DeleteComponent(string name);
	bool CreateParticleSystem(string name);
	bool ReleaseParticleSystem(string name);
	bool SetParticleSysProperty(DWORD type, void *pData);
	bool GetParticleSysProperty(DWORD type, void *pData);
	bool ResetScene();
	bool SetComponentProperty(DWORD type, void *pData);
	bool SetComponentParent(string ParentName, string ChildName);
	bool SetMaterialProperty();
	bool SetWeather(SETWEATHER*);
	void SelectComponent(string name);
	void SelectParticleSys(string name);

	void GetComponentMaterials();

	bool LoadTargetData(string);
	bool SaveTargetData(string);

	//Particles

	bool SetParticleSystemStartColor(D3DXVECTOR3 startColor);
	bool SetParticleSystemEndColor(D3DXVECTOR3 endColor);
	bool SetParticleSystemMaterial(string name);

	
};
