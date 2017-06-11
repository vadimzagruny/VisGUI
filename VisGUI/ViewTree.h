
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Окно CViewTree

class CViewTree : public CTreeCtrl
{
// Создание
public:
	CViewTree();

// Переопределение
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	HTREEITEM m_hitemDrag;
	HTREEITEM m_hitemDrop;

	HTREEITEM CopyBranch(HTREEITEM oldItem, HTREEITEM newParent, HTREEITEM iPosition);
	void SetNewParent(HTREEITEM newPosition, HTREEITEM oldPosition);
	void UpdateComponentsParent(HTREEITEM hItem, CString parent, CString child);

private:
	BOOL m_bDragging;

// Реализация
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
};
