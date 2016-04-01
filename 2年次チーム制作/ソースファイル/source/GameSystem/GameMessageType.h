#ifndef __GAME_MESSAGE_TYPE_H__
#define __GAME_MESSAGE_TYPE_H__

//**********************************************************
//	�Q�[�����Ŏg���郁�b�Z�[�W���
//**********************************************************

namespace msg_t
{
	enum _ENUM_MSG_TYPE	//�������������typedef���Ă���ق����g���Ăق����c
	{
		error_msg,	//�G���[�^�C�v

		//�������b�Z�[�W
		env_sound,
		memory_update,
		//�L�����N�^�X�e�[�g�e�[�u���ɑ΂��ăX�e�[�g��ύX���郁�b�Z�[�W(exinfo�Ƃ���CHARACTER_STATE_TYPE�^�̃|�C���^��ݒ肷��ƒ��ŕύX����)
		chr_table_state_change,
		//�L�����N�^�X�e�[�g�e�[�u���ɑ΂��ĕύX�\�ł���΃X�e�[�g��ύX���郁�b�Z�[�W
		chr_table_state_change_low_lvl,
		//�L�����N�^�̌��ݎ��s���Ă���X�e�[�g�ɑ΂��郁�b�Z�[�W(exinfo�Ƃ���CharacterStateMessage�̃|�C���^��ݒ肷��ƒ��ŏ�������)
		to_chr_state,
		//�L�����N�^�ɑ΂��A���Ԃւ̃��b�Z�[�W�A�C�R����\�����Ăق����Ƃ��ɂ����郁�b�Z�[�W(exinfo�Ƃ���FriendMessageIcon::MsgType�^�̃|�C���^��ݒ肷��ƒ��ŏ�������)
		chr_friend_msg_icon,
		//�����S���̔]�ɑ΂��āA���b�Z�[�W�A�C�R����\�������Ƃ��ɑ��郁�b�Z�[�W(exinfo�Ƃ���FriendIconMsgInfo�̃|�C���^�𑗂�)
		apper_friend_msg_icon,
		//�U����������������character�ɑ΂��đ��郁�b�Z�[�W
		hit_my_attack,
		//�L�����N�^�̔\�͒l��UP������(exinfo�Ƃ��Ēl(float)�𑗂�)
		up_character_move_speed,
		up_character_charge_speed,
		//�o���b�g�}�l�[�W���ɑ΂��郁�b�Z�[�W(exinfo�Ƃ���BulletBase�^���p�������N���X�|�C���^��ݒ肷��ƒe���X�g�ɒǉ�����)
		add_bullet,

		//MapObjectInterface�ɑ΂��āA��������̏Ռ��𑗂郁�b�Z�[�W(exinfo�Ƃ���ControllObj�^�̃|�C���^�𑗂�)
		ride_obj,
		
		//�G��|�������̃X�R�A���Z���b�Z�[�W(exinfo�Ƃ���AddScoreMsgData�𑗂�)
		add_score_msg,
		//�X�R�A�`�惁�b�Z�[�W(exinfo�Ƃ���TEAM_TYPE�𑗂�)
		render_score_msg,
		//�A�C�R���ǉ����b�Z�[�W
		send_screen_icon_msg,
		//�e���b�v�}�l�[�W���ɑ΂��A�e���b�v�ǉ����b�Z�[�W(exinfo�Ƃ���NewsTelopBase�^�𑗂�)
		send_news_telop,

		//�A�C�R���`�惁�b�Z�[�W(exinfo�Ƃ���RenderOwnerMsgExinfo�𑗂�)
		news_telop_render_msg,

		//�A�C�R���`�惁�b�Z�[�W(exinfo�Ƃ���RenderOwnerMsgExinfo�𑗂�)
		icon_render_msg,

		//���ԕ`�惁�b�Z�[�W(exinfo�Ƃ���TimerRenderMsgInfo�𑗂�)
		timer_render_msg,

		//SE�Đ����b�Z�[�W(exinfo�Ƃ���cod_sound::PlaySEParam�𑗂�)
		play_se_msg,

		//SE�Đ����b�Z�[�W(exinfo�Ƃ���bgm_type�𑗂�)
		play_bgm_msg,

		//�v�����j���O
		replanning,

		//�]�ɑ΂��ă_���[�W���󂯂��Ƃ��ɑ��郁�b�Z�[�W(exinfo�Ƃ���DamageParam�^�𑗂�)
		damage_hit,
	};
}

typedef msg_t::_ENUM_MSG_TYPE		MSG_TYPE;	//�Q�[�������b�Z�[�W�^�C�v



#endif