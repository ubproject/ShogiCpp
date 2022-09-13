#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <wchar.h>

typedef char MINI;
typedef unsigned int UINT;
typedef unsigned char ID;

//ボードの情報
#define WIDTH  9
#define HEIGHT 9
#define NUM_ARRAY WIDTH*HEIGHT


//NONE:所有者なし(空白)
constexpr ID air_user = 0;
//user1:上部
constexpr ID user1 = 1;
//user2:下部
constexpr ID user2 = 2;


//X,Yを格納 0~8
typedef struct {
	MINI x;
	MINI y;
}position;


//positionをarrayへ直す
UINT pos_to_arraynum(position pos) { return (pos.y * WIDTH + pos.x); }
//array番号をpositionへ直す
position arraynum_to_pos(UINT num) { return { (MINI)(num % WIDTH),(MINI)(num / WIDTH) }; }


//position同士を比べる
bool is_equal(position d1, position d2) {
	return d1.x == d2.x && d1.y == d2.y;
}

//成ることのできるボーダライン
constexpr MINI BORDERY_USER1 = 2;
constexpr MINI BORDERY_USER2 = 6;

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

class piece_base {
protected:
	//コピー
	inline void copy_piece_info(const piece_base& pieceg) {
		request_nari = pieceg.request_nari;
		is_nari = pieceg.is_nari;
		piece = pieceg.piece;
		user_id = pieceg.user_id;
	}
public:
	//ユーザに成るかならないかを質問
	bool request_nari = true;
	//駒コードを格納
	piece_info piece;
	//所有者を格納
	ID user_id;
	//成っているかどうか
	bool is_nari = false;
	//NONE専用
	bool is_used;

	piece_base() {
		is_used = false;
		request_nari = true;
		is_nari = false;
		user_id = air_user;
		piece = piece_info::NONE;
	}
	piece_base operator =(ID user) {
		user_id = user;
		return *this;
	}
	//場外判定 over:true  not over:false
	bool is_over(position pos) {
		return pos.x < WIDTH || pos.y < HEIGHT ? false:true;
	}
	//動きが正しいか判定(オーバーライド)
	virtual bool is_true_move(position from, position to) {
		return false;
	}
	//駒を表示(オーバーライド)
	virtual void show() {}
	//成るかならないか質問
	virtual void question() {
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