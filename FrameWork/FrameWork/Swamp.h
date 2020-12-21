#pragma once
class cSwamp : public cObject
{
private:
	LPD3DXMESH		m_pMesh;
	LPD3DXEFFECT	m_pShader;
	std::vector<D3DMATERIAL9> m_vecMtl;
	LPDIRECT3DTEXTURE9  m_pTexcoord;
	LPDIRECT3DTEXTURE9	m_pNoise;
public:
	cSwamp();
	~cSwamp();
public:
	void SetEFFECT(LPD3DXEFFECT shader) { m_pShader = shader; }
	void Setup();
	void Update();
	void Render(D3DXMATRIXA16 *pmat = NULL);
	void CollisionProcess(cObject* pObject) override;
};
