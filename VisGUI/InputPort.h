// InputPort.h: interface for the CInputPort class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _INPUTPORT_H__INCLUDED_
#define _INPUTPORT_H__INCLUDED_

#include <dinput.h>

typedef struct _PROCESSDATA
{
	CAMERA	 camera;
}PROCESSDATA;

class CCamera;
class CInputPort : public CNetObject 
{
private:

	LPDIRECTINPUT8			m_pDInput;
	LPDIRECTINPUTDEVICE8	m_pKeyboardDevice;
	LPDIRECTINPUTDEVICE8	m_pMouseDevice;

	DIMOUSESTATE			m_MouseBuffer; 
	char					m_KeyBuffer[256]; 

	PROCESSDATA				m_PData;
	CAMERA					m_Camera;
	CCamera					*m_pCamera;

	float					m_fSpeedZ;
	float					m_fSpeedX;

	float					m_fDistance;
	D3DXVECTOR3				m_vecCamOrigin;				
	HWND m_hWnd;
public:
	bool ProcessMessage(const void *pData);
	CInputPort();
	virtual ~CInputPort();

	bool Create(HWND hWnd,CCamera *pCamera = NULL);
	bool Update(float fDeltaTime);

	char *GetKeyBuffer(){return m_KeyBuffer;}


};

#endif // _INPUTPORT_H__INCLUDED_
