#include "stdafx.h"
#include "ParticleSysPropertiesGrid.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CParticleSysPropertiesGrid::CParticleSysPropertiesGrid()
{
}


CParticleSysPropertiesGrid::~CParticleSysPropertiesGrid()
{
}

bool CParticleSysPropertiesGrid::InitProp()
{
	CMFCPropertyGridCtrl::RemoveAll();

	CMFCPropertyGridProperty* pGeneralGroup = new CMFCPropertyGridProperty(_T("General"));

	CMFCPropertyGridProperty* pProp;

	//Pos
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("Position"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t(0.0f)), _T("Указывает смещение по оси X"));
	//pProp->EnableSpinControl(TRUE, -1000, 1000);
	pProp->SetData(IDS_PARTICLE_SYS_POS_X);
	pPosition->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Y"), (_variant_t(0.0f)), _T("Указывает смещение по оси Y"));
	pProp->SetData(IDS_PARTICLE_SYS_POS_Y);
	pPosition->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Z"), (_variant_t(0.0f)), _T("Указывает смещение по оси Z"));
	pProp->SetData(IDS_PARTICLE_SYS_POS_Z);
	pPosition->AddSubItem(pProp);

	//Vel 
	CMFCPropertyGridProperty* pVelocity = new CMFCPropertyGridProperty(_T("Velocity"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t(0.0f)), _T("Указывает скорость по оси X"));
	//pProp->EnableSpinControl(TRUE, 1, 1000);
	pProp->SetData(IDS_PARTICLE_SYS_VEL_X);
	pVelocity->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Y"), (_variant_t(0.0f)), _T("Указывает скорость по оси Y"));
	//pProp->EnableSpinControl(TRUE, 1, 1000);
	pProp->SetData(IDS_PARTICLE_SYS_VEL_Y);
	pVelocity->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Z"), (_variant_t(0.0f)), _T("Указывает скорость по оси Z"));
	//pProp->EnableSpinControl(TRUE, 1, 1000);
	pProp->SetData(IDS_PARTICLE_SYS_VEL_Z);
	pVelocity->AddSubItem(pProp);

	pGeneralGroup->AddSubItem(pPosition);
	pGeneralGroup->AddSubItem(pVelocity);
	
	//
	CMFCPropertyGridProperty* pItem = nullptr;

	//Angular Velocity
	pItem = new CMFCPropertyGridProperty(_T("AngularVelocity"), (_variant_t(0.0f)), _T("Указывает угловую скорость вращения частцы"), IDS_PARTICLE_SYS_ANGULAR_VEL);
	pGeneralGroup->AddSubItem(pItem);
	//Time Life
	pItem = new CMFCPropertyGridProperty(_T("TimeLife"), (_variant_t(0.0f)), _T("Указывает время жизни частцы"), IDS_PARTICLE_SYS_TIME_LIFE);
	pGeneralGroup->AddSubItem(pItem);
	//Start Size
	pItem = new CMFCPropertyGridProperty(_T("StartSize"), (_variant_t(0.0f)), _T("Указывает начальный размер частцы"), IDS_PARTICLE_SYS_START_SIZE);
	pGeneralGroup->AddSubItem(pItem);
	//End Size
	pItem = new CMFCPropertyGridProperty(_T("EndSize"), (_variant_t(0.0f)), _T("Указывает конечный размер частцы"), IDS_PARTICLE_SYS_END_SIZE);
	pGeneralGroup->AddSubItem(pItem);
	//Start Transparency
	pItem = new CMFCPropertyGridProperty(_T("StartTransparency"), (_variant_t(0.0f)), _T("Указывает начальную прозрачность частцы"), IDS_PARTICLE_SYS_START_TRANSP);
	pGeneralGroup->AddSubItem(pItem);
	//End Transparency
	pItem = new CMFCPropertyGridProperty(_T("EndTransparency"), (_variant_t(0.0f)), _T("Указывает конечную прозрачность частцы"), IDS_PARTICLE_SYS_END_TRANSP);
	pGeneralGroup->AddSubItem(pItem);
	//Gravity
	pItem = new CMFCPropertyGridProperty(_T("Gravity"), (_variant_t(0.0f)), _T("Указывает силу притяжения частиц"), IDS_PARTICLE_SYS_GRAVITY);
	pGeneralGroup->AddSubItem(pItem);
	//ExpKoeVelAtten
	pItem = new CMFCPropertyGridProperty(_T("ExpKoeVelAtten"), (_variant_t(0.0f)), _T("Указывает коэффициент затухания скорости частиц"), IDS_PARTICLE_SYS_EXP_VEL_ATTEN);
	pGeneralGroup->AddSubItem(pItem);
	//TimeEvalExpKoef
	pItem = new CMFCPropertyGridProperty(_T("TimeEvalExpKoef"), (_variant_t(0.0f)), _T("Указывает ... частиц"), IDS_PARTICLE_SYS_TIME_EVAL_KOEF);
	pGeneralGroup->AddSubItem(pItem);


	// Material path
	pItem = new CMFCPropertyGridFileProperty(L"MaterialsPath", true,L"");
	pItem->SetData(IDS_PARTICLE_SYS_MATERIAL);
	pGeneralGroup->AddSubItem(pItem);
	//Start Color
	pItem = new CMFCPropertyGridColorProperty(L"StartColor", 0xFFFFFF, 0, L"Стартовый цвет частицы", IDS_PARTICLE_SYS_START_COLOR);
	((CMFCPropertyGridColorProperty*)pItem)->EnableOtherButton(_T("Палитра"));
	pGeneralGroup->AddSubItem(pItem);
	//End Color
	pItem = new CMFCPropertyGridColorProperty(L"EndColor", 0xFFFFFF, 0, L"Конечный цвет частицы", IDS_PARTICLE_SYS_END_COLOR);
	((CMFCPropertyGridColorProperty*)pItem)->EnableOtherButton(_T("Палитра"));
	pGeneralGroup->AddSubItem(pItem);

	m_iIdPropGeneral = AddProperty(pGeneralGroup);


	return true;
}

void CParticleSysPropertiesGrid::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	CString str = pProp->GetName();

	switch (pProp->GetData()){
	case IDS_PARTICLE_SYS_POS_X:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_POS_X, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_POS_Y:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_POS_Y, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_POS_Z:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_POS_Z, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_VEL_X:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_VEL_X, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_VEL_Y:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_VEL_Y, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_VEL_Z:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_VEL_Z, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_ANGULAR_VEL:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_ANGULAR_VEL, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_TIME_LIFE:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_TIME_LIFE, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_START_SIZE:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_START_SIZE, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_END_SIZE:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_END_SIZE, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_START_TRANSP:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_START_TRANSP, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_END_TRANSP:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_END_TRANSP, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_GRAVITY:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_GRAVITY, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_EXP_VEL_ATTEN:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_EXP_VEL_ATTEN, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_TIME_EVAL_KOEF:
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_TIME_EVAL_KOEF, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PARTICLE_SYS_MATERIAL:{
		CString s = pProp->GetValue().bstrVal;
		char c_str[MAX_PATH] = { 0 };
		WideCharToMultiByte(CP_ACP, 0, s.GetString(), -1, c_str, MAX_PATH, 0, 0);
		theApp.GetPresenter()->SetParticleSystemMaterial(string(c_str));
	}
		break;
	case IDS_PARTICLE_SYS_START_COLOR:{
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_START_COLOR, (int*)&pProp->GetValue().intVal);
	}
		break;
	case IDS_PARTICLE_SYS_END_COLOR:{
		theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_END_COLOR, (int*)&pProp->GetValue().intVal);
	}
		break;
	default:
		ASSERT(0);
	}

	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}


bool CParticleSysPropertiesGrid::SetProperty(DWORD_PTR type, void *pData)
{
	CMFCPropertyGridProperty *pPropItem = nullptr;

	switch (type){
	case IDS_PARTICLE_SYS_ANGULAR_VEL:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_ANGULAR_VEL);
		pPropItem->SetValue(COleVariant(*(float*)pData));
		break;
	case IDS_PARTICLE_SYS_TIME_LIFE:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_TIME_LIFE);
		pPropItem->SetValue(COleVariant(*(float*)pData)); 
		break;
	case IDS_PARTICLE_SYS_START_SIZE:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_START_SIZE);
		pPropItem->SetValue(COleVariant(*(float*)pData));
		break;
	case IDS_PARTICLE_SYS_END_SIZE:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_END_SIZE);
		pPropItem->SetValue(COleVariant(*(float*)pData));
		break;
	case IDS_PARTICLE_SYS_START_TRANSP:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_START_TRANSP);
		pPropItem->SetValue(COleVariant(*(float*)pData));
		break;
	case IDS_PARTICLE_SYS_END_TRANSP:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_END_TRANSP);
		pPropItem->SetValue(COleVariant(*(float*)pData));
		break;
	case IDS_PARTICLE_SYS_GRAVITY:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_GRAVITY);
		pPropItem->SetValue(COleVariant(*(float*)pData));
		break;
	case IDS_PARTICLE_SYS_EXP_VEL_ATTEN:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_EXP_VEL_ATTEN);
		pPropItem->SetValue(COleVariant(*(float*)pData));
		break;
	case IDS_PARTICLE_SYS_TIME_EVAL_KOEF:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_TIME_EVAL_KOEF);
		pPropItem->SetValue(COleVariant(*(float*)pData));
		break;
	case IDS_PARTICLE_SYS_START_COLOR:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_START_COLOR);
		((CMFCPropertyGridColorProperty*)pPropItem)->SetColor((*(COLORREF*)pData));
		break;
	case IDS_PARTICLE_SYS_END_COLOR:
		pPropItem = this->FindItemByData(IDS_PARTICLE_SYS_END_COLOR);
		((CMFCPropertyGridColorProperty*)pPropItem)->SetColor((*(COLORREF*)pData));
		break;
	default:
		ASSERT(0);
	}

	return true;
}


bool CParticleSysPropertiesGrid::SetPosition(float x, float y, float z)
{
	CMFCPropertyGridProperty* m_pPropGeneral = GetProperty(m_iIdPropGeneral);

	int itemCnt = m_pPropGeneral->GetSubItemsCount();

	CMFCPropertyGridProperty *pPropPosX = this->FindItemByData(IDS_PARTICLE_SYS_POS_X);
	CMFCPropertyGridProperty *pPropPosY = this->FindItemByData(IDS_PARTICLE_SYS_POS_Y);
	CMFCPropertyGridProperty *pPropPosZ = this->FindItemByData(IDS_PARTICLE_SYS_POS_Z);

	//var.vt = VT_R4;
	pPropPosX->SetValue(COleVariant(x));
	pPropPosY->SetValue(COleVariant(y));
	pPropPosZ->SetValue(COleVariant(z));
	return true;
}
bool CParticleSysPropertiesGrid::SetVelocity(float x, float y, float z)
{
	CMFCPropertyGridProperty *pPropVelX = this->FindItemByData(IDS_PARTICLE_SYS_VEL_X);
	CMFCPropertyGridProperty *pPropVelY = this->FindItemByData(IDS_PARTICLE_SYS_VEL_Y);
	CMFCPropertyGridProperty *pPropVelZ = this->FindItemByData(IDS_PARTICLE_SYS_VEL_Z);

	//var.vt = VT_R4;
	pPropVelX->SetValue(COleVariant(x));
	pPropVelY->SetValue(COleVariant(y));
	pPropVelZ->SetValue(COleVariant(z));
	return true;
}
