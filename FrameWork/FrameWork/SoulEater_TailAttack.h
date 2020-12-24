#pragma once
#include "SoulEaterState.h"
class cSoulEater_TailAttack :
	public cSoulEaterState
{
private:
	bool  m_IsAnimBlend;

	// ����ġ�� // ���� 1
	float				m_TailAttack_Physic_Rate;
	float				m_TailAttack_Elemental_Rate;
public:
	cSoulEater_TailAttack();
	cSoulEater_TailAttack(cDragonSoulEater*);
	~cSoulEater_TailAttack();
	void handle() override;
};

