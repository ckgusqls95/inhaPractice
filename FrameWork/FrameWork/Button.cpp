#include "stdafx.h"
#include "Button.h"
#include "TextureManager.h"

cButton::cButton()
{
	m_State = enum_Off;
	m_PreState = enum_Off;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Percentage = 0;
	m_Power = true;
	m_Fixed = true;
	EventProcess = NULL;
	m_pPaladin = NULL;
	m_pTrophies = NULL;
	
}

cButton::~cButton()
{
	SafeRelease(m_pSprite);
	SafeRelease(m_pTextureUI);

	EventManager->Detach(*this); // 씬 이동시 자동으로 이벤트 해제
}

void cButton::Setup(char* root, char* fileName, D3DXVECTOR3 position, float x, float y, float z, float percent, bool powerOnOff, bool fixed)
{
	m_Percentage = percent;
	m_X = x;
	m_Y = y;
	m_Z = z;

	m_Position.x = position.x + m_X;
	m_Position.y = position.y + m_Y;
	m_Position.z = position.z + m_Z;

	m_Power = powerOnOff;
	m_Fixed = fixed;

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	string fileRoot(root);
	fileRoot = fileRoot + string("/") + string(fileName);
	str_filename = string(fileName);

	LoadTexture((char*)fileRoot.c_str());
}

void cButton::Update(EventType message)
{	
	if (m_Power)
	{
		if(EventProcess)
		EventProcess(message, this);


		for (int i = 0; i < m_vecPopupBtnList.size(); i++)
		{
			if (this->m_Power)
				m_vecPopupBtnList[i]->Update(message);
		}

	}
}

// 현재 동작안하는 메서드니까 확인바람
void cButton::Render()
{
	// 부모객체가 없거나 부모객체의 파워가 true인 경우에 렌더하게 변경
	bool chk_parents = 0;
	if (pParent)
		chk_parents = pParent->GetState();

	if (chk_parents || !pParent)
		if (m_Power)
		{
			m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
			SetRect(&m_Rect, 0, 0, m_ImageInfoWidth, m_ImageInfoHeight);
			D3DXMATRIXA16 matT, matS, matWorld;
			D3DXMatrixIdentity(&matT);
			D3DXMatrixIdentity(&matS);
			D3DXMatrixIdentity(&matWorld);

			D3DXMatrixScaling(&matS, m_Percentage, m_Percentage, m_Percentage);
			D3DXMatrixTranslation(&matT, m_Position.x, m_Position.y, 0);


			matWorld = matS * matT;
			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(m_pTextureUI, &m_Rect, &D3DXVECTOR3(0, 0, 0),
				&D3DXVECTOR3(0, 0, 0),
				D3DCOLOR_ARGB(255, 255, 255, 255));

			m_pSprite->End();
		}
}

/*
void cButton::Render(D3DXVECTOR3 position)
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	SetRect(&m_Rect, 0, 0, m_ImageInfo.Width, m_ImageInfo.Height);
	D3DXMATRIXA16 matT, matS, matWorld;
	D3DXMatrixIdentity(&matT);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixIdentity(&matWorld);

	//D3DXMatrixScaling(&matS, m_Percentage, m_Percentage, m_Percentage);

	//matWorld = matS * matT;
	//m_pSprite->SetTransform(&matWorld);
	m_pSprite->Draw(m_pTextureUI, &m_Rect, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(position.x, position.y, position.z), D3DCOLOR_ARGB(255, 255, 255, 255));

	matWorld = matS * matT;
	m_pSprite->End();
}
*/

int cButton::GetState()
{
	return m_State;
}

void cButton::SetStateChange(int state)
{
	m_State = state;
}

D3DXVECTOR3 cButton::GetPosition()
{
	return m_Position;
}

float cButton::GetImageInfoWidth()
{
	return (float)m_ImageInfo.Width;
}

float cButton::GetImageInfoHeight()
{
	return (float)m_ImageInfo.Height;
}

void cButton::LoadTexture(char * szFullPath)
{
	D3DXCreateTextureFromFileExA(g_pD3DDevice,
		szFullPath,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&m_ImageInfo,
		NULL,
		&m_pTextureUI
	);

	m_ImageInfoWidth = m_ImageInfo.Width;
	m_ImageInfoHeight = m_ImageInfo.Height;
	SetRect(&m_Rect, 0, 0, m_ImageInfoWidth, m_ImageInfoHeight);
	//SetRect(&m_Rect, 0, 0, m_ImageInfo.Width, m_ImageInfo.Height);

	g_pTextureManager->AddTexture(szFullPath, m_pTextureUI);
	g_pTextureManager->AddImageInfo(szFullPath, m_ImageInfo);
}

void cButton::ChangeSprite(char * szFullPath)
{
	if (!g_pTextureManager->GetTexture(szFullPath)) 
	{
		LoadTexture(szFullPath);
	}

	m_pTextureUI = g_pTextureManager->GetTexture(szFullPath);
	m_ImageInfo = g_pTextureManager->GetImageInfo(szFullPath);
}

int cButton::GetPreState()
{
	return m_PreState;
}

void cButton::SetPreState(int state)
{
	m_PreState = state;
}

float cButton::GetPercent()
{
	return m_Percentage;
}

//void cButton::PowerOnOff()
//{
//	m_Power = !m_Power;
//
//	for (int i = 0; i < m_vecPopupBtnList.size(); i++)
//	{
//		m_vecPopupBtnList[i]->PowerOnOff();
//	}
//}
//
//void cButton::PowerOnOff(bool state)
//{
//	m_Power = state;
//	
//}

void cButton::Destroy()
{
	EventManager->Detach(*this);
}



