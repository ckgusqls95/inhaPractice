#include "stdafx.h"
#include "TitleScene.h"
#include "PopUp.h"
#include "Button.h"
#include "ObjectPool.h"
#include "TitleSceneBtnEvent.h"

cTitleScene::cTitleScene(SceneType T)
	:cScene(T)
{
}


cTitleScene::~cTitleScene()
{
	
}

void cTitleScene::Setup()
{

	//Ŭ���̾�Ʈ ���� �޾ƿͼ�
	//����ȭ
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	cout << "Left : " << rc.left << endl; // 0
	cout << "Right : " << rc.right << endl; // 1584
	cout << "Bottom : " << rc.bottom << endl; //860
	cout << "Top : " << rc.top << endl; // 0

	float nRight = 0.33;
	float nBottom = 0.37;

	cPopUp *pBackgroundPopup = new cPopUp;
	pBackgroundPopup->Setup("data/UI/TitleScene", "NW_Background.png",
		D3DXVECTOR3(0, 0, 0), 2, true, true);

	cPopUp *pTitleImagePopup = new cPopUp;
	pTitleImagePopup->Setup("data/UI/TitleScene", "NW_Titleletter.png",
		D3DXVECTOR3(370, 0, 0), 2, true, true);
	pBackgroundPopup->cButtonPushBack(pTitleImagePopup);

	cPopUp *pBackGoundBtnPopup = new cPopUp;
	pBackGoundBtnPopup->Setup("data/UI/TitleScene", "NW_Start_UI_Back.png",
		D3DXVECTOR3(rc.right * nRight, rc.bottom * nBottom, 0), 2, true, true);
	pBackgroundPopup->cButtonPushBack(pBackGoundBtnPopup);

	//���ӽ���
	cButton *pButton = new cButton;
	pButton->Setup("data/UI/TitleScene/START", "NW_StartButton_Idle.png", 
		D3DXVECTOR3(rc.right * nRight, rc.bottom * nBottom, 0), 180, 60, 0, 2, true, true);
	pBackGoundBtnPopup->cButtonPushBack(pButton);
	pButton->EventProcess = StartGameBtnEvent;

	//�̾��ϱ�
	pButton = new cButton;
	pButton->Setup("data/UI/TitleScene/CONTINUE", "NW_ContinueButton_Idle.png", 
		D3DXVECTOR3(rc.right * nRight, rc.bottom * nBottom, 0), 180, 160, 0, 2, true, true);
	pBackGoundBtnPopup->cButtonPushBack(pButton);
	pButton->EventProcess = ContinueGameBtnEvent;

	//����
	pButton = new cButton;
	pButton->Setup("data/UI/TitleScene/SETTING", "NW_SettingButton_Idle.png", 
		D3DXVECTOR3(rc.right * nRight, rc.bottom * nBottom, 0), 180, 260, 0, 2, true, true);
	pBackGoundBtnPopup->cButtonPushBack(pButton);
	pButton->EventProcess = SetupGameBtnEvent;

	//��������
	pButton = new cButton;
	pButton->Setup("data/UI/TitleScene/END", "NW_EndButton_Idle.png", 
		D3DXVECTOR3(rc.right * nRight, rc.bottom * nBottom, 0), 180, 360, 0, 2, true, true);
	pBackGoundBtnPopup->cButtonPushBack(pButton);
	pButton->EventProcess = EndGameBtnEvent;

	EventManager->Attach(pBackgroundPopup);
	ObjectManager->AddUIChild(pBackgroundPopup);
}
