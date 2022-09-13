#include "board.h"
#include <time.h>
#include <stdlib.h>

//駒操作 終了の場合true
bool selectdialog(board* main,ID user) {
	printf("\n操作を選択してください。\nAキー:一手指す\nBキー:持ち駒を指す\nCキー:再表示\nDキー:降参する\n");
	switch (_getwch()) {
	case 'a':
		{//変数エラーをなくすため
		printf("動かす駒を選択してください。\n");
		wint_t x = 0, y = 0;
		printf("X:");
		x = _getwch();
		x -= '0';
		printf("%d\n",x);
		printf("Y:");
		y = _getwch();
		y -= '0';
		printf("%d\n",y);
		if (x<0||y<0) {
			errout("負の数の値はありません\n",2);
			selectdialog(main,user);
		}
		if (x > 8 || y > 8) {
			errout("8以上の値はありません\n",2);
			selectdialog(main,user);
		}
		basic_info data = main->get_basic_info({ (MINI)x,(MINI)y });
		if (data.user != user) {
			errout("指定した駒はあなたの駒ではありません。\n", 2);
			selectdialog(main,user);
		}
		wint_t tox = 0, toy = 0;
		main->command_cls();
		main->show(user, { (MINI)x,(MINI)y });
		errout("\n選択しました。移動先を選択してください。\n", 0);
		printf("X:");
		tox = _getwch();
		tox -= '0';
		printf("%d\n", tox);
		printf("Y:");
		toy = _getwch();
		toy -= '0';
		printf("%d\n", toy);
		if (tox < 0 || toy < 0) {
			errout("負の数の値はありません\n", 2);
			selectdialog(main, user);
		}
		if (!main->userset({ (MINI)x,(MINI)y }, { (MINI)tox,(MINI)toy }, user)) {
			errout("失敗しました再入力してください。\n", 2);
			selectdialog(main, user);
		}
		if (main->user1_lose||main->user2_lose) {
			set_console_color(colors::black,colors::white);
			main->user1_lose?printf("USER2が勝利しました。\n") : printf("USER1が勝利しました。\n");
			set_console_color();
			system("pause");
			return true;
		}
		break;
		}//変数エラーをなくすため
	case 'b':
		if (!main->have_pieces_set(user)) 
			selectdialog(main,user);
		break;
	case 'c':
		main->command_cls();
		main->show(user);
		selectdialog(main,user);
		break;
	case 'd':
		user == user1 ? main->user1_lose = true : main->user2_lose=true;
		set_console_color(colors::black, colors::white);
		main->user1_lose ? printf("USER2が勝利しました。\n") : printf("USER1が勝利しました。\n");
		set_console_color();
		system("pause");
		return true;
	default:
		selectdialog(main,user);
		return false;
	}
	return false;
}
//通常対戦
void normalmode() {
	board main;
	while (1) {
		main.show(user1);
		if(selectdialog(&main,user1))return;
		main.command_cls();
		main.show(user2);
		if(selectdialog(&main,user2))return;
		main.command_cls();
	}
}
//お試しモード
void testmode() {
	//未完成
}
//操作説明
void explanation() {
	//未完成
}

void dialog() {
	printf("モードを選択してください。\nAキー:対戦\nBキー:お試しモード\nCキー:操作説明\n\n");
	switch (_getwch()) {
	case 'a':
		normalmode();
		break;
	case 'b':
		testmode();
		break;
	case 'c':
		explanation();
		break;
	default:
		errout("不明なモードです。再入力してください。\n",2);
		break;
	}
}
int main(){
	system("title 将棋プログラム ver:2.0");
	while (1) {
		system("cls");
		set_console_color(colors::black, colors::white);
		printf("将棋プログラム Ver:2.0\n");
		set_console_color();
		printf("ProjectName:Shogi\nDev:SanaeProject\nVer:2.0\n\n");
		dialog();
	}
}
