#pragma once

class IComponentPropGrid
{
public:
	virtual bool SetCompPosition(float, float, float) = 0;
	virtual bool SetCompRotation(float, float, float) = 0;
	virtual bool SetCompScale(float, float, float) = 0;
	virtual void ClearMaterialsProp() = 0;
	virtual void AddMaterialsPropSubItem(CString nameMat) = 0;

};

class CCompPropertiesGrid : public CMFCPropertyGridCtrl, public IComponentPropGrid
{
	int m_iIdPropGeneral;
	int m_iIdPropMaterial;

public:
	bool InitCompPropertyList();

	void ClearMaterialsProp();
	void AddMaterialsPropSubItem(CString nameMat);
	//
	bool SetCompPosition(float, float, float);
	bool SetCompRotation(float, float, float);
	bool SetCompScale(float, float, float);
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
};

