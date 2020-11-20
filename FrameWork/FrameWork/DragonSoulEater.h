#pragma once
#include "cObject.h"
#include "SkinnedMesh.h"

class SoulEaterState;
class cOBB;
class DragonSoulEater :
	public cObject
{
private:
	cSkinnedMesh*   m_pSkinnedUnit;
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vRot;

	D3DMATERIAL9	m_Mstl;

	cOBB*			m_pOBB;
	SoulEaterState*	m_pCurState;
	DWORD			m_dwDelayState;

	float			m_fCurHeathpoint;
	float			m_fMaxHeathPoint;

	D3DXMATRIXA16	m_matWorld;
public:
	DragonSoulEater();
	~DragonSoulEater();
	void Update() override;
	void Render(D3DXMATRIXA16* pmat) override;
	void Setup(char* szFolder, char* szFileName);
	void SetState();
	cSkinnedMesh& GetSkinnedMesh() { return *m_pSkinnedUnit; }

	void GetWorldMatrix(D3DXMATRIXA16* matWorld);

};
