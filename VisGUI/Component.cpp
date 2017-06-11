// Component.cpp: implementation of the CComponent class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "RTClass.h"
#include "VisGUI.h"
#include "GraphicPort.h"
#include "MeshSource.h"
#include "Model.h"
#include "Component.h"

#ifdef _DETECT_MEMORY_LEAKS

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif // _DETECT_MEMORY_LEAKS

IMPLEMENT_CLASS(CComponent);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComponent::CComponent()
{
	m_pOwner = NULL;
	m_bVisible = true;
}

CComponent::~CComponent()
{

}

const D3DXMATRIX *CComponent::GetWorldTransform(D3DXMATRIX *pMat)
{
	if(pMat)
	{
		*pMat = m_matDrawComponent;
	}
	return &m_matDrawComponent;
}

bool CComponent::Update(float fDeltaTime)
{
	if(m_pOwner)
	{
		D3DXMatrixMultiply(&m_matDrawComponent,&m_matTransform,&m_pOwner->m_matDrawComponent);
		return (m_pModel ? m_pModel->Draw(&m_matDrawComponent) : true);
	}
	else
	{
		m_matDrawComponent = m_matTransform;

		if(m_pModel)
		return CActor::Update(fDeltaTime);
	}
	return true;
}
