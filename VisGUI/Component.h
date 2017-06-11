// Component.h: interface for the CComponent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _COMPONENT_H__INCLUDED_
#define _COMPONENT_H__INCLUDED_

#include "Actor.h"

class CComponent : public CActor  
{
	DECLARE_CLASS(CComponent);
protected:
	CComponent *m_pOwner;
	D3DXMATRIX m_matDrawComponent;
	bool m_bVisible;
public:
	CComponent();
	virtual ~CComponent();

	void SetOwner(CComponent *pOwner){ASSERT(pOwner); m_pOwner = pOwner;}
	CComponent *GetOwner(){ return m_pOwner;}							 
	void ResetOwner(){ m_pOwner = NULL;}
	const D3DXMATRIX *GetWorldTransform(D3DXMATRIX *pMat = NULL);
	bool Update(float fDeltaTime);

	void SetVisible(bool visible){m_bVisible = visible;}
	bool GetVisible(){return m_bVisible;}

};

#endif // _COMPONENT_H__INCLUDED_
