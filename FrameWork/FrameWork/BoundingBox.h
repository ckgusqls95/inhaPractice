#pragma once

class cBoundingBox
{
private:
	D3DXVECTOR3 m_vPivot;
	D3DXVECTOR3 m_vOriCenter;
	D3DXVECTOR3 m_vOriAxisDir[3];

	D3DXVECTOR3 m_vCenterPos;
	D3DXVECTOR3 m_vAxisDir[3];
	float		m_fAxisLen[3];

	float		m_fAxisHalfLen[3];

	D3DXMATRIXA16 m_matWorldTM;
public:
	cBoundingBox();
	~cBoundingBox();

	void Setup(LPD3DXMESH pMesh);
	void Update(D3DXMATRIXA16* pmatWorld);
	void Render(D3DCOLOR c);
	static bool IsCollision(cBoundingBox* pOBB1, cBoundingBox* pOBB2);
};

