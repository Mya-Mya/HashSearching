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
static LIST *HSHeader[HS_MAX];//ハッシュ情報およびヘッダを含むポインタ配列
static LIST *Add_Now=NULL;//追加作業用。今。
static LIST *Add_Before=NULL;//追加作業用、１個前のリストに今のポインタを渡す。

static LIST *Search_p;//検索作業用。

int add;//追加する数値を入れる
int found;//見つかった数を入れる
int searchint = 0;//検索項目を入れる

typedef enum {
	scene_menu,
	scene_search,
}scene_t;

static scene_t nowscene = scene_menu;

//メニュー
int menu_main() {
	DrawFormatString(0, 0, GetColor(255, 255, 255), "メニュー\nLで検索");
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%dのデータは%d個見つかりました！", searchint, found);

	if (Keyboard_Get(KEY_INPUT_L) == 1) {
		nowscene = scene_search;
	}
	return 0;
}

//検索
void search_main() {
	DrawFormatString(0, 0, GetColor(255, 255, 255), "検索内容を入力してください");
	searchint = (KeyInputNumber(30, 50, DATA_MAX, 0, FALSE));

	Search_p = HSHeader[searchint%HS_MAX];//検索内容からハッシュを算出しヘッダポインタを確定

	found = 0;

	while (Search_p != NULL) {
		if (Search_p->data == searchint) {//検索内容と->dataが一致したら
			found++;
		}
		Search_p = Search_p->next;//チェックイン
	}
	nowscene = scene_menu;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	//HSHeaderの初期化
	for (int i = 0; i < HS_MAX; i++) {
		HSHeader[i] = NULL;
	}

	//乱数の初期化
	srand((unsigned)time(NULL));

	DrawFormatString(0, 0, GetColor(255, 100, 255), "何かキーを押してリスト生成");
	WaitKey();

	//リストの生成
	for (int i = 0; i < LIST_NUMBERES; i++) {
		add = (rand() % DATA_MAX);//追加項目の作成

		Add_Now = (LIST*)malloc(sizeof(LIST));//メモリ確保
		Add_Now->data = add;//項目の代入
		Add_Now->next = NULL;
		
		Add_Before = HSHeader[add%HS_MAX];//追加内容からハッシュを算出しヘッダポインタを確定
		if (Add_Before == NULL) {//ハッシュヘッダがNULLだったら
			HSHeader[add%HS_MAX] = Add_Now;
		}
		else {
			while (Add_Before->next != NULL) {//Add_Beforeをそのハッシュから続くリストの末尾までチェックイン
				Add_Before = Add_Before->next;
			}
			Add_Before->next = Add_Now;//前のリストに今のポインタをつなぐ
			
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