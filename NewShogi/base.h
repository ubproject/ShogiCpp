#ifndef BASE_H
#define BASE_H

/*���N���X�Ȃǂ̃w�b�_*/

/*INCLUDE*/
#include <stdio.h>
#include <wchar.h>


//x,y�Ȃǃ|�W�V������\���p
typedef char MINI;
//�l�����Z�ȂǑ��p�r
typedef unsigned int UINT;
//���[�h��l�Ȃǂ����ʂ���p
typedef unsigned char ID;


/*�{�[�h�̏��*/
//����
#define WIDTH  9
//����
#define HEIGHT 9
//�z��̗v�f��
#define NUM_ARRAY WIDTH*HEIGHT


/*���[�U*/
//NONE:���L�҂Ȃ�(��)
constexpr ID air_user = 0;
//user1:�㕔
constexpr ID user1 = 1;
//user2:����
constexpr ID user2 = 2;


/*position�֘A*/
//X,Y���i�[ 0~8
typedef struct {
	MINI x;
	MINI y;
}position;

//position���m���ׂ�
bool is_equal(position d1, position d2) {
	return d1.x == d2.x && d1.y == d2.y;
}

//���邱�Ƃ̂ł���{�[�_���C��
constexpr MINI BORDERY_USER1 = 2;
constexpr MINI BORDERY_USER2 = 6;

//position��array�֒���
UINT pos_to_arraynum(position pos) { return (pos.y * WIDTH + pos.x); }
//array�ԍ���position�֒���
position arraynum_to_pos(UINT num) { return { (MINI)(num % WIDTH),(MINI)(num / WIDTH) }; }

//����̃{�[�_�[���C���֐N��������
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


//��R�[�h���i�[
enum struct piece_info:UINT{
	NONE  = 000,//��
	KING  = 100,//�ʏ�
	HISYA = 101,//���
	KAKU  = 102,//�p�s
	KIN   = 103,//��
	GIN   = 104,//��
	KEIMA = 105,//�j�n
	KYOUSYA = 106,//����
	HUHYOU  = 107,//����
	//����:200�`
	RYUOU    = 201,//����
	RYOUMA   = 202,//���n
	NARIGIN  = 204,//����
	NARIKEI  = 205,//���j
	NARIKYOU = 206,//����
	TOKIN    = 207	//�Ƌ�
};


/*��̊��N���X*/
class piece_base {
protected:

	//�R�s�[�R���X�g���N�^�̏����p
	inline void copy_piece_info(const piece_base& pieceg) {
		request_nari = pieceg.request_nari;
		is_nari = pieceg.is_nari;
		piece = pieceg.piece;
		user_id = pieceg.user_id;
	}

public:

	/*�ϐ�*/
	//��R�[�h���i�[
	piece_info piece  = piece_info::NONE;
	//���[�U�ɐ��邩�Ȃ�Ȃ���������
	bool request_nari = true;
	//���L�҂��i�[
	ID   user_id      = air_user;
	//�����Ă��邩�ǂ���
	bool is_nari      = false;

	//NONE��p(�ʏ��͎g�p���Ȃ�)
	bool is_used      = false;


	//�R���X�g���N�^
	piece_base() {
		//�����l
		piece        = piece_info::NONE;
		request_nari = true;
		user_id = air_user;
		is_nari = false;
		is_used = false;
	}
	
	//��O���� over:true  notover:false
	bool is_over(position pos) {
		return pos.x < WIDTH || pos.y < HEIGHT ? false:true;
	}

	//������������������(�I�[�o�[���C�h����)
	virtual bool is_true_move(position from, position to) {
		return false;
	}

	//���\��(�I�[�o�[���C�h����)
	virtual void show() {}


	//���邩�Ȃ�Ȃ�������(�I�[�o�[���C�h���Ȃ�)
	virtual void question() final{
		if (request_nari && !is_nari) {
			printf("\n����܂����H\nYes:Y�L�[ No:���̑��L�[\n");
			if (_getwch()=='y') {
				is_nari = true;
				UINT buf = (UINT)piece;
				//100�𑫂��Ɛ����ɂȂ�
				buf += 100;
				piece = (piece_info)buf;
			}else {
				is_nari = false;
			}
			//��x���₵����I��
			request_nari = false;
		}
		return;
	}
};

#endif