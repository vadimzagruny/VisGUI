#pragma once

#include "clients.h"

// ���������� ���� CWeatherEditor

/**
\brief Weather Editor Class to edit weather params.
*  \author    Vadim Zagruny
*  \version   2.1
*  \date      2017
*  \copyright  Vadim Zagruny KhMBDB.
*/
class CWeatherEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CWeatherEditor)
	bool m_bActive;

	SETWEATHER m_setWeather;

	CSliderCtrl *m_pSliderHourCtrl;
	CSliderCtrl *m_pSliderMinuteCtrl;
	CSliderCtrl *m_pSliderFogCtrl;
	CSliderCtrl *m_pSliderWindCtrl;

public:
	CWeatherEditor(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CWeatherEditor();

// ������ ����������� ����
	enum { IDD = IDD_WEATHER_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawWeather(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawWeatherMinuteSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawWeatherFogSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawWeatherWindSlider(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
};
