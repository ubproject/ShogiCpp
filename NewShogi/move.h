#ifndef MOVE_H
#define MOVE_H

//position用
#include "base.h"

/*動き判定*/
/*
user1は上部から下部へ移動
user2は下部から上部へ移動する
is_upなどはそのことをあらかじめ想定して作成されている。
*/
//移動したか
bool is_hold(position from, position to) {
	return (from.x==to.x&&to.y==from.y);
}
//上
bool is_up(position from, position to,ID user) {
	return (((from.y + 1) == to.y)&&user==user1) || (((from.y - 1) == to.y) && user == user2);
}
bool is_up_all(position from,position to,ID user){
	if (from.x == to.x && (to.y - from.y) > 0 && user == user1) return true;
	if (from.x == to.x && (to.y - from.y) < 0 && user == user2) return true;
	return false;
}
//下
bool is_down(position from, position to,ID user) {
	return (((from.y + 1) == to.y) && user == user2) || (((from.y - 1) == to.y) && user == user1);
}
//左
bool is_left(position from, position to,ID user) {
	return ((from.x - 1) == to.x && user==user2)||((from.x + 1) == to.x && user == user1);
}
//右
bool is_right(position from, position to,ID user) {
	return ((from.x + 1) == to.x && user==user2)|| ((from.x - 1) == to.x && user == user1);
}
//上下左右
bool is_UpDownLeftRight(position from,position to,ID user) {
	MINI count = is_up(from, to, user) + is_down(from, to, user) + is_left(from, to, user) + is_right(from, to, user);
	return count==1?true:false;
}
/*斜め*/
//右上
bool is_upperright(position from, position to,ID user) {
	return (is_up(from, to, user) && is_right(from, to,user));
}
//移動の途中に駒がないか
bool is_upperright_all(piece_base** boardp, piece_base* piec, position from, position to) {
	for (MINI x = from.x + 1, y = from.y - 1; x < to.x; x++, y--) 
		if (boardp[pos_to_arraynum({ x,y })]->piece != piece_info::NONE)
			return false;
	return true;
}
//左上
bool is_upperleft(position from, position to,ID user) {
	return (is_up(from, to,user) && is_left(from, to,user));
}
//移動の途中に駒がないか
bool is_upperleft_all(piece_base** boardp, piece_base* piec, position from, position to) {
	for (MINI x = from.x - 1, y = from.y - 1; x > to.x; x--, y--) 
		if (boardp[pos_to_arraynum({ x,y })]->piece != piece_info::NONE)
			return false;
	return true;
}
//右下
bool is_lowerright(position from, position to,ID user) {
	return (is_down(from, to,user) && is_right(from, to,user));
}
//移動の途中に駒がないか
bool is_lowerright_all(piece_base** boardp, piece_base* piec, position from, position to) {
	for (MINI x = from.x + 1, y = from.y + 1; x < to.x; x++, y++)
		if (boardp[pos_to_arraynum({ x,y })]->piece != piece_info::NONE)
			return false;
	return true;
}
//左下
bool is_lowerleft(position from, position to,ID user) {
	return (is_down(from, to,user) && is_left(from, to,user));
}
//移動の途中に駒がないか
bool is_lowerleft_all(piece_base** boardp, piece_base* piec, position from, position to) {
	for (MINI x = from.x - 1, y = from.y + 1; x > to.x; x--, y++) {
		if (boardp[pos_to_arraynum({ x,y })]->piece != piece_info::NONE)
			return false;
	}
	return true;
}
//移動の途中に駒がないかを調べる(これを使うこと)
bool is_diagonal_all_caller(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;
	position change = {to.x-from.x,to.y-from.y};
	bool rightupper = change.x > 0 && change.y < 0;
	bool leftupper  = change.x < 0 && change.y < 0;
	bool rightlower = change.x > 0 && change.y > 0;
	bool leftlower  = change.x < 0 && change.y > 0;
	if (rightupper) {
		return is_upperright_all(boardp,piec,from,to);
	}
	else if (leftupper) {
		return is_upperleft_all(boardp, piec, from, to);
	}
	else if (rightlower) {
		return is_lowerright_all(boardp, piec, from, to);
	}
	else if (leftlower) {
		return is_lowerleft_all(boardp, piec, from, to);
	}
	return false;
}
//右上左上右下左下
bool is_UpperLower(position from,position to,ID user) {
	MINI count = 0;
	return (count = is_upperright(from, to,user) + is_upperleft(from, to,user) + is_lowerright(from, to,user) + is_lowerleft(from, to,user)) == 1 ? true : false;;
}
//金の動き(成でよく使用)
bool is_kin_move(position from,position to,ID user) {
	//上下左右,右上,左上
	return is_UpDownLeftRight(from, to, user) || is_upperright(from, to, user) || is_upperleft(from, to, user);
}
/*ALL*/
//水平
bool is_horizontal(position from,position to) {
	return (from.y == to.y && !is_hold(from,to));
}
//垂直
bool is_vertical(position from,position to) {
	return (from.x == to.x && !is_hold(from, to));
}
//上に動く場合(ユーザは考慮しない)
bool is_true_move_upall(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;//動きが合っているか
	for (MINI i = (from.y - 1); i > to.y; i--)
		if (boardp[pos_to_arraynum({ from.x,i })]->piece != piece_info::NONE) return false;
	return true;
}
//下に動く場合(ユーザは考慮しない)
bool is_true_move_downall(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;//動きが合っているか
	for (MINI i = (from.y + 1); i < to.y; i++)
		if (boardp[pos_to_arraynum({ from.x,i })]->piece != piece_info::NONE) return false;
	return true;
}
//左に動く場合(ユーザは考慮しない)
bool is_true_move_leftall(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;//動きが合っているか
	for (MINI i = (from.x - 1); i > to.x; i--)
		if (boardp[pos_to_arraynum({ i,from.y })]->piece != piece_info::NONE) return false;
	return true;
}
//右に動く場合(ユーザは考慮しない)
bool is_true_move_rightall(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;//動きが合っているか
	for (MINI i = (from.x + 1); i < to.x; i++)
		if (boardp[pos_to_arraynum({ from.x,i })]->piece != piece_info::NONE) return false;
	return true;
}
//斜めALL
bool is_diagonal_all(position from,position to) {
	//変化量を測定
	position range_change = {from.x-to.x,from.y-to.y};
	//絶対値へ変換
	range_change.x *= range_change.x < 0 ? -1 : 1;
	range_change.y *= range_change.y < 0 ? -1 : 1;
	//変化量が同じ場合斜め方向
	return range_change.x == range_change.y ? true : false;
}
//桂馬の動き
bool is_keima_move(position from,position to,ID user) {
	if (user==user2) {
		position bufr = {from.x+1,from.y-2};
		position bufl = {from.x-1,from.y-2};
		return is_equal(bufr, to) || is_equal(bufl,to);
	}
	else if (user==user1) {
		position bufr = { from.x - 1,from.y + 2 };
		position bufl = { from.x + 1,from.y + 2 };
		return is_equal(bufr, to) || is_equal(bufl, to);
	}
	return false;
}
#endif
