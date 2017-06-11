#include "stdafx.h"
#include "CompPropertiesGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


bool CCompPropertiesGrid::InitCompPropertyList()
{
	CMFCPropertyGridCtrl::RemoveAll();

	CMFCPropertyGridProperty* pGeneralGroup = new CMFCPropertyGridProperty(_T("General"));

	CMFCPropertyGridProperty* pProp;

	//Pos
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("Position"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t(0.0f)), _T("”казывает смещение по оси X"));
	//pProp->EnableSpinControl(TRUE, -1000, 1000);
	pProp->SetData(IDS_PROP_POS_X);
	pPosition->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Y"), (_variant_t(0.0f)), _T("”казывает смещение по оси Y"));
	pProp->SetData(IDS_PROP_POS_Y);
	pPosition->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Z"), (_variant_t(0.0f)), _T("”казывает смещение по оси Z"));
	pProp->SetData(IDS_PROP_POS_Z);
	pPosition->AddSubItem(pProp);

	//Scal 
	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty(_T("Scale"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t(0.0f)), _T("”казывает масштабирование по оси X"));
	//pProp->EnableSpinControl(TRUE, 1, 1000);

	pScale->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Y"), (_variant_t(0.0f)), _T("”казывает масштабирование по оси Y"));
	//pProp->EnableSpinControl(TRUE, 1, 1000);
	pScale->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Z"), (_variant_t(0.0f)), _T("”казывает масштабирование по оси Z"));
	//pProp->EnableSpinControl(TRUE, 1, 1000);
	pScale->AddSubItem(pProp);


	//Rot
	CMFCPropertyGridProperty* pRotation = new CMFCPropertyGridProperty(_T("Rotation"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t(0.0f)), _T("”казывает поворот по оси X"));
	//pProp->EnableSpinControl(TRUE, -1000, 1000);
	pProp->SetData(IDS_PROP_ROT_X);
	pRotation->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Y"), (_variant_t(0.0f)), _T("”казывает поворот по оси Y"));
	//pProp->EnableSpinControl(TRUE, -1000, 1000);
	pProp->SetData(IDS_PROP_ROT_Y);
	pRotation->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Z"), (_variant_t(0.0f)), _T("”казывает поворот по оси Z"));
	//pProp->EnableSpinControl(TRUE, -1000, 1000);
	pProp->SetData(IDS_PROP_ROT_Z);
	pRotation->AddSubItem(pProp);

	pGeneralGroup->AddSubItem(pPosition);
	pGeneralGroup->AddSubItem(pScale);
	pGeneralGroup->AddSubItem(pRotation);

	////Mat
	CMFCPropertyGridProperty* pMatGroup = new CMFCPropertyGridProperty(_T("Materials"));

	m_iIdPropGeneral = AddProperty(pGeneralGroup);
	m_iIdPropMaterial = AddProperty(pMatGroup);

	return true;
}

void CCompPropertiesGrid::ClearMaterialsProp()
{
	CMFCPropertyGridProperty* pPropMat = GetProperty(m_iIdPropMaterial);
	int itemCnt = pPropMat->GetSubItemsCount();

	while (pPropMat->GetSubItemsCount()){

		CMFCPropertyGridProperty *prop = pPropMat->GetSubItem(0);
		pPropMat->RemoveSubItem(prop);

	}

}
void CCompPropertiesGrid::AddMaterialsPropSubItem(CString nameMat)
{
	CMFCPropertyGridProperty* pSubMat = new CMFCPropertyGridProperty(_T("SubMesh Material"), (_variant_t(nameMat)), _T("”казывает материал"));
	CMFCPropertyGridProperty* pPropMaterial = GetProperty(m_iIdPropMaterial);
	pPropMaterial->AddSubItem(pSubMat);

	pPropMaterial->Expand(FALSE);
}

//
bool CCompPropertiesGrid::SetCompPosition(float posX, float posY, float posZ)
{

	CMFCPropertyGridProperty* m_pPropGeneral = GetProperty(m_iIdPropGeneral);

	int itemCnt = m_pPropGeneral->GetSubItemsCount();

	CMFCPropertyGridProperty *pPropPosX = this->FindItemByData(IDS_PROP_POS_X);
	CMFCPropertyGridProperty *pPropPosY = this->FindItemByData(IDS_PROP_POS_Y);
	CMFCPropertyGridProperty *pPropPosZ = this->FindItemByData(IDS_PROP_POS_Z);

	//var.vt = VT_R4;
	pPropPosX->SetValue(COleVariant(posX));
	pPropPosY->SetValue(COleVariant(posY));
	pPropPosZ->SetValue(COleVariant(posZ));

	return true;
}
bool CCompPropertiesGrid::SetCompRotation(float rotX, float rotY, float rotZ)
{
	CMFCPropertyGridProperty* m_pPropGeneral = GetProperty(m_iIdPropGeneral);

	int itemCnt = m_pPropGeneral->GetSubItemsCount();

	CMFCPropertyGridProperty *pPropRotX = this->FindItemByData(IDS_PROP_ROT_X);
	CMFCPropertyGridProperty *pPropRotY = this->FindItemByData(IDS_PROP_ROT_Y);
	CMFCPropertyGridProperty *pPropRotZ = this->FindItemByData(IDS_PROP_ROT_Z);

	//var.vt = VT_R4;
	pPropRotX->SetValue(COleVariant(rotX));
	pPropRotY->SetValue(COleVariant(rotY));
	pPropRotZ->SetValue(COleVariant(rotZ));

	return true;
}
bool CCompPropertiesGrid::SetCompScale(float, float, float)
{
	return true;
}


void CCompPropertiesGrid::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{

	CString str = pProp->GetName();

	switch (pProp->GetData()){
	case IDS_PROP_POS_X:
		theApp.GetPresenter()->SetComponentProperty(IDS_PROP_POS_X, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PROP_POS_Y:
		theApp.GetPresenter()->SetComponentProperty(IDS_PROP_POS_Y, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PROP_POS_Z:
		theApp.GetPresenter()->SetComponentProperty(IDS_PROP_POS_Z, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PROP_ROT_X:
		theApp.GetPresenter()->SetComponentProperty(IDS_PROP_ROT_X, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PROP_ROT_Y:
		theApp.GetPresenter()->SetComponentProperty(IDS_PROP_ROT_Y, (float*)&pProp->GetValue().fltVal);
		break;
	case IDS_PROP_ROT_Z:
		theApp.GetPresenter()->SetComponentProperty(IDS_PROP_ROT_Z, (float*)&pProp->GetValue().fltVal);
		break;
	}

	return __super::OnPropertyChanged(pProp);
}
