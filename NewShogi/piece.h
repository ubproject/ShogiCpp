#ifndef PIECE_H
#define PIECE_H

/*すべての駒を定義*/

/*INCLUDE*/
#include "move.h"
#include "base.h"


/*各駒の定義*/
//空白:所有者なし
class NONE : public piece_base {
public:
	NONE() :piece_base() {
		request_nari = false;
		piece        = piece_info::NONE;
		user_id      = air_user;
		is_used      = true;
	}
	NONE operator =(const NONE& data){
		copy_piece_info(data);
		return *this;
	}
	bool is_true_move(position from, position to) override {
		return false;
	}
	void show() override {
		printf("　");
	}
};


//玉将
class KING : public piece_base {
public:
	KING(ID user):piece_base() {
		//玉将は成れない
		request_nari = false;
		piece        = piece_info::KING;
		user_id      = user;
	}
	KING operator =(const KING& data) {
		copy_piece_info(data);
		return *this;
	}
	bool is_true_move(position from,position to) override {
		if (is_over(to))return false;
		//上下左右,右上,左上,右下,左下
		return (is_UpDownLeftRight(from,to,user_id)||is_UpperLower(from,to,user_id));
	}
	void show() override {
		printf("王");
	}
};


//飛車
class HISYA : public piece_base {
public:
	HISYA(ID user) :piece_base() {
		request_nari = true;
		piece        = piece_info::HISYA;
		user_id      = user;
	}
	HISYA operator =(const HISYA& data) {
		copy_piece_info(data);
		return *this;
	}
	bool is_true_move(position from, position to) override {
		//範囲を超えていないか
		if (is_over(to))return false;
		//成っている場合
		if(is_nari){
			//水平(全),垂直(全),斜めに移動可能(成っている)
			return (is_horizontal(from, to) || is_vertical(from, to) || is_UpperLower(from,to,user_id));
		}else {
			//水平(全),垂直(全) (成っていない)
			return (is_horizontal(from, to) || is_vertical(from, to));
		}
	}
	void show() override {
		if (is_nari) {
			printf("龍");
		}
		else {
			printf("飛");
		}
	}
};


//角
class KAKU : public piece_base {
public:
	KAKU(ID user) :piece_base() {
		request_nari = true;
		piece        = piece_info::KAKU;
		user_id      = user;
	}
	KAKU operator =(const KAKU& data) {
		copy_piece_info(data);
		return *this;
	}
	bool is_true_move(position from, position to) override {
		//範囲を超えていないか
		if (is_over(to))return false;
		//成っている場合
		if (is_nari) {
			//斜め(全)と上下左右に移動可能(成っている)
			return is_diagonal_all(from, to)||is_UpDownLeftRight(from,to,user_id);
		}else {
			//斜め(全)に移動可能(成っていない)
			return is_diagonal_all(from, to);
		}
	}
	void show() override {
		if (is_nari) {
			printf("馬");
		}
		else {
			printf("角");
		}
	}
};


//金
class KIN : public piece_base {
public:
	KIN(ID user) :piece_base() {
		//金は成ることができない
		request_nari = false;
		piece        = piece_info::KIN;
		user_id      = user;
	}
	KIN operator =(const KIN& data) {
		copy_piece_info(data);
		return *this;
	}
	bool is_true_move(position from, position to) override {
		if (is_over(to))return false;
		//上下左右,右上,左上
		return is_kin_move(from,to,user_id);
	}
	void show() override {
		printf("金");
	}
};


//銀
class GIN : public piece_base {
public:
	GIN(ID user) :piece_base() {
		request_nari = true;
		piece        = piece_info::GIN;
		user_id      = user;
	}
	GIN operator =(const GIN& data) {
		copy_piece_info(data);
		return *this;
	}
	bool is_true_move(position from, position to) override {
		if (is_over(to))return false;
		if(is_nari){
			//成銀は金と同じ動き
			return is_kin_move(from,to,user_id);
		}else {
			//右上,左上,右下,左下,上
			return is_UpperLower(from, to, user_id) || is_up(from, to, user_id);
		}
	}
	void show() override {
		if (is_nari) {
			printf("全");
		}
		else {
			printf("銀");
		}
	}
};


//桂馬
class KEIMA : public piece_base {
public:
	KEIMA(ID user) :piece_base() {
		request_nari = true;
		piece        = piece_info::KEIMA;
		user_id      = user;
	}
	KEIMA operator =(const KEIMA& data) {
		copy_piece_info(data);
		return *this;
	}
	bool is_true_move(position from, position to) override {
		if (is_over(to))return false;
		if (is_nari) {
			//成桂は金と同じ動き
			return is_kin_move(from,to,user_id);
		}else{
			//桂馬の動き
			return is_keima_move(from, to, user_id);
		}
	}
	void show() override {
		if (is_nari) {
			printf("圭");
		}
		else {
			printf("桂");
		}
	}
};


//香車
class KYOUSYA : public piece_base {
public:
	KYOUSYA(ID user) :piece_base() {
		request_nari = true;
		piece        = piece_info::KYOUSYA;
		user_id      = user;
	}
	KYOUSYA operator =(const KYOUSYA& data) {
		copy_piece_info(data);
		return *this;
	}
	bool is_true_move(position from, position to) override {
		if (is_over(to))return false;
		if(is_nari){
			//成香は金と同じ動き
			return is_kin_move(from,to,user_id);
		}else {
			//香車の動き(垂直に動く)
			return is_up_all(from, to,user_id);
		}
	}
	void show() override {
		if (is_nari) {
			printf("杏");
		}
		else {
			printf("香");
		}
	}
};


//歩兵
class HUHYOU : public piece_base {
public:
	HUHYOU(ID user) :piece_base() {
		request_nari = true;
		piece        = piece_info::HUHYOU;
		user_id      = user;
	}
	HUHYOU operator =(const HUHYOU& data) {
		copy_piece_info(data);
		return *this;
	}
	HUHYOU() :piece_base() {
		request_nari = true;
		piece = piece_info::HUHYOU;
		user_id = air_user;
	}
	bool is_true_move(position from, position to) override {
		if (is_over(to))return false;
		if (is_nari) {
			//と金は金と同じ動き
			return is_kin_move(from,to,user_id);
		}else {
			//歩兵の動き
			return is_up(from, to, user_id);
		}
	}
	void show() override {
		if (is_nari) {
			printf("と");
		}
		else {
			printf("歩");
		}
	}
};


#endif
