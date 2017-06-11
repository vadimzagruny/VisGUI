
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "VisGUI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_nComboHeight = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(IDS_PROP_TEST, &CPropertiesWnd::OnIdsPropTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// обработчики сообщений CResourceViewBar

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndComponentProp.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndParticleSysProp.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPoorProp.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Создать поле со списком:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Не удалось создать поле со списком \"Свойства\" \n");
		return -1;      // не удалось создать
	}

	m_wndObjectCombo.AddString(_T("Приложение"));
	m_wndObjectCombo.AddString(_T("Окно \"Свойства\""));
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndComponentProp.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Не удалось создать сетку свойств\n");
		return -1;      // не удалось создать
	}

	//create PartSysProps
	if (!m_wndParticleSysProp.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 3))
	{
		TRACE0("Не удалось создать сетку свойств\n");
		return -1;      // не удалось создать
	}

	//create poorGrid
	if (!m_wndPoorProp.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 3))
	{
		TRACE0("Не удалось создать сетку свойств\n");
		return -1;      // не удалось создать
	}

	// Set custom colors for property grid
	//m_wndComponentProp.SetCustomColors(RGB(228, 243, 254), RGB(46, 70, 165), RGB(200, 236, 209), RGB(33, 102, 49), RGB(255, 229, 216), RGB(128, 0, 0), RGB(159, 159, 255));


	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Заблокирован */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Заблокирован */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// Все команды будут перенаправлены через этот элемент управления, а не через родительскую рамку:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndComponentProp.ExpandAll();
	m_wndParticleSysProp.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndComponentProp.SetAlphabeticMode(!m_wndComponentProp.IsAlphabeticMode());
	m_wndParticleSysProp.SetAlphabeticMode(!m_wndParticleSysProp.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndComponentProp.IsAlphabeticMode());
	pCmdUI->SetCheck(m_wndParticleSysProp.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: добавьте сюда код обработчика команд
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: добавьте сюда код обработчика команд обновления интерфейса пользователя
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: добавьте сюда код обработчика команд
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: добавьте сюда код обработчика команд обновления интерфейса пользователя
}


void CPropertiesWnd::ShowComponentProps()
{
	m_wndComponentProp.ShowWindow(SW_SHOW);
	m_wndParticleSysProp.ShowWindow(SW_HIDE);
	m_wndPoorProp.ShowWindow(SW_HIDE);
	m_wndComponentProp.SetFocus();

}
void CPropertiesWnd::ShowParticleSysProps()
{
	m_wndParticleSysProp.ShowWindow(SW_SHOW);
	m_wndComponentProp.ShowWindow(SW_HIDE);
	m_wndPoorProp.ShowWindow(SW_HIDE);
	m_wndParticleSysProp.SetFocus();
}
void CPropertiesWnd::HideProps()
{
	m_wndComponentProp.ShowWindow(SW_HIDE);
	m_wndParticleSysProp.ShowWindow(SW_HIDE);
	m_wndPoorProp.ShowWindow(SW_SHOW);
}


void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	//init poorGrid
	m_wndPoorProp.EnableHeaderCtrl(FALSE);
	m_wndPoorProp.EnableDescriptionArea();
	m_wndPoorProp.SetVSDotNetLook();

	m_wndPoorProp.ShowWindow(SW_SHOW);
	
	//init component Grid
	m_wndComponentProp.EnableHeaderCtrl(FALSE);
	m_wndComponentProp.EnableDescriptionArea();
	m_wndComponentProp.SetVSDotNetLook();
	m_wndComponentProp.MarkModifiedProperties();

	m_wndComponentProp.InitCompPropertyList();

	m_wndComponentProp.ShowWindow(SW_HIDE);

	//init PartSys Grid
	m_wndParticleSysProp.EnableHeaderCtrl(FALSE);
	m_wndParticleSysProp.EnableDescriptionArea();
	m_wndParticleSysProp.SetVSDotNetLook();
	m_wndParticleSysProp.MarkModifiedProperties();

	m_wndParticleSysProp.InitProp();
	m_wndParticleSysProp.ShowWindow(SW_HIDE);
	

}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndComponentProp.SetFocus();
	m_wndParticleSysProp.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndComponentProp.SetFont(&m_fntPropList);
	m_wndParticleSysProp.SetFont(&m_fntPropList);
	m_wndPoorProp.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}


void CPropertiesWnd::OnIdsPropTest()
{
	int ass = 0;
	ass =ass;// TODO: добавьте свой код обработчика команд
}


///CPropertiesGrid
