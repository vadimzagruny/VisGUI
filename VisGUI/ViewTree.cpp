
#include "stdafx.h"
#include "ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
	m_bDragging = FALSE;
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDblclk)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CViewTree::OnTvnBegindrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// обработчики сообщений CViewTree

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}


	return bRes;
}


void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM item = GetSelectedItem();
	CString str;

	if(item){
		DWORD_PTR itemType = GetItemData(item);

		str = GetItemText(item);
		char c_str[200] = { 0 };
		WideCharToMultiByte(CP_ACP, 0, str.GetString(), -1, c_str,200,0,0);

		switch (itemType){
			case IDS_TREE_VIEW_COMPONENT_TYPE:
				theApp.GetPresenter()->SelectComponent(string(c_str));
				break;
			case IDS_TREE_VIEW_PARTICLE_SYS_TYPE:
				theApp.GetPresenter()->SelectParticleSys(string(c_str));
				break;
			default:
				ASSERT(0);
		}

	}
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}



void CViewTree::OnMouseMove(UINT nFlags, CPoint cpoint)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного


	//HTREEITEM htiTarget;  // Handle to target item. 
	TVHITTESTINFO tvht;   // Hit test information. 

	if (m_bDragging)
	{
		// Drag the item to the current position of the mouse pointer. 
		// First convert the dialog coordinates to control coordinates. 
		POINT point;
		point.x = cpoint.x;
		point.y = cpoint.y;
		ClientToScreen( &point);
		ScreenToClient( &point);
		ImageList_DragMove(point.x, point.y);
		// Turn off the dragged image so the background can be refreshed.
		ImageList_DragShowNolock(FALSE);

		// Find out if the pointer is on the item. If it is, 
		// highlight the item as a drop target. 
		tvht.pt.x = point.x;
		tvht.pt.y = point.y;
		if ((m_hitemDrop = TreeView_HitTest(GetSafeHwnd(), &tvht)) != NULL)
		{
			TreeView_SelectDropTarget(GetSafeHwnd(), m_hitemDrop);
		}
		ImageList_DragShowNolock(TRUE);
	}

	CTreeCtrl::OnMouseMove(nFlags, cpoint);
}


void CViewTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	if (m_bDragging)
	{
		// Get destination item.
		HTREEITEM htiDest = TreeView_GetDropHilight(this->GetSafeHwnd());
		if (htiDest != NULL)
		{
			/*HTREEITEM newItem = InsertItem(GetItemText(m_hitemDrag), htiDest);
			DeleteItem(m_hitemDrag);
			m_hitemDrag = NULL;*/

			Expand(htiDest, TVE_EXPAND);
			HTREEITEM htiNew = CopyBranch(m_hitemDrag, m_hitemDrop, TVI_LAST);

			UpdateComponentsParent(m_hitemDrag, GetItemText(m_hitemDrop), GetItemText(m_hitemDrag));
			DeleteItem(m_hitemDrag);
			SelectItem(htiNew);
			// To do: handle the actual moving of the dragged node.
		}
		ImageList_EndDrag();
		TreeView_SelectDropTarget(this->GetSafeHwnd(), NULL);
		ReleaseCapture();
		ShowCursor(TRUE);
		m_bDragging = FALSE;
	}

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

// Begin dragging an item in a tree-view control. 
// hwndTV - handle to the image list. 
// lpnmtv - address of information about the item being dragged.
//
// g_fDragging -- global BOOL that specifies whether dragging is underway.

void CViewTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	HIMAGELIST himl;    // handle to image list 
	RECT rcItem;        // bounding rectangle of item 

	// Tell the tree-view control to create an image to use 
	// for dragging. 
	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_hitemDrop = NULL;

	himl = TreeView_CreateDragImage(pNMHDR->hwndFrom, pNMTreeView->itemNew.hItem);

	// Get the bounding rectangle of the item being dragged. 
	TreeView_GetItemRect(pNMHDR->hwndFrom, pNMTreeView->itemNew.hItem, &rcItem, TRUE);

	// Start the drag operation. 
	ImageList_BeginDrag(himl, 0, 0, 0);
	ImageList_DragEnter(pNMHDR->hwndFrom, pNMTreeView->ptDrag.x, pNMTreeView->ptDrag.x);

	// Hide the mouse pointer, and direct mouse input to the 
	// parent window. 
	ShowCursor(FALSE);
	SetCapture();
	m_bDragging = TRUE;
	*pResult = 0;
}


HTREEITEM CViewTree::CopyBranch(HTREEITEM oldItem, HTREEITEM newParent, HTREEITEM iPosition)
{
	HTREEITEM newItem = InsertItem(GetItemText(oldItem), newParent, iPosition);
	SetNewParent(newItem, oldItem);

	SetItemData(newItem, GetItemData(oldItem));

	return newItem;
}
void CViewTree::SetNewParent(HTREEITEM newPosition, HTREEITEM oldPosition)
{
	HTREEITEM oldChild = GetChildItem(oldPosition);
	HTREEITEM newChild;
	while (oldChild){
		newChild = InsertItem(GetItemText(oldChild), newPosition, TVI_LAST);
		SetItemData(newChild, GetItemData(oldChild));
		SetNewParent(newChild, oldChild);
		oldChild = GetNextItem(oldChild, TVGN_NEXT);
	}

}

void CViewTree::UpdateComponentsParent(HTREEITEM hItem, CString parent, CString child)
{
	char c_strParent[200] = { 0 };
	char c_strChild[200]  = { 0 };

	WideCharToMultiByte(CP_ACP, 0, parent.GetString(), -1, c_strParent, 200, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, child.GetString(), -1, c_strChild, 200, 0, 0);

	DWORD_PTR itemType = GetItemData(hItem);

	switch (itemType){
	case IDS_TREE_VIEW_COMPONENT_TYPE:
		theApp.GetPresenter()->SetComponentParent(string(c_strParent), string(c_strChild));
		break;
	case IDS_TREE_VIEW_PARTICLE_SYS_TYPE:

		break;
	default:
		ASSERT(0);
	}
}


