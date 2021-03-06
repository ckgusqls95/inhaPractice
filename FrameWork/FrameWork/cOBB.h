#pragma once
class cSkinnedMesh;

class cOBB
{
public:
	cOBB();
	~cOBB();
private:
	D3DXVECTOR3		m_vOriCenterPos;
	D3DXVECTOR3		m_vOriAXidsDir[3];

	D3DXVECTOR3		m_vCenterPos;
	D3DXVECTOR3		m_vAxisDir[3];
	float			m_fAxisLen[3];
	float			m_fAxisHalfLen[3];
	D3DXMATRIXA16	m_matWorldTM;
	std::vector<D3DXVECTOR3> list;
	std::vector<ST_PC_VERTEX> m_vecVertex;
public:
	void Setup(cSkinnedMesh* pSkinnedMesh,D3DXMATRIXA16* pmat = NULL);
	void Setup(D3DXVECTOR3 vmin, D3DXVECTOR3 vmax, D3DXMATRIXA16* pmat = NULL);
	void Setup(D3DXFRAME* pFrame, D3DXMESHCONTAINER* pMesh, D3DXMATRIXA16* pmat);
	void BuildVertices();
	void Update(D3DXMATRIXA16* pmatWorld);
	static bool IsCollision(cOBB* pOBB1,cOBB* pOBB);

	void OBBBOX_Render(D3DXCOLOR c);
	D3DXMATRIXA16 GetWorldMatrix() { return m_matWorldTM; }
	std::vector<D3DXVECTOR3> GetList() { return list; }
};

