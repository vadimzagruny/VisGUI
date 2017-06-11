// InputPort.cpp: implementation of the CInputPort class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "GraphicPort.h"
#include "camera.h"
#include "Clients.h"
#include "InputPort.h"

#ifdef _DETECT_MEMORY_LEAKS

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif // _DETECT_MEMORY_LEAKS


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInputPort::CInputPort()
{
	m_pDInput = NULL;
	m_pKeyboardDevice = NULL;
	m_pMouseDevice = NULL;

	m_pCamera = NULL;

	m_fSpeedZ = 0;
	m_fSpeedX = 0;
	m_Camera.y   = 50.0f;
	m_Camera.Yaw = -0.02f;
	memset(m_KeyBuffer,0,sizeof(m_KeyBuffer));

	m_vecCamOrigin = D3DXVECTOR3(0,0,0);

	m_fDistance = 5.0f;
}

CInputPort::~CInputPort()
{
	HRESULT hr = m_pKeyboardDevice->Unacquire();
	ASSERT(hr == DI_OK);

	hr = m_pMouseDevice->Unacquire();
	ASSERT(hr == DI_OK);

	
	SAFE_RELEASE(m_pKeyboardDevice);
	SAFE_RELEASE(m_pMouseDevice);

	SAFE_RELEASE(m_pDInput);
}

bool CInputPort::Create(HWND hWnd,CCamera *pCamera)
{

	m_hWnd = hWnd;

	HRESULT hr;
	hr = DirectInput8Create(GetModuleHandle(NULL),
						    DIRECTINPUT_VERSION,
							IID_IDirectInput8,
							(void **)&m_pDInput,
							NULL);
	ASSERT(hr == DI_OK);

	hr = m_pDInput->CreateDevice(GUID_SysKeyboard,
								 &m_pKeyboardDevice,
								 NULL);
	ASSERT(hr == DI_OK);

	hr = m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard); 
	ASSERT(hr == DI_OK);

	hr = m_pKeyboardDevice->SetCooperativeLevel(hWnd,DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);  //
	ASSERT(hr == DI_OK);
 
	hr = m_pKeyboardDevice->Acquire();
	ASSERT(hr == DI_OK);

	hr = m_pDInput->CreateDevice(GUID_SysMouse,
								 &m_pMouseDevice,
								 NULL);
	ASSERT(hr == DI_OK);

	hr = m_pMouseDevice->SetDataFormat(&c_dfDIMouse); 
	ASSERT(hr == DI_OK);

	hr = m_pMouseDevice->SetCooperativeLevel(hWnd,DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);  //DISCL_NONEXCLUSIVE
	ASSERT(hr == DI_OK);
 
	hr = m_pMouseDevice->Acquire();
	ASSERT(hr == DI_OK);

	m_pCamera = pCamera;

	return true;
}
#define SPEED 0.3f
bool CInputPort::Update(float fDeltaTime)
{
	//Application *a=GetApp();
	CTarget *target = const_cast<CTarget*>(m_pCamera->GetObservedTarget());
#define _DIALOG
#ifdef _DIALOG

	HRESULT hr = m_pKeyboardDevice->GetDeviceState(256,m_KeyBuffer);
	if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
	{
		hr = m_pKeyboardDevice->Acquire();
	}

	hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),&m_MouseBuffer);
	if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
	{
		hr = m_pMouseDevice->Acquire();
	}


	D3DXMATRIX mat;
	if (m_MouseBuffer.rgbButtons[0]){
		m_Camera.Yaw	+= m_MouseBuffer.lX * 0.01f;
		m_Camera.Pitch	+= m_MouseBuffer.lY * 0.01f;

		
	}
		D3DXMatrixRotationYawPitchRoll(&mat,m_Camera.Yaw, m_Camera.Pitch, m_Camera.Roll);

	
		static float XOffset = 0.0f, YOffset = 0.f;


		m_Camera.x = -mat._31 *m_fDistance - XOffset;
		m_Camera.y = -mat._32 *m_fDistance - YOffset;
		m_Camera.z = -mat._33 *m_fDistance;





	if (m_MouseBuffer.rgbButtons[2]){
		XOffset += m_MouseBuffer.lX * 0.1f;//m_Camera.x	+= m_MouseBuffer.lX * 0.1f;
		YOffset += m_MouseBuffer.lY * 0.1f;//m_Camera.y	+= m_MouseBuffer.lY * 0.1f;
	
	}
	

	

	m_fDistance += m_MouseBuffer.lZ * 0.0015f;
	if(m_fDistance<2){
		m_fDistance = 2.5;
	}

	m_PData.camera = m_Camera;
	m_PData.camera.Yaw		*= -1;
	m_PData.camera.Pitch	*= -1;
	m_PData.camera.Roll		*= -1;
	m_pCamera->ProcessMessage(&m_PData);

#else
	

	if(GetFocus()!=theApp.GetMainWnd()->GetSafeHwnd()){
		if (target)
		{
			D3DXMATRIX mat;
			D3DXMatrixRotationYawPitchRoll(&mat,
										   m_Camera.Yaw,
										   m_Camera.Pitch,
										   m_Camera.Roll);

	// 		CTarget *target = const_cast<CTarget*>(m_pCamera->GetObservedTarget());
			mat._41 +=0.f;
			mat._42 +=5.f;
			mat._43 -=5.f;


			D3DXMATRIX mat2, targetMat;
			targetMat = target->m_MyMat;

//			D3DXMatrixMultiply(&mat2, &mat, &targetMat);
			mat2 = targetMat;
// 			mat2._41 +=0.f;
// 			mat2._42 +=5.f;
// 			mat2._43 -=10.f;

			float pitch = (float)asin(mat2._32/*dir[1]*/);
			float yaw   = (float)atan2(-mat2._31/*-dir[0]*/, mat2._33/*dir[2]*/);
			float roll  = (float)asin((mat2._33/*dir[2]*/ * mat2._21/*up[0]*/
											 -mat2._31/*dir[0]*/ * mat2._23/*up[2]*/)
											 / cos(pitch/*alpha*/));
 			m_Camera.x = mat2._41;
 			m_Camera.y = mat2._42;
 			m_Camera.z = mat2._43;

			*(D3DXVECTOR3*)&m_Camera.x += *(D3DXVECTOR3*)&mat2._21 * 5.f;//0.256f наверно вверх-вниз
			*(D3DXVECTOR3*)&m_Camera.x += *(D3DXVECTOR3*)&mat2._31 * -10.0f;//1.53f наверно вперед-назад
			*(D3DXVECTOR3*)&m_Camera.x -= *(D3DXVECTOR3*)&mat2._11 * 0.0f;//вправо-влево

			m_PData.camera = m_Camera;
			m_PData.camera.Yaw = yaw;
			m_PData.camera.Pitch = pitch-0.2f;
			m_PData.camera.Roll = roll;
// 			m_PData.camera.Yaw *= -1;
// 			m_PData.camera.Pitch *= -1;
// 			m_PData.camera.Roll *= -1;
			m_pCamera->ProcessMessage(&m_PData);
		}
		return true;
	}
//*
	HRESULT hr = m_pKeyboardDevice->GetDeviceState(256,m_KeyBuffer);
	if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
	{
		hr = m_pKeyboardDevice->Acquire();
	}

	hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),&m_MouseBuffer);
	if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
	{
		hr = m_pMouseDevice->Acquire();
	}
	
	if(m_pCamera)
	if (target)
	{
		//нажатием двух кнопок - снимаем наблюдение с цели
		if (m_MouseBuffer.rgbButtons[0] && m_MouseBuffer.rgbButtons[1])
		{
			m_pCamera->SetObservedTarget(NULL);
			return true;
		}

		m_Camera.Yaw += m_MouseBuffer.lX * 0.01f;
		m_Camera.Pitch += m_MouseBuffer.lY * 0.01f;
		if (m_Camera.Pitch>1.0f) m_Camera.Pitch=1.0f;
		if (m_Camera.Pitch<-0.2f) m_Camera.Pitch=-0.2f;
		D3DXMATRIX mat, mat1, mat2, targetMat;

		D3DXMatrixRotationYawPitchRoll(&mat,
									   m_Camera.Yaw,
									   m_Camera.Pitch,
									   m_Camera.Roll);

// 		CTarget *target = const_cast<CTarget*>(m_pCamera->GetObservedTarget());
		targetMat = target->m_MyMat;
//		mat1=targetMat;
		D3DXMatrixIdentity(&mat1);

// 		mat1._41 =0.f;
// 		mat1._42 =5.f;
// 		mat1._43 =-10.f;

		D3DXMatrixMultiply(&mat1, &mat1, &mat);


		D3DXMatrixMultiply(&mat2, &mat1, &targetMat);

		m_Camera.x = mat2._41;
		m_Camera.y = mat2._42;
		m_Camera.z = mat2._43;

		float pitch = (float)asin(mat._32/*dir[1]*/);
		float yaw   = (float)atan2(-mat._31/*-dir[0]*/, mat._33/*dir[2]*/);
		float roll  = (float)asin((mat._33/*dir[2]*/ * mat._21/*up[0]*/
										 -mat._31/*dir[0]*/ *mat._23/*up[2]*/)
										 / cos(pitch/*alpha*/));
		
//		TRACE("m32= %f,   pitch %f \n", mat2._32, pitch);

		static float dist =1.f;
		dist += m_MouseBuffer.lZ * 0.0005f;
		if (dist<0.5f) dist=0.5f;
		
		*(D3DXVECTOR3*)&m_Camera.x += *(D3DXVECTOR3*)&mat._21 * 5.f;//0.256f наверно вверх-вниз
		*(D3DXVECTOR3*)&m_Camera.x += *(D3DXVECTOR3*)&mat._31 * (-10.0f*dist);//1.53f наверно вперед-назад
		*(D3DXVECTOR3*)&m_Camera.x -= *(D3DXVECTOR3*)&mat._11 * 0.0f;//вправо-влево

		m_PData.camera = m_Camera;
		m_PData.camera.Yaw = yaw;
		m_PData.camera.Pitch = pitch-0.2f;
		m_PData.camera.Roll = roll;
// 		m_PData.camera.Yaw *= -1;
// 		m_PData.camera.Pitch *= -1;
// 		m_PData.camera.Roll *= -1;
		m_pCamera->ProcessMessage(&m_PData);
	}
	else
	{


		if(m_pCamera)
		{
 			#ifdef _NONETWORK
 			float dx=0.f;
 			float dy=0.f;
 			if(m_KeyBuffer[DIK_LEFT] && 0x80)
 				dx=-0.00001f;
 			if(m_KeyBuffer[DIK_RIGHT] && 0x80)
 				dx=0.00001f;
 			if(m_KeyBuffer[DIK_UP] && 0x80)
 				dy=-0.00001f;
 			if(m_KeyBuffer[DIK_DOWN] && 0x80)
 				dy=0.00001f;	
 				
 				
 
 			m_Camera.Yaw += m_MouseBuffer.lX * 0.002f + dx;
 			m_Camera.Pitch += m_MouseBuffer.lY * 0.002f +dy;
 
 			#else
			m_Camera.Yaw += m_MouseBuffer.lX * 0.01f;
			m_Camera.Pitch += m_MouseBuffer.lY * 0.01f;
			#endif
			
			m_Camera.y += m_MouseBuffer.lZ * 0.0015f;
// 			m_Camera.Yaw = 1.0f;
// 			m_Camera.Pitch = 1.0f;
	

			D3DXMATRIX mat;
			D3DXMatrixRotationYawPitchRoll(&mat,
										   m_Camera.Yaw,
										   m_Camera.Pitch,
										   m_Camera.Roll);
			bool bBreakX = true;
			bool bBreakZ = true;
			if((m_KeyBuffer[DIK_W] && 0x80) | m_MouseBuffer.rgbButtons[0])
			{
				m_fSpeedZ += SPEED * fDeltaTime;	
				bBreakZ = false;
			}

			if((m_KeyBuffer[DIK_S] && 0x80)| m_MouseBuffer.rgbButtons[1])
			{
				m_fSpeedZ += -SPEED * fDeltaTime;	
				bBreakZ = false;
			}

			if(m_KeyBuffer[DIK_A] && 0x80)
			{
				m_fSpeedX += -SPEED * fDeltaTime;	
				bBreakX = false;
			}

			if(m_KeyBuffer[DIK_D] && 0x80)
			{
				m_fSpeedX += SPEED * fDeltaTime;	
				bBreakX = false;
			}

			m_fSpeedX = m_fSpeedX > 60.0f ? 60.0f : m_fSpeedX;
			m_fSpeedZ = m_fSpeedZ > 60.0f ? 60.0f : m_fSpeedZ;

			m_fSpeedX = m_fSpeedX < -60.0f ? -60.0f : m_fSpeedX;
			m_fSpeedZ = m_fSpeedZ < -60.0f ? -60.0f : m_fSpeedZ;
			
						
			m_Camera.x += mat._31 * m_fSpeedZ + mat._11 * m_fSpeedX; 
			m_Camera.y += mat._32 * m_fSpeedZ + mat._12 * m_fSpeedX; 
			m_Camera.z += mat._33 * m_fSpeedZ + mat._13 * m_fSpeedX; 

			if(bBreakX)
			{
				if(m_fSpeedX > 0.0f)
				{
					m_fSpeedX -= 3.0f * fDeltaTime; 
					if(m_fSpeedX < 0.0f)
						m_fSpeedX = 0.0f;
				}

				if(m_fSpeedX < 0.0f)
				{
					m_fSpeedX += 3.0f * fDeltaTime; 
					if(m_fSpeedX > 0.0f)
						m_fSpeedX = 0.0f;
				}
			}

			if(bBreakZ)
			{
				if(m_fSpeedZ > 0.0f)
				{
					m_fSpeedZ -= 3.0f * fDeltaTime; 
					if(m_fSpeedZ < 0.0f)
						m_fSpeedZ = 0.0f;
				}

				if(m_fSpeedZ < 0.0f)
				{
					m_fSpeedZ += 3.0f * fDeltaTime; 
					if(m_fSpeedZ > 0.0f)
						m_fSpeedZ = 0.0f;
				}
			}

			m_PData.camera = m_Camera;
			m_PData.camera.Yaw *= -1;
			m_PData.camera.Pitch *= -1;
			m_PData.camera.Roll *= -1;
			m_pCamera->ProcessMessage(&m_PData);
		}
	}
#endif
	return true;
//*/
}


bool CInputPort::ProcessMessage(const void *pData)
{
	ASSERT(pData);
	CAMERA *pCamera = (CAMERA *)(pData);
	m_Camera= *pCamera;
	return true;
}
