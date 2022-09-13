/*INCLUDE*/
#include "board.h"
#include <time.h>
#include <stdlib.h>

/*本体
* 基本情報:
* author :SanaeProject
* version:2.0
*/


/*演出*/
void delayprint(const char* text,bool fast=false) {
	for (UINT i = 0; i < strlen(text);i++) {
		printf("%c",*(text+i));
		Sleep(fast?(DWORD)5:(DWORD)20);
	}
}
void delayprint_color(const char* text,colors c,bool fast=false) {
	set_console_color(c,colors::black);
	for (UINT i = 0; i < strlen(text); i++) {
		printf("%c", *(text + i));
		Sleep(fast ? (DWORD)5 : (DWORD)20);
	}
	set_console_color();
}


//駒操作のダイアログ
bool selectdialog(board* main,ID user) {
	delayprint("\n操作を選択してください。\nAキー:一手指す\nBキー:持ち駒を指す\nCキー:再表示\nDキー:降参する\n",true);
	switch (_getwch()) {
	case 'a':
		{//変数エラーをなくすため
		printf("動かす駒を選択してください。\n");

		wint_t x = 0, y = 0;
		
		printf("X:");
		x = _getwch();
		//charの値を整数に戻す
		x -= '0';
		printf("%d\n",x);
		
		printf("Y:");
		y = _getwch();
		//charの値を整数に戻す
		y -= '0';
		printf("%d\n",y);
		
		if (x<0||y<0) {
			errout("負の数の値はありません\n",2);
			selectdialog(main,user);
			return false;
		}
		if (x >= WIDTH || y >= WIDTH) {
			errout("8以上の値はありません\n",2);
			selectdialog(main,user);
			return false;
		}
		basic_info data = main->get_basic_info({ (MINI)x,(MINI)y });
		if (data.user != user) {
			errout("指定した駒はあなたの駒ではありません。\n", 2);
			selectdialog(main,user);
			return false;
		}

		wint_t tox = 0, toy = 0;

		main->command_cls();
		main->show(user, { (MINI)x,(MINI)y });
		delayprint_color("\n選択しました。移動先を選択してください。\n", colors::green);
		
		printf("X:");
		tox = _getwch();
		//charの値を整数に戻す
		tox -= '0';

		printf("%d\n", tox);
		printf("Y:");
		toy = _getwch();
		//charの値を整数に戻す
		toy -= '0';

		printf("%d\n", toy);
		if (tox < 0 || toy < 0) {
			errout("負の数の値はありません\n", 2);
			selectdialog(main, user);
			return false;
		}
		if (tox >= WIDTH || toy >= WIDTH) {
			errout("8以上の値はありません\n", 2);
			selectdialog(main, user);
			return false;
		}

		if (!main->userset({ (MINI)x,(MINI)y }, { (MINI)tox,(MINI)toy }, user)) {
			errout("失敗しました再入力してください。\n", 2);
			selectdialog(main, user);
			return false;
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
	ID user = user2;
	board main=true;
	printf("使用する駒を選択してください。\n");
	printf("駒		コード\n"
			"玉将[王]	100\n"
			"飛車[飛]	101\n"
			"角行[角]	102\n"
			"金将[金]	103\n"
			"銀将[銀]	104\n"
			"桂馬[桂]	105\n"
			"香車[香]	106\n"
			"歩兵[歩]	107\n"
			"竜王(成飛車)[龍]201\n"
			"竜馬(成角行)[馬]202\n"
			"成銀[銀]	204\n"
			"成桂[桂]	205\n"
			"成香[香]	206\n"
			"と金[と]	207\n"
	);
	UINT number = 0;
	scanf_s("%u",&number);
	if (!main.set_test((piece_info)number, { 3,6 }, user2)) {
		errout("設定に失敗しました再入力してください。\n",2);
		testmode();
	}
	errout("\n成功しました。\n",0);
	while (1) {
		main.show(user);
		if (selectdialog(&main, user))return;
		main.command_cls();
	}
}


//詳細
void info() {
	system("cls");
	delayprint_color("プログラムの詳細\n",colors::green);
	delayprint("ProjectName:Shogi\n");
	delayprint("Author  :SanaeProject\n");
	delayprint("Language:C++\n");
	delayprint("Editor  :VisualStudio2022\n");
	delayprint_color("\nVersion:2.0\n",colors::yellow);
	system("pause");
	return;
}


//この関数により通常対戦かお試しモードか詳細へ送る
void dialog() {
	printf("モードを選択してください。\nAキー:対戦\nBキー:お試しモード\nCキー:プログラムの詳細\n\n");
	switch (_getwch()) {
	case 'a':
		normalmode();
		break;
	case 'b':
		testmode();
		break;
	case 'c':
		info();
		break;
	default:
		errout("不明なモードです。再入力してください。\n",2);
		break;
	}
}


//main関数
int main(){
	system("title 将棋プログラム ver:2.0");
	while (1) {
		system("cls");
		set_console_color(colors::black, colors::white);
		delayprint("将棋プログラム Ver:2.0\n");
		set_console_color();
		delayprint("ProjectName:Shogi\nDev:SanaeProject\nVer:2.0\n\n",true);
		dialog();
	}
}
