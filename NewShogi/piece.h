#ifndef PIECE_H
#define PIECE_H

/*���ׂĂ̋���`*/

/*INCLUDE*/
#include "move.h"
#include "base.h"


/*�e��̒�`*/
//��:���L�҂Ȃ�
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
		printf("�@");
	}
};


//�ʏ�
class KING : public piece_base {
public:
	KING(ID user):piece_base() {
		//�ʏ��͐���Ȃ�
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
		//�㉺���E,�E��,����,�E��,����
		return (is_UpDownLeftRight(from,to,user_id)||is_UpperLower(from,to,user_id));
	}
	void show() override {
		printf("��");
	}
};


//���
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
		//�͈͂𒴂��Ă��Ȃ���
		if (is_over(to))return false;
		//�����Ă���ꍇ
		if(is_nari){
			//����(�S),����(�S),�΂߂Ɉړ��\(�����Ă���)
			return (is_horizontal(from, to) || is_vertical(from, to) || is_UpperLower(from,to,user_id));
		}else {
			//����(�S),����(�S) (�����Ă��Ȃ�)
			return (is_horizontal(from, to) || is_vertical(from, to));
		}
	}
	void show() override {
		if (is_nari) {
			printf("��");
		}
		else {
			printf("��");
		}
	}
};


//�p
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
		//�͈͂𒴂��Ă��Ȃ���
		if (is_over(to))return false;
		//�����Ă���ꍇ
		if (is_nari) {
			//�΂�(�S)�Ə㉺���E�Ɉړ��\(�����Ă���)
			return is_diagonal_all(from, to)||is_UpDownLeftRight(from,to,user_id);
		}else {
			//�΂�(�S)�Ɉړ��\(�����Ă��Ȃ�)
			return is_diagonal_all(from, to);
		}
	}
	void show() override {
		if (is_nari) {
			printf("�n");
		}
		else {
			printf("�p");
		}
	}
};


//��
class KIN : public piece_base {
public:
	KIN(ID user) :piece_base() {
		//���͐��邱�Ƃ��ł��Ȃ�
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
		//�㉺���E,�E��,����
		return is_kin_move(from,to,user_id);
	}
	void show() override {
		printf("��");
	}
};


//��
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
			//����͋��Ɠ�������
			return is_kin_move(from,to,user_id);
		}else {
			//�E��,����,�E��,����,��
			return is_UpperLower(from, to, user_id) || is_up(from, to, user_id);
		}
	}
	void show() override {
		if (is_nari) {
			printf("�S");
		}
		else {
			printf("��");
		}
	}
};


//�j�n
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
			//���j�͋��Ɠ�������
			return is_kin_move(from,to,user_id);
		}else{
			//�j�n�̓���
			return is_keima_move(from, to, user_id);
		}
	}
	void show() override {
		if (is_nari) {
			printf("�\");
		}
		else {
			printf("�j");
		}
	}
};


//����
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
			//�����͋��Ɠ�������
			return is_kin_move(from,to,user_id);
		}else {
			//���Ԃ̓���(�����ɓ���)
			return is_up_all(from, to,user_id);
		}
	}
	void show() override {
		if (is_nari) {
			printf("��");
		}
		else {
			printf("��");
		}
	}
};


//����
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
			//�Ƌ��͋��Ɠ�������
			return is_kin_move(from,to,user_id);
		}else {
			//�����̓���
			return is_up(from, to, user_id);
		}
	}
	void show() override {
		if (is_nari) {
			printf("��");
		}
		else {
			printf("��");
		}
	}
};


#endif
