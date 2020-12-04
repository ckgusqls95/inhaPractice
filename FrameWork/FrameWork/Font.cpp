#include "stdafx.h"
#include "Font.h"
#include "FontManager.h"


cFont::cFont()
	:m_Color(D3DCOLOR_ARGB(255, 255, 255, 255))
	,m_Format(DT_LEFT | DT_TOP | DT_NOCLIP)
	,m_pFont(NULL)
{

}


cFont::~cFont()
{
	SafeRelease(m_pFont);
}


void cFont::Setup(string text, eFontType fontType)
{
	m_Text = text;
	m_pFont = g_pFontManager->GetFont(fontType);
}

void cFont::Setup(string text, eFontType fontType, D3DXVECTOR3 position)
{
	// TODO position�� �ش��ϴ� ��ġ�� �����ǰ� ����
	
	m_Text = text;
	m_pFont = g_pFontManager->GetFont(fontType);
}

void cFont::Update()
{

}

// cPopUp�� ��ӹ޾� �����ϱ⶧���� objectManager���� cPopup::Render(D3DXMATRIXA16 * pmat)�� ����� ������ �����
void cFont::Render()
{

	m_pFont->DrawTextA(NULL, m_Text.c_str(), m_Text.length(), &m_Rect, m_Format, m_Color);
}


