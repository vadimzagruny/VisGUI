
// VisGUIView.cpp : ���������� ������ CVisGUIView
//

#include "stdafx.h"
// SHARED_HANDLERS ����� ���������� � ������������ �������� ��������� ���������� ������� ATL, �������
// � ������; ��������� ��������� ������������ ��� ��������� � ������ �������.
#ifndef SHARED_HANDLERS
#include "VisGUI.h"
#endif

#include "VisGUIDoc.h"
#include "VisGUIView.h"


#include "World.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisGUIView

IMPLEMENT_DYNCREATE(CVisGUIView, CView)

BEGIN_MESSAGE_MAP(CVisGUIView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EDIT_EDITWEATHER, &CVisGUIView::OnEditEditweather)
	ON_COMMAND(ID_LOAD_MODEL, &CVisGUIView::OnLoadModel)
END_MESSAGE_MAP()

// ��������/����������� CVisGUIView

CVisGUIView::CVisGUIView():m_WetherEditDlg()
{
	// TODO: �������� ��� ��������
	m_bGPortAktive = false;
}

CVisGUIView::~CVisGUIView()
{
}

BOOL CVisGUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �������� ����� Window ��� ����� ����������� ���������
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// ��������� CVisGUIView

void CVisGUIView::OnDraw(CDC* /*pDC*/)
{
	CVisGUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//if(m_bGPortAktive)
	//	theApp.GetPresenter()->Paint();//theApp.GetGraphicPort()->Update();

	// TODO: �������� ����� ��� ��������� ��� ����������� ������
}

void CVisGUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVisGUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// ����������� CVisGUIView

#ifdef _DEBUG
void CVisGUIView::AssertValid() const
{
	CView::AssertValid();
}

void CVisGUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVisGUIDoc* CVisGUIView::GetDocument() const // �������� ������������ ������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisGUIDoc)));
	return (CVisGUIDoc*)m_pDocument;
}
#endif //_DEBUG


// ����������� ��������� CVisGUIView


void CVisGUIView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	if(!m_bGPortAktive){
		/*theApp.GetGraphicPort()->Create(GetSafeHwnd());
		theApp.GetWorld()->Create(GetSafeHwnd());*/
		theApp.GetPresenter()->CreateDevice(GetSafeHwnd());
		m_bGPortAktive = true;
	}
	// TODO: �������� ������������������ ��� ��� ����� �������� ������
}


BOOL CVisGUIView::OnEraseBkgnd(CDC* pDC)
{
	//CWnd::OnEraseBkgnd(pDC);
	return true;
}

void CVisGUIView::OnSize(UINT nType, int cx, int cy)
{

	//if(m_bGPortAktive)
	//theApp.GetGraphicPort()-> ResizeViewPort(cx,  cy) ;
	//

	return CView::OnSize(nType,cx,cy);
}

BOOL CVisGUIView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �������� ������������������ ��� ��� ����� �������� ������

	return CView::OnCommand(wParam, lParam);
}


void CVisGUIView::OnEditEditweather()
{
	// TODO: �������� ���� ��� ����������� ������

	m_WetherEditDlg.Create(MAKEINTRESOURCE(IDD_WEATHER_EDIT_DIALOG),this);
}



void CVisGUIView::OnLoadModel()
{
	// TODO: �������� ���� ��� ����������� ������
}
