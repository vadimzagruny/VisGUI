#pragma once


// диалоговое окно CEditParticles

class CEditParticles : public CDialogEx
{
	DECLARE_DYNAMIC(CEditParticles)

	float UpdeteEditFromSpin(DWORD ID,LPNMUPDOWN);

	BOOL m_bActive;

	CListBox *m_pListCtrl;

public:
	CEditParticles(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CEditParticles();

// Данные диалогового окна
	enum { IDD = IDD_PARTICLE_EDITOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDeltaposParticleExpKoefVelAttenSpin(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnDeltaposParticleTimeEvalExpKoefSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticleTimeLifeSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeParticleStartSizeEdit();
	afx_msg void OnDeltaposParticleStartSizeSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticleEndSizeSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticleStartTranspSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticleEndTranspSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticleAngularVelositySpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedParticleDlgCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnLbnDblclkParticleList();
	afx_msg void OnDeltaposParticlePositionXSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticlePositionYSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticlePositionZSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticleVelocityXSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticleVelocityYSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposParticleVelocityZSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeParticleMatBrowse();
	afx_msg void OnEnChangeParticleExpKoefVelAttenEdit();
	afx_msg void OnEnChangeParticleTimeLifeEdit();
	afx_msg void OnEnChangeParticleTimeEvalExpKoefEdit();
	afx_msg void OnEnChangeParticleEndSizeEdit();
	afx_msg void OnEnChangeParticleStartTranspEdit();
	afx_msg void OnEnChangeParticleEndTranspEdit();
	afx_msg void OnEnChangeParticleAngularVelosityEdit();
	afx_msg void OnDeltaposParticleGravitySpin(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnClickedParticleColorLerpStart();
	afx_msg void OnClickedParticleColorLerpEnd();
};
