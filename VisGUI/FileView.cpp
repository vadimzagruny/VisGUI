
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "VisGUI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_PASTE, &CFileView::OnEditPaste)
//	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CFileView::OnUpdateEditPaste)
//ON_COMMAND(ID_EDIT_PASTE_COMPONENT, &CFileView::OnEditPasteComponent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// обработчики сообщений CWorkspaceBar

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Создание представления:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Не удалось создать представление файлов\n");
		return -1;      // не удалось создать
	}


	// Загрузить изображения представления:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// Все команды будут перенаправлены через этот элемент управления, а не через родительскую рамку:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Введите некоторые данные статического представления в виде дерева (пустой код, ничего более)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	/*HTREEITEM hRoot = m_wndFileView.InsertItem(_T("Файлы FakeApp"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("Исходные файлы FakeApp"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.cpp"), 0, 0, hSrc);
	m_wndFileView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	HTREEITEM hInc = m_wndFileView.InsertItem(_T("Файлы заголовка FakeApp"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("Resource.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	HTREEITEM hRes = m_wndFileView.InsertItem(_T("Файлы ресурсов FakeApp"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);*/

	/*m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hInc, TVE_EXPAND);*/
}

void CFileView::Clear()
{
	m_wndFileView.DeleteAllItems();
}

void CFileView::AddFile(DWORD_PTR fileType, LPCTSTR lpszItem)
{
	HTREEITEM hItem;

	switch (fileType){
	case IDS_TREE_VIEW_COMPONENT_TYPE:
			hItem = m_wndFileView.InsertItem(lpszItem,0,0,0);
			m_wndFileView.SetItemData(hItem, fileType);
			m_wndFileView.SelectItem(hItem);
		break;
	case IDS_TREE_VIEW_PARTICLE_SYS_TYPE:
			hItem = m_wndFileView.InsertItem(lpszItem, 1,1,0);
			m_wndFileView.SetItemData(hItem, fileType);
			m_wndFileView.SelectItem(hItem);
		break;
	default:
		ASSERT(0);
	}

	m_wndFileView.SetFocus();

}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Выбрать нажатый элемент:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, theApp.GetMainWnd() , TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("Свойства: HMD - ФАЙЛ"));

}


void CFileView::OnDummyCompile()
{
	// TODO: добавьте сюда код обработчика команд
}



void CFileView::OnEditPaste()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndFileView;
	ASSERT_VALID(pWndTree);
	
	HTREEITEM hSelItem = pWndTree->GetSelectedItem();
	CString  wstr = pWndTree->GetItemText(hSelItem);
	char c_str[300];
	
	
	WideCharToMultiByte(CP_ACP,0,wstr,-1,c_str,300,0,0);
	
	theApp.GetPresenter()->PasteComponent(string(c_str));
}

void CFileView::OnEditCut()
{
	int a = 0;
	a = a;// TODO: добавьте сюда код обработчика команд
}

void CFileView::OnEditCopy()
{
	// TODO: добавьте сюда код обработчика команд
}

void CFileView::OnEditClear()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndFileView;
	ASSERT_VALID(pWndTree);

	HTREEITEM hSelItem = pWndTree->GetSelectedItem();

	if (!hSelItem)return;

	DeleteChildrens(hSelItem);

	CString  wstr = pWndTree->GetItemText(hSelItem);
	char cstr[255] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, cstr, 255, 0, 0);

	DWORD_PTR itemType = pWndTree->GetItemData(hSelItem);

	switch (itemType){
	case IDS_TREE_VIEW_COMPONENT_TYPE:
		theApp.GetPresenter()->DeleteComponent(string(cstr));
		break;
	case IDS_TREE_VIEW_PARTICLE_SYS_TYPE:
		theApp.GetPresenter()->ReleaseParticleSystem(string(cstr));
		break;
	default:
		ASSERT(0);
	}
	

	pWndTree->DeleteItem(hSelItem);
}

void CFileView::DeleteChildrens(HTREEITEM hSelItem)
{
	if (m_wndFileView.ItemHasChildren(hSelItem)){
		
		HTREEITEM hItem = m_wndFileView.GetChildItem(hSelItem);
		char cstr[255] = {0};

		while (hItem != NULL){

			DeleteChildrens(hItem);
			CString  wstr = m_wndFileView.GetItemText(hItem);

			WideCharToMultiByte(CP_ACP,0,wstr,-1,cstr,255,0,0);

			DWORD_PTR itemType = m_wndFileView.GetItemData(hSelItem);

			switch (itemType){
			case IDS_TREE_VIEW_COMPONENT_TYPE:
				theApp.GetPresenter()->DeleteComponent(string(cstr));
				break;
			case IDS_TREE_VIEW_PARTICLE_SYS_TYPE:
				theApp.GetPresenter()->ReleaseParticleSystem(string(cstr));
				break;
			default:
				ASSERT(0);
			}

			hItem = m_wndFileView.GetNextSiblingItem(hItem);
		}
		
	}
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // контекст устройства для рисования

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();

}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Не удается загрузить точечный рисунок: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}




