#ifndef BASE_H
#define BASE_H

/*基底クラスなどのヘッダ*/

/*INCLUDE*/
#include <stdio.h>
#include <wchar.h>


//x,yなどポジションを表す用
typedef char MINI;
//四則演算など多用途
typedef unsigned int UINT;
//モードや人などを識別する用
typedef unsigned char ID;


/*ボードの情報*/
//横幅
#define WIDTH  9
//立幅
#define HEIGHT 9
//配列の要素数
#define NUM_ARRAY WIDTH*HEIGHT


/*ユーザ*/
//NONE:所有者なし(空白)
constexpr ID air_user = 0;
//user1:上部
constexpr ID user1 = 1;
//user2:下部
constexpr ID user2 = 2;


/*position関連*/
//X,Yを格納 0~8
typedef struct {
	MINI x;
	MINI y;
}position;

//position同士を比べる
bool is_equal(position d1, position d2) {
	return d1.x == d2.x && d1.y == d2.y;
}

//成ることのできるボーダライン
constexpr MINI BORDERY_USER1 = 2;
constexpr MINI BORDERY_USER2 = 6;

//positionをarrayへ直す
UINT pos_to_arraynum(position pos) { return (pos.y * WIDTH + pos.x); }
//array番号をpositionへ直す
position arraynum_to_pos(UINT num) { return { (MINI)(num % WIDTH),(MINI)(num / WIDTH) }; }

//相手のボーダーラインへ侵入したか
bool is_intoborderline(ID user,position pos) {
	switch (user) {
	case user1:
		return pos.y >= BORDERY_USER2;
	case user2:
		return pos.y <= BORDERY_USER1;
	default:
		return false;
	}
}


//駒コードを格納
enum struct piece_info:UINT{
	NONE  = 000,//空白
	KING  = 100,//玉将
	HISYA = 101,//飛車
	KAKU  = 102,//角行
	KIN   = 103,//金
	GIN   = 104,//銀
	KEIMA = 105,//桂馬
	KYOUSYA = 106,//香車
	HUHYOU  = 107,//歩兵
	//成駒:200～
	RYUOU    = 201,//竜王
	RYOUMA   = 202,//竜馬
	NARIGIN  = 204,//成銀
	NARIKEI  = 205,//成桂
	NARIKYOU = 206,//成香
	TOKIN    = 207	//と金
};


/*駒の基底クラス*/
class piece_base {
protected:

	//コピーコンストラクタの処理用
	inline void copy_piece_info(const piece_base& pieceg) {
		request_nari = pieceg.request_nari;
		is_nari = pieceg.is_nari;
		piece = pieceg.piece;
		user_id = pieceg.user_id;
	}

public:

	/*変数*/
	//駒コードを格納
	piece_info piece  = piece_info::NONE;
	//ユーザに成るかならないかを質問
	bool request_nari = true;
	//所有者を格納
	ID   user_id      = air_user;
	//成っているかどうか
	bool is_nari      = false;

	//NONE専用(通常駒は使用しない)
	bool is_used      = false;


	//コンストラクタ
	piece_base() {
		//初期値
		piece        = piece_info::NONE;
		request_nari = true;
		user_id = air_user;
		is_nari = false;
		is_used = false;
	}
	
	//場外判定 over:true  notover:false
	bool is_over(position pos) {
		return pos.x < WIDTH || pos.y < HEIGHT ? false:true;
	}

	//動きが正しいか判定(オーバーライドする)
	virtual bool is_true_move(position from, position to) {
		return false;
	}

	//駒を表示(オーバーライドする)
	virtual void show() {}


	//成るかならないか質問(オーバーライドしない)
	virtual void question() final{
		if (request_nari && !is_nari) {
			printf("\n成りますか？\nYes:Yキー No:その他キー\n");
			if (_getwch()=='y') {
				is_nari = true;
				UINT buf = (UINT)piece;
				//100を足すと成り駒になる
				buf += 100;
				piece = (piece_info)buf;
			}else {
				is_nari = false;
			}
			//一度質問したら終了
			request_nari = false;
		}
		return;
	}
};

#endif