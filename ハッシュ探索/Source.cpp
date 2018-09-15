#include <stdio.h>
#include <stdlib.h>      
#include <string.h> 

#include"DxLib.h"
#include"keyboard.h"
#include<time.h>
#define LIST_NUMBERES 16384
#define HS_MAX 32
#define DATA_MAX 512

struct LIST {
	int data;
	LIST *next;
};
static LIST *HSHeader[HS_MAX];//�n�b�V����񂨂�уw�b�_���܂ރ|�C���^�z��
static LIST *Add_Now=NULL;//�ǉ���Ɨp�B���B
static LIST *Add_Before=NULL;//�ǉ���Ɨp�A�P�O�̃��X�g�ɍ��̃|�C���^��n���B

static LIST *Search_p;//������Ɨp�B

int add;//�ǉ����鐔�l������
int found;//����������������
int searchint = 0;//�������ڂ�����

typedef enum {
	scene_menu,
	scene_search,
}scene_t;

static scene_t nowscene = scene_menu;

//���j���[
int menu_main() {
	DrawFormatString(0, 0, GetColor(255, 255, 255), "���j���[\nL�Ō���");
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%d�̃f�[�^��%d������܂����I", searchint, found);

	if (Keyboard_Get(KEY_INPUT_L) == 1) {
		nowscene = scene_search;
	}
	return 0;
}

//����
void search_main() {
	DrawFormatString(0, 0, GetColor(255, 255, 255), "�������e����͂��Ă�������");
	searchint = (KeyInputNumber(30, 50, DATA_MAX, 0, FALSE));

	Search_p = HSHeader[searchint%HS_MAX];//�������e����n�b�V�����Z�o���w�b�_�|�C���^���m��

	found = 0;

	while (Search_p != NULL) {
		if (Search_p->data == searchint) {//�������e��->data����v������
			found++;
		}
		Search_p = Search_p->next;//�`�F�b�N�C��
	}
	nowscene = scene_menu;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	//HSHeader�̏�����
	for (int i = 0; i < HS_MAX; i++) {
		HSHeader[i] = NULL;
	}

	//�����̏�����
	srand((unsigned)time(NULL));

	DrawFormatString(0, 0, GetColor(255, 100, 255), "�����L�[�������ă��X�g����");
	WaitKey();

	//���X�g�̐���
	for (int i = 0; i < LIST_NUMBERES; i++) {
		add = (rand() % DATA_MAX);//�ǉ����ڂ̍쐬

		Add_Now = (LIST*)malloc(sizeof(LIST));//�������m��
		Add_Now->data = add;//���ڂ̑��
		Add_Now->next = NULL;
		
		Add_Before = HSHeader[add%HS_MAX];//�ǉ����e����n�b�V�����Z�o���w�b�_�|�C���^���m��
		if (Add_Before == NULL) {//�n�b�V���w�b�_��NULL��������
			HSHeader[add%HS_MAX] = Add_Now;
		}
		else {
			while (Add_Before->next != NULL) {//Add_Before�����̃n�b�V�����瑱�����X�g�̖����܂Ń`�F�b�N�C��
				Add_Before = Add_Before->next;
			}
			Add_Before->next = Add_Now;//�O�̃��X�g�ɍ��̃|�C���^���Ȃ�
			
		}
	}

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		Keyboard_Update();
		switch (nowscene) {
		case scene_menu:
			menu_main();
			break;
		case scene_search:
			search_main();
			break;
		}
	}
	DxLib_End();
	return 0;
}