// EditParticles.cpp: файл реализации
//

#include "stdafx.h"
#include "VisGUI.h"
#include "EditParticles.h"
#include "afxdialogex.h"


// диалоговое окно CEditParticles

IMPLEMENT_DYNAMIC(CEditParticles, CDialogEx)

CEditParticles::CEditParticles(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditParticles::IDD, pParent)
{
	m_bActive = false;

	m_pListCtrl = 0;

}

CEditParticles::~CEditParticles()
{
}

void CEditParticles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditParticles, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_EXP_KOEF_VEL_ATTEN_SPIN, &CEditParticles::OnDeltaposParticleExpKoefVelAttenSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_TIME_EVAL_EXP_KOEF_SPIN, &CEditParticles::OnDeltaposParticleTimeEvalExpKoefSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_TIME_LIFE_SPIN, &CEditParticles::OnDeltaposParticleTimeLifeSpin)
	ON_EN_CHANGE(IDC_PARTICLE_START_SIZE_EDIT, &CEditParticles::OnEnChangeParticleStartSizeEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_START_SIZE_SPIN, &CEditParticles::OnDeltaposParticleStartSizeSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_END_SIZE_SPIN, &CEditParticles::OnDeltaposParticleEndSizeSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_START_TRANSP_SPIN, &CEditParticles::OnDeltaposParticleStartTranspSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_END_TRANSP_SPIN, &CEditParticles::OnDeltaposParticleEndTranspSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_ANGULAR_VELOSITY_SPIN, &CEditParticles::OnDeltaposParticleAngularVelositySpin)
	ON_BN_CLICKED(ID_PARTICLE_DLG_CANCEL, &CEditParticles::OnBnClickedParticleDlgCancel)
	ON_BN_CLICKED(IDOK, &CEditParticles::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_PARTICLE_LIST, &CEditParticles::OnLbnDblclkParticleList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_POSITION_X_SPIN, &CEditParticles::OnDeltaposParticlePositionXSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_POSITION_Y_SPIN, &CEditParticles::OnDeltaposParticlePositionYSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_POSITION_Z_SPIN, &CEditParticles::OnDeltaposParticlePositionZSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_VELOCITY_X_SPIN, &CEditParticles::OnDeltaposParticleVelocityXSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_VELOCITY_Y_SPIN, &CEditParticles::OnDeltaposParticleVelocityYSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_VELOCITY_Z_SPIN, &CEditParticles::OnDeltaposParticleVelocityZSpin)
	ON_EN_CHANGE(IDC_PARTICLE_MAT_BROWSE, &CEditParticles::OnEnChangeParticleMatBrowse)
	ON_EN_CHANGE(IDC_PARTICLE_EXP_KOEF_VEL_ATTEN_EDIT, &CEditParticles::OnEnChangeParticleExpKoefVelAttenEdit)
	ON_EN_CHANGE(IDC_PARTICLE_TIME_LIFE_EDIT, &CEditParticles::OnEnChangeParticleTimeLifeEdit)
	ON_EN_CHANGE(IDC_PARTICLE_TIME_EVAL_EXP_KOEF_EDIT, &CEditParticles::OnEnChangeParticleTimeEvalExpKoefEdit)
	ON_EN_CHANGE(IDC_PARTICLE_END_SIZE_EDIT, &CEditParticles::OnEnChangeParticleEndSizeEdit)
	ON_EN_CHANGE(IDC_PARTICLE_START_TRANSP_EDIT, &CEditParticles::OnEnChangeParticleStartTranspEdit)
	ON_EN_CHANGE(IDC_PARTICLE_END_TRANSP_EDIT, &CEditParticles::OnEnChangeParticleEndTranspEdit)
	ON_EN_CHANGE(IDC_PARTICLE_ANGULAR_VELOSITY_EDIT, &CEditParticles::OnEnChangeParticleAngularVelosityEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PARTICLE_GRAVITY_SPIN, &CEditParticles::OnDeltaposParticleGravitySpin)
	ON_BN_CLICKED(IDC_PARTICLE_COLOR_LERP_START, &CEditParticles::OnClickedParticleColorLerpStart)
	ON_BN_CLICKED(IDC_PARTICLE_COLOR_LERP_END, &CEditParticles::OnClickedParticleColorLerpEnd)
END_MESSAGE_MAP()


// обработчики сообщений CEditParticles



BOOL CEditParticles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию

	theApp.GetPresenter()->CreateParticleSystem(string("dust_"));
	CMFCEditBrowseCtrl *pD = (CMFCEditBrowseCtrl*)GetDlgItem(IDC_PARTICLE_MAT_BROWSE);
	pD->EnableFileBrowseButton();
	
	//
	D3DXVECTOR3 Position	;
	D3DXVECTOR3 Velosity	;
							
	float fExpKoefVelAtten	;
	float fTimeEvalExpKoef	;
	float fTimeLife			;
	float fStartSize		;
	float fEndSize			;
	float fStartTransparency;
	float fEndTransparency	;
	float fAngularVelocity	;
	float fGravity			;

	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_POS,			(D3DXVECTOR3*)&Position);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_VEL,			(D3DXVECTOR3*)&Velosity);

	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_ANGULAR_VEL, (float*)&fAngularVelocity);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_START_SIZE,	(float*)&fStartSize);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_END_SIZE,	(float*)&fEndSize);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_START_TRANSP,(float*)&fStartTransparency);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_END_TRANSP,	(float*)&fEndTransparency);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_TIME_LIFE,	(float*)&fTimeLife);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_EXP_VEL_ATTEN, (float*)&fExpKoefVelAtten);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_TIME_EVAL_KOEF,(float*)&fTimeEvalExpKoef);
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_GRAVITY,		(float*)&fGravity);
	
	WCHAR wstr[200];
	char  cstr[200];

	sprintf(cstr,"%.4f",fExpKoefVelAtten);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);

	SetDlgItemText(IDC_PARTICLE_EXP_KOEF_VEL_ATTEN_EDIT,wstr);

	sprintf(cstr,"%.4f",fTimeEvalExpKoef);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);

	SetDlgItemText(IDC_PARTICLE_TIME_EVAL_EXP_KOEF_EDIT,wstr);

	sprintf(cstr,"%.4f",fTimeLife);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);

	SetDlgItemText(IDC_PARTICLE_TIME_LIFE_EDIT,			wstr);

	sprintf(cstr,"%.4f",fStartSize);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);
	SetDlgItemText(IDC_PARTICLE_START_SIZE_EDIT,		wstr);

	sprintf(cstr,"%.4f",fEndSize);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);

	SetDlgItemText(IDC_PARTICLE_END_SIZE_EDIT,			wstr);

	sprintf(cstr,"%.4f",fStartTransparency);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);

	SetDlgItemText(IDC_PARTICLE_START_TRANSP_EDIT,		wstr);

	sprintf(cstr,"%.4f",fEndTransparency);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);

	SetDlgItemText(IDC_PARTICLE_END_TRANSP_EDIT,		wstr);

	sprintf(cstr,"%.4f",fAngularVelocity);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);

	SetDlgItemText(IDC_PARTICLE_ANGULAR_VELOSITY_EDIT,	wstr);

	sprintf(cstr, "%.4f", fGravity);
	MultiByteToWideChar(CP_ACP, 0, cstr, -1, wstr, 200);

	SetDlgItemText(IDC_PARTICLE_GRAVITY_EDIT, wstr);

	//Position

	sprintf(cstr,"%.4f",Position.x);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);
	SetDlgItemText(IDC_PARTICLE_POSITION_X_EDIT,	wstr);

	sprintf(cstr,"%.4f",Position.y);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);
	SetDlgItemText(IDC_PARTICLE_POSITION_Y_EDIT,	wstr);

	sprintf(cstr,"%.4f",Position.z);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);
	SetDlgItemText(IDC_PARTICLE_POSITION_Z_EDIT,	wstr);

	//Velocity
	sprintf(cstr,"%.4f",Velosity.x);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);
	SetDlgItemText(IDC_PARTICLE_VELOCITY_X_EDIT,	wstr);

	sprintf(cstr,"%.4f",Velosity.y);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);
	SetDlgItemText(IDC_PARTICLE_VELOCITY_Y_EDIT,	wstr);

	sprintf(cstr,"%.4f",Velosity.z);
	MultiByteToWideChar(CP_ACP,0,cstr,-1,wstr,200);
	SetDlgItemText(IDC_PARTICLE_VELOCITY_Z_EDIT,	wstr);

	//

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}



void CEditParticles::OnLbnDblclkParticleList()
{
	// TODO: добавьте свой код обработчика уведомлений
	int iSel= m_pListCtrl->GetCurSel();
	CString str;
	m_pListCtrl->GetText(iSel,str);

	MessageBox(str);
}


void CEditParticles::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений

	theApp.GetPresenter()->ReleaseParticleSystem(string("dust_"));

	this->DestroyWindow();
	CDialogEx::OnCancel();
}




LRESULT CEditParticles::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: добавьте специализированный код или вызов базового класса

	return CDialogEx::WindowProc(message, wParam, lParam);
}

float CEditParticles::UpdeteEditFromSpin(DWORD ID,LPNMUPDOWN pNMUpDown)
{
	CString str;
	WCHAR	wstr[255];
	char	c_str[255] = {0};
	
	GetDlgItemText(ID,str);
	wsprintf(wstr,str.GetBuffer());

	for(int i= 0;i<255 && wstr[i]!=0;i++){
		c_str[i] = wstr[i]; 
	}

	float fValue = atof(c_str);
	
	fValue -= (float)pNMUpDown->iDelta *0.1f;

	sprintf(c_str,"%.3f",fValue);
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,c_str,-1, wstr,255);

	SetDlgItemText(ID,wstr);


	return fValue;
}

void CEditParticles::OnDeltaposParticleExpKoefVelAttenSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	float fvalue = UpdeteEditFromSpin(IDC_PARTICLE_EXP_KOEF_VEL_ATTEN_EDIT, pNMUpDown);

	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_EXP_VEL_ATTEN, (float*)&fvalue );
	*pResult = 0;
}



void CEditParticles::OnDeltaposParticleTimeEvalExpKoefSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	float fvalue = UpdeteEditFromSpin(IDC_PARTICLE_TIME_EVAL_EXP_KOEF_EDIT, pNMUpDown);

	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_TIME_EVAL_KOEF, (float*)&fvalue);

	*pResult = 0;
}


void CEditParticles::OnDeltaposParticleTimeLifeSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	float fValue =  UpdeteEditFromSpin(IDC_PARTICLE_TIME_LIFE_EDIT, pNMUpDown);

	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_TIME_LIFE, (float*)&fValue);
	*pResult = 0;
}




void CEditParticles::OnDeltaposParticleStartSizeSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	float fValue =  UpdeteEditFromSpin(IDC_PARTICLE_START_SIZE_EDIT, pNMUpDown);
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_START_SIZE, (float*)&fValue);
	*pResult = 0;
}


void CEditParticles::OnDeltaposParticleEndSizeSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue =  UpdeteEditFromSpin(IDC_PARTICLE_END_SIZE_EDIT, pNMUpDown);
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_END_SIZE, (float*)&fValue);
	*pResult = 0;
}


void CEditParticles::OnDeltaposParticleStartTranspSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue =  UpdeteEditFromSpin(IDC_PARTICLE_START_TRANSP_EDIT, pNMUpDown);
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_START_TRANSP, (float*)&fValue);
	*pResult = 0;
}


void CEditParticles::OnDeltaposParticleEndTranspSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue =  UpdeteEditFromSpin(IDC_PARTICLE_END_TRANSP_EDIT, pNMUpDown);
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_END_TRANSP, (float*)&fValue);
	*pResult = 0;
}


void CEditParticles::OnDeltaposParticleAngularVelositySpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue =  UpdeteEditFromSpin(IDC_PARTICLE_ANGULAR_VELOSITY_EDIT, pNMUpDown);
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_ANGULAR_VEL, (float*)&fValue);
	*pResult = 0;
}


void CEditParticles::OnEnChangeParticleStartSizeEdit()
{
	char cstr[255] = { 0 };
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_START_SIZE_EDIT, cstr, 255);

	//theApp.GetPresenter()->SetParticlefStartSize(atof(cstr));
	float fValue = atof(cstr);
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_START_SIZE, (float*)&fValue);
}

void CEditParticles::OnBnClickedParticleDlgCancel()
{
	theApp.GetPresenter()->ReleaseParticleSystem(string("dust_"));

	//m_bActive = false;
	//this->DestroyWindow();
	this->ShowWindow(SW_HIDE);
	//CDialogEx::OnCancel();
	// TODO: добавьте свой код обработчика уведомлений
}


void CEditParticles::OnBnClickedOk()
{
	//m_bActive = false;
	theApp.GetPresenter()->ReleaseParticleSystem(string("dust_"));
	// TODO: добавьте свой код обработчика уведомлений
	//this->DestroyWindow();
	this->ShowWindow(SW_HIDE);
	//CDialogEx::OnOK();
}


BOOL CEditParticles::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: добавьте специализированный код или вызов базового класса

	if(!m_bActive){
		m_bActive = true;
		return	CDialogEx::Create(lpszTemplateName, pParentWnd);
	}

	this->ShowWindow(SW_SHOW);
	this->SetFocus();

	return TRUE;
}


void CEditParticles::OnDeltaposParticlePositionXSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue = UpdeteEditFromSpin(IDC_PARTICLE_POSITION_X_EDIT, pNMUpDown);

	D3DXVECTOR3 Pos;
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_POS, (D3DXVECTOR3*)&Pos);
	Pos.x = fValue;
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_POS, (D3DXVECTOR3*)&Pos);

	*pResult = 0;
}


void CEditParticles::OnDeltaposParticlePositionYSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue = UpdeteEditFromSpin(IDC_PARTICLE_POSITION_Y_EDIT, pNMUpDown);


	D3DXVECTOR3 Pos;
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_POS, (D3DXVECTOR3*)&Pos);
	Pos.y = fValue;
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_POS, (D3DXVECTOR3*)&Pos);

	*pResult = 0;
}


void CEditParticles::OnDeltaposParticlePositionZSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue = UpdeteEditFromSpin(IDC_PARTICLE_POSITION_Z_EDIT, pNMUpDown);

	D3DXVECTOR3 Pos;
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_POS, (D3DXVECTOR3*)&Pos);
	Pos.z = fValue;
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_POS, (D3DXVECTOR3*)&Pos);


	*pResult = 0;
}


void CEditParticles::OnDeltaposParticleVelocityXSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue = UpdeteEditFromSpin(IDC_PARTICLE_VELOCITY_X_EDIT, pNMUpDown);

	D3DXVECTOR3 Vel;
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_VEL, (D3DXVECTOR3*)&Vel);
	Vel.x = fValue;
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_VEL, (D3DXVECTOR3*)&Vel);


	*pResult = 0;
}


void CEditParticles::OnDeltaposParticleVelocityYSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue = UpdeteEditFromSpin(IDC_PARTICLE_VELOCITY_Y_EDIT, pNMUpDown);

	D3DXVECTOR3 Vel;
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_VEL, (D3DXVECTOR3*)&Vel);
	Vel.y = fValue;
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_VEL, (D3DXVECTOR3*)&Vel);

	*pResult = 0;
}


void CEditParticles::OnDeltaposParticleVelocityZSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	float fValue = UpdeteEditFromSpin(IDC_PARTICLE_VELOCITY_Z_EDIT, pNMUpDown);

	D3DXVECTOR3 Vel;
	theApp.GetPresenter()->GetParticleSysProperty(IDS_PARTICLE_SYS_VEL, (D3DXVECTOR3*)&Vel);
	Vel.z = fValue;
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_VEL, (D3DXVECTOR3*)&Vel);

	*pResult = 0;
}


void CEditParticles::OnEnChangeParticleMatBrowse()
{
	char cstr[255] = { 0 };

	int cb = GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_MAT_BROWSE, cstr, 255);

	theApp.GetPresenter()->SetParticleSystemMaterial(string(cstr));

	// TODO:  Добавьте код элемента управления
}


void CEditParticles::OnEnChangeParticleExpKoefVelAttenEdit()
{
	char cstr[255] = {0};
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_EXP_KOEF_VEL_ATTEN_EDIT, cstr,255);

	//theApp.GetPresenter()->SetParticlefExpKoefVelAtten(atof(cstr));
	float fValue = atof(cstr);
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_EXP_VEL_ATTEN, (float*)&fValue);

}


void CEditParticles::OnEnChangeParticleTimeLifeEdit()
{
	char cstr[255] = { 0 };
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_TIME_LIFE_EDIT, cstr, 255);

	//theApp.GetPresenter()->SetParticlefTimeLife
	float fValue = (atof(cstr));
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_TIME_LIFE, (float*)&fValue);
}


void CEditParticles::OnEnChangeParticleTimeEvalExpKoefEdit()
{
	char cstr[255] = { 0 };
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_TIME_EVAL_EXP_KOEF_EDIT, cstr, 255);

	//theApp.GetPresenter()->SetParticlefTimeEvalExpKoef
	float fValue = (atof(cstr));
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_TIME_EVAL_KOEF, (float*)&fValue);
}


void CEditParticles::OnEnChangeParticleEndSizeEdit()
{
	char cstr[255] = { 0 };
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_END_SIZE_EDIT, cstr, 255);

	//theApp.GetPresenter()->SetParticlefEndSize
	float fValue = (atof(cstr));
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_END_SIZE, (float*)&fValue);
}


void CEditParticles::OnEnChangeParticleStartTranspEdit()
{
	char cstr[255] = { 0 };
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_START_TRANSP_EDIT, cstr, 255);

	//theApp.GetPresenter()->SetParticlefStartTransparency
	float fValue = (atof(cstr));
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_START_TRANSP, (float*)&fValue);
}


void CEditParticles::OnEnChangeParticleEndTranspEdit()
{
	char cstr[255] = { 0 };
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_END_TRANSP_EDIT, cstr, 255);

	//theApp.GetPresenter()->SetParticlefEndTransparency(atof(cstr));
	float fValue = (atof(cstr));
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_END_TRANSP, (float*)&fValue);
}


void CEditParticles::OnEnChangeParticleAngularVelosityEdit()
{
	char cstr[255] = { 0 };
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_PARTICLE_ANGULAR_VELOSITY_EDIT, cstr, 255);

	//theApp.GetPresenter()->SetParticlefAngularVelocity(atof(cstr));
	float fValue = (atof(cstr));
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_ANGULAR_VEL, (float*)&fValue);
}


void CEditParticles::OnDeltaposParticleGravitySpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	float fValue = UpdeteEditFromSpin(IDC_PARTICLE_GRAVITY_EDIT, pNMUpDown);

	//theApp.GetPresenter()->SetParticleSysGravity(fValue);
	theApp.GetPresenter()->SetParticleSysProperty(IDS_PARTICLE_SYS_GRAVITY, (float*)&fValue);

	*pResult = 0;
}


void CEditParticles::OnClickedParticleColorLerpStart()
{
	CMFCColorButton *pColorButtonStart = (CMFCColorButton *)GetDlgItem(IDC_PARTICLE_COLOR_LERP_START);
	CMFCColorButton *pColorButtonEnd   = (CMFCColorButton *)GetDlgItem(IDC_PARTICLE_COLOR_LERP_END);
	COLORREF colorRefStart	= pColorButtonStart->GetColor();
	COLORREF colorRefEnd	= pColorButtonEnd->GetColor();

	D3DXVECTOR3 start, end;

	start.x = ((colorRefStart >> 0 )&(0x000000ff)) / 255.0f;
	start.y = ((colorRefStart >> 8 )&(0x000000ff)) / 255.0f;
	start.z = ((colorRefStart >> 16)&(0x000000ff)) / 255.0f;

	end.x   = ((colorRefEnd	  >> 0 )&(0x000000ff)) / 255.0f;
	end.y   = ((colorRefEnd	  >> 8 )&(0x000000ff)) / 255.0f;
	end.z   = ((colorRefEnd	  >> 16)&(0x000000ff)) / 255.0f;

	//theApp.GetPresenter()->SetParticleSystemLerpColors(start,end);
}


void CEditParticles::OnClickedParticleColorLerpEnd()
{
	CMFCColorButton *pColorButtonStart = (CMFCColorButton *)GetDlgItem(IDC_PARTICLE_COLOR_LERP_START);
	CMFCColorButton *pColorButtonEnd	= (CMFCColorButton *)GetDlgItem(IDC_PARTICLE_COLOR_LERP_END);
	COLORREF colorRefStart = pColorButtonStart->GetColor();
	COLORREF colorRefEnd = pColorButtonEnd->GetColor();

	D3DXVECTOR3 start, end;

	start.x = ((colorRefStart >> 0 )&(0x000000ff)) / 255.0f;
	start.y = ((colorRefStart >> 8 )&(0x000000ff)) / 255.0f;
	start.z = ((colorRefStart >> 16)&(0x000000ff)) / 255.0f;

	end.x	= ((colorRefEnd	  >> 0 )&(0x000000ff)) / 255.0f;
	end.y	= ((colorRefEnd	  >> 8 )&(0x000000ff)) / 255.0f;
	end.z	= ((colorRefEnd	  >> 16)&(0x000000ff)) / 255.0f;

	//theApp.GetPresenter()->SetParticleSystemLerpColors(start, end);
}
