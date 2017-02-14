#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init(void)
{
	//�ε�Ŭ���� ���� �� �ʱ�ȭ
	_loading = new loading;
	_loading->init();

	//�̹��� �� ���� �ε�
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release(void)
{
	//�ε�Ŭ���� ����
	SAFE_DELETE(_loading);
}

void loadingScene::update(void)
{
	//�ε�Ŭ���� ������Ʈ
	_loading->update();

	//�ε��� �����ٸ�
	if (_loading->loadingDone())
	{
		//ESCŰ�� ������
		if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE)) 
		{
			//���μ��� ����
			PostQuitMessage(0); 
		}

		else if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			//ESC Ű�� ������ Ű�� ������ �� ��ȯ
			SCENEMANAGER->changeScene("�޴�ȭ��");
		}
	}
}

void loadingScene::render(void)
{
	//�ε�Ŭ���� ����
	_loading->render();
	//�ε��� ��� �Ϸ�Ǹ� 
	if (_loading->loadingDone()) { IMAGEMANAGER->render("guiLoadingScene", getMemDC(), 217.5f, 390); }
}

//�ε��̹��� �Լ�(�̰��� �̹����� ���� �־��)
void loadingScene::loadingImage(void)
{
	//_loading->loadImage();
	//_loading->loadFrameImage();

	//�ε��� �ʹ� ������ ����Ǽ� õõ�� ���ư����� �׽�Ʈ������ ����� ����
	for (int i = 0; i < 100; i++)
	{
		_loading->loadImage("�׽�Ʈ", WINSIZEX, WINSIZEY);
	}

	//�ε�ȭ�� �̹���
	_loading->loadImage("guiLoadingScene", "guiLoadingScene.bmp", 365, 30, true, RGB(255, 0, 255));

	//�޴�ȭ�� �̹���
	_loading->loadImage("mainmenu", "mainmenu.bmp", WINSIZEX, WINSIZEY);
	_loading->loadImage("menu_playbutton", "menu_playbutton.bmp", -85, 240, 301, 34, true, RGB(255, 0, 255));
	_loading->loadImage("menu_mapbutton", "menu_mapbutton.bmp", -85, 300, 301, 34, true, RGB(255, 0, 255));
	_loading->loadImage("menu_quitbutton", "menu_quitbutton.bmp", -85, 360, 301, 34, true, RGB(255, 0, 255));
	
	//��׶��� �̹���
	_loading->loadImage("��׶���", "background.bmp", WINSIZEX, WINSIZEY);
	
	//����ȭ��
	_loading->loadFrameImage("tileMapBase", "tileMapBase.bmp", 192, 160, 8, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("tileMapBaseBig", "tileMapBaseBig.bmp", 336, 320, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("stairs", "stairs.bmp", 48, 24, 2, 1);

	_loading->loadImage("dragon_green", "dragon_green.bmp", 27, 26, true, RGB(255, 0, 255));
	_loading->loadImage("dragon_green_big", "dragon_green_big.bmp", 54, 52, true, RGB(1, 0, 0));
	_loading->loadImage("maptoolCase", "maptoolCase.bmp", 380, 450, true, RGB(255, 0, 255));
	_loading->loadImage("save", "save.bmp", 71, 44, true, RGB(255, 0, 255));
	_loading->loadImage("load", "load.bmp", 71, 44, true, RGB(255, 0, 255));
	_loading->loadImage("select", "select.bmp", 128, 49, true, RGB(255, 0, 255));
	_loading->loadImage("maptoolframe", "maptoolframe.bmp", 202, 170, true, RGB(255, 0, 255));
	_loading->loadFrameImage("slot1", "slot1.bmp", 76, 88, 1, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("slot2", "slot2.bmp", 76, 88, 1, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("slot3", "slot3.bmp", 76, 88, 1, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("eraser", "eraser.bmp", 86, 88, 1, 2, true, RGB(255, 0, 255));

	//���� 
	_loading->loadFrameImage("Slime", "Slime.bmp", 192, 96, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("Blueslime", "Blueslime.bmp", 384, 96, 8, 2, true, RGB(255, 0, 255));
	//���� ����Ʈ
	_loading->loadFrameImage("enemy_swipe", "enemy_swipe.bmp", 135, 24, 5, 1, true, RGB(255, 0, 255));

	//����ȭ��
	_loading->loadFrameImage("player", "player.bmp", 384, 480, 8, 10, true, RGB(255, 0, 255));
	_loading->loadFrameImage("heartbeat", "heartbeat.bmp", 164, 104, 2, 1, true, RGB(255, 0, 255));
	_loading->loadImage("heart", "heart.bmp", 48, 44, true, RGB(255, 0, 255));
	_loading->loadImage("heart_half", "heart_half.bmp", 48, 44, true, RGB(255, 0, 255));
	_loading->loadImage("heart_empty", "heart_empty.bmp", 48, 44, true, RGB(255, 0, 255));
	_loading->loadImage("TEMP_beat_marker_green", "TEMP_beat_marker_green.bmp", 12, 64, true, RGB(255, 0, 255));
	_loading->loadImage("hud_weapon", "hud_weapon.bmp", 60, 66, true, RGB(255, 0, 255));
	_loading->loadImage("hud_armor", "hud_armor.bmp", 60, 66, true, RGB(255, 0, 255));
	_loading->loadImage("weapon_dagger", "weapon_dagger.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("weapon_longsword", "weapon_longsword.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("weapon_broadsword", "weapon_broadsword.bmp", 48, 48, true, RGB(255, 0, 255));

	_loading->loadImage("armor_tunic", "armor_tunic.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("armor_leather", "armor_leather.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("armor_chainmail", "armor_chainmail.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("armor_plate", "armor_plate.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("armor_heavyplate", "armor_heavyplate.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("armor_karate", "armor_karate.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("armor_obsidian01", "armor_obsidian01.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("armor_obsidian02", "armor_obsidian02.bmp", 52, 52, true, RGB(255, 0, 255));
	_loading->loadImage("armor_obsidian03", "armor_obsidian03.bmp", 52, 52, true, RGB(255, 0, 255));
	_loading->loadImage("armor_glass", "armor_glass.bmp", 48, 48, true, RGB(255, 0, 255));

	_loading->loadFrameImage("swipe_dagger_left", "swipe_dagger_left.bmp", 144, 48, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_dagger_right", "swipe_dagger_right.bmp", 144, 48, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_dagger_up", "swipe_dagger_up.bmp", 144, 48, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_dagger_down", "swipe_dagger_down.bmp", 144, 48, 3, 1, true, RGB(255, 0, 255));

	_loading->loadFrameImage("swipe_longsword_left", "swipe_longsword_left.bmp", 384, 48, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_longsword_right", "swipe_longsword_right.bmp", 384, 48, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_longsword_up", "swipe_longsword_up.bmp", 192, 96, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_longsword_down", "swipe_longsword_down.bmp", 192, 96, 3, 1, true, RGB(255, 0, 255));

	_loading->loadFrameImage("swipe_broadsword_left", "swipe_broadsword_left.bmp", 144, 144, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_broadsword_right", "swipe_broadsword_right.bmp", 144, 144, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_broadsword_up", "swipe_broadsword_up.bmp", 432, 48, 3, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("swipe_broadsword_down", "swipe_broadsword_down.bmp", 432, 48, 3, 1, true, RGB(255, 0, 255));
}
//�ε����� �Լ�(�̰��� ���带 ���� �־��)
void loadingScene::loadingSound(void)
{
	//�޴� ���� �ε�
	_loading->loadSound("main_menu", "main_menu.ogg", TRUE, TRUE);

	//���Ӿ� ���� �ε�
	_loading->loadSound("zone1_1", "zone1_1.ogg", TRUE, FALSE);
	_loading->loadSound("vo_ari_death_01", "vo_ari_death_01.ogg", FALSE, FALSE);
	_loading->loadSound("vo_ari_hurt_01", "vo_ari_hurt_01.ogg", FALSE, FALSE);
	_loading->loadSound("vo_ari_melee_1_01", "vo_ari_melee_1_01.ogg", FALSE, FALSE);
}