#ifndef BOARD_H
#define BOARD_H

/*��{�I�ȓ���������*/

/*INCLUDE*/
//user_have�Ŏg�p����
#include <vector>

#include "base.h"
#include "piece.h"
#include "SanaeUtil.h"


//set_console_color�Ȃǂ��g������
using namespace sanae::util;


//��̖��O�Ə��L�҂��i�[
typedef struct {
	piece_info piece;
	ID         user;
}basic_info;


//�G���[�o�� 0~2 {0:�� 1:���F 2:��}
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


//�g���Ă��Ȃ�NONE��T���Ĕz��ԍ���Ԃ��܂�
UINT find_unused_none(NONE* data) {
	for (UINT i = 0; i < NUM_ARRAY;i++)
		if (!data[i].is_used)return i;
	return 0;
}


/*board*/
class board {
protected:

	//�{�[�h�{�� *�|�C���^���i�[
	piece_base* mainboard[NUM_ARRAY];

private:


	/*��ׂ�*/
	NONE	air[NUM_ARRAY];
	KING    king   [2] = {user1,user2};
	HISYA   hisya  [2] = {user1,user2};
	KAKU    kaku   [2] = {user1,user2 };
	KIN     kin    [4] = {user1,user1,user2,user2};
	GIN     gin    [4] = {user1,user1,user2,user2};
	KEIMA   keima  [4] = {user1,user1,user2,user2};
	KYOUSYA kyousya[4] = {user1,user1,user2,user2};
	HUHYOU  huhyou[18];

	//���[�U�̎�����
	std::vector<piece_base*> user1_have;
	std::vector<piece_base*> user2_have;
	

	//�����z�u
	void abs_set(position pos, piece_base* data) {
		if (mainboard[pos_to_arraynum(pos)]->piece == piece_info::NONE) {
			mainboard[pos_to_arraynum(pos)]->is_used = false;
		}
		mainboard[pos_to_arraynum(pos)] = data;
	}


	//�����������Ă��邩���m�F����(��{�̂�is_true_move�Ɠ���ȃA���S���Y���Ŋm�F)&�ʏ����Ƃ�ꂽ������
	bool is_true_move(piece_base* data,position from,position to,ID user) {
		if (mainboard[pos_to_arraynum(from)]->user_id!=user) {
			errout("���Ȃ��̋�ł͂���܂���B\n",0);
			return false;
		}
		//�u����ɉ��������ꍇ
		if (mainboard[pos_to_arraynum(to)]->piece != piece_info::NONE) {
			//�����̋�̏ꏊ�ɒu�����Ƃ͂ł��Ȃ�
			if (mainboard[pos_to_arraynum(to)]->user_id == data->user_id) {
				errout("�����ɒu�����Ƃ͂ł��܂���B\n",0);
				return false;
			}
		}
		//�����������������ꍇ
		if (!data->is_true_move(from, to)) {
			errout("���������Ⴂ�܂��B\n",0);
			return false;
		}
		//���s����
		if (mainboard[pos_to_arraynum(to)]->piece == piece_info::KING) {
			if (user==user1) {
				user2_lose = true;
			}
			else {
				user1_lose = true;
			}
			return true;
		}
		/*���,�p,���Ԃ̓����������Ă��邩*/
		//���,�p,���Ԃ̂ǂꂩ����Ȃ��ꍇtrue
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

	//���[�U�̏��s�t���O(board::is_true_move�ɂ���Ĕ���)
	bool user1_lose = false;
	bool user2_lose = false;


	//�w�肵����̏��L�҂Ƌ�̖��O��Ԃ��܂�
	basic_info get_basic_info(position pos) {
		return { mainboard[pos_to_arraynum(pos)]->piece,mainboard[pos_to_arraynum(pos)]->user_id };
	}


	//��̏������ׂČ��ɖ߂�
	inline void clean() {
		command_cls();
		//�{�[�h���܂�����ɂ���
		for (UINT i = 0; i < NUM_ARRAY; i++) {
			mainboard[i]  = &air[i];
		}
		//�����̎������������user1:user2
		for (UINT i = 0; i < 18; i++) {
			if (i < 9) {
				huhyou[i] = user1;
			}
			else if (i < 18) {
				huhyou[i] = user2;
			}
		}
		//�ʏ�,���,�p�̎������ʏ�֖߂�
		for (UINT i = 0; i < 2; i++) {
			ID user  = i == 0 ? user1 : user2;
			king[i]  = user;
			hisya[i] = user;
			kaku[i]  = user;
		}
		//��,��,�j�n,���Ԃ̎������ʏ�֖߂�
		for (UINT i = 0; i < 4; i++) {
			ID user    = i < 2 ? user1 : user2;
			kin[i]     = user;
			gin[i]     = user;
			keima[i]   = user;
			kyousya[i] = user;
		}
	}


	//�R���X�g���N�^
	board() {
		clean();
		set_basic_position();
	}

	//�R���X�g���N�^:testmode�̈�����true�̏ꍇclean�݂̂��s��
	board(bool testmode) {
		if (testmode) {
			clean();
		} else {
			clean();
			set_basic_position();
		}
	}


	//��{�̃|�W�V�����ɂ���
	inline void set_basic_position() {
		//�������Z�b�g
		for (MINI i = 0; i < 9; i++)
			abs_set({ i,2 }, &huhyou[i]);
		for (MINI i = 9; i < 18; i++)
			abs_set({ i,5 }, &huhyou[i]);
		//����
		abs_set({ 0,0 }, &kyousya[0]);
		abs_set({ 8,0 }, &kyousya[1]);
		abs_set({ 0,8 }, &kyousya[2]);
		abs_set({ 8,8 }, &kyousya[3]);
		//�j�n
		abs_set({ 1,0 }, &keima[0]);
		abs_set({ 7,0 }, &keima[1]);
		abs_set({ 1,8 }, &keima[2]);
		abs_set({ 7,8 }, &keima[3]);
		//��
		abs_set({ 2,0 }, &gin[0]);
		abs_set({ 6,0 }, &gin[1]);
		abs_set({ 2,8 }, &gin[2]);
		abs_set({ 6,8 }, &gin[3]);
		//��
		abs_set({ 3,0 }, &kin[0]);
		abs_set({ 5,0 }, &kin[1]);
		abs_set({ 3,8 }, &kin[2]);
		abs_set({ 5,8 }, &kin[3]);
		//��
		abs_set({ 4,0 }, &king[0]);
		abs_set({ 4,8 }, &king[1]);
		//�p
		abs_set({ 1,1 }, &hisya[0]);
		abs_set({ 7,1 }, &kaku[0]);
		abs_set({ 1,7 }, &kaku[1]);
		abs_set({ 7,7 }, &hisya[1]);
	}


	//��ʃN���A������(�R�}���h)
	void command_cls() {
		system("cls");
	}


	//�{�[�h��\������
	void show(ID user, position lightup = {-1,-1}) {
		printf(" �O|�P|�Q|�R|�S|�T|�U|�V|�W|X��\n");
		bool is_normal = lightup.x < 0 && lightup.y < 0;
		for (UINT i = 0; i < NUM_ARRAY;i++) {
			if (i == 0)printf("0");
			if (i % 9 == 0 && i!=0)printf("\n%u",i/9);
			//���삷�郆�[�U�ƕ\�����������ꍇ�w�i�����F�ɂ���Blightup�̏ꏊ�̔w�i��΂ɂ���B
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
				//�����Ă���ꍇ�\�����@��ς���
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


	//������m�F&set����
	bool have_pieces_set(ID user) {
		std::vector<piece_base*>* user_have = user == user1 ? &user1_have : &user2_have;
		printf("���Ȃ��̎�����\n");
		for (UINT i = 0; i < user_have->size(); i++) {
			printf("%u:", i);
			(*user_have)[i]->show();
			printf("\n");
		}
		printf("�w�����I�����Ă��������B\n");
		UINT num = 0;
		scanf_s("%u", &num);
		if (user_have->size() <= num) {
			command_cls();
			errout("�w�肵���i���o�[�͂���܂���\n", 2);
			return false;
		}
		printf("�z�u�����͂��Ă��������B\n");
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
			errout("�����ɒu�����Ƃ͂ł��܂���B", 2);
			return false;
		}
		//�w��̋���Z�b�g
		abs_set({ x,y }, (*user_have)[num]);
		//�������폜
		(*user_have).erase((*user_have).begin() + num);
		command_cls();
		errout("�������܂����B", 0);
		return true;
	}


	//���[�U�̑���𔽉f&����
	bool userset(position from,position to,ID user) {
		//���삵�悤�Ƃ��Ă��������̂��̂�
		if (mainboard[pos_to_arraynum(from)]->user_id != user) {
			errout("�����̋�̂ݓ��������Ƃ��ł��܂��B\n",1);
			return false;
		}
		//������������������
		if (!is_true_move(mainboard[pos_to_arraynum(from)], from, to, user)) { 
			return false;
		}
		//�{�[�_�[���C���ɓ������ꍇ
		if (is_intoborderline(user,to)) {
			mainboard[pos_to_arraynum(from)]->question();
		}
		//����
		piece_base* buf = mainboard[pos_to_arraynum(to)];
		mainboard[pos_to_arraynum(to)] = mainboard[pos_to_arraynum(from)];
		if (buf->piece == piece_info::NONE) {
			mainboard[pos_to_arraynum(from)] = buf;
		}
		else {
			mainboard[pos_to_arraynum(from)] = &air[find_unused_none(air)];
			std::vector<piece_base*>* user_have = user == user1 ? &user1_have : &user2_have;
			buf->user_id     = user;
			//���肵�����������
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


	/*�e�X�g���[�h�Ŏg����*/
	bool set_test(piece_info name,position setpos,ID user) {
		//�����Ă���ꍇ
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