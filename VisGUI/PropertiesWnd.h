
#pragma once

#include "CompPropertiesGrid.h"
#include "ParticleSysPropertiesGrid.h"

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// Создание

public:
	CPropertiesWnd();

	void AdjustLayout();

// Атрибуты
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		//poorGrid
		m_wndPoorProp.SetVSDotNetLook(bSet);
		m_wndPoorProp.SetGroupNameFullWidth(bSet);
		
		//
		m_wndComponentProp.SetVSDotNetLook(bSet);
		m_wndComponentProp.SetGroupNameFullWidth(bSet);

		//
		m_wndParticleSysProp.SetVSDotNetLook(bSet);
		m_wndParticleSysProp.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont						m_fntPropList;
	CComboBox					m_wndObjectCombo;
	CPropertiesToolBar			m_wndToolBar;
	CCompPropertiesGrid			m_wndComponentProp;
	CParticleSysPropertiesGrid	m_wndParticleSysProp;

	CMFCPropertyGridCtrl		m_wndPoorProp;

// Реализация
public:
	virtual ~CPropertiesWnd();

	IComponentPropGrid *GetPropGrid(){ return &m_wndComponentProp; }
	IParticleSysPropGrid *GetParticleSysPropGrid(){ return &m_wndParticleSysProp; }

	void ShowComponentProps();
	void ShowParticleSysProps();
	void HideProps();


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

	int m_nComboHeight;
public:
	afx_msg void OnIdsPropTest();
};

