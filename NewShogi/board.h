#ifndef BOARD_H
#define BOARD_H

/*基本的な動きを実装*/

/*INCLUDE*/
//user_haveで使用する
#include <vector>

#include "base.h"
#include "piece.h"
#include "SanaeUtil.h"


//set_console_colorなどを使うため
using namespace sanae::util;


//駒の名前と所有者を格納
typedef struct {
	piece_info piece;
	ID         user;
}basic_info;


//エラー出力 0~2 {0:緑 1:黄色 2:赤}
void errout(const char* text,ID serverity) {
	switch (serverity) {
	case 0:
		set_console_color(colors::green);
		printf(text);
		set_console_color();
		break;
	case 1:
		set_console_color(colors::yellow);
		printf(text);
		set_console_color();
		break;
	case 2:
		set_console_color(colors::red);
		printf(text);
		set_console_color();
		break;
	}
}


//使われていないNONEを探して配列番号を返します
UINT find_unused_none(NONE* data) {
	for (UINT i = 0; i < NUM_ARRAY;i++)
		if (!data[i].is_used)return i;
	return 0;
}


/*board*/
class board {
protected:

	//ボード本体 *ポインタを格納
	piece_base* mainboard[NUM_ARRAY];

private:


	/*駒すべて*/
	NONE	air[NUM_ARRAY];
	KING    king   [2] = {user1,user2};
	HISYA   hisya  [2] = {user1,user2};
	KAKU    kaku   [2] = {user1,user2 };
	KIN     kin    [4] = {user1,user1,user2,user2};
	GIN     gin    [4] = {user1,user1,user2,user2};
	KEIMA   keima  [4] = {user1,user1,user2,user2};
	KYOUSYA kyousya[4] = {user1,user1,user2,user2};
	HUHYOU  huhyou[18];

	//ユーザの持ち駒
	std::vector<piece_base*> user1_have;
	std::vector<piece_base*> user2_have;
	

	//強制配置
	void abs_set(position pos, piece_base* data) {
		if (mainboard[pos_to_arraynum(pos)]->piece == piece_info::NONE) {
			mainboard[pos_to_arraynum(pos)]->is_used = false;
		}
		mainboard[pos_to_arraynum(pos)] = data;
	}


	//動きが合っているかを確認する(駒本体のis_true_moveと特殊なアルゴリズムで確認)&玉将をとられたか判定
	bool is_true_move(piece_base* data,position from,position to,ID user) {
		if (mainboard[pos_to_arraynum(from)]->user_id!=user) {
			errout("あなたの駒ではありません。\n",0);
			return false;
		}
		//置く先に何か駒がある場合
		if (mainboard[pos_to_arraynum(to)]->piece != piece_info::NONE) {
			//自分の駒の場所に置くことはできない
			if (mainboard[pos_to_arraynum(to)]->user_id == data->user_id) {
				errout("そこに置くことはできません。\n",0);
				return false;
			}
		}
		//動き方がおかしい場合
		if (!data->is_true_move(from, to)) {
			errout("動き方が違います。\n",0);
			return false;
		}
		//勝敗判定
		if (mainboard[pos_to_arraynum(to)]->piece == piece_info::KING) {
			if (user==user1) {
				user2_lose = true;
			}
			else {
				user1_lose = true;
			}
			return true;
		}
		/*飛車,角,香車の動きがあっているか*/
		//飛車,角,香車のどれかじゃない場合true
		if (data->piece!=piece_info::KYOUSYA&&data->piece!=piece_info::KAKU&&data->piece!=piece_info::HISYA) 
			return true;
		switch (data->piece) {
		case piece_info::HISYA:
			return is_true_move_upall(mainboard,data,from,to) || is_true_move_downall(mainboard, data, from, to) || is_true_move_leftall(mainboard, data, from, to) || is_true_move_rightall(mainboard, data, from, to);
		case piece_info::KAKU:
			return is_diagonal_all_caller(mainboard,data,from,to);
		case piece_info::KYOUSYA:
			return is_true_move_upall(mainboard, data, from, to);
		};
		return false;
	}

public:

	//ユーザの勝敗フラグ(board::is_true_moveによって判定)
	bool user1_lose = false;
	bool user2_lose = false;


	//指定した駒の所有者と駒の名前を返します
	basic_info get_basic_info(position pos) {
		return { mainboard[pos_to_arraynum(pos)]->piece,mainboard[pos_to_arraynum(pos)]->user_id };
	}


	//駒の情報をすべて元に戻す
	inline void clean() {
		command_cls();
		//ボードをまっさらにする
		for (UINT i = 0; i < NUM_ARRAY; i++) {
			mainboard[i]  = &air[i];
		}
		//歩兵の持ち主を初期化user1:user2
		for (UINT i = 0; i < 18; i++) {
			if (i < 9) {
				huhyou[i] = user1;
			}
			else if (i < 18) {
				huhyou[i] = user2;
			}
		}
		//玉将,飛車,角の持ち主を通常へ戻す
		for (UINT i = 0; i < 2; i++) {
			ID user  = i == 0 ? user1 : user2;
			king[i]  = user;
			hisya[i] = user;
			kaku[i]  = user;
		}
		//金,銀,桂馬,香車の持ち主を通常へ戻す
		for (UINT i = 0; i < 4; i++) {
			ID user    = i < 2 ? user1 : user2;
			kin[i]     = user;
			gin[i]     = user;
			keima[i]   = user;
			kyousya[i] = user;
		}
	}


	//コンストラクタ
	board() {
		clean();
		set_basic_position();
	}

	//コンストラクタ:testmodeの引数がtrueの場合cleanのみを行う
	board(bool testmode) {
		if (testmode) {
			clean();
		} else {
			clean();
			set_basic_position();
		}
	}


	//基本のポジションにする
	inline void set_basic_position() {
		//歩兵をセット
		for (MINI i = 0; i < 9; i++)
			abs_set({ i,2 }, &huhyou[i]);
		for (MINI i = 9; i < 18; i++)
			abs_set({ i,5 }, &huhyou[i]);
		//香車
		abs_set({ 0,0 }, &kyousya[0]);
		abs_set({ 8,0 }, &kyousya[1]);
		abs_set({ 0,8 }, &kyousya[2]);
		abs_set({ 8,8 }, &kyousya[3]);
		//桂馬
		abs_set({ 1,0 }, &keima[0]);
		abs_set({ 7,0 }, &keima[1]);
		abs_set({ 1,8 }, &keima[2]);
		abs_set({ 7,8 }, &keima[3]);
		//銀
		abs_set({ 2,0 }, &gin[0]);
		abs_set({ 6,0 }, &gin[1]);
		abs_set({ 2,8 }, &gin[2]);
		abs_set({ 6,8 }, &gin[3]);
		//金
		abs_set({ 3,0 }, &kin[0]);
		abs_set({ 5,0 }, &kin[1]);
		abs_set({ 3,8 }, &kin[2]);
		abs_set({ 5,8 }, &kin[3]);
		//王
		abs_set({ 4,0 }, &king[0]);
		abs_set({ 4,8 }, &king[1]);
		//角
		abs_set({ 1,1 }, &hisya[0]);
		abs_set({ 7,1 }, &kaku[0]);
		abs_set({ 1,7 }, &kaku[1]);
		abs_set({ 7,7 }, &hisya[1]);
	}


	//画面クリアをする(コマンド)
	void command_cls() {
		system("cls");
	}


	//ボードを表示する
	void show(ID user, position lightup = {-1,-1}) {
		printf(" ０|１|２|３|４|５|６|７|８|X軸\n");
		bool is_normal = lightup.x < 0 && lightup.y < 0;
		for (UINT i = 0; i < NUM_ARRAY;i++) {
			if (i == 0)printf("0");
			if (i % 9 == 0 && i!=0)printf("\n%u",i/9);
			//操作するユーザと表示する駒が同じ場合背景を黄色にする。lightupの場所の背景を緑にする。
			if ((*mainboard[i]).user_id == user) {
				if ((*mainboard[i]).is_nari) {
					set_console_color(colors::red, colors::yellow);
				}else{
					set_console_color(colors::white, colors::yellow);
				}
				if (!is_normal) {
					if (pos_to_arraynum(lightup) == i) {
						if ((*mainboard[i]).is_nari) {
							set_console_color(colors::red, colors::green);
						}
						else {
							set_console_color(colors::white, colors::green);
						}
					}
				}
			}
			else {
				//成っている場合表示方法を変える
				if ((*mainboard[i]).is_nari) {
					set_console_color(colors::red);
				}
				else {
					set_console_color();
				}
			}
			(*mainboard[i]).show();
			set_console_color();
			printf("|");
		}
		set_console_color();
	}


	//持ち駒確認&setする
	bool have_pieces_set(ID user) {
		std::vector<piece_base*>* user_have = user == user1 ? &user1_have : &user2_have;
		printf("あなたの持ち駒\n");
		for (UINT i = 0; i < user_have->size(); i++) {
			printf("%u:", i);
			(*user_have)[i]->show();
			printf("\n");
		}
		printf("指す駒を選択してください。\n");
		UINT num = 0;
		scanf_s("%u", &num);
		if (user_have->size() <= num) {
			command_cls();
			errout("指定したナンバーはありません\n", 2);
			return false;
		}
		printf("配置先を入力してください。\n");
		MINI x = 0, y = 0;
		printf("X:");
		x = _getwch();
		x -= '0';
		printf("%d\n", x);
		printf("Y:");
		y = _getwch();
		y -= '0';
		printf("%d\n", y);
		if (mainboard[pos_to_arraynum({ x,y })]->piece != piece_info::NONE) {
			command_cls();
			errout("そこに置くことはできません。", 2);
			return false;
		}
		//指定の駒をセット
		abs_set({ x,y }, (*user_have)[num]);
		//持ち駒から削除
		(*user_have).erase((*user_have).begin() + num);
		command_cls();
		errout("成功しました。", 0);
		return true;
	}


	//ユーザの操作を反映&判定
	bool userset(position from,position to,ID user) {
		//操作しようとしている駒が自分のものか
		if (mainboard[pos_to_arraynum(from)]->user_id != user) {
			errout("自分の駒のみ動かすことができます。\n",1);
			return false;
		}
		//動きが正しいか判定
		if (!is_true_move(mainboard[pos_to_arraynum(from)], from, to, user)) { 
			return false;
		}
		//ボーダーラインに入った場合
		if (is_intoborderline(user,to)) {
			mainboard[pos_to_arraynum(from)]->question();
		}
		//交換
		piece_base* buf = mainboard[pos_to_arraynum(to)];
		mainboard[pos_to_arraynum(to)] = mainboard[pos_to_arraynum(from)];
		if (buf->piece == piece_info::NONE) {
			mainboard[pos_to_arraynum(from)] = buf;
		}
		else {
			mainboard[pos_to_arraynum(from)] = &air[find_unused_none(air)];
			std::vector<piece_base*>* user_have = user == user1 ? &user1_have : &user2_have;
			buf->user_id     = user;
			//入手した駒を初期化
			if (buf->is_nari) {
				UINT bufi    = (UINT)buf->piece;
				bufi        -= 100;
				buf->piece   = (piece_info)bufi;
				buf->is_nari = false;
			}
			if (buf->piece!=piece_info::KIN) {
				buf->request_nari = true;
			}
			user_have->push_back(buf);
		}
		return true;
	}


	/*テストモードで使われる*/
	bool set_test(piece_info name,position setpos,ID user) {
		//成っている場合
		piece_info buf;
		bool is_nari = false;
		if ((UINT)name>200) {
			UINT namebuf = (UINT)name;
			namebuf     -= 100;
			buf          = (piece_info)namebuf;
			is_nari      = true;
		} else {
			buf = name;
		}
		switch (buf) {
		case piece_info::KING:
			mainboard[pos_to_arraynum(setpos)] = user == user1 ? &king[0]    : &king[1];
			break;
		case piece_info::HISYA:
			mainboard[pos_to_arraynum(setpos)] = user == user1 ? &hisya[0]   : &hisya[1];
			break;
		case piece_info::KAKU:
			mainboard[pos_to_arraynum(setpos)] = user == user1 ? &kaku[0]    : &kaku[1];
			break;
		case piece_info::KIN:
			mainboard[pos_to_arraynum(setpos)] = user == user1 ? &kin[0]     : &kin[3];
			break;
		case piece_info::GIN:
			mainboard[pos_to_arraynum(setpos)] = user == user1 ? &gin[0]     : &gin[3];
			break;
		case piece_info::KEIMA:
			mainboard[pos_to_arraynum(setpos)] = user == user1 ? &keima[0]   : &keima[3];
			break;
		case piece_info::KYOUSYA:
			mainboard[pos_to_arraynum(setpos)] = user == user1 ? &kyousya[0] : &kyousya[1];
			break;
		case piece_info::HUHYOU:
			mainboard[pos_to_arraynum(setpos)] = user == user1 ? &huhyou[0]  : &huhyou[17];
			break;
		default:
			return false;
		}
		if (is_nari) {
			mainboard[pos_to_arraynum(setpos)]->is_nari = true;
		}
		return true;
	}
};
#endif