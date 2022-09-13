#ifndef MOVE_H
#define MOVE_H

//position�p
#include "base.h"

/*��������*/
/*
user1�͏㕔���牺���ֈړ�
user2�͉�������㕔�ֈړ�����
is_up�Ȃǂ͂��̂��Ƃ����炩���ߑz�肵�č쐬����Ă���B
*/
//�ړ�������
bool is_hold(position from, position to) {
	return (from.x==to.x&&to.y==from.y);
}
//��
bool is_up(position from, position to,ID user) {
	return (((from.y + 1) == to.y)&&user==user1) || (((from.y - 1) == to.y) && user == user2);
}
bool is_up_all(position from,position to,ID user){
	if (from.x == to.x && (to.y - from.y) > 0 && user == user1) return true;
	if (from.x == to.x && (to.y - from.y) < 0 && user == user2) return true;
	return false;
}
//��
bool is_down(position from, position to,ID user) {
	return (((from.y + 1) == to.y) && user == user2) || (((from.y - 1) == to.y) && user == user1);
}
//��
bool is_left(position from, position to,ID user) {
	return ((from.x - 1) == to.x && user==user2)||((from.x + 1) == to.x && user == user1);
}
//�E
bool is_right(position from, position to,ID user) {
	return ((from.x + 1) == to.x && user==user2)|| ((from.x - 1) == to.x && user == user1);
}
//�㉺���E
bool is_UpDownLeftRight(position from,position to,ID user) {
	MINI count = is_up(from, to, user) + is_down(from, to, user) + is_left(from, to, user) + is_right(from, to, user);
	return count==1?true:false;
}
/*�΂�*/
//�E��
bool is_upperright(position from, position to,ID user) {
	return (is_up(from, to, user) && is_right(from, to,user));
}
//�ړ��̓r���ɋ�Ȃ���
bool is_upperright_all(piece_base** boardp, piece_base* piec, position from, position to) {
	for (MINI x = from.x + 1, y = from.y - 1; x < to.x; x++, y--) 
		if (boardp[pos_to_arraynum({ x,y })]->piece != piece_info::NONE)
			return false;
	return true;
}
//����
bool is_upperleft(position from, position to,ID user) {
	return (is_up(from, to,user) && is_left(from, to,user));
}
//�ړ��̓r���ɋ�Ȃ���
bool is_upperleft_all(piece_base** boardp, piece_base* piec, position from, position to) {
	for (MINI x = from.x - 1, y = from.y - 1; x > to.x; x--, y--) 
		if (boardp[pos_to_arraynum({ x,y })]->piece != piece_info::NONE)
			return false;
	return true;
}
//�E��
bool is_lowerright(position from, position to,ID user) {
	return (is_down(from, to,user) && is_right(from, to,user));
}
//�ړ��̓r���ɋ�Ȃ���
bool is_lowerright_all(piece_base** boardp, piece_base* piec, position from, position to) {
	for (MINI x = from.x + 1, y = from.y + 1; x < to.x; x++, y++)
		if (boardp[pos_to_arraynum({ x,y })]->piece != piece_info::NONE)
			return false;
	return true;
}
//����
bool is_lowerleft(position from, position to,ID user) {
	return (is_down(from, to,user) && is_left(from, to,user));
}
//�ړ��̓r���ɋ�Ȃ���
bool is_lowerleft_all(piece_base** boardp, piece_base* piec, position from, position to) {
	for (MINI x = from.x - 1, y = from.y + 1; x > to.x; x--, y++) {
		if (boardp[pos_to_arraynum({ x,y })]->piece != piece_info::NONE)
			return false;
	}
	return true;
}
//�ړ��̓r���ɋ�Ȃ����𒲂ׂ�(������g������)
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
//�E�㍶��E������
bool is_UpperLower(position from,position to,ID user) {
	MINI count = 0;
	return (count = is_upperright(from, to,user) + is_upperleft(from, to,user) + is_lowerright(from, to,user) + is_lowerleft(from, to,user)) == 1 ? true : false;;
}
//���̓���(���ł悭�g�p)
bool is_kin_move(position from,position to,ID user) {
	//�㉺���E,�E��,����
	return is_UpDownLeftRight(from, to, user) || is_upperright(from, to, user) || is_upperleft(from, to, user);
}
/*ALL*/
//����
bool is_horizontal(position from,position to) {
	return (from.y == to.y && !is_hold(from,to));
}
//����
bool is_vertical(position from,position to) {
	return (from.x == to.x && !is_hold(from, to));
}
//��ɓ����ꍇ(���[�U�͍l�����Ȃ�)
bool is_true_move_upall(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;//�����������Ă��邩
	for (MINI i = (from.y - 1); i > to.y; i--)
		if (boardp[pos_to_arraynum({ from.x,i })]->piece != piece_info::NONE) return false;
	return true;
}
//���ɓ����ꍇ(���[�U�͍l�����Ȃ�)
bool is_true_move_downall(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;//�����������Ă��邩
	for (MINI i = (from.y + 1); i < to.y; i++)
		if (boardp[pos_to_arraynum({ from.x,i })]->piece != piece_info::NONE) return false;
	return true;
}
//���ɓ����ꍇ(���[�U�͍l�����Ȃ�)
bool is_true_move_leftall(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;//�����������Ă��邩
	for (MINI i = (from.x - 1); i > to.x; i--)
		if (boardp[pos_to_arraynum({ i,from.y })]->piece != piece_info::NONE) return false;
	return true;
}
//�E�ɓ����ꍇ(���[�U�͍l�����Ȃ�)
bool is_true_move_rightall(piece_base** boardp, piece_base* piec, position from, position to) {
	if (!piec->is_true_move(from, to))return false;//�����������Ă��邩
	for (MINI i = (from.x + 1); i < to.x; i++)
		if (boardp[pos_to_arraynum({ from.x,i })]->piece != piece_info::NONE) return false;
	return true;
}
//�΂�ALL
bool is_diagonal_all(position from,position to) {
	//�ω��ʂ𑪒�
	position range_change = {from.x-to.x,from.y-to.y};
	//��Βl�֕ϊ�
	range_change.x *= range_change.x < 0 ? -1 : 1;
	range_change.y *= range_change.y < 0 ? -1 : 1;
	//�ω��ʂ������ꍇ�΂ߕ���
	return range_change.x == range_change.y ? true : false;
}
//�j�n�̓���
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
