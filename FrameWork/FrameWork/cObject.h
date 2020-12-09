#pragma once


const enum Tag
{
	Tag_Camera,
	Tag_SkyBox,
	Tag_Map,
	Tag_Player,
	Tag_Boss,
	Tag_LavaGolem,
	Tag_RunStone,
	Tag_cObj
};


enum TAG_UI
{
	TagUI_Title,
	TagUI_InGame,
	TagUI_Trophies,
	TagUI_Hp,
	TagUI_Stamina,

	TagUI_ESC_Menu,
	TagUI_NewGameCheck,
	TagUI_ExitCheck,

	TagUI_InGameObject,
	TagUI_GameOver,
};

enum SceneType;

class cOBB;
class BoneOBB;

struct CollisionInfo
{
	DWORD dwCollsionTime;
	DWORD dwDelayTime;
};

class cObject
{
protected:
	int			m_nTag;
	cOBB*		m_pOBB;
	vector<BoneOBB*> m_vBoneArray;
	std::map<int, CollisionInfo> mapCollisionList; // 
protected:

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vDir;
	D3DXVECTOR3 m_vRot;
	D3DXVECTOR3 m_vScale;

public:
	cObject();
	virtual ~cObject();
	virtual void Update() = 0;
	virtual void Render(D3DXMATRIXA16 * pmat = NULL) = 0;
	void Tagging(int enumTag) { m_nTag = enumTag; }
	int GetTag() { return m_nTag; }
	cOBB* GetOBB();
	D3DXVECTOR3 GetPos() { return m_vPos; }
	void SetPos(D3DXVECTOR3 pos) { m_vPos = pos; }
	D3DXVECTOR3* GetpPos() { return &m_vPos; }
	void SetScale(D3DXVECTOR3 scale) { m_vScale = scale; }
	D3DXVECTOR3 GetScale() { return m_vScale; }
	void BuildBoneData(DWORD *BoneNum,
		D3DXFRAME *Frame,
		D3DXMESHCONTAINER *pMesh);

	D3DXVECTOR3 GetDirection() { return m_vDir; }
	void SetDirection(D3DXVECTOR3 dir) { m_vDir = dir; }
	D3DXVECTOR3 GetRotation() { return m_vRot; }
	void SetRotation(D3DXVECTOR3 Rot) { m_vRot = Rot; }

	bool m_isDelete;
	virtual void CollisionProcess(cObject* pObject, DWORD dwDelayTime) {};
	void AddCollisionInfo(int,CollisionInfo);
	CollisionInfo* GetCollsionInfo(int nTag);
};

void GetBoundingBoxSize(D3DXFRAME *pFrame,
	D3DXMESHCONTAINER *pMesh,
	D3DXVECTOR3 *vecSize,
	D3DXVECTOR3 *vecJointOffset);

struct BoneOBB
{
	string szName; // 이걸 통해서 com
	vector<ST_PC_VERTEX> vPoints;
	D3DXVECTOR3			vJointOffset;
	D3DXVECTOR3			vPosition;
};

struct BoundingBox
{
	string szName;
	cOBB*	Box;
	D3DXVECTOR3 m_vScale;

};

