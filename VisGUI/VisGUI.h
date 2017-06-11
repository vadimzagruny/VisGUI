
// VisGUI.h : ������� ���� ��������� ��� ���������� VisGUI
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������
#include "afxwinappex.h"
//
class GFrameTimer;
class GraphicPort;
class SoundPort;
class CShaderResourse;
class CSharedResource;
class CWorld;
class IPresenter;
class CFileManager;
class CLogger;

/**
\brief VisGUI Application class.
*  \author    Vadim Zagruny
*  \version   2.1
*  \date      2017
*  \copyright  Vadim Zagruny KhMBDB.
*/
class CVisGUIApp : public CWinAppEx
{
public:
	CVisGUIApp();

	//
	GraphicPort		*GetGraphicPort()	{return m_pGrPort;}
	CWorld			*GetWorld()			{return m_pWorld;}
	GFrameTimer		*GetTimer()			{return m_pTimer;}
	CSharedResource *GetSharedResource(){return m_pSharedResource;}
	CShaderResourse *GetShaderResourse(){return m_pShaderResourse;}
	CFileManager	*GetFileManager()	{return m_pFileManager;}

	CLogger			*GetFileLogger()	{ return m_pFileLogger; }

	char* GetMessageSource(){return m_szFuckingMessageSource;}
	void SetMessageSource(const char* src);

	IPresenter *GetPresenter(){return m_pPresenter;}

protected:
	GFrameTimer		*m_pTimer;			//������ ������ 
	CWorld			*m_pWorld;			//���������� ���
	GraphicPort		*m_pGrPort;			//�������
	CFileManager	*m_pFileManager;	//���� ��������
	CSharedResource	*m_pSharedResource;
	CShaderResourse *m_pShaderResourse; //�������

	CLogger			*m_pFileLogger;//������

	char*			m_szFuckingMessageSource;


	IPresenter *m_pPresenter;

// ���������������
public:
	virtual BOOL InitInstance();

// ����������
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual void OnFinalRelease();
	virtual void OnClosingMainFrame(CFrameImpl* pFrameImpl);
};

extern CVisGUIApp theApp;

//CVisGUIApp *GetApp(){return &theApp;}