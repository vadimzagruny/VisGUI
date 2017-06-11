#pragma once

class IParticleSysPropGrid
{
public:
	virtual bool SetPosition(float, float, float) = 0;
	virtual bool SetVelocity(float, float, float) = 0;
	virtual bool SetProperty(DWORD_PTR type, void *pData)= 0;
};


class CParticleSysPropertiesGrid : public CMFCPropertyGridCtrl, public IParticleSysPropGrid
{

	int m_iIdPropGeneral;

public:
	CParticleSysPropertiesGrid();
	~CParticleSysPropertiesGrid();

	bool InitProp();

	bool SetPosition(float x, float y, float z);
	bool SetVelocity(float x, float y, float z);

	bool SetProperty(DWORD_PTR type,void *pData);

	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
};

