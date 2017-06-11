#include "stdafx.h"
#include "Presenter.h"


#include "frametimer.h"
#include "VisSlave.h"
#include "World.h"
#include "GraphicPort.h"
#include "ParticleEmmiter.h"
#include "clients.h"
#include "Component.h"
#include "ParticleSystem.h"
#include "SharedResource.h"
#include "SharedShaderResource.h"
#include "Model.h"
#include "Material.h"
#include "MainFrm.h"

#include "FileManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CPresenter::CPresenter()
{
	m_pWorld						= NULL;	
	m_pGrPort						= NULL;
	m_pCurrentComponent				= NULL;
	m_pCurrentParticleEmmiter		= NULL;
	m_pIMainFrame					= NULL;	
}

CPresenter::~CPresenter()
{
	
}



bool CPresenter::Init(IMainView*pIMainFrame, CWorld*pWorld, GraphicPort*pGraphicPort)
{
	m_pGrPort		= pGraphicPort;
	m_pWorld		= pWorld;
	m_pIMainFrame	= pIMainFrame;
	
	return true;
}
bool CPresenter::Update(float fDeltaTime)
{	
	for (auto it : m_listParticleEmmiter){
		it->Emit(fDeltaTime);
	}
	
	return true;
}

bool CPresenter::Destroy()
{
	m_pWorld->Destroy();
	delete m_pWorld;

	for (auto it : m_listParticleEmmiter){
		SAFE_DELETE(it);
	}

	return true;
}

bool CPresenter::CreateDevice(HWND hWnd)
{
	theApp.GetGraphicPort()->Create(hWnd);

	m_pWorld->Create(hWnd);
	return true;
}
bool CPresenter::Paint()
{
	theApp.GetGraphicPort()->Update();
	return true;
}

bool CPresenter::DeleteComponent(string name)
{
	return m_pWorld->ReleaseComponent(name);
}
bool CPresenter::ReleaseParticleSystem(string name)
{
	m_pWorld->ReleaseParticleSystem(name);
	
	list<CParticleEmmiter*> ::iterator it = m_listParticleEmmiter.begin();

	while (it != m_listParticleEmmiter.end()){
		string TempName;
		CParticleEmmiter *pEmmiter = (*it);
		pEmmiter->GetName(TempName);
		if (!name.compare(TempName)){
			SAFE_DELETE(*it);
			it = m_listParticleEmmiter.erase(it);
			return true;
		}
		it++;
	}

	return true;
}
bool CPresenter::CreateParticleSystem(string name)
{

	CParticleSystem  *pParticleSystem = m_pWorld->CreateParticleSystem(name);
	CParticleEmmiter *pEmmiter = new CParticleEmmiter(pParticleSystem);
	pEmmiter->SetName(name);

	WCHAR CutName[300] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, name.c_str(), -1, CutName, 300);
	m_pIMainFrame->GetFileView()->AddFile(IDS_TREE_VIEW_PARTICLE_SYS_TYPE, CutName);

	m_listParticleEmmiter.push_back(pEmmiter);
	m_pCurrentParticleEmmiter = pEmmiter;

	return true;
}

bool CPresenter::SetParticleSystemStartColor(D3DXVECTOR3 startColor)
{
	if (m_pCurrentParticleEmmiter){

		m_pCurrentParticleEmmiter->GetParticeSystem()->SetStartColor(startColor);
		return true;
	}

	return false;
}
bool CPresenter::SetParticleSystemEndColor(D3DXVECTOR3 endColor)
{
	if (m_pCurrentParticleEmmiter){

		m_pCurrentParticleEmmiter->GetParticeSystem()->SetEndColor(endColor);
		return true;
	}

	return false;
}
bool CPresenter::ResetScene()
{
	m_pCurrentComponent = nullptr;

	for (auto it : m_listParticleEmmiter){
		SAFE_DELETE(it);
	}
	m_listParticleEmmiter.clear();

	m_pWorld->ReleaseModel();

	CPropertiesWnd *pPropGridWmd = m_pIMainFrame->GetPropertiesGrid();
	pPropGridWmd->HideProps();

	return true;
}

bool CPresenter::SetParticleSystemMaterial(string name)
{
	return m_pCurrentParticleEmmiter->GetParticeSystem()->ChangeMaterial(name);
}

bool CPresenter::GetParticleSysProperty(DWORD type, void *pData)
{

	switch (type){
	case IDS_PARTICLE_SYS_POS:
		*(D3DXVECTOR3*)pData	= m_pCurrentParticleEmmiter->GetfParticlePosition();
		break;
	case IDS_PARTICLE_SYS_VEL:
		*(D3DXVECTOR3*)pData	= m_pCurrentParticleEmmiter->GetfParticleVelocity();
		break;
	case IDS_PARTICLE_SYS_TIME_LIFE:
		*((float*)pData)		= m_pCurrentParticleEmmiter->GetfParticlefTimeLife();
		break;
	case IDS_PARTICLE_SYS_START_SIZE:
		*((float*)pData) = m_pCurrentParticleEmmiter->GetfParticlefStartSize();
		break;
	case IDS_PARTICLE_SYS_END_SIZE:
		*((float*)pData) = m_pCurrentParticleEmmiter->GetfParticlefEndSize();
		break;
	case IDS_PARTICLE_SYS_START_TRANSP:
		*((float*)pData) = m_pCurrentParticleEmmiter->GetfParticlefStartTransparency();
		break;
	case IDS_PARTICLE_SYS_END_TRANSP:
		*((float*)pData) = m_pCurrentParticleEmmiter->GetfParticlefEndTransparency();
		break;
	case IDS_PARTICLE_SYS_ANGULAR_VEL:
		*((float*)pData) = m_pCurrentParticleEmmiter->GetfParticlefAngularVelocity();
		break;
	case IDS_PARTICLE_SYS_GRAVITY:
		*((float*)pData) = m_pCurrentParticleEmmiter->GetParticleSysGravity();
		break;
	case IDS_PARTICLE_SYS_EXP_VEL_ATTEN:
		*((float*)pData) = m_pCurrentParticleEmmiter->GetfParticlefExpKoefVelAtten();
		break;
	case IDS_PARTICLE_SYS_TIME_EVAL_KOEF:
		*((float*)pData) = m_pCurrentParticleEmmiter->GetfParticlefTimeEvalExpKoef();
		break;
	}

	return true;
}

bool CPresenter::SetParticleSysProperty(DWORD type, void *pData)
{
	switch (type){
		case IDS_PARTICLE_SYS_POS:
			m_pCurrentParticleEmmiter->SetParticlePosition((D3DXVECTOR3*)pData);
			break;
		case IDS_PARTICLE_SYS_POS_X:{
			D3DXVECTOR3 pos = m_pCurrentParticleEmmiter->GetfParticlePosition();
			m_pCurrentParticleEmmiter->SetParticlePosition(&(D3DXVECTOR3((*(float*)pData), pos.y, pos.z)));
		}
			break;
		case IDS_PARTICLE_SYS_POS_Y:{
			D3DXVECTOR3 pos = m_pCurrentParticleEmmiter->GetfParticlePosition();
			m_pCurrentParticleEmmiter->SetParticlePosition(&(D3DXVECTOR3(pos.x, (*(float*)pData), pos.z)));
		}
			break;
		case IDS_PARTICLE_SYS_POS_Z:{
			D3DXVECTOR3 pos = m_pCurrentParticleEmmiter->GetfParticlePosition();
			m_pCurrentParticleEmmiter->SetParticlePosition(&(D3DXVECTOR3(pos.x, pos.y, (*(float*)pData))));
		}
			break;
		case IDS_PARTICLE_SYS_VEL:
			m_pCurrentParticleEmmiter->SetParticleVelocity((D3DXVECTOR3*)pData);
			break;
		case IDS_PARTICLE_SYS_VEL_X:{
			D3DXVECTOR3 vel = m_pCurrentParticleEmmiter->GetfParticleVelocity();
			m_pCurrentParticleEmmiter->SetParticleVelocity(&(D3DXVECTOR3((*(float*)pData), vel.y, vel.z)));
		}
			break;
		case IDS_PARTICLE_SYS_VEL_Y:{
			D3DXVECTOR3 vel = m_pCurrentParticleEmmiter->GetfParticleVelocity();
			m_pCurrentParticleEmmiter->SetParticleVelocity(&(D3DXVECTOR3(vel.x, (*(float*)pData), vel.z)));
		}
			break;
		case IDS_PARTICLE_SYS_VEL_Z:{
			D3DXVECTOR3 vel = m_pCurrentParticleEmmiter->GetfParticleVelocity();
			m_pCurrentParticleEmmiter->SetParticleVelocity(&(D3DXVECTOR3(vel.x, vel.y, (*(float*)pData))));
		}
			break;
		case IDS_PARTICLE_SYS_TIME_LIFE:
			m_pCurrentParticleEmmiter->SetParticlefTimeLife(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_START_SIZE:
			m_pCurrentParticleEmmiter->SetParticlefStartSize(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_END_SIZE:
			m_pCurrentParticleEmmiter->SetParticlefEndSize(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_START_TRANSP:
			m_pCurrentParticleEmmiter->SetParticlefStartTransparency(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_END_TRANSP:
			m_pCurrentParticleEmmiter->SetParticlefEndTransparency(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_ANGULAR_VEL:
			m_pCurrentParticleEmmiter->SetParticlefAngularVelocity(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_GRAVITY:
			m_pCurrentParticleEmmiter->SetParticleSysGravity(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_EXP_VEL_ATTEN:
			m_pCurrentParticleEmmiter->SetParticlefExpKoefVelAtten(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_TIME_EVAL_KOEF:
			m_pCurrentParticleEmmiter->SetParticlefTimeEvalExpKoef(*((float*)pData));
			break;
		case IDS_PARTICLE_SYS_START_COLOR:{
			D3DXVECTOR3 Color; (*((int*)pData));
			Color.x = (((*((int*)pData)) >> 0 )&(0x000000ff)) / 255.0f;
			Color.y = (((*((int*)pData)) >> 8 )&(0x000000ff)) / 255.0f;
			Color.z = (((*((int*)pData)) >> 16)&(0x000000ff)) / 255.0f;
			m_pCurrentParticleEmmiter->GetParticeSystem()->SetStartColor(Color);
		}
			break;
		case IDS_PARTICLE_SYS_END_COLOR:{
			D3DXVECTOR3 Color; (*((int*)pData));
			Color.x = (((*((int*)pData)) >> 0 )&(0x000000ff)) / 255.0f;
			Color.y = (((*((int*)pData)) >> 8 )&(0x000000ff)) / 255.0f;
			Color.z = (((*((int*)pData)) >> 16)&(0x000000ff)) / 255.0f;
			m_pCurrentParticleEmmiter->GetParticeSystem()->SetEndColor(Color);
		}
			break;
	}

	return true;
}

bool CPresenter::SetComponentProperty(DWORD type, void *pData)
{
	if (!m_pCurrentComponent)return false;

	D3DXMATRIX mat,matRot;
	m_pCurrentComponent->GetTransform(&mat);

	switch (type){
		case IDS_PROP_POS_X:
			mat._41 = *(float*)pData;
			break;
		case IDS_PROP_POS_Y:
			mat._42 = *(float*)pData;
			break;
		case IDS_PROP_POS_Z:
			mat._43 = *(float*)pData;
			break;
		case IDS_PROP_ROT_X:{

			//float alpha  = (float)asin(mat._32);			// alpha
			float beta   = (float)atan2(-mat._31 , mat._33);// beta
			float gamma  = (float)atan2(-mat._12,mat._22);	// gamma

			D3DXMatrixRotationYawPitchRoll(&matRot,-beta,D3DXToRadian(*(float*)pData),-gamma);
			*(D3DXVECTOR3*)&mat._11 = *(D3DXVECTOR3*)&matRot._11;
			*(D3DXVECTOR3*)&mat._21 = *(D3DXVECTOR3*)&matRot._21;
			*(D3DXVECTOR3*)&mat._31 = *(D3DXVECTOR3*)&matRot._31;
		}
		break;
	
		case IDS_PROP_ROT_Y:{
			float alpha  = (float)asin(mat._32);			// alpha
			//float beta   = (float)atan2(-mat._31 , mat._33);// beta
			float gamma  = (float)atan2(-mat._12,mat._22);	// gamma

			D3DXMatrixRotationYawPitchRoll(&matRot,D3DXToRadian(*(float*)pData),-alpha,-gamma);
			*(D3DXVECTOR3*)&mat._11 = *(D3DXVECTOR3*)&matRot._11;
			*(D3DXVECTOR3*)&mat._21 = *(D3DXVECTOR3*)&matRot._21;
			*(D3DXVECTOR3*)&mat._31 = *(D3DXVECTOR3*)&matRot._31;
		}
		break;

		case IDS_PROP_ROT_Z:{
			float alpha  = (float)asin(mat._32);			// alpha
			float beta   = (float)atan2(-mat._31 , mat._33);// beta
			//float gamma  = (float)atan2(-mat._12,mat._22);	// gamma

			D3DXMatrixRotationYawPitchRoll(&matRot,-beta,-alpha,D3DXToRadian(*(float*)pData));
			*(D3DXVECTOR3*)&mat._11 = *(D3DXVECTOR3*)&matRot._11;
			*(D3DXVECTOR3*)&mat._21 = *(D3DXVECTOR3*)&matRot._21;
			*(D3DXVECTOR3*)&mat._31 = *(D3DXVECTOR3*)&matRot._31;
		}
		break;
	}

	m_pCurrentComponent->SetTransform(&mat);

	return true;
}
bool CPresenter::SetMaterialProperty()
{
	return true;
}

CParticleEmmiter* CPresenter::GetParticleEmmiter(string &name)
{
	for (auto it : m_listParticleEmmiter){
		string TempName;
		it->GetName(TempName);
		if (!name.compare(TempName)){
			return it;
		}
	}

	return nullptr;
}

void CPresenter::SelectParticleSys(string name)
{
	CPropertiesWnd *pPropGridWmd = m_pIMainFrame->GetPropertiesGrid();
	pPropGridWmd->ShowParticleSysProps();
	IParticleSysPropGrid *pIParticleSysGrid = pPropGridWmd->GetParticleSysPropGrid();

	m_pCurrentParticleEmmiter = GetParticleEmmiter(name);
	ASSERT(m_pCurrentParticleEmmiter);

	D3DXVECTOR3 pos = m_pCurrentParticleEmmiter->GetfParticlePosition();
	D3DXVECTOR3 vel = m_pCurrentParticleEmmiter->GetfParticleVelocity();

	pIParticleSysGrid->SetPosition(pos.x, pos.y, pos.z);
	pIParticleSysGrid->SetVelocity(vel.x, vel.y, vel.z);

	float fValue;

	fValue = m_pCurrentParticleEmmiter->GetfParticlefAngularVelocity();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_ANGULAR_VEL, (float*)&fValue);

	fValue = m_pCurrentParticleEmmiter->GetfParticlefTimeLife();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_TIME_LIFE, (float*)&fValue);

	fValue = m_pCurrentParticleEmmiter->GetfParticlefStartSize();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_START_SIZE, (float*)&fValue);

	fValue = m_pCurrentParticleEmmiter->GetfParticlefEndSize();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_END_SIZE, (float*)&fValue);

	fValue = m_pCurrentParticleEmmiter->GetfParticlefStartTransparency();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_START_TRANSP, (float*)&fValue);

	fValue = m_pCurrentParticleEmmiter->GetfParticlefEndTransparency();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_END_TRANSP, (float*)&fValue);

	fValue = m_pCurrentParticleEmmiter->GetfParticlefExpKoefVelAtten();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_EXP_VEL_ATTEN, (float*)&fValue);

	fValue = m_pCurrentParticleEmmiter->GetfParticlefTimeEvalExpKoef();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_TIME_EVAL_KOEF, (float*)&fValue);

	fValue = m_pCurrentParticleEmmiter->GetParticleSysGravity();
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_GRAVITY, (float*)&fValue);

	//Set Colors
	D3DXVECTOR3 Color;
	int iColor;
	
	m_pCurrentParticleEmmiter->GetParticeSystem()->GetStartColor(Color);
	iColor = (((int)(Color.x * 255)) << 0) | (((int)(Color.y * 255)) << 8) | (((int)(Color.z * 255)) << 16);
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_START_COLOR, (int*)&iColor);

	m_pCurrentParticleEmmiter->GetParticeSystem()->GetEndColor(Color);
	iColor = (((int)(Color.x * 255)) << 0) | (((int)(Color.y * 255)) << 8) | (((int)(Color.z * 255)) << 16);
	pIParticleSysGrid->SetProperty(IDS_PARTICLE_SYS_END_COLOR, (int*)&iColor);


}

void CPresenter::SelectComponent(string name)
{
	
	CPropertiesWnd *pPropGridWmd = m_pIMainFrame->GetPropertiesGrid();
	pPropGridWmd->ShowComponentProps();

	IComponentPropGrid* pCompPropGrid = pPropGridWmd->GetPropGrid();
	
	m_pCurrentComponent = m_pWorld->GetModel(name);

	if (!m_pCurrentComponent)return;

	D3DXMATRIX mat;
	m_pCurrentComponent->GetTransform(&mat);

	pCompPropGrid->SetCompPosition(mat._41, mat._42, mat._43);


	float alpha  = (float)asin(mat._32);			// alpha
	float beta   = (float)atan2(-mat._31 , mat._33);// beta
	float gamma  = (float)atan2(-mat._12,mat._22);	// gamma

	pCompPropGrid->SetCompRotation(D3DXToDegree(-alpha), D3DXToDegree(-beta), D3DXToDegree(-gamma));



	GetComponentMaterials();

	return;
}
bool CPresenter::SetWeather(SETWEATHER*pWeatherData)
{
	if (m_pWorld)
	m_pWorld->SetWeather(pWeatherData);
	return true;
}

bool CPresenter::SetComponentParent(string ParentName, string ChildName)
{
	CComponent*pParent = m_pWorld->GetModel(ParentName);
	CComponent*pChild = m_pWorld->GetModel(ChildName);

	if (pChild != NULL){
		if (pParent){
			pChild->SetOwner(pParent);
		}
		else{
			pChild->ResetOwner();
		}
		return true;
	}
	return false;
}

void CPresenter::GetComponentMaterials()
{
	if (!m_pCurrentComponent)return;

	CModel* pModel = m_pCurrentComponent->GetModel();

	vector<GraphicPort::POSMATERIAL> vecCurrentPosMat;

	pModel->GetPosMaterials(vecCurrentPosMat);

	//
	IComponentPropGrid* pPropGrid = m_pIMainFrame->GetPropertiesGrid()->GetPropGrid();

	pPropGrid->ClearMaterialsProp();

	for (auto it : vecCurrentPosMat){
		string str;
		(*it)->GetName(str);


		WCHAR w_str[MAX_PATH];

		MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,w_str,MAX_PATH);

		pPropGrid->AddMaterialsPropSubItem(CString(w_str));
	}

}

bool CPresenter::LoadTargetData(string)
{
	return true;
}

bool CPresenter::PasteComponent(string name)
{
	
	CComponent *pComp = m_pWorld->GetModel(name);
	
	if (!pComp)return false;

	string ModelName;
	pComp->GetModel()->GetName(ModelName);

	if (!m_pWorld->LoadModel(ModelName))return false;

	WCHAR CutName[300] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, ModelName.c_str(), -1, CutName, 300);

	m_pIMainFrame->GetFileView()->AddFile(IDS_TREE_VIEW_COMPONENT_TYPE, CutName);

	SelectComponent(ModelName);

	return true;
}

/**------------------------------------------------------
	Load Models Component
	/return true if sucsess
---------------------------------------------------------*/
bool CPresenter::LoadComponent()
{

	OPENFILENAME ofn;       // common dialog box structure
	WCHAR szFile[260];       // buffer for file name
	CHAR  c_str[255];
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = theApp.GetMainWnd()->GetSafeHwnd();
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';					// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	ofn.nMaxFile = sizeof(szFile);		// use the contents of szFile to initialize itself.
	ofn.lpstrFilter = L"HMD\0*.HMD\0All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	string ModelName;

	if (GetOpenFileName(&ofn) == TRUE){
		//SetCurrentDirectory(sODir);

		WideCharToMultiByte(CP_ACP, 0, ofn.lpstrFile, -1, c_str, 255, 0, 0);
		ModelName = c_str;
		m_pWorld->LoadModel(ModelName);


		WCHAR CutName[300] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, ModelName.c_str(), -1, CutName, 300);

		m_pIMainFrame->GetFileView()->AddFile(IDS_TREE_VIEW_COMPONENT_TYPE, CutName);

		SelectComponent(ModelName);

	}

	return true;
}

bool CPresenter::SaveTargetData(string name)
{
	
	OPENFILENAME ofn;       // common dialog box structure

	WCHAR szFile[260];       // buffer for file name
	CHAR  c_str[255];	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= theApp.GetMainWnd()->GetSafeHwnd();
	ofn.lpstrFile		= szFile;
	ofn.lpstrFile[0]	= '\0';					// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	ofn.nMaxFile		= sizeof(szFile);		// use the contents of szFile to initialize itself.
	ofn.lpstrFilter		= L"XML\0*.XML\0All\0*.*\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrDefExt		= NULL;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = L"D:\\";
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	if (GetSaveFileName(&ofn) == TRUE){
		
		WCHAR CutName[300] = {0};
		CHAR c_str[255]		= {0};
		CString wstr = ofn.lpstrFile;

		int length		= wstr.GetLength(); 
		int slashPos	= wstr.ReverseFind(*_T("\\"));

		for(int i=slashPos+1, j = 0;i<length;i++,j++){
			CutName[j] = wstr.GetString()[i];
		}

		WideCharToMultiByte(CP_ACP,0,CutName,-1,c_str,255,0,0);
		m_pWorld->SaveTargetData(string(c_str));
	}

	return true;
}