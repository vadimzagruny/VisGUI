
// VisGUIView.h : интерфейс класса CVisGUIView
//

#pragma once

#include "WeatherEditor.h"

class CVisGUIView : public CView
{
protected: // создать только из сериализации
	CVisGUIView();
	DECLARE_DYNCREATE(CVisGUIView)

// Атрибуты

	bool m_bGPortAktive;

	CWeatherEditor m_WetherEditDlg;



public:
	CVisGUIDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CVisGUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditEditweather();
	afx_msg void OnLoadModel();
};

#ifndef _DEBUG  // отладочная версия в VisGUIView.cpp
inline CVisGUIDoc* CVisGUIView::GetDocument() const
   { return reinterpret_cast<CVisGUIDoc*>(m_pDocument); }
#endif

