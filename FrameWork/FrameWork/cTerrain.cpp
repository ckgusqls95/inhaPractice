#include "stdafx.h"
#include "cTerrain.h"
#include "basic.h"
cTerrain::cTerrain()
	:m_pTexture(NULL)
	, m_pTerrainMesh(NULL)
	, m_nTile(0)
	, TerrainThread(NULL)
	, m_IsKeyThread(true)
{
	m_CullingRect = { 0,0,0,0 };
	
}


cTerrain::~cTerrain()
{

	
}

void cTerrain::Render()
{
	if(m_pTerrainMesh)
	{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		g_pD3DDevice->SetTexture(0,m_pTexture);
		g_pD3DDevice->SetMaterial(&m_stMtl);
		m_pTerrainMesh->DrawSubset(0);
	}
}

void cTerrain::NewTerrain(D3DXVECTOR3 vec)
{
	
	float x = ((float)(m_nTile) / 2.0f) + vec.x;
	float z = ((float)(m_nTile) / 2.0f) - vec.z;

	float col = ::floorf(x);
	float row = ::floorf(z);
	
	if (col < 0 || col > m_nTile || row < 0 || row > m_nTile)
	{
		m_IsKeyThread = true;
		return;
	}

	RECT InPlayArea = { 0,0,0,0 };


	InPlayArea.left   = col - 100 < 0 ? 0 : col - 100;
	InPlayArea.top    = row - 100 < 0  ? 0 : row - 100;
	InPlayArea.right  = col + 100 > m_nTile ? m_nTile : col + 100;
	InPlayArea.bottom = row + 100 > m_nTile ? m_nTile : row + 100;

	// 0 ~ 100
	//		100 100
	

	std::vector<DWORD> vecIndex;
	std::vector<ST_PNT_VERTEX> vecVetex;

	for (int y = InPlayArea.top; y <= InPlayArea.bottom; y++)
	{
		for (int x = InPlayArea.left; x <= InPlayArea.right; x++)
		{
			vecVetex.push_back(m_vecMapVertex[x + y * (m_nTile+1)]);
			//g_pLogger->ValueLog(__FUNCTION__, __LINE__, "f",(double)(x + y*m_nTile));
		}
	}


	// Index Buffer
	for(int y = 0;  y < (InPlayArea.bottom - InPlayArea.top); ++y)
	{
		for(int x = 0; x < (InPlayArea.right - InPlayArea.left); ++x)
		{
			int v0 = (x + 0) + (y + 0) * (InPlayArea.right - InPlayArea.left + 1);
			int v1 = (x + 1) + (y + 0) * (InPlayArea.right - InPlayArea.left + 1);
			int v2 = (x + 1) + (y + 1) * (InPlayArea.right - InPlayArea.left + 1);
			int v3 = (x + 0) + (y + 1) * (InPlayArea.right - InPlayArea.left + 1);

			vecIndex.push_back(v1);
			vecIndex.push_back(v3);
			vecIndex.push_back(v0);

			vecIndex.push_back(v1);
			vecIndex.push_back(v2);
			vecIndex.push_back(v3);
		}
		
	}
	// Index Buffer

	LPD3DXMESH substitute;
	
	D3DXCreateMeshFVF(vecIndex.size() / 3,
		vecVetex.size(), 
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&substitute);

	ST_PNT_VERTEX *pV = NULL;
	substitute->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVetex[0], vecVetex.size() * sizeof(ST_PNT_VERTEX));
	substitute->UnlockVertexBuffer();

	DWORD *pI = NULL;
	substitute->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	substitute->UnlockIndexBuffer();

	DWORD* pA = NULL;

	substitute->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	substitute->UnlockAttributeBuffer();
	
	std::vector<DWORD> vecAdj;
	vecAdj.resize(vecIndex.size());
	
	substitute->GenerateAdjacency(0.001f, &vecAdj[0]);
	substitute->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE ,
		&vecAdj[0],
		0, 0, 0);


	m_pTerrainMesh = substitute;
	m_CullingRect = InPlayArea;
	m_IsKeyThread = true;
}


float cTerrain::getHeight(D3DXVECTOR3 vec)
{
	float x = ((float)(m_nTile + 1) / 2.0f) + vec.x;
	float z = ((float)(m_nTile + 1) / 2.0f) - vec.z;

	float col = ::floorf(x);
	float row = ::floorf(z);

	if (col >= (m_nTile + 1) || row >= (m_nTile + 1))
		return 0.0f;

	float A = getHeightMapEntry(row + 0, col + 0);
	float B = getHeightMapEntry(row + 0, col + 1);
	float C = getHeightMapEntry(row + 1, col + 0);
	float D = getHeightMapEntry(row + 1, col + 1);

	float dx = x - col;
	float dz = z - row;

	float height = 0.0f;
	if (dz < 1.0 - dx)
	{
		float uy = B - A;
		float vy = C - A;
		height = A + LerpPosition(0.0f, uy, dx)
			+ LerpPosition(0.0f, vy, dz);
	}
	else
	{
		float uy = C - D;
		float vy = B - D;
		height = D + LerpPosition(0.0f, uy, 1.0f - dx)
			+ LerpPosition(0.0f, vy, 1.0f - dz);
	}
	return height;
	
}

void cTerrain::Setup(std::string strFolder, std::string strTex,
	std::string strRaw,DWORD dwBytesPerPixel)
{
	using namespace std;
	string strTexPath = strFolder + string("/") + strTex;
	string strRawPath = strFolder + string("/") + strRaw;

	
	D3DXCreateTextureFromFileA(g_pD3DDevice, strTexPath.c_str(), &m_pTexture);

	FILE* fpRaw;
	fopen_s(&fpRaw,strRawPath.c_str(), "rb");
	
	fseek(fpRaw, 0, SEEK_END);
	int nFileLength = ftell(fpRaw);
	fseek(fpRaw, 0, SEEK_SET);

	int nNumVertex = nFileLength / dwBytesPerPixel;
	int nRow  = (int)(sqrt(float(nNumVertex) + 0.001f));
	int nCol  = nRow;
	int nTile = nRow - 1;

	m_nTile = nTile;


	ST_PNT_VERTEX v;
	
	for(int i = 0; i <= m_nTile; i++)
	{
		for(int j = 0; j <= m_nTile; j++)
		{
			int k = (m_nTile + 1) * i + j;
			v.t = D3DXVECTOR2((float)j /m_nTile, (float)i / m_nTile);
			v.p.y = (unsigned char)fgetc(fpRaw) / 10.0f - 10.0f;
			v.p.x = -m_nTile * 0.5 +j;
			v.p.z = +m_nTile * 0.5 - i;
			v.n = D3DXVECTOR3(0, 1, 0);
			m_vecMapVertex.push_back(v);
		}
	}

	
	fclose(fpRaw);

	for (int z = 1; z < nTile; ++z)
	{
		for (int x = 1; x < nTile; ++x)
		{
			int k = (m_nTile)* z + x;

			D3DXVECTOR3 v0, v1, vCross;

			v0 = m_vecMapVertex[k - (m_nTile + 1)].p - m_vecMapVertex[k + (m_nTile + 1)].p;
			v1 = m_vecMapVertex[k - 1].p - m_vecMapVertex[k + 1].p;
			
			D3DXVec3Cross(&vCross, &v0, &v1);
			D3DXVec3Normalize(&vCross, &vCross);
			m_vecMapVertex[k].n = vCross;
		}
	}

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
}

void cTerrain::callThread(D3DXVECTOR3 vec)
{
	static D3DXVECTOR3 PrevVec = vec;
	
	if (m_IsKeyThread && PrevVec != vec)
	{
		PrevVec = vec;
		m_IsKeyThread = false;
		TerrainThread = new std::thread([&]() {NewTerrain(vec); });
		
	}

	
	
}

float cTerrain::LerpPosition(float a , float b, float t)
{
	return a - (a *t) + (b * t);
}