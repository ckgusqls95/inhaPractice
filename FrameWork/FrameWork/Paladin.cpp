#include "stdafx.h"
#include "cOBB.h"
#include "SkinnedMesh.h"
#include "PaladinState.h"
#include "TimerManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"

#include "Paladin.h"

#include "AllocateHierarchy.h"

cPaladin::cPaladin()
	:m_fvelocity(0.0f)
	,m_isMoving(false)
	,m_pSkinnedUnit(NULL)
	,m_pCurState(NULL)
	,m_pWeaponOBB(NULL)
	, m_Hp(0)
	, m_Stamina(0)
	, m_MaxHp(1000)
	, m_MaxStamina(500)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cPaladin::~cPaladin()
{
	SafeDelete(m_pOBB);
	SafeDelete(m_pWeaponOBB);
	SafeDelete(m_pCurState);
	SafeDelete(m_pSkinnedUnit);
}

void cPaladin::Setup(char* szFolder, char* szFile)
{
	m_MaxHp = 1000;
	m_MaxStamina = 500;

	m_Hp = 1000;
	m_Stamina = 500;
	m_pSkinnedUnit = new cSkinnedMesh;
	m_pSkinnedUnit->Setup(szFolder, szFile);
	m_pSkinnedUnit->SetAnimationIndex(9);

	m_vPos = D3DXVECTOR3(20, 0, 30);
	m_vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	m_pSkinnedUnit->Update();
	EventManager->Attach(this);

	D3DXMATRIXA16 mat;
	D3DXMatrixScaling(&mat, m_vScale.x, m_vScale.y, m_vScale.z);
	
	m_pWeaponOBB = new cOBB;
	m_pWeaponOBB->Setup(D3DXFrameFind(m_pSkinnedUnit->GetFrame(), "Sword_joint"), 
		D3DXFrameFind(m_pSkinnedUnit->GetFrame(), "Paladin_J_Nordstrom_Sword")->pMeshContainer, &mat);

	if (m_pSkinnedUnit->m_pTransformationMatrix)
		mat *= *m_pSkinnedUnit->m_pTransformationMatrix;
	
	m_pOBB = new cOBB;
	m_pOBB->Setup(m_pSkinnedUnit, &mat);

	ZeroMemory(&m_Mstl, sizeof(D3DMATERIAL9));
	m_Mstl.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_Mstl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_Mstl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	ShaderSetup();
}

void cPaladin::ShaderSetup()
{
	LPD3DXEFFECT pShader = g_pShaderManager->GetShader(eShader::Specular_DSL);

	D3DLIGHT9   Light;
	g_pD3DDevice->GetLight(0, &Light);
	D3DXVECTOR4 vLightPos = D3DXVECTOR4(Light.Direction.x, Light.Direction.y, Light.Direction.z, 1);
	//D3DXVECTOR4 vLightPos = D3DXVECTOR4(Light.Position.x, Light.Position.y, Light.Position.z, 1);
	//D3DXVECTOR4 vLightPos = D3DXVECTOR4(500.00, 500.00, -500.00, 1.00);
	D3DXCOLOR c = Light.Diffuse;
	D3DXVECTOR4 LightColor = D3DXVECTOR4(c.r, c.g, c.b, c.a);

	pShader->SetVector("gWorldLightPos", &vLightPos);
	pShader->SetVector("gLightColor", &LightColor);
}

void cPaladin::Update()
{
	D3DXMatrixScaling(&m_matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationY(&m_matRot, m_vRot.y);
	D3DXMatrixTranslation(&m_matTranse, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = m_matScale * m_matRot * m_matTranse;
	m_pSkinnedUnit->m_matWorldTM = m_matWorld;

	if (m_fvelocity != 0 && !m_isMoving)
	{
		m_isMoving = true;
	}
	else if (m_fvelocity == 0 && m_isMoving)
	{
		m_isMoving = false;
	}

	m_pSkinnedUnit->Update();

	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);
	D3DXMATRIXA16 matRx = MatrixIdentity;
	D3DXMatrixRotationX(&matRx, D3DXToRadian(90));
	D3DXMATRIXA16 matTr;
	D3DXMatrixTranslation(&matTr, 0.0f, 0.0f, 0.0f);
	
	ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(m_pSkinnedUnit->GetFrame(), "Sword_joint");
	m_matWeapon = matScale * matRx * matTr * pBone->CombinedTransformationMatrix;
	//m_matWeapon = pBone->CombinedTransformationMatrix;
	m_pWeaponOBB->Update(&m_matWeapon);
}

void cPaladin::Update(EventType event)
{
	D3DXMATRIXA16 TempRot;
	D3DXMatrixIdentity(&TempRot);

	float delta = g_pTimeManager->GetElapsedTime();
	//float delta = 0.001f;
	static bool isKeyDown = false;

	if (event == EventType::EVENT_ARROW_UP)
	{
		m_fvelocity = 300.0f * delta;
		isKeyDown = true;
	}
	if (event == EventType::EVENT_ARROW_LEFT)
	{
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, D3DXMatrixRotationY(&TempRot, -125.0f * delta));
		m_vRot.y -= 125.0f * delta;
		isKeyDown = true;
	}
	if (event == EventType::EVENT_ARROW_DOWN)
	{
		m_fvelocity = -118.5f * delta;
		isKeyDown = true;
	}
	if (event == EventType::EVENT_ARROW_RIGHT)
	{
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, D3DXMatrixRotationY(&TempRot, 125.0f * delta));
		m_vRot.y += 125.0f * delta;
		isKeyDown = true;
	}

	/// new Code 11-28 차현빈 3:#8
	if (event == EventType::EVENT_KEYUP && isKeyDown)
	{
		m_fvelocity = 0;
		isKeyDown = false;
	}

	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vPos += m_vDir * m_fvelocity;

	static int n = 0;

	if (event == EventType::EVENT_LBUTTONDOWN)
	{
		m_pSkinnedUnit->SetAnimationIndexBlend(n++);
	}
}

void cPaladin::Render(D3DXMATRIXA16* pmat)
{
	ShaderRender();
	m_pOBB->OBBBOX_Render(D3DCOLOR_XRGB(255, 255, 255));

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &MatrixIdentity);
	m_pWeaponOBB->OBBBOX_Render(D3DCOLOR_XRGB(255, 255, 255));
}

void cPaladin::ShaderRender()
{
	LPD3DXEFFECT pShader = g_pShaderManager->GetShader(eShader::Specular_DSL);

	if (pShader)
	{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		
		D3DXMATRIXA16	matView;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

		D3DXMATRIXA16	matProjection;
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

		D3DXMATRIXA16	matWVP;
		matWVP = matView * matProjection * m_matScale;

		pShader->SetMatrix("gViewMatrix", &matView);
		pShader->SetMatrix("gProjectionMatrix", &matProjection);
		
		// 쉐이더 전역변수들을 설정
		pShader->SetMatrix("gWorldMatrix", &m_matScale);
		pShader->SetMatrix("gWorldViewProjectionMatrix", &matWVP);
		
		pShader->SetTexture("DiffuseMap_Tex", g_pTextureManager->GetTexture("data/XFile/Paladin/Paladin_diffuse.png"));
		pShader->SetTexture("SpecularMap_Tex", g_pTextureManager->GetTexture("data/XFile/Paladin/Paladin_specular.png"));

		//g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);
		
		UINT numPasses = 0;
		pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				pShader->BeginPass(i);
				{
					m_pSkinnedUnit->Render();
				}
				pShader->EndPass();
			}
		}
		pShader->End();
	}
}

void cPaladin::SetTranseform(D3DXMATRIXA16* pmat)
{
}

int cPaladin::GetHp()
{
	return m_Hp;
}

int cPaladin::GetStamina()
{
	return m_Stamina;
}

int cPaladin::GetMaxHp()
{
	return m_MaxHp;
}

int cPaladin::GetMaxStamina()
{
	return m_MaxStamina;
}
