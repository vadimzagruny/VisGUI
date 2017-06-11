// World.cpp: implementation of the CWorld class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ObjMan.h"
#include "clients.h"
#include "frametimer.h"
#include "Weather.h"
#include "World.h"
#include "GraphicPort.h"
#include "SharedResource.h"
#include "VisGUI.h"
#include "LandScape.h"
#include "Component.h"
#include "RTClass.h" 

#include "Camera.h"
#include "InputPort.h"
#include "ParticleSystem.h"
#include "Component.h"
#include "Land_vis.h"

#include "FileManager.h"

#ifdef _DETECT_MEMORY_LEAKS

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif // _DETECT_MEMORY_LEAKS

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorld::CWorld()
{	
	m_pWeather			= nullptr;

	D3DXMatrixIdentity(&m_matTransform);
}

CWorld::~CWorld()
{
	if(m_pWeather){
		SAFE_DELETE(m_pWeather);
	}

	for(auto iter : m_listObjects){
		SAFE_DELETE((iter));
	}
}

CComponent *CWorld::GetModel(string name)
{
	for (auto it:m_listComponents ){
		string TempName;
		it->GetName(TempName);
		if(!name.compare(TempName)){
			return it;
		}
	}

	return nullptr;
}

VOID CWorld::SaveTargetData(string name)
{
	TargetDataParser::_TATGETDATA data;

	data.ClassName = "CTarget";

	for(auto it : m_listComponents){
		TargetDataParser::_MESHDATA meshData;
		
		//Set Mesh Name
		it->GetName(meshData.MeshName);
		
		int pos = meshData.MeshName.find("__");
		if (pos > 0){
			meshData.MeshName = meshData.MeshName.substr(0, pos);
		}

		//Set Parent Name
		CComponent *pOwner = it->GetOwner();
		if(pOwner){
			pOwner->GetName( meshData.ParentName );

			int pos = meshData.ParentName.find("__");
			if (pos > 0){
				meshData.ParentName = meshData.ParentName.substr(0, pos);
			}

		}

		//Set Transform
		D3DXMATRIX mat;
		it->GetTransform(&mat);
		meshData.Rot[0] = (float)asin(mat._32);				// alpha
		meshData.Rot[1] = (float)atan2(-mat._31 , mat._33);	// beta
		meshData.Rot[2] = (float)atan2(-mat._12,mat._22);	// gamma

		meshData.Pos[0]	= mat._41;
		meshData.Pos[1]	= mat._42;
		meshData.Pos[2]	= mat._43;

		data.listMesh.push_back(meshData);
	}


	theApp.GetFileManager()->SaveTargetData(name,data);
}


BOOL CWorld::ReleaseComponent(string name)
{
	list<CComponent *> ::iterator it = m_listComponents.begin();

	while (it != m_listComponents.end()){
		string TempName;
		CComponent *pComp = (*it);
		pComp->GetName(TempName);
		if (!name.compare(TempName)){
			SAFE_DELETE(pComp);
			it = m_listComponents.erase(it);
			return true;
		}
		it++;
	}

	return false;
}


VOID CWorld::CheckName(string &name, EnObjType type)
{
	static int count = 0;

	switch (type){
		case eCOMPONENT:{
			for (auto it : m_listComponents){
				string CompName;
				it->GetName(CompName);

				if (!CompName.compare(name)){
					char c_str[200] = { 0 };
					sprintf(c_str, "__%d", count);

					int pos = name.find("__");
					if (pos > 0){
						name = name.substr(0, pos);
					}
					name += c_str;
					count++;
					CheckName(name, type);
					break;
				}
			}
		}
		break;
		case ePARTICLE_SYSTEM:
		{
			for (auto it : m_listParticleSystem){
				string CompName;
				it->GetName(CompName);

				if (!CompName.compare(name)){
					char c_str[200] = { 0 };
					sprintf(c_str, "__%d", count);

					int pos = name.find("__");
					if (pos > 0){
						name = name.substr(0, pos);
					}
					name += c_str;
					count++;
					CheckName(name, type);
					break;
				}
			}
		}
		break;
		default:
			ASSERT(0);
	}
	count = 0;
}

BOOL CWorld::LoadModel(string &name)
{

	CComponent *pComponent = new CComponent;

	if (!pComponent->Load(name)){
		delete pComponent;
		return false;
	}
	
	m_listComponents.push_back(pComponent);

	int slashPos = name.find_last_of('\\');
	int dotPos   = name.find_last_of('.');
	name = name.substr(slashPos + 1, dotPos - slashPos - 1);

	this->CheckName(name, eCOMPONENT);

	pComponent->SetName(name);
	//WCHAR wstr[255];
	//MultiByteToWideChar(CP_ACP,0,name.c_str(),-1,wstr,255);
	//::SetWindowText(theApp.GetMainWnd()->GetSafeHwnd(),wstr);
	

	return true;
}
BOOL CWorld::ReleaseModel()
{
	for (auto it:m_listComponents ){
		SAFE_DELETE(it);
	}

	for (auto it : m_listParticleSystem){
		theApp.GetSharedResource()->ReleaseResource(it);
	}

	m_listParticleSystem.clear();
	m_listComponents.clear();
	return true;
}

VOID CWorld::SetWeather(SETWEATHER *pSetWeather)
{
	if(m_pWeather)
		m_pWeather->ProcessMessage(pSetWeather);
}

CParticleSystem* CWorld::CreateParticleSystem(string &name)
{

	CheckName(name,ePARTICLE_SYSTEM);

	CParticleSystem *pParticleSystem;
	pParticleSystem = (CParticleSystem*)theApp.GetSharedResource()->QueryResource(name, string("CParticleSystem"));
	pParticleSystem->Cached(false);

	m_listParticleSystem.push_back(pParticleSystem);

	return pParticleSystem;
}

VOID CWorld::ReleaseParticleSystem(string name)
{

	list<CParticleSystem *> ::iterator it = m_listParticleSystem.begin();

	while (it != m_listParticleSystem.end()){
		string TempName;
		CParticleSystem *pSystem = (*it);
		pSystem->GetName(TempName);
		if (!name.compare(TempName)){
			theApp.GetSharedResource()->ReleaseResource(*it);
			it = m_listParticleSystem.erase(it);
			return ;
		}
		it++;
	}

	/*for (auto it : m_listParticleSystem){
		theApp.GetSharedResource()->ReleaseResource(it);
	}
	m_listParticleSystem.clear();*/
}

BOOL CWorld::Create(HWND mHWnd)
{

	// LOAD LAND
	m_pLand = new CLand;
	LOADLAND loadLand;
	strcpy(loadLand.sceneName,"TestGrid");

	m_pLand->Init();
	m_pLand->ProcessMessage(&loadLand);
	m_listDebugObjects.push_back((CNetObject *&)m_pLand);
	
	//LOAD WEATHETR
	SETWEATHER debugWeather;
	memset(&debugWeather,0,sizeof(SETWEATHER));

	debugWeather.iHour = 10;
	debugWeather.iMin  = 10;

	m_pWeather = new CWeather;
	m_pWeather->Init();
	m_pWeather->ProcessMessage(&debugWeather);

	//LOAD CAMERA
	CAMERA cameraPos;
	cameraPos.Yaw = -0.02f;
	cameraPos.Pitch = 0.09f;
	cameraPos.x = 0;
	cameraPos.y = 0.55f;
	cameraPos.z = 0;

	CCamera *pCamera = new CCamera;
	pCamera->Init();
	m_listDebugObjects.push_front((CNetObject *&)pCamera);


	// LOAD INPUTPORT
	m_pInputPort = new CInputPort;
	m_pInputPort->Init();
	((CInputPort*)m_pInputPort)->Create(theApp.GetMainWnd()->GetSafeHwnd(),pCamera);
	m_listDebugObjects.push_front((CNetObject *&)m_pInputPort);

	m_pInputPort->ProcessMessage((char *)&cameraPos);


	return TRUE;
}

BOOL CWorld::Update(float fDeltaTime)
{

	for(auto it:m_listComponents){
		it->Update(fDeltaTime);
	}
	
	if (m_pWeather){
		m_pWeather->Update(fDeltaTime);
	}

	for (auto it : m_listParticleSystem){
		it->Update(fDeltaTime);
	}

	for(auto iter : m_listDebugObjects){
		iter->Update(fDeltaTime);
	}

	m_pKeyBuffer = ((CInputPort*)m_pInputPort)->GetKeyBuffer();

	return TRUE;
}


BOOL CWorld::Destroy()
{
	list<CNetObject*>::iterator iter = m_listObjects.begin();
	while(iter != m_listObjects.end())
	{
		SAFE_DELETE((*iter));
		iter++;
	}
	m_listObjects.clear();

	list<CNetObject*>::iterator it = m_listDebugObjects.begin();
	while(it != m_listDebugObjects.end())
	{
		SAFE_DELETE((*it));
		it++;
	}
	m_listDebugObjects.clear();

	for (auto it:m_listComponents ){
		SAFE_DELETE(it);
	}
	m_listComponents.clear();

	for (auto it : m_listParticleSystem){
		theApp.GetSharedResource()->ReleaseResource(it);
	}
	m_listParticleSystem.clear();

	return TRUE;
}
