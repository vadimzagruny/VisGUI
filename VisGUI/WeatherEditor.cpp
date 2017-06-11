// WeatherEditor.cpp: файл реализации
//

#include "stdafx.h"
#include "VisGUI.h"
#include "WeatherEditor.h"
#include "afxdialogex.h"


#include "World.h"

// диалоговое окно CWeatherEditor

IMPLEMENT_DYNAMIC(CWeatherEditor, CDialogEx)

CWeatherEditor::CWeatherEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWeatherEditor::IDD, pParent)
{
	m_bActive = false;

	memset(&m_setWeather,0,sizeof(SETWEATHER));

	m_pSliderHourCtrl	= NULL ;
	m_pSliderMinuteCtrl	= NULL ;
	m_pSliderFogCtrl	= NULL ;
	m_pSliderWindCtrl	= NULL ;

}

CWeatherEditor::~CWeatherEditor()
{
}

void CWeatherEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWeatherEditor, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WEATHER__HOUR_SLIDER, &CWeatherEditor::OnNMCustomdrawWeather)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WEATHER_MINUTE_SLIDER, &CWeatherEditor::OnNMCustomdrawWeatherMinuteSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WEATHER_FOG_SLIDER, &CWeatherEditor::OnNMCustomdrawWeatherFogSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WEATHER_WIND_SLIDER, &CWeatherEditor::OnNMCustomdrawWeatherWindSlider)
END_MESSAGE_MAP()


// обработчики сообщений CWeatherEditor


void CWeatherEditor::OnNMCustomdrawWeather(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	int iVal = m_pSliderHourCtrl->GetPos();
	SetDlgItemInt(IDC_WEATHER_HOUR_EDIT,iVal,false);
	
	m_setWeather.iHour = iVal;
	theApp.GetPresenter()->SetWeather(&m_setWeather);//theApp.GetWorld()->SetWeather(&m_setWeather);
	
	*pResult = 0;
}


void CWeatherEditor::OnNMCustomdrawWeatherMinuteSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	int iVal = m_pSliderMinuteCtrl->GetPos();
	SetDlgItemInt(IDC_WEATHER_MINUTE_EDIT,iVal,false);

	m_setWeather.iMin = iVal;
	theApp.GetPresenter()->SetWeather(&m_setWeather);//theApp.GetWorld()->SetWeather(&m_setWeather);

	*pResult = 0;
}


void CWeatherEditor::OnNMCustomdrawWeatherFogSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	int iVal = m_pSliderFogCtrl->GetPos();
	SetDlgItemInt(IDC_WEATHER__FOG_EDIT,iVal,false);

	m_setWeather.fFogIntensity = ((float)iVal/100.0f);
	theApp.GetPresenter()->SetWeather(&m_setWeather);//theApp.GetWorld()->SetWeather(&m_setWeather);

	*pResult = 0;
}


void CWeatherEditor::OnNMCustomdrawWeatherWindSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	
	int iVal = m_pSliderWindCtrl->GetPos();
	SetDlgItemInt(IDC_WEATHER__WIND_EDIT,iVal,false);
	
	m_setWeather.fSpeedWind = (float)iVal;
	theApp.GetPresenter()->SetWeather(&m_setWeather);//theApp.GetWorld()->SetWeather(&m_setWeather);

	*pResult = 0;
}


void CWeatherEditor::OnCancel()
{
	// TODO: добавьте специализированный код или вызов базового класса
	m_bActive = false;
	this->DestroyWindow();//CDialogEx::OnCancel();
}


void CWeatherEditor::OnOK()
{
	// TODO: добавьте специализированный код или вызов базового класса
	m_bActive = false;
	this->DestroyWindow();//CDialogEx::OnOK();
}


BOOL CWeatherEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Добавить дополнительную инициализацию


	m_pSliderHourCtrl	= (CSliderCtrl*)GetDlgItem(IDC_WEATHER__HOUR_SLIDER);
	m_pSliderMinuteCtrl = (CSliderCtrl*)GetDlgItem(IDC_WEATHER_MINUTE_SLIDER);
	m_pSliderFogCtrl	= (CSliderCtrl*)GetDlgItem(IDC_WEATHER_FOG_SLIDER);
	m_pSliderWindCtrl	= (CSliderCtrl*)GetDlgItem(IDC_WEATHER_WIND_SLIDER);

	m_pSliderFogCtrl->SetRange(0,100);
	m_pSliderWindCtrl->SetRange(0,30);
	m_pSliderHourCtrl->SetRange(0,23);
	m_pSliderMinuteCtrl->SetRange(0,59);

	m_pSliderHourCtrl->SetPos(10);
	m_pSliderMinuteCtrl->SetPos(23);

	m_setWeather.iHour = 10;
	m_setWeather.iHour = 23;

	theApp.GetPresenter()->SetWeather(&m_setWeather);//theApp.GetWorld()->SetWeather(&m_setWeather);

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


BOOL CWeatherEditor::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: добавьте специализированный код или вызов базового класса
	if(!m_bActive){
		m_bActive = true;
		return CDialogEx::Create(lpszTemplateName, pParentWnd);
	}

	this->SetFocus();

	return TRUE;
}
