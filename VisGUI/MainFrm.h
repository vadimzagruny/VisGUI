
// MainFrm.h : интерфейс класса CMainFrame
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "PropertiesWnd.h"
#include "EditParticles.h"
#include "MaterialEditor.h"
#include "ParticleSysNameDlg.h"

class IMainView
{
public:
	virtual CFileView*GetFileView() = 0;
	virtual CPropertiesWnd*GetPropertiesGrid() = 0;
};

class CMainFrame : public CFrameWndEx , public IMainView
{
	
protected: // создать только из сериализации
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Атрибуты

	//CEditParticles		EditParticleDlg;
	CMaterialEditor		EditMaterialDlg;
	CParticleSysNameDlg	m_PartSysDlg;

public:

// Операции
public:
	CFileView		*GetFileView(){return &m_wndFileView;}
	CPropertiesWnd	*GetPropertiesGrid(){ return &m_wndProperties; }

// Переопределение.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Реализация
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // встроенные члены панели элементов управления
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	CPropertiesWnd    m_wndProperties;

// Созданные функции схемы сообщений
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnEditParticles();
	afx_msg void OnLoadComponent();
	afx_msg void OnClearscene();
	afx_msg void OnEditEditmaterial();
//	afx_msg void OnPopUpExplorerOpen();
//	afx_msg void OnPopoutLoadComp();
	afx_msg void OnSaveTargetdataFile();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnProperties();
	afx_msg void OnEditCreateparticlesystem();
};


