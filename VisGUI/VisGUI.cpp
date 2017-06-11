
// VisGUI.cpp : Определяет поведение классов для приложения.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "VisGUI.h"
#include "MainFrm.h"

#include "VisGUIDoc.h"
#include "VisGUIView.h"


///
#include "frametimer.h"
#include "VisSlave.h"
#include "World.h"
#include "GraphicPort.h"

#include "SharedResource.h"
#include "SharedShaderResource.h"
#include "FileManager.h"
#include "Presenter.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisGUIApp

BEGIN_MESSAGE_MAP(CVisGUIApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CVisGUIApp::OnAppAbout)
	// Стандартные команды по работе с файлами документов
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// создание CVisGUIApp

CVisGUIApp::CVisGUIApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: замените ниже строку идентификатора приложения строкой уникального идентификатора; рекомендуемый
	// формат для строки: ИмяКомпании.ИмяПродукта.СубПродукт.СведенияОВерсии
	SetAppID(_T("KhMBDB.VisGUI.2.0"));

	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance

	m_szFuckingMessageSource = NULL;

	m_pTimer			= nullptr;
	m_pPresenter		= nullptr;
	m_pWorld			= nullptr;
	m_pSharedResource	= nullptr;
	m_pShaderResourse	= nullptr;
	m_pFileManager		= nullptr;
	m_pGrPort			= nullptr;
	m_pFileLogger		= nullptr;

}

void CVisGUIApp::SetMessageSource(const char* src)
{
	if(m_szFuckingMessageSource != NULL) delete m_szFuckingMessageSource;

	m_szFuckingMessageSource = new char[strlen(src)+1];
	strcpy(m_szFuckingMessageSource,src);
}


// Единственный объект CVisGUIApp

CVisGUIApp theApp;


// инициализация CVisGUIApp

BOOL CVisGUIApp::InitInstance()
{
	/*{
		CSingleDocTemplate* pNewDocTemplate = new CSingleDocTemplate(
			RUNTIME_CLASS(CVisGUIDoc),
			RUNTIME_CLASS(CMainFrame));
		AddDocTemplate(pNewDocTemplate);
	}*/
	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// Для использования элемента управления RichEdit требуется метод AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// Стандартная инициализация
	// Если эти возможности не используются и необходимо уменьшить размер
	// конечного исполняемого файла, необходимо удалить из следующего
	// конкретные процедуры инициализации, которые не требуются
	// Измените раздел реестра, в котором хранятся параметры
	// TODO: следует изменить эту строку на что-нибудь подходящее,
	// например на название организации
	SetRegistryKey(_T("KhMBDB"));
	LoadStdProfileSettings(4);  // Загрузите стандартные параметры INI-файла (включая MRU)
	Unregister();

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Зарегистрируйте шаблоны документов приложения. Шаблоны документов
	//  выступают в роли посредника между документами, окнами рамок и представлениями
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVisGUIDoc),
		RUNTIME_CLASS(CMainFrame),       // основное окно рамки SDI
		RUNTIME_CLASS(CVisGUIView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Разрешить использование расширенных символов в горячих клавишах меню
	CMFCToolBar::m_bExtCharTranslation = TRUE;

	// Синтаксический разбор командной строки на стандартные команды оболочки, DDE, открытие файлов
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


		// Init vis components
	m_pSharedResource = new CSharedResource;
	m_pShaderResourse = new CShaderResourse;
	m_pTimer		  = new GFrameTimer;
	m_pTimer->Start(25.0f);
	m_pGrPort		  =	new GraphicPort;
	m_pFileManager	  = new CFileManager;
	m_pWorld		  = new CWorld;
	m_pFileLogger	  = new CFileLogger(string("logger"));
	m_pPresenter = new CPresenter;
	//m_pPresenter->Init();
	m_pFileManager->Init();

	// Команды диспетчеризации, указанные в командной строке. Значение FALSE будет возвращено, если
	// приложение было запущено с параметром /RegServer, /Register, /Unregserver или /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Одно и только одно окно было инициализировано, поэтому отобразите и обновите его
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	//

	return TRUE;
}

// обработчики сообщений CVisGUIApp


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Команда приложения для запуска диалога
void CVisGUIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CVisGUIApp настройка методов загрузки и сохранения

void CVisGUIApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CVisGUIApp::LoadCustomState()
{
}

void CVisGUIApp::SaveCustomState()
{
}

// обработчики сообщений CVisGUIApp

BOOL CVisGUIApp::OnIdle(LONG lCount)
{
	// TODO: добавьте специализированный код или вызов базового класса

	m_pTimer->Frame();
	float fDeltaTime = m_pTimer->GetSecsPerFrame();

	char str[255];
	sprintf(str,"FPS = %f",1.0f / fDeltaTime);


	m_pGrPort->DrawText(0.03f,0.02f,string(str));

	m_pPresenter->Update(fDeltaTime);
	m_pWorld->Update(fDeltaTime);
	m_pShaderResourse->Update(fDeltaTime);
	m_pSharedResource->Update(fDeltaTime);
	
	m_pPresenter->Paint();

	theApp.GetMainWnd()->Invalidate(false);

	return true;//CWinAppEx::OnIdle(lCount);
}


void CVisGUIApp::OnFinalRelease()
{
	// TODO: добавьте специализированный код или вызов базового класса

	

	CWinAppEx::OnFinalRelease();
}


void CVisGUIApp::OnClosingMainFrame(CFrameImpl* pFrameImpl)
{
	// TODO: добавьте специализированный код или вызов базового класса

	/*m_pWorld->Destroy();
	delete m_pWorld;*/
	m_pPresenter->Destroy();
	delete (CPresenter*)m_pPresenter;

	m_pSharedResource->ClearCach();
	delete m_pSharedResource;

	delete m_pShaderResourse;

	m_pGrPort->Destroy();
	delete m_pGrPort;

	delete m_pTimer;

	delete m_pFileManager;

	delete m_pFileLogger;

	return CWinAppEx::OnClosingMainFrame(pFrameImpl);
}
